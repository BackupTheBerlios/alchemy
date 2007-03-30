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
#ifndef MCSAT_H_
#define MCSAT_H_

#include "mcmc.h"
#include "mcsatparams.h"
#include "unitpropagation.h"
#include "maxwalksat.h"

const int msdebug = false;

/**
 * MC-SAT is an MCMC inference algorithm designed to deal efficiently with
 * probabilistic and deterministic dependencies (See Poon and Domingos, 2006).
 * It wraps a procedure around SampleSat, thus enabling it to sample nearly
 * uniform.
 */
class MCSAT : public MCMC
{
 public:

  /**
   * Constructor: Constructs unit propagation and SampleSat.
   */  
  MCSAT(VariableState* state, long int seed, const bool& trackClauseTrueCnts,
        MCSatParams* mcsatParams)
    : MCMC(state, seed, trackClauseTrueCnts, mcsatParams)
  {
    numStepsEveryMCSat_ = mcsatParams->numStepsEveryMCSat;
      // We don't need to track clause true counts in up and ss
    up_ = new UnitPropagation(state_, seed, false);
    mws_ = new MaxWalkSat(state_, seed, false, mcsatParams->mwsParams);
  }

  /**
   * Destructor: Instances of unit propagation and MaxWalksat are deleted.
   */
  ~MCSAT()
  {
    delete up_;
    delete mws_;
  }
  
  /**
   * Initializes MC-SAT with MaxWalksat on hard clauses.
   */
  void init()
  {
    assert(numChains_ == 1);

    cout << "Initializing MC-SAT with MaxWalksat on hard clauses..." << endl;
    state_->eliminateSoftClauses();
      // Set num. of solutions temporarily to 1
    int numSolutions = mws_->getNumSolutions();
    mws_->setNumSolutions(1);

      // Initialize with MWS
    mws_->init();
    mws_->infer();

    if (msdebug) 
    {
      cout << "Low state:" << endl;
      state_->printLowState(cout);
    }
    state_->saveLowStateToGndPreds();

      // Set heuristic to SampleSat (Initialization could have been different)
    mws_->setHeuristic(SS);
    mws_->setNumSolutions(numSolutions);
    mws_->setTargetCost(0.0);
    state_->resetDeadClauses();
    state_->makeUnitCosts();
  }

  /**
   * Performs MC-SAT inference.
   */
  void infer()
  {
    initNumTrue();
    Timer timer;
      // Burn-in only if burnMaxSteps positive
    bool burningIn = (burnMaxSteps_ > 0) ? true : false;
    double secondsElapsed = 0;
    upSecondsElapsed_ = 0;
    ssSecondsElapsed_ = 0;
    double startTimeSec = timer.time();
    double currentTimeSec;
    int samplesPerOutput = 100;

      // If keeping track of true clause groundings, then init to zero
    if (trackClauseTrueCnts_)
      for (int clauseno = 0; clauseno < clauseTrueCnts_->size(); clauseno++)
        (*clauseTrueCnts_)[clauseno] = 0;

      // Holds the ground preds which have currently been affected
    GroundPredicateHashArray affectedGndPreds;
    Array<int> affectedGndPredIndices;
      // Update the weights for Gibbs step
    int numAtoms = state_->getNumAtoms();
    for (int i = 0; i < numAtoms; i++)
    {
      affectedGndPreds.append(state_->getGndPred(i), numAtoms);
      affectedGndPredIndices.append(i);
    }
    updateWtsForGndPreds(affectedGndPreds, affectedGndPredIndices, 0);
    affectedGndPreds.clear();
    affectedGndPredIndices.clear();

    cout << "Running MC-SAT sampling..." << endl;
      // Sampling loop
    int sample = 0;
    int numSamplesPerPred = 0;
    bool done = false;
    while (!done)
    {
      ++sample;
      bool mcSatStep = (sample % numStepsEveryMCSat_ == 0);
      if (sample % samplesPerOutput == 0)
      { 
        currentTimeSec = timer.time();
        secondsElapsed = currentTimeSec - startTimeSec;
        cout << "Sample (per pred) " << sample << ", time elapsed = ";
        Timer::printTime(cout, secondsElapsed); cout << endl;
      }

        // For each node, generate the node's new truth value
      if (mcSatStep) performMCSatStep(burningIn);
        // Defined in MCMC. Chain is set to 0, but single chain is considered
        // in performGibbsStep
      else performGibbsStep(0, burningIn, affectedGndPreds,
                            affectedGndPredIndices);
        
      if (!burningIn) numSamplesPerPred++;

      if (burningIn) 
      {
        if (   (burnMaxSteps_ >= 0 && sample >= burnMaxSteps_)
            || (maxSeconds_ > 0 && secondsElapsed >= maxSeconds_))
        {
          cout << "Done burning. " << sample << " samples." << endl;
          burningIn = false;
          sample = 0;
        }
      }
      else 
      {
        if (   (maxSteps_ >= 0 && sample >= maxSteps_)
            || (maxSeconds_ > 0 && secondsElapsed >= maxSeconds_)) 
        {
          cout << "Done MC-SAT sampling. " << sample << " samples."             
               << endl;
          done = true;
        }
      }
      cout.flush();
    } // while (!done)

    cout<< "Time taken for MC-SAT sampling = "; 
    Timer::printTime(cout, timer.time() - startTimeSec); cout << endl;

    cout<< "Time taken for unit propagation = "; 
    Timer::printTime(cout, upSecondsElapsed_); cout << endl;

    cout<< "Time taken for SampleSat = "; 
    Timer::printTime(cout, ssSecondsElapsed_); cout << endl;

      // Update gndPreds probability that it is true
    for (int i = 0; i < state_->getNumAtoms(); i++)
    {
      setProbTrue(i, numTrue_[i] / numSamplesPerPred);
    }

      // If keeping track of true clause groundings
    if (trackClauseTrueCnts_)
    {
        // Set the true counts to the average over all samples
      for (int i = 0; i < clauseTrueCnts_->size(); i++)
        (*clauseTrueCnts_)[i] = (*clauseTrueCnts_)[i] / numSamplesPerPred;
    }
  }

 private:
 
  /**
   * Performs one step of MC-SAT.
   * 
   * @param burningIn Indicates if we are in a burning-in phase or not.
   */
  void performMCSatStep(const bool& burningIn)
  {
    Timer timer;
    double startTime;
    if (msdebug) cout << "Entering MC-SAT step" << endl;
      // Clause selection
    state_->setUseThreshold(true);
    int start = 0;
    state_->killClauses(start);

    if (msdebug)
    {
      cout << "Num of clauses " << state_->getNumClauses() << endl;
      cout << "Num of dead clauses " << state_->getNumDeadClauses() << endl;
    }
    
      // Unit propagation on the clauses
    startTime = timer.time();
    up_->init();
    up_->infer();
    upSecondsElapsed_ += (timer.time() - startTime);
      // SampleSat on the clauses
    startTime = timer.time();
    mws_->init();
    mws_->infer();
    ssSecondsElapsed_ += (timer.time() - startTime);

    if (msdebug) 
    {
      cout << "Low state:" << endl;
      state_->printLowState(cout);
    }
    state_->saveLowStateToGndPreds();

      // Reset parameters needed for MCSat step
    state_->resetFixedAtoms();
    state_->resetDeadClauses();
    state_->setUseThreshold(false);
    int numAtoms = state_->getNumAtoms();
      // If lazy, atoms may have been added, so have to grow numTrue_
    numTrue_.growToSize(numAtoms, 0);

    for (int i = 0; i < numAtoms; i++)
    {
      GroundPredicate* gndPred = state_->getGndPred(i);
      bool newAssignment = state_->getValueOfLowAtom(i + 1);
      
        // No need to update weight but still need to update truth/NumSat
      if (newAssignment != gndPred->getTruthValue())
      {
        gndPred->setTruthValue(newAssignment);
        updateClauses(i);
      }

        // If in actual sampling phase, track the num of times
        // the ground predicate is set to true
      if (!burningIn && newAssignment) numTrue_[i]++;
    }
    
      // If keeping track of true clause groundings
    if (!burningIn && trackClauseTrueCnts_)
      state_->getNumClauseGndings(clauseTrueCnts_, true);
    
    if (msdebug) cout << "Leaving MC-SAT step" << endl;
  }
  
  /**
   * Updates the number of satisfied literals in the clauses in which a ground
   * predicate occurs.
   * 
   * @param gndPredIdx Index of ground predicate which occurs in the ground
   * clauses being updated.
   */
  void updateClauses(const int& gndPredIdx)
  {
    if (msdebug) cout << "Entering updateClauses" << endl;
    GroundPredicate* gndPred = state_->getGndPred(gndPredIdx);
    Array<int>& negGndClauses =
      state_->getNegOccurenceArray(gndPredIdx + 1);
    Array<int>& posGndClauses =
      state_->getPosOccurenceArray(gndPredIdx + 1);
    int gndClauseIdx;
    bool sense;

    for (int i = 0; i < negGndClauses.size() + posGndClauses.size(); i++)
    {
      if (i < negGndClauses.size())
      {
        gndClauseIdx = negGndClauses[i];
        sense = false;
      }
      else
      {
        gndClauseIdx = posGndClauses[i - negGndClauses.size()];
        sense = true;
      }

      if (gndPred->getTruthValue() == sense)
        state_->incrementNumTrueLits(gndClauseIdx);
      else
        state_->decrementNumTrueLits(gndClauseIdx);
    }
    if (msdebug) cout << "Leaving updateClauses" << endl;
  }
  
 private:

    // Number of total steps (MC-SAT & Gibbs) for each MC-SAT step
  int numStepsEveryMCSat_;

    // Unit propagation is performed in MC-SAT  
  UnitPropagation* up_;
    // The base algorithm is SampleSat (MaxWalkSat with SS parameters)
  MaxWalkSat* mws_;

    // Time spent on UnitPropagation
  double upSecondsElapsed_;
    // Time spent on SampleSat
  double ssSecondsElapsed_;
};

#endif /*MCSAT_H_*/
