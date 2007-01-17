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
#ifndef GROUNDCLAUSE_H_JUN_26_2005
#define GROUNDCLAUSE_H_JUN_26_2005

#include <cfloat>
#include <ext/hash_set>
using namespace __gnu_cxx;
#include "hasharray.h"
#include "array.h"
#include "hash.h"
#include <map>

using namespace std;

// containers    /////////////////////////////////////////////
typedef map<int,int> IntPair;
typedef IntPair::iterator IntPairItr;  
//////////////////////////////////////////////////////////////

// Constants
const double HARD_GROUNDCLAUSE_WT = DBL_MAX;

// Forward declarations
class Domain;
class Clause;
class GroundPredicate;
class HashGroundPredicate;
class EqualGroundPredicate;

// Typedefs
typedef HashArray<GroundPredicate*, HashGroundPredicate, EqualGroundPredicate> 
 GroundPredicateHashArray;

/**
 * Represents a grounded clause.
 */
class GroundClause
{
 public:
  //GroundClause(const Clause* const & c);
  GroundClause(const Clause* const & c, 
               GroundPredicateHashArray* const & gndPredHashArray);

  ~GroundClause() 
  { 
    if (gndPredIndexes_) delete gndPredIndexes_;
    if (parentWtPtrs_) delete parentWtPtrs_;
    if (foClauseFrequencies_) delete foClauseFrequencies_;
  }

  void deleteParentWtPtrs()
  { 
    if (parentWtPtrs_) 
    {
      for (int i = 0; i < parentWtPtrs_->size(); i++)
        delete (*parentWtPtrs_)[i];
      delete parentWtPtrs_;
    }
    parentWtPtrs_ = NULL;
  }

  void deleteFoClauseFrequencies()
  { 
    if (foClauseFrequencies_) delete foClauseFrequencies_;
    foClauseFrequencies_ = NULL;
  }

  void addWt(const double& wt) 
  { if (wt_ == HARD_GROUNDCLAUSE_WT) return; wt_ += wt; }

  void setWt(const double& wt) 
  { if (wt_ == HARD_GROUNDCLAUSE_WT) return; wt_ = wt; }

  double getWt() const { return wt_; }

  void setWtToHardWt() { wt_ = HARD_GROUNDCLAUSE_WT; }
  bool isHardClause() const { return (wt_ == HARD_GROUNDCLAUSE_WT); }

  int getNumGroundPredicates() const { return gndPredIndexes_->size(); }

  const GroundPredicate* getGroundPredicate(const int& i,
                      GroundPredicateHashArray* const & gndPredHashArray) const 
  { 
    return (*gndPredHashArray)[abs((*gndPredIndexes_)[i]) - 1];
  }

  /**
   * Appends this GroundClause to all GroundPredicates in it.
   * 
   * @param gndPredHashArray Reference HashArray containing the
   * GroundPredicates indexed in the GroundClause.
   */
  void appendToGndPreds(GroundPredicateHashArray* const & gndPredHashArray);

  bool getGroundPredicateSense(const int& i) const 
  { return ((*gndPredIndexes_)[i] > 0); }

  void setGroundPredicateIndex(const int& i, const int& gndPredIdx) 
  { (*gndPredIndexes_)[i] = gndPredIdx; }

  int getGroundPredicateIndex(const int& i) const 
  { return (*gndPredIndexes_)[i]; }

  const Array<int>* getGndPredIndexes() const
  {
    return gndPredIndexes_;
  }

  void appendParentWtPtr(const double* const & wtPtr) 
  {
    if (parentWtPtrs_ == NULL)
      parentWtPtrs_ = new Array<const double*>;
    parentWtPtrs_->append(wtPtr);
  }

  void setWtToSumOfParentWts()
  {
    assert(parentWtPtrs_);
    wt_ = 0;
    for (int i = 0; i < parentWtPtrs_->size(); i++)
      wt_ += *((*parentWtPtrs_)[i]);
  }
  
  IntPair *getClauseFrequencies()
  {
    return foClauseFrequencies_;
  }

  int getClauseFrequency(int clauseno)
  {
	if (!foClauseFrequencies_) return 0;
	IntPairItr itr = foClauseFrequencies_->find(clauseno);
	if (itr == foClauseFrequencies_->end()) 
	  return 0;
	else
	  return itr->second;
  }
  
  void incrementClauseFrequency(int clauseno, int increment)
  {
	if (!foClauseFrequencies_)
      foClauseFrequencies_ = new IntPair;
	IntPairItr itr = foClauseFrequencies_->find(clauseno);
	if (itr == foClauseFrequencies_->end()) 
	  foClauseFrequencies_->insert(IntPair::value_type(clauseno, increment));
	else
	  itr->second += increment;
  }

  size_t hashCode() { return hashCode_; }
  
  bool same(const GroundClause* const & gc)
  {
    if (this == gc) return true;
    if (gndPredIndexes_->size() != gc->getGndPredIndexes()->size())
    {
      return false;
    }
    return (memcmp(gndPredIndexes_->getItems(),
                   gc->getGndPredIndexes()->getItems(), 
                   (gndPredIndexes_->size())*sizeof(int)) == 0);
  }

  void printWithoutWt(ostream& out) const;
  void print(ostream& out) const;

  ostream& print(ostream& out, const Domain* const& domain, 
                 const bool& withWt, const bool& asInt, 
                 const bool& withStrVar,
                 const GroundPredicateHashArray* const & predHashArray) const;

  ostream& printWithoutWt(ostream& out, const Domain* const & domain,
                  const GroundPredicateHashArray* const & predHashArray) const;

  ostream& 
  printWithoutWtWithStrVar(ostream& out, const Domain* const & domain,
                  const GroundPredicateHashArray* const & predHashArray) const;

  ostream& printWithWtAndStrVar(ostream& out, const Domain* const& domain,
                  const GroundPredicateHashArray* const & predHashArray) const;

  ostream& print(ostream& out, const Domain* const& domain,
                 const GroundPredicateHashArray* const & predHashArray) const;
    
  ostream& printWithoutWtWithStrVarAndPeriod(ostream& out, 
                                             const Domain* const& domain,
                  const GroundPredicateHashArray* const & predHashArray) const;

  double sizeKB();

 private:
    // Hash code of this ground clause
  size_t hashCode_;
  Array<int>* gndPredIndexes_; // 4 + 4*n bytes (n is no. of preds)

    // overloaded to indicate whether this is a hard clause
    // if this is a hard clause, wt_ is set to HARD_GROUNDCLAUSE_WT
  double wt_; // 8 bytes

    // Number of first-order clauses this clause corresponds to
  IntPair* foClauseFrequencies_;

    // pointers to weights of first-order clauses that, after being grounded 
    // and having their ground predicates with known truth values removed, 
    // are equal to ground clause
  Array<const double*>* parentWtPtrs_;

};


////////////////////////////////// hash /////////////////////////////////


class HashGroundClause
{
 public: 
  size_t operator()(GroundClause* const & gc) const  { return gc->hashCode(); }
};


class EqualGroundClause
{
 public:
  bool operator()(GroundClause* const & c1, GroundClause* const & c2) const
    { return c1->same(c2); }
};


/////////////////////////////// containers  /////////////////////////////////

typedef HashArray<GroundClause*, HashGroundClause, EqualGroundClause> 
  GroundClauseHashArray;

typedef hash_set<GroundClause*, HashGroundClause, EqualGroundClause> 
  GroundClauseSet;

  
#endif
