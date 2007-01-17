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
#ifndef INTCLAUSE_H_JAN_13_2006
#define INTCLAUSE_H_JAN_13_2006

#include <cfloat>
#include <ext/hash_set>
using namespace __gnu_cxx;
#include "hasharray.h"
#include "array.h"
#include "hash.h"
#include "predicate.h"

class Domain;
class Clause;
class Database;

//const double HARD_INTCLAUSE_WT = DBL_MAX;
//const double HARD_INTCLAUSE_WT = 10.0;

class IntClause
{
 public:

  IntClause(const Clause* const & c, PredicateHashArray* const & predHashArray);

    // Copy constructor
  IntClause(const IntClause& ic);

  ~IntClause()
  {
  	//if(intArrRep_) delete intArrRep_;
  }
 
  void deleteIntPredicates()
  {
	if (intPreds_) { intPreds_->compress(); delete intPreds_; }
  }

//  void addWt(const double& wt) 
//  { if (wt_ == LWInfo::HARD_WT) return; wt_ += wt; }

//  void setWt(const double& wt) 
//  { if (wt_ == LWInfo::HARD_WT) return; wt_ = wt; }

  void addWt(const double& wt);
  void setWt(const double& wt);

  double getWt() const { return wt_; }

//  void setWtToHardWt() { wt_ = LWInfo::HARD_WT; }

//  bool isHardClause() const { return (wt_ == LWInfo::HARD_WT); }

  void setWtToHardWt();
  bool isHardClause() const;
  
  int getNumIntPredicates() const { return intPreds_->size(); }

  const int getIntPredicate(const int& i) const 
  { return (*intPreds_)[i]; }
  
  const Array<int>* getIntPredicates() const 
  { return intPreds_; }

  bool isSatisfied(const PredicateHashArray* const & predHashArray,
	  		   	   const Database* const & db) const;

  /*
  const Array<unsigned int>* getIntArrRep() const
  { return intArrRep_; }
  */
  
  size_t hashCode() { return hashCode_;}
  
  bool same(const IntClause* const & c)
  {
    if(this == c) return true;
    
    if (intPreds_->size() != c->getIntPredicates()->size()) return false;

	const int* myItems = intPreds_->getItems();
    const int* cItems  = c->getIntPredicates()->getItems();
    
	return (memcmp(myItems,cItems,(intPreds_->size())*sizeof(int)) == 0);
  } 	

  void printWithoutWt(ostream& out) const
  {
  	for (int i = 0; i < intPreds_->size(); i++)
  	{
      out << (*intPreds_)[i];
      if (i < intPreds_->size()-1) out << " v ";
  	}
  }

  void print(ostream& out) const
  { out << wt_ << " "; printWithoutWt(out); }


  ostream& print(ostream& out, const Domain* const& domain, 
                 const bool& withWt, const bool& asInt, 
                 const bool& withStrVar,
                 const PredicateHashArray* const & predHashArray) const
  {
    if (withWt) out << wt_ << " ";

    Array<Predicate*> eqPreds;
    Array<Predicate*> internalPreds;
    for (int i = 0; i < intPreds_->size(); i++)
    {
      int index = (*intPreds_)[i];
      Predicate* pred = new Predicate((*(*predHashArray)[abs(index)-1]));
      assert(pred);
      if (index < 0) pred->setSense(false);
      else pred->setSense(true);

      if (pred->isEqualPred()) 
        eqPreds.append(pred);
      else
      if (pred->isInternalPred()) 
        internalPreds.append(pred);
      else
      {
        if (asInt)           pred->printAsInt(out);
        else if (withStrVar) pred->printWithStrVar(out, domain);
        else                 pred->print(out,domain);
        if (i < intPreds_->size()-1 || !eqPreds.empty() ||
            !internalPreds.empty()) out << " v ";
        delete pred;
      }
    }

    for (int i = 0; i < eqPreds.size(); i++)
    {
      if (asInt)           eqPreds[i]->printAsInt(out);
      else if (withStrVar) eqPreds[i]->printWithStrVar(out,domain);
      else                 eqPreds[i]->print(out,domain);
      out << ((i != eqPreds.size()-1 || !internalPreds.empty())?" v ":"");      
      delete eqPreds[i];
    }

    for (int i = 0; i < internalPreds.size(); i++)
    {
      if (asInt)           internalPreds[i]->printAsInt(out);
      else if (withStrVar) internalPreds[i]->printWithStrVar(out,domain);
      else                 internalPreds[i]->print(out,domain);
      out << ((i!=internalPreds.size()-1)?" v ":"");      
      delete internalPreds[i];
    }

    return out;
  }


  ostream& printWithoutWt(ostream& out, const Domain* const & domain,
                          const PredicateHashArray* const & predHashArray) const
  { return print(out, domain, false, false, false, predHashArray); }

  ostream& 
  printWithoutWtWithStrVar(ostream& out, const Domain* const & domain,
                          const PredicateHashArray* const & predHashArray) const
  { return print(out, domain, false, false, true, predHashArray); }

  ostream& printWithWtAndStrVar(ostream& out, const Domain* const& domain,
                          const PredicateHashArray* const & predHashArray) const
  { return print(out, domain, true, false, true, predHashArray); }

  ostream& print(ostream& out, const Domain* const& domain,
                 const PredicateHashArray* const & predHashArray) const
  { return print(out, domain, true, false, false, predHashArray); }
    
  ostream& printWithoutWtWithStrVarAndPeriod(ostream& out, 
                          const Domain* const& domain,
                          const PredicateHashArray* const & predHashArray) const
  {
    printWithoutWtWithStrVar(out, domain, predHashArray);
    if (isHardClause()) out << ".";
    return out;
  }


 private:
  size_t hashCode_;

  //Array<unsigned int>* intArrRep_;  
  
  Array<int>* intPreds_;
  
  // overloaded to indicate whether this is a hard clause
  // if this is a hard clause, wt_ is set to LWInfo::HARD_WT
  double wt_;

};

////////////////////////////////// hash /////////////////////////////////


class HashIntClause
{
 public: 
  size_t operator()(IntClause* const & c) const  { return c->hashCode(); }
};


class EqualIntClause
{
 public:
  bool operator()(IntClause* const & c1, IntClause* const & c2) const
    { return c1->same(c2); }
};


/////////////////////////////// containers  /////////////////////////////////

typedef HashArray<IntClause*, HashIntClause, EqualIntClause> 
  IntClauseHashArray;


#endif

