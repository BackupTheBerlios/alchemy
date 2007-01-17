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
#include <unistd.h>
#include <fstream>
#include <climits>
#include <sys/times.h>
#include "fol.h"
#include "arguments.h"
#include "util.h"
#include "learnwts.h"
#include "infer.h"
#include "inferenceargs.h"
#include "maxwalksat.h"
#include "mcsat.h"
#include "gibbssampler.h"
#include "simulatedtempering.h"

extern const char* ZZ_TMP_FILE_POSTFIX; //defined in fol.y

// Variables for holding inference command line args are in inferenceargs.h

char* aevidenceFiles  = NULL;
char* aresultsFile    = NULL;
char* aqueryPredsStr  = NULL;
char* aqueryFile      = NULL;

  // TODO: List the arguments common to learnwts and inference in
  // inferenceargs.h. This can't be done with a static array.
ARGS ARGS::Args[] = 
{
    // BEGIN: Common arguments
  ARGS("i", ARGS::Req, ainMLNFiles, 
       "Comma-separated input .mln files."),

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

    // BEGIN: Args specific to stand-alone inference
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
    // END: Args specific to stand-alone inference

  ARGS()
};


/**
 * Prints the results of inference to a stream.
 * 
 * @param queryFile File name containing the query predicates. This is only
 * used with lazy inference. If empty, this is not used.
 * @param query String of query predicates separated by commas without spaces.
 * This is only used with lazy inference. If empty, this is not used.
 * @param domain Domain in which the predicates exist.
 * @param out Stream to which the results are printed.
 * @param queries Queries already built from query file or string. This is only
 * used with eager inference.
 * @param inference Inference algorithm used which contains the results.
 * @param state VariableState used by the inference algorithm which contains
 * the results.
 */
void printResults(const string& queryFile, const string& queryPredsStr,
                  Domain *domain, ostream& out, 
                  GroundPredicateHashArray* const &queries,
                  Inference* const &inference, VariableState* const &state)
{
    // Lazy version: Have to generate the queries from the file or query string.
    // This involves calling createQueryFilePreds / createComLineQueryPreds
  if (aLazy)
  {
    const GroundPredicateHashArray* gndPredHashArray = NULL;
    Array<double>* gndPredProbs = NULL;
      // Inference algorithms with probs: have to retrieve this info from state.
      // These are the ground preds which have been brought into memory. All
      // others have always been false throughout sampling.
    if (!(amapPos || amapAll))
    {
      gndPredHashArray = state->getGndPredHashArrayPtr();
      gndPredProbs = new Array<double>;
      gndPredProbs->growToSize(gndPredHashArray->size());
      for (int i = 0; i < gndPredProbs->size(); i++)
        (*gndPredProbs)[i] =
          inference->getProbability((*gndPredHashArray)[i]);
    }
    
    if (queryFile.length() > 0)
    {
      cout << "Writing query predicates that are specified in query file..."
           << endl;
      bool ok = createQueryFilePreds(queryFile, domain, domain->getDB(), NULL,
                                     NULL, true, out, amapPos,
                                     gndPredHashArray, gndPredProbs);
      if (!ok) { cout <<"Failed to create query predicates."<< endl; exit(-1); }
    }

    Array<int> allPredGndingsAreQueries;
    allPredGndingsAreQueries.growToSize(domain->getNumPredicates(), false);
    if (queryPredsStr.length() > 0)
    {
      cout << "Writing query predicates that are specified on command line..." 
           << endl;
      bool ok = createComLineQueryPreds(queryPredsStr, domain, domain->getDB(), 
                                        NULL, NULL, &allPredGndingsAreQueries,
                                        true, out, amapPos, gndPredHashArray,
                                        gndPredProbs);
      if (!ok) { cout <<"Failed to create query predicates."<< endl; exit(-1); }
    }
    
    if (!(amapPos || amapAll))
      delete gndPredProbs;
  }
    // Eager version: Queries have already been generated and we can get the
    // information directly from the state
  else
  {
    if (amapPos)
      inference->printTruePreds(out);
    else
    {
      for (int i = 0; i < queries->size(); i++)
      {
          // Prob is smoothed in inference->getProbability
        double prob = inference->getProbability((*queries)[i]);
        (*queries)[i]->print(out, domain); out << " " << prob << endl;
      }
    }
  }
}


/**
 * The specified inference algorithm is run. First, the MLN and evidence files
 * are parsed and the database is filled. All evidence predicates are
 * closed-world by default (this can be changed with the -o option) and all
 * non-evidence predicates (query and hidden predicates) are open-world by
 * default (this can be changed with the -c option, however query atoms are
 * always open-world).
 */
int main(int argc, char* argv[])
{
  ///////////////////////////  read & prepare parameters ///////////////////////
  ARGS::parse(argc, argv, &cout);

  Timer timer;
  double begSec = timer.time(); 

  string inMLNFile, wkMLNFile, evidenceFile, queryPredsStr, queryFile;

  StringHashArray queryPredNames;
  StringHashArray owPredNames;
  StringHashArray cwPredNames;
  Domain* domain = NULL;
  MLN* mln = NULL;
  GroundPredicateHashArray queries;
  GroundPredicateHashArray knownQueries;
  Array<string> constFilesArr;
  Array<string> evidenceFilesArr;

  Array<Predicate *> queryPreds;
  Array<TruthValue> queryPredValues;
  
  //the second .mln file to the last one in ainMLNFiles _may_ be used 
  //to hold constants, so they are held in constFilesArr. They will be
  //included into the first .mln file.

    //extract .mln, .db file names
  extractFileNames(ainMLNFiles, constFilesArr);
  assert(constFilesArr.size() >= 1);
  inMLNFile.append(constFilesArr[0]);
  constFilesArr.removeItem(0);
  extractFileNames(aevidenceFiles, evidenceFilesArr);
  
  if (aqueryPredsStr) queryPredsStr.append(aqueryPredsStr);
  if (aqueryFile) queryFile.append(aqueryFile);

  if (queryPredsStr.length() == 0 && queryFile.length() == 0)
  { cout << "No query predicates specified" << endl; return -1; }

  ofstream resultsOut(aresultsFile);
  if (!resultsOut.good())
  { cout << "ERROR: unable to open " << aresultsFile << endl; return -1; }

  if (agibbsInfer && amcmcNumChains < 2) 
  {
    cout << "ERROR: there must be at least 2 MCMC chains in Gibbs sampling" 
         << endl; return -1;
  }

  if (!asimtpInfer && !amapPos && !amapAll && !agibbsInfer && !amcsatInfer)
  {
    cout << "ERROR: must specify one of -ms/-simtp/-m/-a/-p flags." << endl;
    return-1;
  }

    //extract names of all query predicates
  if (queryPredsStr.length() > 0 || queryFile.length() > 0)
  {
    if (!extractPredNames(queryPredsStr, &queryFile, queryPredNames)) return -1;
  }

  if (amwsMaxSteps <= 0)
  { cout << "ERROR: mwsMaxSteps must be positive" << endl; return -1; }

  if (amwsTries <= 0)
  { cout << "ERROR: mwsTries must be positive" << endl; return -1; }

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
    if (!checkQueryPredsNotInClosedWorldPreds(queryPredNames, cwPredNames))
      return -1;
  }

  // TODO: Check if query atom in -o -> error

  // TODO: Check if atom in -c and -o -> error


  // TODO: Check if ev. atom in -c or
  // non-evidence in -o -> warning (this is default)


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

  //////////////////// read in clauses & evidence predicates //////////////////

  cout << "Reading formulas and evidence predicates..." << endl;

    // Copy inMLNFile to workingMLNFile & app '#include "evid.db"'
  string::size_type bslash = inMLNFile.rfind("/");
  string tmp = (bslash == string::npos) ? 
               inMLNFile:inMLNFile.substr(bslash+1,inMLNFile.length()-bslash-1);
  char buf[100];
  sprintf(buf, "%s%s", tmp.c_str(), ZZ_TMP_FILE_POSTFIX);
  wkMLNFile = buf;
  copyFileAndAppendDbFile(inMLNFile, wkMLNFile,
                          evidenceFilesArr, constFilesArr);

    // Parse wkMLNFile, and create the domain, MLN, database
  domain = new Domain;
  mln = new MLN();
  bool addUnitClauses = false;
  bool mustHaveWtOrFullStop = true;
  bool warnAboutDupGndPreds = true;
  bool flipWtsOfFlippedClause = true;
  //bool allPredsExceptQueriesAreCW = true;
  bool allPredsExceptQueriesAreCW = owPredNames.empty();
  Domain* forCheckingPlusTypes = NULL;

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

    //////////////////////////// run inference /////////////////////////////////

    ///////////////////////// read & create query predicates ///////////////////
  Array<int> allPredGndingsAreQueries;

    // Eager inference: Build the queries for the mrf
    // Lazy version evaluates the query string / file when printing out
  if (!aLazy)
  {
    if (queryFile.length() > 0)
    {
      cout << "Reading query predicates that are specified in query file..."
           << endl;
      bool ok = createQueryFilePreds(queryFile, domain, domain->getDB(),
                                     &queries, &knownQueries);
      if (!ok) { cout<<"Failed to create query predicates."<<endl; exit(-1); }
    }

    allPredGndingsAreQueries.growToSize(domain->getNumPredicates(), false);
    if (queryPredsStr.length() > 0)
    {
      cout << "Creating query predicates that are specified on command line..." 
           << endl;
      bool ok = createComLineQueryPreds(queryPredsStr, domain, domain->getDB(), 
                                        &queries, &knownQueries, 
                                        &allPredGndingsAreQueries);
      if (!ok) { cout<<"Failed to create query predicates."<<endl; exit(-1); }
    }
  }

    // Create inference algorithm and state based on queries and mln / domain
  bool markHardGndClauses = false;
  bool trackParentClauseWts = false;
    // Lazy version: queries and allPredGndingsAreQueries are empty,
    // markHardGndClause and trackParentClauseWts are not used
  VariableState* state = new VariableState(&queries, NULL, NULL,
                                           &allPredGndingsAreQueries,
                                           markHardGndClauses,
                                           trackParentClauseWts,
                                           mln, domain, aLazy);
  Inference* inference = NULL;
  bool trackClauseTrueCnts = false;
    // MAP inference, MC-SAT, Gibbs or Sim. Temp.
  if (amapPos || amapAll || amcsatInfer || agibbsInfer || asimtpInfer)
  {
    if (amapPos || amapAll)
    { // MaxWalkSat
        // When standalone MWS, numSolutions is always 1
        // (maybe there is a better way to do this?)
      mwsparams->numSolutions = 1;
      inference = new MaxWalkSat(state, aSeed, trackClauseTrueCnts, mwsparams);
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

    inference->init();
    inference->infer();
    
    printResults(queryFile, queryPredsStr, domain, resultsOut, &queries,
                 inference, state);
  }

  resultsOut.close();
  if (mwsparams) delete mwsparams;
  if (ssparams) delete ssparams;
  if (msparams) delete msparams;
  if (gibbsparams) delete gibbsparams;
  if (stparams) delete stparams;
  delete domain;
  for (int i = 0; i < knownQueries.size(); i++)  delete knownQueries[i];
  delete inference;
  delete state;
  
  cout << "total time taken = "; Timer::printTime(cout, timer.time()-begSec);
  cout << endl;
}

