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

#include <sys/times.h>

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

bool  amcsat = false;	// mc-sat
bool  asimtp = false;	// simulated tempering
int hardWt = -1;		// soften hard clauses with the given wt if specified

int  mwsMaxSteps = 100000;
int  mwsTries    = 1;
int  mwsTargetWt = INT_MAX;
bool mwsHard     = false;
bool lazy = false;
int mwsSeed = -1;
// Limit in kbytes before using lazy version
int mwsLimit = -1;
bool lazywsGnded = false;
bool lazyNoApprox = false;

// MC-SAT params
int numStepsEveryMCSat = 1;	// number of total steps (mcsat & Gibbs) for each MC-SAT step
int numsol = 10;		// the nth solution to return
int ws_noise = 15;		// ws noise
int ws_restart = 10;	// ws restart = numtry
int ws_cutoff = 100000;	// ws cutoff
int saRatio = 50;		// percent of SA steps
int temperature = 50;	// temperature/100: SA temperature
bool latesa = false;	// sa only at a plateur

// Simulated Tempering params
int subInterval = 2;
int numST = 3;
int numSwap = 10;

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
  
  ARGS("ms", ARGS::Tog, amcsat,
       "Run inference using MC-SAT and return probabilities "
       "for all query atoms"),

  ARGS("simtp", ARGS::Tog, asimtp,
       "Run inference using simulated tempering and return probabilities "
       "for all query atoms"),

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
  
//  ARGS("mwsSeed", ARGS::Opt, mwsSeed, 
//       "[-1] Seed for random number generator used in MaxWalksat. "
//	   "-1 means it is intialized randomly"),
  
  ARGS("mwsLimit", ARGS::Opt, mwsLimit, 
       "[-1] Maximum limit in kbytes which MaxWalksat should use for inference. "
	   "-1 means main memory available on system is used"),
  
  //ARGS("gnded", ARGS::Tog, lazywsGnded, 
  //     "Run the grounded version of lazy maxwalksat if this flag is set. This flag "
  //     "is ignored if -lazy is not set."),
  
  ARGS("lazy", ARGS::Tog, lazy, 
       "Run lazy version of MAP inference / MC-SAT if this flag is set."),
  
  ARGS("lazyNoApprox", ARGS::Tog, lazyNoApprox, 
       "LazySat will not approximate by deactivating atoms to save memory. This flag "
	   "is ignored if -lazy is not set."),
  
  ARGS("mcmcSmooth", ARGS::Opt, asmooth, 
       "[1] Number of samples to use for smoothing."),

  //ARGS("hardwt", ARGS::Opt, hardWt,
  //     "[-1] soften hard clause with the given weight"),

  ARGS("simtpSubInterval", ARGS::Opt, subInterval,
		"[2] Selection interval btw swap attempts"),

  ARGS("simtpNumRuns", ARGS::Opt, numST,
		"[3] Number of sim. tempering runs"),

  ARGS("simtpNumSwap", ARGS::Opt, numSwap,
		"[10] Number of swapping chains"),

  ARGS("numStepsEveryMCSat", ARGS::Opt, numStepsEveryMCSat,
       "[1] Number of total steps (mcsat & gibbs) for every mcsat step"),

  ARGS("mcsatSaRatio", ARGS::Opt, saRatio,
       "[50] Ratio of sim. annealing steps mixed with WalkSAT in MC-SAT"),

  ARGS("mcsatSaTemperature", ARGS::Opt, temperature,
		"[10] Temperature (/100) for sim. annealing"),

  ARGS("mcsatLateSa", ARGS::Tog, latesa,
       "[false] Run sim. annealing from the start in samplesat"),

  ARGS("mcsatNumSolutions", ARGS::Opt, numsol,
       "[10] Return nth SAT solution in samplesat"),

  //ARGS("wsnoise", ARGS::Opt, ws_noise,
  //     "[15] WS noise level"),

  //ARGS("wsrestart", ARGS::Opt, ws_restart,
  //     "[10] WS restart"),

  //ARGS("wscutoff", ARGS::Opt, ws_cutoff,
  //     "[100000] WS cutoff"),

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

  if (lwinfo->getVarCount() != solutions[0]->size())
  {
    cout << "Error in LazySat: lwinfo->getVarCount() != solutions[0]->size()." <<endl;
    good = false;
  }

  delete lwinfo;
  delete lw;
 
  if (good)
  {
  	  //compute the fraction of hard clauses satisfied & the sum of their wts
    int numSatHard = 0, totalHard = 0;
    double satSoftWt = 0, satHardWt = 0, totalWt = 0;
    const ClauseHashArray* clauses = mln->getClauses();
    double hardClauseWt = LWInfo::HARD_WT;
    
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
        << numSatHard << " out of " << totalHard << endl
        << "//Each hard clause is given a wt of " << hardClauseWt << endl
        << "//Weight of satisfied hard clauses  = " << satHardWt << endl;
    }
    else
      out << "//No hard clause." << endl;
    out << "//Weight of satisfied soft clauses  = " << satSoftWt 
               << endl;
    if (totalHard > 0)
      out << "//Weight of all satisfied clauses   = " 
                 << satHardWt+satSoftWt << endl;

	if (queryFile.length() > 0)
  	{
      cout <<"Reading query predicates that are specified in query file..."<<endl;
      bool ok = createQueryFilePreds(queryFile, domain, domain->getDB(), NULL,
      	                             NULL, true, out, amapPos, NULL);
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
                                      	true, out, amapPos, NULL);
      if (!ok) { cout <<"Failed to create query predicates." << endl; exit(-1); }
  	}
  }
  return;
}


/* Lazy version of MC-SAT */
int runLazyMCSatSampling(MLN *mln, Domain *domain,
					     const MCSatParams& mcsatParams,
					     const MaxWalksatParams* const & mwsparams,
                         const string& queryFile,
                         const string& queryPredsStr,
                         ostream& out, StringHashArray& predNames)
//                         GroundPredicateHashArray* const & queries) 
{
  struct timeval tv;
  struct timezone tzp;
  gettimeofday(&tv,&tzp);
  long int seed = (long int)((( tv.tv_sec & 0177 ) * 1000000) + tv.tv_usec);
  Random random;
  random.init(-seed);
  
  Timer timer;
  double begSec;

    // extract MCSat sampling parameters
  bool showDebug = mcsatParams.showDebug;
  const SampleSatParams sampleSatParams = mcsatParams.sampleSatParams;
  const GibbsParams gibbsParams = mcsatParams.gibbsParams;

  int samplesPerConvergenceTest = gibbsParams.samplesPerConvergenceTest;
  int burnMaxSteps = gibbsParams.burnMaxSteps;
  int gibbsMaxSteps = gibbsParams.gibbsMaxSteps;
  int maxSeconds = gibbsParams.maxSeconds;
    
  	// We need just one chain for mcsat is supposed
	// to be able to jump around diff modes.
  int numChains = 1;

  begSec = timer.time();

    // Ids of query preds
  Array<int> queryPredIds;
  for (int i = 0; i < predNames.size(); i++)
  {
    int id = domain->getPredicateId(predNames[i].c_str());
    assert(id >= 0);
    queryPredIds.append(id);
  }

    // Ground preds set to true sometime during sampling
  GroundPredicateHashArray predsSetToTrue;

	// Do an initial assignment for each chain
  for (int c = 0; c < numChains; c++)
  {
	  // Initial assignment satisfies hard clauses
    if (mln->getNumHardClauses() > 0)
  	{
	  	// run LazyWalksat on hard clauses
  	  LWInfo *lwinfo = new LWInfo(mln, domain);
  	  lwinfo->removeSoftClauses();
  	  LazyWalksat *lw = new LazyWalksat(lwinfo, mwsLimit);
  	  int numSolutions = 1;
  	  bool includeUnsatSolutions = true;
  	  Array<Array<bool>*> solutions;
  	  Array<int> numBad;
    
  	  lw->infer(mwsparams, numSolutions, includeUnsatSolutions,
  			    solutions, numBad, true);
  	  	// Assignment which satisfies hard clauses is now in DB
	  delete lwinfo;
	  delete lw;
	
	  if (solutions.size() == 0)
  	  {
      	cout << "LazySat was unable to find a solution for hard clauses." << endl;
  	  }
  	  else
  	  {
  	  	cout << "Initial assignment based on LazySat solution" << endl;
  	  }
  	  
  	  	// Delete solutions array
  	  for (int i = 0; i < solutions.size(); i++)  
        if (solutions[i]) delete solutions[i];
      solutions.clearAndCompress();
  	  
  	}
  	else
  	{ // No hard clauses -> no assignment
      cout << "No hard clauses." << endl;
  	}

  }// Initial assignment for each chain

  int numSamplesPerPred = 0, isamp = 0;
  bool done      = false;
  bool burningIn = true;

  cout << "running MC-SAT sampling..." << endl;

	// whether to show time stat etc.
  showDebug = false;
  //showDebug = true;
  double* times = new double[3]; int ti;	  

  double secondsElapsed = 0;
  double startTimeSec, currTimeSec;
  startTimeSec = timer.time();

  	// -- sampling loop
  while(!done) 
  {
    ++isamp;

      // for each chain, run lazy samplesat
    for (int c = 0; c < numChains; c++)
	{   // numchain==1
	  if (showDebug) {ti = 0; times[ti++] = timer.time();}

  	  LWInfo *lwinfo = new LWInfo(mln, domain);
  	  LazyWalksat *lw = new LazyWalksat(lwinfo, mwsLimit);
      lwinfo->setSampleSat(true);
      lwinfo->setPrevDB();
      lwinfo->initFixedAtoms();

		// call samplesat
	  if (lw->sample(mwsparams, sampleSatParams))
      {
          // Assume first that gnd pred was set to false
        for (int i = 0; i < predsSetToTrue.size(); i++)
          predsSetToTrue[i]->setTruthValue(c, false);

  		  // Assignment is now in DB
        for (int i = 0; i < queryPredIds.size(); i++)
        {
          const PredicateHashArray* truePreds =
            domain->getDB()->getTrueNonEvidenceGndPreds(queryPredIds[i]);

          for (int j = 0; j < truePreds->size(); j++)
          {
            GroundPredicate* gpred = new GroundPredicate((*truePreds)[j]);
            int found = predsSetToTrue.find(gpred);
            if (found == -1)
            {
                // First time set to true
              found = predsSetToTrue.append(gpred);
                //initialize query preds' truthValues & wts
              predsSetToTrue[found]->initTruthValuesAndWts(numChains);
            }
            else
              delete gpred;
            
              // No need to update weight but still need to update truth/NumSat
            predsSetToTrue[found]->setTruthValue(c, true);

              // if in actual sampling phase, track the num of
              // times the ground predicate is set to true                      
            if (!burningIn) predsSetToTrue[found]->incrementNumTrue();
            delete (*truePreds)[j];
          }
          delete truePreds;
        }
      }
	  else
	  {
		cout <<"[" << isamp << "." << c << ": " << 
			   "] No solution found!"<<endl;

	      // need to count as well, otherwise undersample hard problems
	  	if (!burningIn)
		  for (int i = 0; i < predsSetToTrue.size(); i++) 
		  	if (predsSetToTrue[i]->getTruthValue(c))
			  predsSetToTrue[i]->incrementNumTrue();
	  }
	  lwinfo->setAllInactive();
	  delete lwinfo;
	  delete lw;

	  if (showDebug) times[ti++] = timer.time();
            
	  if (showDebug)
	  {
		ti = 0;
		cout<<"["<<isamp<<"."<<c<<"]"<<endl;
		cout<<"\tSampleSat: "<<(times[ti+1]-times[ti])<<" secs"<<endl; ti++;
	  }
		// stat
	  if (!burningIn) numSamplesPerPred++;

	} // For each chain

	currTimeSec = timer.time();
	secondsElapsed = currTimeSec-startTimeSec;

    if (isamp % samplesPerConvergenceTest == 0)
    { 
      cout << "Sample (per chain) " << isamp << ", time elapsed = " 
           << secondsElapsed << " sec" << endl;
    }

	  //if (isamp % samplesPerConvergenceTest != 0) continue;      
    if (burningIn) 
    {
	  if (isamp >= burnMaxSteps)
	  {
        cout << "Done burning. " << isamp << " samples per chain "<<endl;
        burningIn = false;
        isamp = 0;
	  }
    }
    else 
    {
	  if ( (maxSeconds > 0 && secondsElapsed >= maxSeconds) ||
	  	   (maxSeconds <= 0 && gibbsMaxSteps >= 0 && isamp >= gibbsMaxSteps) )
      {
        cout << "Done MC-SAT sampling. " << isamp << " samples per chain"				
             << endl;
        done = true;
      }
    }
    cout.flush();
  } // while (!done);
  
  delete [] times;
  
    // update queries probability that it is true
  for (int i = 0; i < predsSetToTrue.size(); i++)
    predsSetToTrue[i]->setProbTrue(predsSetToTrue[i]->getNumTrue() / numSamplesPerPred);

  cout << "Ground predicates set to true: " << predsSetToTrue.size() << endl;

  if (queryFile.length() > 0)
  {
    cout << "Writing probabilities of query predicates that are specified in query file..."
         << endl;
    bool ok = createQueryFilePreds(queryFile, domain, domain->getDB(), NULL,
                                   NULL, true, out, false, &predsSetToTrue);
    if (!ok) { cout <<"Failed to write probabilities of query predicates." << endl; exit(-1); }
  }

  Array<int> allPredGndingsAreQueries;
  allPredGndingsAreQueries.growToSize(domain->getNumPredicates(), false);
  if (queryPredsStr.length() > 0)
  {
    cout << "Writing probabilities of query predicates that are specified on command line..." 
         << endl;
    bool ok = createComLineQueryPreds(queryPredsStr, domain, domain->getDB(), 
                                      NULL, NULL, &allPredGndingsAreQueries,
                                      true, out, false, &predsSetToTrue);
    if (!ok) { cout <<"Failed to write probabilities of query predicates." << endl; exit(-1); }
  }

  return numSamplesPerPred;
}

int main(int argc, char* argv[])
{
  ///////////////////////////  read & prepare parameters ///////////////////////
  ARGS::parse(argc, argv, &cout);

  Timer timer;
  double begSec = timer.time(); 

  // Init random w. different number each time
  struct timeval tv;
  struct timezone tzp;
  gettimeofday(&tv,&tzp);
  long int seed = (long int)((( tv.tv_sec & 0177 ) * 1000000) + tv.tv_usec);
  Random random;
  random.init(-seed);

  string inMLNFile, wkMLNFile, evidenceFile, queryPredsStr, queryFile;

  MCSatParams mcSatParams;
  GibbsParams gibbsParams;
  SampleSatParams sampleSatParams;
  TemperingParams temperingParams;

  StringHashArray queryPredNames;
  StringHashArray owPredNames;
  Domain* domain = NULL;
  MLN* mln = NULL;
  GroundPredicateHashArray queries;
  GroundPredicateHashArray knownQueries;
  Array<string> constFilesArr;
  Array<string> evidenceFilesArr;
  Array<string> funcFilesArr;

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

  if (!asimtp && !amapPos && !amapAll && !agibbsInfer && !amcsat)
  { cout << "ERROR: must specify one of -ms/-simtp/-m/-a/-p flags." << endl; return-1; }

  if (lazy && !(amapPos || amapAll || amcsat))
  {
  	cout << "ERROR: -lazy can only be used with MAP inference "
  	  		"or MC-SAT (-m, -a, or -ms option)." << endl;
  	return-1;
  }

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
  gibbsParams.hardWt = hardWt;

  sampleSatParams.saRatio = saRatio;
  sampleSatParams.temperature = temperature;
  sampleSatParams.latesa = latesa;
  sampleSatParams.numsol = numsol;
  sampleSatParams.ws_noise = ws_noise;
  sampleSatParams.ws_restart = ws_restart;
  sampleSatParams.ws_cutoff = ws_cutoff;

  mcSatParams.gibbsParams = gibbsParams;
  mcSatParams.sampleSatParams = sampleSatParams;
  mcSatParams.numStepsEveryMCSat = numStepsEveryMCSat;
  mcSatParams.showDebug = false;

  temperingParams.subInterval = subInterval;
  temperingParams.numST = numST;
  temperingParams.numSwap = numSwap;
  temperingParams.gibbsParams = gibbsParams;

  MaxWalksatParams* wsparams = NULL;
  wsparams = new MaxWalksatParams;
  wsparams->maxSteps = mwsMaxSteps;
  wsparams->tries = mwsTries;
  wsparams->targetWt = mwsTargetWt;
  wsparams->hard = mwsHard;
  wsparams->lazyGnded = lazywsGnded;
  wsparams->lazyNoApprox = lazyNoApprox;
  
  /*
  if(mwsSeed == -1)
  {
    struct timeval tv;
    gettimeofday(&tv,0);
    mwsSeed = (( tv.tv_sec & 0177 ) * 1000000) + tv.tv_usec;
    //cout<<"seed = "<<mwsSeed<<endl;
  }
  wsparams->seed = mwsSeed;
  */
  
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
  bool flipWtsOfFlippedClause = true;
  bool allPredsExceptQueriesAreCW = aclosedWorld;
  Domain* forCheckingPlusTypes = NULL;

  if (!allPredsExceptQueriesAreCW && owPredNames.empty())
  {
    Array<string> pnames;
    domain->getNonEqualPredicateNames(pnames);
    for (int i = 0; i < pnames.size(); i++) owPredNames.append(pnames[i]);
  }

	// Parse as if lazy inference is set to true to set evidence atoms in DB
    // If lazy is not used, this is removed from DB
  if (!runYYParser(mln, domain, wkMLNFile.c_str(), allPredsExceptQueriesAreCW, 
                   &owPredNames, &queryPredNames, addUnitClauses, 
                   warnAboutDupGndPreds, 0, mustHaveWtOrFullStop, 
                   forCheckingPlusTypes, true, flipWtsOfFlippedClause))
  {
    unlink(wkMLNFile.c_str());
    return -1;
  }

  unlink(wkMLNFile.c_str());

  //////////////////////////// run inference //////////////////////////////////

    // do not mark hard clauses for gibbs and simulated tempering
  bool markHardGndClauses = (!agibbsInfer && !asimtp) ? true : false;
  bool trackParentClauseWts = false;
  MRF* mrf = NULL;
  
    // MAP inference or MC-SAT (types of inference for which a lazy version exists)
  if (amapPos || amapAll || amcsat) 
  {
	  ///////////////////////// determine if MaxWalkSat can be instantiated /////////////////////	
  	unsigned long pages;
  	unsigned long bytesPerPage;
  	unsigned long kilobytes;

  	pages = sysconf(_SC_PHYS_PAGES);
  	bytesPerPage = sysconf(_SC_PAGESIZE);
  	kilobytes = pages*(bytesPerPage/1024);
	  // If memory limit was given by user, use it
	  // Otherwise take 70% of memory
  	if (mwsLimit == -1)
      mwsLimit = (int)(0.7*kilobytes);

  	if (!lazy)
  	{
		// See if all clauses can fit in memory
	  try
  	  {
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

  	  	mrf = new MRF(&queries, &allPredGndingsAreQueries, domain, domain->getDB(), mln,
        		  	  markHardGndClauses, trackParentClauseWts, mwsLimit);
  
  	  	  //delete to save space. Can be deleted because no more hashing of
      	  //gndClauses are required beyond this point
  	  	mrf->deleteGndClausesIntArrReps();
      	  //delete to save space. Can be deleted because no more gndClauses are
      	  //appended to gndPreds beyond this point
  	  	mrf->deleteGndPredsGndClauseSets();
  	  	  //do not delete the intArrRep in gndPreds_;    
      	  
      	  //Remove evidence atoms structure from DB
      	domain->getDB()->setLazyFlag(false);
  	  }
  	  catch (int e)
  	  {
  	  	cout << "Memory required for all ground clauses exceeds memory available (" <<
  	  	mwsLimit << " kbytes)" << endl;
		if (amcsat)
  	  	  cout << "Using memory-efficient MC-SAT" << endl;
		if (amapPos || amapAll)
  	  	  cout << "Using memory-efficient MAP inference (LazySat)" << endl;
	  	delete mrf;
	  	lazy = true;
  	  }
  	}
  }
  else
  {
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

  	mrf = new MRF(&queries, &allPredGndingsAreQueries, domain, domain->getDB(), mln,
        	  		markHardGndClauses, trackParentClauseWts, -1);
  
  	  //delete to save space. Can be deleted because no more hashing of
      //gndClauses are required beyond this point
  	mrf->deleteGndClausesIntArrReps();
      //delete to save space. Can be deleted because no more gndClauses are
      //appended to gndPreds beyond this point
  	mrf->deleteGndPredsGndClauseSets();
  	  //do not delete the intArrRep in gndPreds_;
  }
  

  if (amcsat)
  {
  	if (lazy)
  	{
        // Lazy version of MC-SAT
      domain->getDB()->setPerformingInference(true);
      wsparams->targetWt = 0;
  	  int numSamples = runLazyMCSatSampling(mln, domain, mcSatParams, wsparams, queryFile,
                                            queryPredsStr, resultsOut, queryPredNames);
	  cout << "Lazy MC-SAT: Number of samples = "<< numSamples << endl;
  	}
  	else
  	{
        // Non-lazy version of MC-SAT
	  	// not needed by mc-sat sampling and deleted to save space
      cout << "deleting DB and MLN to save space since they are not needed by "
           << "MC-Sat sampling..." << endl;
      domain->deleteDB();
      delete mln;

      bool saveSpace = true;
	  mrf->runMCSatSampling(mcSatParams, wsparams, saveSpace, domain);
  	}

//MS: Known queries?
    for (int i = 0; i < queries.size(); i++)
    {
   	  double prob = queries[i]->getProbTrue();

	  	// Uniform smoothing
      	//if (asmooth>0) prob = (prob*numSamples+asmooth/2.0)/(numSamples+asmooth);
	  prob = (prob*10000+1/2.0)/(10000+1.0);
      queries[i]->print(resultsOut, domain); resultsOut << " " << prob << endl;
    }
  }
  else if (asimtp)
  {
	  // not needed by simulated tempering sampling and deleted to save space
    cout << "deleting DB and MLN to save space since they are not needed by "
         << "Simulated Tempering sampling..." << endl;
    domain->deleteDB();
    delete mln;

    bool saveSpace = true;
	mrf->runTemperingSampling(temperingParams, wsparams, saveSpace, domain);

//MS: Known queries?
    for (int i = 0; i < queries.size(); i++)
    {
      double prob = queries[i]->getProbTrue();

        // Uniform smoothing
        //if (asmooth>0) prob = (prob*numSamples+asmooth/2.0)/(numSamples+asmooth);
	  prob = (prob*10000+1/2.0)/(10000+1.0);

      queries[i]->print(resultsOut, domain); resultsOut << " " << prob << endl;
    }
  }
  else if (agibbsInfer)
  {
      // not needed by gibbs sampling and deleted to save space
    cout << "deleting DB and MLN to save space since they are not needed by "
         << "Gibbs sampling..." << endl;
    domain->deleteDB();
    delete mln;

    bool saveSpace = true;
    mrf->runGibbsSampling(gibbsParams, wsparams, saveSpace, domain);

    for (int i = 0; i < knownQueries.size(); i++)
    {
	  knownQueries[i]->print(resultsOut, domain); 
      resultsOut << " " << knownQueries[i]->getProbTrue() << endl;
	}
	
    for (int i = 0; i < queries.size(); i++)
    {
	  double prob = queries[i]->getProbTrue();

        // Uniform smoothing
        //if (asmooth>0) prob = (prob*numSamples+asmooth/2.0)/(numSamples+asmooth);
	  prob = (prob*10000+1/2.0)/(10000+1.0);

      queries[i]->print(resultsOut, domain); resultsOut << " " << prob << endl;
	}
  } 
  else
  {   //MAP inference
    assert(amapPos || amapAll);
    domain->getDB()->setPerformingInference(true);
    
	if(lazy)
  	{
  	  runLazyWalksat(mln, domain, wsparams, queryFile, queryPredsStr, resultsOut);
  	}
    else
    {
      int numChain = 1;
      int chainIdx = 0;
      bool initIfMaxWalksatFails = false;
      double hardClauseWt;

      mrf->initGndPredsTruthValues(numChain);
    
      if (mrf->runMaxWalksat(chainIdx, initIfMaxWalksatFails, wsparams, hardClauseWt))
      {
          //add unknown predicates into DB
        const Array<GroundPredicate*>* unknownGndPreds = mrf->getGndPreds();
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
    }
  }// else is MAP inference

  resultsOut.close();
  if (wsparams) delete wsparams;
  delete domain;
  for (int i = 0; i < knownQueries.size(); i++)  delete knownQueries[i];

  cout << "total time taken = "; Timer::printTime(cout, timer.time()-begSec);
  cout << endl;
}
