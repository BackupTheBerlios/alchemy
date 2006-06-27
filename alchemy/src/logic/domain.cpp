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
  
  if (emptyPredTemplate_) delete emptyPredTemplate_;
  
  if (emptyFuncUnaryTemplate_) delete emptyFuncUnaryTemplate_;
  
  if (emptyFuncBinaryTemplate_) delete emptyFuncBinaryTemplate_;
  
  if (constantsByType_)
  {
    for (int i = 0; i < constantsByType_->size(); i++)
      delete (*constantsByType_)[i];
    delete constantsByType_;
  }

  if (predBlocks_) delete predBlocks_;

  if (blockEvidence_) delete blockEvidence_;

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

    // Change the const ids in the pred blocks
  for (int i = 0; i < predBlocks_->size(); i++)
  {
    Array<Predicate*>* predBlock = (*predBlocks_)[i];
    for (int j = 0; j < predBlock->size(); j++)
      changePredTermsToNewIds((*predBlock)[j], oldToNewConstIds);
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

int Domain::getNumNonEvidenceAtoms() const
{
  int numAtoms = 0;
  for (int i = 0; i < getNumPredicates(); i++)
  {
    numAtoms += (db_->getNumGroundings(i) - db_->getNumEvidenceGndPreds(i));
  }
  return numAtoms;
}

/*
 * Caller is responsible for deleting returned Predicate* if necessary
 */
Predicate* Domain::getNonEvidenceAtom(int index) const
{
  int predId = -1;
  int numAtomsPerPred;
  int numAtoms = 0;
  for (int i = 0; i < getNumPredicates(); i++)
  {
    numAtomsPerPred = (db_->getNumGroundings(i) - db_->getNumEvidenceGndPreds(i));
    if (numAtoms + numAtomsPerPred >= index + 1)
    {
      predId = i;
      break;
    }
    numAtoms += numAtomsPerPred;
  }
  assert(predId >= 0);
  index = index - numAtoms;
  
  Array<Predicate*> preds;
  Predicate* gndPred = NULL;
  Predicate::createAllGroundings(predId, this, preds);
  int nePreds = 0;
  for (int i = 0; i < preds.size(); i++)
  {
    if (!db_->getEvidenceStatus(preds[i]) && index == nePreds++)
      gndPred = preds[i];
    else
      delete preds[i];
  }
  assert(gndPred);
  return gndPred;
}

int Domain::addPredBlock(Array<Predicate*>* const & predBlock) const
{
  int idx = predBlocks_->append(predBlock);
  blockEvidence_->append(false);
  return idx;
}

  // returns the index of the block which the ground predicate
  // with index predIndex is in. If not in any, returns -1
int Domain::getBlock(Predicate* pred) const
{
  for (int i = 0; i < predBlocks_->size(); i++)
  {
    Array<Predicate*>* block = (*predBlocks_)[i];
    for (int j = 0; j < block->size(); j++)
    {
      if (pred->same((*block)[j]))
        return i;
    }
  }
  return -1;
}

int Domain::getNumPredBlocks() const
{
  return predBlocks_->size();
}

const Array<Array<Predicate*>*>* Domain::getPredBlocks() const
{
  return predBlocks_;
}

const Array<Predicate*>* Domain::getPredBlock(const int index) const
{
  return (*predBlocks_)[index];
}

const Array<bool>* Domain::getBlockEvidenceArray() const
{
  return blockEvidence_;
}

const bool Domain::getBlockEvidence(const int index) const
{
  return (*blockEvidence_)[index];
}

void Domain::setBlockEvidence(const int index, const bool value) const
{
  (*blockEvidence_)[index] = value;
}

int Domain::getEvidenceIdxInBlock(const int index) const
{
  Array<Predicate*>* block = (*predBlocks_)[index];
  for (int i = 0; i < block->size(); i++)
  {
    if (db_->getEvidenceStatus((*block)[i]))
      return i;
  }
  return -1;
}
