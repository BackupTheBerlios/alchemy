#include "groundclause.h"
#include "groundpredicate.h"
#include "clause.h"

GroundClause::GroundClause(const Clause* const & c) 
  : numSatLiterals_(NULL), wt_(c->getWt()), parentWtPtrs_(NULL), foClauseFrequencies_(NULL)
{
  int numPreds = c->getNumPredicates();
  intArrRep_ = new Array<unsigned int>;
  gndPreds_ = new Array<GroundPredicate*>(numPreds);
  gndPreds_->growToSize(numPreds);
  gndPredSenses_ = new Array<bool>(numPreds);
  gndPredSenses_->growToSize(numPreds);
  gndPredIndexes_ = new Array<int>(numPreds);
  gndPredIndexes_->growToSize(numPreds);
  
    // for each predicate in clause c
  for (int i = 0; i < numPreds; i++)
  {
      // add the predicate's intArrRep to GroundClause's intArrRep_
    Predicate* pred = c->getPredicate(i);
    bool predSense = pred->getSense();
    GroundPredicate* gndPred = new GroundPredicate(pred);
    if (predSense) intArrRep_->append(1);
    else           intArrRep_->append((unsigned int) 0);
    gndPred->appendIntArrRep(*intArrRep_);
    (*gndPreds_)[i] = gndPred;
    (*gndPredSenses_)[i] = predSense;
    (*gndPredIndexes_)[i] = -1; //set this later
  }

  unsigned int hashCode = Hash::hash(*intArrRep_);
  intArrRep_->append(hashCode);
  intArrRep_->compress();

  for (int i = 0; i < numPreds; i++)
  {
    bool ok = (*gndPreds_)[i]->appendGndClause(this, (*gndPredSenses_)[i]);
    assert(ok); ok = true;  // avoid compilation warning
  }
}


void GroundClause::deleteAllGndPreds()
{
  for (int i = 0; i < gndPreds_->size(); i++)
    delete (*gndPreds_)[i];
}


void GroundClause::replaceGroundPredicate(const int& i, 
                                          GroundPredicate* const & gndPred)
{
  GroundPredicate* old = (*gndPreds_)[i];
  (*gndPreds_)[i] = gndPred;
  delete old;
}


  //the GroundPredicates that make up this GroundClause must already have 
  //their truth values set
void GroundClause::initNumSatLiterals(const int& numChains)
{ 
  if(numSatLiterals_) delete numSatLiterals_;
  numSatLiterals_ = new Array<int>;
  numSatLiterals_->growToSize(numChains, 0);
  assert(gndPreds_->size() == gndPredSenses_->size());
  for (int c = 0; c < numChains; c++)
    for (int i = 0; i < gndPreds_->size(); i++)
    {
      if( (*gndPreds_)[i]->getTruthValue(c) == (*gndPredSenses_)[i] )
      {
        (*numSatLiterals_)[c]++;
        assert((*numSatLiterals_)[c] <= gndPreds_->size());
      }
      
    }
}


void GroundClause::printWithoutWt(ostream& out, 
                                  const Domain* const & domain) const
{
  for (int i = 0; i < gndPreds_->size(); i++)
  {
    if (!(*gndPredSenses_)[i]) out << "!";
    (*gndPreds_)[i]->print(out,domain);
    if (i < gndPreds_->size()-1) out << " v ";
  }
}


void GroundClause::printWithoutWt(ostream& out) const
{
  for (int i = 0; i < gndPreds_->size(); i++)
  {
    if (!(*gndPredSenses_)[i]) out << "!";
    (*gndPreds_)[i]->print(out);
    if (i < gndPreds_->size()-1) out << " v ";
  }
}


void GroundClause::print(ostream& out, const Domain* const & domain) const
{ out << wt_ << " "; printWithoutWt(out,domain); }


void GroundClause::print(ostream& out) const
{ out << wt_ << " "; printWithoutWt(out); }


/**
 * Computes and returns the size of this ground clause.
 */
double GroundClause::sizeKB()
{
  double size = 0;
    //intArrRep_
  if (intArrRep_)
    size += (intArrRep_->size()*sizeof(unsigned int) / 1024.0);
    // gndPreds_
  //if (gndPreds_)
  //{
  //  for (int i = 0; i < gndPreds_->size(); i++)
  //    size += (((*gndPreds_)[i])->sizeKB());
  //}
    // gndPredSenses_
  if (gndPredSenses_)
    size += (gndPredSenses_->size()*sizeof(bool) / 1024.0);
    // gndPredIndexes_
  if (gndPredIndexes_)
    size += (gndPredIndexes_->size()*sizeof(int) / 1024.0);
    // numSatLiterals_
  if (numSatLiterals_)
    size += (numSatLiterals_->size()*sizeof(int) / 1024.0);
    // wt_
  size += (sizeof(double) / 1024.0);
    // threshold_
  size += (sizeof(double) / 1024.0);
    // parentWtPtrs_
  if (parentWtPtrs_)
    size += (parentWtPtrs_->size()*sizeof(const double*) / 1024.0);
    // foClauseFrequencies_
  if (foClauseFrequencies_)
    size += (foClauseFrequencies_->size()*2*sizeof(int) / 1024.0);
  
  return size;
}

