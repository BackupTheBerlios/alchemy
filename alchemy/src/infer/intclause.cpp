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
#include "intclause.h"
#include "clause.h"
#include "lazyinfo.h"

double LazyInfo::HARD_WT = 10.0;
double LazyInfo::WSCALE = 1.0;

IntClause::IntClause(const Clause* const & c, PredicateHashArray* const & predHashArray) 
  : wt_(c->getWt())
{
  int numPreds = c->getNumPredicates();
  intPreds_ = new Array<int>;
  Array<unsigned int>* intArrRep = new Array<unsigned int>;
  
  // for each predicate in clause c
  for (int i = 0; i < numPreds; i++)
  {
  	bool deletePred = false;
    Predicate* pred = new Predicate(*(c->getPredicate(i)));
	int index = predHashArray->find(pred);
	if(index < 0 )
	{
	  index = predHashArray->append(pred) + 1;
	}
	else
	{
	  deletePred = true;
	  index++;
	}
	int wlit;
	if(pred->getSense())
	{
	  wlit = index;
	  intArrRep->append(1);
	}
	else
	{
	  wlit = -index;
	  intArrRep->append((unsigned int)0);
	}
	intArrRep->append(index);
	intPreds_->append(wlit);
	if (deletePred) delete pred;
  }
  
  hashCode_ = Hash::hash(*intArrRep);
  delete intArrRep;
  intPreds_->compress();
}

  // Copy constructor
IntClause::IntClause(const IntClause& ic) 
{
  wt_ = ic.getWt();
  
  intPreds_ = new Array<int>;
  intPreds_->growToSize(ic.intPreds_->size());
  for (int i = 0; i < ic.intPreds_->size(); i++)
    (*intPreds_)[i] = (*ic.intPreds_)[i];
  
  hashCode_ = ic.hashCode_;
}
void IntClause::addWt(const double& wt) 
{ if (wt_ == LazyInfo::HARD_WT) return; wt_ += wt; }

void IntClause::setWt(const double& wt) 
{ if (wt_ == LazyInfo::HARD_WT) return; wt_ = wt; }

void IntClause::setWtToHardWt() { wt_ = LazyInfo::HARD_WT; }

bool IntClause::isHardClause() const { return (wt_ == LazyInfo::HARD_WT); }

/* 
 * Checks if this intclause is satisfied given the reference predHashArray
 * and the assignment in db.
 */
bool IntClause::isSatisfied(const PredicateHashArray* const & predHashArray,
					   		const Database* const & db) const
{
  for (int i = 0; i < intPreds_->size(); i++)
  {
  	int index = (*intPreds_)[i];
  	Predicate* pred = new Predicate((*(*predHashArray)[abs(index)-1]));
  	assert(pred);
  	if (index < 0) pred->setSense(false);
  	else pred->setSense(true);
  	TruthValue tv = db->getValue(pred);
    if (db->sameTruthValueAndSense(tv, pred->getSense()))
    {
      delete pred;
      return true;
    }
    delete pred;
  }
  return false;
}
