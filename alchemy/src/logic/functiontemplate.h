#ifndef FUNCTIONTEMPLATE_JUN_22_2005
#define FUNCTIONTEMPLATE_JUN_22_2005

#include "predicatetemplate.h"

class FunctionTemplate : public PredicateTemplate
{
 public:
  static const char* EMPTY_FTEMPLATE_NAME; // name of empty template
 public:
  FunctionTemplate() : PredicateTemplate(), retTypeId_(-1),retTypeName_(NULL) {}
  virtual ~FunctionTemplate() { delete [] retTypeName_; }


    // Sets name and id of return type.
  void setRetTypeId(const int& id, const Domain* const & domain);

    // Sets name and id of return type.
    // Caller is responsible for deleteing name if required
  void setRetTypeName(const char* const & name, const Domain* const & domain);

  
  int getRetTypeId() const { return retTypeId_; }
  
    // Caller should not delete the returned const char*
  const char* getRetTypeName() const { return retTypeName_; }

    // returns funcName append with _typeNames
  static string createInternalFuncTypeName(const char* const & funcName,
  										   const Array<string>& typeNames)
  {
    string ftName;
    ftName.append(funcName);
    for (int i = 0; i < typeNames.size(); i++)
    {
	  ftName.append("_").append(typeNames[i]);
    }
    return ftName;
  }
  
  static bool isInternalFunctionTemplateName(const char* funcName)
  { 
  	return (strncmp(funcName, SUCC_NAME, strlen(SUCC_NAME)) == 0 || 
  			strncmp(funcName, PLUS_NAME, strlen(PLUS_NAME)) == 0 ||
  			strncmp(funcName, MINUS_NAME, strlen(MINUS_NAME)) == 0 ||
  			strncmp(funcName, TIMES_NAME, strlen(TIMES_NAME)) == 0 ||
  			strncmp(funcName, DIVIDEDBY_NAME, strlen(DIVIDEDBY_NAME)) == 0 ||
  			strncmp(funcName, MOD_NAME, strlen(MOD_NAME)) == 0 ||
  			strncmp(funcName, CONCAT_NAME, strlen(CONCAT_NAME)) == 0);
  }

  static bool isInternalFunctionUnaryTemplateName(const char* funcName)
  { 
  	return (strncmp(funcName, SUCC_NAME, strlen(SUCC_NAME)) == 0);
  }


  bool isInternalFunctionTemplate() const 
  { 
  	return (strncmp(name_, SUCC_NAME, strlen(SUCC_NAME)) == 0 || 
  			strncmp(name_, PLUS_NAME, strlen(PLUS_NAME)) == 0 ||
  			strncmp(name_, MINUS_NAME, strlen(MINUS_NAME)) == 0 ||
  			strncmp(name_, TIMES_NAME, strlen(TIMES_NAME)) == 0 ||
  			strncmp(name_, DIVIDEDBY_NAME, strlen(DIVIDEDBY_NAME)) == 0 ||
  			strncmp(name_, MOD_NAME, strlen(MOD_NAME)) == 0 ||
  			strncmp(name_, CONCAT_NAME, strlen(CONCAT_NAME)) == 0);
  }

  ostream& print(ostream& out) const
  {
  	out << retTypeName_ << " ";
    out << name_ << "(";
    for (int i = 0; i < termTypesAsStr_->size(); i++)
    {
      out << (*termTypesAsStr_)[i]; 
      out << ((i!=termTypesAsStr_->size()-1)?",":")");
    }
    return out;
  }


  ostream& printWithStrVar(ostream& out) const
  {
  	out << retTypeName_ << " ";
    out << name_ << "(";
    for (int i = 0; i < termTypesAsStr_->size(); i++)
    {
      out << "a" << i+1; 
      out << ((i!=termTypesAsStr_->size()-1)?",":")");
    }
    return out;
  }
  
  
 private:
  int retTypeId_;
  char* retTypeName_;
};

#endif
