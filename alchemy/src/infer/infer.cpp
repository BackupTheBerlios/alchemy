#include <unistd.h>
#include <fstream>
#include <climits>
#include "fol.h"
#include "mrf.h"
#include "arguments.h"
#include "util.h"
#include "learnwts.h"
#include "infer.h"
#include "lazywalksat.h"
#include "lwutil.h"

extern const char* ZZ_TMP_FILE_POSTFIX; //defined in fol.y

char* ainMLNFiles     = NULL;
char* aevidenceFiles  = NULL;
char* afuncFiles  = NULL;
char* aresultsFile   = NULL;
char* aqueryPredsStr = NULL;
char* aqueryFile     = NULL;
bool  aclosedWorld   = false; 
char* aopenWorldPredsStr = NULL;
bool  amapPos = false;
bool  amapAll = false;
bool  agibbsInfer = false;
int   asmooth = 1;

bool averifyResults = false;
bool aisQueryEvidence = false;

bool mwsLazy = false;

int  mwsMaxSteps = 1000000;
int  mwsTries    = 1;
int  mwsTargetWt = INT_MAX;
bool mwsHard     = false;
int mwsSeed = -1;
// Limit in kbytes before using LazyWalksat
int mwsLimit = -1;

bool lazywsGnded = false;
bool lazyNoApprox = false;
bool mwsOriginal = false;


int    mcmcNumChains    = 10;
int    mcmcBurnMinSteps = 100;
int    mcmcBurnMaxSteps = 100;
double mcmcDelta        = 0.05;
double mcmcEpsilonError = 0.01;
int    mcmcMinSteps     = -1;
int    mcmcMaxSteps     = 1000;
double mcmcFracConverged= 0.95;
int    mcmcWalksatType  = 1;
int    mcmcMaxSeconds   = -1;
int    mcmcSamplesPerTest= 100;


ARGS ARGS::Args[] = 
{
  ARGS("i", ARGS::Req, ainMLNFiles, "Comma-separated input .mln files."),

  ARGS("e", ARGS::Req, aevidenceFiles, 
       "Comma-separated .db files containing known ground atoms (evidence), "
       "including function definitions."),

// Marc: functions defined in .db files
//  ARGS("functions", ARGS::Opt, afuncFiles, 
//       "Comma-separated .func files containing function definitions."),

  ARGS("r", ARGS::Req, aresultsFile,
       "The probability estimates are written to this file."),
    
  ARGS("q", ARGS::Opt, aqueryPredsStr, 
       "Query atoms (comma-separated with no space)  "
       ",e.g., cancer,smokes(x),friends(Stan,x). Query atoms are always "
       "open world."),

  ARGS("f", ARGS::Opt, aqueryFile,
       "A .db file containing ground query atoms, "
       "which are are always open world."),

  ARGS("c", ARGS::Tog, aclosedWorld,
       "If specified, all atoms are closed-world except the query ones; "
       "otherwise, all atoms are open world. "
       "Cannot be used with the -o option."),

  ARGS("o", ARGS::Opt, aopenWorldPredsStr,
       "Specified atoms (comma-separated with no space) are open world, "
       "while others are closed-world (except query ones). "
       "Cannot be used with the -c option."),

  ARGS("m", ARGS::Tog, amapPos, 
       "Run MAP inference and return only positive query atoms."),

  ARGS("a", ARGS::Tog, amapAll, 
       "Run MAP inference and show 0/1 results for all query atoms."),

  ARGS("p", ARGS::Tog, agibbsInfer, 
       "Run inference using MCMC (Gibbs sampling) and return probabilities "
       "for all query atoms."),
  
  ARGS("v", ARGS::Tog, averifyResults, 
       "If this flag is set, the query preds are taken to be closed world, for the purpose "
	   " of verification. At the time of the inference, all the query preds are set to unknown."),
  
  ARGS("queryEvidence", ARGS::Tog, aisQueryEvidence, 
       "If this flag is set along with the verify flag, then all the groundings of query preds"
	   " not in db are assumed false evudence. This flag is ignored if verify flag is not set"),
 
  ARGS("lazy", ARGS::Tog, mwsLazy, 
       "run lazy maxwalksat if this flag is set."),
  
  ARGS("mwsMaxSteps", ARGS::Opt, mwsMaxSteps,
       "[1000000] The max number of steps taken by MaxWalksat."),

  ARGS("mwsTries", ARGS::Opt, mwsTries, 
       "[1] The max number of attempts MaxWalksat takes to find a solution."),

  ARGS("mwsTargetWt", ARGS::Opt, mwsTargetWt,
       "[the best possible] MaxWalksat tries to find a solution with weight <= "
       "specified weight."),

  ARGS("mwsHard", ARGS::Opt, mwsHard, 
       "[false] MaxWalksat never breaks a hard clause in order to satisfy "
       "a soft one."),
  
  ARGS("mwsSeed", ARGS::Opt, mwsSeed, 
       "[-1] seed for random number generator used in MaxWalksat. "
	   "-1 means it is intialized randomly"),
  
  ARGS("mwsLimit", ARGS::Opt, mwsLimit, 
       "[-1] Maximum limit in kbytes which MaxWalksat should use for inference. "
	   "-1 means main memory available on system is used"),
  
  ARGS("gnded", ARGS::Tog, lazywsGnded, 
       "run the grounded version of lazy maxwalksat if this flag is set. This flag "
	   "is ignored if -lazy is not set."),
  
  ARGS("noApprox", ARGS::Tog, lazyNoApprox, 
       "LazySat will not approximate by deactivating atoms to save memory. This flag "
	   "is ignored if -lazy is not set."),
  
  ARGS("mcmcSmooth", ARGS::Opt, asmooth, 
       "[1] Number of samples to use for smoothing."),

  ARGS("mcmcNumChains", ARGS::Opt, mcmcNumChains, 
       "[10] Number of MCMC chains (there must be at least 2)."),

  ARGS("mcmcBurnMinSteps", ARGS::Opt, mcmcBurnMinSteps,
       "[100] Minimun number of burn in steps (-1: no minimum)."),

  ARGS("mcmcBurnMaxSteps", ARGS::Opt, mcmcBurnMaxSteps,
       "[100] Maximum number of burn-in steps (-1: no maximum)."),

  ARGS("mcmcDelta", ARGS::Opt, mcmcDelta,
       "[0.05] During Gibbs sampling, probabilty that epsilon error is "
       "exceeded is less than this value."),

  ARGS("mcmcEpsilonError", ARGS::Opt, mcmcEpsilonError,
       "[0.01] Fractional error from true probability."),

  ARGS("mcmcMinSteps", ARGS::Opt, mcmcMinSteps, 
       "[-1] Minimum number of Gibbs sampling steps."),

  ARGS("mcmcMaxSteps", ARGS::Opt, mcmcMaxSteps, 
       "[1000] Maximum number of Gibbs sampling steps."),

  ARGS("mcmcFracConverged", ARGS::Opt, mcmcFracConverged, 
       "[0.95] Fraction of ground atoms with probabilities that "
       "have converged."),

  ARGS("mcmcWalksatType", ARGS::Opt, mcmcWalksatType, 
       "[1] Use Max Walksat to initialize ground atoms' truth values "
       "(1: use Max Walksat, 0: random initialization)."),

  ARGS("mcmcMaxSeconds", ARGS::Opt, mcmcMaxSeconds, 
       "[-1] Max number of seconds to run MCMC (-1: no maximum)."),
  
  ARGS("mcmcSamplesPerTest", ARGS::Opt, mcmcSamplesPerTest, 
       "[100] Perform convergence test once after this many number of samples "
       "per chain."),

  ARGS()
};

/************************ Functions Added by Parag ***************************/
	
/* run the lazy walksat */
void runLazyWalksat(MLN *mln, Domain *domain,
					const MaxWalksatParams* const & params,
					const string& queryFile,
					const string& queryPredsStr,
					ostream& out) 
{
  LWInfo *lwinfo = new LWInfo(mln, domain);
  LazyWalksat *lw = new LazyWalksat(lwinfo, mwsLimit);
  int numSolutions = 1;
  bool includeUnsatSolutions = true;
  Array<Array<bool>*> solutions;
  Array<int> numBad;
    
  // run LazyWalksat
  lw->infer(params, numSolutions, includeUnsatSolutions,
  			solutions, numBad, true);
  
  bool good = true;
  if (solutions.size() == 0) 
  {
    cout << "LazySat was unable to find a solution." << endl;
    good = false;
  }
  else
  if (solutions.size() > 1) 
  {
    cout << "LazySat unexpectedly returned multiple solutions." << endl;
    good = false;
  }

  if (solutions.size() == 1 &&
      lwinfo->getVarCount() != solutions[0]->size()) 
  {
    cout << "Error in LazySat: lwinfo->getVarCount() != solutions[0]->size()." <<endl;
    good = false;
  }
 
  if (good)
  {  	
  	  //compute the fraction of hard clauses satisfied & the sum of their wts
    int numSatHard = 0, totalHard = 0;
    double satSoftWt = 0, totalWt = 0;
    const ClauseHashArray* clauses = mln->getClauses();
    for (int i = 0; i < clauses->size(); i++)
    {
      Clause* clause = (*clauses)[i];
      assert(clause->getNumUnknownGroundings(domain,domain->getDB(),true)==0);

      if (clause->isHardClause())
      {
        int h = (int) clause->getNumGroundings(domain);
        //totalWt += h * hardClauseWt;
        totalHard += h;
        double numSat = clause->getNumTrueGroundings(domain,domain->getDB(),
                                                     false);
        if (numSat > 0) 
        {
          numSatHard += int(numSat); 
        }
      }
      else
      {  // a soft clause
        totalWt += clause->getNumGroundings(domain) * clause->getWt();
        double numSat = clause->getNumTrueGroundings(domain, domain->getDB(),
                                                     false);
        if (numSat>0) satSoftWt += numSat*clause->getWt();
      }
    }

    if (totalHard > 0)
    {
      out 
        << "//Number of hard clauses satisfied = " 
        << numSatHard << " out of " << totalHard << endl;
    }
    else
      out << "//No hard clause." << endl;
    out << "//Weight of satisfied soft clauses  = " << satSoftWt 
               << endl;
    if (totalHard > 0)
      out << "//Weight of all satisfied clauses   = " 
                 << satSoftWt << endl;


	if (queryFile.length() > 0)
  	{
      cout <<"Reading query predicates that are specified in query file..."<<endl;
      bool ok = createQueryFilePreds(queryFile, domain, domain->getDB(), NULL,
      	                             NULL, true, out, amapPos);
      if (!ok) { cout <<"Failed to create query predicates." << endl; exit(-1); }
  	}

  	Array<int> allPredGndingsAreQueries;
  	allPredGndingsAreQueries.growToSize(domain->getNumPredicates(), false);
  	if (queryPredsStr.length() > 0)
  	{
      cout << "Creating query predicates that are specified on command line..." 
      	   << endl;
      bool ok = createComLineQueryPreds(queryPredsStr, domain, domain->getDB(), 
                                      	NULL, NULL, &allPredGndingsAreQueries,
                                      	true, out, amapPos);
      if (!ok) { cout <<"Failed to create query predicates." << endl; exit(-1); }
  	}
  }
  else
  {
  	exit(0);
  }  
}

/************************ END Functions Added by Parag ***************************/

int main(int argc, char* argv[])
{
  ///////////////////////////  read & prepare parameters ///////////////////////
  ARGS::parse(argc, argv, &cout);

  Timer timer;
  double begSec = timer.time(); 

  string inMLNFile, wkMLNFile, evidenceFile, queryPredsStr, queryFile;
  GibbsParams gibbsParams;
  StringHashArray queryPredNames;
  StringHashArray owPredNames;
  Domain* domain = NULL;
  MLN* mln = NULL;
  GroundPredicateHashArray queries;
  GroundPredicateHashArray knownQueries;
  Array<string> constFilesArr;
  Array<string> evidenceFilesArr;
  Array<string> funcFilesArr;

  //Parag
  Array<Predicate *> queryPreds;
  Array<TruthValue> queryPredValues;
  
  //the second .mln file to the last one in ainMLNFiles _may_ be used 
  //to hold constants, so they are held in constFilesArr. They will be
  //included into the first .mln file.

    //extract .mln, .db, .func file names
  extractFileNames(ainMLNFiles, constFilesArr);
  assert(constFilesArr.size() >= 1);
  inMLNFile.append(constFilesArr[0]);
  constFilesArr.removeItem(0);
  extractFileNames(aevidenceFiles, evidenceFilesArr);
  extractFileNames(afuncFiles, funcFilesArr);
  
  if (aqueryPredsStr) queryPredsStr.append(aqueryPredsStr);
  if (aqueryFile) queryFile.append(aqueryFile);

  if (queryPredsStr.length() == 0 && queryFile.length() == 0)
  { cout << "No query predicates specified" << endl; return -1; }

  ofstream resultsOut(aresultsFile);
  if (!resultsOut.good())
  { cout << "ERROR: unable to open " << aresultsFile << endl; return -1; }

  if (mcmcNumChains < 2) 
  { cout << "ERROR: there must be at least 2 MCMC chains" << endl; return -1; }

  if (!amapPos && !amapAll && !agibbsInfer)
  { cout << "ERROR: must specify one of -m/-a/-p options." << endl; return-1; }

  if (aclosedWorld && aopenWorldPredsStr)
  { cout << "ERROR: cannot specify both -c & -o together." << endl; return -1; }

    //extract names of all query predicates
  if (queryPredsStr.length() > 0 || queryFile.length() > 0)
  {
    if (!extractPredNames(queryPredsStr, &queryFile, queryPredNames)) return -1;
  }

  if (mwsMaxSteps <= 0)
  { cout << "ERROR: mwsMaxSteps must be positive" << endl; return -1; }

  if (mwsTries <= 0)
  { cout << "ERROR: mwsTries must be positive" << endl; return -1; }

    //extract names of closed-world predicates
  if (aopenWorldPredsStr)
  {
    if (!extractPredNames(string(aopenWorldPredsStr), NULL, owPredNames)) 
      return -1;
    assert(owPredNames.size() > 0);
    //if (!checkQueryPredsNotInClosedWorldPreds(queryPredNames, owPredNames))
    //return -1;
  }


  gibbsParams.numChains     = mcmcNumChains;
  gibbsParams.burnMinSteps  = mcmcBurnMinSteps;
  gibbsParams.burnMaxSteps  = mcmcBurnMaxSteps;
  gibbsParams.gamma         = 1-mcmcDelta;
  gibbsParams.epsilonFrac   = mcmcEpsilonError;
  gibbsParams.gibbsMinSteps = mcmcMinSteps;
  gibbsParams.gibbsMaxSteps = mcmcMaxSteps;
  gibbsParams.fracConverged = mcmcFracConverged;
  gibbsParams.walksatType   = (mcmcWalksatType==1) ? MAXWALKSAT : NONE;
  gibbsParams.maxSeconds    = mcmcMaxSeconds;
  gibbsParams.samplesPerConvergenceTest = mcmcSamplesPerTest;

  MaxWalksatParams* wsparams = NULL;
  wsparams = new MaxWalksatParams;
  wsparams->maxSteps = mwsMaxSteps;
  wsparams->tries = mwsTries;
  wsparams->targetWt = mwsTargetWt;
  wsparams->hard = mwsHard;
  wsparams->lazyGnded = lazywsGnded;
  wsparams->lazyNoApprox = lazyNoApprox;
  
  //Parag:
  if(mwsSeed == -1)
  {
    struct timeval tv;
    gettimeofday(&tv,0);
    mwsSeed = (( tv.tv_sec & 0177 ) * 1000000) + tv.tv_usec;
    cout<<"seed = "<<mwsSeed<<endl;
  }
  wsparams->seed = mwsSeed;
  
  //////////////////// read in clauses & evidence predicates //////////////////

  cout << "Reading formulas and evidence predicates..." << endl;

    //copy inMLNFile to workingMLNFile & app '#include "evid.db"'
  string::size_type bslash = inMLNFile.rfind("/");
  string tmp = (bslash == string::npos) ? 
               inMLNFile:inMLNFile.substr(bslash+1,inMLNFile.length()-bslash-1);
  char buf[100];
  sprintf(buf, "%s%s", tmp.c_str(), ZZ_TMP_FILE_POSTFIX);
  wkMLNFile = buf;
  copyFileAndAppendDbFile(inMLNFile, wkMLNFile, evidenceFilesArr, constFilesArr,
                          funcFilesArr);

    // parse wkMLNFile, and create the domain, MLN, database
  domain = new Domain;
  mln = new MLN();
  bool addUnitClauses = false;
  bool warnAboutDupGndPreds = true;
  bool mustHaveWtOrFullStop = true;
  bool allPredsExceptQueriesAreCW = aclosedWorld;
  Domain* forCheckingPlusTypes = NULL;

  if (!allPredsExceptQueriesAreCW && owPredNames.empty())
  {
    Array<string> pnames;
    domain->getNonEqualPredicateNames(pnames);
    for (int i = 0; i < pnames.size(); i++) owPredNames.append(pnames[i]);
  }

	// Evidence atoms are added during parsing, so need to know if lazy inference
  if (!runYYParser(mln, domain, wkMLNFile.c_str(), allPredsExceptQueriesAreCW, 
                   &owPredNames, &queryPredNames, addUnitClauses, 
                   warnAboutDupGndPreds, 0, mustHaveWtOrFullStop, 
                   forCheckingPlusTypes, mwsLazy)) 
  {
    unlink(wkMLNFile.c_str());
    return -1;
  }

  unlink(wkMLNFile.c_str());

  ///////////////////////// determine if MaxWalkSat can be instantiated /////////////////////	
  unsigned long pages;
  unsigned long bytesPerPage;
  unsigned long kilobytes;

  pages = sysconf(_SC_PHYS_PAGES);
  bytesPerPage = sysconf(_SC_PAGESIZE);
  kilobytes = pages*(bytesPerPage/1024);
	// If memory limit was given by user, use it
  if (mwsLimit == -1)
    mwsLimit = kilobytes;
    
  if ((amapPos || amapAll) && !mwsLazy)
  {
    long double numConstants = domain->getNumConstants();
    int highestArity = domain->getHighestPredicateArity();

    // Upper bound on memory needed = (size of predicate) * (constants ^ arity)
    long double memNeeded = pow(numConstants, highestArity)*
                        	(((double)(sizeof(Predicate) + sizeof(Array<Term*>) + sizeof(Array<int>) +
                        		   	   highestArity * (sizeof(Term*) + sizeof(int))))
                        	/ 1024.0);
	printf("Approximate memory needed for MaxWalkSat: %12lu kbytes\n", (unsigned long)memNeeded);      
	  
    if (memNeeded > mwsLimit)
    {
	  printf("Memory needed exceeds memory available (%d kbytes), using LazySat\n", mwsLimit);      
      mwsLazy = true;
    }
  }

  if(mwsLazy)
  {
	domain->getDB()->setPerformingInference(true);
  	runLazyWalksat(mln, domain, wsparams, queryFile, queryPredsStr, resultsOut);
  	exit(0);
  } 
    
  ///////////////////////// read & create query predicates /////////////////////
  if (queryFile.length() > 0)
  {
    cout <<"Reading query predicates that are specified in query file..."<<endl;
    bool ok = createQueryFilePreds(queryFile, domain, domain->getDB(), &queries,
                                   &knownQueries);
    if (!ok) { cout <<"Failed to create query predicates." << endl; exit(-1); }
  }

  Array<int> allPredGndingsAreQueries;
  allPredGndingsAreQueries.growToSize(domain->getNumPredicates(), false);
  if (queryPredsStr.length() > 0)
  {
    cout << "Creating query predicates that are specified on command line..." 
         << endl;
    bool ok = createComLineQueryPreds(queryPredsStr, domain, domain->getDB(), 
                                      &queries, &knownQueries, 
                                      &allPredGndingsAreQueries);
    if (!ok) { cout <<"Failed to create query predicates." << endl; exit(-1); }
  }

  //////////////////////////// run inference //////////////////////////////////

    // if performing MAP inference, mark those gnd clause that are hard ones
  bool markHardGndClauses = (!agibbsInfer) ? true : false;
  bool trackParentClauseWts = false;
  MRF mrf(&queries, &allPredGndingsAreQueries, domain, domain->getDB(), mln,
          markHardGndClauses, trackParentClauseWts);
  
  	//delete to save space. Can be deleted because no more hashing of
    //gndClauses are required beyond this point
  mrf.deleteGndClausesIntArrReps() ;
    //delete to save space. Can be deleted because no more gndClauses are
    //appended to gndPreds beyond this point
  mrf.deleteGndPredsGndClauseSets();
  //do not delete the intArrRep in gndPreds_;    

  if (agibbsInfer)
  {
	//Deletion of DB and MLN moved down because of verify results
    bool saveSpace = true;
    int numSamples = mrf.runGibbsSampling(gibbsParams, wsparams, saveSpace, domain);

	if(!averifyResults)
	{
      for (int i = 0; i < knownQueries.size(); i++)
      {
	      // Parag : print the real value as well probability
	    if(averifyResults)
	    {
          Predicate* p = knownQueries[i]->createEquivalentPredicate(domain);
          TruthValue tv = domain->getDB()->getValue(p);
       	  delete p;
          int val;
	      if(tv == TRUE)
		    val = 1;
	      else 
		    val = 0;
	  
	      resultsOut << " " << knownQueries[i]->getProbTrue() << " "<<val<<" ";
	      knownQueries[i]->print(resultsOut, domain); 
          resultsOut<<endl;
	  	}
	  	else
	  	{ 
	   	    //original version when results are not verified
       	  knownQueries[i]->print(resultsOut, domain); 
       	  resultsOut << " " << knownQueries[i]->getProbTrue() << endl;
	   	}
	   }
	}
	
    for (int i = 0; i < queries.size(); i++)
    {
	  // Parag : print the real value as well probability
	  if(averifyResults)
	  {
        Predicate* p = queries[i]->createEquivalentPredicate(domain);
        TruthValue tv = domain->getDB()->getValue(p);
        delete p;
	  
	    int val;
	    if(tv == TRUE)
		  val = 1;
	    else 
		  val = 0;
	  
	    double prob = queries[i]->getProbTrue();
        if (asmooth>0) prob = (prob*numSamples+asmooth/2.0)/(numSamples+asmooth);
	   
	    resultsOut << " " << prob << " "<<val<<" ";
        queries[i]->print(resultsOut, domain); 
        resultsOut<<endl;
	  }
	  else
	  {
	  	//original version when results are not verified
	  	double prob = queries[i]->getProbTrue();
      	if (asmooth>0) prob = (prob*numSamples+asmooth/2.0)/(numSamples+asmooth);
      	queries[i]->print(resultsOut, domain); resultsOut << " " << prob << endl;
	  }
    } 
	//Parag:
	cout << "deleting DB and MLN to save space since they are not needed by "
         << "Gibbs sampling..." << endl;
    domain->deleteDB();
    delete mln;
 
  }
  else
  {   //MAP inference
    assert(amapPos || amapAll);
    
    int numChain = 1;
    int chainIdx = 0;
    bool initIfMaxWalksatFails = false;
    double hardClauseWt;

    mrf.initGndPredsTruthValues(numChain);
    
	domain->getDB()->setPerformingInference(true);
    if (mrf.runMaxWalksat(chainIdx, initIfMaxWalksatFails, wsparams, hardClauseWt))
    {
        //add unknown predicates into DB
      const Array<GroundPredicate*>* unknownGndPreds = mrf.getGndPreds();
      for (int i = 0; i < unknownGndPreds->size(); i++)
      {
        Predicate* p = (*unknownGndPreds)[i]->createEquivalentPredicate(domain);
        TruthValue tv = (*unknownGndPreds)[i]->getTruthValue(chainIdx) ? 
                        TRUE : FALSE;
        domain->getDB()->setValue(p,tv);
        delete p;
      }

        //compute the fraction of hard clauses satisfied & the sum of their wts
      int numSatHard = 0, totalHard = 0;
      double satSoftWt = 0, satHardWt = 0, totalWt = 0;
      const ClauseHashArray* clauses = mln->getClauses();
      for (int i = 0; i < clauses->size(); i++)
      {
        Clause* clause = (*clauses)[i];
        assert(clause->getNumUnknownGroundings(domain,domain->getDB(),true)==0);

        if (clause->isHardClause())
        {
          int h = (int) clause->getNumGroundings(domain);
          totalWt += h * hardClauseWt;
          totalHard += h;
          double numSat = clause->getNumTrueGroundings(domain,domain->getDB(),
                                                       false);
          if (numSat > 0) 
          {
            numSatHard += int(numSat); 
            satHardWt += numSat*hardClauseWt;
          }
          //bool sat = clause->isSatisfiable(domain, domain->getDB(), true);
          //if (sat) { numSatHard++; satHardWt += hardClauseWt; }
        }
        else
        {  // a soft clause
          totalWt += clause->getNumGroundings(domain) * clause->getWt();
          double numSat = clause->getNumTrueGroundings(domain, domain->getDB(),
                                                       false);
          if (numSat>0) satSoftWt += numSat*clause->getWt();
          //bool sat = clause->isSatisfiable(domain, domain->getDB(), true);
          //if (sat) satSoftWt += clause->getWt();
        }
      }


      if (totalHard > 0)
      {
        resultsOut 
          << "//Number of hard clauses satisfied = " 
          << numSatHard << " out of " << totalHard << endl
          << "//Each hard clause is given a wt of " << hardClauseWt << endl
          << "//Weight of satisfied hard clauses  = " << satHardWt << endl;
      }
      else
        resultsOut << "//No hard clause." << endl;
      resultsOut << "//Weight of satisfied soft clauses  = " << satSoftWt 
                 << endl;
      if (totalHard > 0)
        resultsOut << "//Weight of all satisfied clauses   = " 
                   << satHardWt+satSoftWt << endl;
      //resultsOut << "//Weight of all (sat/unsat) clauses = " << totalWt<<endl;


      for (int i = 0; i < knownQueries.size(); i++)
      {
        if (amapPos) //if show postive ground query predicates only
        {
          if (knownQueries[i]->getProbTrue() == 1)
          {
            knownQueries[i]->print(resultsOut, domain); resultsOut << endl;
          }
        }
        else
        {   //print all ground query predicates
          knownQueries[i]->print(resultsOut, domain); 
          resultsOut << " " << ((knownQueries[i]->getProbTrue()==1)?1:0) <<endl;
        }
      }

      for (int i = 0; i < queries.size(); i++)
      {
        if (amapPos) //if show postive ground query predicates only
        {
          if (queries[i]->getTruthValue(chainIdx))
          {
            queries[i]->print(resultsOut, domain); resultsOut << endl;
          }
        }
        else
        { //print all ground query predicates
          queries[i]->print(resultsOut, domain); 
          resultsOut << (queries[i]->getTruthValue(chainIdx)?" 1":" 0") << endl;
        }
      }
    }// if MaxWalksat is successful
    delete mln;
    cout << "Add time taken by MaxWalksat to total time." << endl;
  }// else is MAP inference

  resultsOut.close();
  if (wsparams) delete wsparams;
  delete domain;
  for (int i = 0; i < knownQueries.size(); i++)  delete knownQueries[i];

  cout << "total time taken = "; Timer::printTime(cout, timer.time()-begSec);
  cout << endl;
}
