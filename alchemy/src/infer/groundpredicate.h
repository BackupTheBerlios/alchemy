#ifndef GROUNDPREDICATE_H_JUN_26_2005
#define GROUNDPREDICATE_H_JUN_26_2005

#include <cmath>
#include "domain.h"
#include "predicate.h"
#include "groundclause.h"

class GroundPredicate
{
 public:
  GroundPredicate(Predicate* const & pred)
    : gndClauses_(new Array<GroundClause*>),gndClauseSet_(new GroundClauseSet),
      senseInGndClauses_(new Array<bool>), truthValues_(NULL), 
      wtsWhenFalse_(NULL), wtsWhenTrue_(NULL) , numTrue_(0)
  {
    assert(pred->isGrounded());
    int numTerms = pred->getNumTerms();    
    intArrRep_ = new Array<unsigned int>(numTerms+1);
    intArrRep_->growToSize(numTerms+1);
    (*intArrRep_)[0] = pred->getId();
    assert((*intArrRep_)[0] >= 0);
    for (int i = 0; i < numTerms; i++)
    {
      (*intArrRep_)[i+1] = pred->getTerm(i)->getId();
      assert((*intArrRep_)[i+1] >= 0);
    }
    intArrRep_->compress();
    hashCode_ = Hash::hash(*intArrRep_);
  }

  
  ~GroundPredicate() 
  { 
    if (intArrRep_)    delete intArrRep_; 
    if (truthValues_)  delete truthValues_;
    if (wtsWhenFalse_) delete wtsWhenFalse_;
    if (wtsWhenTrue_)  delete wtsWhenTrue_;
    if (gndClauseSet_) delete gndClauseSet_;
    delete gndClauses_;
    delete senseInGndClauses_;
  }


  void deleteGndClauseSet() 
  { if (gndClauseSet_) delete gndClauseSet_; gndClauseSet_ = NULL; }


    // the caller is responsible for deleting the returned Predicate*
  Predicate* createEquivalentPredicate(const Domain* const & domain)
  {
    const PredicateTemplate* pt = domain->getPredicateTemplate(getId());
    assert(pt);
    Predicate* pred = new Predicate(pt);
    for (int j = 0; j < pt->getNumTerms(); j++)
      pred->appendTerm(new Term(getTermId(j), (void*)pred, true));
    return pred;
  }


  void initTruthValues(const int& numChains)
  {
    if (truthValues_) delete truthValues_;
    truthValues_ = new Array<bool>;
    truthValues_->growToSize(numChains, false);
  }  


  void initTruthValuesAndWts(const int& numChains)
  {
    initTruthValues(numChains);
    if (wtsWhenFalse_) delete wtsWhenFalse_;
    wtsWhenFalse_ = new Array<double>;
    wtsWhenFalse_->growToSize(numChains, 0);
    if (wtsWhenTrue_) delete wtsWhenTrue_;
    wtsWhenTrue_ = new Array<double>;
    wtsWhenTrue_->growToSize(numChains, 0);
  }


  unsigned int getId() const { return (*intArrRep_)[0]; }
  unsigned int getTermId(const int& idx) const { return (*intArrRep_)[idx+1]; }
  unsigned int getNumTerms() const { return intArrRep_->size() - 1; }

  const Array<bool>* getTruthValues() const { return truthValues_; }
  bool getTruthValue(const int& c) const { return (*truthValues_)[c]; }
  void setTruthValue(const int& c, const bool& tv) { (*truthValues_)[c] = tv; }

  const Array<double>* getWtsWhenFalse() const { return wtsWhenFalse_; }
  double getWtWhenFalse(const int& c) const { return (*wtsWhenFalse_)[c]; }
  void setWtWhenFalse(const int& c, const double& wt) {(*wtsWhenFalse_)[c]=wt;}
  void addWtWhenFalse(const int& c, const double& wt) {(*wtsWhenFalse_)[c]+=wt;}

  const Array<double>* getWtsWhenTrue() const { return wtsWhenTrue_; }
  double getWtWhenTrue(const int& c) const { return (*wtsWhenTrue_)[c]; }
  void setWtWhenTrue(const int& c, const double& wt) {(*wtsWhenTrue_)[c]=wt;}
  void addWtWhenTrue(const int& c, const double& wt) {(*wtsWhenTrue_)[c]+=wt;}

  double getNumTrue() const { return numTrue_; }
  void setNumTrue(const double& n) { assert(n >= 0); numTrue_ = n; }
  void incrementNumTrue() { numTrue_++; }

    //numTrue_ is overloaded to hold the probability that the predicate is true
  double getProbTrue() const { return numTrue_; }
  void setProbTrue(const double& p) { assert(p >= 0); numTrue_ = p; }


    // get the probability that the ground predicate is true
  double getProbability(const int& c)
  { return 1.0 / ( 1.0 + exp((*wtsWhenFalse_)[c] - (*wtsWhenTrue_)[c]) ); }


  void compress()
  {
    gndClauses_->compress();
    senseInGndClauses_->compress();
    //truthValues_, wtsWhenFalse_, wtsWhenTrue_ need not be compress because
    //they were assigned the correct exact size in the constructor
  }


  size_t hashCode() { return hashCode_; }
  
  const Array<unsigned int>* getIntArrRep() const { return intArrRep_; }

    // add a copy of this GroundPredicate's intArrRep_ to rep
  void appendIntArrRep(Array<unsigned int>& rep) { rep.append(*intArrRep_); }


  bool appendGndClause(GroundClause* const & gc, const bool& senseInGndClause) 
  { 
    if (gndClauseSet_->find(gc) == gndClauseSet_->end())
    {
      gndClauseSet_->insert(gc);
      gndClauses_->append(gc);
      senseInGndClauses_->append(senseInGndClause);
      return true;
    }
    return false;
  } 


  const Array<GroundClause*>* getGndClauses() const { return gndClauses_; }
  const Array<bool>* getSenseInGndClauses() const { return senseInGndClauses_;}


  bool same(const GroundPredicate* const & gp)
  {
    if (intArrRep_->size() != gp->getIntArrRep()->size()) return false;
    return (memcmp(intArrRep_->getItems(), gp->getIntArrRep()->getItems(), 
                   intArrRep_->size()*sizeof(unsigned int)) == 0);
  }


  void print(ostream& out, const Domain* const & domain) const
  {
    const char* predName = domain->getPredicateName((*intArrRep_)[0]);
    out << predName << "(";
    int size = intArrRep_->size();
    for (int i = 1; i < size; i++)
    {
      const char* name = domain->getConstantName((*intArrRep_)[i]);
      assert(name);
      out << name;
      if (i < size-1) out << ",";
      else            out << ")";
    }
  }


  void print(ostream& out) const
  {
    out << (*intArrRep_)[0] << "(";
    int size = intArrRep_->size();
    for (int i = 1; i < size; i++)
    {
      out << (*intArrRep_)[i];
      if (i < size-1) out << ",";
      else            out << ")";
    }
  }
  

 private:
    //intArrRep_[0] is the pred id; intArrRep_[i] is the constant id where i > 0
  Array<unsigned int>* intArrRep_;
  size_t hashCode_;

  Array<GroundClause*>* gndClauses_;
  GroundClauseSet* gndClauseSet_;

  Array<bool>* senseInGndClauses_;

  Array<bool>* truthValues_;
  Array<double>* wtsWhenFalse_;
  Array<double>* wtsWhenTrue_;

    //number of times the ground predicate is set to true
    //overloaded to hold probability that ground predicate is true
  double numTrue_; 

};


////////////////////////////////// hash /////////////////////////////////


class HashGroundPredicate
{
 public:
  size_t operator()(GroundPredicate* const & gp) const  {return gp->hashCode();}
};


class EqualGroundPredicate
{
 public:
  bool operator()(GroundPredicate* const & p1, GroundPredicate* const& p2) const
  { return p1->same(p2); }
};

////////////////////////////////// containers /////////////////////////////////

typedef hash_set<GroundPredicate*, HashGroundPredicate, EqualGroundPredicate> 
  GroundPredicateSet;

typedef hash_map<GroundPredicate*, int,HashGroundPredicate,EqualGroundPredicate>
  GroundPredicateToIntMap;



#endif
