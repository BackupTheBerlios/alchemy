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

const double HARD_INTCLAUSE_WT = DBL_MAX;

class IntClause
{
 public:

IntClause(const Clause* const & c, PredicateHashArray* const & predHashArray);
  
	 
~IntClause() { 
  //if(intArrRep_) delete intArrRep_;
 }
 
  void deleteIntPredicates(){
	 if (intPreds_) delete intPreds_;
  } 

  void addWt(const double& wt) 
  { if (wt_ == HARD_INTCLAUSE_WT) return; wt_ += wt; }

  void setWt(const double& wt) 
  { if (wt_ == HARD_INTCLAUSE_WT) return; wt_ = wt; }

  double getWt() const { return wt_; }

  void setWtToHardWt() { wt_ = HARD_INTCLAUSE_WT; }

  bool isHardClause() const { return (wt_ == HARD_INTCLAUSE_WT); }

  int getNumIntPredicates() const { return intPreds_->size(); }

  const int getIntPredicate(const int& i) const 
  { return (*intPreds_)[i]; }
  
  const Array<int>* getIntPredicates() const 
  { return intPreds_; }

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


 private:
  size_t hashCode_;

  //Array<unsigned int>* intArrRep_;  
  
  Array<int>* intPreds_;
  
  // overloaded to indicate whether this is a hard clause
  // if this is a hard clause, wt_ is set to HARD_INTCLAUSE_WT
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

