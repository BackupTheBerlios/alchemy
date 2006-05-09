#ifndef DOMAIN_H_JUN_21_2005
#define DOMAIN_H_JUN_21_2005

#include <ext/hash_set>
#include "equalstr.h"
#include "dualmap.h"
#include "constdualmap.h"
#include "predicatetemplate.h"
#include "functiontemplate.h"
#include "predicate.h"
#include "function.h"

class Clause;
class Database;
class TrueFalseGroundingsStore;
class MLN;


typedef hash_map<const char*, const PredicateTemplate*, hash<const char*>, EqualStr>  StrToPredTemplateMap;

typedef hash_map<const char*, const FunctionTemplate*, hash<const char*>, EqualStr>  StrToFuncTemplateMap;


class Domain
{
 public:
  Domain() : typeDualMap_(new DualMap), constDualMap_(new ConstDualMap),
             predDualMap_(new DualMap), funcDualMap_(new DualMap),
             strToPredTemplateMap_(new StrToPredTemplateMap),
             strToFuncTemplateMap_(new StrToFuncTemplateMap),
             constantsByType_(new Array<Array<int>*>), db_(NULL),
             trueFalseGroundingsStore_(NULL), funcSet_(new FunctionSet) 
  {
    equalPredTemplate_ = new PredicateTemplate();
    equalPredTemplate_->setName(PredicateTemplate::EQUAL_NAME);
    emptyPredTemplate_ = new PredicateTemplate();
    emptyPredTemplate_->setName(PredicateTemplate::EMPTY_NAME);
    emptyFuncUnaryTemplate_ = new FunctionTemplate();
    emptyFuncUnaryTemplate_->setName(FunctionTemplate::EMPTY_FTEMPLATE_NAME);
    emptyFuncBinaryTemplate_ = new FunctionTemplate();
    emptyFuncBinaryTemplate_->setName(FunctionTemplate::EMPTY_FTEMPLATE_NAME);
    
    int typeId = addType(PredicateTemplate::ANY_TYPE_NAME);
    if (typeId != 0)
    {
      cout << "ERROR in domain.h: typeId of EQUAL predicate is not 0." << endl;
      exit(-1);
    }
    equalPredTemplate_->appendTermType(PredicateTemplate::ANY_TYPE_NAME, 
                                       false, this);
    equalPredTemplate_->appendTermType(PredicateTemplate::ANY_TYPE_NAME, 
                                       false, this);
    emptyPredTemplate_->appendTermType(PredicateTemplate::ANY_TYPE_NAME, 
                                       false, this);
    emptyPredTemplate_->appendTermType(PredicateTemplate::ANY_TYPE_NAME, 
                                       false, this);
    emptyFuncUnaryTemplate_->appendTermType(FunctionTemplate::ANY_TYPE_NAME, 
                                       false, this);
    emptyFuncBinaryTemplate_->appendTermType(FunctionTemplate::ANY_TYPE_NAME, 
                                       false, this);
    emptyFuncBinaryTemplate_->appendTermType(FunctionTemplate::ANY_TYPE_NAME, 
                                       false, this);
                                       
  }


  ~Domain();


  void replaceTypeDualMap(DualMap* const & map)
  {
    if (typeDualMap_) delete typeDualMap_;
    typeDualMap_ =  map;
  }

  void setTypeDualMap(DualMap* const & map) { typeDualMap_ =  map; }
  

  const DualMap* getTypeDualMap() const { return typeDualMap_; }


  void replaceStrToPredTemplateMapAndPredDualMap(StrToPredTemplateMap* const& m,
                                                 DualMap* const & predDualMap)
  {
    if (strToPredTemplateMap_)
    {
      StrToPredTemplateMap::iterator it = strToPredTemplateMap_->begin(); 
      for (; it != strToPredTemplateMap_->end(); it++)
        delete (*it).second; //delete PredTemplate*;
      delete strToPredTemplateMap_;
    }
    if (predDualMap_) delete predDualMap_;
    strToPredTemplateMap_ = m;
    predDualMap_ = predDualMap;
  }


  void setStrToPredTemplateMapAndPredDualMap(StrToPredTemplateMap* const & map,
                                             DualMap* const & predDualMap)
  { strToPredTemplateMap_ = map; predDualMap_ = predDualMap; }


  const StrToPredTemplateMap* getStrToPredTemplateMap() const
  { return strToPredTemplateMap_; }


  const DualMap* getPredDualMap() const { return predDualMap_; }


  void replaceStrToFuncTemplateMapAndFuncDualMap(StrToFuncTemplateMap* const& m,
                                                 DualMap* const & funcDualMap)
  {
    if (strToFuncTemplateMap_)
    {
      StrToFuncTemplateMap::iterator it2 = strToFuncTemplateMap_->begin(); 
      for (; it2 != strToFuncTemplateMap_->end(); it2++)
        delete (*it2).second; //delete FuncTemplate*;
      delete strToFuncTemplateMap_;
    }
    if (funcDualMap_) delete funcDualMap_;
    strToFuncTemplateMap_ = m; 
    funcDualMap_ = funcDualMap;
  }


  void setStrToFuncTemplateMapAndFuncDualMap(StrToFuncTemplateMap* const& m,
                                             DualMap* const & funcDualMap)
  { strToFuncTemplateMap_ = m;  funcDualMap_ = funcDualMap; }


  const StrToFuncTemplateMap* getStrToFuncTemplateMap() const
  { return strToFuncTemplateMap_; }


  const DualMap* getFuncDualMap() const { return funcDualMap_; }

  
  void replaceEqualPredTemplate(PredicateTemplate* const & t)
  {
    if (equalPredTemplate_) delete equalPredTemplate_;
    equalPredTemplate_ = t;
  }

  void setEqualPredTemplate(PredicateTemplate* const & t) 
  { equalPredTemplate_ = t; }

  
  const PredicateTemplate* getEqualPredTemplate() const
  { return equalPredTemplate_; }

  void replaceEmptyPredTemplate(PredicateTemplate* const & t)
  {
    if (emptyPredTemplate_) delete emptyPredTemplate_;
    emptyPredTemplate_ = t;
  }

  void setEmptyPredTemplate(PredicateTemplate* const & t) 
  { emptyPredTemplate_ = t; }

  
  const PredicateTemplate* getEmptyPredTemplate() const
  { return emptyPredTemplate_; }

  void replaceEmptyFuncUnaryTemplate(FunctionTemplate* const & t)
  {
    if (emptyFuncUnaryTemplate_) delete emptyFuncUnaryTemplate_;
    emptyFuncUnaryTemplate_ = t;
  }

  void replaceEmptyFuncBinaryTemplate(FunctionTemplate* const & t)
  {
    if (emptyFuncBinaryTemplate_) delete emptyFuncBinaryTemplate_;
    emptyFuncBinaryTemplate_ = t;
  }

  void setEmptyFuncUnaryTemplate(FunctionTemplate* const & t) 
  { emptyFuncUnaryTemplate_ = t; }

  void setEmptyFuncBinaryTemplate(FunctionTemplate* const & t) 
  { emptyFuncBinaryTemplate_ = t; }

  
  const FunctionTemplate* getEmptyFuncUnaryTemplate() const
  { return emptyFuncUnaryTemplate_; }

  const FunctionTemplate* getEmptyFuncBinaryTemplate() const
  { return emptyFuncBinaryTemplate_; }

  void replaceConstDualMap(ConstDualMap* const & map)
  {
    if (constDualMap_) delete constDualMap_;
    constDualMap_ = map;
  }

  void setConstDualMap(ConstDualMap* const & map) { constDualMap_ = map; }


  const ConstDualMap* getConstDualMap() const { return constDualMap_; }


  void replaceConstantsByType(Array<Array<int>*>* const & cbt)
  {
    if (constantsByType_)
    {
      for (int i = 0; i < constantsByType_->size(); i++)
        delete (*constantsByType_)[i];
      delete constantsByType_;
    }
    constantsByType_ = cbt;
  }


  void setConstantsByType(Array<Array<int>*>* const & cbt) 
  { constantsByType_ = cbt; }

  
  const Array<Array<int>*>* getConstantsByType() const 
  { return constantsByType_; }


  void replaceFuncSet(FunctionSet* const & funcSet)
  {
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
    funcSet_ = funcSet;
  }


  void setFuncSet(FunctionSet* const & funcSet)  { funcSet_ = funcSet; }


  const FunctionSet* getFuncSet() const { return funcSet_; }

  void deleteDB();
  Database* getDB() const { return db_; }
  void setDB(Database* const & db) { db_ = db; }

  void newTrueFalseGroundingsStore();
  TrueFalseGroundingsStore* getTrueFalseGroundingsStore() const
  { return trueFalseGroundingsStore_; }
  void setTrueFalseGroundingsStore(TrueFalseGroundingsStore* const & tfgs)
  { trueFalseGroundingsStore_ = tfgs; }


  void compress();

  void reorderConstants(MLN* const & mln, 
                        hash_map<int, PredicateHashArray*>& predIdToPredsMap);

    //Caller is responsible for deleting returned pointer
  Predicate* createPredicate(const int& predId, 
                             const bool& includeEqualPreds) const;

    //Caller is responsible for deleting Array and its contents
  void createPredicates(Array<Predicate*>* const & preds,
                        const bool& includeEqualPreds) const;

  void createPredicates(Array<Predicate*>* const & preds,
                        const Array<string>* const & predNames);

  //////////////////////// type functions //////////////////////

    // Returns id of type or -1 if type has been added before.
    // Type id increases by one each time addType() is called.
    // Caller is responsible for deleting name if required. 
  int addType(const char* const& name) 
  { 
    int typeId = typeDualMap_->insert(name); 
    if (typeId < 0) return -1;

    if (typeId != constantsByType_->size())
    {
      cout << "Error: In Domain::addType(). Expected typeId " << typeId 
           << " to be equal to " << constantsByType_->size() << endl;
      exit(-1);
    }
      
    constantsByType_->append(new Array<int>);
    return typeId;
  }


  int getNumTypes() const { return typeDualMap_->getNumInt(); }


    // Caller should NOT delete the returned array.
  const Array<const char*>* getTypeNames() const  
  { return typeDualMap_->getIntToStrArr(); }


    // Caller is responsible for deleting name if required.
    // Returns -1 if type does not exist.
  int getTypeId(const char* const & name) const 
  { return typeDualMap_->getInt(name); }
  
    // Caller should not delete returned const char*
    // Returns NULL if type id does not exist.
  const char* getTypeName(const int typeId) const
  { return typeDualMap_->getStr(typeId); }

  bool isType(const char* const & name) const { return (getTypeId(name) >= 0); }
  bool isType(const int& id) const { return (getTypeName(id) != NULL); }


  //////////////////////// constant functions //////////////////////

    // Caller must not delete returned pointer.
  const Array<int>* getConstantsByType(const int& typeId) const
  { return (*constantsByType_)[typeId]; }

  const Array<int>* getConstantsByType(const char* const & typeName) const
  { 
    int typeId = getTypeId(typeName);
    if (typeId < 0) return NULL;
    return (*constantsByType_)[typeId]; 
  }

	// Returns the index of the given constant of all constants of this type
  int getConstantIndexInType(const int& constId) const
  {
  	int typeId = getConstantTypeId(constId);
  	const Array<int>* constArray = getConstantsByType(typeId);
  	for (int i = 0; i < constArray->size(); i++)
  	  if (constId == (*constArray)[i]) return i;
  	return -1;
  }
  
  int getNumConstantsByType(const int& typeId) const
  { return ((*constantsByType_)[typeId])->size(); }


  int getNumConstantsByType(const char* const & typeName) const
  {
    int typeId = getTypeId(typeName);
    if (typeId < 0) return 0;
    return ((*constantsByType_)[typeId])->size(); 
  }


    // Returns id of constant or -1 if constant has been added before.
    // Constant id increases by one each time addConstant() is called.
    // Caller is responsible for deleting name and typeName if needed.
  int addConstant(const char* const & constName, const char* const & typeName)
  {
    int typeId =  typeDualMap_->getInt(typeName);
    if (typeId < 0)
    {
      cout << "Warning: failed to add constant " << constName 
           << " because its type of " << typeName << " doesn't exist" << endl;
      return typeId;
    }
    
    int constId 
      = constDualMap_->insert(constName, typeId);
    if (constId < 0)
    {
      cout << "Warning: failed to add constant " << constName << " of type " 
           << typeName << endl;
      return constId;
    }
    
    (*constantsByType_)[typeId]->append(constId);
    return constId;
  }

  /**
   * Replaces the type of a constant from the domain.
   * 
   * @param constName Name of constant being changed.
   * @param oldTypeName Type of constant being chnaged.
   * @param newTypeName New type of the constant.
   * 
   * @return id of constant changed, or -1 if not found.
   */
  int replaceTypeOfConstant(const char* const & constName,
  							const char* const & oldTypeName,
  							const char* const & newTypeName)
  {
    int oldTypeId = typeDualMap_->getInt(oldTypeName);
    if (oldTypeId < 0)
    {
      cout << "Warning: failed to replace type of constant " << constName 
           << " because its type of " << oldTypeName << " doesn't exist" << endl;
      return oldTypeId;
    }
    int newTypeId = typeDualMap_->getInt(newTypeName);
    if (newTypeId < 0)
    {
      cout << "Warning: failed to replace type of constant " << constName 
           << " because its type of " << newTypeName << " doesn't exist" << endl;
      return newTypeId;
    }
    
    int constId 
      = constDualMap_->insert(constName, newTypeId);
    if (constId < 0)
    {
      cout << "Warning: failed to remove constant " << constName << " of type " 
           << oldTypeName << endl;
      return constId;
    }
    
    (*constantsByType_)[oldTypeId]->append(constId);    
    (*constantsByType_)[newTypeId]->append(constId);
    return constId;
  }
  
  int getNumConstants() const { return constDualMap_->getNumInt(); }

    // Caller SHOULD delete the returned array but not its contents.
  const Array<const char*>* getConstantNames()
  { return constDualMap_->getIntToStrArr(); }


    // Caller should not delete returned const char*
    // Returns NULL if id does not exist.
  const char* getConstantName(const int& id) const
  { return constDualMap_->getStr(id); }


    // Caller is responsible for deleting name if required.
    // Returns -1 if the constant isn't found
  int getConstantId(const char* const & name) const
  { return constDualMap_->getInt(name); }


  bool isConstant(const char* const & name) const {return (getConstantId(name) >= 0);}
  bool isConstant(const int& id) const { return (getConstantName(id) != NULL); }

    // Caller should delete the constName argument if required
  int getConstantTypeId(const char* const & constName) const 
  { return constDualMap_->getInt2(constName); }


  int getConstantTypeId(const int& constId) const
  { return constDualMap_->getInt2(constId); }


    // Caller should not delete returned const char*
    // Returns NULL if id does not exist.
  const char* getConstantTypeName(const int& constId) const
  { 
    int typeId = getConstantTypeId(constId);
    return getTypeName(typeId);
  }




  //////////////////////// predicate functions ///////////////////

    // Returns id of predicate or -1 if PredicateTemplate has been added before.
    // predTemplate should have its name_, termTypes_, and domain_
    // assigned before this function is called.
    // Predicate id increases by one each time addPredicateTemplate() is called.
    // Caller should not delete predTemplate. 
  int addPredicateTemplate(const PredicateTemplate* const & predTemplate)
  {
    int predId = predDualMap_->insert(predTemplate->getName());
    if (predId < 0)
    {
      cout << "Warning: failed to add predicate template " 
           << predTemplate->getName() << endl;
      return predId;
    }

    const char* predName = predDualMap_->getStr(predId);
     //strToPredTemplateMap_ shares the predName object with predDualMap_
    (*strToPredTemplateMap_)[predName] = predTemplate; 
    return predId;
  }


  int getNumPredicates() const { return predDualMap_->getNumInt(); }


    // Caller should NOT delete the returned array and its contents.
  const Array<const char*>* getPredicateNames() const 
  { return predDualMap_->getIntToStrArr(); }


  void getNonEqualPredicateNames(Array<string>& predNames) const
  {
    for (int i = 0; i < getNumPredicates(); i++)
    {
      if (getPredicateTemplate(i)->isEqualPredicateTemplate()) continue;
      if (getPredicateTemplate(i)->isInternalPredicateTemplate()) continue;
      if (getPredicateTemplate(i)->isPredicateTemplateFromFunction()) continue;
      predNames.append(getPredicateName(i));
    }
  }


    // Caller is responsible for deleting name if required.
    // Returns id of predicate if it exist, otherwise returns -1
  int getPredicateId(const char* const & name) const
  { return predDualMap_->getInt(name); }

    // Caller should not delete returned const char*
    // Returns name of predicate if it exists, otherwise returns NULL
  const char* getPredicateName(const int& id) const
  { return predDualMap_->getStr(id); }

    //Returns the term type ids of the predicate with the specified name, or
    //NULL if there is no predicate with specified name.
    //Caller should delete name if required.
    //Caller should not delete the returned const Array<int>*
  const Array<int>* getPredicateTermTypesAsInt(const char* const & name) const
  {
    StrToPredTemplateMap::iterator it;
    if ((it=strToPredTemplateMap_->find(name)) == strToPredTemplateMap_->end())
      return NULL;
    return (*it).second->getTermTypesAsInt();
  }


    //Returns the term type ids of the predicate with the specified id, or
    //NULL if there is no predicate with specified id.
    //Caller should not delete the returned const Array<int>*
  const Array<int>* getPredicateTermTypesAsInt(const int& id) const
  { return getPredicateTermTypesAsInt(getPredicateName(id)); }


    //Returns the term type ids of the predicate with the specified name, or
    //NULL if there is no predicate with specified name.
    //Caller should delete name if required.
    //Caller should not delete the returned const Array<const char*>*
  const Array<const char*>* getPredicateTermTypesAsStr(const char* const& name)
    const
  {
    StrToPredTemplateMap::iterator it;
    if ((it=strToPredTemplateMap_->find(name)) == strToPredTemplateMap_->end())
      return NULL;
    return (*it).second->getTermTypesAsStr();
  }


    //Returns the term type ids of the predicate with the specified id, or
    //NULL if there is no predicate with specified id.
    //Caller should not delete the returned const Array<const char*>*
  const Array<const char*>* getPredicateTermTypesAsStr(const int& id)
  { return getPredicateTermTypesAsStr(getPredicateName(id)); }


    // Returns the PredicateTemplate* with the given name or NULL if there is no
    // PredicateTemplate with the specified name.
    // Caller is responsible for deleting name if required.
    // Caller should not delete the returned PredicateTemplate* nor modify it.
  const PredicateTemplate* getPredicateTemplate(const char* const & name) const
  {
    StrToPredTemplateMap::iterator it;
    if ((it=strToPredTemplateMap_->find(name)) == strToPredTemplateMap_->end())
      return NULL;
    return (*it).second;
  }


    // Returns the PredicateTemplate* with the given name or NULL if there is no
    // PredicateTemplate with the specified name.
    // Caller is responsible for deleting name if required.
    // Caller should not delete the returned PredicateTemplate* nor modify it.
  const PredicateTemplate* getPredicateTemplate(const int& id) const
  {
    const char* predName = ((Domain*)this)->getPredicateName(id);
    return getPredicateTemplate(predName);
  }

	/**
	 * Finds the maximum arity of all predicates present in the domain.
	 *  
	 * @return maximum arity of all predicates in the domain.
	 */
  const int getHighestPredicateArity() const
  {
  	int highestArity = 1;
  	int arity;
  	for (int i = 0; i < getNumPredicates(); i++)
    {
      arity = getPredicateTemplate(i)->getNumTerms();
      if (arity > highestArity) highestArity = arity;
    }
    
  	return highestArity;
  }

  const PredicateTemplate* getEqualPredicateTemplate() const
  { return equalPredTemplate_; }
  
  const PredicateTemplate* getEmptyPredicateTemplate() const
  { return emptyPredTemplate_; }
 
  const FunctionTemplate* getEmptyFunctionUnaryTemplate() const
  { return emptyFuncUnaryTemplate_; }

  const FunctionTemplate* getEmptyFunctionBinaryTemplate() const
  { return emptyFuncBinaryTemplate_; }

  bool isPredicate(const char* const & name) const 
  {
  	return (getPredicateId(name)>=0 ||
  			strcmp(name, PredicateTemplate::GT_NAME) == 0 || 
  			strcmp(name, PredicateTemplate::LT_NAME) == 0 ||
  			strcmp(name, PredicateTemplate::GTEQ_NAME) == 0 ||
  			strcmp(name, PredicateTemplate::LTEQ_NAME) == 0 ||
  			strcmp(name, PredicateTemplate::SUBSTR_NAME) == 0);
  	
  }

  bool isPredicate(const int& id) const {return (getPredicateName(id) != NULL);}


  //////////////////////// function functions ///////////////////

    // Returns id of function or -1 if function has been added before.
    // functionTemplate should have its name_, termTypes_, domain_, 
    // and retTypeName_ assigned before this function is called.
    // Function id increases by one each time addFunctionTemplate() is called.
    // Caller should not delete funcTemplate. 
  int addFunctionTemplate(const FunctionTemplate* const & funcTemplate)
  {
    int funcId = funcDualMap_->insert(funcTemplate->getName());
    if (funcId < 0)
    {
      cout << "Warning: failed to add function template " 
           << funcTemplate->getName() << endl;
      return funcId;
    }

    const char* funcName = funcDualMap_->getStr(funcId);
     //strToFuncTemplateMap_ shares the funcName object with funcDualMap_
    (*strToFuncTemplateMap_)[funcName] = funcTemplate; 
    return funcId;
  }


  int getNumFunctions() const { return funcDualMap_->getNumInt(); }


    // Caller should NOT delete the returned array but not its contents.
  const Array<const char*>* getFunctionNames() const 
  { return funcDualMap_->getIntToStrArr(); }


    // Caller is responsible for deleting name if required.
  int getFunctionId(const char* const & name) const
  { return funcDualMap_->getInt(name); }

    // Caller should not delete returned const char*
  const char* getFunctionName(const int& id) const
  { return funcDualMap_->getStr(id); }

    //Returns the term type ids of the function with the specified name, or
    //NULL if there is no function with specified name.
    //Caller should delete name if required.
    //Caller should not delete the returned const Array<int>*
  const Array<int>* getFunctionTermTypesAsInt(const char* const & name) const
  {
    StrToFuncTemplateMap::iterator it;
    if ((it=strToFuncTemplateMap_->find(name)) == strToFuncTemplateMap_->end())
      return NULL;
    return (*it).second->getTermTypesAsInt();
  }


    //Returns the term type ids of the function with the specified name, or
    //NULL if there is no function with specified name.
    //Caller should delete name if required.
    //Caller should not delete the returned const Array<const char*>*
  const Array<const char*>* getFunctionTermTypesAsStr(const char* const & name)
    const
  {
    StrToFuncTemplateMap::iterator it;
    if ((it=strToFuncTemplateMap_->find(name)) == strToFuncTemplateMap_->end())
      return NULL;
    return (*it).second->getTermTypesAsStr();
  }


    // Returns the FunctionTemplate with the given name 
    // Caller is responsible for deleting name if required.
    // Caller should not delete the returned FunctionTemplate* nor modify it.
  const FunctionTemplate* getFunctionTemplate(const char* const & name) const
  {
    StrToFuncTemplateMap::iterator it;
    if ((it=strToFuncTemplateMap_->find(name)) == strToFuncTemplateMap_->end())
      return NULL;
    return (*it).second;
  }

    // Returns the PredicateTemplate* with the given id or NULL if there is no
    // PredicateTemplate with the specified id.
    // Caller should not delete the returned PredicateTemplate* nor modify it.
  const FunctionTemplate* getFunctionTemplate(const int& id) const
  {
    const char* funcName = ((Domain*)this)->getFunctionName(id);
    return getFunctionTemplate(funcName);
  }
  
  
  bool isFunction(const char* const & name) const 
  {
  	return (getFunctionId(name)>=0 ||
  			strcmp(name, FunctionTemplate::SUCC_NAME) == 0 || 
  			strcmp(name, FunctionTemplate::PLUS_NAME) == 0 ||
  			strcmp(name, FunctionTemplate::MINUS_NAME) == 0 ||
  			strcmp(name, FunctionTemplate::TIMES_NAME) == 0 ||
  			strcmp(name, FunctionTemplate::DIVIDEDBY_NAME) == 0 ||
  			strcmp(name, FunctionTemplate::MOD_NAME) == 0 ||
  			strcmp(name, FunctionTemplate::CONCAT_NAME) == 0);
  }
  
  bool isFunction(const int& id) const { return (getFunctionName(id) != NULL); }


    // Caller shouldn't delete the returned function set nor modify its contents
  const FunctionSet* getFunctionMappings() const { return funcSet_; }

  
    // f is owned by Domain and caller should not delete it
    // returns true if f is added, and false if f already exists
  bool addFunctionMapping(Function* f) 
  {
    if (funcSet_->find(f) ==  funcSet_->end())
    {
      funcSet_->insert(f);
      return true;
    }
    return false;
  }

  
  int getFunctionRetConstId(Function* const & f)
  {
    FunctionSet::iterator it;
    if ((it = funcSet_->find(f)) == funcSet_->end()) return -1;
    return (*it)->getRetConstId();
  }


  void printPredicateTemplates(ostream& out) const
  {
    StrToPredTemplateMap::iterator it = strToPredTemplateMap_->begin();
    for (; it !=  strToPredTemplateMap_->end(); it++)
    {
//      if (!(*it).second->isEqualPredicateTemplate() &&
//      	  !(*it).second->isInternalPredicateTemplate() &&
//      	  !(*it).second->isPredicateTemplateFromFunction())
      if (!(*it).second->isEqualPredicateTemplate() &&
      	  !(*it).second->isInternalPredicateTemplate())
        out << *((*it).second) << endl;
    }
  }

  /**
   * print out the function declarations.
   * 
   * @out output stream to which the declarations are written
   */
  void printFunctionTemplates(ostream& out) const
  {
    StrToFuncTemplateMap::iterator it = strToFuncTemplateMap_->begin();
    for (; it !=  strToFuncTemplateMap_->end(); it++)
    {
      if (!(*it).second->isInternalFunctionTemplate())
      	out << *((*it).second) << endl;
    }
  }

 private:
  void changePredTermsToNewIds(Predicate* const & p,
                               hash_map<int,int>& oldToNewConstIds);


 private:
  DualMap* typeDualMap_;  //maps type id to name and vice versa
  ConstDualMap* constDualMap_; //maps constant id to name and vice versa
  DualMap* predDualMap_;  //maps predicate id to name and vice versa
  DualMap* funcDualMap_;  //maps func id to name and vice versa
    //maps predicate name to PredicateTemplatex*. shares key with predDualMap_
  StrToPredTemplateMap* strToPredTemplateMap_;
    //maps function name to FunctionTemplate*. shares its key with funcDualMap_
  StrToFuncTemplateMap* strToFuncTemplateMap_;

    // corresponds to the predicate EQUAL_NAME(SK_ANY, SK_ANY)
  PredicateTemplate* equalPredTemplate_;
  
    // empty binary predicate template used when infix operators are used
  PredicateTemplate* emptyPredTemplate_;
  
    // empty unary function template used when infix operators are used
  FunctionTemplate* emptyFuncUnaryTemplate_;

    // empty binary function template used when infix operators are used
  FunctionTemplate* emptyFuncBinaryTemplate_;
  
    // constantsByTypeArr_[0] is an Array<int> of constants of type 0
  Array<Array<int>*>* constantsByType_;
  Database* db_;
  TrueFalseGroundingsStore* trueFalseGroundingsStore_; //for sampling clauses
  FunctionSet* funcSet_;
};


#endif
