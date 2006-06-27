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

const double HARD_GROUNDCLAUSE_WT = DBL_MAX;

class Domain;
class Clause;
class GroundPredicate;
class HashGroundPredicate;
class EqualGroundPredicate;
typedef HashArray<GroundPredicate*, HashGroundPredicate, EqualGroundPredicate> 
 GroundPredicateHashArray;


class GroundClause
{
 public:
  GroundClause(const Clause* const & c);

    //The GroundPredicate* in gndPreds are not deleted. 
    //Use deleteAllGndPreds() to do so.
  ~GroundClause() 
  { 
    if (intArrRep_)      delete intArrRep_; 
    if (gndPredSenses_)  delete gndPredSenses_;
    if (gndPredIndexes_) delete gndPredIndexes_;
    if (numSatLiterals_) delete numSatLiterals_;
    delete gndPreds_; 
    if (parentWtPtrs_) delete parentWtPtrs_;
    if (foClauseFrequencies_) delete foClauseFrequencies_;
  
  }

  void deleteAllGndPreds();

  void deleteIntArrRep() 
  { if (intArrRep_) delete intArrRep_;  intArrRep_ = NULL;}

  void deleteGndPredSenses() 
  { if (gndPredSenses_) delete gndPredSenses_; gndPredSenses_ = NULL; }

  void deleteGndPredIndexes()
  { if (gndPredIndexes_) delete gndPredIndexes_; gndPredIndexes_ = NULL; }

    //the GroundPredicates that make up this GroundClause must already have 
    //their truth values set
  void initNumSatLiterals(const int& numChains);


  const Array<int>* getNumSatLiterals() const { return numSatLiterals_; }

  int getNumSatLiterals(const int& idx) const { return (*numSatLiterals_)[idx];}
  

  void setNumSatLiterals(const int& idx, const int& n) 
  { 
    assert(n >= 0); 
    (*numSatLiterals_)[idx] = n; 
  }

  void incrementNumSatLiterals(const int& idx) 
  {
    ((*numSatLiterals_)[idx])++;
    assert((*numSatLiterals_)[idx] >= 0);
  }


  void decrementNumSatLiterals(const int& idx) 
  {
    ((*numSatLiterals_)[idx])--;
    assert((*numSatLiterals_)[idx] >= 0);
  }

  void addWt(const double& wt) 
  { if (wt_ == HARD_GROUNDCLAUSE_WT) return; wt_ += wt; }

  void setWt(const double& wt) 
  { if (wt_ == HARD_GROUNDCLAUSE_WT) return; wt_ = wt; }

  double getWt() const { return wt_; }

  void setThreshold(const double& threshold) 
  { threshold_ = threshold; }

  double getThreshold() const { return threshold_; }


  void setWtToHardWt() { wt_ = HARD_GROUNDCLAUSE_WT; }
  bool isHardClause() const { return (wt_ == HARD_GROUNDCLAUSE_WT); }

  int getNumGroundPredicates() const { return gndPreds_->size(); }

  const GroundPredicate* getGroundPredicate(const int& i) const 
  { return (*gndPreds_)[i]; }

  void replaceGroundPredicate(const int& i, GroundPredicate* const & gndPred);

  bool getGroundPredicateSense(const int& i) const 
  { return (*gndPredSenses_)[i]; }

  void setGroundPredicateIndex(const int& i, const int& gndPredIdx) 
  { (*gndPredIndexes_)[i] = gndPredIdx; }

  int getGroundPredicateIndex(const int& i) const 
  { return (*gndPredIndexes_)[i]; }


  void appendParentWtPtr(const double* const & wtPtr) 
  {
    if (parentWtPtrs_ == NULL) parentWtPtrs_ = new Array<const double*>;
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
	if(!foClauseFrequencies_) return 0;
	IntPairItr itr = foClauseFrequencies_->find(clauseno);
	if(itr == foClauseFrequencies_->end()) 
	  return 0;
	else
	  return itr->second;
  }
  
  void incrementClauseFrequency(int clauseno, int increment)
  {
	if(!foClauseFrequencies_) foClauseFrequencies_ = new IntPair;
	IntPairItr itr = foClauseFrequencies_->find(clauseno);
	if(itr == foClauseFrequencies_->end()) 
	  foClauseFrequencies_->insert(IntPair::value_type(clauseno,increment));
	else
	  itr->second+=increment;
  }

  size_t hashCode() { return intArrRep_->lastItem(); }
  
  const Array<unsigned int>* getIntArrRep() const { return intArrRep_; }

  bool same(const GroundClause* const & gc)
  {
    if (intArrRep_->size() != gc->getIntArrRep()->size()) return false;
    return (memcmp(intArrRep_->getItems(), gc->getIntArrRep()->getItems(), 
                   (intArrRep_->size())*sizeof(unsigned int)) == 0);
  }

  void printWithoutWt(ostream& out, const Domain* const & domain) const;
  void printWithoutWt(ostream& out) const;
  void print(ostream& out, const Domain* const & domain) const;
  void print(ostream& out) const;

  double sizeKB();

 private:
  Array<unsigned int>* intArrRep_;  // hash code is stored as the last item
  Array<GroundPredicate*>* gndPreds_;
  Array<bool>* gndPredSenses_;
  Array<int>* gndPredIndexes_;

    //numSatLiterals[c] is the numSatLiterals for MCMC chain c
  Array<int>* numSatLiterals_; 
    // overloaded to indicate whether this is a hard clause
    // if this is a hard clause, wt_ is set to HARD_GROUNDCLAUSE_WT
  double wt_;

  double threshold_;

    // pointers to weights of first-order clauses that, after being grounded 
    // and having their ground predicates with known truth values removed, 
    // are equal to ground clause
  Array<const double*>* parentWtPtrs_;
  
  IntPair* foClauseFrequencies_;
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
