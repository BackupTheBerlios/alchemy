/* This code is based on the MaxWalkSat package of Kautz et al. */
#include "lazywalksat.h"
#include "lwutil.h"
#include "lwinfo.h"


LazyWalksat::LazyWalksat(LWInfo *lwInfo, int memLimit)
{
  clauseStorePtr = NULL;
  clauseFreeStore = 0;
   
  atomStorePtr = NULL;
  atomFreeStore = 0;
   
  atomFSMgr = new FreeStoreManager(BLOCKSIZE);
  clauseFSMgr = new FreeStoreManager(BLOCKSIZE);
   
  costexpected = SAT_TRUE;   
  hard=0; 
  gnded = false;
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
  //that can be stored)
  // if it is Huge, then it is a pointer and hence no memory has
  // been allocated yet
  #ifdef Huge
   clausemem = 0;
   atommem = 0;
  #else 
   clausemem = MAXCLAUSE;
   atommem = MAXATOM;
  #endif
}

LazyWalksat::~LazyWalksat()
{
  clauseFSMgr->releaseAllStorePtr();
  atomFSMgr->releaseAllStorePtr();
 
  deleteClauseMemory();
  deleteAtomMemory();
		  
  delete clauseFSMgr;
  delete atomFSMgr;
}

/*
void LazyWalksat::reset()
{
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
   
  costexpected = SAT_TRUE;   
  hard = 0;
  gnded = false;

  this->lwInfo = lwInfo;
  
  numatom = 0;
  basenumatom = 0;
  numclause = 0;
  numliterals = 0;
   
  //currently allocated memory (in terms of number of items
  //that can be stored)
  // if it is Huge, then it is a pointer and hence no memory has
  // been allocated yet
  #ifdef Huge
   clausemem = 0;
   atommem = 0;
  #else 
   clausemem = MAXCLAUSE;
   atommem = MAXATOM;
  #endif
}
*/

void LazyWalksat::infer(const MaxWalksatParams* const & params,
 						const int& numSolutions, const bool&  includeUnsatSolutions,
        		        Array<Array<bool>*>& solutions, Array<int>& numBad,
        		        const bool& initial)
{
  int k;			/* loop counter */
  int base_cutoff = 100000;
  int printonlysol = SAT_FALSE;
  int printfalse = SAT_FALSE;
  int printlow = SAT_TRUE;
  int superlinear = SAT_FALSE;
  //int printtrace = 1000;
  int printtrace = 0;
  long int totalflip = 0;	/* total number of flips in all tries so far */
  long int totalsuccessflip = 0; /* total number of flips in all tries which succeeded so far */
  int numtry = 0;		/* total attempts at solutions */
  int numsuccesstry = 0;	/* total found solutions */
  int numsol = 1;	        /* stop after this many tries succeeds */
  int seed;			/* seed for random */
  struct timeval tv;
  struct timezone tzp;
  double setuptime;
  double expertime;
  double totaltime;
    
  long int lowbad;		/* lowest number of bad clauses during try */
  long int lowcost;		/* lowest cost of bad clauses during try */
  long x;
  long integer_sum_x = 0;
  double sum_x = 0.0;
  double sum_x_squared = 0.0;
  double mean_x = -1;
  double second_moment_x;
  double variance_x  = -1;
  double std_dev_x = -1;
  double std_error_mean_x = -1;
  double seconds_per_flip;
  int r;
  int sum_r = 0;
  double sum_r_squared = 0.0;
  double mean_r = -1;

  //double variance_r;
  //double std_dev_r;
  //double std_error_mean_r;
    
  int worst_cost, computed_cost;

  gettimeofday(&tv,&tzp);
  seed = (( tv.tv_sec & 0177 ) * 1000000) + tv.tv_usec;
  heuristic = BEST;
  numerator = NOVALUE;
  denominator = 100;

  int targetcost = params->targetWt;	/* the cost at which the program terminates*/
  if (targetcost == INT_MAX) targetcost = 0;
  if (params->hard) hard = SAT_TRUE;

  int cutoff = params->maxSteps;
  int numrun = params->tries;
  seed = params->seed;
  if (params->lazyGnded) gnded = true;
  if (params->lazyNoApprox) noApprox = true;
  
  base_cutoff = cutoff;
  if (numerator==NOVALUE)
  {
  	if (heuristic==BEST)
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
  	cout << "Max. no. of clauses memory can hold: " << clauseLimit << endl;
  }

  srandom(seed);
  setuptime = 0;
  expertime = 0;
  elapsed_seconds();
  if (initial) init();
  //else lwInfo->makePositiveWeights();
  setuptime =  elapsed_seconds();
	
	//printf("numatom = %i, numclause = %i, numliterals = %i\n",numatom,numclause,numliterals);
    
	//printf("wff read in\n");
    //printf("                                           average             average       mean              standard\n");
    //printf("    lowest     worst    number                when                over      flips                 error\n");
    //printf("      cost    clause    #unsat    #flips     model   success       all      until        std         of\n");
    //printf("  this try  this try  this try  this try     found      rate     tries     assign        dev       mean\n");

	
  abort_flag = SAT_FALSE;
  numnullflip = 0;
  x = 0; r = 0;
  lowcost = BIG;
	
  for(k = 0; k < numrun; k++)
  {
	initrun();
	lowbad = numfalse; 
	lowcost = costofnumfalse;
	save_low_assign();
	numflip = 0;

	if (superlinear) cutoff = base_cutoff * super(r+1);

	  //cout<<"in the beginning, costofnumfalse = "<<costofnumfalse<<endl; 
	  //cout<<"cutoff = "<<cutoff<<endl;
	  //cout<<"numflip = "<<numflip<<endl;
      //cout<<"numfalse = "<<numfalse<<endl;
	  //cout<<"numclause = "<<numclause<<endl;
	  
	  //cout<<"costofnumfalse \t  numfalse \t numflip \t numclause"<<endl;
	  
	while((numflip < cutoff) && (costofnumfalse > targetcost))
	{

	
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

      if (hard && eqhighest && costexpected)
      {
		fix(selecthigh(1+random()%numhighest));
      }
      else
      {
		fix(falseclause[random()%numfalse]);
	  }

	  if (costofnumfalse < lowcost)
	  {
		lowcost = costofnumfalse;
		lowbad = numfalse;
		save_low_assign();
	  }
/*
      if(numflip % 100000 == 0)
      {
		expertime += elapsed_seconds();	 
	    totaltime = setuptime + expertime;
	    cout<<endl<<"Averaging for "<<(totalflip+numflip)<<" flips"<<endl;
		 printf("expertime - average flips per second = %10li\n", (long)((totalflip+numflip)/expertime));
         printf("totaltime - average flips per second = %10li\n", (long)((totalflip+numflip)/totaltime));
	     printf("numatom = %d , numclause = %d \n",numatom,numclause);
		 cout<<endl;
	  }
*/
	}
	  
	numtry++;
	totalflip += numflip;
	x += numflip;
	r ++;
	if(costofnumfalse <= targetcost)
	{
	  numsuccesstry++;
	  totalsuccessflip += numflip;
	  integer_sum_x += x;
	  sum_x = (double) integer_sum_x;
	  sum_x_squared += ((double)x)*((double)x);
	  mean_x = sum_x / numsuccesstry;
	  if (numsuccesstry > 1)
	  {
		second_moment_x = sum_x_squared / numsuccesstry;
		variance_x = second_moment_x - (mean_x * mean_x);
		/* Adjustment for small small sample size */
		variance_x = (variance_x * numsuccesstry)/(numsuccesstry - 1);
		std_dev_x = sqrt(variance_x);
		std_error_mean_x = std_dev_x / sqrt((double)numsuccesstry);
	  }
	  sum_r += r;
	  mean_r = ((double)sum_r)/numsuccesstry;
	  sum_r_squared += ((double)r)*((double)r);
	  x = 0;
	  r = 0;
	}

	countlowunsatcost(&computed_cost, &worst_cost);
	  
	  // Fill the solutions array
	if (printlow && (!printonlysol || costofnumfalse<=targetcost))
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
/*    
    if(numsuccesstry == 0)
	  printf("%10li%10i%10li%10li         *         0         *          *          *          *\n",
		   	 lowcost,worst_cost,lowbad,numflip);
	else if (numsuccesstry == 1)
	  printf("%10li%10i%10li%10li%10li%10i%10li %10.1f          *          *\n",
		   	 lowcost,worst_cost,lowbad,numflip,totalsuccessflip/numsuccesstry,
		   	 (numsuccesstry*100)/numtry,totalflip/numsuccesstry,
		   	 mean_x);
	else
	  printf("%10li%10i%10li%10li%10li%10i%10li %10.1f %10.1f %10.1f\n",
		   	 lowcost,worst_cost,lowbad,numflip,totalsuccessflip/numsuccesstry,
		   	 (numsuccesstry*100)/numtry,totalflip/numsuccesstry,
		   	 mean_x, std_dev_x, std_error_mean_x);
*/	
	if (numfalse>0 && printfalse)
	  print_false_clauses_cost(lowbad);
    
	if (numsuccesstry >= numsol) break;
	if (abort_flag) break;
	fflush(stdout);
  }

  //now, set the db values to the best values observed
  lwInfo->setVarVals(lowatom);

  expertime += elapsed_seconds();
  totaltime = setuptime + expertime;
  seconds_per_flip = expertime / totalflip;
/*
  printf("\nset up seconds = %f\n", setuptime);
  printf("\nwalksat run seconds = %f\n", expertime);
  printf("\ntotal elapsed seconds = %f\n", totaltime);
    
  cout<<endl<<"Averaging for all the flips ("<<totalflip<<")"<<endl;
  printf("expertime - average flips per second = %10li\n", (long)(totalflip/expertime));
  printf("totaltime - average flips per second = %10li\n", (long)(totalflip/totaltime));
  cout<<endl;
	
  if (heuristic == TABU)
    printf("proportion null flips = %f\n", ((double)numnullflip)/totalflip);
  printf("number of solutions found = %d\n", numsuccesstry);
  if (numsuccesstry > 0)
  {
	printf("mean flips until assign = %f\n", mean_x);
	if (numsuccesstry>1)
	{
	  printf("  variance = %f\n", variance_x);
	  printf("  standard deviation = %f\n", std_dev_x);
	  printf("  standard error of mean = %f\n", std_error_mean_x);
	}
	printf("mean seconds until assign = %f\n", mean_x * seconds_per_flip);
	if (numsuccesstry>1)
	{
	  printf("  variance = %f\n", variance_x * seconds_per_flip * seconds_per_flip);
	  printf("  standard deviation = %f\n", std_dev_x * seconds_per_flip);
	  printf("  standard error of mean = %f\n", std_error_mean_x * seconds_per_flip);
	}
	printf("mean restarts until assign = %f\n", mean_r);
	if (numsuccesstry>1)
	{
	  variance_r = (sum_r_squared / numsuccesstry) - (mean_r * mean_r);
	  variance_r = (variance_r * numsuccesstry)/(numsuccesstry - 1);	   
	  std_dev_r = sqrt(variance_r);
	  std_error_mean_r = std_dev_r / sqrt((double)numsuccesstry);
	  printf("  variance = %f\n", variance_r);
	  printf("  standard deviation = %f\n", std_dev_r);
	  printf("  standard error of mean = %f\n", std_error_mean_r);
	}
  }  
*/
  return;
}

int LazyWalksat::getNumInitClauses()
{
  return initClauseCount;
}

int LazyWalksat::getNumClauses()
{
  return numclause;
}

/*******************************************************************************/
/****************************** PRIVATE FUNCTIONS ****************************/
/*******************************************************************************/


/* initialize a particular run */
void LazyWalksat::init()
{
    //should be input by user ideally
    //highestcost=1;
  highestcost=BIG;
    
  numclause = 0;
  numatom = 0;
  numliterals = 0;

  cout<<"inside init (lazywalksat.cpp) "<<endl;

  for(int i = 0; i < newClauses.size(); i++)
    delete newClauses[i];
  newClauses.clear();
  newClauseWeights.clear();
  lwInfo->getWalksatClauses(newClauses, newClauseWeights);
  
  for(int i = 0; i < newClauses.size(); i++)
    delete newClauses[i];
	
  basenumatom = lwInfo->getVarCount();
  cout<<"Number of Baseatoms = "<<basenumatom<<endl;
    
  int defaultCnt = newClauses.size();
  int defaultCost = 0;
  initClauseCount = defaultCnt;
  for(int i = 0; i < defaultCnt; i++)
	defaultCost += newClauseWeights[i];

	cout<<endl;
	cout<<"Default => Cost\t"<<"******\t"<<" Clause Cnt\t"<<endl;
	cout<<"           "<<defaultCost<<"\t"<<"******\t"<<defaultCnt<<"\t"<<endl<<endl;
	
  newClauses.clear();
  newClauseWeights.clear();
	
  if(gnded)
	lwInfo->setAllActive();
  else
  {
	for(int i = 1; i < basenumatom+1; i++) 
	  lwInfo->setActive(i);
  }
  
	//getting the initial set of clausees
  lwInfo->getWalksatClauses(newClauses, newClauseWeights);
	
	//adding the clauses
  bool initial = true;
  addNewClauses(initial);
  cout<<"done with constructing the initial network!"<<endl;
}


/* initialize a particular run */
void LazyWalksat::initrun()
{
  //cout<<"****** inside initrun *****"<<endl;
  int newval; 
  numfalse = 0;
  costofnumfalse = 0;
  eqhighest = 0;
  numhighest = 0;

  for(int i = 1; i < numatom+1; i++)
  {
	changed[i] = -BIG;
	breakcost[i] = 0;
      
    // if initially active
	if(i <= basenumatom) 
	  newval = random()%2;	   
	else
	  newval = initatom[i];
	  
	//newval = random()%2;	   
	  
	if(atom[i] != newval)
	{
	  lwInfo->flipVar(i);
	  atom[i] = newval;
	}
  }
  initializeBreakCost(0);
  //cout<<"****** leaving initrun *****"<<endl;
}


void LazyWalksat::fix(int tofix)
{	
  int numbreak[MAXLENGTH];	/* number of clauses changing */
    /* each atoms would make false */
  int i;			/* loop counter */
    //int j;			/* another loop counter */
  int choice;
    //static int (*pickcode[])(int *numbreak,int clausesize, int tofix) = 
     // {pickrandom,pickproductsum,pickreciprocal,pickadditive,
    //pickbest,pickexponential,picktabu};
    //cout<<"size of the clause to be fixed.."<<size[tofix]<<endl;  
	
	//cout<<"unsat clause -> ";

  for(i = 0; i < size[tofix]; i++)
  {
	int atm = abs(clause[tofix][i]);	 
	
	// If atm has been deactivated, then give it a very high weight
	if (haveDeactivated && lwInfo->isDeactivated(atm)) 
	{
	  numbreak[i] = INT_MAX;
	  continue;
	}
	//if (lwInfo->isDeactivated(atm)) cout << "Looked at deactivated atom" << endl;
	
	  //interfacing with external world - if this atom is not active,
	  //need to get the cost of clauses which would become unsatisfied
	  //by flipping it. 
	  //cout<<"gnded = "<<gnded<<endl;
	if(!gnded && !lwInfo->isActive(atm))
	{
	  numbreak[i] = lwInfo->getUnSatCostWhenFlipped(atm);
	}
	else
	{ 
	  numbreak[i] = breakcost[atm];
	}
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
	  choice = pickbest(numbreak,size[tofix],tofix);
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
	
    //choice = (pickcode[heuristic])(numbreak,size[tofix],tofix);
    
  if (choice == NOVALUE)
    numnullflip++;
  else
  {
	int atm = abs(clause[tofix][choice]);

	// If chosen atom is deactivated, then continue to next flip
	if (haveDeactivated && lwInfo->isDeactivated(atm))
	{
	  numnullflip++;
	  return;	
	}
	if(!gnded && !lwInfo->isActive(atm))
	{
	    //cout<<" ** not active"<<endl;
	  lwInfo->getWalksatClausesWhenFlipped(atm, newClauses, newClauseWeights);
		 
	  bool initial = false;
	  addNewClauses(initial);
	  lwInfo->setActive(atm);
	} 
	flipatom(atm);
	if(!gnded)
	  lwInfo->flipVar(atm); 
  }
}


/* added by Parag - add clauses*/
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
	//if gnded, we know before hand what all clauses we need to work on.
	//therefore, there is no need to allocate extra memory
	if(gnded) 
	  allocateClauseMemory(numclause);
	//if memory is getting tight, then can not allocate 2 times
	else if (2*numclause > clauseLimit)
	  allocateClauseMemory(clauseLimit);
	else
	  allocateClauseMemory(2*numclause);
  } 

  /* allocate atom memory */
  if(atommem < numatom)
  {
	//if gnded, we know before hand what all atoms we need to work on.
	//therefore, there is no need to allocate extra memory
	if(gnded) 
	  allocateAtomMemory(numatom);
	else
	  allocateAtomMemory(2*numatom);
  }

  //initialize various structues for atoms
  for(int i = oldnumatom+1; i < numatom+1; i++)
  {
	atom[i] = lwInfo->getVarVal(i);
	initatom[i] = atom[i];
	lowatom[i] = atom[i];
	changed[i] = -BIG;
	breakcost[i] = 0;
  }
  for(int i = 2*oldnumatom+1; i < 2*numatom+1; i++)
  {
	numoccurence[i] = 0;
	newnumoccurence[i] = 0;
	occurencemem[i] = 0;
  }
	
	//cout<<"at checkpoint 2.."<<endl;

  for(i = oldnumclause; i < numclause;i++)
  {
	size[i] = 0; 
	if (costexpected)
	{
      //cout<<"not assigning default cost. some problem.."<<endl;
	  cost[i] = allClauseWeights[i];
	}
	else
	{
	  cost[i] = 1; /*the default cost of a clause violation is unit 1*/
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
		//cout<<"lit = "<<lit<<endl;
	  if(lit != 0)
	  {
		//cout<<"freestore  = "<<freestore<<endl;
		*(clauseStorePtr++) = lit; /* clause[i][size[i]] = j; */
		clauseFreeStore--;
		numliterals++;
		//cout<<"done assigning lit to store ptr."<<endl;
	    litindex = 2*abs(lit)-LWUtil::mySign(lit);
		newnumoccurence[litindex]++;
		//cout<<"done updating numoccurrence.."<<endl;
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
	  litindex = 2*abs(lit)-LWUtil::mySign(lit);
	  int totaloccurence = newnumoccurence[litindex] + numoccurence[litindex];
	    //cout<<"litindex = "<<litindex<<endl;	
		//cout<<"total occurence = "<<totaloccurence<<endl;
	  if(occurencemem[litindex] < totaloccurence)
	  {
	    int tmpoccurencemem = 2*totaloccurence;
	    if(atomFreeStore < tmpoccurencemem)
	    {
		  atomStorePtr = atomFSMgr->getStorePtr();
		  atomFreeStore = atomFSMgr->getBlockSize();
		  fprintf(stderr,"allocating memory...\n");
	    }
		
	  	int *tmpoccurence = atomStorePtr;
	  	atomFreeStore -= tmpoccurencemem;
	  	atomStorePtr += tmpoccurencemem;
	  	for(int k=0;k<occurencemem[litindex];k++)
		  tmpoccurence[k] = occurence[litindex][k];
	  	occurencemem[litindex] = tmpoccurencemem;
	  	occurence[litindex] = tmpoccurence;
		//cout<<"occurencemem["<<litindex<<"] = "<<occurencemem[litindex]<<endl;
      }
	}
  }	

  for(i=oldnumclause; i<numclause; i++)
  {
	    //printClause(i); 
		//cout<<" [ ** ";
	for(j = 0;j < size[i];j++)
	{
	  //cout<<clause[i][j]<<"*"<<numoccurence[clause[i][j]+numatom]<<" ";
	  lit = clause[i][j];
	  if(lit == 0)
	    continue;
	  litindex = 2*abs(lit)-LWUtil::mySign(lit);
	  occurence[litindex][numoccurence[litindex]] = i;
	  numoccurence[litindex]++;
	  newnumoccurence[litindex]--;
	}
		//cout<<" ** ] "<<endl<<endl;
  }
  if(!initial)
    initializeBreakCost(oldnumclause);
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

cout << "sorting" << endl;
	costs.quicksort();
cout << "sorted" << endl;

  	int sumOfDeactivated = 0;
  	int numOfDeactivated = 0;
  	for (int i = 0; i < costs.size(); i++)
  	{
  	  sumOfDeactivated += costs[i].first;
  	  	// 10% of total weight
  	  if (sumOfDeactivated * 10 <= sumOfBreakCost)
  		//if (sumOfDeactivated <= sumOfBreakCost)
  	  {
	  	numOfDeactivated++;
  	  	//cout << "deactivating " << costs[i].first << " " << costs[i].second << endl;
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
  int thetruelit = -1;
  for(int i = startclause; i < numclause; i++)
  {
    numtruelit[i] = 0;
	for(int j = 0;j < size[i];j++)
	{
	  if((clause[i][j] > 0) == atom[abs(clause[i][j])])
	  {
		numtruelit[i]++;
		thetruelit = clause[i][j];
	  }
    }

    if(numtruelit[i] == 0)
    {
//cout << "Clause no. " << i << " no true lits, cost " << cost[i] << endl;
	  wherefalse[i] = numfalse;
	  falseclause[numfalse] = i;
      numfalse++;
	  costofnumfalse += cost[i];
      if (highestcost == cost[i]) {eqhighest = 1; numhighest++;}
    }
    else if (numtruelit[i] == 1)
    {
	  breakcost[abs(thetruelit)] += cost[i];
	}
  }
}
	 

/* added by Parag - allocate memory for clauses */
void LazyWalksat::allocateClauseMemory(int allocCount) {
#ifdef Huge
	cout<<"Allocating Memory to Store "<<allocCount<<" Clauses.."<<endl;
	
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
	 
 #else
	cout<<"Problem here. Pre-Allocated clause memory not sufficient!!!"<<endl;
 #endif

}


/* added by Parag - allocate memory for clauses */
void LazyWalksat::allocateAtomMemory(int allocCount) {    
	
#ifdef Huge
	cout<<"Allocating Memory to Store "<<allocCount<<" Atoms.."<<endl;
	
	/*
	int **tmpoccurence = (int **) malloc(sizeof(int *)*(2*allocCount+1));	
    int *tmpnumoccurence = (int *) malloc(sizeof(int)*(2*allocCount+1));	
    int *tmpnewnumoccurence = (int *) malloc(sizeof(int)*(2*allocCount+1));	
    int *tmpoccurencemem  = (int *) malloc(sizeof(int)*(2*allocCount+1));	
	
	int *tmpatom = (int *) malloc(sizeof(int)*(allocCount+1));	
	int *tmpinitatom = (int *) malloc(sizeof(int)*(allocCount+1));	
	int *tmplowatom = (int *) malloc(sizeof(int)*(allocCount+1));	
    int *tmpchanged = (int *) malloc(sizeof(int)*(allocCount+1));	
    int *tmpbreakcost = (int *) malloc(sizeof(int)*(allocCount+1));	
   */
	
	int **tmpoccurence = new int*[2*allocCount+1];	
    int *tmpnumoccurence = new int[2*allocCount+1];	
    int *tmpnewnumoccurence = new int[2*allocCount+1];	
    int *tmpoccurencemem  = new int[2*allocCount+1];	
	
	int *tmpatom = new int[allocCount+1];	
	int *tmpinitatom = new int[allocCount+1];	
	int *tmplowatom = new int[allocCount+1];	
    int *tmpchanged = new int[allocCount+1];	
    int *tmpbreakcost = new int[allocCount+1];	

	
	if(atommem > 0) {
	 for(int i=0;i<2*atommem+1;i++){
      tmpoccurence[i] = occurence[i];
	  tmpnumoccurence[i] = numoccurence[i];
	  tmpnewnumoccurence[i] = newnumoccurence[i];
	  tmpoccurencemem[i] = occurencemem[i];
	 }
	 for(int i=0;i<atommem+1;i++){
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

#else
	cout<<"Problem here. Pre-Allocated atom memory not sufficient!!!"<<endl;
#endif
   
}


/* added by Parag - delete memory allocated for clauses */
void LazyWalksat::deleteClauseMemory() {
if(clausemem == 0)
	 return;
#ifdef Huge
	delete clause;
    delete cost;
    delete size;
    delete falseclause;
    delete wherefalse;
    delete numtruelit;
	 
	clausemem = 0;
#endif
	
}

/* added by Parag - delete memory allocated for atoms */
void LazyWalksat::deleteAtomMemory() {    
if(atommem == 0)
	 return;
#ifdef Huge
    delete occurence;
    delete numoccurence;
    delete newnumoccurence;
	delete occurencemem;
	
	delete atom;
    delete initatom;
	delete lowatom;
    delete changed;
    delete breakcost;
    
	atommem = 0;
#endif
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
  atom[toflip] = 1-atom[toflip];
	
  int sign,oppsign;
	
  sign = atom[toflip];
    //oppsign = 1 - sign;
  oppsign = sign ^ 1;
    
	//cout<<"sign = "<<sign<<"oppsign ="<<oppsign<<endl;
  litindex = 2*toflip - oppsign; 
  numocc = numoccurence[litindex];
  occptr = occurence[litindex];
    
	//cout<<"numatom = "<<numatom<<endl;
	//cout<<"enforce = "<<(-toenforce)<<endl;
	//cout<<"numocc = "<<numocc<<endl;
	//cout<<"clauses which contain flipped atom in the opposite sense.. -> ";
  for(i = 0; i < numocc; i++)
  {
	  /* cli = occurence[numatom-toenforce][i]; */
	cli = *(occptr++);
      //printClause(cli);
	  
	if (--numtruelit[cli] == 0)
	{
		//cout<<"and this clause now becomes false.."<<endl<<endl; 
	  falseclause[numfalse] = cli;
	  wherefalse[cli] = numfalse;
	  numfalse++;
      costofnumfalse += cost[cli];
	    /* Decrement toflip's breakcost */
	  breakcost[toflip] -= cost[cli];
      if (cost[cli] == highestcost) { eqhighest = 1; numhighest++; }
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
   
  litindex = 2*toflip - sign;
  numocc = numoccurence[litindex];
  occptr = occurence[litindex];
	
	//cout<<"numatom = "<<numatom<<endl;
	//cout<<"enforced = "<<toenforce<<endl;
	//cout<<"numocc = "<<numocc<<endl;
	//cout<<"clauses which contain the flipped atom in the right sense.. -> ";
  for(i = 0; i < numocc; i++)
  {
	  /* cli = occurence[numatom+toenforce][i]; */
	cli = *(occptr++);
	  //printClause(cli);

	if (++numtruelit[cli] == 1)
	{
	    //cout<<"and this clause now becomes true.."<<endl; 
	  numfalse--;
	  costofnumfalse -= cost[cli];
      falseclause[wherefalse[cli]] = falseclause[numfalse];
	  wherefalse[falseclause[numfalse]] = wherefalse[cli];
	      
	    /* Increment toflip's breakcount */
	  breakcost[toflip] += cost[cli];
        //cout<<"cost = "<<cost[cli]<<endl;
		//cout<<"highestcost = "<<highestcost<<endl;
		// cout<<"numhighest = "<<numhighest<<endl;
	  if (cost[cli] == highestcost)
      { numhighest--; if (numhighest==0) eqhighest = 0; }
	  //cout<<"After changing, numhighest = "<<numhighest<<endl;
	          
	}
	else if (numtruelit[cli] == 2)
	{
	    /* Find the lit in this clause other than toflip that makes it true,
		   and decrement its breakcount */
	  sz = size[cli];
	  litptr = clause[cli];
	  for (j=0; j<sz; j++)
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


long LazyWalksat::super(int i)
{
    long power;
    int k;

    if (i<=0){
	fprintf(stderr, "bad argument super(%d)\n", i);
	exit(1);
    }
    /* let 2^k be the least power of 2 >= (i+1) */
    k = 1;
    power = 2;
    while (power < (i+1)){
	k += 1;
	power *= 2;
    }
    if (power == (i+1)) return (power/2);
    return (super(i - (power/2) + 1));
}


void LazyWalksat::scanone(int argc, char *argv[], int i, int *varptr)
{
    if (i>=argc || sscanf(argv[i],"%i",varptr)!=1){
	fprintf(stderr, "Bad argument %s\n", i<argc ? argv[i] : argv[argc-1]);
	exit(-1);
    }
}

void
 LazyWalksat::print_false_clauses_cost(long int lowbad)
{
    int i, j;
    int bad;
    int lit, sign;

    printf("Unsatisfied clauses:\n");
    for (i=0; i<numclause; i++){
	bad = SAT_TRUE;
	for (j=0; j < size[i]; j++) {
	    lit = clause[i][j];
	    sign = lit > 0 ? 1 : 0;
	    if ( lowatom[abs(lit)] == sign ){
		bad = SAT_FALSE;
		break;
	    }
	}
	if (bad){
	    printf("Cost %i ", cost[i]);
	    for (j=0; j<size[i]; j++){
		printf("%d ", clause[i][j]);
	    }
	    printf("0\n");
	}
    }
    printf("End unsatisfied clauses\n");
}


int LazyWalksat::countunsat(void)
{
  int i, j, unsat, bad, lit, sign;

  unsat = 0;
  for (i=0;i < numclause;i++)
  {
	bad = SAT_TRUE;
	for (j=0; j < size[i]; j++)
	{
	  lit = clause[i][j];
	  sign = lit > 0 ? 1 : 0;
	  if ( atom[abs(lit)] == sign )
	  {
		bad = SAT_FALSE;
		break;
	  }
	}
	if (bad) unsat++;
  }
  return unsat;
}

void LazyWalksat::countlowunsatcost(int * unsatcostptr, int * worstcostptr)
{
  int i, j, bad, lit, sign, unsatcost, worstcost;

  unsatcost = 0;
  worstcost = 0;
  for (i=0;i < numclause;i++)
  {
	bad = SAT_TRUE;
	for (j=0; j < size[i]; j++)
	{
	  lit = clause[i][j];
	  sign = lit > 0 ? 1 : 0;
	  if ( lowatom[abs(lit)] == sign )
	  {
		bad = SAT_FALSE;
		break;
	  }
	}
	if (bad)
	{
	  unsatcost += cost[i] ;
	  if (cost[i] > worstcost) worstcost = cost[i];
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
    j=1;
    for (i=1; i<=numatom; i++){
	if (lowatom[i]>0){
	    printf(" %d", i);
	    if (j++ % 10 == 0) printf("\n");
	}
    }
    if ((j-1) % 10 != 0) printf("\n");
    printf("End assign\n");
}

void LazyWalksat::save_low_assign(void)
{
  int i;
  for (i = 1; i <= numatom; i++)
    lowatom[i] = atom[i];
}


void LazyWalksat::printClause(int cl)
{
  cout<<"(";
  for(int i = 0;i < size[cl];i++)
  {    
	cout<<clause[cl][i]<<",";
  }
  cout<<")"<<endl;
}

int LazyWalksat::selecthigh(int high)
{
  //cout<<"inside select high.."<<endl;
  int counter=0;
  int i=0;
  while ((i<numfalse) && (counter<high))
  {
    if (cost[falseclause[i]] == highestcost)
       counter++;
    i++;
  }
 /* fprintf(stderr, "The cost of the selected clause %i\n", cost[false[i-1]]);*/
  //cout<<"successful in select high.."<<endl;
  return(falseclause[i-1]);
}
   

int LazyWalksat::pickrandom(int *numbreak,int clausesize, int tofix)
	/* returns a random number */
	{
	return(random()%clausesize);
	}

int LazyWalksat::pickproductsum(int *numbreak,int clausesize, int tofix)
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
	for(i = 0;i < clausesize;i++)
		{
		totalproduct *= numbreak[i];
		totalsum += numbreak[i];
		}
	for(i = 0;i < clausesize;i++)
		{
		weight[i] = (totalproduct/numbreak[i])*
			(totalsum-numbreak[i]);
		}
	return(pickweight(weight,clausesize));
	}

int LazyWalksat::pickreciprocal(int *numbreak,int clausesize, int tofix)
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

int LazyWalksat::pickadditive(int *numbreak,int clausesize, int tofix)
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
	for(i = 0;i < clausesize;i++)
		totalsum += numbreak[i];
	for(i = 0;i < clausesize;i++)
		weight[i] = (totalsum-numbreak[i]);
	return(pickweight(weight,clausesize));
	}

int LazyWalksat::pickbest(int *numbreak,int clausesize, int tofix)
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
	  if (!hard || cost[tofix] >= highestcost) return(random()%clausesize); /* allow random breaks of hard clauses */

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
    
    //STAN: Added this to avoid a crash when numbest is 0 in (random()%numbest)
    if (numbest == 0) { return(random()%clausesize); }
	
	return(best[random()%numbest]); 

}


int LazyWalksat::picktabu(int *numbreak,int clausesize, int tofix)
{
    int i;			/* a loop counter */
    int best[MAXLENGTH];	/* best possibility so far */
    int numbest;		/* how many are tied for best */
    int bestvalue;		/* best value so far */
    //int tabu_level;
    //int val;

    numbest = 0;
    bestvalue = BIG;

    if (numerator>0 && (random()%denominator < numerator)){
	for (i=0; i< clausesize; i++){
	    if ((tabu_length < numflip - changed[abs(clause[tofix][i])]) ||
		numbreak[i]==0){
		if (numbreak[i]==0) numbest=0;
		best[numbest++] = i;
	    }
	}
    }
    else {
	for (i=0; i< clausesize; i++){
	    if (numbreak[i]<=bestvalue && 
		((tabu_length < numflip - changed[abs(clause[tofix][i])]) ||
		 numbreak[i]==0)){
		if (numbreak[i]<bestvalue) numbest=0;
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
	//int best[MAXLENGTH];               /* best possibility so far */
	//int numbest;                       /* how many are tied for best */
	//int bestvalue;                     /* best value so far */
	int weight[MAXLENGTH];             /* weights of each possibility */
	int tochange;                      /* value to return */
	//int totalproduct = 1;              /* product of all numbreaks */
	//int totalsum = 0;                  /* sum of all numbreaks */

	if(clausesize == 1)
		return(0);
	if((tochange = pickzero(numbreak,clausesize)) != NOVALUE)
		return(tochange);
	for(i = 0;i < clausesize;i++)
		weight[i] = 2*2*2*2*2*2*2*2*2*2*2*2*2*2;
	for(i = 0;i < clausesize;i++)
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

	for(i = 0;i < clausesize;i++)
		{
		if(numbreak[i] == 0)
			numzero++;
		}
	if(numzero == 0)
		return(NOVALUE);
	select = random()%numzero;
	for(i = 0;select >= 0;i++)
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

	for(i = 0;i < clausesize;i++)
		total += weight[i];
	if(total == 0)
		return(random()%clausesize);
	select = random()%total;
	for(i = 0;subtotal <= select;i++)
		subtotal += weight[i];
	return(i-1);
	}
