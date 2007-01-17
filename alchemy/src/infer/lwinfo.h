#ifndef _LW_Info_H_
#define _LW_Info_H_

#include<fstream>
#include <sys/time.h>

#include "clause.h"
#include "clausefactory.h"
#include "lwutil.h"
#include "intclause.h"
#include "mln.h"
#include "timer.h"

class LWInfo
{
public:

 //constructor	 
LWInfo(MLN *mln, Domain *domain)
{
      // Randomizer
  int seed;
  struct timeval tv;
  struct timezone tzp;
  gettimeofday(&tv,&tzp);
  seed = (unsigned int)((( tv.tv_sec & 0177 ) * 1000000) + tv.tv_usec);
  srandom(seed);
    
  this->mln_ = mln;
  this->domain_ = domain;
  copyMLN();
  setHardClauseWeight();
  sampleSat_ = false;
  prevDB_ = NULL;
  numDBAtoms_ = domain_->getNumNonEvidenceAtoms();
  initBlocks();
  inBlock_ = false;
}
  
 //destructor
~LWInfo()
{

  for(int i = 0; i < predHashArray_.size(); i++)
	delete predHashArray_[i];

    // mln_ is a copy made in copyMLN()
  delete mln_;

  if (prevDB_) delete prevDB_;

  for(int i = 0; i < deadClauses_.size(); i++)
  {
  	deadClauses_[i]->deleteIntPredicates();
	delete deadClauses_[i];
  }
}
 
inline int getVarCount() { return predHashArray_.size(); }

/*
 * Makes a copy of the mln.
 */
inline void copyMLN()
{

  MLN* posmln = new MLN();
  int clauseCnt = mln_->getNumClauses();

	// For each clause, add it to copy
  for (int i = 0; i < clauseCnt; i++)
  {
  	Clause* clause = (Clause *) mln_->getClause(i);
    if (clause->getWt() == 0) continue;
	int numPreds = clause->getNumPredicates();

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
  }
  mln_ = posmln;
  //mln_->printMLN(cout, domain_);
}

/*
 * Computes the hard clause weight from the first order clauses in the mln.
 */
inline void setHardClauseWeight()
{
	// This is the weight used if no soft clauses are present
  LWInfo::HARD_WT = 10.0;
  
  int clauseCnt = mln_->getNumClauses();
  double sumSoftWts = 0.0;
  double minWt = DBL_MAX;
  double maxWt = DBL_MIN;
    //this is a reasonable number to prevent the external executable 
    //MaxWalksat from have having overflow errors
  int maxAllowedWt = 4000;
  
	// Sum up the soft weights of all grounded clauses
  for (int clauseno = 0; clauseno < clauseCnt; clauseno++)
  {
    Clause* fclause = (Clause *) mln_->getClause(clauseno);
	
	if (fclause->isHardClause()) continue;
	
      // Weight could be negative
	double wt = abs(fclause->getWt());

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
  //cout << "Set hard weight to " << LWInfo::HARD_WT << endl;
}

/*
 * Removes all soft clauses from the mln.
 */
inline void removeSoftClauses()
{
	// For each clause, remove it if not hard
  for (int i = 0; i < mln_->getNumClauses(); i++)
  {
  	Clause* clause = (Clause *) mln_->getClause(i);
	if (!clause->isHardClause())
	{
	  mln_->removeClause(i);
	  i--;
	}
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

/* set the given atom as inactive */
inline void setInactive(int atom)
{
  domain_->getDB()->setActiveStatus(predArray_[atom], false);
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

  // Gets the set of all possible initial active clauses
void getSupersetClauses(Array<IntClause *> &supersetClauses)
{
    // Randomizer
  int seed;
  struct timeval tv;
  struct timezone tzp;
  gettimeofday(&tv,&tzp);
  seed = (unsigned int)((( tv.tv_sec & 0177 ) * 1000000) + tv.tv_usec);
  srandom(seed);
  
  Clause *fclause;
  IntClause *intClause;
  int clauseCnt;
  IntClauseHashArray clauseHashArray;

  Array<IntClause *>* intClauses = new Array<IntClause *>; 
  
  clauseCnt = mln_->getNumClauses();

  int clauseno = 0;
  while(clauseno < clauseCnt)
  {
    fclause = (Clause *) mln_->getClause(clauseno);
    
    double wt = fclause->getWt();
    intClauses->clear();
    bool ignoreActivePreds = false;
    fclause->getActiveClauses(NULL, domain_, intClauses,
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
      
  for(int i = 0; i < clauseHashArray.size(); i++)
  {
    intClause = clauseHashArray[i];
    supersetClauses.append(intClause);
  }
  delete intClauses;
}


  // Perform clause selection on the superset of initial active
  // clauses and convert weights
void selectClauses(const Array<IntClause *> &supersetClauses,
                   Array<Array<int> *> &walksatClauses,
                   Array<int> &walksatClauseWts)
{
  assert(sampleSat_);
  
    // Randomizer
  int seed;
  struct timeval tv;
  struct timezone tzp;
  gettimeofday(&tv,&tzp);
  seed = (unsigned int)((( tv.tv_sec & 0177 ) * 1000000) + tv.tv_usec);
  srandom(seed);

  for(int i = 0; i < deadClauses_.size(); i++)
  {
    deadClauses_[i]->deleteIntPredicates();
    delete deadClauses_[i];
  }
  deadClauses_.clearAndCompress();

    // Look at each clause in the superset and do clause
    // selection and convert weight
  for (int i = 0; i < supersetClauses.size(); i++)
  {
    assert(prevDB_);
    IntClause* intClause = supersetClauses[i];
    double wt = intClause->getWt();
    if (wt == 0) continue;

      // Pos. clause not satisfied in prev. iteration: don't activate
      // Neg. clause satisfied in prev. iteration: don't activate
    bool sat = intClause->isSatisfied(&predHashArray_, prevDB_);
    if ((wt > 0 && !sat) ||
        (wt < 0 && sat))
    {
      continue;
    }

      // With prob. exp(-wt) don't ever activate it
    double threshold =
      intClause->isHardClause() ? RAND_MAX : RAND_MAX*(1-exp(-abs(wt)));

    if (random() > threshold)
    {
      deadClauses_.append(new IntClause(*intClause));
      continue;
    }

      // If we're here, then clause has been selected
    Array<int>* litClause = (Array<int> *)intClause->getIntPredicates();
    walksatClauses.append(new Array<int>(*litClause));
    if (wt >= 0) walksatClauseWts.append(1);
    else walksatClauseWts.append(-1);
  }
}


/** 
 * Get clauses and weights activated by the predicate inputPred,
 * if active is true. If false, inactive clauses (and their weights)
 * containing inputPred are retrieved.
 * If inputPred is NULL, then all active (or inactive) clauses and
 * their weights are retrieved.
 */
void getWalksatClauses(Predicate *inputPred,
			    	   Array<Array<int> *> &walksatClauses,
				       Array<int> &walksatClauseWts,
                       bool const & active)
{
    // Randomizer
  int seed;
  struct timeval tv;
  struct timezone tzp;
  gettimeofday(&tv,&tzp);
  seed = (unsigned int)((( tv.tv_sec & 0177 ) * 1000000) + tv.tv_usec);
  srandom(seed);
  
  Clause *fclause;
  IntClause *intClause;
  int clauseCnt;
  IntClauseHashArray clauseHashArray;

  Array<IntClause *>* intClauses = new Array<IntClause *>; 
  
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

    if (active)
    {
	  fclause->getActiveClauses(inputPred, domain_, intClauses,
	                            &predHashArray_, ignoreActivePreds);
    }
    else
    {
      fclause->getInactiveClauses(inputPred, domain_, intClauses,
                                  &predHashArray_);
    }
    updatePredArray();
cout << "intClauses size " << intClauses->size() << endl;
	for (int i = 0; i < intClauses->size(); i++)
    {
      intClause = (*intClauses)[i];

		// If using samplesat, then do clause selection
	  if (sampleSat_)
	  {
		assert(prevDB_);

	  	  // Pos. clause not satisfied in prev. iteration: don't activate
          // Neg. clause satisfied in prev. iteration: don't activate
        bool sat = intClause->isSatisfied(&predHashArray_, prevDB_);
	  	if ((wt >= 0 && !sat) ||
            (wt < 0 && sat))
	  	{
	  	  intClause->deleteIntPredicates();
	  	  delete intClause;
	  	  continue;
	  	}

          // In dead clauses: don't activate
        if (deadClauses_.contains(intClause))
        {
          intClause->deleteIntPredicates();
          delete intClause;
          continue;
        }

          // With prob. exp(-wt) don't ever activate it
		double threshold =
		  fclause->isHardClause() ? RAND_MAX : RAND_MAX*(1-exp(-abs(wt)));

		if (random() > threshold)
		{
		  deadClauses_.append(intClause);
		  continue;
		}
	  }

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
	litClause = (Array<int> *)intClause->getIntPredicates();
	walksatClauses.append(litClause);
	if (sampleSat_)
    {
      if (weight >= 0) walksatClauseWts.append(1);
      else walksatClauseWts.append(-1);
    }
	else
	{
      if (weight >= 0)
        walksatClauseWts.append((int)(weight*LWInfo::WSCALE + 0.5));
      else
        walksatClauseWts.append((int)(weight*LWInfo::WSCALE - 0.5));
	}
	
    delete intClause;
  }
	 	 
  delete intClauses;
}

/* get all the active clauses and append to the
 * allClauses array */
void getWalksatClauses(Array<Array<int> *> &allClauses, Array<int> &allClauseWeights)
{
  getWalksatClauses(NULL, allClauses, allClauseWeights, true);
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
  
    // Used to store other pred flipped in block
  Predicate* otherPred = NULL;
  inBlock_ = false;
  
  int blockIdx = domain_->getBlock(pred);
  if (blockIdx >= 0)
  {
      // Dealing with pred in a block
    const Array<Predicate*>* block = domain_->getPredBlock(blockIdx);
    if (block->size() > 1)
    {
      inBlock_ = true;
      int chosen = -1;
        // 1->0: Pick one at random to flip
      if (oldval == TRUE)
      {
        bool ok = false;
        while(!ok)
        {
          chosen = random() % block->size();
          if (!pred->same((*block)[chosen]))
            ok = true;
        }
        assert(domain_->getDB()->getValue((*block)[chosen]) == FALSE);
      }
        // 0->1: Flip the pred with value 1
      else
      {
        for (int i = 0; i < block->size(); i++)
        {
          if (domain_->getDB()->getValue((*block)[i]) == TRUE)
          {
            chosen = i;
            assert(!pred->same((*block)[chosen]));
            break;
          }
        }
      }
      assert(chosen >= 0);
      otherPred = (*block)[chosen];
        // Set new value of other atom in block
      domain_->getDB()->setValue(otherPred, oldval);  
    }
  }
    // Set new value of actual atom
  domain_->getDB()->setValue(pred, val);

  getWalksatClauses(pred, walksatClauses, walksatClauseWts, true);
  if (inBlock_)
    getWalksatClauses(otherPred, walksatClauses, walksatClauseWts, true);
    
    // Set old value of atom and other in block
  domain_->getDB()->setValue(pred,oldval);
  if (inBlock_)
  {
    domain_->getDB()->setValue(otherPred, val);
    
      // Set index of other pred
    for(int atom = 1; atom <= getVarCount(); atom++)
    {
      if (otherPred->same(predArray_[atom]))
      {
        otherAtom_ = atom;
        break;
      }
    }
  }
}

/* Get the cost of all the clauses which become unsatisfied by flipping this atom */
int getUnSatCostPerPred(Predicate* pred,
                        Array<Array<int> *> &walksatClauses,
                        Array<int> &walksatClauseWts)
{
    // Randomizer
  int seed;
  struct timeval tv;
  struct timezone tzp;
  gettimeofday(&tv,&tzp);
  seed = (unsigned int)((( tv.tv_sec & 0177 ) * 1000000) + tv.tv_usec);
  srandom(seed);
  
  int unSatCost = 0;
  Clause *clause;
  IntClause *intClause;
  IntClauseHashArray clauseHashArray;

  Array<IntClause *>* intClauses = new Array<IntClause *>; 

  const Array<IndexClause*>* indclauses;
  int predid = pred->getId(); 
  indclauses = mln_->getClausesContainingPred(predid);
  
  for(int j = 0; j < indclauses->size(); j++)
  {      
    clause = (Clause *) (*indclauses)[j]->clause;
/*    
    double weight = clause->getWt()*LWInfo::WSCALE;
    //int wt = abs((int)(clause->getWt()*LWInfo::WSCALE+0.5));
    int wt;

      // Samplesat: all weights are 1 or -1
    if (sampleSat_)
    {
      if (weight >= 0)
        wt = 1;
      else
        wt = -1;
    }
    else
    {
      if (weight >= 0)
        wt = (int)(weight + 0.5);
      else
        wt = (int)(weight - 0.5);
    }
*/
    double wt = clause->getWt();
    intClauses->clear();

    if(abs(wt) < WEIGHT_EPSILON)
      continue;
      
    bool ignoreActivePreds = true;
    clause->getActiveClauses(pred, domain_, intClauses,
                             &predHashArray_, ignoreActivePreds);
    updatePredArray();
    
    for (int i = 0; i < intClauses->size(); i++)
    {
      intClause = (*intClauses)[i];

        // If using samplesat, then do clause selection
      if (sampleSat_)
      {
        assert(prevDB_);

//cout << i << " wt " << wt << endl;

          // Pos. clause not satisfied in prev. iteration: don't activate
          // Neg. clause satisfied in prev. iteration: don't activate
        bool sat = intClause->isSatisfied(&predHashArray_, prevDB_);
        if ((wt >= 0 && !sat) ||
            (wt < 0 && sat))
        {
          intClause->deleteIntPredicates();
          delete intClause;
          continue;
        }

          // In dead clauses: don't activate
        if (deadClauses_.contains(intClause))
        {
          intClause->deleteIntPredicates();
          delete intClause;
          continue;
        }

          // With prob. exp(-wt) don't ever activate it
        double threshold =
          clause->isHardClause() ? RAND_MAX : RAND_MAX*(1-exp(-abs(wt)));

        if (random() > threshold)
        {
          deadClauses_.append(intClause);
          continue;
        }
      }

      int pos = clauseHashArray.find(intClause);
      if(pos >= 0)
      {
        clauseHashArray[pos]->addWt(wt);
        intClause->deleteIntPredicates();
        delete intClause;
        continue;
      }

//cout << "Hard wt. " << LWInfo::HARD_WT << endl;           
//cout << i << " wt2 " << wt << endl;
      if (wt == LWInfo::HARD_WT) intClause->setWtToHardWt();
      else intClause->setWt(wt);
      clauseHashArray.append(intClause);
//cout << i << " intClause->setWt " << intClause->getWt() << endl;
    }
    
  }
  
  Array<int>* litClause;
  for(int i = 0; i < clauseHashArray.size(); i++)
  {
    intClause = clauseHashArray[i];
//cout << i << " intClause->getWt() " << intClause->getWt() << endl;
    int weight = (int)(intClause->getWt());
    litClause = (Array<int> *)intClause->getIntPredicates();
    walksatClauses.append(litClause);
    if (sampleSat_)
    {
      if (weight >= 0) walksatClauseWts.append(1);
      else walksatClauseWts.append(-1);
      unSatCost += 1;
    }
    else
    {
      if (weight >= 0)
      {
        walksatClauseWts.append((int)(weight*LWInfo::WSCALE + 0.5));
        unSatCost += (int)(weight*LWInfo::WSCALE + 0.5);
      }
      else
      {
        walksatClauseWts.append((int)(weight*LWInfo::WSCALE - 0.5));
        unSatCost += (int)(weight*LWInfo::WSCALE - 0.5);
      }
    }

    //walksatClauseWts.append(weight);
//cout << i << " Weight " << weight << endl;
    //unSatCost += abs(weight);
    delete intClause;
  }
  delete intClauses;
  
  return unSatCost;
}

/* 
 * Get the cost of all the clauses which become unsatisfied by flipping this atom,
 * taking blocks into account
 */
int getUnSatCostWhenFlipped(int atom,
                            Array<Array<int> *> &walksatClauses,
                            Array<int> &walksatClauseWts)
{	 
  int unSatCost = 0;
  TruthValue val;
  Predicate *pred = predArray_[atom];
  TruthValue oldval = domain_->getDB()->getValue(pred);
  (oldval == TRUE) ? val = FALSE: val = TRUE;

    // Used to store other pred in block and its previous value
  Predicate* otherPred = NULL;
  bool inBlock = false;
  
  int blockIdx = domain_->getBlock(pred);
  if (blockIdx >= 0)
  {
      // Dealing with pred in a block
    const Array<Predicate*>* block = domain_->getPredBlock(blockIdx);
    if (block->size() > 1)
    {
      inBlock = true;
      int chosen = -1;
        // 1->0: Pick one at random to flip
      if (oldval == TRUE)
      {
        bool ok = false;
        while(!ok)
        {
          chosen = random() % block->size();
          if (!pred->same((*block)[chosen]))
            ok = true;
        }
        assert(domain_->getDB()->getValue((*block)[chosen]) == FALSE);
      }
        // 0->1: Flip the pred with value 1
      else
      {
        for (int i = 0; i < block->size(); i++)
        {
          if (domain_->getDB()->getValue((*block)[i]) == TRUE)
          {
            chosen = i;
            assert(!pred->same((*block)[chosen]));
            break;
          }
        }
      }
      assert(chosen >= 0);
      otherPred = (*block)[chosen];
        // Set new value of other atom in block
      domain_->getDB()->setValue(otherPred, oldval);  
    }
  }
    // Set new value of actual atom
  domain_->getDB()->setValue(pred, val);

    // Get unsat cost of pred and other pred
  unSatCost += getUnSatCostPerPred(pred, walksatClauses, walksatClauseWts);
  if (inBlock)
    unSatCost += getUnSatCostPerPred(otherPred, walksatClauses, walksatClauseWts);
    
    // Set old value of atom and other in block
  domain_->getDB()->setValue(pred,oldval);
  if (inBlock)
    domain_->getDB()->setValue(otherPred, val);  
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

/* sets the value (true/false) of the particular variable */
void setVarVal(int atom, bool val)
{
  Predicate *ipred = predArray_[atom];
  if (val)
    domain_->getDB()->setValue(ipred, TRUE);
  else
    domain_->getDB()->setValue(ipred, FALSE);
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

void setAllFalse()
{
  for(int atom = 1; atom <= getVarCount(); atom++)
  {
	Predicate* ipred = predArray_[atom];
  	domain_->getDB()->setValue(ipred, FALSE);
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
  //cout << "reset " << endl;
}

void setSampleSat(bool s)
{
  sampleSat_ = s;
}

bool getSampleSat()
{
  return sampleSat_;
}

  //Make a deep copy of the assignments from last iteration
void setPrevDB()
{
  if (prevDB_) { delete prevDB_; prevDB_ = NULL; }
  prevDB_ = new Database((*domain_->getDB()));
}

int getNumDBAtoms()
{
  return numDBAtoms_;
}

/*
 * Attempts to activate a random atom not already in memory and gets the
 * clauses activated by doing this. Returns true if successful, otherwise false.
 */
bool activateRandomAtom(Array<Array<int> *> &walksatClauses,
                        Array<int> &walksatClauseWts,
                        int& toflip)
{
    // Randomizer
  int seed;
  struct timeval tv;
  struct timezone tzp;
  gettimeofday(&tv,&tzp);
  seed = (unsigned int)((( tv.tv_sec & 0177 ) * 1000000) + tv.tv_usec);
  srandom(seed);

  Predicate* pred;
  pred = domain_->getNonEvidenceAtom(random() % numDBAtoms_);

  int pos = -1;
  if ((pos = predHashArray_.find(pred)) == -1)
  {
    domain_->getDB()->setActiveStatus(pred, true);
    predHashArray_.append(pred);
    updatePredArray();
    toflip = predHashArray_.size();
    getWalksatClauses(pred, walksatClauses, walksatClauseWts, true);
    return true;
  }
  else
  {
    delete pred;
    toflip = pos + 1;
    return false;
  }
}

/* If atom is in block, then another atom is chosen to flip and activated */
void chooseOtherToFlip(int atom,
                       Array<Array<int> *> &walksatClauses,
                       Array<int> &walksatClauseWts)
{
    // atom is assumed to be active
  assert(isActive(atom));
  Predicate *pred = predArray_[atom];
  TruthValue oldval = domain_->getDB()->getValue(pred);
  TruthValue val;
  (oldval == TRUE)? val = FALSE : val = TRUE;
  
    // Used to store other pred flipped in block
  Predicate* otherPred = NULL;
  inBlock_ = false;
  int blockIdx = domain_->getBlock(pred);

  if (blockIdx < 0)
  {
    return;
  }
  else
  {
      // Dealing with pred in a block
    const Array<Predicate*>* block = domain_->getPredBlock(blockIdx);
    if (block->size() > 1)
    {
      inBlock_ = true;
      int chosen = -1;
        // 1->0: Pick one at random to flip
      if (oldval == TRUE)
      {
        bool ok = false;
        while(!ok)
        {
          chosen = random() % block->size();
          if (!pred->same((*block)[chosen]))
            ok = true;
        }
        assert(domain_->getDB()->getValue((*block)[chosen]) == FALSE);
      }
        // 0->1: Flip the pred with value 1
      else
      {
        for (int i = 0; i < block->size(); i++)
        {
          if (domain_->getDB()->getValue((*block)[i]) == TRUE)
          {
            chosen = i;
            assert(!pred->same((*block)[chosen]));
            break;
          }
        }
      }
      assert(chosen >= 0);
      otherPred = (*block)[chosen];
        // Set new value of other atom in block
      domain_->getDB()->setValue(otherPred, oldval);  
    }
  }
    // Set new value of actual atom
  domain_->getDB()->setValue(pred, val);
  if (inBlock_)
    getWalksatClauses(otherPred, walksatClauses, walksatClauseWts, true);
    
    // Set old value of atom and other in block
  domain_->getDB()->setValue(pred,oldval);
  if (inBlock_)
  {
    domain_->getDB()->setValue(otherPred, val);
    
      // Set index of other pred
    for(int atom = 1; atom <= getVarCount(); atom++)
    {
      if (otherPred->same(predArray_[atom]))
      {
        otherAtom_ = atom;
        break;
      }
    }
  }
}

  // Sets the truth values of all atoms in the
  // block blockIdx except for the one with index atomIdx
void setOthersInBlockToFalse(const int& atomIdx,
                             const int& blockIdx)
{
  const Array<Predicate*>* block = domain_->getPredBlock(blockIdx);
  for (int i = 0; i < block->size(); i++)
  {
    if (i != atomIdx)
      domain_->getDB()->setValue((*block)[i], FALSE);
  }
}

  // Makes an assignment to all preds in blocks which does not
  // violate the blocks. One randomly chosen pred in each box is set to true.
void initBlocks()
{
  const Array<Array<Predicate*>*>* blocks = domain_->getPredBlocks();
  const Array<bool>* blockEvidence = domain_->getBlockEvidenceArray();
  
  for (int i = 0; i < blocks->size(); i++)
  {
    Array<Predicate*>* block = (*blocks)[i];
    int chosen = -1;
      // If evidence atom exists, then all others are false
    if ((*blockEvidence)[i])
    {
      chosen = domain_->getEvidenceIdxInBlock(i);
      setOthersInBlockToFalse(chosen, i);
      continue;
    }
    else
    {
        // Set one at random to true
      chosen = random() % block->size();
    }
    assert(chosen >= 0);
    domain_->getDB()->setValue((*block)[chosen], TRUE);
    setOthersInBlockToFalse(chosen, i);
  }
}

int getBlock(int atom)
{
  const Array<Array<Predicate*>*>* blocks = domain_->getPredBlocks();
  Predicate* pred = getVar(atom);
  
  for (int i = 0; i < blocks->size(); i++)
  {
    Array<Predicate*>* block = (*blocks)[i];
    for (int j = 0; j < block->size(); j++)
    {
      if ((*block)[j]->same(pred))
        return i;
    }
  }
  return -1;
}

bool inBlock(int atom)
{
  return (getBlock(atom) >= 0);
}

bool inBlockWithEvidence(int atom)
{
  int blockIdx = getBlock(atom);
  if (blockIdx >= 0 && domain_->getBlockEvidence(blockIdx))
    return true;
  return false;
}

bool getInBlock()
{
  return inBlock_;
}

void setInBlock(const bool val)
{
  inBlock_ = val;
}

int getOtherAtom()
{
  assert(inBlock_ && otherAtom_ >= 0);
  return otherAtom_;
}

void setOtherAtom(const int val)
{
  otherAtom_ = val;
}

  // Set evidence status of given atom to given value
void setEvidence(const int atom, const bool val)
{
  domain_->getDB()->setEvidenceStatus(predArray_[atom], val);
}

  // Get evidence status of given atom
bool getEvidence(const int atom)
{
  return domain_->getDB()->getEvidenceStatus(predArray_[atom]);
}

void incrementNumDBAtoms()
{
  numDBAtoms_++;
}

void decrementNumDBAtoms()
{
  numDBAtoms_--;
}

void printIntClauses(Array<IntClause *> clauses)
{
  for (int i = 0; i < clauses.size(); i++)
  {
    clauses[i]->printWithWtAndStrVar(cout, domain_, &predHashArray_);
    cout << endl;
  }
}


/* 
 * Fixes atoms using unit propagation on non-active clauses.
 * Fixed atoms are stored in fixedAtoms_ and set as evidence in the DB.
 */
void propagateFixedAtoms(Array<Array<int> *> &clauses,
                         Array<int> &clauseWeights,
                         bool* fixedAtoms,
                         int maxFixedAtoms)
{
  Array<Array<int> *> tmpClauses;
  Array<int> tmpClauseWeights;
    
    //TEMP: count fixed atoms
  int count = 0;
  for (int i = 0; i < maxFixedAtoms; i++)
  {
    if (fixedAtoms[i]) count++;
  }
  cout << "Fixed atoms before propagating: " << count << endl;
  
    // Look at all non-active clauses containing a fixed atom
  for (int i = 0; i < maxFixedAtoms; i++)
  {
    if (fixedAtoms[i])
    {
        // Get all non-active clauses containing it,
        // filtering out dead clauses
      getWalksatClauses(predArray_[i], tmpClauses, tmpClauseWeights, false);
cout << "Atom " << i << endl;
cout << "Clauses " << tmpClauses.size() << endl;

        // clauses now contains potential clauses of all length
        // want to look at only unit clauses
      for (int j = 0; j < tmpClauses.size(); j++)
      {
          // Unit clause
        if (tmpClauses[j]->size() == 1)
        {
          clauses.append(tmpClauses[j]);
          clauseWeights.append(clauseWeights[j]);
          fixedAtoms[(*tmpClauses[j])[0]] = true;
        }
        else // Not unit clause
        {
          delete tmpClauses[j];
        }
      }
    }
    tmpClauses.clear();
    tmpClauseWeights.clear();
  }
  
      //TEMP: count fixed atoms
  count = 0;
  for (int i = 0; i < maxFixedAtoms; i++)
  {
    if (fixedAtoms[i]) count++;
  }
  cout << "Fixed atoms after propagating: " << count << endl;
  
  exit(0);
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

	// Set to true when performing sample sat
  bool sampleSat_;
  	// Dead clauses not to be considered for activation
  IntClauseHashArray deadClauses_;
	// DB holding assignment from previous iteration
  Database* prevDB_;
    // Total number of non-evidence ground atoms in the DB
  int numDBAtoms_;
    // Flag indicating if an atom in a block is being flipped
  bool inBlock_;
    // Index of other atom to be flipped in block
  int otherAtom_;

};

#endif

