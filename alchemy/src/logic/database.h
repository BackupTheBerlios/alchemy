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
#ifndef DATABASE_H_JUN_28_2005
#define DATABASE_H_JUN_28_2005

#include "groundpreds.h"
#include "domain.h"
#include "arraysaccessor.h"
#include "groundpredicate.h"

  // Used by Database::paramMultByPred_. 
  // First value is the multiplier and the second is the class id.
typedef pair<unsigned int, unsigned int> MultAndType;

const int dbdebug = 0;
//const int dbdebug = 1;

// T_INDEX = True, FA_INDEX = False and Active
enum IndexType { T_INDEX = 0, FA_INDEX = 1, INDEX_TYPE_COUNT = 2 };

struct NumTrueFalse 
{ 
  NumTrueFalse(): numTrue(0), numFalse(0) {}  
  int numTrue; 
  int numFalse; 
};


class Database
{
 public:
    //ASSUMPTION: constants of the same type are ordered consecutively
    //            should have called d's reorderConstants()
  Database(Domain* const & d, const Array<bool>& closedWorld, 
           const bool &storeGndPreds) : domain_(d)
  {
	lazyFlag_ = false;
	performingInference_ = false;
	assert(closedWorld.size() == domain_->getNumPredicates());
    int numFOPreds = domain_->getNumPredicates();

    int numTypes = domain_->getNumTypes();
    firstConstIdByType_.growToSize(numTypes);
    for (int i = 0; i < numTypes; i++)
    {
      assert(domain_->isType(i));
      const Array<int>* constIds = domain_->getConstantsByType(i);
      if (constIds->empty()) firstConstIdByType_[i] = (unsigned int)0;
      else                   firstConstIdByType_[i] = (*constIds)[0];
    }

    closedWorld_.growToSize(numFOPreds);
    memcpy((void*)closedWorld_.getItems(), closedWorld.getItems(),
           closedWorld.size()*sizeof(bool));
	
    termMultByPred_.growToSize(numFOPreds, NULL);

    truePredIdxSet_ = new Array<hash_set<int> >(numFOPreds);
    falsePredIdxSet_ = new Array<hash_set<int> >(numFOPreds);
    numberOfGroundings_.growToSize(numFOPreds);
    truePredIdxSet_->growToSize(numFOPreds);
    falsePredIdxSet_->growToSize(numFOPreds);

    paramPredsArray_ = new Array<Array<Array<Array<Array<int>*>*>*>*>;
    paramPredsArray_->growToSize(INDEX_TYPE_COUNT);
      
	  //Initialize inverse index
    for(int idxType = 0; idxType < INDEX_TYPE_COUNT; idxType++)
    {
      (*paramPredsArray_)[idxType] = new Array<Array<Array<Array<int>*>*>*>;
      (*paramPredsArray_)[idxType]->growToSize(numFOPreds, NULL);
    }

    for (int i = 0; i < numFOPreds; i++)
    {
        // if this is a '=' pred, leave termMultByPred_[i]
        // as NULL
      const PredicateTemplate* t = domain_->getPredicateTemplate(i);
	  if (t->isEqualPredWithType()) continue;
		// Internal predicates are included!
		
	  const Array<int>* termTypes = domain_->getPredicateTermTypesAsInt(i);
      Array<MultAndType>* matArr = new Array<MultAndType>;
      int numTermTypes = termTypes->size(); 
      matArr->growToSize(numTermTypes);
      int curMult = 1;
      for (int j = numTermTypes-1; j >= 0; j--)
      {
        int typeId = (*termTypes)[j];
        (*matArr)[j] = MultAndType(curMult,typeId);
        curMult *= domain_->getNumConstantsByType(typeId);
      }
      termMultByPred_[i] = matArr;

        //Initialize inverse index for this predicate
      for(int idxType = 0; idxType < INDEX_TYPE_COUNT; idxType++)
      {
        (*((*paramPredsArray_)[idxType]))[i] = new Array<Array<Array<int>*>*>;
        (*((*paramPredsArray_)[idxType]))[i]->growToSize(t->getNumTerms());
      }
      	
      int numGnd = 1;
      for (int j = 0; j < t->getNumTerms(); j++)
      {
        int numConstByType =
          domain_->getNumConstantsByType(t->getTermTypeAsInt(j));
        for(int idxType = 0; idxType < INDEX_TYPE_COUNT; idxType++)
        {
          (*((*((*paramPredsArray_)[idxType]))[i]))[j] = new Array<Array<int>*>;
          (*((*((*paramPredsArray_)[idxType]))[i]))[j]->
            growToSize(numConstByType);
          for (int k = 0; k < numConstByType; k++)
          {
            (*((*((*((*paramPredsArray_)[idxType]))[i]))[j]))[k] =
              new Array<int>;
          }
        }
        numGnd *= numConstByType;
      }
      numberOfGroundings_[i] = numGnd;
    }
  
    predIdToNumTF_ = new Array<NumTrueFalse>(numFOPreds);
    predIdToNumTF_->growToSize(numFOPreds);

    activePredIdxSet_ = NULL;
    evidencePredIdxSet_ = NULL;
    deactivatedPredIdxSet_ = NULL;
  }

    //Copy constructor
  Database(const Database& db)
  {
    domain_ = db.domain_;
  	closedWorld_.growToSize(db.closedWorld_.size());
    memcpy((void*)closedWorld_.getItems(), db.closedWorld_.getItems(),
           db.closedWorld_.size()*sizeof(bool));
    //closedWorld_ = db.closedWorld_;
    lazyFlag_ = db.lazyFlag_;
    performingInference_ = db.performingInference_;
    firstConstIdByType_ = db.firstConstIdByType_;
    numberOfGroundings_ = db.numberOfGroundings_;
    
    oppEqGndPreds_ = db.oppEqGndPreds_;

	termMultByPred_.growToSize(db.termMultByPred_.size(), NULL);
	for (int i = 0; i < db.termMultByPred_.size(); i++)
	{
	  if (db.termMultByPred_[i])
	  {
	    Array<MultAndType>* matArr = new Array<MultAndType>;
      	matArr->growToSize(db.termMultByPred_[i]->size());
      	for (int j = 0; j < db.termMultByPred_[i]->size(); j++)
      	{
		  MultAndType mat;
		  mat.first = (*db.termMultByPred_[i])[j].first;
		  mat.second = (*db.termMultByPred_[i])[j].second;
          (*matArr)[j] = mat;
      	}
      	termMultByPred_[i] = matArr;		
	  }
	}

    if (db.paramPredsArray_)
    {
      paramPredsArray_ = new Array<Array<Array<Array<Array<int>*>*>*>*>;
      int size0 = db.paramPredsArray_->size();
      paramPredsArray_->growToSize(size0);
      
      for(int idxType = 0; idxType < size0; idxType++)
      {
        Array<Array<Array<Array<int>*>*>*>* arr1 =
          ((*db.paramPredsArray_)[idxType]);
        int size1 = arr1->size();
        (*paramPredsArray_)[idxType] = new Array<Array<Array<Array<int>*>*>*>;
        (*paramPredsArray_)[idxType]->growToSize(size1);
        
        for (int i = 0; i < size1; i++)
        {
          Array<Array<Array<int>*>*>* arr2 = ((*arr1)[i]);
          if (arr2 == NULL) 
          {
            (*(*paramPredsArray_)[idxType])[i] = NULL;
            continue;
          }
          int size2 = arr2->size();
          (*(*paramPredsArray_)[idxType])[i] = new Array<Array<Array<int>*>*>;
          (*(*paramPredsArray_)[idxType])[i]->growToSize(size2);
          
          for (int j = 0; j < size2; j++)
          {
            Array<Array<int>*>* arr3 = ((*arr2)[j]);
            int size3 = arr3->size();
            (*(*(*paramPredsArray_)[idxType])[i])[j] = new Array<Array<int>*>;
            (*(*(*paramPredsArray_)[idxType])[i])[j]->growToSize(size3);
            
            for (int k = 0; k < size3; k++)
            {
              Array<int>* arr4 = ((*arr3)[k]);
              int size4 = arr4->size();
              (*(*(*(*paramPredsArray_)[idxType])[i])[j])[k] = new Array<int>;
              (*(*(*(*paramPredsArray_)[idxType])[i])[j])[k]->growToSize(size4);
              
              for (int l = 0; l < size4; l++)
              {
                (*(*(*(*(*paramPredsArray_)[idxType])[i])[j])[k])[l] = 
                  ((*arr4)[l]);
              }
            }
          }
        }
      }
    }

	if (db.predIdToNumTF_)
	{
	  predIdToNumTF_ = new Array<NumTrueFalse>(db.predIdToNumTF_->size());
	  predIdToNumTF_->growToSize(db.predIdToNumTF_->size());
	  for (int i = 0; i < db.predIdToNumTF_->size(); i++)
	  {
	  	(*predIdToNumTF_)[i] = (*db.predIdToNumTF_)[i];
	  }
	}
	
	if (db.truePredIdxSet_)
	{
	  truePredIdxSet_ = new Array<hash_set<int> >(db.truePredIdxSet_->size());
	  truePredIdxSet_->growToSize(db.truePredIdxSet_->size());
	  for (int i = 0; i < db.truePredIdxSet_->size(); i++)
	  {
	  	(*truePredIdxSet_)[i] = (*db.truePredIdxSet_)[i];
	  }
	}
	
	if (db.falsePredIdxSet_)
	{	
	  falsePredIdxSet_ = new Array<hash_set<int> >();
	  falsePredIdxSet_->growToSize(db.falsePredIdxSet_->size());
	  for (int i = 0; i < db.falsePredIdxSet_->size(); i++)
	  	(*falsePredIdxSet_)[i] = (*db.falsePredIdxSet_)[i];
	}
	
	if (db.activePredIdxSet_)
	{
	  activePredIdxSet_ = new Array<hash_set<int> >();
	  activePredIdxSet_->growToSize(db.activePredIdxSet_->size());
	  for (int i = 0; i < db.activePredIdxSet_->size(); i++)
	  	(*activePredIdxSet_)[i] = (*db.activePredIdxSet_)[i];
	}
	
	if (db.evidencePredIdxSet_)
	{
	  evidencePredIdxSet_ = new Array<hash_set<int> >();
	  evidencePredIdxSet_->growToSize(db.evidencePredIdxSet_->size());
	  for (int i = 0; i < db.evidencePredIdxSet_->size(); i++)
	  	(*evidencePredIdxSet_)[i] = (*db.evidencePredIdxSet_)[i];
	}

	if (db.deactivatedPredIdxSet_)
	{
	  deactivatedPredIdxSet_ = new Array<hash_set<int> >();
	  deactivatedPredIdxSet_->growToSize(db.deactivatedPredIdxSet_->size());
	  for (int i = 0; i < db.deactivatedPredIdxSet_->size(); i++)
	  	(*deactivatedPredIdxSet_)[i] = (*db.deactivatedPredIdxSet_)[i];
	}
  }
  
  ~Database()
  {
    for (int i = 0; i < termMultByPred_.size(); i++) 
      if (termMultByPred_[i]) delete termMultByPred_[i];
    termMultByPred_.clearAndCompress();

    for (int i = 0; i < truePredIdxSet_->size(); i++)
      (*truePredIdxSet_)[i].clear();
    delete truePredIdxSet_;
	  	
    for (int i = 0; i < falsePredIdxSet_->size(); i++)
      (*falsePredIdxSet_)[i].clear();
    delete falsePredIdxSet_;

    if (lazyFlag_)
    {
      for (int i = 0; i < activePredIdxSet_->size(); i++)
        (*activePredIdxSet_)[i].clear();
      delete activePredIdxSet_;

      for (int i = 0; i < evidencePredIdxSet_->size(); i++)
        (*evidencePredIdxSet_)[i].clear();
      delete evidencePredIdxSet_;
	  	
      for (int i = 0; i < deactivatedPredIdxSet_->size(); i++)
        (*deactivatedPredIdxSet_)[i].clear();
      delete deactivatedPredIdxSet_;
    }

    if (predIdToNumTF_)  { delete predIdToNumTF_; predIdToNumTF_ = NULL; }

    if (paramPredsArray_)
    {
      int size0 = paramPredsArray_->size();
      for(int idxType = 0; idxType < size0; idxType++)
      {
        Array<Array<Array<Array<int>*>*>*>* arr1 =
          ((*paramPredsArray_)[idxType]);
        int size1 = arr1->size();
        
        for (int i = 0; i < size1; i++)
        {
          Array<Array<Array<int>*>*>* arr2 = ((*arr1)[i]);
          if (arr2 == NULL) 
          {
            continue;
          }
          int size2 = arr2->size();
          
          for (int j = 0; j < size2; j++)
          {
            Array<Array<int>*>* arr3 = ((*arr2)[j]);
            arr3->deleteItemsAndClear();
          }
          arr2->deleteItemsAndClear();
        }
        arr1->deleteItemsAndClear();
      }
      paramPredsArray_->deleteItemsAndClear();
      delete paramPredsArray_;
    }
  }
  
  void compress()
  {
    for (int i = 0; i < termMultByPred_.size(); i++) 
      if (termMultByPred_[i]) termMultByPred_[i]->compress();
    termMultByPred_.compress();

    firstConstIdByType_.compress();
  }

  void printInfo()
  {
    cout << "GNDINGS " << endl;
    for (int i = 0; i < numberOfGroundings_.size(); i++)
      cout << i << ": " << numberOfGroundings_[i] << endl;
      
    cout << "TRUE " << truePredIdxSet_->size() << endl;
    for (int i = 0; i < truePredIdxSet_->size(); i++)
    {
      hash_set<int> hs = (*truePredIdxSet_)[i];
      cout << i << ": " << hs.size() << endl;
    }
    cout << "FALSE " << falsePredIdxSet_->size() << endl;
    for (int i = 0; i < falsePredIdxSet_->size(); i++)
    {
      hash_set<int> hs = (*falsePredIdxSet_)[i];
      cout << i << ": " << hs.size() << endl;
    }
    cout << "ACTIVE " << activePredIdxSet_->size() << endl;
    cout << "EVIDENCE " << evidencePredIdxSet_->size() << endl;
    cout << "DEACTIVE " << deactivatedPredIdxSet_->size() << endl;
  }
  
  // Set the lazy flag, update the structures accordingly
  void setLazyFlag()
  {
    setLazyFlag(true);
  }

  void setLazyFlag(const bool& lf)
  {
      // If lazyFlag_ is already set this way, do nothing
    if ((lf && lazyFlag_) || (!lf && !lazyFlag_)) return;

      // If changing lazy flag, our lazy index sets become invalid
      if (activePredIdxSet_)
      {
        for (int i = 0; i < activePredIdxSet_->size(); i++)
          (*activePredIdxSet_)[i].clear();
        delete activePredIdxSet_;
      activePredIdxSet_ = NULL;
      }
      if (evidencePredIdxSet_)
      {
        for (int i = 0; i < evidencePredIdxSet_->size(); i++)
          (*evidencePredIdxSet_)[i].clear();
        delete evidencePredIdxSet_;
      evidencePredIdxSet_ = NULL;
      }
      if (deactivatedPredIdxSet_)
      {
        for (int i = 0; i < deactivatedPredIdxSet_->size(); i++)
          (*deactivatedPredIdxSet_)[i].clear();
        delete deactivatedPredIdxSet_;
      deactivatedPredIdxSet_ = NULL;
      }

    if (lf)
    {
      lazyFlag_ = true;
      int numFOPreds = domain_->getNumPredicates();

      activePredIdxSet_ = new Array<hash_set<int> >(numFOPreds);
      evidencePredIdxSet_ = new Array<hash_set<int> >(numFOPreds);
      deactivatedPredIdxSet_ = new Array<hash_set<int> >(numFOPreds);
      activePredIdxSet_->growToSize(numFOPreds);
      evidencePredIdxSet_->growToSize(numFOPreds);
      deactivatedPredIdxSet_->growToSize(numFOPreds);
    }
    if (!lf)
    {
      lazyFlag_ = false;    
    }
  }
       
  // Set the performing inference flag. Returns the previous setting
  bool setPerformingInference(bool pi)
  {
    bool previous = performingInference_;
    performingInference_ = pi;
    return previous;
  }

  const Domain* getDomain() const { return domain_; }

  static bool sameTruthValueAndSense(const TruthValue& tv, const bool& sense)
  { return (tv==TRUE && sense) || (tv==FALSE && !sense); }

  /**
   * Get the truth value of a predicate in this database.
   */
  TruthValue getValue(const Predicate* const& pred) const
  {
  	if (dbdebug >= 1) cout << "Calling database::getValue" << endl;	  	
    assert(((Predicate*)pred)->isGrounded());
    Predicate* ppred = (Predicate*) pred;
    int predId = pred->getId();

      //if this is a '=' predicate
    if (pred->isEqualPredWithType())
    {
      // If the two const ids are the same, actual truth value is TRUE,
      // else FALSE
      TruthValue actual 
        = (pred->getTerm(0)->getId()==pred->getTerm(1)->getId()) ? TRUE : FALSE;

        //if pred is in oppEqGndPreds_, return opposite of actual value
      for (int i = 0; i < oppEqGndPreds_.size(); i++)
        if (ppred->same(oppEqGndPreds_[i]))
        {
          if (actual == TRUE)
          {
          	if (dbdebug >= 1) cout << "Returning FALSE" << endl;
          	return FALSE;
          }
          if (dbdebug >= 1) cout << "Returning TRUE" << endl;
          return TRUE;
        }

      if (dbdebug >= 1) cout << "Returning " << actual << endl;
      return actual;
    }
    
    int idx = getIdxOfGndPredValues(pred);
    return getValue(idx, predId);
  }
  
  /**
   * Get the truth value of a predicate (in GroundPredicate representation)
   * in this database.
   */
  TruthValue getValue(const GroundPredicate* const& pred) const
  {
    if (dbdebug >= 1) cout << "Calling database::getValue" << endl;     
    int predId = pred->getId();
    
    int idx = getIdxOfGndPredValues(pred);
    return getValue(idx, predId);
  }
 
  /**
   * Get the active status of a predicate in this database.
   */
  bool getActiveStatus(const Predicate* const& pred) const
  {
  	if (dbdebug >= 1) cout << "Calling database::getActiveStatus" << endl;
	if (pred->isEqualPredWithType()) return false;
	assert(lazyFlag_);
	assert(((Predicate*)pred)->isGrounded());
    
    int idx = getIdxOfGndPredValues(pred);
    return getActiveStatus(idx, pred->getId());
  }
  
  /**
   * Get the active status of a predicate (in GroundPredicate representation)
   * in this database.
   */
  bool getActiveStatus(const GroundPredicate* const& pred) const
  {
    if (dbdebug >= 1) cout << "Calling database::getActiveStatus" << endl;
    assert(lazyFlag_);

    int idx = getIdxOfGndPredValues(pred);
    return getActiveStatus(idx, pred->getId());
  }
  

  /**
   * Get the deactivated status of a predicate in this database.
   */  
  bool getDeactivatedStatus(const Predicate* const& pred) const
  {
  	if (dbdebug >= 1) cout << "Calling database::getDeactivatedStatus" << endl;
  	if (pred->isEqualPredWithType()) return false;
	assert(lazyFlag_);
	assert(((Predicate*)pred)->isGrounded());
    
    int idx = getIdxOfGndPredValues(pred);
    return getDeactivatedStatus(idx, pred->getId());
  }
  
  /**
   * Get the deactivated status of a predicate (in GroundPredicate
   * representation) in this database.
   */  
  bool getDeactivatedStatus(const GroundPredicate* const& pred) const
  {
    if (dbdebug >= 1) cout << "Calling database::getDeactivatedStatus" << endl;
    assert(lazyFlag_);
    
    int idx = getIdxOfGndPredValues(pred);
    return getDeactivatedStatus(idx, pred->getId());
  }
    
  /**
   * Get the evidence status of a predicate in this database.
   */
  bool getEvidenceStatus(const Predicate* const& pred) const
  {	
  	if (dbdebug >= 1) cout << "Calling database::getEvidenceStatus" << endl;
	if (pred->isEqualPredWithType()) return true;
    assert(lazyFlag_);
	assert(((Predicate*)pred)->isGrounded());

    int idx = getIdxOfGndPredValues(pred);
    return getEvidenceStatus(idx, pred->getId());
  }
  
  /**
   * Get the evidence status of a predicate (in GroundPredicate representation)
   * in this database.
   */
  bool getEvidenceStatus(const GroundPredicate* const& pred) const
  { 
    if (dbdebug >= 1) cout << "Calling database::getEvidenceStatus" << endl;
    assert(lazyFlag_);

    int idx = getIdxOfGndPredValues(pred);
    return getEvidenceStatus(idx, pred->getId());
  }
  
  string getValueAsString(const Predicate* const& pred) const
  {
    TruthValue tv = getValue(pred);
    if (tv == TRUE)    return "TRUE";
    if (tv == FALSE)   return "FALSE";
    if (tv == UNKNOWN) return "UNKNOWN";
    assert(false); 
    return "UNKNOWN";
  }


  /**
   * Sets the value of a predicate in this database. Caller is responsible for
   * deleting pred.
   * 
   * @param flip If true, value of predicate is flipped.
   * @param ttv TruthValue to which the predicate is set (This is ignored
   * if flip is true)
   * 
   * @return Previous value of the predicate in this database.
   */
  TruthValue setValue(const Predicate* const & pred, const TruthValue& tv)
  { return setValueHelper(pred, false, tv); }

  /**
   * Sets the value of a predicate (in GroundPredicate representation)
   * in this database. Caller is responsible for deleting pred.
   * 
   * @param flip If true, value of predicate is flipped.
   * @param ttv TruthValue to which the predicate is set (This is ignored
   * if flip is true)
   * 
   * @return Previous value of the predicate in this database.
   */
  TruthValue setValue(const GroundPredicate* const & pred, const TruthValue& tv)
  { 
    return setValueHelper(pred, false, tv);
  }

  /**
   * Flips the value of a predicate in this database. Caller is responsible for
   * deleting pred.
   * 
   * @return Previous value of the predicate in this database.
   */
  TruthValue flipValue(const Predicate* const & pred)
  { return setValueHelper(pred, true, UNKNOWN); }
    
  /**
   * Flips the value of a predicate (in GroundPredicate representation)
   * in this database. Caller is responsible for deleting pred.
   * 
   * @return Previous value of the predicate in this database.
   */
  TruthValue flipValue(const GroundPredicate* const & pred)
  { return setValueHelper(pred, true, UNKNOWN); }
    
  /**
   * Sets the active status of a predicate in this database. Caller is
   * responsible for deleting pred.
   * 
   * @return Previous active status of predicate.
   */
  bool setActiveStatus(const Predicate* const & pred, const bool& as)
  { 
    if (dbdebug >= 1) cout << "Calling database::setActiveStatus" << endl;
    if (pred->isEqualPredWithType())
    {
      if (dbdebug >= 1) cout << "Returning false" << endl;
      return false;
    }
      // Active preds can not be evidence
    if (as) assert(!getEvidenceStatus(pred));
    
    assert(lazyFlag_);
    assert(((Predicate*)pred)->isGrounded());
    
    int predId = pred->getId();
    int idx = getIdxOfGndPredValues(pred);
    return setActiveStatus(idx, predId, as, pred, NULL);
  }
  
  /**
   * Sets the active status of a predicate (in GroundPredicate representation)
   * in this database. Caller is responsible for deleting pred.
   * 
   * @return Previous active status of predicate.
   */
  bool setActiveStatus(const GroundPredicate* const & pred, const bool& as)
  { 
    if (dbdebug >= 1) cout << "Calling database::setActiveStatus" << endl;
      // Active preds can not be evidence
    if (as) assert(!getEvidenceStatus(pred));
    assert(lazyFlag_);
    
    int predId = pred->getId();
    int idx = getIdxOfGndPredValues(pred);
    return setActiveStatus(idx, predId, as, NULL, pred);
  }
  
  void resetActiveStatus()
  {
  	assert(lazyFlag_);
    for (int i = 0; i < activePredIdxSet_->size(); i++)
      (*activePredIdxSet_)[i].clear();
  }

  /**
   * Sets the deactivated status of a predicate in this database. Caller is
   * responsible for deleting pred.
   * 
   * @return Previous deactivated status of predicate.
   */
  bool setDeactivatedStatus(const Predicate* const & pred, const bool& das)
  { 
  	if (dbdebug >= 1) cout << "Calling database::setDeactivatedStatus" << endl;
	if (pred->isEqualPredWithType())
	{
	  if (dbdebug >= 1) cout << "Returning false" << endl;
	  return false;
	}
	//deactivated preds can not be evidence
	if (das) assert(!getEvidenceStatus(pred));
	
	assert(lazyFlag_);
    assert(((Predicate*)pred)->isGrounded());
	
    int predId = pred->getId();
    int idx = getIdxOfGndPredValues(pred);
    return setDeactivatedStatus(idx, predId, das);
  }
  
  /**
   * Sets the deactivated status of a predicate (in GroundPredicate
   * representation) in this database. Caller is responsible for deleting pred.
   * 
   * @return Previous deactivated status of predicate.
   */
  bool setDeactivatedStatus(const GroundPredicate* const & pred,
                            const bool& das)
  { 
    if (dbdebug >= 1) cout << "Calling database::setDeactivatedStatus" << endl;
      // Deactivated preds can not be evidence
    if (das) assert(!getEvidenceStatus(pred));
    assert(lazyFlag_);
    
    int predId = pred->getId();
    int idx = getIdxOfGndPredValues(pred);
    return setDeactivatedStatus(idx, predId, das);
  }
  
  void resetDeactivatedStatus()
  {
  	assert(lazyFlag_);
    for (int i = 0; i < deactivatedPredIdxSet_->size(); i++)
      (*deactivatedPredIdxSet_)[i].clear();
  }

  /**
   * Sets the evidence status of a predicate in this database. Caller is
   * responsible for deleting pred.
   * 
   * @return Previous evidence status of predicate.
   */
  bool setEvidenceStatus(const Predicate* const & pred, const bool& es)
  {
  	if (dbdebug >= 1) cout << "Calling database::setEvidenceStatus" << endl;
	if (pred->isEqualPredWithType())
	{
  	  if (dbdebug >= 1) cout << "Returning true" << endl;
	  return true;
	}
	//active preds can not be evidence
	if (es) assert(!getActiveStatus(pred));
				  
	assert(lazyFlag_);
    assert(((Predicate*)pred)->isGrounded());

    int predId = pred->getId();
    int idx = getIdxOfGndPredValues(pred);
    return setEvidenceStatus(idx, predId, es);
  }
  
  /**
   * Sets the evidence status of a predicate (in GroundPredicate representation)
   * in this database. Caller is responsible for deleting pred.
   * 
   * @return Previous evidence status of predicate.
   */
  bool setEvidenceStatus(const GroundPredicate* const & pred, const bool& es)
  {
    if (dbdebug >= 1) cout << "Calling database::setEvidenceStatus" << endl;
      // Active preds can not be evidence
    if (es) assert(!getActiveStatus(pred));
    assert(lazyFlag_);

    int predId = pred->getId();
    int idx = getIdxOfGndPredValues(pred);
    return setEvidenceStatus(idx, predId, es);
  }
  
  void setValuesToUnknown(const Array<Predicate*>* const & gndPreds,
                          Array<TruthValue>* const & prevValues)
  {
    if (prevValues) prevValues->clear();
    for (int i = 0; i < gndPreds->size(); i++)
    {
      TruthValue prev = setValue((*gndPreds)[i], UNKNOWN);
      if (prevValues) prevValues->append(prev);
    }
  }

  void setValuesToUnknown(const GroundPredicateHashArray* const & gndPreds,
                          Array<TruthValue>* const & prevValues)
  {
    if (prevValues) prevValues->clear();
    for (int i = 0; i < gndPreds->size(); i++)
    {
      TruthValue prev = setValue((*gndPreds)[i], UNKNOWN);
      if (prevValues) prevValues->append(prev);
    }
  }


  void setValuesToGivenValues(const Array<Predicate*>* const & gndPreds,
                              const Array<TruthValue>* const & values)
  {
    assert(values);
    assert(values->size() == gndPreds->size());
    for (int i = 0; i < gndPreds->size(); i++)
      setValue((*gndPreds)[i], (*values)[i]);
  }

  void setValuesToGivenValues(const GroundPredicateHashArray* const & gndPreds,
                              const Array<TruthValue>* const & values)
  {
    assert(values);
    assert(values->size() == gndPreds->size());
    for (int i = 0; i < gndPreds->size(); i++)
      setValue((*gndPreds)[i], (*values)[i]);
  }

    //Change all ground predicates with toBeAlteredVal to newVal. 
    //gndPredValues contains the previous values of gndPreds.
  void alterTruthValue(const Array<Predicate*>* const & gndPreds, 
                       const TruthValue& tobeAlteredVal,
                       const TruthValue& newVal, 
                       Array<TruthValue>* const & gndPredValues) 
  {
    for (int i = 0; i < gndPreds->size(); i++) 
    {
	  TruthValue val = getValue((*gndPreds)[i]);
	  gndPredValues->append(val);
	  if (val == tobeAlteredVal) 
      {
        setValue((*gndPreds)[i], newVal);
	  }
    }
  }


	// Called only for evidence atoms
    // Caller should delete pred if required
  void addEvidenceGroundPredicate(Predicate* const & pred)
  {
  	if (dbdebug >= 1)
      cout << "Calling database::addEvidenceGroundPredicate" << endl;
  	setValue(pred, pred->getTruthValue());
  	  //Evidence status only needs to be set for open-world preds
  	if (lazyFlag_ && !closedWorld_[pred->getId()])
  	{
  	  setEvidenceStatus(pred, true);
  	}
  	if (dbdebug >= 1) cout << "Returning" << endl;
  }

  int getNumGroundings(const int& predId) const
  {
      //if it is a '=' predicate
    const PredicateTemplate* t = domain_->getPredicateTemplate(predId);    
    if (t->isEqualPredWithType())
    {
      int nc = domain_->getNumConstantsByType(t->getTermTypeAsInt(0));
      return nc*nc;
    }
    
    return numberOfGroundings_[predId];
  }

  int getNumEvidenceGndPreds(const int& predId) const
  {
      // All '=' predicates are evidence
    const PredicateTemplate* t = domain_->getPredicateTemplate(predId);    
    if (t->isEqualPredWithType())
    {
      int nc = domain_->getNumConstantsByType(t->getTermTypeAsInt(0));
      return nc*nc;
    }

      // All closed-world preds are evidence
    if (closedWorld_[predId])
    {
      return numberOfGroundings_[predId];
    }
    return (*evidencePredIdxSet_)[predId].size();
  }

  int getNumTrueGndPreds(const int& predId) const
  {
      //if it is a '=' predicate
    const PredicateTemplate* t = domain_->getPredicateTemplate(predId);    
    if (t->isEqualPredWithType())
    {
      int nc = domain_->getNumConstantsByType(t->getTermTypeAsInt(0));
      int minus = 0;
      for (int i = 0; i < oppEqGndPreds_.size(); i++)
      {
        Predicate* pred = oppEqGndPreds_[i];
        if (pred->getId() == predId &&
            pred->getTerm(0)->getId() == pred->getTerm(1)->getId())
          minus++;
      }

      return nc-minus;
    }

    return (*predIdToNumTF_)[predId].numTrue;
  }


  int getNumFalseGndPreds(const int& predId) const
  {
      //if it is a '=' predicate
    const PredicateTemplate* t = domain_->getPredicateTemplate(predId);    
    if (t->isEqualPredWithType())
    {
      int nc = domain_->getNumConstantsByType(t->getTermTypeAsInt(0));
      int minus = 0;
      for (int i = 0; i < oppEqGndPreds_.size(); i++)
      {
        Predicate* pred = oppEqGndPreds_[i];
        if (pred->getId() == predId &&
            pred->getTerm(0)->getId() != pred->getTerm(1)->getId())
          minus++;
      }

      return nc*(nc-1)-minus;
    }

    if (closedWorld_[predId])
      return numberOfGroundings_[predId]-(*predIdToNumTF_)[predId].numTrue;
    else
      return (*predIdToNumTF_)[predId].numFalse;
  }


  int getNumUnknownGndPreds(const int& predId) const
  {
      //if it is a '=' predicate
    const PredicateTemplate* t = domain_->getPredicateTemplate(predId);    
    if (t->isEqualPredWithType()) return 0;

    //assert(!closedWorld_[predId]);
    if (closedWorld_[predId]) return 0;
    
    return numberOfGroundings_[predId] -(*predIdToNumTF_)[predId].numTrue
                                       -(*predIdToNumTF_)[predId].numFalse;
  }


  int getNumPredIds() const 
  {
    return predIdToNumTF_->size();
  }

  bool isClosedWorld(const int& predId) const { return closedWorld_[predId]; }
  void setClosedWorld(const int& predId, const bool& b) 
  { closedWorld_[predId] = b; }
  const Array<bool>& getClosedWorld() const { return closedWorld_; }
  
    // Caller should delete returned array but not modify its contents.
  const PredicateHashArray* getTrueNonEvidenceGndPreds(const int& predId) const
  {
    PredicateHashArray* preds = new PredicateHashArray();
    
    hash_set<int>::const_iterator it = (*truePredIdxSet_)[predId].begin();
    for (; it != (*truePredIdxSet_)[predId].end(); it++)
    {
        // Check if in evidence
      if ((*evidencePredIdxSet_)[predId].find(*it) ==
          (*evidencePredIdxSet_)[predId].end())
      {
        Predicate* p = getPredFromIdx((*it),
                                      domain_->getPredicateTemplate(predId));
        preds->append(p);
      }
    }
    return preds;
  }
  
  /**
   * Retrieves indexed groundings of a predicate. The predicate must have
   * exactly 2 terms. The indexed groundings are the true (and false and
   * active) groundings of the (possibly partially grounded) predicate.
   * Caller should delete indexedGndings and elements of indexedGndings, if
   * necessary.
   * 
   * @param indexedGndings Groundings found are appended here.
   * @param pred Predicate whose groundings are found.
   * @param ignoreActivePreds If true, only true groundings are retrieved;
   * otherwise, groundings which are false and active are retrieved as well.
   */
  void getIndexedGndings(Array<Predicate*>* const & indexedGndings,
  						 Predicate* const & pred,
                         bool const & ignoreActivePreds)
  {
    assert(pred->getNumTerms() == 2);
    int predId = pred->getId();

    const Term* term0 = pred->getTerm(0);
    const Term* term1 = pred->getTerm(1);
	  
	  // Both terms are grounded
    if (term0->getType() == Term::CONSTANT &&
        term1->getType() == Term::CONSTANT)
    {
        // Check if pred is true or, if getting active preds, active
      TruthValue tv = getValue(pred);
      if (tv == TRUE || (!ignoreActivePreds && tv == FALSE))
      {
        Predicate* p = new Predicate(*pred);
        indexedGndings->append(p);
      }
      return;
    }

	  // Neither term is grounded
	if (term0->getType() != Term::CONSTANT && 
        term1->getType() != Term::CONSTANT)
	{
	  const Array<int>* constArray =
  	    domain_->getConstantsByType(pred->getTermTypeAsInt(0));
	
		//
  	  for (int j = 0; j < constArray->size(); j++)
  	  {
  	  	int constIdx = domain_->getConstantIndexInType((*constArray)[j]);
        Array<int>* preds =
          (*(*(*(*paramPredsArray_)[T_INDEX])[predId])[0])[constIdx];
        for (int i = 0; i < preds->size(); i++)
  	  	{
          Predicate* p = getPredFromIdx((*preds)[i], pred->getTemplate());
		  indexedGndings->append(p);
  	  	}
  	  }
  	
	  if (!ignoreActivePreds)
  	  {
  	  	for (int j = 0; j < constArray->size(); j++)
  	  	{
  	  	  int constIdx = domain_->getConstantIndexInType((*constArray)[j]);
          Array<int>* preds =
            (*(*(*(*paramPredsArray_)[FA_INDEX])[predId])[0])[constIdx];
          for (int i = 0; i < preds->size(); i++)
  	      {
            Predicate* p = getPredFromIdx((*preds)[i], pred->getTemplate());
		  	indexedGndings->append(p);
  	      }
  	  	}
  	  }	
	  return;	
	}
	  // If we've reached here, then one term is grounded, the other isn't
	int gndTerm = (term0->getType() == Term::CONSTANT) ? 0 : 1;
	int constIdx =
      domain_->getConstantIndexInType(pred->getTerm(gndTerm)->getId());
    Array<int>* preds =
      (*(*(*(*paramPredsArray_)[T_INDEX])[predId])[gndTerm])[constIdx];
    for (int i = 0; i < preds->size(); i++)
  	{
      Predicate* p = getPredFromIdx((*preds)[i], pred->getTemplate());
	  indexedGndings->append(p);
  	 }
  	  
  	if (!ignoreActivePreds)
  	{
      Array<int>* preds =
        (*(*(*(*paramPredsArray_)[FA_INDEX])[predId])[gndTerm])[constIdx];
      for (int i = 0; i < preds->size(); i++)
  	  {
        Predicate* p = getPredFromIdx((*preds)[i], pred->getTemplate());
		indexedGndings->append(p);
  	  }
  	}	
  }





 private:

  /**
   * Get the index of the ground predicate in Predicate representation. This
   * does exactly the same thing as getIdxOfGndPredValues(GroundPredicate*).
   * The index is computed from the number of terms, the id of the predicate
   * and the ids of the terms which are identical in both representations.
   * This should be fixed so that the two functions share code, i.e. Predicate
   * and GroundPredicate should be a subclass of a common class. This, however,
   * slows down the code because it prevents inlining of the functions.
   * 
   * @param pred Predicate for which the index is computed.
   * 
   * @return Index of the Predicate.
   */  
  int getIdxOfGndPredValues(const Predicate* const & pred) const
  {
    int numTerms = pred->getNumTerms();
    Array<MultAndType>* multAndTypes = termMultByPred_[pred->getId()];
    int idx = 0;
    for (int i = 0; i < numTerms; i++)
    {
      int constId = pred->getTerm(i)->getId();
      assert(constId >= 0);
        // idx += mutliplier * num of constants belonging to type d]);
      idx += (*multAndTypes)[i].first 
        * (constId - firstConstIdByType_[(*multAndTypes)[i].second]);
    }
    return idx;
  }

  /**
   * Get the index of the ground predicate in GroundPredicate representation.
   * This does exactly the same thing as getIdxOfGndPredValues(Predicate*).
   * The index is computed from the number of terms, the id of the predicate
   * and the ids of the terms which are identical in both representations.
   * This should be fixed so that the two functions share code, i.e. Predicate
   * and GroundPredicate should be a subclass of a common class. This, however,
   * slows down the code because it prevents inlining of the functions.
   * 
   * @param pred GroundPredicate for which the index is computed.
   * 
   * @return Index of the GroundPredicate.
   */  
  int getIdxOfGndPredValues(const GroundPredicate* const & pred) const
  {
    int numTerms = pred->getNumTerms();
    Array<MultAndType>* multAndTypes = termMultByPred_[pred->getId()];
    int idx = 0;
    for (int i = 0; i < numTerms; i++)
    {
      int constId = pred->getTermId(i);
      assert(constId >= 0);
        // idx += mutliplier * num of constants belonging to type d]);
      idx += (*multAndTypes)[i].first 
        * (constId - firstConstIdByType_[(*multAndTypes)[i].second]);
    }
    return idx;
  }
  
  /**
   * Create a Predicate based on the index in this database and its template.
   * 
   * @param idx Index of the predicate in this database.
   * @param predTemplate PredicateTemplate of the predicate being created.
   * 
   * @return Predicate created.
   */
  Predicate* getPredFromIdx(const int& idx,
                            const PredicateTemplate* const & predTemplate) const
  {
	Predicate* p = new Predicate(predTemplate);
	Array<int>* auxIdx = new Array<int>(predTemplate->getNumTerms());
	auxIdx->growToSize(predTemplate->getNumTerms());
	Array<MultAndType>* multAndTypes = termMultByPred_[p->getId()];
	for (int i = 0; i < predTemplate->getNumTerms(); i++)
	{
	  int aux = 0;
	  for (int j = 0; j < i; j++)
	  {
	  	aux += (*auxIdx)[j] * ((*multAndTypes)[j].first);
	  }
	  (*auxIdx)[i] = (idx - aux) / (*multAndTypes)[i].first;
	  int constId = (*auxIdx)[i] +
                    firstConstIdByType_[(*multAndTypes)[i].second];
	  p->setTermToConstant(i, constId);
	}
	delete auxIdx;
	return p;
  }

  /**
   * Get the truth value of a predicate in this database.
   * 
   * @param idx Index of the predicate in this database.
   * @param predId Id of the predicate.
   */
  TruthValue getValue(const int& idx, const int& predId) const
  {
    hash_set<int>::const_iterator it = (*truePredIdxSet_)[predId].find(idx);
    if (it != (*truePredIdxSet_)[predId].end())
    {
      if (dbdebug >= 1) cout << "returning TRUE" << endl;
      return TRUE;
    }

    if (!closedWorld_[predId] && !performingInference_)
    {
      if((*falsePredIdxSet_)[predId].find(idx) !=
         (*falsePredIdxSet_)[predId].end())
      {
        if (dbdebug >= 1) cout << "returning FALSE" << endl;
        return FALSE;
      }
      else
      {
        if (dbdebug >= 1) cout << "returning UNKNOWN" << endl;
        return UNKNOWN;
      }
    }
    if (dbdebug >= 1) cout << "returning FALSE" << endl;
    return FALSE;
  }
  
  /**
   * Get the active status of a predicate in this database.
   * 
   * @param idx Index of the predicate in this database.
   * @param predId Id of the predicate.
   */
  bool getActiveStatus(const int& idx, const int& predId) const
  { 
      // Evidence atoms can not be active
    if (getEvidenceStatus(idx, predId))
    {
      if (dbdebug >= 1) cout << "Returning false(1)" << endl;       
      return false;
    }

    if ((*activePredIdxSet_)[predId].find(idx) !=
        (*activePredIdxSet_)[predId].end())
    {
      if (dbdebug >= 1) cout << "Returning true" << endl;
      return true;
    }
    if (dbdebug >= 1) cout << "Returning false(2)" << endl;     
    return false;
  }

  /**
   * Get the deactivated status of a predicate in this database.
   * 
   * @param idx Index of the predicate in this database.
   * @param predId Id of the predicate.
   */  
  bool getDeactivatedStatus(const int& idx, const int& predId) const
  {
      // Evidence atoms can not be active or deactivated
    if (getEvidenceStatus(idx, predId))
    {
      if (dbdebug >= 1) cout << "Returning false(1)" << endl;       
      return false;
    }

    if ((*deactivatedPredIdxSet_)[predId].find(idx) !=
        (*deactivatedPredIdxSet_)[predId].end())
    {
      if (dbdebug >= 1) cout << "Returning true" << endl;       
      return true;
    }
    if (dbdebug >= 1) cout << "Returning false(2)" << endl;     
    return false;
  }
  
  /**
   * Get the evidence status of a predicate in this database.
   * 
   * @param idx Index of the predicate in this database.
   * @param predId Id of the predicate.
   */
  bool getEvidenceStatus(const int& idx, const int& predId) const
  {
      // All closed-world preds are evidence
    if (closedWorld_[predId])
    {
      if (dbdebug >= 1) cout << "Returning true(1)" << endl;        
      return true;
    }
    if ((*evidencePredIdxSet_)[predId].find(idx) !=
        (*evidencePredIdxSet_)[predId].end())
    {
      if (dbdebug >= 1) cout << "Returning true(2)" << endl;        
      return true;
    }
    if (dbdebug >= 1) cout << "Returning false" << endl;        
    return false;
  }

  /**
   * Sets the value of a predicate in this database.
   * 
   * @param flip If true, value of predicate is flipped.
   * @param ttv TruthValue to which the predicate is set (This is ignored
   * if flip is true)
   * 
   * @return Previous value of the predicate in this database.
   */
  TruthValue setValueHelper(const Predicate* const & pred, const bool& flip,
                            const TruthValue& ttv)
  {
    if (dbdebug >= 1) cout << "Calling database::setValueHelper" << endl;       
    Predicate* ppred = (Predicate *)pred;
    assert(ppred->isGrounded());
      
      // If this is a '=' predicate
    if (pred->isEqualPredWithType())
    {
      // If the two const ids are the same, actual truth value is TRUE,
      // otherwise FALSE
      TruthValue actual
        = (pred->getTerm(0)->getId() == pred->getTerm(1)->getId()) ?
          TRUE : FALSE;
      TruthValue opposite = (actual==TRUE) ? FALSE : TRUE;

        //if pred is in oppEqGndPreds_, return opposite of actual value
      for (int i = 0; i < oppEqGndPreds_.size(); i++)
        if (ppred->same(oppEqGndPreds_[i]))
        {
          if (flip || ttv == actual)
          {
            Predicate* pr = oppEqGndPreds_[i];
            oppEqGndPreds_.removeItemFastDisorder(i);
            delete pr;
          }
          if (dbdebug >= 1) cout << "returning " << opposite << endl;
          return opposite;
        }

      if (flip || ttv == opposite) oppEqGndPreds_.append(new Predicate(*pred));
      if (dbdebug >= 1) cout << "returning " << actual << endl;
      return actual;
    }

    int idx = getIdxOfGndPredValues(pred);
    int predId = pred->getId();
    return setValueHelper(idx, predId, flip, ttv, pred, NULL);
  }

  /**
   * Sets the value of a predicate (in GroundPredicate representation)
   * in this database.
   * 
   * @param flip If true, value of predicate is flipped.
   * @param ttv TruthValue to which the predicate is set (This is ignored
   * if flip is true)
   * 
   * @return Previous value of the predicate in this database.
   */
  TruthValue setValueHelper(const GroundPredicate* const & pred,
                            const bool& flip, const TruthValue& ttv)
  {
    if (dbdebug >= 1) cout << "Calling database::setValueHelper" << endl;      
    int idx = getIdxOfGndPredValues(pred);
    int predId = pred->getId();
    return setValueHelper(idx, predId, flip, ttv, NULL, pred);
  }

  /**
   * Sets the value of a predicate in this database.
   * 
   * Right now, pred and gndPred are being sent as parameters in order to
   * utilize the inverse index. This needs to be fixed. Exactly one of pred
   * and gndPred must be NULL.
   * 
   * @param idx Index of the predicate in this database.
   * @param predId Id of the predicate.
   * @param flip If true, value of predicate is flipped.
   * @param ttv TruthValue to which the predicate is set (This is ignored
   * if flip is true)
   * 
   * @return Previous value of the predicate in this database.
   */
  TruthValue setValueHelper(const int& idx, const int& predId,
                            const bool& flip, const TruthValue& ttv,
                            const Predicate* const & pred,
                            const GroundPredicate* const & gndPred)
  {
    assert((pred && !gndPred) || (!pred && gndPred));
    TruthValue oldtv = getValue(idx, predId);
    //TruthValue oldtv = FALSE;
    TruthValue tv = ttv;

    if (flip) 
    {
      assert(oldtv == TRUE || oldtv == FALSE);
      if (oldtv == FALSE) tv = TRUE;
      else                tv = FALSE;
    }

    if (oldtv != tv)
    {
      if (closedWorld_[predId])
      {
        if (oldtv == TRUE)  //TRUE->FALSE
        {
          assert(tv == FALSE);
          (*predIdToNumTF_)[predId].numTrue--;
          (*truePredIdxSet_)[predId].erase(idx);
          if (lazyFlag_)
          {
            if (pred) removeFromInverseIndex(pred, idx, T_INDEX);
            else if (gndPred) removeFromInverseIndex(gndPred, idx, T_INDEX);

            if (getActiveStatus(idx, predId))
            {
              if (pred) addToInverseIndex(pred, idx, FA_INDEX);
              else if (gndPred) addToInverseIndex(gndPred, idx, FA_INDEX);
            }
          }
        }
        else 
        { //FALSE->TRUE
          assert(oldtv == FALSE && tv == TRUE);
          (*predIdToNumTF_)[predId].numTrue++;
          (*truePredIdxSet_)[predId].insert(idx);
          if (lazyFlag_)
          {
            if (pred) addToInverseIndex(pred, idx, T_INDEX);
            else if (gndPred) addToInverseIndex(gndPred, idx, T_INDEX);
 
            if (getActiveStatus(idx, predId))
            {
              if (pred) removeFromInverseIndex(pred, idx, FA_INDEX);
              else if (gndPred) removeFromInverseIndex(gndPred, idx, FA_INDEX);
            }
          }
        }
      }
      else
      { //open world
        if (oldtv == UNKNOWN)
        {
          if (tv == TRUE)
          { //UNKNOWN->TRUE
            (*predIdToNumTF_)[predId].numTrue++;
            (*truePredIdxSet_)[predId].insert(idx);
            if (lazyFlag_)
            {
              if (pred) addToInverseIndex(pred, idx, T_INDEX);
              else if (gndPred) addToInverseIndex(gndPred, idx, T_INDEX);
            }
          }
          else
          { //UKNOWN->FALSE
            (*predIdToNumTF_)[predId].numFalse++;
            if (!performingInference_) (*falsePredIdxSet_)[predId].insert(idx);
            if (lazyFlag_)
            {
              if (getActiveStatus(idx, predId))
              {
                if (pred) addToInverseIndex(pred, idx, FA_INDEX);
                if (gndPred) addToInverseIndex(gndPred, idx, FA_INDEX);
              }
            }
          }
        }
        else
        if (oldtv == TRUE)
        {
          (*predIdToNumTF_)[predId].numTrue--;
          (*truePredIdxSet_)[predId].erase(idx);
          if (lazyFlag_)
          {
            if (pred) removeFromInverseIndex(pred, idx, T_INDEX);
            if (gndPred) removeFromInverseIndex(gndPred, idx, T_INDEX);
          }
          if (tv == FALSE) 
          { //TRUE->FALSE
            (*predIdToNumTF_)[predId].numFalse++;
            if (!performingInference_) (*falsePredIdxSet_)[predId].insert(idx);
            if (lazyFlag_)
            {
              if (getActiveStatus(idx, predId))
              {
                if (pred) addToInverseIndex(pred, idx, FA_INDEX);
                if (gndPred) addToInverseIndex(gndPred, idx, FA_INDEX);
              }
            }
          }
        }
        else
        {
          assert(oldtv == FALSE);
          (*predIdToNumTF_)[predId].numFalse--;
          if (!performingInference_) (*falsePredIdxSet_)[predId].erase(idx);
          if (lazyFlag_)
          {
            if (getActiveStatus(idx, predId))
            {
              if (pred) removeFromInverseIndex(pred, idx, FA_INDEX);
              else if (gndPred) removeFromInverseIndex(gndPred, idx, FA_INDEX);
            }
          }
          if (tv == TRUE)
          { //FALSE->TRUE
            (*predIdToNumTF_)[predId].numTrue++;
            (*truePredIdxSet_)[predId].insert(idx);
            if (lazyFlag_)
            {
              if (pred) addToInverseIndex(pred, idx, T_INDEX);
              else if (gndPred) addToInverseIndex(gndPred, idx, T_INDEX);
            }
          }
        }          
      }
    } // if (oldtv != tv)
    if (dbdebug >= 1) cout << "returning " << oldtv << endl;
    return oldtv;
  }

  /**
   * Sets the active status of a predicate in this database. Caller is
   * responsible for deleting pred.
   * 
   * Right now, pred and gndPred are being sent as parameters in order to
   * utilize the inverse index. This needs to be fixed. Exactly one of pred
   * and gndPred must be NULL.
   * 
   * @return Previous active status of predicate.
   */
  bool setActiveStatus(const int& idx, const int& predId,
                       const bool& as, const Predicate* const & pred,
                       const GroundPredicate* const & gndPred)
  {
    assert((pred && !gndPred) || (!pred && gndPred));
    bool oldas = getActiveStatus(idx, predId);
    if (oldas != as)
    {
      if (as)
      {
        (*activePredIdxSet_)[predId].insert(idx);
      }
      else
      {
        (*activePredIdxSet_)[predId].erase(idx);
      }
      
      // If truth value is false then add to/remove from inverse index
      if (lazyFlag_)
      {
        TruthValue tv = getValue(idx, predId);
        if (tv == FALSE)
        {
          if (as)
          {
            if (pred) addToInverseIndex(pred, idx, FA_INDEX);
            else if (gndPred) addToInverseIndex(gndPred, idx, FA_INDEX);
          }
          else
          {
            if (pred) removeFromInverseIndex(pred, idx, FA_INDEX);
            if (gndPred) removeFromInverseIndex(gndPred, idx, FA_INDEX);
          }
        }
      }
    }
    if (dbdebug >= 1) cout << "Returning " << oldas << endl;
    return oldas;
  }

  /**
   * Sets the deactivated status of a predicate in this database. Caller is
   * responsible for deleting pred.
   * 
   * @return Previous deactivated status of predicate.
   */
  bool setDeactivatedStatus(const int& idx, const int& predId, const bool& das)
  {
    bool olddas = getDeactivatedStatus(idx, predId);
    if (olddas != das)
    {
      if (das)
      {
        (*deactivatedPredIdxSet_)[predId].insert(idx);
      }
      else
      {
        (*deactivatedPredIdxSet_)[predId].erase(idx);
      }
    }
    if (dbdebug >= 1) cout << "Returning " << olddas << endl;
    return olddas;
  }

  /**
   * Sets the evidence status of a predicate in this database. Caller is
   * responsible for deleting pred.
   * 
   * @return Previous evidence status of predicate.
   */
  bool setEvidenceStatus(const int& idx, const int& predId, const bool& es)
  {  
    bool oldes = getEvidenceStatus(idx, predId);
    if (oldes != es)
    {
      if (es)
      {
        (*evidencePredIdxSet_)[predId].insert(idx);
      }
      else
      {
        (*evidencePredIdxSet_)[predId].erase(idx);
      }
    }
    if (dbdebug >= 1) cout << "Returning " << oldes << endl;
    return oldes;
  }

  /**
   * Add a predicate to the given index type.
   */
  void addToInverseIndex(const Predicate* const & pred, const int& predIdx,
                         IndexType idxType)
  {
    int numTerms = pred->getNumTerms();
    if (numTerms == 2)
    {
      int predId = pred->getId();
      for (int term = 0; term < numTerms; term++)
      {
        int constantId = pred->getTerm(term)->getId();
        assert(constantId >= 0);
        int constantIndex = domain_->getConstantIndexInType(constantId);
        (*(*(*(*paramPredsArray_)[idxType])[predId])[term])
          [constantIndex]->append(predIdx);
      }
    }
  }

  /**
   * Add a predicate to the given index type.
   */
  void addToInverseIndex(const GroundPredicate* const & pred,
                         const int& predIdx, IndexType idxType)
  {
    int numTerms = pred->getNumTerms();
    if (numTerms == 2)
    {
      int predId = pred->getId();
      for (int term = 0; term < numTerms; term++)
      {
        int constantId = pred->getTermId(term);
        assert(constantId >= 0);
        int constantIndex = domain_->getConstantIndexInType(constantId);
        (*(*(*(*paramPredsArray_)[idxType])[predId])[term])
          [constantIndex]->append(predIdx);
      }
    }
  }

  /**
   * Remove a predicate from the given index type.
   */
  void removeFromInverseIndex(const Predicate* const & pred,
                              const int& predIdx, IndexType idxType)
  {
    int numTerms = pred->getNumTerms();
    if (numTerms == 2)
    {
      int predId = pred->getId();
      for (int term = 0; term < numTerms; term++)
      {
        int constantId = pred->getTerm(term)->getId();
        assert(constantId >= 0);
        int constantIndex = domain_->getConstantIndexInType(constantId);
        int pos = (*(*(*(*paramPredsArray_)[idxType])[predId])[term])
                    [constantIndex]->find(predIdx);
        if (pos > -1)
        {
          (*(*(*(*paramPredsArray_)[idxType])[predId])[term])
            [constantIndex]->removeItemFastDisorder(pos);
        }
      }
    }
  }

  /**
   * Remove a predicate from the given index type.
   */
  void removeFromInverseIndex(const GroundPredicate* const & pred,
                              const int& predIdx, IndexType idxType)
  {
    int numTerms = pred->getNumTerms();
    if (numTerms == 2)
    {
      int predId = pred->getId();
      for (int term = 0; term < numTerms; term++)
      {
        int constantId = pred->getTermId(term);
        assert(constantId >= 0);
        int constantIndex = domain_->getConstantIndexInType(constantId);
        int pos = (*(*(*(*paramPredsArray_)[idxType])[predId])[term])
                    [constantIndex]->find(predIdx);
        if (pos > -1)
        {
          (*(*(*(*paramPredsArray_)[idxType])[predId])[term])
            [constantIndex]->removeItemFastDisorder(pos);
        }
      }
    }
  }

 private:
  const Domain* domain_; // not owned by Database

    // closedWorld_[p]==true: the closed world assumption is made for pred p
  Array<bool> closedWorld_; 

  	// Flag is set when performing lazy inference
  bool lazyFlag_;
  
    // Flag set when performing inference. If set, hash table of false ground
    // atoms is not used (all atoms are true or false).
  bool performingInference_;
  
    // firstConstId_[c] is the id of the first constant of class c
  Array<unsigned int> firstConstIdByType_;

    //Suppose a predicate with id i has three terms, and the number of 
    //groundings of the first, second and third terms are 
    //4, 3, 2 respectively. Then termMultByPred_[i][0].first = 3x2,
    //termMultByPred_[i][1].first = 2, and termMultByPred_[i][2].first = 1.
    //termMultByPred_[i][j].first is the product of the number of groundings
    //of terms j > i;
  Array<Array<MultAndType>*> termMultByPred_;

    // predIdToNumTF_[p] contains the number of TRUE/FALSE groundings of pred
    // with id p  
  Array<NumTrueFalse>* predIdToNumTF_;


    //'=' ground preds that are set to opposite of their actual values
    //We are using a simple array because we are setting one ground predicate 
    //at a time to the opposite of its actual value and then reverting it to 
    //its actual value,so there's no efficiency penalty when searching the array
  Array<Predicate*> oppEqGndPreds_;
  
    //The following structure implements the inverse index
	//3 levels of indirection. First - the first order predicate
    //the true list is for, second - parameter in the predicate it 
    //is indexed, and third - value of the parameter it is
    //indexed.
  Array<Array<Array<Array<Array<int>*>*>*>*>* paramPredsArray_;

	// Hash set of true ground atoms
  Array<hash_set<int> >* truePredIdxSet_;
  
	// Hash set of false ground atoms - only used for open world preds
  Array<hash_set<int> >* falsePredIdxSet_;

	// Hash set of active ground atoms
  Array<hash_set<int> >* activePredIdxSet_;

	// Hash set of evidence ground atoms
  Array<hash_set<int> >* evidencePredIdxSet_;
  
	// Hash set of deactivated ground atoms
  Array<hash_set<int> >* deactivatedPredIdxSet_;

    // Total number of groundings for each predicate
  Array<int> numberOfGroundings_;
};


#endif
