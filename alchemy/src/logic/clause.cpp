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
    
	if (unknownGndClauses)
    {
      unknownGndClauses->append(new GroundClause(clause));
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

void addPredicateToHash(const Clause* const & c, PredicateHashArray* const & predHashArray)
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


//create and add ground active clause
bool Clause::createAndAddActiveClause(Array<IntClause *> * const & activeIntClauses,
                                      Array<GroundClause *> * const & activeGroundClauses,
		                              IntClauseHashArray * const & uniqueClauses,
		                              PredicateHashArray * const & seenPreds,
		                              PredicateHashArray * const & uniquePreds,
		                              const Database* const & db)
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
    if ( (iter=predSet.find(predicate)) != predSet.end() )
    {
          // the two gnd preds are of opp sense, so clause must be satisfied
		  // and no point in adding it 
	  if ((*iter)->getSense() !=  predicate->getSense())
      {
        if (fullClause) delete fullClause;
        if (clause) delete clause;
		return false;
      }

        // since the two gnd preds are identical, no point adding a dup
      continue;
	}
    else
      predSet.insert(predicate);
      
    if(uniqueClauses)
    {
	  if (!fullClause) fullClause = new Clause();
     
	  cpred = new Predicate(*predicate, fullClause);
      assert(cpred);
	  fullClause->appendPredicate(cpred);
	}
	 
	bool isEvidence = db->getEvidenceStatus(predicate);
	if(!isEvidence)
	  isEmpty = false;
	  
	  //add only non-evidence prdicates
	if(accumulateClauses && !isEvidence)
	{
	  if (!clause) clause = new Clause();
        
	  cpred = new Predicate(*predicate, clause);
      assert(cpred);
	  clause->appendPredicate(cpred);
	}
  }

  //if the clause is empty after taking evidence into account, it should 
  //be discarded
  if(isEmpty)
  {
	assert(!clause);
	if(fullClause)
	  delete fullClause;
    return false;
  }
  //came at this point means that the clause is active (and hence nonEmpty)
  else
  {
	   
  	//check if the clause is already present
  	if(uniqueClauses)
  	{
	  assert(uniquePreds);
	  assert(fullClause);  
     
	 	//Note: fullClause should not be canonicalized
	  intClause = new IntClause(fullClause,uniquePreds);
	  delete fullClause; 
	 
	 	//if the clause is already present then it should
	 	//not be added again
	  if(uniqueClauses->append(intClause) < 0)
	  {
	  	intClause->deleteIntPredicates();
	  	delete intClause;
	  	if(clause)
	      delete clause;
	  	return false;
      } 
  	} 
    
   	//add the corresponding ground clause if accumulateClauses is true
   	if(accumulateClauses)
   	{
	  assert(clause);	
      clause->canonicalizeWithoutVariables();
    
	  if(activeIntClauses)
	  {
	  	intClause = new IntClause(clause,seenPreds);
   
       	if (isHardClause_) 
      	  intClause->setWtToHardWt();
   
	    activeIntClauses->append(intClause);
	  }
	  else
	  {
	    groundClause = new GroundClause(clause);
     
        if (isHardClause_) 
          groundClause->setWtToHardWt();
   
	    activeGroundClauses->append(groundClause);
	  }
	
	  delete clause;
    }
    return true;
  } 
}


