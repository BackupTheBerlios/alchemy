#ifndef _LW_Info_H_
#define _LW_Info_H_

#include<fstream>

#include "clause.h"
#include "clausefactory.h"
#include "lwutil.h"
#include "intclause.h"
#include "mln.h"

class LWInfo {

public:
	 
 //constructor	 
LWInfo(MLN *mln, Domain *domain)
{
  this->mln_ = mln;
  this->domain_ = domain;
  //LWUtil::initPredStatus(domain_);
  makePositiveWeights();
  setHardClauseWeight();
}
  
 //destructor
~LWInfo()
{
  for(int i = 0; i < predHashArray_.size(); i++)
	delete predHashArray_[i];
  delete mln_;
}
 
inline int getVarCount() { return predHashArray_.size(); }

/*
 * Makes a copy of the mln, flips the negative weights and distributes them
 * over the predicates in the clause. The sense of each predicate is flipped
 * and they are added as unit clauses to the copy of the mln.
 */
inline void makePositiveWeights()
{
  MLN* posmln = new MLN();
  int clauseCnt = mln_->getNumClauses();

	// For each clause, add it to copy (if pos. wt.) or flip and
	// add unit clauses to copy (if neg. wt.)
  for (int i = 0; i < clauseCnt; i++)
  {
  	Clause* clause = (Clause *) mln_->getClause(i);
	int numPreds = clause->getNumPredicates();
	if (clause->getWt() >= 0)
	{
		// Add clause to mln copy
	  int idx;
      ostringstream oss;
      Clause* newClause = new Clause(*clause);
      newClause->printWithoutWtWithStrVar(oss, domain_);
      bool app = posmln->appendClause(oss.str(), false, newClause,
      								  clause->getWt(), clause->isHardClause(), idx);
      if (app)
      {
		posmln->setFormulaNumPreds(oss.str(), numPreds);
      	posmln->setFormulaIsHard(oss.str(), clause->isHardClause());
      	posmln->setFormulaPriorMean(oss.str(), clause->getWt());
      }
      continue;
	}
	
	  // Negative weight, so flip and add unit clauses
	double negWt = -clause->getWt()/((double)numPreds);
    for (int i = 0; i < numPreds; i++)
    {
      Predicate* pred = clause->getPredicate(i);
	  Clause* unitClause = ClauseFactory::createUnitClause(pred, true);
	  unitClause->getPredicate(0)->setSense(!pred->getSense());
      
		// If unit clause already exists, then just add weight
	  if (int cIdx = posmln->findClauseIdx(unitClause) > -1) {
	  	((Clause *)posmln->getClause(cIdx))->addWt(negWt);
	  	delete unitClause;
	  	continue;
	  }

		// Unit clause does not exist, so add it
      int idx;
      ostringstream oss;
      unitClause->getPredicate(0)->getTemplate()->printWithStrVar(oss);
      bool app = posmln->appendClause(oss.str(), false, unitClause, negWt, false, idx);
      if (app)
      {
		posmln->setFormulaNumPreds(oss.str(), 1);
      	posmln->setFormulaIsHard(oss.str(), false);
      	posmln->setFormulaPriorMean(oss.str(), negWt);
      }
    }
  }  
  mln_ = posmln;
}

/*
 * Computes the hard clause weight from the first order clauses in the mln.
 * 
 */
inline void setHardClauseWeight()
{
	// This is the weight used if no soft clauses are present
  LWInfo::HARD_WT = 10.0;
  
  int clauseCnt = mln_->getNumClauses();
  double sumSoftWts = 0.0;
  double minWt = DBL_MAX;
  double maxWt = DBL_MIN;
  int maxAllowedWt = 4000;
  
	// Sum up the soft weights of all grounded clauses
  for (int clauseno = 0; clauseno < clauseCnt; clauseno++)
  {
    Clause* fclause = (Clause *) mln_->getClause(clauseno);
	
	if (fclause->isHardClause()) continue;
	
	double wt = fclause->getWt();
	double numGndings = fclause->getNumGroundings(domain_);

	if (wt < minWt) minWt = wt;
	if (wt > maxWt) maxWt = wt;
	sumSoftWts += wt*numGndings;
	assert(minWt >= 0);
    assert(maxWt >= 0);
  } //for(clauseno < clauseCnt)
  assert(sumSoftWts >= 0);

	// If at least one soft clause
  if (sumSoftWts > 0)
  {
  	  //find out how much to scale weights by
  	LWInfo::WSCALE = 1.0;
    if (maxWt > maxAllowedWt) LWInfo::WSCALE = maxAllowedWt/maxWt;
    else
    {
      if (minWt < 10)
      {
        LWInfo::WSCALE = 10/minWt;
        if (LWInfo::WSCALE*maxWt > maxAllowedWt) LWInfo::WSCALE = maxAllowedWt/maxWt;
      }
    }

	LWInfo::HARD_WT = (sumSoftWts + 10.0)*LWInfo::WSCALE;
  }
}

inline void reset()
{ 
  for(int i = 0; i < predHashArray_.size(); i++)
  {
	delete predHashArray_[i];
  }
  predHashArray_.clear(); 
  predArray_.clear();
}

/* check if the given atom is active or not */
inline bool isActive(int atom)
{
  return (domain_->getDB()->getActiveStatus(predArray_[atom]));
}

/* check if the given atom has been previously deactivated or not */
inline bool isDeactivated(int atom)
{
  return (domain_->getDB()->getDeactivatedStatus(predArray_[atom]));
}

/* set the given atom as active */
inline void setActive(int atom)
{
  domain_->getDB()->setActiveStatus(predArray_[atom], true);
}

void updatePredArray()
{
  int startindex = 0;
  if(predArray_.size() > 0)
    startindex = predArray_.size()-1;
	 
  predArray_.growToSize(predHashArray_.size()+1);
  for(int i = startindex; i < predHashArray_.size(); i++)
    predArray_[i+1] = predHashArray_[i];
}

/* add clauses */
void getWalksatClauses(Predicate *inputPred,
			    	   Array<Array<int> *> &walksatClauses,
				       Array<int> &walksatClauseWts)
{
  Clause *fclause;
  IntClause *intClause;
  int clauseCnt;
  IntClauseHashArray clauseHashArray;

  Array<IntClause *> *intClauses = new Array<IntClause *>; 
     
  const Array<IndexClause*>* indexClauses = NULL;
      
  if(inputPred == NULL)
  {
	clauseCnt = mln_->getNumClauses();
  }
  else
  {
  	if (domain_->getDB()->getDeactivatedStatus(inputPred)) return;
	int predid = inputPred->getId(); 
	indexClauses = mln_->getClausesContainingPred(predid);
	clauseCnt = indexClauses->size();
  }

  int clauseno = 0;
  while(clauseno < clauseCnt)
  {
	if(inputPred)
	  fclause = (Clause *) (*indexClauses)[clauseno]->clause;			
	else
	  fclause = (Clause *) mln_->getClause(clauseno);
	
	double wt = fclause->getWt();
    intClauses->clear();
	bool ignoreActivePreds = false;
	fclause->getActiveClauses(inputPred, domain_, intClauses,
							  &predHashArray_, ignoreActivePreds);
    updatePredArray();
	for (int i = 0; i < intClauses->size(); i++) 
    {
      intClause = (*intClauses)[i];
	  int pos = clauseHashArray.find(intClause);
	  if(pos >= 0)
	  {
		clauseHashArray[pos]->addWt(wt);
		intClause->deleteIntPredicates();
		delete intClause;
		continue;
	  }
           
	  intClause->setWt(wt);
	  clauseHashArray.append(intClause);
	}
	clauseno++;
	    
  } //while(clauseno < clauseCnt)
	  
  Array<int>* litClause;
  for(int i = 0; i < clauseHashArray.size(); i++)
  {
	intClause = clauseHashArray[i];
	double weight = intClause->getWt();
// Neg. weights are taken out when lwinfo is constructed
/*
	// If weight is negative, then flip the clause and distribute weights
	if (weight < 0)
	{
	  int numPreds = intClause->getNumIntPredicates();
	  double negWt = -weight/((double)numPreds);
      for (int i = 0; i < numPreds; i++)
      {
		int idx = intClause->getIntPredicate(i);
      	int lit = -idx;
      	litClause = new Array<int>(1);
      	litClause->append(lit);
      	if (int j = walksatClauses.find(litClause) >= 0)
      	{
      	  // litClause already present, then add weight to it
      	  walksatClauseWts[j] += (int)(negWt*WSCALE+0.5);
      	  delete litClause;
      	}
      	else
      	{
      	  // litClause not present, then append it
	      walksatClauses.append(litClause);
	 	  walksatClauseWts.append((int)(negWt*WSCALE+0.5));
      	}
      }
	}
	else
	{
*/
	litClause = (Array<int> *)intClause->getIntPredicates();
	walksatClauses.append(litClause);
	walksatClauseWts.append((int)(weight*LWInfo::WSCALE+0.5));
	
    delete intClause;
  }
	 	 
  delete intClauses;
}

/* get all the active clauses and append to the
 * allClauses array */
void getWalksatClauses(Array<Array<int> *> &allClauses, Array<int> &allClauseWeights)
{
  getWalksatClauses(NULL,allClauses,allClauseWeights);
}

/* Get all the clauses which become active by flipping this atom */
void getWalksatClausesWhenFlipped(int atom,            
								  Array<Array<int> *> &walksatClauses,
		  						  Array<int> &walksatClauseWts)
{
	 
  Predicate *pred = predArray_[atom];
  TruthValue oldval = domain_->getDB()->getValue(pred);
  TruthValue val;
  (oldval == TRUE)? val=FALSE : val = TRUE;
  domain_->getDB()->setValue(pred, val);
  getWalksatClauses(pred, walksatClauses, walksatClauseWts);
  domain_->getDB()->setValue(pred, oldval);
}

/* Get the cost of all the clauses which become unsatisfied by flipping this atom */
int getUnSatCostWhenFlipped(int atom)
{	 
  int unSatCnt;
  int unSatCost = 0;
  TruthValue val;
  Clause *clause;
  Predicate *pred = predArray_[atom];
  TruthValue oldval = domain_->getDB()->getValue(pred);

  (oldval == TRUE) ? val = FALSE: val = TRUE;
  domain_->getDB()->setValue(pred,val);

  const Array<IndexClause*>* indclauses;
  int predid = pred->getId(); 
  indclauses = mln_->getClausesContainingPred(predid);
     
  for(int j = 0; j < indclauses->size(); j++)
  {      
	clause = (Clause *) (*indclauses)[j]->clause;			
    int wt = (int)(clause->getWt()*LWInfo::WSCALE+0.5);
	if(wt < WEIGHT_EPSILON)
	  continue;
	bool ignoreActivePreds = true;
	unSatCnt = clause->getActiveClauseCnt(pred, domain_, &predHashArray_, ignoreActivePreds);
	updatePredArray();
	unSatCost += unSatCnt * wt;
  }
  domain_->getDB()->setValue(pred,oldval);
  return unSatCost;
}

/* set Database Values to the given array of values */
void setVarVals(int newVals[])
{
  for(int atom = 1; atom <= getVarCount(); atom++)
  {
    Predicate *ipred = predArray_[atom];
	if(newVals[atom] == 1)
	  domain_->getDB()->setValue(ipred,TRUE);
	else
	  domain_->getDB()->setValue(ipred,FALSE);
  }
}

/* flip the predicate corresponding to given atom */
void flipVar(int atom)
{
  Predicate *ipred = predArray_[atom];
  TruthValue val;
  val = domain_->getDB()->getValue(ipred);
  if(val == TRUE)
    domain_->getDB()->setValue(ipred,FALSE);
  else
	domain_->getDB()->setValue(ipred,TRUE);
}

/* get the value (true/false) of the particular variable */
bool getVarVal(int atom)
{
  Predicate *ipred = predArray_[atom];
  bool val;
  (domain_->getDB()->getValue(ipred) == TRUE)? val = true : val = false;
  return val;
}

Predicate* getVar(int atom)
{
  return predArray_[atom];	
}

void setAllActive()
{
  LWUtil::setAllActive(domain_);
}

void setAllInactive()
{
  for(int atom = 1; atom <= getVarCount(); atom++)
  {
	Predicate* ipred = predArray_[atom];
  	domain_->getDB()->setActiveStatus(ipred, false);
  }
}

/* Deactivates and removes atoms at indices indicated */
void removeVars(Array<int> indices)
{
  for (int i = 0; i < indices.size(); i++)
  {
  	Predicate* pred = predArray_[indices[i]];
  	domain_->getDB()->setActiveStatus(pred, false);
 	domain_->getDB()->setDeactivatedStatus(pred, true);
  }
  reset();
  cout << "reset " << endl;
}

public:
	//Weight of hard clauses
  static double HARD_WT;
	//Scale to use on weights
  static double WSCALE;

private:

  MLN *mln_;
  Domain *domain_;
  
  Array<Predicate *> predArray_;
  PredicateHashArray predHashArray_;
  
  Array<Array<int> *> predToClauseIds_;

};

#endif

