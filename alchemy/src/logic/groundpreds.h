#ifndef GROUNDPREDS_H_JUN_28_2005
#define GROUNDPREDS_H_JUN_28_2005

#include "predicate.h"


class GroundPreds
{
 public:
  GroundPreds() : trueGndPreds_(new PredicateHashArray), 
                  falseGndPreds_(new PredicateHashArray),
                  unknownGndPreds_(new PredicateHashArray) {}

  ~GroundPreds()
  {
    for (int i=0; i<trueGndPreds_->size();i++)    delete (*trueGndPreds_)[i];
    for (int i=0; i<falseGndPreds_->size();i++)   delete (*falseGndPreds_)[i];
    for (int i=0; i<unknownGndPreds_->size();i++) delete (*unknownGndPreds_)[i];
    delete trueGndPreds_;
    delete falseGndPreds_;
    delete unknownGndPreds_;
  }


  bool contains(Predicate* const & p)
  {
    return (trueGndPreds_->contains(p) ||  falseGndPreds_->contains(p) || 
            unknownGndPreds_->contains(p));
  }


    // GroundPreds owns pred and is responsible for deleting it.
  void appendTrueGndPred(Predicate* const & p)  
  { p->setTruthValue(TRUE); trueGndPreds_->append(p); }

    // GroundPreds owns pred and is responsible for deleting it.
  void appendFalseGndPred(Predicate* const & p) 
  { p->setTruthValue(FALSE); falseGndPreds_->append(p);}

    // GroundPreds owns pred and is responsible for deleting it.
  void appendUnknownGndPred(Predicate* const & p)
  { p->setTruthValue(UNKNOWN); unknownGndPreds_->append(p); }


    // Caller should not delete returned array nor modify its contents.
  const PredicateHashArray* getTrueGndPreds() const  { return trueGndPreds_; }
  const PredicateHashArray* getFalseGndPreds() const { return falseGndPreds_; }
  const PredicateHashArray* getUnknownGndPreds() const 
  {return unknownGndPreds_; }


  int getNumTrueGndPreds() const    { return trueGndPreds_->size(); }
  int getNumFalseGndPreds() const   { return falseGndPreds_->size(); }
  int getNumUnknownGndPreds() const { return unknownGndPreds_->size(); }


    // caller may delete p if it is not originally obtained from GroundPreds
  void changeGndPredTruthValue(Predicate* const & pred, 
                               const TruthValue& oldValue,
                               const TruthValue& newValue)
  {
    Predicate* ppred = (Predicate*) pred;
    Predicate* p = NULL;
    if (oldValue==TRUE)    p = trueGndPreds_->removeItemFastDisorder(ppred);
    else 
    if (oldValue==FALSE)   p = falseGndPreds_->removeItemFastDisorder(ppred);
    else 
    if (oldValue==UNKNOWN) p = unknownGndPreds_->removeItemFastDisorder(ppred);
    
    assert(p);
    if (newValue == TRUE)         trueGndPreds_->append(p);
    else if (newValue == FALSE)   falseGndPreds_->append(p);
    else if (newValue == UNKNOWN) unknownGndPreds_->append(p);
    else assert(false);
  }
  

  void compress()
  {
    trueGndPreds_->compress();
    falseGndPreds_->compress();
    unknownGndPreds_->compress();
  }


 private:
  PredicateHashArray* trueGndPreds_;
  PredicateHashArray* falseGndPreds_;
  PredicateHashArray* unknownGndPreds_;
  
};


#endif
