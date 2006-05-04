#include "predicatetemplate.h"
#include "domain.h"

const char* PredicateTemplate::EMPTY_NAME = "SK_EMPTY";
const char* PredicateTemplate::EQUAL_NAME = "SK_EQUAL";
const char* PredicateTemplate::GT_NAME = "greaterThan";
const char* PredicateTemplate::LT_NAME = "lessThan";
const char* PredicateTemplate::GTEQ_NAME = "greaterThanEq";
const char* PredicateTemplate::LTEQ_NAME = "lessThanEq";
const char* PredicateTemplate::SUBSTR_NAME = "substr";
const char* PredicateTemplate::ANY_TYPE_NAME = "SK_ANY";
const char* PredicateTemplate::INT_TYPE_NAME = "int";
const char* PredicateTemplate::STRING_TYPE_NAME = "string";
const char* PredicateTemplate::ZZ_RETURN_PREFIX = "isReturnValueOf";

const char* PredicateTemplate::SUCC_NAME = "succ";
const char* PredicateTemplate::PLUS_NAME = "plus";
const char* PredicateTemplate::MINUS_NAME = "minus";
const char* PredicateTemplate::TIMES_NAME = "times";
const char* PredicateTemplate::DIVIDEDBY_NAME = "dividedBy";
const char* PredicateTemplate::MOD_NAME = "mod";
const char* PredicateTemplate::CONCAT_NAME = "concat";

  // Caller is responsible for deleting typeName if required.
  // returns true if termType is successfully added; false otherwise
  // isUnique is false by default 
bool PredicateTemplate::appendTermType(const char* const & typeName,
                                       const bool& isUnique,
                                       const Domain* const & domain) 
{
  int id = domain->getTypeId(typeName);
  if (id < 0) 
  {
    cout << "Warning: In PredicateTemplate::addTermType(). Type " << typeName 
         << " has not been declared." << endl;
    return false;
  }
  
  char* tname = new char[strlen(typeName)+1]; 
  strcpy(tname, typeName);
  termTypesAsStr_->append(tname);
  termTypesAsInt_->append(id);
  termsUnique_->append(isUnique);
  assert(termTypesAsStr_->size() == termTypesAsInt_->size());
  return true;
}


  // returns true if termType is successfully added; false otherwise
  // isUnique is false by default 
bool PredicateTemplate::appendTermType(const int& typeId, const bool& isUnique,
                                       const Domain* const & domain)
{
  const char* typeName = domain->getTypeName(typeId);
  if (typeName==NULL) 
  {
    cout << "Error: In PredicateTemplate::addTermType(). TypeId " << typeId 
         << " does not exist." << endl;
    return false;
  }
  
  char* tname = new char[strlen(typeName)+1]; 
  strcpy(tname, typeName);
  termTypesAsStr_->append(tname);
  termTypesAsInt_->append(typeId);
  termsUnique_->append(isUnique);
  assert(termTypesAsStr_->size() == termTypesAsInt_->size());
  return true;
}


