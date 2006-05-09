#include "intclause.h"
#include "clause.h"
#include "lwinfo.h"

double LWInfo::HARD_WT = 10.0;
double LWInfo::WSCALE = 1.0;

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

void IntClause::addWt(const double& wt) 
{ if (wt_ == LWInfo::HARD_WT) return; wt_ += wt; }

void IntClause::setWt(const double& wt) 
{ if (wt_ == LWInfo::HARD_WT) return; wt_ = wt; }

void IntClause::setWtToHardWt() { wt_ = LWInfo::HARD_WT; }

bool IntClause::isHardClause() const { return (wt_ == LWInfo::HARD_WT); }
