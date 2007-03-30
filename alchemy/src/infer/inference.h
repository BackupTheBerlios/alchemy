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
#ifndef INFERENCE_H_
#define INFERENCE_H_

#include "variablestate.h"

/**
 * Abstract class from which all inference algorithms are derived.
 * At least one function is pure virtual making this an abstract class
 * (it can not be instantiated).
 */
class Inference
{
 public:

  /**
   * Constructor: Every inference algorithm is required to have a VariableState
   * representing the state of variables and clauses and a seed for any
   * randomization in the algorithm. If there is no randomization, seed is not
   * used.
   * 
   * @param state State of the variables and clauses of the inference.
   * @param seed Seed used to initialize randomization in the algorithm.
   * @param trackClauseTrueCnts Indicates if true counts for each first-order
   * clause are being kept
   */
  Inference(VariableState* state, long int seed,
            const bool& trackClauseTrueCnts)
  {
    this->state_ = state;
    this->seed_ = seed;
    if (seed_ <= 0)
    {
      struct timeval tv;
      struct timezone tzp;
      gettimeofday(&tv, &tzp);
      seed_ = (long int)((( tv.tv_sec & 0177 ) * 1000000) + tv.tv_usec);
    }
    srandom(seed_);

    trackClauseTrueCnts_ = trackClauseTrueCnts;
    if (trackClauseTrueCnts_)
    {
        // clauseTrueCnts_ will hold the true counts for each first-order clause
      clauseTrueCnts_ = new Array<double>;
      clauseTrueCnts_->growToSize(state_->getMLN()->getNumClauses(), 0);
    }
  }
  
  /**
   * Virtual destructor.
   */
  virtual ~Inference()
  {
    if (trackClauseTrueCnts_) delete clauseTrueCnts_;
  }

  /**
   * Initializes the inference algorithm.
   */
  virtual void init() = 0;

  /**
   * Performs the inference algorithm.
   */
  virtual void infer() = 0;

  /**
   * Prints the probabilities of each predicate to a stream.
   */
  virtual void printProbabilities(ostream& out) = 0;
  
  /**
   * Puts the predicates whose probability has changed (more than probDelta in
   * the case of prob. inference) with respect to the reference vector oldProbs
   * in string form and the corresponding probabilities of each predicate in two
   * vectors.
   */
  virtual void getChangedPreds(vector<string>& changedPreds,
                               vector<float>& probs,
                               vector<float>& oldProbs,
                               const float& probDelta) = 0;

  
  /**
   * Prints the predicates inferred to be true to a stream.
   */
  virtual void printTruePreds(ostream& out) = 0;

  /**
   * Gets the probability of a ground predicate.
   */
  virtual double getProbability(GroundPredicate* const& gndPred) = 0;

  long int getSeed() { return seed_; }
  void setSeed(long int s) { seed_ = s; }
  
  VariableState* getState() { return state_; }
  void setState(VariableState* s) { state_ = s; }
  
  const Array<double>* getClauseTrueCnts() { return clauseTrueCnts_; }

 protected:
  
    // Seed for randomizer. If not set, then date + time is used
  long int seed_;
    // State of atoms and clauses used during inference
    // Does not belong to inference (do not delete)
  VariableState* state_;
    // Holds the average number of true groundings of each first-order clause
    // in the mln associated with this inference
  Array<double>* clauseTrueCnts_;
    // Indicates if true counts for each first-order clause are being kept
  bool trackClauseTrueCnts_;
};

#endif /*INFERENCE_H_*/
