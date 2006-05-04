#ifndef DATABASE_H_JUN_28_2005
#define DATABASE_H_JUN_28_2005

#include "groundpreds.h"
#include "domain.h"
#include "arraysaccessor.h"

  // Used by Database::paramMultByPred_. 
  // First value is the multiplier and the second is the class id.
typedef pair<unsigned int, unsigned int> MultAndType;

// How to store ground atoms: 0 = Boolean arrays, 1 = hash set, 2 = hash set + inverse index
// 1 + 2 can not be used with storeGndPreds
const int gndAtoms = 2;

const int dbdebug = 0;
//const int dbdebug = 1;

// T_INDEX = True, FA_INDEX = False and Active
enum IndexType {T_INDEX, FA_INDEX, INDEX_TYPE_COUNT};

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
	if (gndAtoms == 0)
	{
      gndPredValues_.growToSize(numFOPreds, NULL);
	}
	else if (gndAtoms >= 1)
	{
	  truePredIdxSet_ = new Array<hash_set<int> >(numFOPreds);
	  falsePredIdxSet_ = new Array<hash_set<int> >(numFOPreds);
	  numberOfGroundings_.growToSize(numFOPreds);
	}
	if (gndAtoms == 2)
	{
	  //Initialize inverse index
	  for(int idxType = 0; idxType < INDEX_TYPE_COUNT; idxType++)
	  {
	    paramPredsArray[idxType] = new Array<int>**[numFOPreds];
	  }
	}

    for (int i = 0; i < numFOPreds; i++)
    {
        // if this is a '=' pred, leave termMultByPred_[i] & gndPredValues_[i]
        // as NULL
      const PredicateTemplate* t = domain_->getPredicateTemplate(i);    
      	//Parag: shifted this condition down into the loop
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

	  if (gndAtoms == 0)
	  {
      	Array<TruthValue>* tvArr = new Array<TruthValue>;
      	TruthValue initialValue = (closedWorld_[i]) ? FALSE : UNKNOWN;

      	tvArr->growToSize(curMult,initialValue);
      	gndPredValues_[i] = tvArr;
      }
	  else if (gndAtoms >= 1)
	  {
      	//Initialize inverse index for this predicate
	  	if (gndAtoms == 2)
		  for(int idxType = 0; idxType < INDEX_TYPE_COUNT; idxType++)
		  	paramPredsArray[idxType][i] = new Array<int>*[t->getNumTerms()];
      	
	  	int numGnd = 1;
  		for (int j = 0; j < t->getNumTerms(); j++)
  		{
  		  int numConstByType = domain_->getNumConstantsByType(t->getTermTypeAsInt(j));
  		  if (gndAtoms == 2)
  		  {  		  	
       	    for(int idxType = 0; idxType < INDEX_TYPE_COUNT; idxType++)
		  	  paramPredsArray[idxType][i][j] = new Array<int>[numConstByType];
  		  }
    	  numGnd *= numConstByType;
  		}
  		numberOfGroundings_[i] = numGnd;
	  }
	}
  
    if (gndAtoms == 0) assert(gndPredValues_.size() == termMultByPred_.size());
	
    if (storeGndPreds)
    {
      predIdToGndPreds_ = new Array<GroundPreds*>(numFOPreds);
      predIdToGndPreds_->growToSize(numFOPreds, NULL);
      predIdToNumTF_ = NULL;
    }
    else
    {
      predIdToGndPreds_ = NULL;
      predIdToNumTF_ = new Array<NumTrueFalse>(numFOPreds);
      predIdToNumTF_->growToSize(numFOPreds);
    }
  }
  
  //Parag: set the lazy flag, update the structures accordingly
  void setLazyFlag()
  {
	lazyFlag_ = true;
    int numFOPreds = domain_->getNumPredicates();

	if (gndAtoms == 0)
	{
	  gndPredActiveStatus_.growToSize(numFOPreds, NULL);
	  gndPredEvidenceStatus_.growToSize(numFOPreds, NULL);
	
	  for (int i = 0; i < numFOPreds; i++)
      {
      	const PredicateTemplate* t = domain_->getPredicateTemplate(i);    
      	//Parag: shifted this condition down into the loop
	  	if (t->isEqualPredWithType()) continue;
		// Internal predicates are included!
		  
	  	const Array<int>* termTypes = domain_->getPredicateTermTypesAsInt(i);
      	int numTermTypes = termTypes->size(); 
      	int curMult = 1;
      	for (int j = numTermTypes-1; j >= 0; j--)
      	{
          int typeId = (*termTypes)[j];
          curMult *= domain_->getNumConstantsByType(typeId);
      	}  
	   
	  	Array<bool>* asArr = new Array<bool>;
	  	asArr->growToSize(curMult,false);
      	gndPredActiveStatus_[i] = asArr;
	   
	  	Array<bool>* esArr = new Array<bool>;
	  	esArr->growToSize(curMult,false);
      	gndPredEvidenceStatus_[i] = esArr;
	   
	  }
	  
	  assert(gndPredActiveStatus_.size() == termMultByPred_.size());
	  assert(gndPredEvidenceStatus_.size() == termMultByPred_.size());
	}
	else if (gndAtoms >= 1)
	{
	  activePredIdxSet_ = new Array<hash_set<int> >(numFOPreds);
	  evidencePredIdxSet_ = new Array<hash_set<int> >(numFOPreds);
	  deactivatedPredIdxSet_ = new Array<hash_set<int> >(numFOPreds);
	}

  }
	   
  //Set the performing inference flag
  void setPerformingInference(bool pi)
  {
	performingInference_ = pi;
  }

  ~Database()
  {
  	if (gndAtoms == 0)
  	{
	  for (int i = 0; i < gndPredValues_.size(); i++)  
      	if (gndPredValues_[i]) delete gndPredValues_[i];
      gndPredValues_.clearAndCompress();
  	
	  //Parag:
	  if(lazyFlag_)
	  {
	    for (int i = 0; i < gndPredActiveStatus_.size(); i++)  
        if (gndPredActiveStatus_[i]) delete gndPredActiveStatus_[i];
        gndPredActiveStatus_.clearAndCompress();
	 
	    for (int i = 0; i < gndPredEvidenceStatus_.size(); i++)  
          if (gndPredEvidenceStatus_[i]) delete gndPredEvidenceStatus_[i];
        gndPredEvidenceStatus_.clearAndCompress();
	  }
	
      if (predIdToGndPreds_)
      {
        for (int i = 0; i < predIdToGndPreds_->size(); i++)
          delete (*predIdToGndPreds_)[i];
        delete predIdToGndPreds_;
        predIdToGndPreds_ = NULL;
      }
	}
    for (int i = 0; i < termMultByPred_.size(); i++) 
    if (termMultByPred_[i]) delete termMultByPred_[i];
    termMultByPred_.clearAndCompress();

    if (predIdToNumTF_)  { delete predIdToNumTF_; predIdToNumTF_ = NULL; }
  }
  
  void compress()
  {
  	if (gndAtoms == 0)
  	{
      for (int i = 0; i < gndPredValues_.size(); i++) 
        if (gndPredValues_[i]) gndPredValues_[i]->compress();
      gndPredValues_.compress();
    
	  //Parag:
	  if(lazyFlag_)
	  {
	    for (int i = 0; i < gndPredActiveStatus_.size(); i++) 
        if (gndPredActiveStatus_[i]) gndPredActiveStatus_[i]->compress();
        gndPredActiveStatus_.compress();
	 
	    for (int i = 0; i < gndPredEvidenceStatus_.size(); i++) 
        if (gndPredEvidenceStatus_[i]) gndPredEvidenceStatus_[i]->compress();
        gndPredEvidenceStatus_.compress();
	  }

      if (predIdToGndPreds_)
      {
        for (int i = 0; i < predIdToGndPreds_->size(); i++)
          if ((*predIdToGndPreds_)[i] != NULL)
            (*predIdToGndPreds_)[i]->compress();
      }
  	}
    for (int i = 0; i < termMultByPred_.size(); i++) 
    if (termMultByPred_[i]) termMultByPred_[i]->compress();
    termMultByPred_.compress();

    firstConstIdByType_.compress();
  }

  const Domain* getDomain() const { return domain_; }

  static bool sameTruthValueAndSense(const TruthValue& tv, const bool& sense)
  { return (tv==TRUE && sense) || (tv==FALSE && !sense); }

  bool storeGroundPreds() const { return (predIdToGndPreds_ != NULL); }

  TruthValue getValue(const Predicate* const& pred) const
  {
  	if (dbdebug >= 1) cout << "Calling database::getValue" << endl;	  	
    assert(((Predicate*)pred)->isGrounded());
    Predicate* ppred = (Predicate*) pred;
    int predId = pred->getId();

      //if this is a '=' predicate
    if (pred->isEqualPredWithType())
    {
      //if the two const ids are the same, actual truth value is TRUE,else FALSE
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
	if (gndAtoms == 0)
	{
	  if (dbdebug >= 1) cout << "Returning " << (*gndPredValues_[predId])[idx] << endl;
      return (*gndPredValues_[predId])[idx];
	}
	else if (gndAtoms >= 1)
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
  }
 
  //Parag : get the active status of the given ground predicate
  bool getActiveStatus(const Predicate* const& pred) const
  {
  	if (dbdebug >= 1) cout << "Calling database::getActiveStatus" << endl;
  	//if (!lazyFlag_) return false;
	if (pred->isEqualPredWithType()) return false;
	assert(lazyFlag_);
	assert(((Predicate*)pred)->isGrounded());
    
    int idx = getIdxOfGndPredValues(pred);
	if (gndAtoms == 0)
	{
      return (*gndPredActiveStatus_[pred->getId()])[idx];
	}
	else if (gndAtoms >= 1)
	{
		// Evidence atoms can not be active
	  if (getEvidenceStatus(pred))
	  {
	  	if (dbdebug >= 1) cout << "Returning false(1)" << endl;	  	
	  	return false;
	  }
	  if ((*activePredIdxSet_)[pred->getId()].find(idx) !=
	  	  (*activePredIdxSet_)[pred->getId()].end())
	  {
	  	if (dbdebug >= 1) cout << "Returning true" << endl;	  	
	  	return true;
	  }
	  if (dbdebug >= 1) cout << "Returning false(2)" << endl;	  	
	  return false;
	}
  }
  
  bool getDeactivatedStatus(const Predicate* const& pred) const
  {
  	if (dbdebug >= 1) cout << "Calling database::getDeactivatedStatus" << endl;
  	if (pred->isEqualPredWithType()) return false;
	assert(lazyFlag_);
	assert(((Predicate*)pred)->isGrounded());
    
    int idx = getIdxOfGndPredValues(pred);
    if (getEvidenceStatus(pred))
	{
	  if (dbdebug >= 1) cout << "Returning false(1)" << endl;	  	
	  return false;
	}

	if ((*deactivatedPredIdxSet_)[pred->getId()].find(idx) !=
		(*deactivatedPredIdxSet_)[pred->getId()].end())
	{
	  if (dbdebug >= 1) cout << "Returning true" << endl;	  	
	  return true;
	}
	if (dbdebug >= 1) cout << "Returning false(2)" << endl;	  	
	return false;
  }
  
  //Parag : get the evidence status of the given ground predicate
  bool getEvidenceStatus(const Predicate* const& pred) const
  {	
  	if (dbdebug >= 1) cout << "Calling database::getEvidenceStatus" << endl;
	if (pred->isEqualPredWithType()) return true;
    assert(lazyFlag_);
	assert(((Predicate*)pred)->isGrounded());

    int idx = getIdxOfGndPredValues(pred);
	if (gndAtoms == 0)
	{
      return (*gndPredEvidenceStatus_[pred->getId()])[idx];
	}
	else if (gndAtoms >= 1)
	{
		// All closed-world preds are evidence
	  if (closedWorld_[pred->getId()])
	  {
	  	if (dbdebug >= 1) cout << "Returning true(1)" << endl;	  	
	  	return true;
	  }
	  if ((*evidencePredIdxSet_)[pred->getId()].find(idx) !=
	  	  (*evidencePredIdxSet_)[pred->getId()].end())
	  {
	  	if (dbdebug >= 1) cout << "Returning true(2)" << endl;	  	
	  	return true;
	  }
	  if (dbdebug >= 1) cout << "Returning false" << endl;	  	
	  return false;
	}
  }

  // Marc: Not being used, so removed
  /*
  TruthValue* getValuePtr(const Predicate* const& pred) const
  {
    assert(((Predicate*)pred)->isGrounded());
      //if this is a '=' predicate
    if (pred->isEqualPredWithType()) return NULL;

    int idx = getIdxOfGndPredValues(pred);
    return &((*gndPredValues_[pred->getId()])[idx]);    
  }
*/

  string getValueAsString(const Predicate* const& pred) const
  {
    TruthValue tv = getValue(pred);
    if (tv == TRUE)    return "TRUE";
    if (tv == FALSE)   return "FALSE";
    if (tv == UNKNOWN) return "UNKNOWN";
    assert(false); 
    return "UNKNOWN";
  }


    // Returns prev TruthValue of pred. Caller is responsible for deleting pred.
  TruthValue setValue(const Predicate* const & pred, const TruthValue& tv)
  { return setValueHelper(pred, false, tv); }

  TruthValue flipValue(const Predicate* const & pred)
  { return setValueHelper(pred, true, UNKNOWN); }
  
  //setValue helper
  TruthValue setValueHelper(const Predicate* const & pred, const bool& flip,
                            const TruthValue& ttv)
  {
	if (dbdebug >= 1) cout << "Calling database::setValueHelper" << endl;	  	
    Predicate* ppred = (Predicate *)pred;
    assert(ppred->isGrounded());
      
      //if this is a '=' predicate
    if (pred->isEqualPredWithType())
    {
      //if the two const ids are the same, actual truth value is TRUE, else FALSE
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

    int predId = pred->getId();

    TruthValue oldtv = getValue(pred);
    TruthValue tv = ttv;


    if (flip) 
    {
      assert(oldtv == TRUE || oldtv == FALSE);
      if (oldtv == FALSE) tv = TRUE;
      else                tv = FALSE;
    }

    int idx = getIdxOfGndPredValues(pred);
    if (gndAtoms == 0)
    {
        // set gndPredValues_
      (*gndPredValues_[predId])[idx] =  tv;
    }

    if (predIdToNumTF_)
    {
      if (oldtv != tv)
      {
        if (closedWorld_[predId])
        {
          if (oldtv == TRUE)  //TRUE->FALSE
          {
            assert(tv == FALSE);
            (*predIdToNumTF_)[predId].numTrue--;
            if (gndAtoms >= 1) (*truePredIdxSet_)[predId].erase(idx);
            if (gndAtoms == 2 && lazyFlag_)
            {
              removeFromInverseIndex((Predicate*)pred, T_INDEX);
              if (getActiveStatus(pred)) addToInverseIndex((Predicate*)pred, FA_INDEX);
            }
          }
          else 
          { //FALSE->TRUE
            assert(oldtv == FALSE && tv == TRUE);
            (*predIdToNumTF_)[predId].numTrue++;
            if (gndAtoms >= 1) (*truePredIdxSet_)[predId].insert(idx);
            if (gndAtoms == 2 && lazyFlag_)
            {
              addToInverseIndex((Predicate*)pred, T_INDEX);
              if (getActiveStatus(pred)) removeFromInverseIndex((Predicate*)pred, FA_INDEX);
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
              if (gndAtoms >= 1) (*truePredIdxSet_)[predId].insert(idx);
              if (gndAtoms == 2 && lazyFlag_) addToInverseIndex((Predicate*)pred, T_INDEX);
            }
            else
            { //UKNOWN->FALSE
              (*predIdToNumTF_)[predId].numFalse++;
              if (gndAtoms >= 1 && !performingInference_) (*falsePredIdxSet_)[predId].insert(idx);
              if (gndAtoms == 2 && lazyFlag_)
              {
              	if (getActiveStatus(pred)) addToInverseIndex((Predicate*)pred, FA_INDEX);
              }
            }
          }
          else
          if (oldtv == TRUE)
          {
            (*predIdToNumTF_)[predId].numTrue--;
            if (gndAtoms >= 1) (*truePredIdxSet_)[predId].erase(idx);
            if (gndAtoms == 2 && lazyFlag_) removeFromInverseIndex((Predicate*)pred, T_INDEX);
            if (tv == FALSE) 
            { //TRUE->FALSE
              (*predIdToNumTF_)[predId].numFalse++;
              if (gndAtoms >= 1 && !performingInference_) (*falsePredIdxSet_)[predId].insert(idx);
              if (gndAtoms == 2 && lazyFlag_)
              {
                if (getActiveStatus(pred)) addToInverseIndex((Predicate*)pred, FA_INDEX);
              }
            }
          }
          else
          {
            assert(oldtv == FALSE);
            (*predIdToNumTF_)[predId].numFalse--;
            if (gndAtoms >= 1 && !performingInference_) (*falsePredIdxSet_)[predId].erase(idx);
            if (gndAtoms == 2 && lazyFlag_)
            {
              if (getActiveStatus(pred)) removeFromInverseIndex((Predicate*)pred, FA_INDEX);
            }
            if (tv == TRUE)
            { //FALSE->TRUE
              (*predIdToNumTF_)[predId].numTrue++;
              if (gndAtoms >= 1) (*truePredIdxSet_)[predId].insert(idx);
              if (gndAtoms == 2 && lazyFlag_) addToInverseIndex((Predicate*)pred, T_INDEX);
            }
          }          
        }
      } // if (oldtv != tv)
    }
    else
    {
      assert(predIdToGndPreds_);
       // set predIdToGndPreds_
      GroundPreds*& gp = (*predIdToGndPreds_)[predId];
        //if pred has been added before 
      if (gp != NULL && gp->contains((Predicate*)pred))  
      {
        if (oldtv != tv)
        {
          gp->changeGndPredTruthValue((Predicate*)pred, oldtv, tv);
          //Update inverse index
		  if (gndAtoms == 2 && lazyFlag_)
		  {
          	if (oldtv==TRUE)
          	{
          	  removeFromInverseIndex((Predicate*)pred, T_INDEX);
          	  if (getActiveStatus(pred)) addToInverseIndex((Predicate*)pred, FA_INDEX);
          	}
          	else if (tv==TRUE)
          	{
          	  addToInverseIndex((Predicate*)pred, T_INDEX);
          	  if (getActiveStatus(pred)) removeFromInverseIndex((Predicate*)pred, FA_INDEX);
          	}
		  }
        }
      }
      else // pred has not been added before, so add it
      {  
        if (gp == NULL) gp = new GroundPreds;
        Predicate* p = new Predicate(*pred);
        if (tv==TRUE)
        {
          gp->appendTrueGndPred(p);
          //Update inverse index
          if (gndAtoms == 2 && lazyFlag_) addToInverseIndex(p, T_INDEX);
        }
        else if (tv==FALSE)
        {
          gp->appendFalseGndPred(p);
          //Update inverse index
          if (gndAtoms == 2 && lazyFlag_)
		  {
          	if (getActiveStatus(pred)) addToInverseIndex((Predicate*)pred, FA_INDEX);
		  }
        }
        else if (tv==UNKNOWN)
        {
          gp->appendUnknownGndPred(p);
        }
        else assert(false);
      }
    }
	if (dbdebug >= 1) cout << "returning " << oldtv << endl;
    return oldtv;
  }
  
  //Parag: 
  // Returns prev ActiveStatus of pred. Caller is responsible for deleting pred.
  bool setActiveStatus(const Predicate* const & pred, const bool& as)
  { 
  	if (dbdebug >= 1) cout << "Calling database::setActiveStatus" << endl;
	if (pred->isEqualPredWithType())
	{
	  if (dbdebug >= 1) cout << "Returning false" << endl;
	  return false;
	}
	//active preds can not be evidence
	if(as) assert(!getEvidenceStatus(pred));
	
	assert(lazyFlag_);
    assert(((Predicate*)pred)->isGrounded());
	
    int predId = pred->getId();

	bool oldas = getActiveStatus(pred);
	if (oldas != as) {
      int idx = getIdxOfGndPredValues(pred);
	  if (gndAtoms == 0)
	  {
    	  // set gndPredValues_
      	(*gndPredActiveStatus_[predId])[idx] =  as;
	  }
      else if (gndAtoms >= 1)
      {
      	if (as)
      	{
      	  (*activePredIdxSet_)[predId].insert(idx);
      	}
      	else
      	{
      	  (*activePredIdxSet_)[predId].erase(idx);
      	}
      }
      // If truth value is false then add to/remove from inverse index
 	  if (gndAtoms == 2 && lazyFlag_)
 	  {
        TruthValue tv = getValue(pred);
        if (tv == FALSE)
        {
          if (as) addToInverseIndex((Predicate *)pred, FA_INDEX);
          else removeFromInverseIndex((Predicate *)pred, FA_INDEX);
        }
 	  }
	}
  	if (dbdebug >= 1) cout << "Returning " << oldas << endl;
	return oldas;
  }
	   

  // Returns prev DeactivatedStatus of pred. Caller is responsible for deleting pred.
  bool setDeactivatedStatus(const Predicate* const & pred, const bool& das)
  { 
  	if (dbdebug >= 1) cout << "Calling database::setDeactivatedStatus" << endl;
	if (pred->isEqualPredWithType())
	{
	  if (dbdebug >= 1) cout << "Returning false" << endl;
	  return false;
	}
	//deactivated preds can not be evidence
	if(das) assert(!getEvidenceStatus(pred));
	
	assert(lazyFlag_);
    assert(((Predicate*)pred)->isGrounded());
	
    int predId = pred->getId();

	bool olddas = getDeactivatedStatus(pred);
	if (olddas != das) {
      int idx = getIdxOfGndPredValues(pred);
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


  //Parag: 
  // Returns prev EvidenceStatus of pred. Caller is responsible for deleting pred.
  bool setEvidenceStatus(const Predicate* const & pred, const bool& es)
  {
  	if (dbdebug >= 1) cout << "Calling database::setEvidenceStatus" << endl;
	if (pred->isEqualPredWithType())
	{
  	  if (dbdebug >= 1) cout << "Returning true" << endl;
	  return true;
	}
	//active preds can not be evidence
	if(es) assert(!getActiveStatus(pred));
				  
	assert(lazyFlag_);
    assert(((Predicate*)pred)->isGrounded());

    int predId = pred->getId();
    
    bool oldes = getEvidenceStatus(pred);
    if (oldes != es)
    {
      int idx = getIdxOfGndPredValues(pred);
	  if (gndAtoms == 0)
	  {
    	  // set gndPredValues_
      	(*gndPredEvidenceStatus_[predId])[idx] =  es;    
	  }
      else if (gndAtoms >= 1)
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
    }
  	if (dbdebug >= 1) cout << "Returning " << oldes << endl;
	return oldes;
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


  void setValuesToGivenValues(const Array<Predicate*>* const & gndPreds,
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
	  if(val == tobeAlteredVal) 
      {
        setValue((*gndPreds)[i], newVal);
	  }
    }
  }


	// Called only for evidence atoms
    // Caller should delete pred if required
  void addEvidenceGroundPredicate(Predicate* const & pred)
  {
  	if (dbdebug >= 1) cout << "Calling database::addEvidenceGroundPredicate" << endl;
  	setValue(pred, pred->getTruthValue());
  	  //Evidence status only needs to be set for open-world preds
  	if (lazyFlag_ && (gndAtoms == 0 || !closedWorld_[pred->getId()]))
  	{
  	  setEvidenceStatus(pred, true);
  	}
  	if (dbdebug >= 1) cout << "Returning" << endl;
  }


    // Add all groundings of each predicate. If a grounding is not already in
    // the database, its value is set to FALSE when the closed world assumption
    // is made, otherwise its value is set to UNKNOWN
  void addAllGroundings()
  {
    if (predIdToGndPreds_)
    {
        // for each predicate
      for (int predId = 0; predId < domain_->getNumPredicates(); predId++)
      {
        const PredicateTemplate* t = domain_->getPredicateTemplate(predId);
          //don't add groundings for '=' predicate
        if (t->isEqualPredWithType()) continue;

        Predicate pred(t);        
        
          // set up all groundings of pred with predId
        ArraysAccessor<int> acc;
        const Array<int>* terms = domain_->getPredicateTermTypesAsInt(predId);
        int numTerms = terms->size();
        for (int j = 0; j < numTerms; j++)
        {
          int typeId = (*terms)[j];
          assert(typeId > 0);
          const Array<int>* constArr = domain_->getConstantsByType(typeId);
          acc.appendArray(constArr);
        }

          // for each grounding
        Array<int> constIds;
        while (acc.getNextCombination(constIds))
        {
          assert(constIds.size() == numTerms);
          for (int k = 0; k < numTerms; k++)
            pred.setTermToConstant(k, constIds[k]);

          GroundPreds*& gp = (*predIdToGndPreds_)[predId];
          if (gp == NULL || !gp->contains(&pred)) //if pred isn't in groundpreds
          {
              //add it into unknowns
            if (gp == NULL) gp = new GroundPreds;
            if (closedWorld_[predId])
              gp->appendFalseGndPred(new Predicate(pred));
            else               
              gp->appendUnknownGndPred(new Predicate(pred));
          }
        } // for each grounding
      } // for each predicate
    }
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
    
    if (gndAtoms == 0)
	  return (gndPredValues_)[predId]->size();
	else if (gndAtoms >= 1)
	  return numberOfGroundings_[predId];
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

    if (predIdToNumTF_)
      return (*predIdToNumTF_)[predId].numTrue;
    else
    {
      if ((*predIdToGndPreds_)[predId]==NULL) return 0;
      return (*predIdToGndPreds_)[predId]->getNumTrueGndPreds();
    }
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

    if (predIdToNumTF_)
    {
      if (closedWorld_[predId])
      {
      	if (gndAtoms == 0)
          return gndPredValues_[predId]->size()-(*predIdToNumTF_)[predId].numTrue;
        else if (gndAtoms >= 1)
          return numberOfGroundings_[predId]-(*predIdToNumTF_)[predId].numTrue;
          
      }
      else
        return (*predIdToNumTF_)[predId].numFalse;
    }
    else
    {
      if ((*predIdToGndPreds_)[predId]==NULL) return 0;
      return (*predIdToGndPreds_)[predId]->getNumFalseGndPreds(); 
    }
  }


  int getNumUnknownGndPreds(const int& predId) const
  {
      //if it is a '=' predicate
    const PredicateTemplate* t = domain_->getPredicateTemplate(predId);    
    if (t->isEqualPredWithType()) return 0;

    if (predIdToNumTF_)
    {
      assert(!closedWorld_[predId]);
      if (gndAtoms == 0)
      	return gndPredValues_[predId]->size() -(*predIdToNumTF_)[predId].numTrue
        	                                  -(*predIdToNumTF_)[predId].numFalse;
      else if (gndAtoms >= 1)
        return numberOfGroundings_[predId] -(*predIdToNumTF_)[predId].numTrue
        	                                  -(*predIdToNumTF_)[predId].numFalse;
    }
    else
    {
      if ((*predIdToGndPreds_)[predId]==NULL) return 0;
      return (*predIdToGndPreds_)[predId]->getNumUnknownGndPreds(); 
    }
  }


  int getNumPredIds() const 
  {
    if (predIdToNumTF_) return predIdToNumTF_->size();
    return predIdToGndPreds_->size(); 
  };

  bool isClosedWorld(const int& predId) const { return closedWorld_[predId]; }
  void setClosedWorld(const int& predId, const bool& b) 
  { closedWorld_[predId] = b; }
  const Array<bool>& getClosedWorld() const { return closedWorld_; }

  
    // FOR TESTING ONLY
    // Caller should not delete returned array nor modify its contents.
  const PredicateHashArray* getTrueGndPreds(const int& predId) const 
  {
    if ((*predIdToGndPreds_)[predId]==NULL) return NULL;
    return (*predIdToGndPreds_)[predId]->getTrueGndPreds(); 
  }

    // FOR TESTING ONLY
    // Caller should not delete returned array nor modify its contents.
  const PredicateHashArray* getFalseGndPreds(const int& predId) const 
  {
    if ((*predIdToGndPreds_)[predId]==NULL) return NULL;
    return (*predIdToGndPreds_)[predId]->getFalseGndPreds(); 
  }

    // FOR TESTING ONLY
    // Caller should not delete returned array nor modify its contents.
  const PredicateHashArray* getUnknownGndPreds(const int& predId) const 
  {
    if ((*predIdToGndPreds_)[predId]==NULL) return NULL;
    return (*predIdToGndPreds_)[predId]->getUnknownGndPreds(); 
  }


	//add this predicate to the given index type
  void addToInverseIndex(Predicate* pred, int idxType)
  {
	int numTerms = pred->getNumTerms();
	if(numTerms == 2)
	{
	  int predId = pred->getId();
      int idx = getIdxOfGndPredValues(pred);
	  for(int term = 0; term < numTerms; term++)
	  {
		int constantId = pred->getTerm(term)->getId();
		assert(constantId >= 0);
		int constantIndex = domain_->getConstantIndexInType(constantId);
		
		paramPredsArray[idxType][predId][term][constantIndex].append(idx);
	  }
    }
  }

	//remove this predicate from the given index type
  void removeFromInverseIndex(Predicate* pred, int idxType)
  {
	int numTerms = pred->getNumTerms();
	if(numTerms == 2)
	{
	  int predId = pred->getId();
      int idx = getIdxOfGndPredValues(pred);
	  for(int term = 0; term < numTerms; term++)
	  {
		int constantId = pred->getTerm(term)->getId();
		assert(constantId >= 0);
		int constantIndex = domain_->getConstantIndexInType(constantId);
		int pos = paramPredsArray[idxType][predId][term][constantIndex].find(idx);
		if (pos > -1)
		{
		  paramPredsArray[idxType][predId][term][constantIndex].removeItemFastDisorder(pos);
		}
	  }
	}
  }

	// Caller should delete indexedGndings and elements of indexedGndings, if necessary
  void getIndexedGndings(Array<Predicate*>* const & indexedGndings,
  						 Predicate* const & pred, bool const & ignoreActivePreds)
  {
  	assert(pred->getNumTerms() == 2);
  	assert(gndAtoms == 2);
  	int predId = pred->getId();

	const Term* term0 = pred->getTerm(0);
	const Term* term1 = pred->getTerm(1);
	  
	  // Both terms are grounded
	if (term0->getType() == Term::CONSTANT && term1->getType() == Term::CONSTANT)
	{
	  Predicate* p = new Predicate(*pred);
	  indexedGndings->append(p);
	  return;
	}

	  // Neither term is grounded
	if (term0->getType() != Term::CONSTANT && term1->getType() != Term::CONSTANT)
	{
	  const Array<int>* constArray =
  	    domain_->getConstantsByType(pred->getTermTypeAsInt(0));
	
		//
  	  for (int j = 0; j < constArray->size(); j++)
  	  {
  	  	int constIdx = domain_->getConstantIndexInType((*constArray)[j]);
  	  	Array<int> preds = paramPredsArray[T_INDEX][predId][0][constIdx];
		for (int i = 0; i < preds.size(); i++)
  	  	{
		  Predicate* p = getPredFromIdx(preds[i], pred->getTemplate());
		  indexedGndings->append(p);
  	  	}
  	  }
  	
	  if (!ignoreActivePreds)
  	  {
  	  	for (int j = 0; j < constArray->size(); j++)
  	  	{
  	  	  int constIdx = domain_->getConstantIndexInType((*constArray)[j]);
  	  	  Array<int> preds = paramPredsArray[FA_INDEX][predId][0][constIdx];
  	      for (int i = 0; i < preds.size(); i++)
  	      {
		  	Predicate* p = getPredFromIdx(preds[i], pred->getTemplate());
		  	indexedGndings->append(p);
  	      }
  	  	}
  	  }	
	  return;	
	}

	  // If we've reached here, then one term is grounded, the other isn't
	int gndTerm = (term0->getType() == Term::CONSTANT) ? 0 : 1;
	int constIdx = domain_->getConstantIndexInType(pred->getTerm(gndTerm)->getId());
  	Array<int> preds = paramPredsArray[T_INDEX][predId][gndTerm][constIdx];
  	for (int i = 0; i < preds.size(); i++)
  	{
	  Predicate* p = getPredFromIdx(preds[i], pred->getTemplate());
	  indexedGndings->append(p);
  	 }
  	  
  	if (!ignoreActivePreds)
  	{
  	  Array<int> preds = paramPredsArray[FA_INDEX][predId][gndTerm][constIdx];
  	  for (int i = 0; i < preds.size(); i++)
  	  {
		Predicate* p = getPredFromIdx(preds[i], pred->getTemplate());
		indexedGndings->append(p);
  	  }
  	}	
  }

 private:
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
  
  Predicate* getPredFromIdx(int idx, const PredicateTemplate* const & predTemplate) const
  {
	Predicate* p = new Predicate(predTemplate);
	Array<int>* auxIdx = new Array<int>(predTemplate->getNumTerms());
	Array<MultAndType>* multAndTypes = termMultByPred_[p->getId()];
	for (int i = 0; i < predTemplate->getNumTerms(); i++)
	{
	  int aux = 0;
	  for (int j = 0; j < i; j++)
	  {
	  	aux += (*auxIdx)[j] * ((*multAndTypes)[j].first);
	  }
	  (*auxIdx)[i] = (idx - aux) / (*multAndTypes)[i].first;
	  int constId = (*auxIdx)[i] + firstConstIdByType_[(*multAndTypes)[i].second];
	  p->setTermToConstant(i, constId);
	}
	delete auxIdx;
	return p;
  }
  

 private:
  const Domain* domain_; // not owned by Database

    // closedWorld_[p]==true: the closed world assumption is made for pred p
  Array<bool> closedWorld_; 

    // gndPredValues_[p] contains the truth values of the groundings of pred p
  Array<Array<TruthValue>*> gndPredValues_;
   
  //added by Parag: Dec 2005
  bool lazyFlag_;
  
  // Flag set when performing inference. If set, hash table of false ground atoms
  // is not used (all atoms are true or false).
  bool performingInference_;
  
  // gndPredActiveStatus_[p] contains the active status of the groundings of pred p
  Array<Array<bool>*> gndPredActiveStatus_;
  
  // gndPredEvidenceStatus_[p] contains whether p is evdience predicate or not
  Array<Array<bool>*> gndPredEvidenceStatus_;

    //firstConstId_[c] is the id of the first constant of class c
  Array<unsigned int> firstConstIdByType_;

    //Suppose a predicate with id i has three terms, and the number of 
    //groundings of the first, second and third terms are 
    //4, 3, 2 respectively. Then termMultByPred_[i][0].first = 3x2,
    //termMultByPred_[i][1].first = 2, and termMultByPred_[i][2].first = 1.
    //termMultByPred_[i][j].first is the product of the number of groundings
    //of terms j > i;
  Array<Array<MultAndType>*> termMultByPred_;

    // predIdToGndPreds_[p] contains the TRUE/FALSE/UNKNOWN groundings of pred
    // with id p
  Array<GroundPreds*>* predIdToGndPreds_;

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
  Array<int> ***paramPredsArray[INDEX_TYPE_COUNT];
  
	// Hash set of true ground atoms
  Array<hash_set<int> >* truePredIdxSet_;
  
	// Hash set of false ground atoms - only used if open world
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
