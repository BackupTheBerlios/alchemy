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
#include "clause.h"
#include "mrf.h"


ClauseSampler* Clause::clauseSampler_ = NULL;
double Clause::fixedSizeB_ = -1;
double AuxClauseData::fixedSizeB_ = -1;


  //returns true if the (ground) clause has two literals with opposite sense
  //i.e. the clause is satisfied; otherwise returns false
bool Clause::createAndAddUnknownClause(
                                Array<GroundClause*>* const& unknownGndClauses,
                                Array<Clause*>* const& unknownClauses,
                                double* const & numUnknownClauses,
                                const AddGroundClauseStruct* const & agcs)
{ 
  PredicateSet predSet; // used to detect duplicates
  PredicateSet::iterator iter;
  
  Clause* clause = NULL;
  for (int i = 0; i < predicates_->size(); i++)
  {
	Predicate* predicate = (*predicates_)[i];
    assert(predicate->isGrounded());
//cout << "Pred: ";
//cout << " " << predicate->getTruthValueAsStr();
//cout << endl;
    if (predicate->getTruthValue() == UNKNOWN)
    {
      if ( (iter=predSet.find(predicate)) != predSet.end() )
      {
          // the two gnd preds are of opp sense, so clause must be satisfied
        if ((*iter)->getSense() !=  predicate->getSense())
        {
          if (clause) delete clause;
          return true;
        }
        // since the two gnd preds are identical, no point adding a dup
        continue;
      }
      else
        predSet.insert(predicate);
      
      if (clause == NULL) clause = new Clause();
      Predicate* pred = new Predicate(*predicate, clause);
      clause->appendPredicate(pred);
    }
  }
  
  if (clause) 
  {
    if (numUnknownClauses) (*numUnknownClauses)++;

    clause->setWt(wt_);
    clause->canonicalizeWithoutVariables();
    
    if (agcs) MRF::addUnknownGndClause(agcs, this, clause, isHardClause_);
    
    // MARC: The case with unknownGndClauses appears to be obsolete!
	if (unknownGndClauses)
    {
      unknownGndClauses->append(new GroundClause(clause, agcs->gndPreds));
	  if (isHardClause_) unknownGndClauses->lastItem()->setWtToHardWt();
    }
    else
    if (unknownClauses)
    {
      unknownClauses->append(clause);
      if (isHardClause_) clause->setIsHardClause(true);
    }
    if (unknownClauses == NULL) delete clause;
  }
  return false;
}

void addPredicateToHash(const Clause* const & c,
                        PredicateHashArray* const & predHashArray)
{
  int numPreds = c->getNumPredicates();
  // for each predicate in clause c
  for (int i = 0; i < numPreds; i++)
  {
    Predicate* pred = new Predicate(*(c->getPredicate(i)));
	int index = predHashArray->find(pred);
	if(index < 0 )
	{
	  index = predHashArray->append(pred) + 1;
	}
	else
	{
	  delete pred;
	  index++;
	}
  }	 
}


/**
 * Creates and adds a ground active clause.
 * 
 * @param activeIntClauses If not NULL, then active IntClauses are accumulated
 * here.
 * @param activeGroundClauses If not NULL, then active GroundClauses are
 * accumulated here.
 * @param uniqueClauses
 * @param seenPreds Predicates which have been seen before. Used when
 * accumulating IntClauses.
 * @param uniquePreds
 * @param seenGndPreds GroundPredicates which have been seen before. Used when
 * accumulating GroundClauses.
 * @param db Database used to check truth values and evidence status of
 * predicates.
 * @param getSatisfied If true, satisfied clauses are also retrieved.
 */
bool Clause::createAndAddActiveClause(
                           Array<IntClause *> * const & activeIntClauses,
                           Array<GroundClause *> * const & activeGroundClauses,
		                   IntClauseHashArray * const & uniqueClauses,
		                   PredicateHashArray * const & seenPreds,
		                   PredicateHashArray * const & uniquePreds,
                           GroundPredicateHashArray* const& seenGndPreds,
		                   const Database* const & db,
                           bool const & getSatisfied)
{
  assert(!activeIntClauses || !activeGroundClauses);	 
  bool accumulateClauses = (activeIntClauses || activeGroundClauses);
  Predicate *cpred;
  PredicateSet predSet; // used to detect duplicates
  PredicateSet::iterator iter;
 
  IntClause *intClause;
  GroundClause *groundClause;
  
  Clause* fullClause = NULL;
  Clause* clause = NULL;
  bool isEmpty = true;
  for (int i = 0; i < predicates_->size(); i++)
  {
    Predicate* predicate = (*predicates_)[i];
    assert(predicate); 
	assert(predicate->isGrounded());
    if ( (iter = predSet.find(predicate)) != predSet.end() )
    {
        // The two gnd preds are of opp sense, so clause must be satisfied
        // and no point in adding it 
	  if (wt_ >= 0 && !getSatisfied &&
          (*iter)->getSense() !=  predicate->getSense())
      {
        if (fullClause) delete fullClause;
        if (clause) delete clause;
		return false;
      }

        // Since the two gnd preds are identical, no point adding a dup
      continue;
	}
    else
      predSet.insert(predicate);
      
    if (uniqueClauses)
    {
	  if (!fullClause) fullClause = new Clause();
     
	  cpred = new Predicate(*predicate, fullClause);
      assert(cpred);
	  fullClause->appendPredicate(cpred);
	}
	 
	bool isEvidence = db->getEvidenceStatus(predicate);
//cout << "isEvidence " << isEvidence << endl;
//predicate->printWithStrVar(cout, db->getDomain());
//cout << endl;
	if (!isEvidence)
      isEmpty = false;
	  
      // True evidence in a neg. clause: Discard clause
    if (wt_ < 0 && isEvidence && !getSatisfied &&
        db->sameTruthValueAndSense(db->getValue(predicate),
                                   predicate->getSense()))
    {
      if (clause) delete clause;
      if (fullClause) delete fullClause;
      return false;
    }
    
	  // Add only non-evidence prdicates
	if (accumulateClauses && !isEvidence)
	{
	  if (!clause) clause = new Clause();
        
	  cpred = new Predicate(*predicate, clause);
      assert(cpred);
	  clause->appendPredicate(cpred);
	}
  }

    // If the clause is empty after taking evidence into account, it should 
    // be discarded
  if (isEmpty)
  {
	assert(!clause);
	if (fullClause)
	  delete fullClause;
    return false;
  }
    // Came to this point means that the clause is active (and hence nonEmpty)
  else
  {
  	  // Check if the clause is already present
  	if (uniqueClauses)
  	{
	  assert(uniquePreds);
	  assert(fullClause);  
     
	 	// Note: fullClause should not be canonicalized
	  intClause = new IntClause(fullClause, uniquePreds);
	  delete fullClause; 
	 
	 	// If the clause is already present then it should
	 	// not be added again
	  if (uniqueClauses->append(intClause) < 0)
	  {
        intClause->deleteIntPredicates();
        delete intClause;
        if (clause)
	      delete clause;
	  	return false;
      } 
  	} 
    
   	  // Add the corresponding ground clause if accumulateClauses is true
   	if (accumulateClauses)
   	{
	  assert(clause);	
      clause->canonicalizeWithoutVariables();
    
	  if (activeIntClauses)
	  {
        intClause = new IntClause(clause, seenPreds);
   
        if (isHardClause_) 
          intClause->setWtToHardWt();
	    activeIntClauses->append(intClause);
      }
      else
      {
        groundClause = new GroundClause(clause, seenGndPreds);
/*
          // Check if preds in clause have been seen
        for (int i = 0; i < groundClause->getNumGroundPredicates(); i++)
        {
          GroundPredicate* gp =
            (GroundPredicate*)groundClause->getGroundPredicate(i);
          int index = seenGndPreds->find(gp);
            // Pred has been seen: Use pointer to previously seen one and tell
            // it that it appears in this clause
          if (index >= 0)
          {
            groundClause->replaceGroundPredicate(i, (*seenGndPreds)[index]);
            bool predSense = groundClause->getGroundPredicateSense(i);
            bool ok = (*seenGndPreds)[index]->appendGndClause(groundClause,
                                                              predSense);
            ok = true; // avoid compilation warning
          }
            // Pred has not been seen, so append it to seen
          else
            index = seenGndPreds->append(gp);
          groundClause->setGroundPredicateIndex(i, index);
        }
*/     
        if (isHardClause_) 
          groundClause->setWtToHardWt();
	    activeGroundClauses->append(groundClause);
      }
	  delete clause;
    }
    return true;
  } 
}


