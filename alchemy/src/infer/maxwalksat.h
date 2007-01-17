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
#ifndef MAXWALKSAT_H
#define MAXWALKSAT_H

#include <cfloat>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <cstring>
#include <sstream>
#include <fstream>
#include "array.h"
#include "timer.h"
#include "util.h"
#include "sat.h"
#include "maxwalksatparams.h"

enum heuristics {RANDOM = 0, BEST = 1, TABU = 2, SS = 3};
const bool mwsdebug = false;

/**
 * The MaxWalkSat algorithm. This code is based on the MaxWalkSat
 * package of Kautz et al. and SampleSat by Wei et al.
 * 
 * Walksat is achieved by using unit weights (1 and -1) in the clauses
 * in the state and setting the target cost to 0.
 * 
 * SampleSat is achieved by using unit weights (1 and -1) in the clauses
 * in the state and setting the target cost to 0 and the heuristic to SS.
 */
class MaxWalkSat : public SAT
{
 public:

  /**
   * Constructor: user-set parameters are set.
   */
  MaxWalkSat(VariableState* state, int seed, const bool& trackClauseTrueCnts,
             MaxWalksatParams* params)
    : SAT(state, seed, trackClauseTrueCnts)
  {
    int numAtoms = state_->getNumAtoms();

      // User-set parameters
    maxSteps_ = params->maxSteps;
    maxTries_ = params->maxTries;
    targetCost_ = params->targetCost;
    hard_ = params->hard;
    numSolutions_ = params->numSolutions;
    heuristic_ = params->heuristic;
    tabuLength_ = params->tabuLength;
    lazyLowState_ = params->lazyLowState;
      // For SampleSat
    saRatio_ = params->ssParams->saRatio;
    saTemp_ = params->ssParams->saTemp;
    lateSa_ = params->ssParams->lateSa;
    
      // Info from SAT class
    if (heuristic_ == TABU || heuristic_ == SS)
      changed_.growToSize(numAtoms + 1);
    numFlips_ = 0;

      // Info from MaxWalkSat
    numerator_ = 50; // User-set?
    denominator_ = 100; // User-set?
      // We assume we are not in a block
    inBlock_ = false;
    flipInBlock_ = NOVALUE;

      // Initialize array of function pointers
    int idx = 0;
    pickcode[idx++] = &MaxWalkSat::pickRandom;
    pickcode[idx++] = &MaxWalkSat::pickBest;
    pickcode[idx++] = &MaxWalkSat::pickTabu;
    pickcode[idx++] = &MaxWalkSat::pickSS;
  }
  
  /**
   * Destructor (destructors from superclasses are called)
   */
  ~MaxWalkSat() { }

  /**
   * Initializes the MaxWalkSat algorithm. A random assignment is given to the
   * atoms and the state is initialized.
   */
  void init()
  {
      // Init changed array if using tabu
    if (heuristic_ == TABU || heuristic_ == SS)
    {
      int numAtoms = state_->getNumAtoms();
      if (changed_.size() != numAtoms + 1)
      {
        if (changed_.size() < numAtoms + 1)
          changed_.growToSize(numAtoms + 1);
        else
          changed_.shrinkToSize(numAtoms + 1);
      }
      for (int i = 1; i <= numAtoms; i++)
        changed_[i] = -(tabuLength_ + 1);
    }
    state_->initRandom();
  }
  
  /**
   * Performs the given number of MaxWalkSat inference tries.
   */
  void infer()
  {
    int numtry = 0;
    int numsuccesstry = 0;
    long double lowCost = LDBL_MAX;

      // If keeping track of true clause groundings, then init to zero
    if (trackClauseTrueCnts_)
      for (int clauseno = 0; clauseno < clauseTrueCnts_->size(); clauseno++)
        (*clauseTrueCnts_)[clauseno] = 0;

      // Perform up to maxTries tries or numSolutions successful tries
    while (numsuccesstry < numSolutions_ &&
           numtry < maxTries_*numSolutions_)
    {
      numtry++;
      numFlips_ = 0;
      bool lowStateInThisTry = false;
      if (lazyLowState_)
        varsFlippedSinceLowState_.clear();

        // Make random assigment in subsequent tries
        // (for first try, init() should have been called)
      if (numtry > 1 && numsuccesstry == 0) init();

      if (mwsdebug)
      {
        cout << "\nIn the beginning of try " << numtry << ": " << endl;
        cout << "Number of clauses: " << state_->getNumClauses() << endl;
        cout << "Number of false clauses: " << state_->getNumFalseClauses()
             << endl;
        cout << "Cost of false clauses: " << state_->getCostOfFalseClauses()
             << endl;
        cout << "Target cost: " << targetCost_ << endl;
      }
      
      while (numFlips_ < maxSteps_ && numsuccesstry < numSolutions_)
      {
        numFlips_++;
        flipAtom((this->*(pickcode[heuristic_]))());
          // If in a block, then another atom was also chosen to flip
        if (inBlock_)
          flipAtom(flipInBlock_);

          // set in block back to false
        inBlock_ = false;
        flipInBlock_ = NOVALUE;
        
        long double costOfFalseClauses = state_->getCostOfFalseClauses();
        //long double lowCost = state_->getLowCost();
          // If the cost of false clauses is lower than the low cost of all
          // tries, then save this as the low state.
        if (costOfFalseClauses < lowCost)
        {
          if (mwsdebug)
          {
            cout << "Cost of false clauses: " << costOfFalseClauses
                 << " less than lowest cost: " << lowCost << endl;
          }
          lowCost = costOfFalseClauses;
          if (!lazyLowState_)
            state_->saveLowState();
          else
          {
            varsFlippedSinceLowState_.clear();
            lowStateInThisTry = true;
          }
        }
        
          // If succesful try
          // Add 0.0001 to targetCost_ because of double precision error
          // This needs to be fixed
        if (costOfFalseClauses <= targetCost_ + 0.0001)
          numsuccesstry++;
      }
      
      if (lowStateInThisTry)
        reconstructLowState();
      state_->saveLowStateToDB();
      
      if (mwsdebug)
      {
        cout<< "In the end of try " << numtry << ": " << endl;
        cout<< "Lowest num. of false clauses: " << state_->getLowBad() << endl;
        cout<< "Lowest cost of false clauses: " << state_->getLowCost() << endl;
        cout<< "Number of flips: " << numFlips_ << endl;
        //cout<< "Active atoms " << state_->getNumActiveAtoms() << endl; 
      }
    }

      // If keeping track of true clause groundings
    if (trackClauseTrueCnts_)
      state_->getNumClauseGndings(clauseTrueCnts_, true);    
  }
  
  const int getHeuristic()
  {
    return heuristic_;
  }
  
  void setHeuristic(const int& heuristic)
  {
    heuristic_ = heuristic;
  }

 protected:
  
  /**
   * Flips the truth value of an atom and marks this iteration as the
   * last time it was flipped.
   * 
   * @param toFlip Index of atom to flip.
   */
  void flipAtom (int toFlip)
  {
    //if (mwsdebug) cout << "Entering MaxWalkSat::flipAtom" << endl;
    if (toFlip == NOVALUE)
      return;

      // Flip the atom in the state
    state_->flipAtom(toFlip);
      // Mark this flip as last time atom was changed if tabu is used
    if (heuristic_ == TABU || heuristic_ == SS)
    {
      changed_[toFlip] = numFlips_;
      changed_.growToSize(state_->getNumAtoms(), -(tabuLength_ + 1));
    }
    
    if (lazyLowState_)
    {
        // Mark this variable as flipped since last low state. If already
        // present, then it has been flipped back to its value in the low
        // state and we can remove it.
      if (varsFlippedSinceLowState_.find(toFlip) !=
          varsFlippedSinceLowState_.end())
      {
        if (mwsdebug)
        {
          //cout << "Atom " << toFlip << " has been flipped since low state, "
          //     << "so removing it" << endl;
        }
        varsFlippedSinceLowState_.erase(toFlip);
      }
      else
      {
        if (mwsdebug)
        {
          //cout << "Atom " << toFlip << " not flipped since low state, "
          //     << "so adding it" << endl;
        }
        varsFlippedSinceLowState_.insert(toFlip);
      }
    }
    //if (mwsdebug) cout << "Leaving MaxWalkSat::flipAtom" << endl;
  }

  /**
   * Pick a random atom in a random unsatisfied pos. clause or a random
   * true literal in a random satsified neg. clause to flip.
   * 
   * @return Index of atom picked.
   */
  int pickRandom()
  {
    //if (mwsdebug) cout << "Entering MaxWalkSat::pickRandom" << endl;
    assert(!inBlock_);
    int atomIdx = state_->getIndexOfAtomInRandomFalseClause();
    assert(atomIdx > 0);

      // If atom is in a block, then another one has to be picked
    int blockIdx = state_->getBlockIndex(atomIdx - 1);
    if (blockIdx >= 0)
    {
        // Atom is in a block
        // If evidence atom exists or in block of size 1, then can not flip
      if (state_->isBlockEvidence(blockIdx) ||
          state_->getBlockSize(blockIdx) == 1)
        return NOVALUE;
      else
      {
          //Dealing with atom in a block
        Array<int>& block = state_->getBlockArray(blockIdx);
          // 0->1: choose atom in block which is already 1
        if (state_->getValueOfAtom(atomIdx) == 0)
        {
          for (int i = 0; i < block.size(); i++)
          {
            if (state_->getValueOfAtom(block[i] + 1) == 1)
            {
              inBlock_ = true;
              flipInBlock_ = block[i] + 1;
              return atomIdx;
            }
          }
        }
          // 1->0: choose to flip the other randomly
          // TODO: choose to flip the other with best improvement
        else
        {
          bool ok = false;
          while (!ok)
          {
            int chosen = random() % block.size();
            if (block[chosen] + 1 != atomIdx)
            {
              inBlock_ = true;
              flipInBlock_ = block[chosen] + 1;
              return atomIdx;
            }
          }
        }
      }
    }
    //if (mwsdebug) cout << "Leaving MaxWalkSat::pickRandom" << endl;
    return atomIdx;
  }

  /**
   * Pick the best atom (clauses made satisfied - clauses made unsatisfied)
   * in a random unsatisfied clause.
   * 
   * @return Index of atom picked.
   */
  int pickBest()
  {
    //if (mwsdebug) cout << "Entering MaxWalkSat::pickBest" << endl;
    assert(!inBlock_);
      // Clause to fix picked at random    
    int toFix = state_->getRandomFalseClauseIndex();
    if (toFix == NOVALUE) return NOVALUE;

    long double improvement;

    Array<int> canNotFlip;
      // If var in candidateBlockedVars is chosen, then
      // corresponding var in othersToFlip is flipped as well
    Array<int> candidateBlockedVars;
    Array<int> othersToFlip;

    int clauseSize = state_->getClauseSize(toFix);
    long double cost = state_->getClauseCost(toFix);
      // Holds the best atoms so far
    Array<int> best;
      // How many are tied for best
    register int numbest = 0;
      // Best value so far
    long double bestvalue = -LDBL_MAX;
      // With prob. do a noisy pick
    bool noisyPick = (numerator_ > 0 && random()%denominator_ < numerator_); 

      // Look at each atom and pick best ones
    for (int i = 0; i < clauseSize; i++)
    {
      register int lit = state_->getAtomInClause(i, toFix);
        // Neg. clause: Only look at true literals
      if (cost < 0 && !state_->isTrueLiteral(lit)) continue;
        // var is the index of the atom
      register int var = abs(lit);

      improvement = calculateImprovement(var, canNotFlip,
                                         candidateBlockedVars,
                                         othersToFlip);

      if (improvement >= bestvalue)
      { // Tied or better than previous best
        if (improvement > bestvalue)
        {
          numbest = 0;
          best.clear();
        }
        bestvalue = improvement;
        best.append(var);
        numbest++;
      }
    }

      // If only false literals in a neg. clause, then numbest is 0
    if (numbest == 0) return NOVALUE;
      // Choose one of the best at random
      // (default if none of the following 2 cases occur)
    int toFlip = best[random()%numbest];

      // 1. If no improvement by best,
      // then with prob. choose a random atom
    if (bestvalue <= 0 && noisyPick)
    {
      if (cost > 0)
        toFlip = abs(state_->getRandomAtomInClause(toFix));
      else
        toFlip = state_->getRandomTrueLitInClause(toFix);
    }
      // 2. Exactly one best atom
    else if (numbest == 1)
      toFlip = best[0];

      // If atom can not be flipped, then null flip
    if (canNotFlip.contains(toFlip)) toFlip = NOVALUE;
    else
    { // If toFlip is in block, then set other to flip
      int idx = candidateBlockedVars.find(toFlip);
      if (idx >= 0)
      {
        inBlock_ = true;
        flipInBlock_ = othersToFlip[idx];
      }
    }

    //if (mwsdebug) cout << "Leaving MaxWalkSat::pickBest" << endl;
    return toFlip;
  }

  /**
   * Pick an atom from a random unsatisfied clause based on the tabu heuristic.
   * 
   * @return Index of atom picked.
   */
  int pickTabu()
  {
    //if (mwsdebug) cout << "Entering MaxWalkSat::pickTabu" << endl;
    assert(!inBlock_);
      // Clause to fix picked at random    
    int toFix = state_->getRandomFalseClauseIndex();
    if (toFix == NOVALUE)
    {
      //if (mwsdebug) cout << "Leaving MaxWalkSat::pickTabu (NOVALUE)" << endl;      
      return NOVALUE;
    }

    long double improvement;
    Array<int> canNotFlip;
      // If var in candidateBlockedVars is chosen, then
      // corresponding var in othersToFlip is flipped as well
    Array<int> candidateBlockedVars;
    Array<int> othersToFlip;

    int clauseSize = state_->getClauseSize(toFix);
    long double cost = state_->getClauseCost(toFix);

      // Holds the best atoms so far
    Array<int> best;
      // How many are tied for best
    register int numbest = 0;
      // Best value so far
    long double bestvalue = -LDBL_MAX;

      // With prob. do a noisy pick
    bool noisyPick = (numerator_ > 0 && random()%denominator_ < numerator_); 

    for (int i = 0; i < clauseSize; i++)
    {
      register int lit = state_->getAtomInClause(i, toFix);
        // Neg. clause: Only look at true literals
      if (cost < 0 && !state_->isTrueLiteral(lit)) continue;
        // var is the index of the atom
      register int var = abs(lit);

      improvement = calculateImprovement(var, canNotFlip, candidateBlockedVars,
                                         othersToFlip);

        // TABU: If pos. improvement, then always add it to best
      if (improvement > 0 && improvement >= bestvalue)
      { // Tied or better than previous best
        if (improvement > bestvalue)
        {
          numbest = 0;
          best.clear();
        }
        bestvalue = improvement;
        best.append(var);
        numbest++;
      }
      else if (tabuLength_ < numFlips_ - changed_[var])
      {
        if (noisyPick && bestvalue <= 0)
        { 
          best.append(var);
          numbest++;
        }
        else if (improvement >= bestvalue)
        { // Tied or better than previous best
          if (improvement > bestvalue)
          {
            numbest = 0;
            best.clear();
          }
          bestvalue = improvement;
          best.append(var);
          numbest++;
        }
      }
    }
    
    if (numbest == 0) 
    {
      //if (mwsdebug) cout << "Leaving MaxWalkSat::pickTabu (NOVALUE)" << endl;
      return NOVALUE;
    }

    int toFlip = NOVALUE;
      // Exactly one best atom
    if (numbest == 1)
      toFlip = best[0];
    else
      // Choose one of the best at random
      toFlip = best[random()%numbest];


      // If atom can not be flipped, then null flip
    if (canNotFlip.contains(toFlip)) toFlip = NOVALUE;
    else
    { // If toflip is in block, then set other to flip
      int idx = candidateBlockedVars.find(toFlip);
      if (idx >= 0)
      {
        inBlock_ = true;
        flipInBlock_ = othersToFlip[idx];
      }
    }

    //if (mwsdebug) cout << "Leaving MaxWalkSat::pickTabu" << endl;
    return toFlip;
  }

  /**
   * Pick an atom according to the SampleSat heuristic. This means sometimes
   * sim. annealing, sometimes MaxWalkSat.
   * 
   * @return Index of atom picked.
   */
  int pickSS()
  {
    //if (mwsdebug) cout << "Entering MaxWalkSat::pickSS" << endl;
    assert(!inBlock_);
    Array<int> canNotFlip;
      // If var in candidateBlockedVars is chosen, then
      // corresponding var in othersToFlip is flipped as well
    Array<int> candidateBlockedVars;
    Array<int> othersToFlip;
    long double costOfFalseClauses = state_->getCostOfFalseClauses();

      // If we have already reached a solution or if in an SA step,
      // then perform SA
      // Add 0.0001 to targetCost_ because of double precision error
      // This needs to be fixed
    if (costOfFalseClauses <= targetCost_ + 0.0001 ||
        (random() % 100 < saRatio_ && !lateSa_))
    {
        // Choose a random atom to flip
      int toFlip = state_->getIndexOfRandomAtom();
      if (toFlip == NOVALUE) return NOVALUE;
      long double improvement = calculateImprovement(toFlip, canNotFlip,
                                                     candidateBlockedVars,
                                                     othersToFlip);

        // If pos. or no improvement, then the atom will be flipped
        // Or neg. improvement: According to temp. flip atom anyway
      if ((improvement >= 0) ||
          (random() <= exp(improvement/(saTemp_/100.0)) * RAND_MAX))
      {
          // If atom can not be flipped, then null flip
        if (canNotFlip.contains(toFlip)) toFlip = NOVALUE;
        else
        { // If toflip is in block, then set other to flip
          int idx = candidateBlockedVars.find(toFlip);
          if (idx >= 0)
          {
            inBlock_ = true;
            flipInBlock_ = othersToFlip[idx];
          }
        }
        return toFlip;
      }
      else
      {
        return NOVALUE;
      }
    }
      // Not in a solution or SA step: perform normal MWS step
    else
    {
      return pickTabu();
    }
  }


  /**
   * Calculates the improvement (makecost - breakcost) by flipping an atom.
   * If the atom is in a block, then its index is saved in candidateBlockedVars
   * and the index of another atom chosen to be flipped in the block is
   * appended to othersToFlip. If the atom is in a block with evidence, then
   * its index is appended to canNotFlip.
   * 
   * @param atomIdx Index of atom for which the improvement is calculated.
   * @param canNotFlip Holds indices of atoms which can not be flipped due
   * to evidence in a block.
   * @param candidateBlockedVars If dealing with an atom in a block, then its
   * index is appended here.
   * @param othersToFlip If dealing with an atom in a block, then the index of
   * the other atom chosen to be flipped is appended here.
   */
  long double calculateImprovement(const int& atomIdx, Array<int>& canNotFlip,
                                   Array<int>& candidateBlockedVars,
                                   Array<int>& othersToFlip)
  {
    int blockIdx = state_->getBlockIndex(atomIdx - 1);
    if (blockIdx == -1)
    {
        // Atom not in a block
      return state_->getImprovementByFlipping(atomIdx);
    }

    else
    {
        // Atom is in a block
        // If evidence atom exists or in block of size 1, then can not flip
      if (state_->isBlockEvidence(blockIdx) ||
          state_->getBlockSize(blockIdx) == 1)
      {
        canNotFlip.append(atomIdx);
        return -LDBL_MAX;
      }
      else
      {
          //Dealing with atom in a block
        Array<int>& block = state_->getBlockArray(blockIdx);
        int chosen = -1;
        int otherToFlip = -1;

          // 0->1: choose atom in block which is already 1
        if (state_->getValueOfAtom(atomIdx) == 0)
        {
          if (mwsdebug)
          {
            cout << "0->1 atom " << atomIdx << endl;
            for (int i = 0; i < block.size(); i++)
            {
              cout << "Atom in block " << block[i] + 1 << " ";
              cout << state_->getValueOfAtom(block[i] + 1) << endl;
            }
          }
          
          for (int i = 0; i < block.size(); i++)
          {
            if (state_->getValueOfAtom(block[i] + 1) == 1)
            {
              chosen = i;
              break;
            }
          }
        }
          // 1->0: choose to flip the other randomly
          // TODO: choose to flip the other with best improvement
        else
        {
          if (mwsdebug)
          {
            cout << "1->0 atom " << atomIdx << endl;
            for (int i = 0; i < block.size(); i++)
            {
              cout << "Atom in block " << block[i] + 1 << " ";
              cout << state_->getValueOfAtom(block[i] + 1) << endl;
            }
          }
          bool ok = false;
          while (!ok)
          {
            chosen = random() % block.size();
            if (block[chosen] + 1 != atomIdx)
              ok = true;
          }
        }
    
        assert(chosen >= 0);
        candidateBlockedVars.append(atomIdx);
        otherToFlip = block[chosen] + 1;
        othersToFlip.append(otherToFlip);
        return (state_->getImprovementByFlipping(atomIdx) +
                state_->getImprovementByFlipping(otherToFlip));

      }
    }
  }

  /**
   * Reconstructs the state with the lowest cost by flipping atoms back to
   * their value in this state.
   */
  void reconstructLowState()
  {
    assert(lazyLowState_);
    if (mwsdebug) cout << "Reconstructing low state ..." << endl;
    hash_set<int>::const_iterator it = varsFlippedSinceLowState_.begin();
    for (; it != varsFlippedSinceLowState_.end(); it++)
    {
      if (mwsdebug)
      {
        cout << "Flipping atom " << (*it) << endl;
      }
      state_->flipAtomValue(*it);
    }
    state_->saveLowState();
    if (mwsdebug) cout << "Done reconstructing low state ..." << endl;
  }

 private:
 
    ////////// BEGIN: User parameters ///////////
    // Heuristic to be used to pick an atom
  int heuristic_;
    // At least this many flips must occur before flipping the same atom
  int tabuLength_;
  
    // BEGIN: SampleSat parameters
    // Percent of sim. annealing steps
  int saRatio_;
    // Sim. annealing temperature
  int saTemp_;
    // Run sim. annealing only at a plateur
  bool lateSa_;
    // END: SampleSat parameters
    ////////// END: User parameters ///////////

    // Function pointer holds which function is to be used to pick an atom
    // = {pickRandom, pickBest, pickTabu, pickSS};
  int (MaxWalkSat::*pickcode[15])(void);
    // If true, never break a highest cost clause
  bool hard_;

    // Make random flip with numerator/denominator probability
  int numerator_;
  int denominator_;
      
    // Which other atom to flip in block
  bool inBlock_;
  int flipInBlock_;
  
    // If false, the naive way of saving low states (each time a low state is
    // found, the whole state is saved) is used; otherwise, a list of variables
    // flipped since the last low state is kept and the low state is
    // reconstructed. This can be much faster for very large data sets.
  bool lazyLowState_;
    // List of variables flipped since last low state was found. This is used
    // to reconstruct the low state when lazyLowState_ is set.
  hash_set<int> varsFlippedSinceLowState_;
};

#endif
