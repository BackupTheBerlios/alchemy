/*
 * All of the documentation and software included in the
 * Alchemy Software is copyrighted by Stanley Kok, Parag
 * Singla, Matthew Richardson, Pedro Domingos, Marc
 * Sumner and Hoifung Poon.
 * 
 * Copyright [2004-07] Stanley Kok, Parag Singla, Matthew
 * Richardson, Pedro Domingos, Marc Sumner and Hoifung
 * Poon. All rights reserved.
 * 
 * Contact: Pedro Domingos, University of Washington
 * (pedrod@cs.washington.edu).
 * 
 * Redistribution and use in source and binary forms, with
 * or without modification, are permitted provided that
 * the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above
 * copyright notice, this list of conditions and the
 * following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the
 * above copyright notice, this list of conditions and the
 * following disclaimer in the documentation and/or other
 * materials provided with the distribution.
 * 
 * 3. All advertising materials mentioning features or use
 * of this software must display the following
 * acknowledgment: "This product includes software
 * developed by Stanley Kok, Parag Singla, Matthew
 * Richardson, Pedro Domingos, Marc Sumner and Hoifung
 * Poon in the Department of Computer Science and
 * Engineering at the University of Washington".
 * 
 * 4. Your publications acknowledge the use or
 * contribution made by the Software to your research
 * using the following citation(s): 
 * Stanley Kok, Parag Singla, Matthew Richardson and
 * Pedro Domingos (2005). "The Alchemy System for
 * Statistical Relational AI", Technical Report,
 * Department of Computer Science and Engineering,
 * University of Washington, Seattle, WA.
 * http://www.cs.washington.edu/ai/alchemy.
 * 
 * 5. Neither the name of the University of Washington nor
 * the names of its contributors may be used to endorse or
 * promote products derived from this software without
 * specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE UNIVERSITY OF WASHINGTON
 * AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE UNIVERSITY
 * OF WASHINGTON OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */
#include <fstream>
#include <iostream>
#include <sstream>
#include "arguments.h"
#include "inferenceargs.h"
#include "lbfgsb.h"
#include "votedperceptron.h"
#include "learnwts.h"
#include "maxwalksat.h"
#include "mcsat.h"
#include "gibbssampler.h"
#include "simulatedtempering.h"

  //set to false to disable printing of clauses when they are counted during 
  //generative learning
bool PRINT_CLAUSE_DURING_COUNT = true;

const double DISC_DEFAULT_STD_DEV = 1;
const double GEN_DEFAULT_STD_DEV = 100;

  // Variables for holding inference command line args are in inferenceargs.h
bool discLearn = false;
bool genLearn = false;
char* outMLNFile = NULL;
char* dbFiles = NULL;
char* nonEvidPredsStr = NULL;
bool noAddUnitClauses = false;
bool multipleDatabases = false;
bool initToZero = false;
bool isQueryEvidence = false;

bool noPrior = false;
double priorMean = 0;
double priorStdDev = -1;

  // Generative learning args
int maxIter = 10000;
double convThresh = 1e-5;
bool noEqualPredWt = false;

  // Discriminative learning args
int numIter = 200;
double learningRate = 0.001;
double momentum = 0.0;
bool rescaleGradient = false;
bool withEM = false;
char* aInferStr = NULL;


  // Inference arguments needed for disc. learning defined in inferenceargs.h
  // TODO: List the arguments common to learnwts and inference in
  // inferenceargs.h. This can't be done with a static array.
ARGS ARGS::Args[] = 
{
    // BEGIN: Common arguments
  ARGS("i", ARGS::Req, ainMLNFiles, 
       "Comma-separated input .mln files. (With the -multipleDatabases "
       "option, the second file to the last one are used to contain constants "
       "from different databases, and they correspond to the .db files "
       "specified with the -t option.)"),

  ARGS("cw", ARGS::Opt, aClosedWorldPredsStr,
       "Specified non-evidence atoms (comma-separated with no space) are "
       "closed world, otherwise, all non-evidence atoms are open world. Atoms "
       "appearing here cannot be query atoms and cannot appear in the -o "
       "option."),

  ARGS("ow", ARGS::Opt, aOpenWorldPredsStr,
       "Specified evidence atoms (comma-separated with no space) are open "
       "world, while other evidence atoms are closed-world. "
       "Atoms appearing here cannot appear in the -c option."),
    // END: Common arguments

    // BEGIN: Common inference arguments
  ARGS("m", ARGS::Tog, amapPos, 
       "Run MAP inference and return only positive query atoms."),

  ARGS("a", ARGS::Tog, amapAll, 
       "Run MAP inference and show 0/1 results for all query atoms."),

  ARGS("p", ARGS::Tog, agibbsInfer, 
       "Run inference using MCMC (Gibbs sampling) and return probabilities "
       "for all query atoms."),
  
  ARGS("ms", ARGS::Tog, amcsatInfer,
       "Run inference using MC-SAT and return probabilities "
       "for all query atoms"),

  ARGS("simtp", ARGS::Tog, asimtpInfer,
       "Run inference using simulated tempering and return probabilities "
       "for all query atoms"),

  ARGS("seed", ARGS::Opt, aSeed,
       "[random] Seed used to initialize the randomizer in the inference "
       "algorithm. If not set, seed is initialized from the current date and "
       "time."),

  ARGS("lazy", ARGS::Opt, aLazy, 
       "[false] Run lazy version of inference if this flag is set."),
  
  ARGS("lazyNoApprox", ARGS::Opt, aLazyNoApprox, 
       "[false] Lazy version of inference will not approximate by deactivating "
       "atoms to save memory. This flag is ignored if -lazy is not set."),
  
  ARGS("memLimit", ARGS::Opt, aMemLimit, 
       "[-1] Maximum limit in kbytes which should be used for inference. "
       "-1 means main memory available on system is used."),
    // END: Common inference arguments

    // BEGIN: MaxWalkSat args
  ARGS("mwsMaxSteps", ARGS::Opt, amwsMaxSteps,
       "[1000000] (MaxWalkSat) The max number of steps taken."),

  ARGS("tries", ARGS::Opt, amwsTries, 
       "[1] (MaxWalkSat) The max number of attempts taken to find a solution."),

  ARGS("targetWt", ARGS::Opt, amwsTargetWt,
       "[the best possible] (MaxWalkSat) MaxWalkSat tries to find a solution "
       "with weight <= specified weight."),

  ARGS("hard", ARGS::Opt, amwsHard, 
       "[false] (MaxWalkSat) MaxWalkSat never breaks a hard clause in order to "
       "satisfy a soft one."),
  
  ARGS("heuristic", ARGS::Opt, amwsHeuristic,
       "[1] (MaxWalkSat) Heuristic used in MaxWalkSat (0 = RANDOM, 1 = BEST, "
       "2 = TABU, 3 = SAMPLESAT)."),
  
  ARGS("tabuLength", ARGS::Opt, amwsTabuLength,
       "[5] (MaxWalkSat) Minimum number of flips between flipping the same "
       "atom when using the tabu heuristic in MaxWalkSat." ),

  ARGS("lazyLowState", ARGS::Opt, amwsLazyLowState, 
       "[false] (MaxWalkSat) If false, the naive way of saving low states "
       "(each time a low state is found, the whole state is saved) is used; "
       "otherwise, a list of variables flipped since the last low state is "
       "kept and the low state is reconstructed. This can be much faster for "
       "very large data sets."),  
    // END: MaxWalkSat args

    // BEGIN: MCMC args
  ARGS("burnMinSteps", ARGS::Opt, amcmcBurnMinSteps,
       "[100] (MCMC) Minimun number of burn in steps (-1: no minimum)."),

  ARGS("burnMaxSteps", ARGS::Opt, amcmcBurnMaxSteps,
       "[100] (MCMC) Maximum number of burn-in steps (-1: no maximum)."),

  ARGS("minSteps", ARGS::Opt, amcmcMinSteps, 
       "[-1] (MCMC) Minimum number of Gibbs sampling steps."),

  ARGS("maxSteps", ARGS::Opt, amcmcMaxSteps, 
       "[1000] (MCMC) Maximum number of Gibbs sampling steps."),

  ARGS("maxSeconds", ARGS::Opt, amcmcMaxSeconds, 
       "[-1] (MCMC) Max number of seconds to run MCMC (-1: no maximum)."),
    // END: MCMC args
  
    // BEGIN: Simulated tempering args
  ARGS("subInterval", ARGS::Opt, asimtpSubInterval,
        "[2] (Simulated Tempering) Selection interval between swap attempts"),

  ARGS("numRuns", ARGS::Opt, asimtpNumST,
        "[3] (Simulated Tempering) Number of simulated tempering runs"),

  ARGS("numSwap", ARGS::Opt, asimtpNumSwap,
        "[10] (Simulated Tempering) Number of swapping chains"),
    // END: Simulated tempering args

    // BEGIN: MC-SAT args
  ARGS("numStepsEveryMCSat", ARGS::Opt, amcsatNumStepsEveryMCSat,
       "[1] (MC-SAT) Number of total steps (mcsat + gibbs) for every mcsat "
       "step"),
    // END: MC-SAT args

    // BEGIN: SampleSat args
  ARGS("numSolutions", ARGS::Opt, amwsNumSolutions,
       "[10] (MC-SAT) Return nth SAT solution in SampleSat"),

  ARGS("saRatio", ARGS::Opt, assSaRatio,
       "[50] (MC-SAT) Ratio of sim. annealing steps mixed with WalkSAT in "
       "MC-SAT"),

  ARGS("saTemperature", ARGS::Opt, assSaTemp,
        "[10] (MC-SAT) Temperature (/100) for sim. annealing step in "
        "SampleSat"),

  ARGS("lateSa", ARGS::Tog, assLateSa,
       "[false] Run simulated annealing from the start in SampleSat"),
    // END: SampleSat args

    // BEGIN: Gibbs sampling args
  ARGS("numChains", ARGS::Opt, amcmcNumChains, 
       "[10] (Gibbs) Number of MCMC chains for Gibbs sampling (there must be "
       "at least 2)."),

  ARGS("delta", ARGS::Opt, agibbsDelta,
       "[0.05] (Gibbs) During Gibbs sampling, probabilty that epsilon error is "
       "exceeded is less than this value."),

  ARGS("epsilonError", ARGS::Opt, agibbsEpsilonError,
       "[0.01] (Gibbs) Fractional error from true probability."),

  ARGS("fracConverged", ARGS::Opt, agibbsFracConverged, 
       "[0.95] (Gibbs) Fraction of ground atoms with probabilities that "
       "have converged."),

  ARGS("walksatType", ARGS::Opt, agibbsWalksatType, 
       "[1] (Gibbs) Use Max Walksat to initialize ground atoms' truth values "
       "in Gibbs sampling (1: use Max Walksat, 0: random initialization)."),

  ARGS("samplesPerTest", ARGS::Opt, agibbsSamplesPerTest, 
       "[100] Perform convergence test once after this many number of samples "
       "per chain."),
    // END: Gibbs sampling args

    // BEGIN: Weight learning specific args
  ARGS("infer", ARGS::Opt, aInferStr,
       "Specified inference parameters when using discriminative learning. "
       "The arguments are to be encapsulated in \"\" and the syntax is "
       "identical to the infer command (run infer with no commands to see "
       "this). If not specified, MaxWalkSat with default parameters is used."),

  ARGS("d", ARGS::Tog, discLearn, "Discriminative weight learning."),

  ARGS("g", ARGS::Tog, genLearn, "Generative weight learning."),

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

  ARGS("withEM", ARGS::Tog, withEM,
       "If set, EM is used to fill in missing truth values; "
       "otherwise missing truth values are set to false."),

  ARGS("dNumIter", ARGS::Opt, numIter, 
       "[200] (For discriminative learning only.) "
       "Number of iterations to run voted perceptron."),
  
  ARGS("dLearningRate", ARGS::Opt, learningRate, 
       "[0.001] (For discriminative learning only) "
       "Learning rate for the gradient descent in voted perceptron algorithm."),

  ARGS("dMomentum", ARGS::Opt, momentum, 
       "[0.0] (For discriminative learning only) "
       "Momentum term for the gradient descent in voted perceptron algorithm."),
       
  ARGS("queryEvidence", ARGS::Tog, isQueryEvidence, 
       "If this flag is set, then all the groundings of query preds not in db "
       "are assumed false evidence."),
       
  ARGS("dRescale", ARGS::Tog, rescaleGradient, 
       "(For discriminative learning only.) "
       "Rescale the gradient by the number of true groundings per weight."),

  ARGS("dZeroInit", ARGS::Tog, initToZero,
       "(For discriminative learning only.) "
       "Initialize clause weights to zero instead of their log odds."),
  
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
  double startSec = timer.time();
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

  if (amwsMaxSteps <= 0)
  { cout << "ERROR: maxSteps must be positive" << endl; return -1; }

  if (amwsTries <= 0)
  { cout << "ERROR: tries must be positive" << endl; return -1; }

  if (aMemLimit <= 0 && aMemLimit != -1)
  { cout << "ERROR: limit must be positive (or -1)" << endl; return -1; }

  if (!discLearn && aLazy)
  {
    cout << "ERROR: lazy can only be used with discriminative learning"
         << endl;
    return -1;
  }

  ofstream out(outMLNFile);
  if (!out.good())
  {
    cout << "ERROR: unable to open " << outMLNFile << endl;
    return -1;
  }

    // Parse the inference parameters, if given
  if (discLearn)
  {
      // If no inference method indicated, then use MAP
    if (!aInferStr)
    {
      amapPos = true;
    }
      // If inference method given, we need to parse the parameters
    else
    {
      int inferArgc = 0;
      char **inferArgv = new char*[200];
      for (int i = 0; i < 200; i++)
      {
        inferArgv[i] = new char[30];
      }

      extractArgs(aInferStr, inferArgc, inferArgv);
      cout << "extractArgs " << inferArgc << endl;
      for (int i = 0; i < inferArgc; i++)
      {
        cout << i << ": " << inferArgv[i] << endl;
      }

      ARGS::parseFromCommandLine(inferArgc, inferArgv);

        // HACK: Argument parser doesn't parse the ARGS::Tog right, so do
        // it here by hand
      for (int i = 0; i < inferArgc; i++)
      {
        if (string(inferArgv[i]) == "-m") amapPos = true;
        else if (string(inferArgv[i]) == "-a") amapAll = true;
        else if (string(inferArgv[i]) == "-p") agibbsInfer = true;
        else if (string(inferArgv[i]) == "-ms") amcsatInfer = true;
        else if (string(inferArgv[i]) == "-simtp") asimtpInfer = true;
      }

        // Delete memory allocated for args
      for (int i = 0; i < 200; i++)
      {
        delete[] inferArgv[i];
      }
      delete[] inferArgv; 
    }
  }


  //the second .mln file to the last one in ainMLNFiles _may_ be used 
  //to hold constants, so they are held in constFilesArr. They will be
  //included into the first .mln file.

    //extract .mln and .db, file names
  Array<string> constFilesArr;
  Array<string> dbFilesArr;
  extractFileNames(ainMLNFiles, constFilesArr);
  assert(constFilesArr.size() >= 1);
  string inMLNFile = constFilesArr[0];
  constFilesArr.removeItem(0);
  extractFileNames(dbFiles, dbFilesArr);

  if (dbFilesArr.size() <= 0)
  {cout<<"ERROR: must specify training data with -t option."<<endl; return -1;}
 
    // if multiple databases, check the number of .db/.func files
  if (multipleDatabases) 
  {
      //if # .mln files containing constants/.func files and .db files are diff
    if ((constFilesArr.size() > 0 && constFilesArr.size() != dbFilesArr.size()))
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

  StringHashArray owPredNames;
  StringHashArray cwPredNames;

  ////////////////////////// create domains and mlns //////////////////////////

  cout << "Parsing MLN and creating domains..." << endl;
  StringHashArray* nePredNames = (discLearn) ? &nonEvidPredNames : NULL;
  Array<Domain*> domains;
  Array<MLN*> mlns;
  begSec = timer.time();
  bool allPredsExceptQueriesAreCW = true;
  if (discLearn)
  {
      //extract names of open-world evidence predicates
    if (aOpenWorldPredsStr)
    {
      if (!extractPredNames(string(aOpenWorldPredsStr), NULL, owPredNames)) 
        return -1;
      assert(owPredNames.size() > 0);
    }

      //extract names of closed-world non-evidence predicates
    if (aClosedWorldPredsStr)
    {
      if (!extractPredNames(string(aClosedWorldPredsStr), NULL, cwPredNames)) 
        return -1;
      assert(cwPredNames.size() > 0);
      if (!checkQueryPredsNotInClosedWorldPreds(nonEvidPredNames, cwPredNames))
        return -1;
    }
 
    allPredsExceptQueriesAreCW = owPredNames.empty();
  }
    // Parse as if lazy inference is set to true to set evidence atoms in DB
    // If lazy is not used, this is removed from DB
  createDomainsAndMLNs(domains, mlns, multipleDatabases, inMLNFile, 
                       constFilesArr, dbFilesArr, nePredNames,
                       !noAddUnitClauses, priorMean, true,
                       allPredsExceptQueriesAreCW, &owPredNames);
  cout << "Parsing MLN and creating domains took "; 
  Timer::printTime(cout, timer.time() - begSec); cout << endl;

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
    cout << endl << "the weights of clauses in the CNFs of existential"
         << " formulas will be tied" << endl;


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
  // HACK -- not sure if this is right... but the old version was broke!
  // This may fail when there's an indexTrans.  [Daniel]
  //int numClausesFormulas = priorMeans.size();
  int numClausesFormulas = mlns[0]->getClauses()->size();


  //////////////////////  discriminative/generative learning /////////////////
  Array<double> wts;

    // Discriminative learning
  if (discLearn) 
  {
    wts.growToSize(numClausesFormulas + 1);
    double* wwts = (double*) wts.getItems();
    wwts++;
      // Non-evid preds as a string
    string nePredsStr = nonEvidPredsStr;

      // Set SampleSat parameters
    SampleSatParams* ssparams = new SampleSatParams;
    ssparams->lateSa = assLateSa;
    ssparams->saRatio = assSaRatio;
    ssparams->saTemp = assSaTemp;

      // Set MaxWalksat parameters
    MaxWalksatParams* mwsparams = NULL;
    mwsparams = new MaxWalksatParams;
    mwsparams->ssParams = ssparams;
    mwsparams->maxSteps = amwsMaxSteps;
    mwsparams->maxTries = amwsTries;
    mwsparams->targetCost = amwsTargetWt;
    mwsparams->hard = amwsHard;
      // numSolutions only applies when used in SampleSat.
      // When just MWS, this is set to 1
    mwsparams->numSolutions = amwsNumSolutions;
    mwsparams->heuristic = amwsHeuristic;
    mwsparams->tabuLength = amwsTabuLength;
    mwsparams->lazyLowState = amwsLazyLowState;

      // Set MC-SAT parameters
    MCSatParams* msparams = new MCSatParams;
    msparams->mwsParams = mwsparams;
      // MC-SAT needs only one chain
    msparams->numChains          = 1;
    msparams->burnMinSteps       = amcmcBurnMinSteps;
    msparams->burnMaxSteps       = amcmcBurnMaxSteps;
    msparams->minSteps           = amcmcMinSteps;
    msparams->maxSteps           = amcmcMaxSteps;
    msparams->maxSeconds         = amcmcMaxSeconds;
    msparams->numStepsEveryMCSat = amcsatNumStepsEveryMCSat;

      // Set Gibbs parameters
    GibbsParams* gibbsparams = new GibbsParams;
    gibbsparams->mwsParams    = mwsparams;
    gibbsparams->numChains    = amcmcNumChains;
    gibbsparams->burnMinSteps = amcmcBurnMinSteps;
    gibbsparams->burnMaxSteps = amcmcBurnMaxSteps;
    gibbsparams->minSteps     = amcmcMinSteps;
    gibbsparams->maxSteps     = amcmcMaxSteps;
    gibbsparams->maxSeconds   = amcmcMaxSeconds;

    gibbsparams->gamma          = 1 - agibbsDelta;
    gibbsparams->epsilonError   = agibbsEpsilonError;
    gibbsparams->fracConverged  = agibbsFracConverged;
    gibbsparams->walksatType    = agibbsWalksatType;
    gibbsparams->samplesPerTest = agibbsSamplesPerTest;
  
      // Set Sim. Tempering parameters
    SimulatedTemperingParams* stparams = new SimulatedTemperingParams;
    stparams->mwsParams    = mwsparams;
    stparams->numChains    = amcmcNumChains;
    stparams->burnMinSteps = amcmcBurnMinSteps;
    stparams->burnMaxSteps = amcmcBurnMaxSteps;
    stparams->minSteps     = amcmcMinSteps;
    stparams->maxSteps     = amcmcMaxSteps;
    stparams->maxSeconds   = amcmcMaxSeconds;

    stparams->subInterval = asimtpSubInterval;
    stparams->numST       = asimtpNumST;
    stparams->numSwap     = asimtpNumSwap;

    Array<VariableState*> states;
    Array<Inference*> inferences;

    states.growToSize(domains.size());
    inferences.growToSize(domains.size());

      // Build the state for inference in each domain
    Array<int> allPredGndingsAreNonEvid;
    Array<Predicate*> ppreds;
    
    for (int i = 0; i < domains.size(); i++)
    {
      Domain* domain = domains[i];
      MLN* mln = mlns[i];

        // Remove evidence atoms structure from DBs
      if (!aLazy)
        domains[i]->getDB()->setLazyFlag(false);
    
        // Unknown non-ev. preds
      GroundPredicateHashArray* unePreds = NULL;

        // Known non-ev. preds
      GroundPredicateHashArray* knePreds = NULL;
      Array<TruthValue>* knePredValues = NULL;

        // Need to set some dummy weight
      for (int j = 0; j < mln->getNumClauses(); j++)
        ((Clause*) mln->getClause(j))->setWt(1);

        // Find all evidence predicates with unknown value
        // and make these non-evidence
      int numPreds = domain->getNumPredicates();
      for (int j = 0; j < numPreds; j++)
      {
        const char* predName = domain->getPredicateName(j);
          // Only look at evidence (but not '=' predicate)
        if (nonEvidPredNames.contains(predName) ||
            domain->getPredicateTemplate(j)->isEqualPredicateTemplate())
          continue;
          
        bool unknownPred = false;
        ppreds.clear();
        Predicate::createAllGroundings(j, domain, ppreds);
        for (int k = 0; k < ppreds.size(); k++)
        {
          TruthValue tv = domain->getDB()->getValue(ppreds[k]);
          if (tv == UNKNOWN)
            unknownPred = true;
          delete ppreds[k];
        }
        if (unknownPred)
        {
          nePredsStr.append(",");
          nePredsStr.append(predName);
          nonEvidPredNames.append(predName);
        }
      }

        // Eager version: Build query preds from command line and set known
        // non-evidence to unknown for building the states
      if (!aLazy)
      {
        unePreds = new GroundPredicateHashArray;
        knePreds = new GroundPredicateHashArray;
        knePredValues = new Array<TruthValue>;

        allPredGndingsAreNonEvid.growToSize(domain->getNumPredicates(), false);
          //defined in infer.h
        createComLineQueryPreds(nePredsStr, domain, domain->getDB(), 
                                unePreds, knePreds, 
                                &allPredGndingsAreNonEvid);

          // Pred values not set to unknown in DB: unePreds contains
          // unknown, knePreds contains known non-evidence

          // Set known NE to unknown for building state
        knePredValues->growToSize(knePreds->size(), FALSE);
        for (int predno = 0; predno < knePreds->size(); predno++) 
          (*knePredValues)[predno] =
            domain->getDB()->setValue((*knePreds)[predno], UNKNOWN);

          // If first order query pred groundings are allowed to be evidence
          // - we assume all the predicates not in db to be false
          // evidence - need a better way code this.
        if (isQueryEvidence)
            // Set unknown NE to false
          for (int predno = 0; predno < unePreds->size(); predno++) 
            domain->getDB()->setValue((*unePreds)[predno], FALSE);
      }
      
        // Create state for inferred counts using unknown and known (set to
        // unknown in the DB) non-evidence preds
      cout << endl << "constructing state for domain " << i << "..." << endl;
      bool markHardGndClauses = false;
      bool trackParentClauseWts = true;
      VariableState*& state = states[i];
      state = new VariableState(unePreds, knePreds, knePredValues,
                                &allPredGndingsAreNonEvid, markHardGndClauses,
                                trackParentClauseWts, mln, domain, aLazy);

      Inference*& inference = inferences[i];
      bool trackClauseTrueCnts = true;
        // Different inference algorithms
      if (amapPos || amapAll)
      { // MaxWalkSat
          // When standalone MWS, numSolutions is always 1
          // (maybe there is a better way to do this?)
        mwsparams->numSolutions = 1;
        inference = new MaxWalkSat(state, aSeed, trackClauseTrueCnts,
                                   mwsparams);
      }
      else if (amcsatInfer)
      { // MC-SAT
        inference = new MCSAT(state, aSeed, trackClauseTrueCnts, msparams);
      }
      else if (asimtpInfer)
      { // Simulated Tempering
          // When MWS is used in Sim. Temp., numSolutions is always 1
          // (maybe there is a better way to do this?)
        mwsparams->numSolutions = 1;
        inference = new SimulatedTempering(state, aSeed, trackClauseTrueCnts,
                                           stparams);
      }
      else if (agibbsInfer)
      { // Gibbs sampling
          // When MWS is used in Gibbs, numSolutions is always 1
          // (maybe there is a better way to do this?)
        mwsparams->numSolutions = 1;
        inference = new GibbsSampler(state, aSeed, trackClauseTrueCnts,
                                     gibbsparams);
      }

      if (!aLazy)
      {
          // Change known NE to original values
        domain->getDB()->setValuesToGivenValues(knePreds, knePredValues);
          // Set unknown NE to false for weight initialization. This seems to
          // give poor results when using EM. We need to leave these
          // as unknown and do the counts accordingly
        for (int predno = 0; predno < unePreds->size(); predno++)
        {
          domain->getDB()->setValue((*unePreds)[predno], FALSE);
        }
      }
    }
    cout << endl << "done constructing variable states" << endl << endl;
    
    VotedPerceptron vp(inferences, nonEvidPredNames, indexTrans, aLazy,
                       rescaleGradient, withEM);
    if (!noPrior) 
      vp.setMeansStdDevs(numClausesFormulas, priorMeans.getItems(),
                         priorStdDevs.getItems());
    else
      vp.setMeansStdDevs(-1, NULL, NULL);
	 
    begSec = timer.time();
    cout << "learning (discriminative) weights .. " << endl;
    vp.learnWeights(wwts, wts.size()-1, numIter, learningRate, momentum,
                    !initToZero);
    cout << endl << endl << "Done learning discriminative weights. "<< endl;
    cout << "Time Taken for learning = ";
    Timer::printTime(cout, (timer.time() - begSec)); cout << endl;

    if (mwsparams) delete mwsparams;
    if (ssparams) delete ssparams;
    if (msparams) delete msparams;
    if (gibbsparams) delete gibbsparams;
    if (stparams) delete stparams;
    for (int i = 0; i < inferences.size(); i++)  delete inferences[i];
    for (int i = 0; i < states.size(); i++)  delete states[i];
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
    PseudoLogLikelihood pll(nePreds, &domains, !noEqualPredWt, false,-1,-1,-1);
    pll.setIndexTranslator(indexTrans);

    if (!noPrior) 
      pll.setMeansStdDevs(numClausesFormulas, priorMeans.getItems(),
                          priorStdDevs.getItems());
    else          
      pll.setMeansStdDevs(-1, NULL, NULL);
    
    ////////////// compute the counts for the clauses

    begSec = timer.time();
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
    Timer::printTime(cout, timer.time() - begSec); cout << endl;
    
    ////////////// learn clause wts

      // initialize the clause weights
    wts.growToSize(numClausesFormulas + 1);
    for (int i = 0; i < numClausesFormulas; i++) wts[i+1] = 0;
    //wts[i+1] = priorMeans[i];

      // optimize the clause weights
    cout << "L-BFGS-B is finding optimal weights......" << endl;
    begSec = timer.time();
    LBFGSB lbfgsb(maxIter, convThresh, &pll, numClausesFormulas);
    int iter;
    bool error;
    double pllValue = lbfgsb.minimize((double*)wts.getItems(), iter, error);
    
    if (error) cout << "LBFGSB returned with an error!" << endl;
    cout << "num iterations        = " << iter << endl;
    cout << "time taken            = ";
    Timer::printTime(cout, timer.time() - begSec);
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
  Timer::printTime(cout, timer.time() - startSec); cout << endl;
}
