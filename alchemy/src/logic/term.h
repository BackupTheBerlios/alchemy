#ifndef TERM_H_JUN_26_2005
#define TERM_H_JUN_26_2005

#include <climits>
#include <stdlib.h>
#include <ostream>
using namespace std;
#include "array.h"

class Function; 
class Domain;

  //Represents a constant, variable, or function.
  //Ensure that the dirty_ bit is consistently updated.
class Term
{
 public:
  enum { NONE, CONSTANT, VARIABLE, FUNCTION };

 public:
  Term() : id_(-1), function_(NULL), intArrRep_(NULL), dirty_(true), 
           parent_(NULL), parentIsPred_(true) {}

  Term(void* const & parent, const bool& parentIsPred) 
    : id_(-1), function_(NULL), intArrRep_(NULL), dirty_(true), 
      parent_(parent), parentIsPred_(parentIsPred) {}
  
  Term(const int& id) 
    : id_(id), function_(NULL), intArrRep_(NULL), dirty_(true), parent_(NULL), 
      parentIsPred_(true) {}

  Term(const int& id, void* const & parent, const bool& parentIsPred) 
    : id_(id), function_(NULL), intArrRep_(NULL), dirty_(true), 
      parent_(parent), parentIsPred_(parentIsPred) {}

  Term(Function* f) : id_(-1), function_(f), intArrRep_(NULL), dirty_(true),
                      parent_(NULL), parentIsPred_(true) {}

  Term(Function* f, void* const & parent, const bool& parentIsPred) 
    : id_(-1), function_(f), intArrRep_(NULL), dirty_(true), 
      parent_(parent), parentIsPred_(parentIsPred) {}

  Term(const Term& t);

  Term(const Term& t, void* const & parent, const bool& parentIsPred);

  ~Term();


    //NOTE: if function is supported include the size of *function_
  double sizeMB() const
  { return (fixedSizeB_ + intArrRep_->size()*sizeof(int))/1000000.0; }


  static void computeFixedSizeB()
  { fixedSizeB_ = sizeof(Term) + sizeof(Array<int>); }


    //NOTE: if function is supported, compress function_
  void compress() { if (intArrRep_) intArrRep_->compress(); }


  void setDirty();
  bool isDirty() const { return dirty_; }

    // Returns CONSTANT, VARIABLE or FUNCTION.
  int getType() const
  {
    if (id_ >= 0) return CONSTANT;
    if (function_ != NULL) return FUNCTION;
    return VARIABLE;
  }

  
  void setParent(void* const parent, const bool& parentIsPred)
  { parent_ = parent; parentIsPred_ = parentIsPred; setDirty(); }


  void* getParent(bool& parentIsPred) const
  { parentIsPred = parentIsPred_; return parent_; }

  
  void setId(const int& id) 
  { 
    id_ = id; 
    setDirty(); 
  }

  int getId() const { return id_; }
  //int* getIdPtr() { return &id_; }

  void revertToBeFunction() { assert(function_); id_ = -1; }

  void setFunction(Function* const & f) { function_ = f; setDirty(); }
  Function* getFunction() const { return function_; }

  bool isConstant() const { return id_ >= 0; }
  bool isGrounded() const { return id_ >= 0; }

    // represent this term as an Array<int> and append it to rep
  void appendIntArrRep(Array<int>& rep) 
  {
    if (dirty_) computeAndStoreIntArrRep();
    rep.append(*intArrRep_);
  }


  bool same(Term* const & t)
  {
    if (this==t) return true;
    const Array<int>* tArr  = t->getIntArrRep();
    const Array<int>* myArr = getIntArrRep();
    if (myArr->size() != tArr->size()) return false;
    const int* tItems  = t->getIntArrRep()->getItems();
    const int* myItems = getIntArrRep()->getItems();
    return (memcmp(myItems, tItems, myArr->size()*sizeof(int))==0); 
  }

  
  void printAsInt(ostream& out) const;
  void printWithStrVar(ostream& out, const Domain* const & domain) const;
  void print(ostream& out, const Domain* const & domain) const;


 private:
  void copy(const Term& t);
  bool noDirtyFunction();

  const Array<int>* getIntArrRep() 
  { if (dirty_) computeAndStoreIntArrRep(); return intArrRep_; }

  void computeAndStoreIntArrRep();
  
 private:
    // if it is a constant, id_ >= 0 and function_ == NULL;
    // if it is a variable, id_ < 0  and function_ == NULL;
    // if it is a function whose returned value has not been determined, 
    //    function_ != NULL and id_ < 0;
    // if it is a function whose returned value has been determined, 
    //    function_ != NULL and id_ >= 0;
  int id_;
  Function* function_; // if non-null, then Term is a function
  Array<int>* intArrRep_; 
  bool dirty_;
  
  void* parent_;  // not owned by Term, so do not delete in destructor
  bool parentIsPred_;

  static double fixedSizeB_;
};

#endif
