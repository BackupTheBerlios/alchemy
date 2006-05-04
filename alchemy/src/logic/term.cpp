#include "term.h"
#include "function.h"
#include "domain.h"

double Term::fixedSizeB_ = -1;


Term::Term(const Term& t)
{
  parent_ = NULL;
  parentIsPred_ = true;
  copy(t);
}


Term::Term(const Term& t, void* const & parent, const bool& parentIsPred)
{
  parent_ = parent;
  parentIsPred_ = parentIsPred;
  copy(t);
}


Term::~Term() 
{ 
  if (function_) delete function_; 
  if (intArrRep_) delete intArrRep_; 
}


void Term::copy(const Term& t)
{
  id_ = t.id_;

  if (t.function_)  function_ = new Function(*(t.function_), this);
  else              function_ = NULL;

  if (t.intArrRep_)  intArrRep_ = new Array<int>(*(t.intArrRep_));
  else               intArrRep_ = NULL;  
  
  dirty_ = t.dirty_;

  if (!dirty_) { assert(noDirtyFunction()); }

  if (parent_ != NULL)
  {
    if (parentIsPred_) ((Predicate*)parent_)->setDirty();
    else               ((Function*)parent_)->setDirty();
  }
}


bool Term::noDirtyFunction()
{
  if (function_ == NULL) return true;
  return !(function_->isDirty());
}


void Term::setDirty() 
{ 
  dirty_ = true;
  if (parent_ != NULL)
  {
    if (parentIsPred_) ((Predicate*)parent_)->setDirty();
    else               ((Function*)parent_)->setDirty();
  }
}


void Term::computeAndStoreIntArrRep()
{
  dirty_ = false;

  if (intArrRep_ == NULL) intArrRep_ = new Array<int>;
  else                    intArrRep_->clear();
  
  if (function_ == NULL)
    intArrRep_->append(id_);
  else
  {
    if (id_ >= 0) intArrRep_->append(id_);
    else          function_->appendIntArrRep(*intArrRep_);
  }
}


void Term::printAsInt(ostream& out) const
{
  if (function_ == NULL) 
    out << id_;
  else 
  { //function_ != NULL;
    if (id_ >= 0) out << id_;
    else          function_->printAsInt(out);
  }
}


void Term::printWithStrVar(ostream& out, const Domain* const & domain) const
{
  if (function_ == NULL)
  {
    if (id_ < 0) 
      out << "a" << -id_;  // variable
    else
    {
      string cn = domain->getConstantName(id_);
      string::size_type at = cn.rfind("@");
      if (at != string::npos) cn = cn.substr(at+1, cn.length()-at-1);
      out << cn; // constant
    }
  }
  else
  { //function_ != NULL;
    if (id_ >= 0) 
    {
      string cn = domain->getConstantName(id_);
      string::size_type at = cn.rfind("@");
      if (at != string::npos) cn = cn.substr(at+1, cn.length()-at-1);
      out << cn; // return value is known
    }
    else          
      function_->print(out,domain); 
  }
}


void Term::print(ostream& out, const Domain* const & domain) const
{
  if (function_ == NULL)
  {
    if (id_ < 0) out << id_;  // variable
    else         
    {
      string cn = domain->getConstantName(id_);
      string::size_type at = cn.rfind("@");
      if (at != string::npos) cn = cn.substr(at+1, cn.length()-at-1);
      out << cn; // constant
    }
  }
  else
  { //function_ != NULL;
    if (id_ >= 0) 
    {
      string cn = domain->getConstantName(id_);
      string::size_type at = cn.rfind("@");
      if (at != string::npos) cn = cn.substr(at+1, cn.length()-at-1);
      out << cn; // return value is known
    }
    else          
      function_->print(out, domain); 
  }
}
