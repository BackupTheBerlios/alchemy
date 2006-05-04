#include "clause.h"
#include "domain.h"
#include "database.h"
#include "mln.h"
#include "truefalsegroundingsstore.h"


Domain::~Domain()
{
  if (typeDualMap_) delete typeDualMap_;
  if (constDualMap_) delete constDualMap_;
  
  if (strToPredTemplateMap_)
  {
      // strToPredTemplateMap_'s keys (predicate names) are shared with 
      // predDualMap_. The latter will delete the keys.
    StrToPredTemplateMap::iterator it = strToPredTemplateMap_->begin(); 
    for (; it != strToPredTemplateMap_->end(); it++)
      delete (*it).second; //delete PredTemplate*;
    delete strToPredTemplateMap_;
  }
  if (predDualMap_) delete predDualMap_;

  if (strToFuncTemplateMap_)
  {
      // strToFuncTemplateMap_'s keys (function names) are shared with 
      // funcDualMap_. The latter will delete the keys.
    StrToFuncTemplateMap::iterator it2 = strToFuncTemplateMap_->begin(); 
    for (; it2 != strToFuncTemplateMap_->end(); it2++)
      delete (*it2).second; //delete FuncTemplate*;
    delete strToFuncTemplateMap_;
  }
  if (funcDualMap_) delete funcDualMap_;
  
  if (equalPredTemplate_) delete equalPredTemplate_;
  
  if (constantsByType_)
  {
    for (int i = 0; i < constantsByType_->size(); i++)
      delete (*constantsByType_)[i];
    delete constantsByType_;
  }

  if (db_) delete db_;

  if (trueFalseGroundingsStore_) delete trueFalseGroundingsStore_;
  
  if (funcSet_)
  {
    FunctionSet::iterator fit;
    while (!funcSet_->empty())
    { 
      fit = funcSet_->begin();
      funcSet_->erase(fit);
      delete *fit;
    }
    delete funcSet_;
  }
}


void Domain::deleteDB() { if (db_) delete db_; db_ = NULL; }


void Domain::newTrueFalseGroundingsStore() 
{ trueFalseGroundingsStore_ = new TrueFalseGroundingsStore(this); }

  
void Domain::compress() 
{
  typeDualMap_->compress();
  constDualMap_->compress();
  predDualMap_->compress();
  funcDualMap_->compress();
  constantsByType_->compress();
  for (int i = 0; i < constantsByType_->size(); i++)
    (*constantsByType_)[i]->compress();
  db_->compress();
}


void Domain::changePredTermsToNewIds(Predicate* const & p,
                                      hash_map<int,int>& oldToNewConstIds)
{
  for (int j = 0; j < p->getNumTerms(); j++)
  {
    Term* t = (Term*) p->getTerm(j);
    if (t->getType() == Term::CONSTANT)
    {
      int oldId = t->getId();
      assert(oldToNewConstIds.find(oldId) != oldToNewConstIds.end());
      t->setId(oldToNewConstIds[oldId]);
    }
  }
} 


  // Change the constant ids so that constants of the same type are ordered 
  // consecutively. Ensure that the constants in the mln and map is consistent
  // with the new constant ids.
void Domain::reorderConstants(MLN* const & mln,
                           hash_map<int, PredicateHashArray*>& predIdToPredsMap)
{  
  //order the constants so that those of the same type are ordered consecutively

  hash_map<int,int> oldToNewConstIds;

  ConstDualMap* newConstDualMap = new ConstDualMap;
  Array<Array<int>*>* newConstantsByType = new Array<Array<int>*>;

  int prevNewConstId = -1;
  bool constChanged = false;
  for (int i = 0; i < constantsByType_->size(); i++)
  {
    newConstantsByType->append(new Array<int>);
    Array<int>* constIds = (*constantsByType_)[i];
    for (int j = 0; j < constIds->size(); j++)
    {
      int constId = (*constIds)[j];
      const char* constName = getConstantName(constId);
      int newConstId = newConstDualMap->insert(constName, i);
      assert(prevNewConstId+1==newConstId);
      prevNewConstId = newConstId;
      (*newConstantsByType)[i]->append(newConstId);
      oldToNewConstIds[constId] = newConstId;
      if (constId != newConstId) constChanged = true;
    }
  }

  if (!constChanged)
  {
    delete newConstDualMap;
    for (int i = 0; i < newConstantsByType->size(); i++)
      delete (*newConstantsByType)[i];
    delete newConstantsByType;
    return;
  }

  delete constDualMap_;
  for (int i = 0; i < constantsByType_->size(); i++)
    delete (*constantsByType_)[i];
  delete constantsByType_;

  constDualMap_ = newConstDualMap;
  constantsByType_ = newConstantsByType;

  constantsByType_->compress();
  for (int i = 0; i < constantsByType_->size(); i++)
    (*constantsByType_)[i]->compress();
  constDualMap_->compress();

  //ensure that the constants in MLN clauses have the new ids 

  const ClauseHashArray* clauses = mln->getClauses();
  for (int i = 0; i < clauses->size(); i++)
  {
    Clause* c = (*clauses)[i];
    for (int i = 0; i < c->getNumPredicates(); i++)
      changePredTermsToNewIds(c->getPredicate(i), oldToNewConstIds);
  }

  hash_map<int, PredicateHashArray*>::iterator mit = predIdToPredsMap.begin();
  for (; mit != predIdToPredsMap.end(); mit++)
  {
    PredicateHashArray* pha = (*mit).second;
    for (int i = 0; i < pha->size(); i++ )
      changePredTermsToNewIds((*pha)[i], oldToNewConstIds);
  }
  
    //clauses and hence their hash values have changed, and they need to be 
    //inserted into the MLN
  mln->rehashClauses();
}


  //Caller is responsible for deleting returned pointer
Predicate* Domain::createPredicate(const int& predId, 
                                   const bool& includeEqualPreds) const
{
  const PredicateTemplate* pt = getPredicateTemplate(predId);
  if (!includeEqualPreds && pt->isEqualPredicateTemplate()) return NULL;
  Predicate* pred = new Predicate(pt);
  pred->setSense(true);
  for (int j = 0; j < pt->getNumTerms(); j++)
    pred->appendTerm(new Term(-(j+1), (void*)pred, true));
  return pred;
}


  //Caller is responsible for deleting Array and its contents
void Domain::createPredicates(Array<Predicate*>* const & preds,
                              const bool& includeEqualPreds) const
{
  for (int i = 0; i < getNumPredicates(); i++)
  {
    Predicate* p = createPredicate(i, includeEqualPreds);
    if (p) preds->append(p);
  }
}


  //Caller is responsible for deleting Array and its contents
void Domain::createPredicates(Array<Predicate*>* const & preds,
                              const Array<string>* const & predNames)
{
  for (int i = 0; i < predNames->size(); i++)
  {
    int predId = getPredicateId((*predNames)[i].c_str());    
    Predicate* p = createPredicate(predId, true);
    if (p) preds->append(p);
  }
}


