#include <fstream>
#include <iostream>
#include <sstream>
#include "arguments.h"
#include "pseudologlikelihood.h"
#include "lbfgsb.h"
//#include "lbfgsb-fortran.h"
#include "votedperceptron.h"
#include "learnwts.h"

  //set to false to disable printing of clauses when they are counted during 
  //generative learning
bool PRINT_CLAUSE_DURING_COUNT = true;

extern const char* ZZ_TMP_FILE_POSTFIX; //defined in fol.y
const double DISC_DEFAULT_STD_DEV = 1;
const double GEN_DEFAULT_STD_DEV = 100; //TODO: REVERT to 100?


bool discLearn = false;
bool genLearn = false;
char* inMLNFiles = NULL;
char* outMLNFile = NULL;
char* dbFiles = NULL;
char* funcFiles = NULL;
char* nonEvidPredsStr = NULL;
bool noAddUnitClauses = false;
bool multipleDatabases = false;

int numIter = 200;
double learningRate = 0.001;
int  mwsMaxSteps = 1000000;
int  mwsTries    = 1;
int  mwsLimit    = -1;
bool  mwsLazy    = false;

int maxIter = 10000;
double convThresh = 1e-5;
bool noEqualPredWt = false;
bool noPrior = false;
double priorMean = 0;
double priorStdDev = -1;


ARGS ARGS::Args[] = 
{
  ARGS("d", ARGS::Tog, discLearn, "Discriminative weight learning."),

  ARGS("g", ARGS::Tog, genLearn, "Generative weight learning."),

  ARGS("i", ARGS::Req, inMLNFiles, 
       "Comma-separated input .mln files. (With the -multipleDatabases "
       "option, the second file to the last one are used to contain constants "
       "from different databases, and they correspond to the .db files "
       "specified with the -t option.)"),

  ARGS("o", ARGS::Req, outMLNFile, 
       "Output .mln file containing formulas with learned weights."),

  ARGS("t", ARGS::Req, dbFiles, 
       "Comma-separated .db files containing the training database "
       "(of true/false ground atoms), including function definitions, "
       "e.g. ai.db,graphics.db,languages.db."),

  ARGS("ne", ARGS::Opt, nonEvidPredsStr, 
       "First-order non-evidence predicates (comma-separated with no space),  "
       "e.g., cancer,smokes,friends. For discriminative learning, at least "
       "one non-evidence predicate must be specified. For generative learning, "
       "the specified predicates are included in the (weighted) pseudo-log-"
       "likelihood computation; if none are specified, all are included."),
    
  ARGS("noAddUnitClauses", ARGS::Tog, noAddUnitClauses,
       "If specified, unit clauses are not included in the .mln file; "
       "otherwise they are included."),

  ARGS("multipleDatabases", ARGS::Tog, multipleDatabases,
       "If specified, each .db file belongs to a separate database; "
       "otherwise all .db files belong to the same database."),

  ARGS("dNumIter", ARGS::Opt, numIter, 
       "[200] (For discriminative learning only.) "
       "Number of iterations to run voted perceptron."),
  
  ARGS("dLearningRate", ARGS::Opt, learningRate, 
       "[0.001] (For discriminative learning only) "
       "Learning rate for the gradient descent in voted perceptron algorithm."),
       
  ARGS("mwsMaxSteps", ARGS::Opt, mwsMaxSteps,
       "[1000000] (For discriminative learning only.) " 
       "The max number of steps taken by MaxWalksat."),

  ARGS("mwsTries", ARGS::Opt, mwsTries, 
       "[1] (For discriminative learning only.) "
       "The max number of attempts MaxWalksat takes to find a solution."),

  ARGS("mwsLimit", ARGS::Opt, mwsLimit, 
       "[-1] (For discriminative learning only.) "
       "Maximum limit in kbytes which MaxWalksat should use for inference. "
	   "-1 means main memory available on system is used"),
  
  ARGS("mwsLazy", ARGS::Tog, mwsLazy, 
       "(For discriminative learning only.) "
       "Run lazy maxwalksat in inference step if this flag is set."),
  
  ARGS("gMaxIter", ARGS::Opt, maxIter, 
       "[10000] (For generative learning only.) "
       "Max number of iterations to run L-BFGS-B, "
       "the optimization algorithm for generative learning."),
  
  ARGS("gConvThresh", ARGS::Opt, convThresh, 
       "[1e-5] (For generative learning only.) "
       "Fractional change in pseudo-log-likelihood at which "
       "L-BFGS-B terminates."),

  ARGS("gNoEqualPredWt", ARGS::Opt, noEqualPredWt, 
       "(For generative learning only.) "
       "If specified, the predicates are not weighted equally in the "
       "pseudo-log-likelihood computation; otherwise they are."),
  
  ARGS("noPrior", ARGS::Tog, noPrior, "No Gaussian priors on formula weights."),

  ARGS("priorMean", ARGS::Opt, priorMean, 
       "[0] Means of Gaussian priors on formula weights. By default, "
       "for each formula, it is the weight given in the .mln input file, " 
       "or fraction thereof if the formula turns into multiple clauses. "
       "This mean applies if no weight is given in the .mln file."),

  ARGS("priorStdDev", ARGS::Opt, priorStdDev, 
       "[1 for discriminative learning. 100 for generative learning] "
       "Standard deviations of Gaussian priors on clause weights."),

  ARGS()
};


//bool extractPredNames(...) defined in infer.h


int main(int argc, char* argv[])
{
  ARGS::parse(argc,argv,&cout);

  if (!discLearn && !genLearn) 
  { 
    cout << "must specify either -d or -g "
         <<"(discriminative or generative learning) " << endl; 
    return -1; 
  }

  Timer timer;
  double startSec = getTimeSec(discLearn, timer);
  double begSec;

  if (priorStdDev < 0)
  {
    if (discLearn) 
    { 
      cout << "priorStdDev set to (discriminative learning's) default of " 
           << DISC_DEFAULT_STD_DEV << endl;
      priorStdDev = DISC_DEFAULT_STD_DEV;
    }
    else
    {
      cout << "priorStdDev set to (generative learning's) default of " 
           << GEN_DEFAULT_STD_DEV << endl;
      priorStdDev = GEN_DEFAULT_STD_DEV;      
    }
  }


  ///////////////////////// check and extract the parameters //////////////////
  if (discLearn && nonEvidPredsStr == NULL)
  {
    cout << "ERROR: you must specify non-evidence predicates for "
         << "discriminative learning" << endl;
    return -1;
  }

  if (maxIter <= 0)  { cout << "maxIter must be > 0" << endl; return -1; }
  if (convThresh <= 0 || convThresh > 1)  
  { cout << "convThresh must be > 0 and <= 1" << endl; return -1;  }
  if (priorStdDev <= 0) { cout << "priorStdDev must be > 0" << endl; return -1;}

  if (mwsMaxSteps <= 0)
  { cout << "ERROR: mwsMaxSteps must be positive" << endl; return -1; }

  if (mwsTries <= 0)
  { cout << "ERROR: mwsTries must be positive" << endl; return -1; }

  if (mwsLimit <= 0 && mwsLimit != -1)
  { cout << "ERROR: mwsLimit must be positive (or -1)" << endl; return -1; }

  if (!discLearn && mwsLazy)
  { cout << "ERROR: mwsLazy can only be used with discriminative learning" << endl; return -1; }

  ofstream out(outMLNFile);
  if (!out.good())
  {
    cout << "ERROR: unable to open " << outMLNFile << endl;
    return -1;
  }


  //the second .mln file to the last one in inMLNFiles _may_ be used 
  //to hold constants, so they are held in constFilesArr. They will be
  //included into the first .mln file.

    //extract .mln and .db, file names
  Array<string> constFilesArr;
  Array<string> dbFilesArr;
  Array<string> funcFilesArr;
  extractFileNames(inMLNFiles, constFilesArr);
  assert(constFilesArr.size() >= 1);
  string inMLNFile = constFilesArr[0];
  constFilesArr.removeItem(0);
  extractFileNames(dbFiles, dbFilesArr);
  extractFileNames(funcFiles, funcFilesArr);

  if (dbFilesArr.size() <= 0)
  { cout<<"ERROR: must specify training data with -t option."<<endl; return -1;}
 
    // if multiple databases, check the number of .db/.func files
  if (multipleDatabases) 
  {
      //if # .mln files containing constants/.func files and .db files are diff
    if ( (constFilesArr.size() > 0 && constFilesArr.size() != dbFilesArr.size()))
//         ||
//         (funcFilesArr.size() > 0 && funcFilesArr.size() != dbFilesArr.size()) )
    {
      cout << "ERROR: when there are multiple databases, if .mln files "
           << "containing constants are specified, there must " 
           << "be the same number of them as .db files" << endl;
      return -1;
    }
  }

  StringHashArray nonEvidPredNames;
  if (nonEvidPredsStr)
  {
    if(!extractPredNames(nonEvidPredsStr, NULL, nonEvidPredNames))
    {
      cout << "ERROR: failed to extract non-evidence predicate names." << endl;
      return -1;
    }
  }


  ////////////////////////// create domains and mlns ///////////////////////////

  cout << "Parsing MLN and creating domains..." << endl;
  StringHashArray* nePredNames = (discLearn) ? &nonEvidPredNames : NULL;
  Array<Domain*> domains;
  Array<MLN*> mlns;
  begSec = getTimeSec(discLearn, timer);
    // Parse as if lazy inference is set to true to set evidence atoms in DB
    // If lazy is not used, this is removed from DB
  createDomainsAndMLNs(domains, mlns, multipleDatabases, inMLNFile, 
                       constFilesArr, funcFilesArr, dbFilesArr, nePredNames,
                       !noAddUnitClauses, priorMean, true);  
  cout << "Parsing MLN and creating domains took "; 
  Timer::printTime(cout, getTimeSec(discLearn,timer) - begSec); cout << endl;

  /*
  cout << "Clause prior means:" << endl;
  cout << "_________________________________" << endl;
  mlns[0]->printClausePriorMeans(cout, domains[0]);
  cout << "_________________________________" << endl;
  cout << endl;

  cout << "Formula prior means:" << endl;
  cout << "_________________________________" << endl;
  mlns[0]->printFormulaPriorMeans(cout);
  cout << "_________________________________" << endl;
  cout << endl;
  */

  //////////////////// set the prior means & standard deviations //////////////

    //we need an index translator if clauses do not line up across multiple DBs
  IndexTranslator* indexTrans 
    = (IndexTranslator::needIndexTranslator(mlns, domains)) ?
       new IndexTranslator(&mlns, &domains) : NULL;  

  if (indexTrans) 
    cout << endl << "the weights of clauses in the CNFs of existential formulas"
         << " will be tied" << endl;


  Array<double> priorMeans, priorStdDevs;
  if (!noPrior)
  {
    if (indexTrans)
    {
      indexTrans->setPriorMeans(priorMeans);
      priorStdDevs.growToSize(priorMeans.size());
      for (int i = 0; i < priorMeans.size(); i++)
        priorStdDevs[i] = priorStdDev;
    }
    else
    {
      const ClauseHashArray* clauses = mlns[0]->getClauses();
      int numClauses = clauses->size();
      for (int i = 0; i < numClauses; i++)
      {
        priorMeans.append((*clauses)[i]->getWt());
        priorStdDevs.append(priorStdDev);
      }
    }
  }
  int numClausesFormulas = priorMeans.size();


  //////////////////////  discriminative/generative learning /////////////////
  Array<double> wts;

  if (discLearn) 
  {
    wts.growToSize(numClausesFormulas+1);
    double* wwts = (double*) wts.getItems();
    wwts++;

    int  mwsTargetWt = INT_MAX;
    bool mwsHard = false;
    VotedPerceptron vp(mlns, domains, nonEvidPredsStr, mwsMaxSteps, mwsTries, 
                       mwsTargetWt, mwsHard, indexTrans, mwsLimit, mwsLazy);
    if (!noPrior) 
      vp.setMeansStdDevs(numClausesFormulas, priorMeans.getItems(),
                         priorStdDevs.getItems());
    else
      vp.setMeansStdDevs(-1, NULL, NULL);
	 
    begSec = getTimeSec(discLearn, timer);
    cout << "learning (discriminative) weights .. " << endl;
    vp.learnWeights(wwts, wts.size()-1, numIter, learningRate);
    cout << endl << endl << "Done learning discriminative weights. "<< endl;
    cout << "Time Taken for learning = "; 
    Timer::printTime(cout, (getTimeSec(discLearn, timer)-begSec)); cout << endl;
  } 
  else
  {   
    ////////////// using generative learning

    Array<bool> areNonEvidPreds;
    if (nonEvidPredNames.empty())
    {
      areNonEvidPreds.growToSize(domains[0]->getNumPredicates(), true);
      for (int i = 0; i < domains[0]->getNumPredicates(); i++)
      {
          //prevent equal pred from being non-evidence preds
        if (domains[0]->getPredicateTemplate(i)->isEqualPred())
        {
          const char* pname = domains[0]->getPredicateTemplate(i)->getName();
          int predId = domains[0]->getPredicateId(pname);
          areNonEvidPreds[predId] = false;
        }
          //prevent internal preds from being non-evidence preds
        if (domains[0]->getPredicateTemplate(i)->isInternalPredicateTemplate())
        {
          const char* pname = domains[0]->getPredicateTemplate(i)->getName();
          int predId = domains[0]->getPredicateId(pname);
          areNonEvidPreds[predId] = false;
        }
      }
    } 
    else
    {
      areNonEvidPreds.growToSize(domains[0]->getNumPredicates(), false);
      for (int i = 0; i < nonEvidPredNames.size(); i++)
      {
        int predId = domains[0]->getPredicateId(nonEvidPredNames[i].c_str());
        if (predId < 0)
        {
          cout << "ERROR: Predicate " << nonEvidPredNames[i] << " undefined." 
               << endl;
          exit(-1);
        }
        areNonEvidPreds[predId] = true;
      }
    }

    Array<bool>* nePreds = &areNonEvidPreds;
    PseudoLogLikelihood pll(nePreds, &domains, !noEqualPredWt, false, -1,-1,-1);
    pll.setIndexTranslator(indexTrans); 

    if (!noPrior) 
      pll.setMeansStdDevs(numClausesFormulas, priorMeans.getItems(),
                          priorStdDevs.getItems());
    else          
      pll.setMeansStdDevs(-1, NULL, NULL);
    
    ////////////// compute the counts for the clauses

    begSec = getTimeSec(discLearn, timer);
    for (int m = 0; m < mlns.size(); m++)
    {
      cout << "Computing counts for clauses in domain " << m << "..." << endl;
      const ClauseHashArray* clauses = mlns[m]->getClauses();
      for (int i = 0; i < clauses->size(); i++)
      {
        if (PRINT_CLAUSE_DURING_COUNT)
        {
          cout << "clause " << i << ": ";
          (*clauses)[i]->printWithoutWt(cout, domains[m]);
          cout << endl; cout.flush();
        }
        MLNClauseInfo* ci = (MLNClauseInfo*) mlns[m]->getMLNClauseInfo(i);
        pll.computeCountsForNewAppendedClause((*clauses)[i], &(ci->index), m, 
                                              NULL, false, NULL);
      }
    }
    pll.compress();
    cout <<"Computing counts took ";
    Timer::printTime(cout, getTimeSec(discLearn, timer) - begSec); cout << endl;
    
    ////////////// learn clause wts

      // initialize the clause weights
    wts.growToSize(numClausesFormulas + 1);
    for (int i = 0; i < numClausesFormulas; i++) wts[i+1] = 0;
    //wts[i+1] = priorMeans[i];

      // optimize the clause weights
    cout << "L-BFGS-B is finding optimal weights......" << endl;
    begSec = getTimeSec(discLearn, timer);
    LBFGSB lbfgsb(maxIter, convThresh, &pll, numClausesFormulas);
    int iter;
    bool error;
    double pllValue = lbfgsb.minimize((double*)wts.getItems(), iter, error);
    
    if (error) cout << "LBFGSB returned with an error!" << endl;
    cout << "num iterations        = " << iter << endl;
    cout << "time taken            = ";
    Timer::printTime(cout, getTimeSec(discLearn, timer) - begSec);
    cout << endl;
    cout << "pseudo-log-likelihood = " << -pllValue << endl;

  } // else using generative learning


  //////////////////////////// output results ////////////////////////////////
  
  if (indexTrans) assignWtsAndOutputMLN(out, mlns, domains, wts, indexTrans);
  else            assignWtsAndOutputMLN(out, mlns, domains, wts);

  out.close();

  /////////////////////////////// clean up ///////////////////////////////////

  deleteDomains(domains);
  for (int i = 0; i < mlns.size(); i++) delete mlns[i];
  PowerSet::deletePowerSet();
  if (indexTrans) delete indexTrans;

  cout << "Total time = "; 
  Timer::printTime(cout, getTimeSec(discLearn, timer) - startSec); cout << endl;
}
