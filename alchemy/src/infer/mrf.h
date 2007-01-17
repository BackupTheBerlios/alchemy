/*
 * All of the documentation and software included in the
 * Alchemy Software is copyrighted by Stanley Kok, Parag
 * Singla, Matthew Richardson, Pedro Domingos, Marc
 * Sumner and Hoifung Poon.
 * 
 * Copyright [2004-07] Stanley Kok, Parag Singla, Matthew
 * Richardson, Pedro Domingos, Marc Sumner and Hoifung
 * Poon. All rights reserved.
 * 
 * Contact: Pedro Domingos, University of Washington
 * (pedrod@cs.washington.edu).
 * 
 * Redistribution and use in source and binary forms, with
 * or without modification, are permitted provided that
 * the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above
 * copyright notice, this list of conditions and the
 * following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the
 * above copyright notice, this list of conditions and the
 * following disclaimer in the documentation and/or other
 * materials provided with the distribution.
 * 
 * 3. All advertising materials mentioning features or use
 * of this software must display the following
 * acknowledgment: "This product includes software
 * developed by Stanley Kok, Parag Singla, Matthew
 * Richardson, Pedro Domingos, Marc Sumner and Hoifung
 * Poon in the Department of Computer Science and
 * Engineering at the University of Washington".
 * 
 * 4. Your publications acknowledge the use or
 * contribution made by the Software to your research
 * using the following citation(s): 
 * Stanley Kok, Parag Singla, Matthew Richardson and
 * Pedro Domingos (2005). "The Alchemy System for
 * Statistical Relational AI", Technical Report,
 * Department of Computer Science and Engineering,
 * University of Washington, Seattle, WA.
 * http://www.cs.washington.edu/ai/alchemy.
 * 
 * 5. Neither the name of the University of Washington nor
 * the names of its contributors may be used to endorse or
 * promote products derived from this software without
 * specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE UNIVERSITY OF WASHINGTON
 * AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE UNIVERSITY
 * OF WASHINGTON OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */
#ifndef MRF_H_SEP_23_2005
#define MRF_H_SEP_23_2005

#include <sys/times.h>
#include <sys/time.h>
#include <cstdlib>
#include <cfloat>
#include <fstream>
#include "timer.h"
#include "mln.h"
#include "groundpredicate.h"

#define MAX_LINE 1000

const bool mrfdebug = false;

///////////////////////////////////////////////////////////////////////////

  // used as parameter of addGndClause()
struct AddGroundClauseStruct
{
  AddGroundClauseStruct(const GroundPredicateSet* const & sseenPreds,
                        GroundPredicateSet* const & uunseenPreds,
                        GroundPredicateHashArray* const & ggndPreds,
                        const Array<int>* const & aallPredGndingsAreQueries,
                        GroundClauseSet* const & ggndClausesSet,
                        Array<GroundClause*>* const & ggndClauses,
                        const bool& mmarkHardGndClauses,
                        const double* const & pparentWtPtr,
                        const int & cclauseId)
    : seenPreds(sseenPreds), unseenPreds(uunseenPreds), gndPreds(ggndPreds),
      allPredGndingsAreQueries(aallPredGndingsAreQueries),
      gndClausesSet(ggndClausesSet),
      gndClauses(ggndClauses), markHardGndClauses(mmarkHardGndClauses),
      parentWtPtr(pparentWtPtr), clauseId(cclauseId) {}
  
  ~AddGroundClauseStruct() {}
  
  const GroundPredicateSet* seenPreds;
  GroundPredicateSet* unseenPreds;
  GroundPredicateHashArray* gndPreds;
  const Array<int>* allPredGndingsAreQueries;
  GroundClauseSet* gndClausesSet;
  Array<GroundClause*>* gndClauses;
  const bool markHardGndClauses;
  const double* parentWtPtr;
  const int clauseId;
};

///////////////////////////////////////////////////////////////////////////


class MRF
{
 public:
    //allPredGndingsAreQueries[p] is 1 (true) if all groundings of predicate p 
    //are in queries, otherwise it is 0 (false). 
    //allPredGndingsAreQueries can be
    //NULL if none of the predicates have all their groundings as queries.
  MRF(const GroundPredicateHashArray* const& queries, 
      const Array<int>* const & allPredGndingsAreQueries,
      const Domain* const & domain,  const Database * const & db, 
      const MLN* const & mln, const bool& markHardGndClauses,
      const bool& trackParentClauseWts, const int& memLimit)
  {
    cout << "creating mrf..." << endl; 
    Timer timer;
    GroundPredicateSet unseenPreds, seenPreds;
    GroundPredicateToIntMap gndPredsMap;
    GroundClauseSet gndClausesSet;
    gndPreds_ = new GroundPredicateHashArray;
    gndClauses_ = new Array<GroundClause*>;
    blocks_ = new Array<Array<int> >;
    blocks_->growToSize(domain->getNumPredBlocks());
    blockEvidence_ = new Array<bool>(*(domain->getBlockEvidenceArray()));
    long double memNeeded = 0;
	
      //add GroundPredicates in queries to unseenPreds
    for (int i = 0; i < queries->size(); i++)
    {
      GroundPredicate* gp = (*queries)[i];
      unseenPreds.insert(gp);
      int gndPredIdx = gndPreds_->append(gp);
      assert(gndPredsMap.find(gp) == gndPredsMap.end());
      gndPredsMap[gp] = gndPredIdx;
    }

      // If too much memory to build MRF then destroy it
    if (memLimit > 0)
    {
      memNeeded = sizeKB();
      if (memNeeded > memLimit)
      {
        for (int i = 0; i < gndClauses_->size(); i++)
          delete (*gndClauses_)[i];
        delete gndClauses_;    

        for (int i = 0; i < gndPreds_->size(); i++)
          if ((*gndPreds_)[i]) delete (*gndPreds_)[i];
        delete gndPreds_;
    
        for (int i = 0; i < blocks_->size(); i++)
          (*blocks_)[i].clearAndCompress();
        delete blocks_;
    
        delete blockEvidence_;
                    
        throw 1;
      }
    }
	
      //while there are still unknown preds we have not looked at
    while (!unseenPreds.empty())   
    {
      GroundPredicateSet::iterator predIt = unseenPreds.begin();
      GroundPredicate* pred = *predIt;
      unsigned int predId = pred->getId();
      //cout << "\tlooking at pred: ";  pred->print(cout, domain); cout << endl;

      bool genClausesForAllPredGndings = false;
        // if all groundings of predicate with predId are queries
      if (allPredGndingsAreQueries && (*allPredGndingsAreQueries)[predId]>=1)
      {
          // if we have not generated gnd clauses containing the queries before
        if ((*allPredGndingsAreQueries)[predId] == 1) 
          genClausesForAllPredGndings = true;
        else
        {   //we have dealt with predicate predId already          
          //cout << "\terasing at pred: ";  pred->print(cout, domain); 
          //cout<< endl;
          unseenPreds.erase(predIt);
          seenPreds.insert(pred);
          continue;
        }
      }
        //get all clauses that contains pred with predId
      const Array<IndexClause*>* clauses
        = mln->getClausesContainingPred(predId);

        //for each clause, ground it and find those with unknown truth values,
        //dropping ground preds which do not matter to the clause's truth value
      for (int i = 0; clauses && i < clauses->size(); i++)
      {
        Clause* c = (*clauses)[i]->clause;
//cout << "\tIn clause c: ";  c->printWithWtAndStrVar(cout, domain); cout << endl;
		const int clauseId = mln->findClauseIdx(c);  
		assert(clauseId >= 0);
		
		  //ignore clause with zero weight
        if (c->getWt() == 0) continue;

          //add gnd clauses with unknown truth values to gndClauses_
        const double* parentWtPtr =
          (trackParentClauseWts) ? c->getWtPtr() : NULL;
        AddGroundClauseStruct agc(&seenPreds, &unseenPreds, gndPreds_,
                                  allPredGndingsAreQueries,
                                  &gndClausesSet, gndClauses_,
                                  markHardGndClauses, parentWtPtr,
                                  clauseId);

    	try
        {
          addUnknownGndClauses(pred, c, domain, db, genClausesForAllPredGndings,
                               &agc);
        }
        catch (bad_alloc&)
        {
          throw 1;
        }

          // If too much memory to build MRF then destroy it
        if (memLimit > 0)
        {
          memNeeded = sizeKB();
            //cout << "preds " << gndPreds_->size() << endl;
            //cout << "clauses " << gndClauses_->size() << endl;
            //cout << "memory " << memNeeded << endl;
            //cout << "limit " << memLimit << endl;
          if (memNeeded > memLimit)
          {
            for (int i = 0; i < gndClauses_->size(); i++)
              delete (*gndClauses_)[i];
            delete gndClauses_;    

            for (int i = 0; i < gndPreds_->size(); i++)
              if ((*gndPreds_)[i]) delete (*gndPreds_)[i];
            delete gndPreds_;
    
            for (int i = 0; i < blocks_->size(); i++)
              (*blocks_)[i].clearAndCompress();
            delete blocks_;
    
            delete blockEvidence_;
            throw 1;
          }
        }
      }

      //clauses with negative wts are handled by the inference algorithms

      //if all the gnd clauses that pred appears in have known truth value,
      //it is not added to gndPreds_ and excluded from the MCMC network

      //cout << "\terasing pred: ";  pred->print(cout, domain); cout << endl;
      unseenPreds.erase(predIt);
      seenPreds.insert(pred);
      if (genClausesForAllPredGndings)
      {
        assert(allPredGndingsAreQueries && 
               (*allPredGndingsAreQueries)[predId]==1);
          //indicate we have seen all groundings of pred predId
        (*allPredGndingsAreQueries)[predId]++;
      }
    }//while (!unseenPreds.empty())

    cout << "number of grounded predicates = " << gndPreds_->size() << endl;
    cout << "number of grounded clauses = " << gndClauses_->size() << endl;
    if (gndClauses_->size() == 0)
      cout<< "Markov blankets of query ground predicates are empty" << endl;

    if (mrfdebug)
    {
      cout << "Clauses in MRF: " << endl;
      for (int i = 0; i < gndClauses_->size(); i++)
      {
        (*gndClauses_)[i]->print(cout, domain, gndPreds_);
        cout << endl;
      }
    }
      // Compress preds and find blocks of preds
    for (int i = 0; i < gndPreds_->size(); i++)
    {
      (*gndPreds_)[i]->compress();

      const Array<Array<Predicate*>*>* blocks = domain->getPredBlocks();
      for (int j = 0; j < blocks->size(); j++)
      {
        Array<Predicate*>* block = (*blocks)[j];
        for (int k = 0; k < block->size(); k++)
        {
          Predicate* pred = (*block)[k];
          if (pred->canBeGroundedAs((*gndPreds_)[i]))
          {
            (*blocks_)[j].append(i);
          }
        }
      }
    }

      // Remove empty blocks (blocks generated in domain, but contain no query
      // atoms)
    int i = 0;
    while (i < blocks_->size())
    {
      Array<int> block = (*blocks_)[i];
      if (block.empty())
      {
        blocks_->removeItem(i);
        blockEvidence_->removeItem(i);
        continue;
      }
      i++;
    }
    
    gndPreds_->compress();
    gndClauses_->compress();

    cout <<"Time taken to construct MRF = ";
    Timer::printTime(cout,timer.time());
    cout << endl;
  }

  /**
   * Computes and returns size of the mrf in kilobytes
   */
  long double sizeKB()
  {
      // # of ground clauses times memory for a ground clause +
      // # of ground predicates times memory for a ground predicate
    long double size = 0;
    for (int i = 0; i < gndClauses_->size(); i++)
      size += (*gndClauses_)[i]->sizeKB();
    for (int i = 0; i < gndPreds_->size(); i++)
      size += (*gndPreds_)[i]->sizeKB();

    return size;    
  }

    // Do not delete the clause and truncClause argument.
    // This function is tightly bound to Clause::createAndAddUnknownClause().
  static void addUnknownGndClause(const AddGroundClauseStruct* const & agcs, 
                                  const Clause* const & clause,
                                  const Clause* const & truncClause,
                                  const bool& isHardClause)
  {
    const GroundPredicateSet* seenPreds     = agcs->seenPreds;
    GroundPredicateSet*       unseenPreds   = agcs->unseenPreds;
    GroundPredicateHashArray* gndPreds      = agcs->gndPreds;
    const Array<int>* allGndingsAreQueries  = agcs->allPredGndingsAreQueries;
    GroundClauseSet*          gndClausesSet = agcs->gndClausesSet;
    Array<GroundClause*>*     gndClauses    = agcs->gndClauses;
    const bool markHardGndClauses           = agcs->markHardGndClauses;
    const double* parentWtPtr               = agcs->parentWtPtr;
    const int clauseId                      = agcs->clauseId;

    // Check none of the grounded clause's predicates have been seen before.
    // If any of them have been seen before, this clause has been created 
    // before (for that seen predicate), and can be ignored

      // Check the untruncated ground clause whether any of its predicates
      // have been seen before
    bool seenBefore = false;
    for (int j = 0; j < clause->getNumPredicates(); j++)
    {
      Predicate* p = clause->getPredicate(j);
      GroundPredicate* gp = new GroundPredicate(p);
      if (seenPreds->find(gp) !=  seenPreds->end() ||
          (allGndingsAreQueries && (*allGndingsAreQueries)[gp->getId()] > 1) )
      { 
        seenBefore = true;
        break;
      }
      delete gp;
    }

    //delete gndClause;
    if (seenBefore) return;

    GroundClause* gndClause = new GroundClause(truncClause, gndPreds);
    if (markHardGndClauses && isHardClause) gndClause->setWtToHardWt();
    assert(gndClause->getWt() != 0);

    GroundClauseSet::iterator iter = gndClausesSet->find(gndClause);
      // If the unknown clause is not in gndClauses
    if (iter == gndClausesSet->end())
    {
      gndClausesSet->insert(gndClause);
      gndClauses->append(gndClause);
      gndClause->appendToGndPreds(gndPreds);
        // gndClause's wt is set when it was constructed
      if (parentWtPtr)
      { 
        gndClause->appendParentWtPtr(parentWtPtr);
        gndClause->incrementClauseFrequency(clauseId, 1);
        assert(gndClause->getWt() == *parentWtPtr);
      }      

        // Add the unknown predicates of the clause to unseenPreds if 
        // the predicates are already not in it
      for (int j = 0; j < gndClause->getNumGroundPredicates(); j++)
      {
        GroundPredicate* gp =
          (GroundPredicate*)gndClause->getGroundPredicate(j, gndPreds);
        assert(seenPreds->find(gp) == seenPreds->end());
          // if the ground predicate is not in unseenPreds
        GroundPredicateSet::iterator it = unseenPreds->find(gp);
        if (it == unseenPreds->end())
        {
          //cout << "\tinserting into unseen pred: ";  
          //pred->print(cout, domain); cout << endl;
          unseenPreds->insert(gp);
        }
      }
    }
    else
    {  // gndClause has appeared before, so just accumulate its weight
      (*iter)->addWt(gndClause->getWt());

      if (parentWtPtr)
      {
        (*iter)->appendParentWtPtr(parentWtPtr);
        (*iter)->incrementClauseFrequency(clauseId, 1);
      }

      delete gndClause;
    }
  } //addUnknownGndClause()



  ~MRF()
  {
    for (int i = 0; i < gndClauses_->size(); i++)
      if ((*gndClauses_)[i]) delete (*gndClauses_)[i];
    delete gndClauses_;    

    for (int i = 0; i < gndPreds_->size(); i++)
      if ((*gndPreds_)[i]) delete (*gndPreds_)[i];
    delete gndPreds_;
    
    for (int i = 0; i < blocks_->size(); i++)
      (*blocks_)[i].clearAndCompress();
    delete blocks_;
    
    delete blockEvidence_;
  }

  void deleteGndPredsGndClauseSets()
  {
    for (int i = 0; i < gndPreds_->size(); i++)
      (*gndPreds_)[i]->deleteGndClauseSet();
  }  

  
  void setGndClausesWtsToSumOfParentWts()
  {
    for (int i = 0; i < gndClauses_->size(); i++)
      (*gndClauses_)[i]->setWtToSumOfParentWts();
  }

  const GroundPredicateHashArray* getGndPreds() const { return gndPreds_; }

  const Array<GroundClause*>* getGndClauses() const { return gndClauses_; }

 private:

  void addUnknownGndClauses(const GroundPredicate* const& queryGndPred,
                            Clause* const & c, const Domain* const & domain, 
                            const Database* const & db, 
                            const bool& genClauseForAllPredGndings,
                            const AddGroundClauseStruct* const & agcs)
  {
    
    if (genClauseForAllPredGndings)
      c->addUnknownClauses(domain, db, -1, NULL, agcs);
    else
    {
      for (int i = 0; i < c->getNumPredicates(); i++)
      {
        if (c->getPredicate(i)->canBeGroundedAs(queryGndPred))
          c->addUnknownClauses(domain, db, i, queryGndPred, agcs);
      }
    }
  } 

 public:

  const int getNumGndPreds()
  {
    return gndPreds_->size();
  }

  const int getNumGndClauses()
  {
    return gndClauses_->size();
  }

  Array<Array<int> >* getBlocks()
  {
    return blocks_;
  }
  
  Array<bool>* getBlockEvidence()
  {
    return blockEvidence_;
  }
  
 private:
  GroundPredicateHashArray* gndPreds_;
  Array<GroundClause*>* gndClauses_;
    // Blocks of gndPred indices which belong together
  Array<Array<int> >* blocks_;
    // Flags indicating if block is fulfilled by evidence
  Array<bool>* blockEvidence_;
};


#endif
