#include "function.h"
#include "domain.h"


void Function::setDirty() {dirty_=true; if (parent_!=NULL) parent_->setDirty();}


ostream& 
Function::printWithRetConstName(ostream& out,const Domain* const & domain) const
{
  print(out, domain);
  out << "=";
  if (retConstId_ >= 0) 
    out << domain->getConstantName(retConstId_);
  else                
    out << "?";
  return out;
}
