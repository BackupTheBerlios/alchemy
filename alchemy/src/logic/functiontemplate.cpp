#include "functiontemplate.h"
#include "domain.h"

const char* FunctionTemplate::EMPTY_FTEMPLATE_NAME = "SK_EMPTY_FTEMPLATE";

// Set id of return type.
void FunctionTemplate::setRetTypeId(const int& id, const Domain* const & domain)
{ 
  const char* name = domain->getTypeName(id);
  if (name==NULL)
  {
    cout << "Error: In FunctionTemplate::addRetTypeId(). TypeId " << id 
         << " does not exist." << endl;
    exit(-1);
  }
  
  retTypeId_ = id; 
  delete [] retTypeName_;
  retTypeName_ = new char[strlen(name)+1];
  strcpy(retTypeName_,name);
}
  

  // Set name of return type.
  // Caller is responsible for deleteing name if required
void FunctionTemplate::setRetTypeName(const char* const & name,
                                      const Domain* const & domain) 
{ 
  int id = domain->getTypeId(name);
  if (id < 0) 
  {
    cout << "Error: In FunctionTemplate::setRetTypeName(). Type " << name 
         << " has not been declared." << endl;
    exit(-1);
  }

  retTypeId_ = id;
  delete [] retTypeName_;
  retTypeName_ = new char[strlen(name)+1];
  strcpy(retTypeName_,name);
}
