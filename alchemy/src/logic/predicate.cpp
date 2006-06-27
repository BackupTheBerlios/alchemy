#include "predicate.h"
#include "clause.h"
#include "groundpredicate.h"

double Predicate::fixedSizeB_ = -1;


void Predicate::setDirty() { dirty_ = true; if (parent_) parent_->setDirty(); }


void Predicate::setParentDirty() { if (parent_) parent_->setDirty(); }


bool Predicate::canBeGroundedAs(const GroundPredicate* const & gndPred)
{
  if ( ((unsigned int) template_->getId()) != gndPred->getId()) return false;
  if (allTermsAreDiffVars()) return true;
    
  int varGndings[MAX_VAR];
  memset(varGndings, -1, MAX_VAR*sizeof(int));
  for (int i = 0; i < terms_->size(); i++) 
  {
    int termType = (*terms_)[i]->getType();
      
    if (termType == Term::CONSTANT) 
    {
      assert((*terms_)[i]->getId() >= 0);
      if ( ((unsigned int) (*terms_)[i]->getId()) != gndPred->getTermId(i)) 
        return false;
    }
    else 
    if (termType == Term::VARIABLE) 
    {
      int varId = -(*terms_)[i]->getId();
      assert(varId > 0);
      assert(varId < MAX_VAR);
      if (varGndings[varId] < 0) // if variable has not been grounded
        varGndings[varId] = gndPred->getTermId(i);
      else 
      if (varGndings[varId] != (int) gndPred->getTermId(i))
        return false;
    }
    else
    {
      assert(false);
    }
  }
  return true;
}

  //Does not consider the sense of the predicate.
bool Predicate::same(const GroundPredicate* const & gp)
{
  if ( getId() != (int) gp->getId() ) return false;
  if ( terms_->size() != (int) gp->getNumTerms() ) return false;
  for (int i = 0; i < terms_->size(); i++)
    if ( (*terms_)[i]->getId() != (int) gp->getTermId(i) ) return false;
  return true;
}

double 
Predicate::getNumGroundingsIfAllVarDiff(const Domain* const & domain) const
{ 
  int numGnd = 1;
  for (int i = 0; i < template_->getNumTerms(); i++)
    numGnd *= domain->getNumConstantsByType(template_->getTermTypeAsInt(i));
  return numGnd;
}

    //Caller is responsible for deleting the Predicate* in returnArray
void Predicate::createAllGroundings(const int& predId, 
                                    const Domain* const & domain,
                                    Array<Predicate*>& returnArray)
{
  const PredicateTemplate* pt = domain->getPredicateTemplate(predId);
  if (pt == NULL)
  {
    cout << "ERROR in Predicate::createAllGroundings: no predicate with id " 
         << predId << " in domain" << endl;
    return;
  }
  if (pt->isEqualPredicateTemplate())
  {
    cout << "ERROR in Predicate::createAllGroundings: cannot create " 
         << "groundings for '=' predicate" << endl;
    return;      
  }
  
  ArraysAccessor<int> acc;
  for (int i = 0; i < pt->getNumTerms(); i++)
  {
    int typeId = pt->getTermTypeAsInt(i);
    assert(typeId >= 0);
    const Array<int>* constArr = domain->getConstantsByType(typeId);
    assert(constArr->size() > 0);
    acc.appendArray(constArr);
  }
  
  
  while (acc.hasNextCombination())
  {
    Predicate* pred = new Predicate(pt);
    int constId;
    while (acc.nextItemInCombination(constId))
      pred->appendTerm(new Term(constId, (void*)pred, true));
    returnArray.append(pred);
    
  }
}

//get all the groundings unifying with the term given 
//Caller is responsible for deleting the Predicate* in returnArray
void Predicate::createAllGroundingsUnifyingWithTerm(const int& predId, 
                                    const Domain* const & domain,
                                    Array<Predicate*>& returnArray,
									int termTypeId,int termConstId)
{
  Array<int> tmpArr;
  const PredicateTemplate* pt = domain->getPredicateTemplate(predId);
  if (pt == NULL)
  {
    cout << "ERROR in Predicate::createAllGroundings: no predicate with id " 
         << predId << " in domain" << endl;
    return;
  }
  if (pt->isEqualPredicateTemplate())
  {
    cout << "ERROR in Predicate::createAllGroundings: cannot create " 
         << "groundings for '=' predicate" << endl;
    return;      
  }
  
  ArraysAccessor<int> acc;
  for (int i = 0; i < pt->getNumTerms(); i++)
  {
    int typeId = pt->getTermTypeAsInt(i);
    assert(typeId >= 0);
	const Array<int>* constArr;
    if(typeId != termTypeId) {
	 constArr = domain->getConstantsByType(typeId);
	} else {
	 tmpArr.clear();
	 tmpArr.append(termConstId);
	 constArr = &tmpArr;
	}
	assert(constArr->size() > 0);
	acc.appendArray(constArr);
  }
  
  while (acc.hasNextCombination())
  {
    Predicate* pred = new Predicate(pt);
    int constId;
    while (acc.nextItemInCombination(constId))
      pred->appendTerm(new Term(constId, (void*)pred, true));
    returnArray.append(pred);
    
  }
}


  //exactly one of  predReturnArray or constReturnArray must be NULL;
void Predicate::createAllGroundings(const Domain* const & domain,
                                    Array<Predicate*>* const & predReturnArray,
                                    Array<int*>* const & constReturnArray)
{
  assert(predReturnArray == NULL || constReturnArray == NULL);

  if (isGrounded())
  {
    if (predReturnArray)
      predReturnArray->append(new Predicate(*this));
    else
    {
      assert(constReturnArray);
      int* constArr = new int[terms_->size()];
      for (int j = 0; j < terms_->size(); j++) 
      {
        constArr[j] = (*terms_)[j]->getId();
        assert(constArr[j] >= 0);
      }
      constReturnArray->append(constArr);
    }
    return;
  }


  Array<VarsGroundedType*> vgtArr;
  for (int i = 0; i < terms_->size(); i++)
  {
    const Term* t = (*terms_)[i];
    if (t->getType() == Term::VARIABLE)
    {
      int id = -(t->getId());
      assert(id > 0);
      if (id >= vgtArr.size()) vgtArr.growToSize(id+1,NULL);
      VarsGroundedType*& vgt = vgtArr[id];
      if (vgt == NULL) 
      {
        vgt = new VarsGroundedType; 
        vgt->typeId = getTermTypeAsInt(i);
        assert(vgt->typeId >= 0);        
      }
      assert(getTermTypeAsInt(i) == vgt->typeId);
      vgt->vars.append((Term*)t);
    }
  }

  Array<int> negVarId; 
  ArraysAccessor<int> acc;
  for (int i = 0; i < vgtArr.size(); i++)
  {
    if (vgtArr[i] == NULL) continue;
    negVarId.append(i);
    acc.appendArray(domain->getConstantsByType(vgtArr[i]->typeId));
  }

  while (acc.hasNextCombination())
  {
    int i = 0;
    int constId;
    while (acc.nextItemInCombination(constId))
    {
      Array<Term*>& vars = vgtArr[ negVarId[i] ]->vars;
      for (int j = 0; j < vars.size(); j++) vars[j]->setId(constId);
      i++;
    }
    
    if (predReturnArray) 
      predReturnArray->append(new Predicate(*this));
    else
    {
      assert(constReturnArray);
      int* constArr = new int[terms_->size()];
      for (int j = 0; j < terms_->size(); j++) 
      {
        constArr[j] = (*terms_)[j]->getId();
        assert(constArr[j] >= 0);
      }
      constReturnArray->append(constArr);
    }
  }

    //restore
  for (int i = 0; i < vgtArr.size(); i++) 
  {
    if (vgtArr[i] == NULL) continue;
    Array<Term*>& vars  = vgtArr[i]->vars;
    for (int j = 0; j < vars.size(); j++) vars[j]->setId(-i);
    delete vgtArr[i];
  }  
}
