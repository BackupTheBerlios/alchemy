#ifndef PREDICATE_H_JUN_26_2005
#define PREDICATE_H_JUN_26_2005

#include <ext/hash_set>
using namespace __gnu_cxx;
#include "predicatetemplate.h"
#include "term.h"
#include "hash.h"
#include "hasharray.h"
#include "function.h"

const int MAX_VAR = 10;
enum TruthValue { FALSE=0, TRUE=1, UNKNOWN=2 };

struct VarsTypeId { Array<Term*> vars; int typeId; }; 

class Clause;
class GroundPredicate;
class Domain;

  //Ensure that the dirty_ bit is consistently updated.
class Predicate
{
 public:
  Predicate(const PredicateTemplate* const & pt) 
    : template_(pt), terms_(new Array<Term*>), sense_(true), 
      truthValue_(UNKNOWN), allTermsAreDiffVars_(false), isGrounded_(false), 
      intArrRep_(NULL), hashCode_(0), dirty_(true), parent_(NULL) {}

  Predicate(const PredicateTemplate* const & pt, Clause* const & parent)
    : template_(pt), terms_(new Array<Term*>), sense_(true), 
      truthValue_(UNKNOWN), allTermsAreDiffVars_(false), isGrounded_(false), 
      intArrRep_(NULL), hashCode_(0), dirty_(true), parent_(parent) {}

  Predicate(const Predicate& p)  { parent_ = NULL; copy(p); }
  Predicate(const Predicate& p,  Clause* const & par) { parent_= par; copy(p); }

  
  ~Predicate()
  {
    for (int i = 0; i < terms_->size(); i++)  delete (*terms_)[i];
    delete terms_;
    if (intArrRep_) delete intArrRep_;
  }


  double sizeMB() const
  {
    double sizeMB = (fixedSizeB_ + intArrRep_->size()*sizeof(int) +
                     terms_->size()*sizeof(Term*))/1000000.0;
    for (int i = 0; i < terms_->size(); i++) sizeMB += (*terms_)[i]->sizeMB();
    return sizeMB;
  }


  static void computeFixedSizeB()
  { fixedSizeB_ = (sizeof(Predicate)+sizeof(Array<Term*>)+sizeof(Array<int>)); }


  void compress() 
  { 
    for (int i = 0; i < terms_->size(); i++)  (*terms_)[i]->compress();
    terms_->compress(); 
    intArrRep_->compress(); 
  }


    //returns true if this predicate represents "="
  bool isEqualPred() const  { return template_->isEqualPred(); }
  
  bool isEqualPredWithType() const  { return template_->isEqualPredWithType(); }

    //returns true if this predicate is empty predicate
  bool isEmptyPred() const  { return template_->isEmptyPredicateTemplate(); }
 
     //returns true if this predicate is an internal predicate
  bool isInternalPred() const  { return template_->isInternalPredicateTemplate(); }
  
  bool isInternalPredWithoutType() const
  { return template_->isInternalPredicateTemplateWithoutType(); }

  void canonicalize()
  {
    Array<int> varIdToNewVarId;
    int newVarId = 0;
    for (int i = 0; i < terms_->size(); i++)
    {
      if ((*terms_)[i]->getType() == Term::VARIABLE)
      {
        int id = -((*terms_)[i]->getId());
        assert(id > 0);
        if (id >= varIdToNewVarId.size()) varIdToNewVarId.growToSize(id+1,0);
          //if a new var id has not been assigned to old var id
        if (varIdToNewVarId[id] >= 0) varIdToNewVarId[id] = --newVarId;
        (*terms_)[i]->setId(varIdToNewVarId[id]);
      }
      assert((*terms_)[i]->getType() != Term::FUNCTION);
    }
  }


    //term is owned by Predicate. Caller should not delete it
  void appendTerm(Term* const & term)
  {
    if (template_->getNumTerms() == terms_->size()) 
    {
      cout << "Error: In Predicate::appendTerm. Tried to add more terms than "
           << "the declared num of " << template_->getNumTerms() << endl;
      exit(-1);
    }
    terms_->append(term);
    setDirty();
  }

  
  bool getSense() const { return sense_; }
  void setSense(const bool& s) { sense_ = s; setDirty();}
  void invertSense() { sense_ = (sense_) ? false : true; setDirty(); }

    //get and set the definitive truth value of the predicate
  TruthValue getTruthValue() const { return truthValue_; }
  void setTruthValue(const TruthValue& tv) { truthValue_ = tv; }
  string getTruthValueAsStr() const 
  {
    if (truthValue_ == TRUE)    return "TRUE";
    if (truthValue_ == FALSE)   return "FALSE";
    if (truthValue_ == UNKNOWN) return "UNKNOWN";
    assert(false); 
    return "UNKNOWN"; //avoid compilation warning
  }


  double getNumGroundingsIfAllVarDiff(const Domain* const & domain) const;
 
  
    //Caller is responsible for deleting the Predicate* in returnArray
  static void createAllGroundings(const int& predId, 
                                  const Domain* const & domain,
                                  Array<Predicate*>& returnArray);
    
  
  //Caller is responsible for deleting the Predicate* in returnArray
  static void createAllGroundingsUnifyingWithTerm(const int& predId, 
                                  const Domain* const & domain,
                                  Array<Predicate*>& returnArray,
								  int termTypeId,int termVal);


  void createAllGroundingsIfAllVarDiff(const Domain* const & domain,
                                       Array<Predicate*>& returnArray)
  { createAllGroundings(getId(), domain, returnArray); }


    //Create groundings of predicate, taking into account vars may be shared.
    //Exactly one of  predReturnArray or constReturnArray must be NULL.
    //Callers are responsible for deleting the parameters and their contents.
  void createAllGroundings(const Domain* const & domain,
                           Array<Predicate*>* const & predReturnArray,
                           Array<int*>* const & constReturnArray);

    //create groundings of predicate, taking into account vars may be shared
  void createAllGroundings(const Domain* const & domain,
                           Array<Predicate*>& returnArray)
  { createAllGroundings(domain, &returnArray, NULL); }


    //create groundings of predicate, taking into account vars may be shared
  void createAllGroundings(const Domain* const & domain,
                           Array<int*>& returnArray)
  { createAllGroundings(domain, NULL, &returnArray); }


  int getNumTerms() const { return terms_->size(); }

  const Term* getTerm(const int& idx) const { return (*terms_)[idx]; }


  void setTermToConstant(const int& termNum, const int& constId)
  {
    assert(termNum < template_->getNumTerms());
    assert(constId >= 0);
    if (termNum >= terms_->size()) terms_->growToSize(termNum+1,NULL);
    if ((*terms_)[termNum]) delete (*terms_)[termNum];
    (*terms_)[termNum] = new Term(constId, (void*)this, true);
    setDirty();
  }

  
  bool containsConstant(const int& constId) const
  {
    assert(constId >= 0);
    for (int i = 0; i < terms_->size(); i++)
    {
      Term* t = (*terms_)[i];
      if (t->getType() == Term::CONSTANT && t->getId() == constId) return true;
    }
    return false;
  }


  void setTemplate(PredicateTemplate* const & t) { template_ = t; }

  const PredicateTemplate* getTemplate() const { return template_; }


    // Caller should not delete the returned const char* 
  const char* getName() const { return template_->getName(); }

  int getId() const { return template_->getId(); }
  

    // Caller should not delete the returned const char* nor modify its
    // contents. Returns NULL if idx is larger than the possible number of 
    // terms
  const char* getTermTypeAsStr(const int& idx) const 
  {
    if (idx >= template_->getNumTerms()) return NULL;
    return template_->getTermTypeAsStr(idx); 
  }

    // Returns -1 if idx is larger than the possible number of terms
  int getTermTypeAsInt(const int& idx) const 
  {
    if (idx >= template_->getNumTerms()) return -1;
    return template_->getTermTypeAsInt(idx); 
  }


  bool allTermsAreDiffVars()
  {
    if (dirty_) computeAndStoreIntArrRep();
    return allTermsAreDiffVars_;
  }
  

  bool checkAllTermsAreDiffVars()
  {
    Array<int> vars;
    allTermsAreDiffVars_ = true;
    for (int i = 0; i < terms_->size(); i++)
    {
      if ((*terms_)[i]->getType() == Term::VARIABLE)
      {
        int tid = (*terms_)[i]->getId();
        if (vars.contains(tid)) { allTermsAreDiffVars_ = false; return false;}
        else                    vars.append(tid);
      }
      else
      { 
        assert((*terms_)[i]->getType() != Term::FUNCTION);
          //is a constant
        allTermsAreDiffVars_ = false;
        return false;
      }
    }
    return true;
  }


  bool isGrounded()
  {
    if (dirty_) computeAndStoreIntArrRep();
    return isGrounded_;    
  }

  
  bool canBeGroundedAs(Predicate* const & gndPred)
  {
    assert(gndPred->isGrounded());
    if (template_->getId() != gndPred->getId()) return false;
    if (allTermsAreDiffVars()) return true;
    if (isGrounded()) return same(gndPred);
    
    int varGndings[MAX_VAR];
    memset(varGndings, -1, MAX_VAR*sizeof(int));
    for (int i = 0; i < terms_->size(); i++) 
    {
      int termType = (*terms_)[i]->getType();
      
      if (termType == Term::CONSTANT) 
      {
        if ((*terms_)[i]->getId() != gndPred->getTerm(i)->getId()) 
          return false;
      }
      else 
      if (termType == Term::VARIABLE) 
      {
        int varId = -(*terms_)[i]->getId();
        assert(varId > 0);
        assert(varId < MAX_VAR);
        if (varGndings[varId] < 0) // if variable has not been grounded
          varGndings[varId] = gndPred->getTerm(i)->getId();
        else 
          if (varGndings[varId] != gndPred->getTerm(i)->getId())
            return false;
      }
      else
      {
        assert(false);
      }
    }
    return true;
  }


  bool canBeGroundedAs(const GroundPredicate* const & gndPred);


  void setDirty();
  bool isDirty() const { return dirty_; }

  void setParent(Clause* const & parent) { parent_ = parent; setDirty(); }
  Clause* getParent() const { return parent_; }

	// True, if the predicate can be used in the inverted index
  bool isIndexable()
  {
	  // At most one of the two terms is grounded
	  // and equality pred is never indexed
	if (isGrounded() || isEqualPred()) return false;
  	return (!getSense() && getNumTerms() == 2);	
  }

  void createVarsTypeIdArr(Array<VarsTypeId*>*& varsTypeIdArr)
  {
    if (varsTypeIdArr == NULL) varsTypeIdArr = new Array<VarsTypeId*>;

      //for each variable of the predicate
    for (int j = 0; j < terms_->size(); j++)
    {
      Term* t = (*terms_)[j];
      if (t->getType() == Term::VARIABLE)
      {
        int id = -(t->getId());
        assert(id > 0);
        if (id >= varsTypeIdArr->size()) varsTypeIdArr->growToSize(id+1,NULL);
        VarsTypeId*& vti = (*varsTypeIdArr)[id];
        if (vti == NULL) 
        {
          vti = new VarsTypeId;
          vti->typeId = getTermTypeAsInt(j);
          assert(vti->typeId >= 0);
        }
        assert(getTermTypeAsInt(j) == vti->typeId);
        vti->vars.append(t);
      }
      assert(t->getType() != Term::FUNCTION);
    }// for each variable of the predicate
  }


  void deleteVarsTypeIdArr(Array<VarsTypeId*>*& varsTypeIdArr)
  {
    for (int i = 0; i < varsTypeIdArr->size(); i++)
      if ((*varsTypeIdArr)[i]) delete (*varsTypeIdArr)[i];
    delete varsTypeIdArr;
    varsTypeIdArr = NULL; 
  }


    //Does not consider the sense of the predicate.
  bool same(Predicate* const & p)
  {
    if (this == p)  return true;
    const Array<int>* pArr  = p->getIntArrRep();
    const Array<int>* myArr = getIntArrRep();
    if (myArr->size() != pArr->size()) return false;
    const int* pItems  = p->getIntArrRep()->getItems();
    const int* myItems = getIntArrRep()->getItems();
    return (memcmp(myItems, pItems, myArr->size()*sizeof(int))==0);
  }


  bool same(const GroundPredicate* const & gp);

  
    // represent function as an Array<int> and append it to rep
  void appendIntArrRep(Array<int>& rep) 
  {
    if (dirty_) computeAndStoreIntArrRep();
    rep.append(*intArrRep_);
  }


  size_t hashCode()
  {
    if (dirty_) computeAndStoreIntArrRep();
    return hashCode_;
  }


  ostream& printAsInt(ostream& out) const
  {
    if(isEqualPred()) return printAsIntEqualPred(out);
    if(isInternalPred()) return printAsIntInternalPred(out);

    if (!sense_) out << "!";
    out << template_->getId() << "(";
    for (int i = 0; i < terms_->size(); i++)
    {
      (*terms_)[i]->printAsInt(out); 
      out << ((i!=terms_->size()-1)?",":")");
    }
    return out;
  }


  ostream& printWithStrVar(ostream& out, const Domain* const & domain) const
  {
    if (isEqualPred()) return printEqualPredWithStrVar(out,domain);
    if (isInternalPred()) return printInternalPredWithStrVar(out,domain);

    if (!sense_) out << "!";
    out << template_->getName() << "(";
    for (int i = 0; i < terms_->size(); i++)
    {
      (*terms_)[i]->printWithStrVar(out, domain); 
      out << ((i!=terms_->size()-1)?",":")");
    }
    return out;
  }


  ostream& print(ostream& out, const Domain* const & domain) const
  {
    if (isEqualPred()) return printEqualPred(out, domain);
    if (isInternalPred()) return printInternalPred(out, domain);

    if (!sense_) out << "!";
    out << template_->getName() << "(";
    for (int i = 0; i < terms_->size(); i++)
    {
      (*terms_)[i]->print(out, domain); 
      out << ((i!=terms_->size()-1)?",":")");
    }
    return out;
  }


 private:
  void copy(const Predicate& p)
  {
    template_ = p.template_;

    terms_ = new Array<Term*>;
    Array<Term*>* tterms = p.terms_;
    for (int i = 0; i < tterms->size(); i++)
    {
      Term* t = (*tterms)[i];
      terms_->append(new Term(*t, (void*)this, true));      
    }
    dirty_ = p.dirty_;

    if (!dirty_) { assert(noDirtyTerms()); }

    sense_ = p.sense_;
    truthValue_ = p.truthValue_;
    allTermsAreDiffVars_ = p.allTermsAreDiffVars_;
    isGrounded_ = p.isGrounded_;
    
    if (p.intArrRep_)  intArrRep_ = new Array<int>(*(p.intArrRep_));
    else               intArrRep_ = NULL;    

    hashCode_ = p.hashCode_;

    setParentDirty();
  }


  void setParentDirty();

  bool noDirtyTerms()
  {
    for (int i = 0; i < terms_->size(); i++)
      if ((*terms_)[i]->isDirty()) return false;
    return true;
  }


  const Array<int>* getIntArrRep() 
  { if (dirty_) computeAndStoreIntArrRep(); return intArrRep_; }


    //Also finds out whether all of its terms are variables/constants.
  void computeAndStoreIntArrRep()
  {
    dirty_ = false;
    if (intArrRep_ == NULL) intArrRep_ = new Array<int>;
    else                    intArrRep_->clear();
      //commented out: predicate comparison ignores sense
    //intArrRep_->append(sense_?1:0);
    intArrRep_->append(template_->getId());
    int numTerms = terms_->size();
    Array<int> vars(numTerms);
    allTermsAreDiffVars_ = true;
    isGrounded_ = true;
    for (int i = 0; i < numTerms; i++)
    {
      (*terms_)[i]->appendIntArrRep(*intArrRep_);
      int termType = (*terms_)[i]->getType();
      
      if (termType == Term::VARIABLE)
      {
        if (allTermsAreDiffVars_)
        {
          int tid = (*terms_)[i]->getId();
          if (vars.contains(tid)) allTermsAreDiffVars_ = false;
          else                    vars.append(tid);
        }
        isGrounded_ = false;
      }
      else
      if (termType == Term::CONSTANT)
      {
        allTermsAreDiffVars_ = false;
        if ((*terms_)[i]->getType() != Term::CONSTANT) isGrounded_ = false;
      }
      else
      {
        assert(false);
      }
    }
    hashCode_ = Hash::hash(*intArrRep_);
  }


  ostream& printAsIntEqualPred(ostream& out) const
  {
    assert(isEqualPred());
    assert(terms_->size()==2);
    if (!sense_) out << "!(";
    (*terms_)[0]->printAsInt(out); 
    out << " = ";
    (*terms_)[1]->printAsInt(out); 
    if (!sense_) out << ")";
    return out;
  }


  ostream& printEqualPred(ostream& out, const Domain* const & domain) const
  {
    assert(isEqualPred());
    assert(terms_->size()==2);
    if (!sense_) out << "!(";
    (*terms_)[0]->print(out, domain); 
    out << " = ";
    (*terms_)[1]->print(out, domain); 
    if (!sense_) out << ")";
    return out;
  }


  ostream& 
  printEqualPredWithStrVar(ostream& out, const Domain* const & domain) const
  {
    assert(isEqualPred());
    assert(terms_->size()==2);
    if (!sense_) out << "!(";
    (*terms_)[0]->printWithStrVar(out, domain); 
    out << " = ";
    (*terms_)[1]->printWithStrVar(out, domain); 
    if (!sense_) out << ")";
    return out;
  }
    
    
  ostream& printAsIntInternalPred(ostream& out) const
  {
    assert(isInternalPred());
    assert(terms_->size()==2);
    
      //No infix
	if (strncmp(getName(), PredicateTemplate::SUBSTR_NAME,
				strlen(PredicateTemplate::SUBSTR_NAME)) == 0)
	{
	  if (!sense_) out << "!";
      out << template_->getId() << "(";
      for (int i = 0; i < terms_->size(); i++)
      {
      	(*terms_)[i]->printAsInt(out); 
        out << ((i!=terms_->size()-1)?",":")");
      }
      return out; 
	}
	else // Infix
	{
      if (!sense_) out << "!(";
      (*terms_)[0]->printAsInt(out); 
	  
	  if (strncmp(getName(), PredicateTemplate::GT_NAME,
				  strlen(PredicateTemplate::GT_NAME)) == 0)
      	out << " > ";
      else
	  if (strncmp(getName(), PredicateTemplate::LT_NAME,
				  strlen(PredicateTemplate::LT_NAME)) == 0)
      	out << " < ";
      else
      if (strncmp(getName(), PredicateTemplate::GTEQ_NAME,
				  strlen(PredicateTemplate::GTEQ_NAME)) == 0)
      	out << " >= ";
      else
	  if (strncmp(getName(), PredicateTemplate::LTEQ_NAME,
				  strlen(PredicateTemplate::LTEQ_NAME)) == 0)
      	out << " <= ";
      
      (*terms_)[1]->printAsInt(out); 
      if (!sense_) out << ")";
      return out;
	}
  }


  ostream& printInternalPred(ostream& out, const Domain* const & domain) const
  {
    assert(isInternalPred());
    assert(terms_->size()==2);

	  //No infix
	if (strncmp(getName(), PredicateTemplate::SUBSTR_NAME,
				strlen(PredicateTemplate::SUBSTR_NAME)) == 0)
	{
      if (!sense_) out << "!";
      out << PredicateTemplate::SUBSTR_NAME << "(";
      for (int i = 0; i < terms_->size(); i++)
      {
        (*terms_)[i]->print(out, domain); 
        out << ((i!=terms_->size()-1)?",":")");
      }
      return out;
	}
	else // Infix
	{
	  if (!sense_) out << "!(";
      (*terms_)[0]->print(out, domain); 
	  
	  if (strncmp(getName(), PredicateTemplate::GT_NAME,
				  strlen(PredicateTemplate::GT_NAME)) == 0)
      	out << " > ";
      else
	  if (strncmp(getName(), PredicateTemplate::LT_NAME,
				  strlen(PredicateTemplate::LT_NAME)) == 0)
      	out << " < ";
      else
      if (strncmp(getName(), PredicateTemplate::GTEQ_NAME,
				  strlen(PredicateTemplate::GTEQ_NAME)) == 0)
      	out << " >= ";
      else
	  if (strncmp(getName(), PredicateTemplate::LTEQ_NAME,
				  strlen(PredicateTemplate::LTEQ_NAME)) == 0)
      	out << " <= ";
      
      (*terms_)[1]->print(out, domain); 
      if (!sense_) out << ")";
      return out;
	}
  }


  ostream& 
  printInternalPredWithStrVar(ostream& out, const Domain* const & domain) const
  {
    assert(isInternalPred());
    assert(terms_->size()==2);

	  //No infix
	if (strncmp(getName(), PredicateTemplate::SUBSTR_NAME,
				strlen(PredicateTemplate::SUBSTR_NAME)) == 0)
	{
      if (!sense_) out << "!";
      out << PredicateTemplate::SUBSTR_NAME << "(";
      for (int i = 0; i < terms_->size(); i++)
      {
        (*terms_)[i]->printWithStrVar(out, domain); 
        out << ((i!=terms_->size()-1)?",":")");
      }
      return out;
	}
	else // Infix
	{
      if (!sense_) out << "!(";
      (*terms_)[0]->printWithStrVar(out, domain);
	  
	  if (strncmp(getName(), PredicateTemplate::GT_NAME,
				  strlen(PredicateTemplate::GT_NAME)) == 0)
      	out << " > ";
      else
	  if (strncmp(getName(), PredicateTemplate::LT_NAME,
				  strlen(PredicateTemplate::LT_NAME)) == 0)
      	out << " < ";
      else
      if (strncmp(getName(), PredicateTemplate::GTEQ_NAME,
				  strlen(PredicateTemplate::GTEQ_NAME)) == 0)
      	out << " >= ";
      else
	  if (strncmp(getName(), PredicateTemplate::LTEQ_NAME,
				  strlen(PredicateTemplate::LTEQ_NAME)) == 0)
      	out << " <= ";
      
      (*terms_)[1]->printWithStrVar(out, domain); 
      if (!sense_) out << ")";
      return out;
	}
  }
  
   
 private:
  const PredicateTemplate* template_; // not owned by Predicate
  Array<Term*>* terms_;
  bool sense_; // indicate whether the predicate is non-negated/negated
  TruthValue truthValue_;

  bool allTermsAreDiffVars_; // all of its terms are different variables
  bool isGrounded_;

  Array<int>* intArrRep_;
  size_t hashCode_;
  bool dirty_;
  Clause* parent_; //not owned by Predicate, so no deleted in destructor

  static double fixedSizeB_;
};

inline
ostream& operator<<(ostream& out, const Predicate& p){return p.printAsInt(out);}


////////////////////////////////// hash /////////////////////////////////

class HashPredicate
{
 public:
  size_t operator()(Predicate* const & p) const  { return p->hashCode(); }
};


class EqualPredicate
{
 public:
  bool operator()(Predicate* const & p1, Predicate* const & p2) 
    const { return p1->same(p2); }
};

/////////////////////////////// containers  /////////////////////////////////

typedef hash_set<Predicate*, HashPredicate, EqualPredicate> PredicateSet;

typedef HashArray<Predicate*, HashPredicate, EqualPredicate> PredicateHashArray;

#endif
