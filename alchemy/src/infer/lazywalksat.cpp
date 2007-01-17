/* This code is based on the MaxWalkSat package of Kautz et al. */
#include "lazywalksat.h"
#include "lwutil.h"
#include "lwinfo.h"
#include "timer.h"

LazyWalksat::LazyWalksat(LWInfo *lwInfo, int memLimit)
{
  clauseStorePtr = NULL;
  clauseFreeStore = 0;
   
  atomStorePtr = NULL;
  atomFreeStore = 0;
   
  atomFSMgr = new FreeStoreManager(BLOCKSIZE);
  clauseFSMgr = new FreeStoreManager(BLOCKSIZE);
   
  costexpected = true;   
  hard = 0;
  noApprox = false;

  this->lwInfo = lwInfo;
  this->memLimit = memLimit;
  clauseLimit = INT_MAX;
  haveDeactivated = false;
  
  numatom = 0;
  basenumatom = 0;
  numclause = 0;
  numliterals = 0;
   
  //currently allocated memory (in terms of number of items
  //that can be stored). This is a pointer and hence no memory has
  // been allocated yet
  clausemem = 0;
  atommem = 0;

  //fixedAtoms_.growToSize(lwInfo->getNumDBAtoms() + 1, false);
  maxFixedAtoms = lwInfo->getNumDBAtoms() + 1;
  fixedAtoms_ = new bool[maxFixedAtoms];
  for (int i = 0; i < maxFixedAtoms; i++)
    fixedAtoms_[i] = false;
  
}

LazyWalksat::~LazyWalksat()
{
  clauseFSMgr->releaseAllStorePtr();
  atomFSMgr->releaseAllStorePtr();
 
  deleteClauseMemory();
  deleteAtomMemory();
		  
  delete clauseFSMgr;
  delete atomFSMgr;
  
  for(int i = 0; i < newClauses.size(); i++)
    delete newClauses[i];
  newClauses.clear();
  newClauseWeights.clear();

  for(int i = 0; i < allClauses.size(); i++)
    delete allClauses[i];
  allClauses.clear();
  allClauseWeights.clear();
  
  for(int i = 0; i < supersetClauses_.size(); i++)
  {
    supersetClauses_[i]->deleteIntPredicates();
    delete supersetClauses_[i];
  }
  supersetClauses_.clear();

  delete [] fixedAtoms_;
}

/* Retrieves initial active atoms and randomizes them*/
void LazyWalksat::randomizeActiveAtoms()
{
  lwInfo->getWalksatClauses(newClauses, newClauseWeights);
  for(int i = 0; i < newClauses.size(); i++)
    delete newClauses[i];

  newClauses.clear();
  newClauseWeights.clear();

  for(int i = 1; i < lwInfo->getVarCount() + 1; i++)
  {
    lwInfo->setVarVal(i, random()%2);
  }
}

/* Lazy version of SampleSat */
bool LazyWalksat::sample(const MaxWalksatParams* const & mwsParams,
						 const SampleSatParams& sampleSatParams,
                         const bool& initial)
{
	// -- samplesat params
  saRatio = sampleSatParams.saRatio;
  temperature = sampleSatParams.temperature;
  latesa = sampleSatParams.latesa;
  numerator = sampleSatParams.ws_noise;
  
  int numsol = sampleSatParams.numsol;

	//Run LazyWalkSAT
  bool includeUnsatSolutions = false;
  Array<Array<bool>*> solutions;
  Array<int> numBad;
  infer(mwsParams, numsol, includeUnsatSolutions,
  		solutions, numBad, initial);

  for(int i = 0; i < newClauses.size(); i++)
    delete newClauses[i];
  newClauses.clear();
  newClauseWeights.clear();

  for(int i = 0; i < allClauses.size(); i++)
    delete allClauses[i];
  allClauses.clear();
  allClauseWeights.clear();

  int solutionsSize = solutions.size();  
  	// Delete solutions array
  for (int i = 0; i < solutions.size(); i++)  
    if (solutions[i]) delete solutions[i];
  solutions.clearAndCompress();
  
  if (solutionsSize >= 1)
	return true;
  else
	return false;
}


/* Lazy version of MaxWalkSat */
void LazyWalksat::infer(const MaxWalksatParams* const & params,
 						const int& numSolutions, const bool&  includeUnsatSolutions,
        		        Array<Array<bool>*>& solutions, Array<int>& numBad,
        		        const bool& initial)
{
//cout << "Inside lazywalksat::infer" << endl;
  int base_cutoff = 100000;
  bool printonlysol = false;
  bool printfalse = false;
  bool printlow = true;
  int printtrace = 1000;
  //int printtrace = 0;
  long int totalflip = 0;	/* total number of flips in all tries so far */
  int numtry = 0;		/* total attempts at solutions */
  int numsuccesstry = 0;	/* total found solutions */
  int numsol = numSolutions;     /* stop after this many tries succeeds */
  int seed;			/* seed for random */
  struct timeval tv;
  struct timezone tzp;
  double setuptime;
  double expertime;
  double totaltime;
    
  long int lowbad;		/* lowest number of bad clauses during try */
  long int lowcost;		/* lowest cost of bad clauses during try */
  double seconds_per_flip;

  int worst_cost, computed_cost;

  gettimeofday(&tv,&tzp);
  seed = (( tv.tv_sec & 0177 ) * 1000000) + tv.tv_usec;
  srandom(seed);

  heuristic = BEST;
  numerator = NOVALUE;
  denominator = 100;

  int targetcost = params->targetWt;	/* the cost at which the program terminates*/
  if (targetcost == INT_MAX) targetcost = 0;
  if (params->hard) hard = true;

  int cutoff = params->maxSteps;
  int numrun = params->tries;

    // No. of runs must be at least as big as no. of solutions
  if (numrun < numsol) numrun = numsol;
  if (params->lazyNoApprox) noApprox = true;
  
  base_cutoff = cutoff;
  if (numerator == NOVALUE)
  {
  	if (heuristic == BEST)
	  numerator = 50;
	else
	  numerator = 0;
  }

	// Make an approximation of how many clauses memory can hold
  if (memLimit > 0)
  {
  	  // Size of a clause (1 2-dim array and 5 1-dim arrays of ints)
  	  // and average one atom appears in 5 clauses
  	  // Size of an atom (1 2-dim array and 8 1-dim arrays of ints)
  	int atomSize = 2*sizeof(int)*sizeof(int) + 11*sizeof(int);
  	int clauseSize = (2 * ((sizeof(int)*sizeof(int) + 5*sizeof(int))
  					  + atomSize / 5));
	  // About 50% of the memory is available (other 50% needed for DB, etc.)
  	clauseLimit = ((memLimit/2) / clauseSize) * 1024;
  	//cout << "Max. no. of clauses memory can hold: " << clauseLimit << endl;
  }

  setuptime = 0;
  expertime = 0;
  elapsed_seconds();
  
  if (initial)
  {
    init();
  }
  else if (lwInfo->getSampleSat())
  {
    newClauses.clear();
    newClauseWeights.clear();
/*
for (int i = 0; i < supersetClauses_.size(); i++)
{
  cout << "Superset " ;
  supersetClauses_[i]->print(cout);
  cout << endl;
}
*/
      // Deactivate atoms activated in last MC-SAT step
    resetSampleSat();
      // Do MC-SAT clause selection
    lwInfo->selectClauses(supersetClauses_, newClauses,
                          newClauseWeights);
      // Unit propagation on active clauses
    initFixedAtoms(newClauses, newClauseWeights);
    lwInfo->propagateFixedAtoms(newClauses, newClauseWeights,
                                fixedAtoms_, maxFixedAtoms);
    
    clauseFSMgr->releaseAllStorePtr();
    atomFSMgr->releaseAllStorePtr();
 
    deleteClauseMemory();
    deleteAtomMemory();
          
    delete clauseFSMgr;
    delete atomFSMgr;

    clauseStorePtr = NULL;
    clauseFreeStore = 0;
   
    atomStorePtr = NULL;
    atomFreeStore = 0;
   
    atomFSMgr = new FreeStoreManager(BLOCKSIZE);
    clauseFSMgr = new FreeStoreManager(BLOCKSIZE);
   
    numatom = 0;
    numclause = 0;
    numliterals = 0;
    numfalse = 0;
    costofnumfalse = 0;
  
      //adding the clauses
    bool initial = true;
    addNewClauses(initial);
  }
  
  setuptime =  elapsed_seconds();
	
  abort_flag = false;
  numnullflip = 0;
  lowcost = BIG;

  
  for (int k = 0; k < numrun; k++)
  {
	initrun();
	lowbad = numfalse; 
	lowcost = costofnumfalse;
	save_low_assign();
	numflip = 0;
        
    //if (superlinear) cutoff = base_cutoff * super(r+1);

    if (printtrace)
    {
      cout<<"in the beginning, costofnumfalse = "<<costofnumfalse<<endl; 
      cout<<"cutoff = "<<cutoff<<endl;
      cout<<"numflip = "<<numflip<<endl;
      cout<<"numfalse = "<<numfalse<<endl;
      cout<<"numclause = "<<numclause<<endl;
      cout<<"targetcost = "<<targetcost<<endl;
      cout<<"hard = "<<hard<<endl;
            
      cout<<"costofnumfalse \t  numfalse \t numflip \t numclause"<<endl;
    }

	while((numflip < cutoff) /* && (costofnumfalse > targetcost) */)
    {
      if (numclause == 0) break;
	  if (printtrace && (numflip % printtrace == 0))
	  {
		printf("%10i \t %10i \t %10li \t %10i\n",
			   costofnumfalse, numfalse, numflip, numclause);
		fflush(stdout);
	  }
	
	  numflip++;

		//If memory is getting full, then deactivate some atoms
	  if (numclause > clauseLimit)
	  {
	  	trimClauses();
	  	initrun();
		lowbad = numfalse; 
		lowcost = costofnumfalse;
		save_low_assign();
	  	continue;
	  }

        // If samplesat, then try sim. annealing first
      if (!lwInfo->getSampleSat() || !simAnnealing())
      {
        if (hard && eqhighest && costexpected)
        {
		  fix(selecthigh(1 + random()%numhighest));
        }
        else
        {
	   	  fix(falseclause[random()%numfalse]);
        }
      }

	  if (costofnumfalse <= lowcost)
	  {
		lowcost = costofnumfalse;
		lowbad = numfalse;
		save_low_assign();
	  }
      
      if (costofnumfalse <= targetcost)
        break;
	}
	  
	numtry++;
	totalflip += numflip;
	if(costofnumfalse <= targetcost)
	{
	  numsuccesstry++;
	}

	countlowunsatcost(&computed_cost, &worst_cost);
	  
	  // Fill the solutions array
	if (printlow && (!printonlysol || costofnumfalse <= targetcost))
	{
      if (includeUnsatSolutions || lowbad == 0) 
      {
        Array<bool>* solution = new Array<bool>(numatom);
        for (int i = 0; i < numatom; i++) solution->append(false);

		for (int i = 1; i <= numatom; i++)
		{
		  if (lowatom[i] == 1) (*solution)[i-1] = true;
		  else (*solution)[i-1] = false;
		}          
        solutions.append(solution);
        numBad.append(lowbad);
      }
	}
	if (numfalse > 0 && printfalse)
	  print_false_clauses_cost(lowbad);
    
	if (numsuccesstry >= numsol) break;
	if (abort_flag) break;
	fflush(stdout);
  }

    // Now, set the db values to the best values observed
  lwInfo->setVarVals(lowatom);

    // Set evidence from unit prop. back to non-evidence
  if (lwInfo->getSampleSat())
    undoFixedAtoms();
  
  expertime += elapsed_seconds();
  totaltime = setuptime + expertime;
  seconds_per_flip = expertime / totalflip;
  
  
  return;
}

int LazyWalksat::getNumInitClauses()
{
  return initClauseCount;
}

int LazyWalksat::getNumInitAtoms()
{
  return initAtomCount;
}

int LazyWalksat::getNumClauses()
{
  return numclause;
}

/**
 * Sets all active atoms not in the base set to inactive and false
 * and removes all activated clauses from memory. This method should
 * be called between steps of SampleSat.
 */
void LazyWalksat::resetSampleSat()
{ 
    // Set all subsequently activated atoms to inactive and false
  for(int atom = basenumatom + 1; atom <= numatom; atom++)
  {
    lwInfo->setVarVal(atom, false);
    lwInfo->setInactive(atom);
  }

    // Remove all activated clauses
  for(int i = allClauses.size() - 1; i >= 0; i++)
  {
    delete allClauses[i];
    allClauses.removeLastItem();
    allClauseWeights.removeLastItem();
  }
  allClauses.shrinkToSize(initClauseCount);
  numclause = 0;
}


/*******************************************************************************/
/****************************** PRIVATE FUNCTIONS ****************************/
/*******************************************************************************/


/**
  * Initialize LazySat. Initial set of active atoms is computed and stored.
  * For lazy MC-SAT: Builds the superset of active clauses, from which clause
  * selection and unit propagation will occur in each iteration.
  * For LazySat: Builds the initial set of active clauses.
  */
void LazyWalksat::init()
{
  Timer timer;
  double begSec = timer.time(); 
    //should be input by user ideally
    //highestcost=1;
  highestcost = BIG;
  
  numclause = 0;
  numatom = 0;
  numliterals = 0;
  //cout<<"inside init (lazywalksat.cpp) "<<endl;

    // If SampleSat, we want to get the superset of initial clauses
  bool sample = lwInfo->getSampleSat();
  
  for(int i = 0; i < newClauses.size(); i++)
    delete newClauses[i];
  newClauses.clear();
  newClauseWeights.clear();

  int defaultCnt = 0;
  double defaultCost = 0;
    //getting the initial set of active atoms
  if (sample)
  {
    lwInfo->getSupersetClauses(supersetClauses_);

    defaultCnt = supersetClauses_.size();
    for(int i = 0; i < defaultCnt; i++)
      defaultCost += supersetClauses_[i]->getWt();

    for(int i = 0; i < supersetClauses_.size(); i++)
    {
      supersetClauses_[i]->deleteIntPredicates();
      delete supersetClauses_[i];
    }

    supersetClauses_.clear();
  }
  else
  {
    lwInfo->getWalksatClauses(newClauses, newClauseWeights);  
    for(int i = 0; i < newClauses.size(); i++)
      delete newClauses[i];

    defaultCnt = newClauses.size();
    for(int i = 0; i < defaultCnt; i++)
      defaultCost += newClauseWeights[i];

    newClauses.clear();
    newClauseWeights.clear();
  }

  cout<<endl;
  cout<<"Default => Cost\t"<<"******\t"<<" Clause Cnt\t"<<endl;
  cout<<"           "<<defaultCost<<"\t"<<"******\t"<<defaultCnt<<"\t"<<endl<<endl;

  basenumatom = lwInfo->getVarCount();
  cout << "Number of Baseatoms = " << basenumatom << endl;

  for(int i = 1; i < basenumatom + 1; i++) 
	lwInfo->setActive(i);
  
	//getting the initial set of active clauses
  if (sample)
  {
    lwInfo->getSupersetClauses(supersetClauses_);
      // Do clause selection on superset and convert weights

cout << "Superset:" << endl;
lwInfo->printIntClauses(supersetClauses_);

    lwInfo->selectClauses(supersetClauses_, newClauses,
                          newClauseWeights);
    initFixedAtoms(newClauses, newClauseWeights);
    lwInfo->propagateFixedAtoms(newClauses, newClauseWeights,
                                fixedAtoms_, maxFixedAtoms);
    
  }
  else
    lwInfo->getWalksatClauses(newClauses, newClauseWeights);
	
	//adding the clauses
  bool initial = true;
  addNewClauses(initial);

  initAtomCount = lwInfo->getVarCount();
  initClauseCount = allClauses.size();

  cout << "Atoms in memory: " << initAtomCount << endl;
  cout << "Initial active clauses: " << initClauseCount << endl;

  cout << "time taken for init = "; Timer::printTime(cout, timer.time()-begSec);
  cout << endl;
}


/* initialize a particular run */
void LazyWalksat::initrun()
{
  //cout<<"****** inside initrun *****"<<endl;
  int newval; 
  numfalse = 0;
  costofnumfalse = 0;
  eqhighest = false;
  numhighest = 0;

    // Blocks have been initialized in LWInfo
    
  for(int i = 1; i < numatom + 1; i++)
  {
	changed[i] = -BIG;
	breakcost[i] = 0;

    if (fixedAtoms_[i])
    {
      newval = lwInfo->getVarVal(i);
    }
    else
    {
      if (i <= basenumatom && !lwInfo->inBlock(i))
        newval = random()%2;
      else
        newval = initatom[i];
    }

    if (atom[i] != newval)
    {
      lwInfo->flipVar(i);
      atom[i] = newval;
    }
  }
  initializeBreakCost(0);
  //cout<<"****** leaving initrun *****"<<endl;
  
}

/* Fixes false clause (index of clause is tofix) */
void LazyWalksat::fix(int tofix)
{
  Timer timer;
  //double begSec = timer.time();
  double interimSec;
  //cout << "Fixing clause ";
    // If neg. clause, then flip all true literals
  if (cost[tofix] < 0)
  {
    //cout << "(neg.)" << endl;
    if (numtruelit[tofix] > 0)
    {
      for (int i = 0; i < size[tofix]; i++)
      {
        int atm = abs(clause[tofix][i]);
          // true literal
        if((clause[tofix][i] > 0) == atom[atm])
        {
          //cout << "\tTrue lit. " << i << ": ";
            // If atm has been deactivated or in block with evidence
            // or fixed, then do not flip it
          if ((haveDeactivated && lwInfo->isDeactivated(atm)) ||
              (lwInfo->inBlockWithEvidence(atm)))
            continue;
        
          if(!lwInfo->isActive(atm))
          {
            interimSec = timer.time();
            
              // If atom is in a block, this is noted here in lwInfo
            lwInfo->getWalksatClausesWhenFlipped(atm, newClauses, newClauseWeights);
         
            bool initial = false;
            addNewClauses(initial);
            lwInfo->setActive(atm);
            
            //cout << "time taken for non-active = "; Timer::printTime(cout, timer.time()-interimSec);
          }
          else
          {
              // Have to check if in block and get other to flip
            if (lwInfo->inBlock(atm))
            {
              lwInfo->chooseOtherToFlip(atm, newClauses, newClauseWeights);
              bool initial = false;
              addNewClauses(initial);
            }
          }

          if (lwInfo->getInBlock())
            lwInfo->setActive(lwInfo->getOtherAtom());

          flipatom(atm);
          if (lwInfo->getInBlock())
          {
            flipatom(lwInfo->getOtherAtom());
          }          

          lwInfo->flipVar(atm);
          if (lwInfo->getInBlock())
            lwInfo->flipVar(lwInfo->getOtherAtom());

            // Set back inBlock and other atom
          lwInfo->setInBlock(false);
          lwInfo->setOtherAtom(-1);

          //cout << endl;
        }
      }
    }
      // No true lits
    else
    {
      numnullflip++;
    }
  }
    // Pos. clause: choose an atom to flip
  else
  {
    //cout << "(pos.)" << endl;
    int numbreak[MAXLENGTH];    /* number of clauses changing */
                                /* each atoms would make false */
    int i;                      /* loop counter */
    int choice;

    for(i = 0; i < size[tofix]; i++)
    {
      //cout << "\tLit. " << i << ": ";
      int atm = abs(clause[tofix][i]);
	
        // If atm has been deactivated or in block with evidence or fixed,
        // then give it a very high weight (do not want to flip)
	  if ((haveDeactivated && lwInfo->isDeactivated(atm)) ||
          (lwInfo->inBlockWithEvidence(atm))) 
	  {
        numbreak[i] = INT_MAX;
        continue;
      }
	
	  //interfacing with external world - if this atom is not active,
	  //need to get the cost of clauses which would become unsatisfied
	  //by flipping it. 
      if(!lwInfo->isActive(atm))
      {
        interimSec = timer.time(); 
        
        numbreak[i] = lwInfo->getUnSatCostWhenFlipped(atm, newClauses, newClauseWeights);
        bool initial = false;
        addNewClauses(initial);
        lwInfo->setActive(atm);
        //cout << "time taken for non-active (1) = "; Timer::printTime(cout, timer.time()-interimSec);
      }
      else
      {
        numbreak[i] = breakcost[atm];
      }
      //cout << endl;
    }
	
    switch(heuristic)
    {
      case RANDOM:
        choice = pickrandom(numbreak,size[tofix],tofix);
        break;
      case PRODUCTSUM:  
        choice = pickproductsum(numbreak,size[tofix],tofix);
        break;
      case RECIPROCAL:  
        choice = pickreciprocal(numbreak,size[tofix],tofix);
        break;
      case ADDITIVE:    
        choice = pickadditive(numbreak,size[tofix],tofix);
        break;
      case BEST:
        interimSec = timer.time();
        
        choice = pickbest(numbreak,size[tofix],tofix);
        
        //cout << "pickbest choice " << choice << endl;
        //cout << "\ttime taken for pickbest = "; Timer::printTime(cout, timer.time()-interimSec);
        //cout << endl;
        break;
      case EXPONENTIAL: 
        choice = pickexponential(numbreak,size[tofix],tofix);
        break;
      case TABU:        
        choice = picktabu(numbreak,size[tofix],tofix);
        break;
      default:          
        choice = pickbest(numbreak,size[tofix],tofix);
    }
	
    if (choice == NOVALUE)
      numnullflip++;
    else
    {
      int atm = abs(clause[tofix][choice]);

        // If chosen atom is deactivated or in block with evidence or fixed,
        // then continue to next flip (do not want to flip)
      if ((haveDeactivated && lwInfo->isDeactivated(atm)) ||
          (lwInfo->inBlockWithEvidence(atm)))
      {
        numnullflip++;
        return;
      }
    
      if(!lwInfo->isActive(atm))
      {
        interimSec = timer.time(); 
        
          // If atom is in a block, this is noted here in lwInfo
        lwInfo->getWalksatClausesWhenFlipped(atm, newClauses, newClauseWeights);
		 
        bool initial = false;
        addNewClauses(initial);
        lwInfo->setActive(atm);
        
        //cout << "\ttime taken for non-active (2) = "; Timer::printTime(cout, timer.time()-interimSec);
        //cout << endl;
      }
      else
      {
          // Have to check if in block and get other to flip
        if (lwInfo->inBlock(atm))
        {
          lwInfo->chooseOtherToFlip(atm, newClauses, newClauseWeights);
          bool initial = false;
          addNewClauses(initial);
        }
      }
      
      if (lwInfo->getInBlock())
        lwInfo->setActive(lwInfo->getOtherAtom());

      flipatom(atm);
      if (lwInfo->getInBlock())
      {
        flipatom(lwInfo->getOtherAtom());
      }
          
      lwInfo->flipVar(atm);
      if (lwInfo->getInBlock())
        lwInfo->flipVar(lwInfo->getOtherAtom());

        // Set back inBlock and other atom
      lwInfo->setInBlock(false);
      lwInfo->setOtherAtom(-1);
    }
  }
  //cout << "time taken for total flip = "; Timer::printTime(cout, timer.time()-begSec);
  //cout << endl;
}


void LazyWalksat::addNewClauses(bool initial)
{
	 //cout<<"adding new clauses.."<<endl;
  allClauses.append(newClauses);
  allClauseWeights.append(newClauseWeights);
  newClauses.clear();
  newClauseWeights.clear();
    
  int i;			/* loop counter */
  int j;			/* another loop counter */
	
  int lit;
  int litindex;

  // store the old number of clauses 
  int oldnumclause = numclause;
  int oldnumatom = numatom;

  numatom = lwInfo->getVarCount();
  numclause = allClauses.size();
	
  //allocate Memory if currently allocated memory is not sufficient.
  //We will allocate the memory to store twice the current number of clauses/atoms 
  //- so that do not have to do it again and again as any new clauses/atoms
  //appear
  /* allocate clause memory */
  if(clausemem < numclause )
  {
	//if memory is getting tight, then can not allocate 2 times
	if (2*numclause > clauseLimit)
	  allocateClauseMemory(clauseLimit);
	else
	  allocateClauseMemory(2*numclause);
  } 

  /* allocate atom memory */
  if(atommem < numatom)
  {
    allocateAtomMemory(2*numatom);
  }

  //initialize various structues for atoms
  for(int i = oldnumatom + 1; i < numatom + 1; i++)
  {
	atom[i] = lwInfo->getVarVal(i);
	initatom[i] = atom[i];
	lowatom[i] = atom[i];
	changed[i] = -BIG;
	breakcost[i] = 0;
  }
  
  for(int i = 2*oldnumatom + 1; i < 2*numatom + 1; i++)
  {
	numoccurence[i] = 0;
	newnumoccurence[i] = 0;
	occurencemem[i] = 0;
  }

  for(i = oldnumclause; i < numclause; i++)
  {
	size[i] = 0; 
	if (costexpected)
	{
	  cost[i] = allClauseWeights[i];
	}
	else
	{
        /*the default cost of a clause violation is unit 1*/
      if (allClauseWeights[i] >= 0)
        cost[i] = 1;
      else
        cost[i] = -1;
    }

	if (clauseFreeStore < MAXLENGTH)
	{
	  clauseStorePtr = clauseFSMgr->getStorePtr();
	  clauseFreeStore = clauseFSMgr->getBlockSize();
	}
	clause[i] = clauseStorePtr;
	
	Array<int> *iclause = allClauses[i];
	for (j = 0; j < iclause->size(); j++)
	{
	  size[i]++;
	  if(size[i] > MAXLENGTH)
	  {
		printf("ERROR - clause too long\n");
		exit(-1);
	  }
	  lit = (*iclause)[j]; 

	  if(lit != 0)
	  {
		*(clauseStorePtr++) = lit; /* clause[i][size[i]] = j; */
		clauseFreeStore--;
		numliterals++;
	    litindex = 2*abs(lit) - LWUtil::mySign(lit);
		newnumoccurence[litindex]++;
	  }
	}
  }
    
  //allocate memory to the literals which do not have sufficient
  //memory to store the list of all clauses they appear in 
  for(i = oldnumclause; i < numclause;i++)
  {
	for (j = 0; j <size[i]; j++)
	{
	  lit = clause[i][j]; 
	  litindex = 2*abs(lit) - LWUtil::mySign(lit);
	  int totaloccurence = newnumoccurence[litindex] + numoccurence[litindex];

	  if(occurencemem[litindex] < totaloccurence)
	  {
	    int tmpoccurencemem = 2*totaloccurence;
	    if(atomFreeStore < tmpoccurencemem)
	    {
		  atomStorePtr = atomFSMgr->getStorePtr();
		  atomFreeStore = atomFSMgr->getBlockSize();
		  //fprintf(stderr,"allocating memory...\n");
	    }
		
	  	int *tmpoccurence = atomStorePtr;
	  	atomFreeStore -= tmpoccurencemem;
	  	atomStorePtr += tmpoccurencemem;
	  	for(int k = 0; k < occurencemem[litindex]; k++)
		  tmpoccurence[k] = occurence[litindex][k];
	  	occurencemem[litindex] = tmpoccurencemem;
	  	occurence[litindex] = tmpoccurence;
		//cout<<"occurencemem["<<litindex<<"] = "<<occurencemem[litindex]<<endl;
      }
	}
  }	

  for(i = oldnumclause; i < numclause; i++)
  {
	for(j = 0; j < size[i]; j++)
	{
	  lit = clause[i][j];
	  if(lit == 0)
	    continue;
	  litindex = 2*abs(lit) - LWUtil::mySign(lit);
	  occurence[litindex][numoccurence[litindex]] = i;
	  numoccurence[litindex]++;
	  newnumoccurence[litindex]--;
	}
  }
  if(!initial)
    initializeBreakCost(oldnumclause);
//cout<<"done adding new clauses.."<<endl;
}

// Deactivate atoms/clauses when memory is tight
void LazyWalksat::trimClauses()
{
  Array<int> idxOfDeactivated;
  if (!haveDeactivated)
  {
	haveDeactivated = true;
	  // Remove only active atoms with zero delta-cost
	for (int i = 1; i <= numatom; i++)
  	  if (lwInfo->isActive(i) && breakcost[i] == 0)
  	  	idxOfDeactivated.append(i);	  
  }
  else
  {
	int sumOfBreakCost = 0;
	  // First: cost, second: index in breakcost
  	Array<pair<int, int> > costs;
  	for (int i = 1; i <= numatom; i++)
  	{
  	  if (lwInfo->isActive(i))
  	  {
  	  	sumOfBreakCost += breakcost[i];
	  	costs.append(pair<int, int>(breakcost[i], i));
  	  }
  	}

	costs.quicksort();

  	int sumOfDeactivated = 0;
  	int numOfDeactivated = 0;
  	for (int i = 0; i < costs.size(); i++)
  	{
  	  sumOfDeactivated += costs[i].first;
  	  	// 10% of total weight
  	  if (sumOfDeactivated * 10 <= sumOfBreakCost)
  	  {
	  	numOfDeactivated++;
  	  	idxOfDeactivated.append(costs[i].second);
  	  }
  	  else
  	  	break;
  	}
  }
  
  clauseFSMgr->releaseAllStorePtr();
  atomFSMgr->releaseAllStorePtr();
 
  deleteClauseMemory();
  deleteAtomMemory();
		  
  delete clauseFSMgr;
  delete atomFSMgr;

  clauseStorePtr = NULL;
  clauseFreeStore = 0;
   
  atomStorePtr = NULL;
  atomFreeStore = 0;
   
  atomFSMgr = new FreeStoreManager(BLOCKSIZE);
  clauseFSMgr = new FreeStoreManager(BLOCKSIZE);
   
  numatom = 0;
  basenumatom = 0;  
  numclause = 0;
  numliterals = 0;
  numfalse = 0;
  costofnumfalse = 0;
  
  lwInfo->removeVars(idxOfDeactivated);

  for(int i = 0; i < newClauses.size(); i++)
    delete newClauses[i];
  newClauses.clear();
  newClauseWeights.clear();

  for(int i = 0; i < allClauses.size(); i++)
    delete allClauses[i];
  allClauses.clear();
  allClauseWeights.clear();

  lwInfo->getWalksatClauses(newClauses, newClauseWeights);

	//adding the clauses
  bool initial = true;
  addNewClauses(initial);
}
  
/* Initialize breakcost in the following: */
void LazyWalksat::initializeBreakCost(int startclause)
{
//cout << "initbreakcost" << endl;
  int thetruelit = -1;
  for(int i = startclause; i < numclause; i++)
  {
    numtruelit[i] = 0;
	for(int j = 0; j < size[i]; j++)
	{
        // true literal
	  if((clause[i][j] > 0) == atom[abs(clause[i][j])])
	  {
		numtruelit[i]++;
		thetruelit = clause[i][j];
	  }
    }

      // Unsatisfied positive-weighted clauses
    if(numtruelit[i] == 0 && cost[i] >= 0)
    {
	  wherefalse[i] = numfalse;
	  falseclause[numfalse] = i;
      ++numfalse;
	  costofnumfalse += cost[i];
      if (highestcost == cost[i]) {eqhighest = true; numhighest++;}
    }
      // Satisfied negative-weighted clauses
    else if(numtruelit[i] > 0 && cost[i] < 0)
    {
      wherefalse[i] = numfalse;
      falseclause[numfalse] = i;
      ++numfalse;
      costofnumfalse += abs(cost[i]);
      if (highestcost == abs(cost[i])) {eqhighest = true; numhighest++;}
    }
      // Pos. clause satisfied by exactly one literal: note breakcost for that lit
    else if (numtruelit[i] == 1 && cost[i] >= 0)
    {
	  breakcost[abs(thetruelit)] += cost[i];
	}
      // Neg. clause not satisfied: note breakcost for all lits
    else if (numtruelit[i] == 0 && cost[i] < 0)
    {
      for(int j = 0; j < size[i]; j++)
      {
        breakcost[abs(clause[i][j])] += abs(cost[i]);
      }
    }
  }
//cout << "done initbreakcost" << endl;
}
	 

void LazyWalksat::allocateClauseMemory(int allocCount)
{
	//cout<<"Allocating Memory to Store "<<allocCount<<" Clauses.."<<endl;
  int **tmpclause = new int*[allocCount];
  int *tmpcost = new int[allocCount];
  int *tmpsize = new int[allocCount];
  
  int *tmpfalseclause = new int[allocCount];
  int *tmpwherefalse = new int[allocCount];
  int *tmpnumtruelit = new int[allocCount];
	
  for(int i = 0; i < clausemem; i++)
  {
    tmpclause[i] = clause[i];
    tmpcost[i] = cost[i];
    tmpsize[i] = size[i];
		
    tmpfalseclause[i] = falseclause[i];
    tmpwherefalse[i] = wherefalse[i];
    tmpnumtruelit[i] = numtruelit[i];
  }
  
  deleteClauseMemory();
    
  clause = tmpclause;
  cost = tmpcost;
  size = tmpsize;
	
  falseclause = tmpfalseclause;
  wherefalse = tmpwherefalse;
  numtruelit = tmpnumtruelit;
	
  clausemem = allocCount;
}


void LazyWalksat::allocateAtomMemory(int allocCount)
{	
	//cout<<"Allocating Memory to Store "<<allocCount<<" Atoms.."<<endl;	
  int **tmpoccurence = new int*[2*allocCount+1];	
  int *tmpnumoccurence = new int[2*allocCount+1];	
  int *tmpnewnumoccurence = new int[2*allocCount+1];	
  int *tmpoccurencemem  = new int[2*allocCount+1];	
	
  int *tmpatom = new int[allocCount+1];	
  int *tmpinitatom = new int[allocCount+1];	
  int *tmplowatom = new int[allocCount+1];	
  int *tmpchanged = new int[allocCount+1];	
  int *tmpbreakcost = new int[allocCount+1];	

  if(atommem > 0)
  {
    for(int i = 0; i < 2*atommem + 1; i++)
    {
      tmpoccurence[i] = occurence[i];
      tmpnumoccurence[i] = numoccurence[i];
      tmpnewnumoccurence[i] = newnumoccurence[i];
      tmpoccurencemem[i] = occurencemem[i];
    }

    for(int i = 0; i < atommem + 1; i++)
    {
      tmpatom[i] = atom[i];
	  tmpinitatom[i] = initatom[i];
	  tmplowatom[i] = lowatom[i];
	  tmpchanged[i] = changed[i];
	  tmpbreakcost[i] = breakcost[i];
	}
  }

  deleteAtomMemory();
    
  occurence = tmpoccurence;
  numoccurence = tmpnumoccurence;
  newnumoccurence = tmpnewnumoccurence;
  occurencemem = tmpoccurencemem;
	
  atom = tmpatom;
  initatom = tmpinitatom;
  lowatom = tmplowatom;
  changed = tmpchanged;
  breakcost = tmpbreakcost;
	
  atommem = allocCount;
}


void LazyWalksat::deleteClauseMemory()
{
  if(clausemem == 0)
    return;

  delete [] clause;
  delete [] cost;
  delete [] size;
  delete [] falseclause;
  delete [] wherefalse;
  delete [] numtruelit;
  
  clausemem = 0;
}

void LazyWalksat::deleteAtomMemory()
{    
  if(atommem == 0)
    return;

  delete [] occurence;
  delete [] numoccurence;
  delete [] newnumoccurence;
  delete [] occurencemem;
	
  delete [] atom;
  delete [] initatom;
  delete [] lowatom;
  delete [] changed;
  delete [] breakcost;

  atommem = 0;
}


void LazyWalksat::flipatom(int toflip)
{
  int i, j;			/* loop counter */
  int litindex;
  register int cli;
  register int lit;
  int numocc;
  register int sz;
  register int * litptr;
  int * occptr;

  changed[toflip] = numflip;
  atom[toflip] = 1 - atom[toflip];

  int sign, oppsign;
	
  sign = atom[toflip];
    //oppsign = 1 - sign;
  oppsign = sign ^ 1;
    
  litindex = 2*toflip - oppsign;
  numocc = numoccurence[litindex];
  occptr = occurence[litindex];
    
  for(i = 0; i < numocc; i++)
  {
	cli = *(occptr++);
	
      // Neg. clause
    if (cost[cli] < 0)
    {
        // Became unsatisfied: Decrease num. of false clauses
      if (--numtruelit[cli] == 0)
      {
        numfalse--;
        costofnumfalse -= abs(cost[cli]);
        falseclause[wherefalse[cli]] = falseclause[numfalse];
        wherefalse[falseclause[numfalse]] = wherefalse[cli];
          
        if (abs(cost[cli]) == highestcost)
          { numhighest--; if (numhighest == 0) eqhighest = false; }
              
          /* Increment all atoms' breakcount */
        sz = size[cli];
        litptr = clause[cli];
        for(j = 0; j < sz; j++)
        {
          lit = *(litptr++);
          breakcost[abs(lit)] += abs(cost[cli]);
        }
      }
    }
      // Pos. clause
    else
    {
      if (--numtruelit[cli] == 0)
      {
        falseclause[numfalse] = cli;
        wherefalse[cli] = numfalse;
        numfalse++;
        costofnumfalse += cost[cli];
	      // Decrement toflip's breakcost
        breakcost[toflip] -= cost[cli];
        if (cost[cli] == highestcost) { eqhighest = true; numhighest++; }
      }
      else if (numtruelit[cli] == 1)
      {
          /* Find the lit in this clause that makes it true, and inc its breakcount */
        sz = size[cli];
        litptr = clause[cli];
        for (j = 0; j < sz; j++)
        {
            /* lit = clause[cli][j]; */
          lit = *(litptr++);
          if((lit > 0) == atom[abs(lit)])
          {
            breakcost[abs(lit)] += cost[cli];
            break;
          }
        }
      }
    }
  }
   
  litindex = 2*toflip - sign;
  numocc = numoccurence[litindex];
  occptr = occurence[litindex];
	
  for(i = 0; i < numocc; i++)
  {
	cli = *(occptr++);

      // Neg. clause
    if (cost[cli] < 0)
    {
        // Became satisfied: increment num. of false clauses
      if (++numtruelit[cli] == 1)
      {
        wherefalse[cli] = numfalse;
        falseclause[numfalse] = cli;
        ++numfalse;
        costofnumfalse += abs(cost[cli]);
        if (highestcost == abs(cost[cli])) {eqhighest = true; numhighest++;}

          /* Decrement all atoms' breakcount */
        sz = size[cli];
        litptr = clause[cli];
        for(j = 0; j < sz; j++)
        {
          lit = *(litptr++);
          breakcost[abs(lit)] -= abs(cost[cli]);
        }
      }
    }
      // Pos. clause
    else
    {
      if (++numtruelit[cli] == 1)
      {
        numfalse--;
        costofnumfalse -= cost[cli];
        falseclause[wherefalse[cli]] = falseclause[numfalse];
        wherefalse[falseclause[numfalse]] = wherefalse[cli];
	      
	      /* Increment toflip's breakcount */
        breakcost[toflip] += cost[cli];
        if (cost[cli] == highestcost)
          { numhighest--; if (numhighest==0) eqhighest = false; }
      }
      else if (numtruelit[cli] == 2)
      {
	      /* Find the lit in this clause other than toflip that makes it true,
		     and decrement its breakcount */
        sz = size[cli];
        litptr = clause[cli];
        for (j = 0; j < sz; j++)
        {
            /* lit = clause[cli][j]; */
          lit = *(litptr++);
          if(((lit > 0) == atom[abs(lit)]) &&
             (toflip != abs(lit)) )
          {
            breakcost[abs(lit)] -= cost[cli];
            break;
          }
        }
      }
    }
  }
}


long LazyWalksat::super(int i)
{
  long power;
  int k;

  if (i <= 0)
  {
	fprintf(stderr, "bad argument super(%d)\n", i);
	exit(1);
  }
    /* let 2^k be the least power of 2 >= (i+1) */
  k = 1;
  power = 2;
  while (power < (i+1))
  {
	k += 1;
	power *= 2;
  }
  if (power == (i+1)) return (power/2);
  return (super(i - (power/2) + 1));
}


void LazyWalksat::scanone(int argc, char *argv[], int i, int *varptr)
{
  if (i >= argc || sscanf(argv[i],"%i",varptr)!=1)
  {
	fprintf(stderr, "Bad argument %s\n", i<argc ? argv[i] : argv[argc-1]);
	exit(-1);
  }
}

void LazyWalksat::print_false_clauses_cost(long int lowbad)
{
  int i, j;
  bool bad;
  int lit, sign;

  printf("Unsatisfied pos. clauses and satisfied neg. clauses:\n");
  for (i = 0; i < numclause; i++)
  {
      // Neg. clause
    if (cost[i] < 0)
      bad = false;
      // Pos. clause
    else
      bad = true;
    
	for (j = 0; j < size[i]; j++)
    {
	  lit = clause[i][j];
	  sign = lit > 0 ? 1 : 0;

	  if ( lowatom[abs(lit)] == sign )
      {
          // Neg. clause
        if (cost[i] < 0)
          bad = true;
          // Pos. clause
        else
          bad = false;
        break;
	  }
	}
    	
    if (bad)
    {
	  printf("Cost %i ", cost[i]);
	  for (j = 0; j < size[i]; j++)
      {
		printf("%d ", clause[i][j]);
	  }
	  printf("0\n");
	}
  }
  printf("End unsatisfied pos. clauses and satisfied neg. clauses\n");
}


int LazyWalksat::countunsat(void)
{
  int i, j, unsat, lit, sign;
  bool bad;

  unsat = 0;
  for (i = 0; i < numclause; i++)
  {
      // Neg. clause
    if (cost[i] < 0)
      bad = false;
      // Pos. clause
    else
      bad = true;

	for (j = 0; j < size[i]; j++)
	{
	  lit = clause[i][j];
	  sign = lit > 0 ? 1 : 0;
	  if ( atom[abs(lit)] == sign )
	  {
          // Neg. clause
        if (cost[i] < 0)
          bad = true;
          // Pos. clause
        else
          bad = false;
        break;
	  }
	}
	if (bad) unsat++;
  }
  return unsat;
}

void LazyWalksat::countlowunsatcost(int * unsatcostptr, int * worstcostptr)
{
  int i, j, lit, sign, unsatcost, worstcost;
  bool bad;

  unsatcost = 0;
  worstcost = 0;
  for (i = 0; i < numclause; i++)
  {
      // Neg. clause
    if (cost[i] < 0)
      bad = false;
      // Pos. clause
    else
      bad = true;

	for (j = 0; j < size[i]; j++)
	{
	  lit = clause[i][j];
	  sign = lit > 0 ? 1 : 0;
	  if ( lowatom[abs(lit)] == sign )
	  {
          // Neg. clause
        if (cost[i] < 0)
          bad = true;
          // Pos. clause
        else
          bad = false;
        break;
	  }
	}
	if (bad)
	{
	  unsatcost += abs(cost[i]);
	  if (abs(cost[i]) > worstcost) worstcost = abs(cost[i]);
	}
  }
  * unsatcostptr = unsatcost;
  * worstcostptr = worstcost;
  return;
}


double LazyWalksat::elapsed_seconds(void)
{
  double answer;

  static struct rusage prog_rusage;
  static long prev_rusage_seconds = 0;
  static long prev_rusage_micro_seconds = 0;

  getrusage(0, &prog_rusage);
  answer = (double)(prog_rusage.ru_utime.tv_sec - prev_rusage_seconds)
    		+ ((double)(prog_rusage.ru_utime.tv_usec - prev_rusage_micro_seconds)) / 
           1000000.0 ;
  prev_rusage_seconds = prog_rusage.ru_utime.tv_sec ;
  prev_rusage_micro_seconds = prog_rusage.ru_utime.tv_usec ;
  return answer;
}


void LazyWalksat::print_low_assign(long int lowbad)
{
  int i, j;

  cout<<"Begin assign with lowest # bad = "<<lowbad<<" (atoms assigned true)\n";
  j = 1;
  for (i = 1; i <= numatom; i++)
  {
	if (lowatom[i]>0)
    {
	  printf(" %d", i);
	  if (j++ % 10 == 0) printf("\n");
	}
  }
  if ((j-1) % 10 != 0) printf("\n");
  printf("End assign\n");
}

void LazyWalksat::save_low_assign(void)
{
  for (int i = 1; i <= numatom; i++)
    lowatom[i] = atom[i];
}


void LazyWalksat::printClause(int cl)
{
  cout<<"(";
  for(int i = 0; i < size[cl]; i++)
  {    
	cout << clause[cl][i];
    if (i != size[cl] - 1)
    cout <<",";
  }
  cout<<")"<<endl;
}

int LazyWalksat::selecthigh(int high)
{
  //cout<<"inside select high.."<<endl;
  int counter = 0;
  int i = 0;
  while ((i < numfalse) && (counter < high))
  {
    if (abs(cost[falseclause[i]]) == highestcost)
      counter++;
    i++;
  }
 /* fprintf(stderr, "The cost of the selected clause %i\n", cost[false[i-1]]);*/
  //cout<<"successful in select high.."<<endl;
  return(falseclause[i-1]);
}
   

int LazyWalksat::pickrandom(int *numbreak, int clausesize, int tofix)
	/* returns a random number */
{
  return(random()%clausesize);
}

int LazyWalksat::pickproductsum(int *numbreak, int clausesize, int tofix)
	/* weights each possibility by the */
	/* product of the product and sum of everything else */
{
  int i;                             /* a loop counter */
  int weight[MAXLENGTH];             /* weights of each possibility */
  int tochange;                      /* value to return */
  int totalproduct = 1;              /* product of all numbreaks */
  int totalsum = 0;                  /* sum of all numbreaks */

  if(clausesize == 1)
	return(0);
  if((tochange = pickzero(numbreak,clausesize)) != NOVALUE)
	return(tochange);
  for(i = 0; i < clausesize; i++)
  {
	totalproduct *= numbreak[i];
	totalsum += numbreak[i];
  }
  for(i = 0; i < clausesize; i++)
  {
	weight[i] = (totalproduct/numbreak[i])*
                (totalsum-numbreak[i]);
  }
  return(pickweight(weight,clausesize));
}

int LazyWalksat::pickreciprocal(int *numbreak, int clausesize, int tofix)
	/* weights each possibility by its reciprocal*/
{
  int i;                             /* a loop counter */
  int weight[MAXLENGTH];             /* weights of each possibility */
  int tochange;                      /* value to return */
  int totalproduct = 1;              /* product of all numbreaks */

  if(clausesize == 1)
	return(0);
  if((tochange = pickzero(numbreak,clausesize)) != NOVALUE)
	return(tochange);
  for(i = 0;i < clausesize;i++)
	totalproduct *= numbreak[i];
  for(i = 0;i < clausesize;i++)
	weight[i] = (totalproduct/numbreak[i]);
  return(pickweight(weight,clausesize));
}

int LazyWalksat::pickadditive(int *numbreak, int clausesize, int tofix)
	/* weights each possibility by the sum of the others */
{
  int i;                             /* a loop counter */
  int weight[MAXLENGTH];             /* weights of each possibility */
  int tochange;                      /* value to return */
  int totalsum = 0;                  /* sum of all numbreaks */

  if(clausesize == 1)
	return(0);
  if((tochange = pickzero(numbreak,clausesize)) != NOVALUE)
	return(tochange);
  for(i = 0; i < clausesize;i++)
	totalsum += numbreak[i];
  for(i = 0; i < clausesize;i++)
	weight[i] = (totalsum-numbreak[i]);
  return(pickweight(weight,clausesize));
}

int LazyWalksat::pickbest(int *numbreak, int clausesize, int tofix)
{
  int i;			/* a loop counter */
  int best[MAXLENGTH];	/* best possibility so far */
  int numbest;		/* how many are tied for best */
  int bestvalue;		/* best value so far */

  numbest = 0;
  bestvalue = INT_MAX;

  for (i = 0; i < clausesize; i++)
  {
    if (numbreak[i] <= bestvalue)
	{
	  if (numbreak[i] < bestvalue) numbest = 0;
	  bestvalue = numbreak[i];
	  best[numbest++] = i;
	}
  }
	
  if (bestvalue > 0 && (random()%denominator < numerator))
  {
	if (!hard || abs(cost[tofix]) >= highestcost)
	  return(random()%clausesize); /* allow random breaks of hard clauses */

	  /* only do a random walk breaking non-hard clauses */
	numbest = 0;
	for (i = 0; i < clausesize; i++)
	{
	  if (numbreak[i] < highestcost)
	  {
        best[numbest++] = i;
	  }
	}
	  /* if (numbest==0) { fprintf(stderr, "Wff is not feasible!\n"); exit(1); } */
	if (numbest == 0) { return(random()%clausesize); }
  }
    
  if (numbest == 1) return best[0];
    
    //Added this to avoid a crash when numbest is 0 in (random()%numbest)
  if (numbest == 0) { return(random()%clausesize); }
	
  return(best[random()%numbest]);
}


int LazyWalksat::picktabu(int *numbreak,int clausesize, int tofix)
{
  int i;			/* a loop counter */
  int best[MAXLENGTH];	/* best possibility so far */
  int numbest;		/* how many are tied for best */
  int bestvalue;		/* best value so far */

  numbest = 0;
  bestvalue = BIG;

  if (numerator>0 && (random()%denominator < numerator))
  {
	for (i = 0; i < clausesize; i++)
    {
	  if ((tabu_length < numflip - changed[abs(clause[tofix][i])]) ||
		  numbreak[i] == 0)
      {
		if (numbreak[i]==0) numbest=0;
		best[numbest++] = i;
	  }
	}
  }
  else
  {
	for (i = 0; i < clausesize; i++)
    {
	  if (numbreak[i] <= bestvalue && 
		  ((tabu_length < numflip - changed[abs(clause[tofix][i])]) ||
		  numbreak[i] == 0))
      {
		if (numbreak[i] < bestvalue) numbest=0;
		bestvalue = numbreak[i];
		best[numbest++] = i;
	  }
	}
  }
    
  if (numbest == 0) return NOVALUE;
  if (numbest == 1) return best[0];
  return (best[random()%numbest]);
}

int LazyWalksat::pickexponential(int *numbreak,int clausesize, int tofix)
{
  int i;                             /* a loop counter */
  int weight[MAXLENGTH];             /* weights of each possibility */
  int tochange;                      /* value to return */

  if(clausesize == 1)
	return(0);
  if((tochange = pickzero(numbreak,clausesize)) != NOVALUE)
	return(tochange);
  for(i = 0; i < clausesize; i++)
	weight[i] = 2*2*2*2*2*2*2*2*2*2*2*2*2*2;
  for(i = 0; i < clausesize; i++)
  {
	weight[i] >>= numbreak[i]; /* weight[i] = weight[i]/(2^numbreak[i]) */
  }

  return(pickweight(weight,clausesize));
}

int LazyWalksat::pickzero(int *numbreak,int clausesize)
{
  int i;                /* loop counter */
  int numzero = 0;      /* number of zeros so far */
  int select;           /* random number */

  for(i = 0; i < clausesize; i++)
  {
	if(numbreak[i] == 0)
	numzero++;
  }
  if(numzero == 0)
	return(NOVALUE);
  select = random()%numzero;
  for(i = 0;select >= 0; i++)
  {
	if(numbreak[i] == 0)
	select--;
  }
  return(i-1);
}

int LazyWalksat::pickweight(int *weight,int clausesize)
{
  int i;                /* loop counter */
  int total = 0;        /* sum of weights */
  int select;           /* random number less than total */
  int subtotal = 0;

  for(i = 0; i < clausesize; i++)
	total += weight[i];
  if(total == 0)
	return(random()%clausesize);
  select = random()%total;
  for(i = 0; subtotal <= select; i++)
	subtotal += weight[i];
  return(i-1);
}

/* Simulated Annealing */
bool LazyWalksat::simAnnealing()
{
    // No non-evidence atoms left
  if (lwInfo->getNumDBAtoms() == 0)
  {
    numnullflip++;
    return true;
  }
    // index of atom chosen to flip
  int toflip = -1;
    // cost change caused by flipping this atom
  int change;

  if (numfalse == 0 || (random() % 100 < saRatio && !latesa))
  {
    bool initial = false;
      // Activate random non-evid. atom
    if (lwInfo->activateRandomAtom(newClauses, newClauseWeights, toflip))
      addNewClauses(initial);
      // If chosen atom is deactivated or in block with evidence
      // or fixed, then continue to next flip
    if ((haveDeactivated && lwInfo->isDeactivated(toflip)) ||
        (lwInfo->inBlockWithEvidence(toflip)))
    {
      numnullflip++;
      return true;
    }

      //interfacing with external world - if this atom is not active,
      //need to get the cost of clauses which would become unsatisfied
      //by flipping it. 
    if(!lwInfo->isActive(toflip))
    {
      change = -lwInfo->getUnSatCostWhenFlipped(toflip, newClauses, newClauseWeights);
      bool initial = false;
      addNewClauses(initial);
      lwInfo->setActive(toflip);
    }
    else
    {
      change = -breakcost[toflip];
    }

      // If change is not neg. or randomly according
      // to temperature, then flip the atom
    if (change >= 0 ||
        random() <= exp(change / (temperature / 100.0)) * RAND_MAX)
    {
      if(!lwInfo->isActive(toflip))
      {
        lwInfo->getWalksatClausesWhenFlipped(toflip, newClauses, newClauseWeights);
         
        bool initial = false;
        addNewClauses(initial);
        lwInfo->setActive(toflip);
      }
      else
      {
          // Have to check if in block and get other to flip
        if (lwInfo->inBlock(toflip))
        {
          lwInfo->chooseOtherToFlip(toflip, newClauses, newClauseWeights);
          bool initial = false;
          addNewClauses(initial);
        }
      }

      if (lwInfo->getInBlock())
        lwInfo->setActive(lwInfo->getOtherAtom());
    
      flipatom(toflip);
      if (lwInfo->getInBlock())
      {
        flipatom(lwInfo->getOtherAtom());
      }
          
      lwInfo->flipVar(toflip);
      if (lwInfo->getInBlock())
        lwInfo->flipVar(lwInfo->getOtherAtom());

        // Set back inBlock and other atom
      lwInfo->setInBlock(false);
      lwInfo->setOtherAtom(-1);
    }
    else numnullflip++;

    return true;
  }
  else
    return false;
}

void LazyWalksat::undoFixedAtoms()
{
  //Timer timer;
  //double begSec = timer.time(); 

  for (int i = 1; i < maxFixedAtoms; i++)
  {
    if (fixedAtoms_[i])
    {
      lwInfo->setEvidence(i, false);
      lwInfo->incrementNumDBAtoms();
      fixedAtoms_[i] = false;
    }
  }

  //cout << "time taken for clearing fixed atoms = "; Timer::printTime(cout, timer.time()-begSec);
  //cout << endl;
}

/* 
 * Fixes atoms using unit propagation on active clauses.
 * Fixed atoms are stored in fixedAtoms_ and set as evidence in the DB.
 */
void LazyWalksat::initFixedAtoms(Array<Array<int> *> &clauses,
                                 Array<int> &clauseWeights)
{
  //cout << "Clauses before UP: " << clauses.size() << endl;
  //cout << "Clauseweights before UP: " << clauseWeights.size() << endl;
  //cout << "Performing unit propagation ..." << endl;
  //Timer timer;
  //double begSec = timer.time(); 

  bool done = false;
  while (!done)
  {
    done = true;
      // For each clause, fix truth value if unit clause
      // after trimming evidence
    for (int i = 0; i < clauses.size(); i++)
    {
      Array<int>* clause = clauses[i];

        // All atoms could have been removed in prev. iteration
      if (clause->size() < 1)
      {
        //delete clauses[i];
        //clauses[i] = NULL;
        delete clauses.removeItemFastDisorder(i);
        clauseWeights.removeItemFastDisorder(i);
        i--;
        continue;
      }

        // Neg. clauses: all atoms fixed to false
      if (clauseWeights[i] < 0)
      {
        for (int j = 0; j < clause->size(); j++)
        {
          int atm = (*clause)[j];
            // If not already fixed, fix it
          if (!fixedAtoms_[abs(atm)])
          {
            lwInfo->setInactive(abs(atm));
            lwInfo->setEvidence(abs(atm), true);
            lwInfo->decrementNumDBAtoms();
            done = false;
            fixedAtoms_[abs(atm)] = true;
              // Neg. clause, neg. atom: set to true
            if (atm < 0)
            {
              lwInfo->setVarVal(abs(atm), true);
            }
              // Neg. clause, pos. atom: set to false
            else if (atm > 0)
            {
              lwInfo->setVarVal(abs(atm), false);
            }
          }
        }
        
        delete clauses.removeItemFastDisorder(i);
        clauseWeights.removeItemFastDisorder(i);
        i--;
        continue;
      }

        // Unit clause
      if (clause->size() == 1)
      {
        int atm = (*clause)[0];

          // If not already fixed, fix it
        if (!fixedAtoms_[abs(atm)])
        {
          lwInfo->setInactive(abs(atm));
          lwInfo->setEvidence(abs(atm), true);
          lwInfo->decrementNumDBAtoms();
          done = false;
          //fixedAtoms_.append(abs(atm));
          fixedAtoms_[abs(atm)] = true;
            // Pos. clause, pos. atom: set to true
            // Neg. clause, neg. atom: set to true
          if ((clauseWeights[i] > 0 && atm > 0) ||
              (clauseWeights[i] < 0 && atm < 0))
          {
            lwInfo->setVarVal(abs(atm), true);
          }
            // Pos. clause, neg. atom: set to false
            // Neg. clause, pos. atom: set to false
          else if ((clauseWeights[i] > 0 && atm < 0) ||
                   (clauseWeights[i] < 0 && atm > 0))
          {
            lwInfo->setVarVal(abs(atm), false);          
          }
        }
        
        delete clauses.removeItemFastDisorder(i);
        clauseWeights.removeItemFastDisorder(i);
        i--;
//cout << "time taken for fixing unit clause = "; Timer::printTime(cout, timer.time()-begSec);
//cout << endl;
        continue;
      }
      
        // Clause with 2+ atoms: try to trim evidence
      for (int j = 0; j < clause->size(); j++)
      {
        int atm = (*clause)[j];
        if (lwInfo->getEvidence(abs(atm)))
        {
          bool sense = (atm > 0);
          TruthValue tv = lwInfo->getVarVal(abs(atm)) ? TRUE : FALSE;
            // True evidence
          if (Database::sameTruthValueAndSense(tv, sense))
          {
              // Pos. clause: clause is satisfied and is removed
              // Neg. clause: clause can not be satisfied: remove it
            delete clauses.removeItemFastDisorder(i);
            clauseWeights.removeItemFastDisorder(i);
            i--;
            break;
          }
            // False evidence: remove atom from clause
          else
          {          
            clause->removeItemFastDisorder(j);
            j--;
            done = false;
          }
        }
//cout << "time taken for trimming evidence = "; Timer::printTime(cout, timer.time()-begSec);
//cout << endl;
        
      }
    }
  }
//cout << "time taken for UP = "; Timer::printTime(cout, timer.time()-begSec);
//cout << endl;

  //cout << "Clauses after UP: " << clauses.size() << endl;
  //cout << "Clauseweights after UP: " << clauseWeights.size() << endl;
  //cout << "Fixed atoms: " << fixedAtoms_.size() << endl;
  //cout << "Num. DB atoms: " << lwInfo->getNumDBAtoms() << endl;
}
