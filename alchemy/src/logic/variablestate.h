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
#ifndef VARIABLESTATE_H_
#define VARIABLESTATE_H_

#include "mrf.h"

const int NOVALUE = -1;
const bool vsdebug = false;

/**
 * Represents the state of propositional variables and clauses. Some of this
 * code is based on the MaxWalkSat package of Kautz et al.
 * 
 * All inference algorithms should have a VariableState to access the
 * information needed in its predicates and clauses.
 * 
 * Each atom has its own index starting at 1. The negation of an atom with
 * index a is represented by -a (this is why the indices do not start at 0).
 * Each clause has its own index starting at 0.
 * 
 * A VariableState is either eager or lazy. Eager states build an MRF upfront
 * based on an MLN and a domain. Thus, all ground clauses and predicates are
 * in memory after building the MRF. Lazy states activate atoms and clauses as
 * they are needed from the MLN and domain. An atom is activated if it is in
 * an unsatisfied clause with the assumption of all atoms being false or if it
 * is looked at during inference (it is flipped or the cost of flipping it is
 * computed). Active clauses are those which contain active atoms.
 */
class VariableState
{
 public:

  /**
   * Constructor for a VariableState. The hard clause weight is set. In lazy
   * mode, the initial active atoms and clauses are retrieved and in eager
   * mode, the MRF is built and the atoms and clauses are retrieved from it.
   * In addition, all information arrays are filled with information.
   * 
   * @param unknownQueries Query predicates with unknown values used to build
   * MRF in eager mode.
   * @param knownQueries Query predicates with known values used to build MRF
   * in eager mode.
   * @param knownQueryValues Truth values of the known query predicates.
   * @param allPredGndingsAreQueries Array used to build MRF in eager mode.
   * @param mln mln and domain are used to build MRF in eager state and to
   * retrieve active atoms in lazy state.
   * @param domain mln and domain are used to build MRF in eager state and to
   * retrieve active atoms in lazy state.
   * @param lazy Flag stating whether lazy mode is used or not.
   */
  VariableState(GroundPredicateHashArray* const& unknownQueries,
                GroundPredicateHashArray* const& knownQueries,
                Array<TruthValue>* const & knownQueryValues,
                const Array<int>* const & allPredGndingsAreQueries,
                const bool& markHardGndClauses,
                const bool& trackParentClauseWts,
                const MLN* const & mln, const Domain* const & domain,
                const bool& lazy)
  {
    this->mln_ = (MLN*)mln;
    this->domain_ = (Domain*)domain;
    this->lazy_ = lazy;

      // Instantiate information
    baseNumAtoms_ = 0;
    activeAtoms_ = 0;
    numFalseClauses_ = 0;
    costOfFalseClauses_ = 0.0;
    lowCost_ = LDBL_MAX;
    lowBad_ = INT_MAX;

      // Clauses and preds are stored in gndClauses_ and gndPreds_
    gndClauses_ = new Array<GroundClause*>;
    gndPreds_ = new Array<GroundPredicate*>;

      // Set the hard clause weight
    setHardClauseWeight();

      // Lazy version: Produce state with initial active atoms and clauses
    if (lazy_)
    {
        // Unknown preds are treated as false
      domain_->getDB()->setPerformingInference(true);

        // Blocks are copied from the domain
      initLazyBlocks();

      clauseLimit_ = INT_MAX;
      noApprox_ = false;
      haveDeactivated_ = false;

        ///// Get initial active atoms and clauses /////
        // Get initial set of active atoms (atoms in unsat. clauses)
        // Assumption is: all atoms are initially false except those in blocks

        // One atom in each block is set to true and activated
      addOneAtomToEachBlock();

//cout << "After addOneAtomToEachBlock" << endl;
//for (int i = 1; i < atom_.size(); i++)
//  cout << atom_[i] << endl;

      bool ignoreActivePreds = false;
      getActiveClauses(newClauses_, ignoreActivePreds);
      int defaultCnt = newClauses_.size();
      long double defaultCost = 0;

      for (int i = 0; i < defaultCnt; i++)
      {
        if (newClauses_[i]->isHardClause())
          defaultCost += hardWt_;
        else
          defaultCost += abs(newClauses_[i]->getWt());
      }

        // Clear ground clauses in the ground preds
      for (int i = 0; i < gndPredHashArray_.size(); i++)
        gndPredHashArray_[i]->removeGndClauses();

        // Delete new clauses
      for (int i = 0; i < newClauses_.size(); i++)
        delete newClauses_[i];
      newClauses_.clear();

      baseNumAtoms_ = gndPredHashArray_.size();
      cout << "Number of Baseatoms = " << baseNumAtoms_ << endl;
      cout << "Default => Cost\t" << "******\t" << " Clause Cnt\t" << endl;
      cout << "           " << defaultCost << "\t" << "******\t" << defaultCnt
           << "\t" << endl << endl;

        // Set base atoms as active in DB
      for (int i = 0; i < baseNumAtoms_; i++)
      {
        domain_->getDB()->setActiveStatus(gndPredHashArray_[i], true);
        activeAtoms_++;        
      }

        // Add the rest of the atoms in the blocks, but don't activate
      fillLazyBlocks();

        // Get the initial set of active clauses
      ignoreActivePreds = false;
      getActiveClauses(newClauses_, ignoreActivePreds);      
    } // End lazy version
      // Eager version: Use KBMC to produce the state
    else
    {
      unePreds_ = unknownQueries;
      knePreds_ = knownQueries;
      knePredValues_ = knownQueryValues;

        // MRF is built on known and unknown queries
      int size = 0;
      if (unknownQueries) size += unknownQueries->size();
      if (knownQueries) size += knownQueries->size();
      GroundPredicateHashArray* queries = new GroundPredicateHashArray(size);
      if (unknownQueries) queries->append(unknownQueries);
      if (knownQueries) queries->append(knownQueries);
      mrf_ = new MRF(queries, allPredGndingsAreQueries, domain_,
                     domain_->getDB(), mln_, markHardGndClauses,
                     trackParentClauseWts, -1);
        //delete to save space. Can be deleted because no more gndClauses are
        //appended to gndPreds beyond this point
      mrf_->deleteGndPredsGndClauseSets();
        //do not delete the intArrRep in gndPreds_;
      delete queries;

        // Blocks built in MRF
      blocks_ = mrf_->getBlocks();
      blockEvidence_ = mrf_->getBlockEvidence();
        
        // Put ground clauses in newClauses_
      newClauses_ = *(Array<GroundClause*>*)mrf_->getGndClauses();
        // Put ground preds in the hash array
      //const Array<GroundPredicate*>* gndPreds = mrf_->getGndPreds();
      const GroundPredicateHashArray* gndPreds = mrf_->getGndPreds();
      for (int i = 0; i < gndPreds->size(); i++)
        gndPredHashArray_.append((*gndPreds)[i]);
    
        // baseNumAtoms_ are all atoms in eager version
      baseNumAtoms_ = gndPredHashArray_.size();        
    } // End eager version
    
      // At this point, ground clauses are held in newClauses_
      // and ground predicates are held in gndPredHashArray_
      // for both versions
    
      // Add the clauses and preds and fill info arrays
    bool initial = true;
    addNewClauses(initial);
    
    cout << "Initial num. of clauses: " << getNumClauses() << endl;
  }

  /**
   * Destructor. Blocks are deleted in lazy version; MRF is deleted in eager
   * version.
   */ 
  ~VariableState()
  {
    if (lazy_)
    {
        // Block information from lazy version is deleted
      for (int i = 0; i < blocks_->size(); i++)
        (*blocks_)[i].clearAndCompress();
      delete blocks_;
    
      delete blockEvidence_;  
    }
    else
    {
        // MRF from eager version is deleted
      if (mrf_) delete mrf_;
      //if (unePreds_) delete unePreds_;
      //if (knePreds_) delete knePreds_;
      //if (knePredValues_) delete knePredValues_;
    }    
  }


  /**
   * New clauses are added to the state. If not the initialization, then
   * makecost and breakcost are updated for the new atoms.
   * 
   * @param initial If true, this is the first time new clauses have been
   * added.
   */
  void addNewClauses(bool initial)
  {
    if (vsdebug)
      cout << "Adding " << newClauses_.size() << " new clauses.." << endl;

      // Store the old number of clauses and atoms
    int oldNumClauses = getNumClauses();
    int oldNumAtoms = getNumAtoms();

    gndClauses_->append(newClauses_);
    gndPreds_->growToSize(gndPredHashArray_.size());

    int numAtoms = getNumAtoms();
    int numClauses = getNumClauses();
      // If no new atoms or clauses have been added, then do nothing
    if (numAtoms == oldNumAtoms && numClauses == oldNumClauses) return;

    if (vsdebug) cout << "Clauses: " << numClauses << endl;
      // atomIdx starts at 1
    atom_.growToSize(numAtoms + 1, false);

    makeCost_.growToSize(numAtoms + 1, 0.0);
    breakCost_.growToSize(numAtoms + 1, 0.0);
    lowAtom_.growToSize(numAtoms + 1, false);
    fixedAtom_.growToSize(numAtoms + 1, 0);

      // Copy ground preds to gndPreds_ and set values in atom and lowAtom
    for (int i = oldNumAtoms; i < gndPredHashArray_.size(); i++)
    {
      (*gndPreds_)[i] = gndPredHashArray_[i];

      if (vsdebug)
      {
        cout << "New pred " << i + 1 << ": ";
        (*gndPreds_)[i]->print(cout, domain_);
        cout << endl;
      }

      lowAtom_[i + 1] = atom_[i + 1] = 
        (domain_->getDB()->getValue((*gndPreds_)[i]) == TRUE) ? true : false;
    }
    newClauses_.clear();

    clause_.growToSize(numClauses);
    clauseCost_.growToSize(numClauses);
    falseClause_.growToSize(numClauses);
    whereFalse_.growToSize(numClauses);
    numTrueLits_.growToSize(numClauses);
    watch1_.growToSize(numClauses);
    watch2_.growToSize(numClauses);
    isSatisfied_.growToSize(numClauses, false);
    deadClause_.growToSize(numClauses, false);
    threshold_.growToSize(numClauses, false);

    occurence_.growToSize(2*numAtoms + 1);

    for (int i = oldNumClauses; i < numClauses; i++)
    {
      GroundClause* gndClause = (*gndClauses_)[i];

      if (vsdebug)
      {
        cout << "New clause " << i << ": ";
        gndClause->print(cout, domain_, &gndPredHashArray_);
        cout << endl;
      }
      
        // Set thresholds for clause selection
      if (gndClause->isHardClause()) threshold_[i] = RAND_MAX;
      else
      {
        double w = gndClause->getWt();
        threshold_[i] = RAND_MAX*(1 - exp(-abs(w)));
        if (vsdebug)
        {
          cout << "Weight: " << w << endl;            
        }
      }
      if (vsdebug)
        cout << "Threshold: " << threshold_[i] << endl;            
      
      int numGndPreds = gndClause->getNumGroundPredicates();
      clause_[i].growToSize(numGndPreds);

      for (int j = 0; j < numGndPreds; j++) 
      {
        int lit = gndClause->getGroundPredicateIndex(j);
        clause_[i][j] = lit;
        int litIdx = 2*abs(lit) - (lit > 0);
        occurence_[litIdx].append(i);
      }

        // Hard clause weight has been previously determined
      if (gndClause->isHardClause())
        clauseCost_[i] = hardWt_;
      else
        clauseCost_[i] = gndClause->getWt();
    }

    if (!initial)
    {
      //initNumSatLiterals(1, oldNumClauses);
      if (useThreshold_)
      {
        killClauses(oldNumClauses);
      }
      else
      {
        initMakeBreakCostWatch(oldNumClauses);
      }
    }
    if (vsdebug) cout << "Done adding new clauses.." << endl;
  }

  /**
   * Information about the state is reset and initialized.
   */
  void init()
  {
      // Initialize info arrays
    initMakeBreakCostWatch();
  }
  
  /**
   * State is re-initialized with all new clauses and atoms.
   */
  void reinit()
  {
    clause_.clearAndCompress();
    clauseCost_.clearAndCompress();
    falseClause_.clearAndCompress();
    whereFalse_.clearAndCompress();
    numTrueLits_.clearAndCompress();
    watch1_.clearAndCompress();
    watch2_.clearAndCompress();
    isSatisfied_.clearAndCompress();
    deadClause_.clearAndCompress();
    threshold_.clearAndCompress();

    newClauses_.append(gndClauses_);
    gndClauses_->clearAndCompress();
    gndPreds_->clearAndCompress();
    for (int i = 0; i < occurence_.size(); i++)
      occurence_.clearAndCompress();
    occurence_.clearAndCompress();
    
      // Add the clauses and preds and fill info arrays
    bool initial = true;
    addNewClauses(initial);
    baseNumAtoms_ = gndPredHashArray_.size();
    init();    
  }
  
  /**
   * Makes a random truth assigment to all (active) atoms. Blocks are
   * taken into account: exactly one atom in the block is set to true
   * and the others are set to false.
   */
  void initRandom()
  {
      // Set one in each block to true randomly
    initBlocksRandom();

      // Random truth value for all not in blocks
    for (int i = 1; i <= baseNumAtoms_; i++)
    {
        // fixedAtom_[i] = -1: false, fixedAtom_[i] = 1: true
      if (fixedAtom_[i] != 0)
      {
        setValueOfAtom(i, (fixedAtom_[i] == 1));
        continue;
      }
        // Blocks are initialized above
      if (getBlockIndex(i - 1) >= 0)
      {
        if (vsdebug) cout << "Atom " << i << " in block" << endl;
        continue;
      }
        // Not fixed and not in block
      else
      {
        if (vsdebug) cout << "Atom " << i << " not in block" << endl;
        setValueOfAtom(i, random() % 2);
      }
    }
    init();
  }

  /**
   * Sets one atom in each block to true and the rest to false.
   */
  void initBlocksRandom()
  {
    if (vsdebug)
    {
      cout << "Initializing blocks randomly" << endl;
      cout << "Num. of blocks: " << blocks_->size() << endl;
    }
    
      // For each block: select one to set to true
    for (int i = 0; i < blocks_->size(); i++)
    {
        // True fixed atom in the block: set others to false
      if (int trueFixedAtomInBlock = getTrueFixedAtomInBlock(i) >= 0)
      {
        if (vsdebug) cout << "True fixed atom in block " << i << endl;
        setOthersInBlockToFalse(trueFixedAtomInBlock, i);
        continue;
      }

        // If evidence atom exists, then all others are false
      if ((*blockEvidence_)[i])
      {
          // If first argument is -1, then all are set to false
        setOthersInBlockToFalse(-1, i);
        continue;
      }

        // Eager version: pick one at random
      Array<int>& block = (*blocks_)[i];
      bool ok = false;
      while (!ok)
      {
        int chosen = random() % block.size();
          // Atom not fixed
        if (fixedAtom_[block[chosen] + 1] == 0)
        {
          if (vsdebug) cout << "Atom " << block[chosen] + 1 
                            << " chosen in block" << endl;
          setValueOfAtom(block[chosen] + 1, true);
          setOthersInBlockToFalse(chosen, i);
          ok = true;
        }
      }
    }
    if (vsdebug) cout << "Done initializing blocks randomly" << endl;
  }      

  /**
   * Re-initializes the makeCost, breakCost and watch arrays based on the
   * current variable assignment.
   */
  void initMakeBreakCostWatch()
  {
      // Reset info concerning true lits, false clauses, etc.
    for (int i = 0; i < getNumClauses(); i++) numTrueLits_[i] = 0;
    numFalseClauses_ = 0;
    costOfFalseClauses_ = 0.0;
    lowCost_ = LDBL_MAX;
    lowBad_ = INT_MAX;

    assert(makeCost_.size() == breakCost_.size());
      // Reset make and break cost to 0
    for (int i = 0; i < makeCost_.size(); i++)
    {
      makeCost_[i] = breakCost_[i] = 0.0;
    }
    initMakeBreakCostWatch(0);
  }

  /**
   * Initialize makeCost and breakCost and watch arrays based on the current
   * variable assignment.
   * 
   * @param startClause All clauses with index of this or greater are
   * initialized.
   */
  void initMakeBreakCostWatch(const int& startClause)
  {
    int theTrueLit = -1;
      // Initialize breakCost and makeCost in the following:
    for (int i = startClause; i < getNumClauses(); i++)
    {
        // Don't look at dead clauses
      if (deadClause_[i]) continue;
      int trueLit1 = 0;
      int trueLit2 = 0;
      long double cost = clauseCost_[i];
      numTrueLits_[i] = 0;
      for (int j = 0; j < getClauseSize(i); j++)
      {
        if (isTrueLiteral(clause_[i][j]))
        { // ij is true lit
          numTrueLits_[i]++;
          theTrueLit = abs(clause_[i][j]);
          if (!trueLit1) trueLit1 = theTrueLit;
          else if (trueLit1 && !trueLit2) trueLit2 = theTrueLit;
        }
      }

        // Unsatisfied positive-weighted clauses or
        // Satisfied negative-weighted clauses
      if ((numTrueLits_[i] == 0 && cost > 0) ||
          (numTrueLits_[i] > 0 && cost < 0))
      {
        whereFalse_[i] = numFalseClauses_;
        falseClause_[numFalseClauses_] = i;
        numFalseClauses_++;
        costOfFalseClauses_ += abs(cost);
        if (highestCost_ == abs(cost)) {eqHighest_ = true; numHighest_++;}

          // Unsat. pos. clause: increase makeCost_ of all atoms
        if (numTrueLits_[i] == 0)
          for (int j = 0; j < getClauseSize(i); j++)
          {
            makeCost_[abs(clause_[i][j])] += cost;
          }

          // Sat. neg. clause: increase makeCost_ if one true literal
        if (numTrueLits_[i] == 1)
        {
            // Subtract neg. cost
          makeCost_[theTrueLit] -= cost;
          watch1_[i] = theTrueLit;
        }
        else if (numTrueLits_[i] > 1)
        {
          watch1_[i] = trueLit1;
          watch2_[i] = trueLit2;
        }
      }
        // Pos. clauses satisfied by one true literal
      else if (numTrueLits_[i] == 1 && cost > 0)
      {
        breakCost_[theTrueLit] += cost;
        watch1_[i] = theTrueLit;
      }
        // Pos. clauses satisfied by 2 or more true literals
      else if (cost > 0)
      { /*if (numtruelit[i] == 2)*/
        watch1_[i] = trueLit1;
        watch2_[i] = trueLit2;
      }
        // Unsat. neg. clauses: increase breakCost of all atoms
      else if (numTrueLits_[i] == 0 && cost < 0)
      {
        for (int j = 0; j < getClauseSize(i); j++)
          breakCost_[abs(clause_[i][j])] -= cost;
      }
    } // for all clauses
  }

  int getNumAtoms() { return gndPreds_->size(); }
  
  int getNumClauses() { return gndClauses_->size(); }
  
  int getNumDeadClauses()
  { 
    int count = 0;
    for (int i = 0; i < deadClause_.size(); i++)
      if (deadClause_[i]) count++;
    return count;
  }

  /**
   * Returns the absolute index of a random atom.
   */
  int getIndexOfRandomAtom()
  {
    int numAtoms = getNumAtoms();
    if (numAtoms == 0) return NOVALUE;
    return random()%numAtoms + 1;
  }

  /**
   * Returns the absolute index of a random atom in a random unsatisfied
   * pos. clause or the absolute index of a random true literal in a random
   * satisfied clause.
   */
  int getIndexOfAtomInRandomFalseClause()
  {
    if (numFalseClauses_ == 0) return NOVALUE;
    int clauseIdx = falseClause_[random()%numFalseClauses_];
      // Pos. clause: return index of random atom
    if (clauseCost_[clauseIdx] > 0)
      return abs(clause_[clauseIdx][random()%getClauseSize(clauseIdx)]);
      // Neg. clause: find random true lit
    else
      return getRandomTrueLitInClause(clauseIdx);
  }
  
  /**
   * Returns the index of a random unsatisfied pos. clause or a random
   * satisfied neg. clause.
   */
  int getRandomFalseClauseIndex()
  {
    if (numFalseClauses_ == 0) return NOVALUE;
    return falseClause_[random()%numFalseClauses_];
  }
  
  /**
   * Returns the cost of the unsatisfied positive and satisfied negative
   * clauses.
   */
  long double getCostOfFalseClauses()
  {
    return costOfFalseClauses_;
  }
  
  /**
   * Returns the number of the unsatisfied positive and
   * satisfied negative clauses.
   */
  int getNumFalseClauses()
  {
    return numFalseClauses_;
  }

  /**
   * Returns the truth value of an atom.
   * 
   * @atomIdx Index of atom whose truth value is returned.
   * @return Truth value of atom.
   */
  bool getValueOfAtom(const int& atomIdx)
  {
    return atom_[atomIdx];
  }

  /**
   * Returns the truth value of an atom in the best state.
   * 
   * @atomIdx Index of atom whose truth value is returned.
   * @return Truth value of atom.
   */
  bool getValueOfLowAtom(const int& atomIdx)
  {
    return lowAtom_[atomIdx];
  }

  /**
   * Sets the truth value of an atom. The truth value is propagated to the
   * database.
   * 
   * @param atomIdx Index of atom whose value is to be set.
   * @param value Truth value being set.
   */
  void setValueOfAtom(const int& atomIdx, const bool& value)
  {
    if (vsdebug) cout << "Setting value of atom " << atomIdx 
                      << " to " << value << endl;
      // If atom already has this value, then do nothing
    if (atom_[atomIdx] == value) return;
      // Propagate assigment to DB
    GroundPredicate* p = gndPredHashArray_[atomIdx - 1];
    if (value)
    {
      domain_->getDB()->setValue(p, TRUE);
    }
    else
    {
      domain_->getDB()->setValue(p, FALSE);
    }
      // If not active, then activate it
    if (lazy_ && !isActive(atomIdx))
    {
      bool ignoreActivePreds = false;
      activateAtom(atomIdx, ignoreActivePreds);
    }
    atom_[atomIdx] = value;
  }

  /**
   * Retrieves the negative occurence array of an atom.
   */
  Array<int>& getNegOccurenceArray(const int& atomIdx)
  {
    int litIdx = 2*atomIdx;
    return getOccurenceArray(litIdx);
  }

  /**
   * Retrieves the positive occurence array of an atom.
   */
  Array<int>& getPosOccurenceArray(const int& atomIdx)
  {
    int litIdx = 2*atomIdx - 1;
    return getOccurenceArray(litIdx);
  }

  /**
   * Flip the truth value of an atom and update info arrays.
   * 
   * @param toFlip Index of atom to be flipped.
   */
  void flipAtom(const int& toFlip)
  {
    bool toFlipValue = getValueOfAtom(toFlip);
    register int clauseIdx;
    int sign;
    int oppSign;
    int litIdx;
    if (toFlipValue)
      sign = 1;
    else
      sign = 0;
    oppSign = sign ^ 1;

    flipAtomValue(toFlip);
    
      // Update all clauses in which the atom occurs as a true literal
    litIdx = 2*toFlip - sign;
    Array<int>& posOccArray = getOccurenceArray(litIdx);
    for (int i = 0; i < posOccArray.size(); i++)
    {
      clauseIdx = posOccArray[i];
        // Don't look at dead clauses
      if (deadClause_[clauseIdx]) continue;
        // The true lit became a false lit
      int numTrueLits = decrementNumTrueLits(clauseIdx);
      long double cost = getClauseCost(clauseIdx);
      int watch1 = getWatch1(clauseIdx);
      int watch2 = getWatch2(clauseIdx);

        // 1. If last true lit was flipped, then we have to update
        // the makecost / breakcost info accordingly        
      if (numTrueLits == 0)
      {
          // Pos. clause
        if (cost > 0)
        {
            // Add this clause as false in the state
          addFalseClause(clauseIdx);
            // Decrease toFlip's breakcost (add neg. cost)
          addBreakCost(toFlip, -cost);
            // Increase makecost of all vars in clause (add pos. cost)
          addMakeCostToAtomsInClause(clauseIdx, cost);
        }
          // Neg. clause
        else
        {
          assert(cost < 0);
            // Remove this clause as false in the state
          removeFalseClause(clauseIdx);
            // Increase breakcost of all vars in clause (add pos. cost)
          addBreakCostToAtomsInClause(clauseIdx, -cost);        
            // Decrease toFlip's makecost (add neg. cost)
          addMakeCost(toFlip, cost);
        }
      }
        // 2. If there is now one true lit left, then move watch2
        // up to watch1 and increase the breakcost / makecost of watch1
      else if (numTrueLits == 1)
      {
        if (watch1 == toFlip)
        {
          assert(watch1 != watch2);
          setWatch1(clauseIdx, watch2);
          watch1 = getWatch1(clauseIdx);
        }

          // Pos. clause: Increase toFlip's breakcost (add pos. cost)
        if (cost > 0)
        {
          addBreakCost(watch1, cost);
        }
          // Neg. clause: Increase toFlip's makecost (add pos. cost)
        else
        {
          assert(cost < 0);
          addMakeCost(watch1, -cost);
        }
      }
        // 3. If there are 2 or more true lits left, then we have to
        // find a new true lit to watch if one was flipped
      else
      { /* numtruelit[clauseIdx] >= 2 */
          // If watch1[clauseIdx] has been flipped
        if (watch1 == toFlip)
        {
            // find a different true literal to watch
          int diffTrueLit = getTrueLiteralOtherThan(clauseIdx, watch1, watch2);
          setWatch1(clauseIdx, diffTrueLit);
        }
          // If watch2[clauseIdx] has been flipped
        else if (watch2 == toFlip)
        {
            // find a different true literal to watch
          int diffTrueLit = getTrueLiteralOtherThan(clauseIdx, watch1, watch2);
          setWatch2(clauseIdx, diffTrueLit);
        }
      }
    }
        
      // Update all clauses in which the atom occurs as a false literal
    litIdx = 2*toFlip - oppSign;
    Array<int>& negOccArray = getOccurenceArray(litIdx);
    for (int i = 0; i < negOccArray.size(); i++)
    {
      clauseIdx = negOccArray[i];
        // Don't look at dead clauses
      if (deadClause_[clauseIdx]) continue;
        // The false lit became a true lit
      int numTrueLits = incrementNumTrueLits(clauseIdx);
      long double cost = getClauseCost(clauseIdx);
      int watch1 = getWatch1(clauseIdx);

        // 1. If this is the only true lit, then we have to update
        // the makecost / breakcost info accordingly        
      if (numTrueLits == 1)
      {
          // Pos. clause
        if (cost > 0)
        {
            // Remove this clause as false in the state
          removeFalseClause(clauseIdx);
            // Increase toFlip's breakcost (add pos. cost)
          addBreakCost(toFlip, cost);        
            // Decrease makecost of all vars in clause (add neg. cost)
          addMakeCostToAtomsInClause(clauseIdx, -cost);
        }
          // Neg. clause
        else
        {
          assert(cost < 0);
            // Add this clause as false in the state
          addFalseClause(clauseIdx);
            // Decrease breakcost of all vars in clause (add neg. cost)
          addBreakCostToAtomsInClause(clauseIdx, cost);
            // Increase toFlip's makecost (add pos. cost)
          addMakeCost(toFlip, -cost);
        }
          // Watch this atom
        setWatch1(clauseIdx, toFlip);
      }
        // 2. If there are now exactly 2 true lits, then watch second atom
        // and update breakcost
      else
      if (numTrueLits == 2)
      {
        if (cost > 0)
        {
            // Pos. clause
            // Decrease breakcost of first atom being watched (add neg. cost)
          addBreakCost(watch1, -cost);
        }
        else
        {
            // Neg. clause
          assert(cost < 0);
            // Decrease makecost of first atom being watched (add neg. cost)
          addMakeCost(watch1, cost);
        }
        
          // Watch second atom
        setWatch2(clauseIdx, toFlip);
      }
    }
  }

  /**
   * Flips the truth value of an atom. If in lazy mode, the truth value
   * is propagated to the database.
   */  
  void flipAtomValue(const int& atomIdx)
  {
    bool opposite = !atom_[atomIdx];
    setValueOfAtom(atomIdx, opposite);
  }

  /**
   * Calculates the improvement achieved by flipping an atom. This is the cost
   * of clauses which flipping the atom makes good minus the cost of clauses
   * which flipping the atom makes bad. In lazy mode, if the atom is not
   * active, then the atom is activated and the makecost and breakcost are
   * updated.
   * 
   * @param atomIdx Index of atom to flip.
   * 
   * @return Improvement achieved by flipping the atom.
   */
  long double getImprovementByFlipping(const int& atomIdx)
  {
    if (lazy_ && !isActive(atomIdx))
    {
        // First flip the atom to activate it, then flip it back
      flipAtom(atomIdx);
      flipAtom(atomIdx);
    }
    long double improvement = makeCost_[atomIdx] - breakCost_[atomIdx];
    return improvement;
  }
  
  /**
   * If in lazy mode, an atom is activated and all clauses activated by this
   * atom are added to the state. If in eager mode, nothing happens.
   * 
   * @param atomIdx Index of atom to be activated.
   */
  void activateAtom(const int& atomIdx, const bool& ignoreActivePreds)
  {
      // Lazy version: if atom is not active, we need to activate clauses
      // and take their cost into account
    if (lazy_ && !isActive(atomIdx))
    {
      Predicate* p =
        gndPredHashArray_[atomIdx - 1]->createEquivalentPredicate(domain_);
      getActiveClauses(p, newClauses_, true, ignoreActivePreds);
        // Add the clauses and preds and fill info arrays
      bool initial = false;
      addNewClauses(initial);
        // Set active status in db
      domain_->getDB()->setActiveStatus(p, true);
      activeAtoms_++;
      delete p;
    }        
  }

  /**
   * Checks if an atom is active.
   * 
   * @param atomIdx Index of atom to be checked.
   * @return true, if atom is active, otherwise false.
   */  
  bool isActive(const int& atomIdx)
  {
    return domain_->getDB()->getActiveStatus(gndPredHashArray_[atomIdx-1]);
  }
  
  /**
   * Checks if an atom is active.
   * 
   * @param pred Predicate to be checked.
   * @return true, if atom is active, otherwise false.
   */  
  bool isActive(const Predicate* pred)
  {
    return domain_->getDB()->getActiveStatus(pred);
  }

  /**
   * Retrieves the occurence array of an atom.
   */
  Array<int>& getOccurenceArray(const int& idx)
  {
    return occurence_[idx];
  }
  
  /**
   * Increments the number of true literals in a clause.
   */
  int incrementNumTrueLits(const int& clauseIdx)
  {
    return ++numTrueLits_[clauseIdx];
  }

  /**
   * Decrements the number of true literals in a clause.
   */
  int decrementNumTrueLits(const int& clauseIdx)
  {
    return --numTrueLits_[clauseIdx];
  }

  /**
   * Retrieves the number of true literals in a clause.
   */
  int getNumTrueLits(const int& clauseIdx)
  {
    return numTrueLits_[clauseIdx];
  }

  /**
   * Retrieves the cost associated with a clause.
   */
  long double getClauseCost(const int& clauseIdx)
  {
    return clauseCost_[clauseIdx];
  }
  
  /**
   * Retrieves the atoms in a clauses as an Array of ints.
   */
  Array<int>& getAtomsInClause(const int& clauseIdx)
  {
    return clause_[clauseIdx];
  }

  /**
   * Marks a clause as false in the state.
   */
  void addFalseClause(const int& clauseIdx)
  {
    falseClause_[numFalseClauses_] = clauseIdx;
    whereFalse_[clauseIdx] = numFalseClauses_;
    numFalseClauses_++;
    costOfFalseClauses_ += abs(clauseCost_[clauseIdx]);
  }
  
  /**
   * Unmarks a clause as false in the state.
   */
  void removeFalseClause(const int& clauseIdx)
  {
    numFalseClauses_--;
    falseClause_[whereFalse_[clauseIdx]] = falseClause_[numFalseClauses_];
    whereFalse_[falseClause_[numFalseClauses_]] = whereFalse_[clauseIdx];
    costOfFalseClauses_ -= abs(clauseCost_[clauseIdx]);
  }

  /**
   * Increases the breakcost of an atom.
   */
  void addBreakCost(const int& atomIdx, const long double& cost)
  {
    breakCost_[atomIdx] += cost;
  }

  /**
   * Decreases the breakcost of an atom.
   */
  void subtractBreakCost(const int& atomIdx, const long double& cost)
  {
    breakCost_[atomIdx] -= cost;
  }

  /**
   * Increases breakCost of all atoms in a given clause.
   * 
   * @param clauseIdx Index of clause whose atoms' breakCost is altered.
   * @param cost Cost to be added to atoms' breakCost.
   */
  void addBreakCostToAtomsInClause(const int& clauseIdx,
                                   const long double& cost)
  {
    register int size = getClauseSize(clauseIdx);
    for (int i = 0; i < size; i++)
    {
      register int lit = clause_[clauseIdx][i];
      breakCost_[abs(lit)] += cost;
    }
  }

  /**
   * Decreases breakCost of all atoms in a given clause.
   * 
   * @param clauseIdx Index of clause whose atoms' breakCost is altered.
   * @param cost Cost to be subtracted from atoms' breakCost.
   */
  void subtractBreakCostFromAtomsInClause(const int& clauseIdx,
                                          const long double& cost)
  {
    register int size = getClauseSize(clauseIdx);
    for (int i = 0; i < size; i++)
    {
      register int lit = clause_[clauseIdx][i];
      breakCost_[abs(lit)] -= cost;
    }
  }

  /**
   * Increases makeCost of an atom.
   * 
   * @param atomIdx Index of atom whose makeCost is altered.
   * @param cost Cost to be added to atom's makeCost.
   */
  void addMakeCost(const int& atomIdx, const long double& cost)
  {
    makeCost_[atomIdx] += cost;
  }

  /**
   * Decreases makeCost of an atom.
   * 
   * @param atomIdx Index of atom whose makeCost is altered.
   * @param cost Cost to be subtracted from atom's makeCost.
   */
  void subtractMakeCost(const int& atomIdx, const long double& cost)
  {
    makeCost_[atomIdx] -= cost;
  }

  /**
   * Increases makeCost of all atoms in a given clause.
   * 
   * @param clauseIdx Index of clause whose atoms' makeCost is altered.
   * @param cost Cost to be added to atoms' makeCost.
   */
  void addMakeCostToAtomsInClause(const int& clauseIdx,
                                  const long double& cost)
  {
    register int size = getClauseSize(clauseIdx);
    for (int i = 0; i < size; i++)
    {
      register int lit = clause_[clauseIdx][i];
      makeCost_[abs(lit)] += cost;
    }
  }

  /**
   * Decreases makeCost of all atoms in a given clause.
   * 
   * @param clauseIdx Index of clause whose atoms' makeCost is altered.
   * @param cost Cost to be subtracted from atoms' makeCost.
   */
  void subtractMakeCostFromAtomsInClause(const int& clauseIdx,
                                         const long double& cost)
  {
    register int size = getClauseSize(clauseIdx);
    for (int i = 0; i < size; i++)
    {
      register int lit = clause_[clauseIdx][i];
      makeCost_[abs(lit)] -= cost;
    }
  }

  /**
   * Retrieves a true literal in a clause other than the two given.
   * 
   * @param clauseIdx Index of clause from which literal is retrieved.
   * @param atomIdx1 Index of first atom excluded from search.
   * @param atomIdx2 Index of second atom excluded from search.
   * 
   * @return Index of atom found.
   */
  const int getTrueLiteralOtherThan(const int& clauseIdx,
                                    const int& atomIdx1,
                                    const int& atomIdx2)
  {
    register int size = getClauseSize(clauseIdx);
    for (int i = 0; i < size; i++)
    {
      register int lit = clause_[clauseIdx][i];
      register int v = abs(lit);
      if (isTrueLiteral(lit) && v != atomIdx1 && v != atomIdx2)
        return v;
    }
      // If we're here, then no other true lit exists
    assert(false);
    return -1;
  }
  
  /**
   * Checks if a literal is true (Negated and false or non-negated an true).
   */
  const bool isTrueLiteral(const int& literal)
  {
    return ((literal > 0) == atom_[abs(literal)]);
  }

  /**
   * Retrieves the absolute index of the nth atom in a clause.
   */
  const int getAtomInClause(const int& atomIdxInClause, const int& clauseIdx)
  {
    return clause_[clauseIdx][atomIdxInClause];
  }

  /**
   * Retrieves the absolute index of a random atom in a clause.
   */
  const int getRandomAtomInClause(const int& clauseIdx)
  {
    return clause_[clauseIdx][random()%getClauseSize(clauseIdx)];
  }

  /**
   * Retrieves the index of a random true literal in a clause.
   * 
   * @param clauseIdx Index of clause from which the literal is retrieved.
   * @return Index of the atom retrieved.
   */  
  const int getRandomTrueLitInClause(const int& clauseIdx)
  {
    assert(numTrueLits_[clauseIdx] > 0);
    int trueLit = random()%numTrueLits_[clauseIdx];
    int whichTrueLit = 0;
    for (int i = 0; i < getClauseSize(clauseIdx); i++)
    {
      int lit = clause_[clauseIdx][i];
      int atm = abs(lit);
        // True literal
      if (isTrueLiteral(lit))
        if (trueLit == whichTrueLit++)
          return atm;
    }
      // If we're here, then no other true lit exists
    assert(false);
    return -1;
  }

  const double getMaxClauseWeight()
  {
    double maxWeight = 0.0;
    for (int i = 0; i < getNumClauses(); i++)
    {
      double weight = abs(clauseCost_[i]);
      if (weight > maxWeight) maxWeight = weight;
    }
    return maxWeight;
  }
  
  const long double getMakeCost(const int& atomIdx)
  {
    return makeCost_[atomIdx];
  }
   
  const long double getBreakCost(const int& atomIdx)
  {
    return breakCost_[atomIdx];
  }
   
  const int getClauseSize(const int& clauseIdx)
  {
    return clause_[clauseIdx].size();
  }

  const int getWatch1(const int& clauseIdx)
  {
    return watch1_[clauseIdx];
  }

  void setWatch1(const int& clauseIdx, const int& atomIdx)
  {
    watch1_[clauseIdx] = atomIdx;
  }
  
  const int getWatch2(const int& clauseIdx)
  {
    return watch2_[clauseIdx];
  }

  void setWatch2(const int& clauseIdx, const int& atomIdx)
  {
    watch2_[clauseIdx] = atomIdx;
  }
  
  const bool isBlockEvidence(const int& blockIdx)
  {
    return (*blockEvidence_)[blockIdx];
  }

  const int getBlockSize(const int& blockIdx)
  {
      // Lazy: blocks are in domain
    if (lazy_)
      return domain_->getPredBlock(blockIdx)->size();
    else
      return (*blocks_)[blockIdx].size();
  }
  
  /** 
   * Returns the index of the block which the atom with index atomIdx
   * is in. If not in any, returns -1.
   */
  const int getBlockIndex(const int& atomIdx)
  {
    for (int i = 0; i < blocks_->size(); i++)
    {
      int blockIdx = (*blocks_)[i].find(atomIdx);
      if (blockIdx >= 0)
        return i;
    }
    return -1;
  }
  
  Array<int>& getBlockArray(const int& blockIdx)
  {
    return (*blocks_)[blockIdx];
  }

  bool getBlockEvidence(const int& blockIdx)
  {
    return (*blockEvidence_)[blockIdx];
  }
  
  int getNumBlocks()
  {
    return blocks_->size();
  }
  
  /**
   * Returns the cost of bad clauses in the optimum state.
   */
  const long double getLowCost()
  {
    return lowCost_; 
  }

  /**
   * Returns the number of bad clauses in the optimum state.
   */
  const int getLowBad()
  {
    return lowBad_;
  }

  /**
   * Turns all costs into units. Positive costs are converted to 1, negative
   * costs are converted to -1
   */
  void makeUnitCosts()
  {
    for (int i = 0; i < clauseCost_.size(); i++)
    {
      if (clauseCost_[i] > 0) clauseCost_[i] = 1.0;
      else
      {
        assert(clauseCost_[i] < 0);
        clauseCost_[i] = -1.0;
      }
    }
    if (vsdebug) cout << "Made unit costs" << endl;
    initMakeBreakCostWatch();
  }

  /**
   * Save current assignment as an optimum state.
   */
  void saveLowState()
  {
    if (vsdebug) cout << "Saving low state: " << endl;
    for (int i = 1; i <= getNumAtoms(); i++)
    {
      lowAtom_[i] = atom_[i];
      if (vsdebug) cout << lowAtom_[i] << endl;
    }
    lowCost_ = costOfFalseClauses_;
    lowBad_ = numFalseClauses_;
  }

  /**
   * Returns index in block if a true fixed atom is in block, otherwise -1    
   */
  int getTrueFixedAtomInBlock(const int blockIdx)
  {
    Array<int>& block = (*blocks_)[blockIdx];
    for (int i = 0; i < block.size(); i++)
      if (fixedAtom_[block[i] + 1] > 0) return i;
    return -1;
  }

  const GroundPredicateHashArray* getGndPredHashArrayPtr() const
  {
    return &gndPredHashArray_;
  }

  const GroundPredicateHashArray* getUnePreds() const
  {
    return unePreds_;
  }

  const GroundPredicateHashArray* getKnePreds() const
  {
    return knePreds_;
  }

  const Array<TruthValue>* getKnePredValues() const
  {
    return knePredValues_;
  }

  /**
   * Sets the weight of a ground clause to the sum of its parents' weights.
   */
  void setGndClausesWtsToSumOfParentWts()
  {
    for (int i = 0; i < gndClauses_->size(); i++)
    {
      GroundClause* gndClause = (*gndClauses_)[i];
      gndClause->setWtToSumOfParentWts();
      if (vsdebug) cout << "Setting cost of clause " << i << " to "
                        << gndClause->getWt() << endl;
      clauseCost_[i] = gndClause->getWt();

        // Set thresholds for clause selection
      if (gndClause->isHardClause()) threshold_[i] = RAND_MAX;
      else
      {
        double w = gndClause->getWt();
        threshold_[i] = RAND_MAX*(1 - exp(-abs(w)));
        if (vsdebug)
        {
          cout << "Weight: " << w << endl;            
        }
      }
      if (vsdebug)
        cout << "Threshold: " << threshold_[i] << endl;
    }
  }

  /**
   * Gets the number of (true or false) clause groundings in this state. If
   * eager, the first order clause frequencies in the mrf are used.
   * 
   * @param numGndings Array being filled with values
   * @param clauseCnt
   * @param tv
   */
  void getNumClauseGndings(Array<double>* const & numGndings, bool tv)
  {
    // TODO: lazy version
    IntPairItr itr;
    IntPair *clauseFrequencies;
    
      // numGndings should have been initialized with non-negative values
    int clauseCnt = numGndings->size();
    assert(clauseCnt == mln_->getNumClauses());
    for (int clauseno = 0; clauseno < clauseCnt; clauseno++)
      assert ((*numGndings)[clauseno] >= 0);
    
    for (int i = 0; i < gndClauses_->size(); i++)
    {
      GroundClause *gndClause = (*gndClauses_)[i];
      int satLitcnt = 0;
      for (int j = 0; j < gndClause->getNumGroundPredicates(); j++)
      {
        int lit = gndClause->getGroundPredicateIndex(j);
        if (isTrueLiteral(lit)) satLitcnt++;
      }
      //int satLitcnt = getNumTrueLits(i);
      if (tv && satLitcnt == 0)
        continue;
      if (!tv && satLitcnt > 0)
        continue;

      clauseFrequencies = gndClause->getClauseFrequencies();
      for (itr = clauseFrequencies->begin();
           itr != clauseFrequencies->end(); itr++)
      {
        int clauseno = itr->first;
        int frequency = itr->second;
        (*numGndings)[clauseno] += frequency;
      }
    }
  }

  /**
   * Gets the number of (true or false) clause groundings in this state. If
   * eager, the first order clause frequencies in the mrf are used.
   * 
   * @param numGndings
   * @param clauseCnt
   * @param tv
   */
  void getNumClauseGndings(double numGndings[], int clauseCnt, bool tv)
  {
    // TODO: lazy version
    IntPairItr itr;
    IntPair *clauseFrequencies;
    
    for (int clauseno = 0; clauseno < clauseCnt; clauseno++)
      numGndings[clauseno] = 0;
    
    for (int i = 0; i < gndClauses_->size(); i++)
    {
      GroundClause *gndClause = (*gndClauses_)[i];
      int satLitcnt = getNumTrueLits(i);
      if (tv && satLitcnt == 0)
        continue;
      if (!tv && satLitcnt > 0)
        continue;

      clauseFrequencies = gndClause->getClauseFrequencies();
      for (itr = clauseFrequencies->begin();
           itr != clauseFrequencies->end(); itr++)
      {
        int clauseno = itr->first;
        int frequency = itr->second;
        numGndings[clauseno] += frequency;
      }
    }
  }

  /**
   * Gets the number of (true or false) clause groundings in this state. If
   * eager, the first order clause frequencies in the mrf are used.
   * 
   * @param numGndings Will hold the number of groundings for each first-order
   * clause.
   * @param clauseCnt Number of first-order clauses whose groundings are being
   * counted.
   * @param tv If true, true groundings are counted, otherwise false groundings.
   * @param unknownPred If pred is marked as unknown, it is ignored in the count
   */
  void getNumClauseGndingsWithUnknown(double numGndings[], int clauseCnt,
                                      bool tv,
                                      const Array<bool>* const& unknownPred)
  {
    assert(unknownPred->size() == getNumAtoms());
    IntPairItr itr;
    IntPair *clauseFrequencies;
    
    for (int clauseno = 0; clauseno < clauseCnt; clauseno++)
      numGndings[clauseno] = 0;
    
    for (int i = 0; i < gndClauses_->size(); i++)
    {
      GroundClause *gndClause = (*gndClauses_)[i];
      int satLitcnt = 0;
      bool unknown = false;
      for (int j = 0; j < gndClause->getNumGroundPredicates(); j++)
      {
        int lit = gndClause->getGroundPredicateIndex(j);
        if ((*unknownPred)[abs(lit) - 1])
        {
          unknown = true;
          continue;
        }
        if (isTrueLiteral(lit)) satLitcnt++;
      }
      
      if (tv && satLitcnt == 0)
        continue;
      if (!tv && (satLitcnt > 0 || unknown))
        continue;

      clauseFrequencies = gndClause->getClauseFrequencies();
      for (itr = clauseFrequencies->begin();
           itr != clauseFrequencies->end(); itr++)
      {
        int clauseno = itr->first;
        int frequency = itr->second;
        numGndings[clauseno] += frequency;
      }
    }
  }

  /**
   * Sets the truth values of all atoms in a block except for the one given.
   * 
   * @param atomIdx Index of atom in block exempt from being set to false.
   * @param blockIdx Index of block whose atoms are set to false.
   */
  void setOthersInBlockToFalse(const int& atomIdx,
                               const int& blockIdx)
  {
    Array<int>& block = (*blocks_)[blockIdx];
    for (int i = 0; i < block.size(); i++)
    {
        // Atom not the one specified and not fixed
      if (i != atomIdx && fixedAtom_[block[i] + 1] == 0)
        setValueOfAtom(block[i] + 1, false);
    }
  }


  /**
   * Fixes an atom to a truth value. This means the atom can not change
   * its truth values again. If the atom has already been fixed to the
   * opposite value, then we have a contradiction and the program terminates.
   * 
   * @param atomIdx Index of atom to be fixed.
   * @param value Truth value to which the atom is fixed.
   */
  void fixAtom(const int& atomIdx, const bool& value)
  {
    assert(atomIdx > 0);
      // If already fixed to opp. sense, then contradiction
    if ((fixedAtom_[atomIdx] == 1 && value == false) ||
        (fixedAtom_[atomIdx] == -1 && value == true))
    {
      cout << "Contradiction: Tried to fix atom " << atomIdx <<
      " to true and false ... exiting." << endl;
      exit(0);
    }

    if (vsdebug)
    {
      cout << "Fixing ";
      (*gndPreds_)[atomIdx - 1]->print(cout, domain_);
      cout << " to " << value << endl;
    }
    
    setValueOfAtom(atomIdx, value);
    fixedAtom_[atomIdx] = (value) ? 1 : -1;
  }

  /**
   * Simplifies a clause using atoms which have been fixed. If clause is
   * satisfied from the fixed atoms, this is marked in isSatisfied_ and an
   * empty array is returned. If clause is empty and not satisfied, then a
   * contradiction has occured. Otherwise, the simplified clause is returned.
   * 
   * Returned array should be deleted.
   * 
   * @param clauseIdx Index of the clause to be simplified
   * @return Simplified clause
   */
  Array<int>* simplifyClauseFromFixedAtoms(const int& clauseIdx)
  {
    Array<int>* returnArray = new Array<int>;
      // If already satisfied from fixed atoms, then return empty array
    if (isSatisfied_[clauseIdx]) return returnArray;

      // Keeps track of pos. clause being satisfied or 
      // neg. clause being unsatisfied due to fixed atoms
    bool isGood = (clauseCost_[clauseIdx] > 0) ? false : true;
      // Keeps track of all atoms being fixed to false in a pos. clause
    bool allFalseAtoms = (clauseCost_[clauseIdx] > 0) ? true : false;
      // Check each literal in clause
    for (int i = 0; i < getClauseSize(clauseIdx); i++)
    {
      int lit = clause_[clauseIdx][i];
      int fixedValue = fixedAtom_[abs(lit)];

      if (clauseCost_[clauseIdx] > 0)
      { // Pos. clause: check if clause is satisfied
        if ((fixedValue == 1 && lit > 0) ||
            (fixedValue == -1 && lit < 0))
        { // True fixed lit
          isGood = true;
          allFalseAtoms = false;
          returnArray->clear();
          break;
        }
        else if (fixedValue == 0)
        { // Lit not fixed
          allFalseAtoms = false;
          returnArray->append(lit);
        }
      }
      else
      { // Neg. clause:
        assert(clauseCost_[clauseIdx] < 0);
        if ((fixedValue == 1 && lit > 0) ||
            (fixedValue == -1 && lit < 0))
        { // True fixed lit
          cout << "Contradiction: Tried to fix atom " << abs(lit) <<
          " to true in a negative clause ... exiting." << endl;
          exit(0);
        }
        else
        { // False fixed lit or non-fixed lit
          returnArray->append(lit);
            // Non-fixed lit
          if (fixedValue == 0) isGood = false;          
        }
      }
    }
    if (allFalseAtoms)
    {
      cout << "Contradiction: All atoms in clause " << clauseIdx <<
      " fixed to false ... exiting." << endl;
      exit(0);
    }
    if (isGood) isSatisfied_[clauseIdx] = true;
    return returnArray;
  }

  /**
   * Checks if a clause is dead.
   * 
   * @param clauseIdx Index of clause being checked.
   * @return True, if clause is dead, otherwise false.
   */
  const bool isDeadClause(const int& clauseIdx)
  {
    return deadClause_[clauseIdx];
  }

  /**
   * Marks soft clauses as dead.
   */
  void eliminateSoftClauses()
  {
    bool atLeastOneDead = false;
    for (int i = 0; i < getNumClauses(); i++)
    {
      if (!(*gndClauses_)[i]->isHardClause())
      {
        atLeastOneDead = true;
        deadClause_[i] = true;
      }
    }
    if (atLeastOneDead) initMakeBreakCostWatch();
  }
 
  /**
   * Marks clauses as dead which were not good in the previous iteration of
   * inference or are not picked according to a weighted coin flip. 
   *
   * @param startClause All clauses with index of this or greater are
   * looked at to be killed.
   */
  void killClauses(const int& startClause)
  {
    for (int i = startClause; i < getNumClauses(); i++)
    {
      GroundClause* clause = (*gndClauses_)[i];
      if ((clauseGoodInPrevious(i)) &&
          (clause->isHardClause() || random() <= threshold_[i]))
      {
        if (vsdebug)
        {
          cout << "Keeping clause "<< i << " ";
          clause->print(cout, domain_, &gndPredHashArray_);
          cout << endl;
        }
        deadClause_[i] = false;
      }
      else
      {
        deadClause_[i] = true;
      }
    }
    initMakeBreakCostWatch();
  }

  
  /**
   * Checks if a clause was good in the previous iteration of inference, i.e.
   * if it is positive and satisfied or negative and unsatisfied.
   * 
   * @param clauseIdx Index of clause being checked.
   * @return true, if clause was good, otherwise false.
   */
  const bool clauseGoodInPrevious(const int& clauseIdx)
  {
    //GroundClause* clause = (*gndClauses_)[clauseIdx];
    int numSatLits = numTrueLits_[clauseIdx];
      // Num. of satisfied lits in previous iteration is stored in clause
    if ((numSatLits > 0 && clauseCost_[clauseIdx] > 0.0) ||
        (numSatLits == 0 && clauseCost_[clauseIdx] < 0.0))
      return true;
    else
      return false;
  }

  /**
   * Resets all dead clauses to be alive again.
   */
  void resetDeadClauses()
  {
    for (int i = 0; i < deadClause_.size(); i++)
      deadClause_[i] = false;
    initMakeBreakCostWatch();
  }
   
  /**
   * Resets all fixed atoms to be not fixed again.
   */
  void resetFixedAtoms()
  {
    for (int i = 0; i < fixedAtom_.size(); i++)
      fixedAtom_[i] = 0;
    for (int i = 0; i < isSatisfied_.size(); i++)
      isSatisfied_[i] = false;
  }

  void setLazy(const bool& l) { lazy_ = l; }
  const bool getLazy() { return lazy_; }

  void setUseThreshold(const bool& t) { useThreshold_ = t;}
  const bool getUseThreshold() { return useThreshold_; }
  
  long double getHardWt() { return hardWt_; }
  
  const Domain* getDomain() { return domain_; }

  const MLN* getMLN() { return mln_; }

  /**
   * Prints the best state found to a stream.
   * 
   * @param out Stream to which the state is printed.
   */
  void printLowState(ostream& out)
  {
    for (int i = 0; i < getNumAtoms(); i++)
    {
      (*gndPreds_)[i]->print(out, domain_);
      out << " " << lowAtom_[i + 1] << endl;
    }
  }

  /**
   * Prints a ground predicate to a stream.
   * 
   * @param predIndex Index of predicate to be printed.
   * @param out Stream to which predicate is printed.
   */
  void printGndPred(const int& predIndex, ostream& out)
  {
    (*gndPreds_)[predIndex]->print(out, domain_);
  }

  /**
   * Sets a GroundPredicate to be evidence and sets its truth value. If it is
   * already present as evidence with the given truth value, then nothing
   * happens. If the predicate was a query, then additional clauses may be
   * eliminated. reinit() should be called after this in order to ensure that
   * the clause and atom information is correct.
   * 
   * @param predicate GroundPredicate to be set as evidence.
   * @param trueEvidence The truth value of the predicate is set to this.
   */
  void setAsEvidence(const GroundPredicate* const & predicate,
                     const bool& trueEvidence)
  {
    if (vsdebug)
    {
      cout << "Setting to evidence " ;
      predicate->print(cout, domain_);
      cout << endl;
    }
    Database* db = domain_->getDB();
    int atomIdx = gndPredHashArray_.find((GroundPredicate*)predicate);
      // If already evidence, then check its truth value
    if (atomIdx <= 0)
    {
        // If predicate already evidence with same truth value, then do nothing
      if (db->getValue(predicate) == trueEvidence)
        return;
        
        // Changing truth value of evidence
      if (trueEvidence)
        db->setValue(predicate, TRUE);
      else
        db->setValue(predicate, FALSE);
    }
    else
    {
      Array<int> gndClauseIndexes;      
      gndClauseIndexes = getNegOccurenceArray(atomIdx + 1);
      for (int i = 0; i < gndClauseIndexes.size(); i++)
      {
          // Atom appears neg. in these clauses, so remove the atom from
          // these clauses if true evidence, or remove clause if false evidence
          // or a unit clause
        if (!trueEvidence ||
            (*gndClauses_)[gndClauseIndexes[i]]->getNumGroundPredicates() == 1)
        {          
          if (vsdebug)
            cout << "Deleting ground clause " << gndClauseIndexes[i] << endl;
          delete (*gndClauses_)[gndClauseIndexes[i]];
          (*gndClauses_)[gndClauseIndexes[i]] = NULL;
        }
        else
        {
          if (vsdebug)
          {
            cout << "Removing gnd pred " << -(atomIdx + 1)
                 << " from ground clause " << gndClauseIndexes[i] << endl;
          }
          (*gndClauses_)[gndClauseIndexes[i]]->removeGndPred(-(atomIdx + 1));
        }
      }

      gndClauseIndexes = getPosOccurenceArray(atomIdx + 1);
      for (int i = 0; i < gndClauseIndexes.size(); i++)
      {
          // Atom appears pos. in these clauses, so remove the atom from
          // these clauses if false evidence, or remove clause if true evidence
          // or a unit clause
        if (trueEvidence ||
            (*gndClauses_)[gndClauseIndexes[i]]->getNumGroundPredicates() == 1)
        {
          if (vsdebug)
            cout << "Deleting ground clause " << gndClauseIndexes[i] << endl;
          delete (*gndClauses_)[gndClauseIndexes[i]];
          (*gndClauses_)[gndClauseIndexes[i]] = NULL;
        }
        else
        {
          if (vsdebug)
          {
            cout << "Removing gnd pred " << -(atomIdx + 1)
                 << " from ground clause " << gndClauseIndexes[i] << endl;
          }
          (*gndClauses_)[gndClauseIndexes[i]]->removeGndPred(atomIdx + 1);
        }
      }
      
      gndPredHashArray_.removeItemFastDisorder(atomIdx);
      gndPredHashArray_.compress();
      gndPreds_->removeItemFastDisorder(atomIdx);
      gndPreds_->compress();
        // By removing a pred, the pred at the end of the array gets the
        // index of the pred deleted, so we have to update to the new index
        // in all clauses
      int oldIdx = gndPredHashArray_.size();
      replaceAtomIndexInAllClauses(oldIdx, atomIdx);      
      gndClauses_->removeAllNull();
    }
  }

  /**
   * Sets a GroundPredicate to be query. If it is already present as query,
   * then nothing happens. If the predicate was evidence, then additional
   * clauses may be added. reinit() should be called after this in order to
   * ensure that the clause and atom information is correct.
   * 
   * @param predicate GroundPredicate to be set as a query.
   */
  void setAsQuery(const GroundPredicate* const & predicate)
  {
    if (vsdebug)
    {
      cout << "Setting to query " ;
      predicate->print(cout, domain_);
      cout << endl;
    }
    Database* db = domain_->getDB();
      // If already non-evidence, then do nothing
    if (gndPredHashArray_.contains((GroundPredicate*)predicate))
      return;
    else
    {
        // Evidence -> query
        // Add predicate to query set and get clauses
      gndPredHashArray_.append((GroundPredicate*)predicate);
      Predicate* p = predicate->createEquivalentPredicate(domain_);
      db->setEvidenceStatus(p, false);
      bool ignoreActivePreds = true;
      getActiveClauses(p, newClauses_, true, ignoreActivePreds);
    }
  }

  ////////////// BEGIN: MCMC Functions //////////////
  
  /**
   * Gets a pointer to a GroundPredicate.
   * 
   * @param index Index of the GroundPredicate to be retrieved.
   * @return Pointer to the GroundPredicate 
   */
  GroundPredicate* getGndPred(const int& index)
  {
    return (*gndPreds_)[index];
  }

  /**
   * Gets a pointer to a GroundClause.
   * 
   * @param index Index of the GroundClause to be retrieved.
   * @return Pointer to the GroundClause 
   */
  GroundClause* getGndClause(const int& index)
  {
    return (*gndClauses_)[index];
  }

  /**
   * The atom assignment in the best state is saved to the ground predicates.
   */
  void saveLowStateToGndPreds()
  {
    for (int i = 0; i < getNumAtoms(); i++)
      (*gndPreds_)[i]->setTruthValue(lowAtom_[i + 1]);
  }

  /**
   * The atom assignment in the best state is saved to the database.
   */
  void saveLowStateToDB()
  {
    for (int i = 0; i < getNumAtoms(); i++)
    {
      GroundPredicate* p = gndPredHashArray_[i];
      bool value = lowAtom_[i + 1];
      if (value)
      {
        domain_->getDB()->setValue(p, TRUE);
      }
      else
      {
        domain_->getDB()->setValue(p, FALSE);
      }
    }
  }

  /**
   * Gets the index of a GroundPredicate in this state.
   * 
   * @param gndPred GroundPredicate whose index is being found.
   * @return Index of the GroundPredicate, if found; otherwise -1.
   */
  const int getGndPredIndex(GroundPredicate* const& gndPred)
  {
    return gndPreds_->find(gndPred);
  }

     
  ////////////// END: MCMC Functions //////////////


  ////////////// BEGIN: Lazy Functions //////////////
 
  /** 
   * Gets clauses and weights activated by the predicate inputPred,
   * if active is true. If false, inactive clauses (and their weights)
   * containing inputPred are retrieved. If inputPred is NULL, then all
   * active (or inactive) clauses and their weights are retrieved.
   * 
   * @param inputPred Only clauses containing this Predicate are looked at.
   * If NULL, then all active clauses are retrieved.
   * @param activeClauses New active clauses are put here.
   * @param active If true, active clauses are retrieved, otherwise inactive.
   * @param ignoreActivePreds If true, active preds are not taken into account.
   * This results in the retrieval of all unsatisfied clauses.
   */
  void getActiveClauses(Predicate *inputPred,
                        Array<GroundClause*>& activeClauses,
                        bool const & active,
                        bool const & ignoreActivePreds)
  {
    Clause *fclause;
    GroundClause* newClause;
    int clauseCnt;
    GroundClauseHashArray clauseHashArray;

    Array<GroundClause*>* newClauses = new Array<GroundClause*>; 
  
    const Array<IndexClause*>* indexClauses = NULL;
      
      // inputPred is null: all active clauses should be retrieved
    if (inputPred == NULL)
    {
      clauseCnt = mln_->getNumClauses();
    }
      // Otherwise, look at all first order clauses containing the pred
    else
    {
      if (domain_->getDB()->getDeactivatedStatus(inputPred)) return;
      int predId = inputPred->getId();
      indexClauses = mln_->getClausesContainingPred(predId);
      clauseCnt = indexClauses->size();
    }

      // Look at each first-order clause and get active groundings
    int clauseno = 0;
    while (clauseno < clauseCnt)
    {
      if (inputPred)
        fclause = (Clause *) (*indexClauses)[clauseno]->clause;           
      else
        fclause = (Clause *) mln_->getClause(clauseno);

      if (vsdebug)
      {
        cout << "Getting active clauses for FO clause: ";
        fclause->print(cout, domain_);
        cout << endl;
      }
      
      long double wt = fclause->getWt();
      const double* parentWtPtr = NULL;
      if (!fclause->isHardClause()) parentWtPtr = fclause->getWtPtr();
      const int clauseId = mln_->findClauseIdx(fclause);
      newClauses->clear();

      fclause->getActiveClauses(inputPred, domain_, newClauses,
                                &gndPredHashArray_, ignoreActivePreds);

      for (int i = 0; i < newClauses->size(); i++)
      {
        newClause = (*newClauses)[i];
        int pos = clauseHashArray.find(newClause);
          // If clause already present, then just add weight
        if (pos >= 0)
        {
          if (vsdebug)
          {
            cout << "Adding weight " << wt << " to clause ";
            clauseHashArray[pos]->print(cout, domain_, &gndPredHashArray_);
            cout << endl;
          }
          clauseHashArray[pos]->addWt(wt);
          if (parentWtPtr)
          {
            clauseHashArray[pos]->appendParentWtPtr(parentWtPtr);
            clauseHashArray[pos]->incrementClauseFrequency(clauseId, 1);
          }
          delete newClause;
          continue;
        }

          // If here, then clause is not yet present        
        newClause->setWt(wt);
        newClause->appendToGndPreds(&gndPredHashArray_);
        if (parentWtPtr)
        {
          newClause->appendParentWtPtr(parentWtPtr);
          newClause->incrementClauseFrequency(clauseId, 1);
          assert(newClause->getWt() == *parentWtPtr);
        }      

        if (vsdebug)
        {
          cout << "Appending clause ";
          newClause->print(cout, domain_, &gndPredHashArray_);
          cout << endl;
        }
        clauseHashArray.append(newClause);
      }
      clauseno++; 
    } //while (clauseno < clauseCnt)

    for (int i = 0; i < clauseHashArray.size(); i++)
    {
      newClause = clauseHashArray[i];
      activeClauses.append(newClause);
    }
    delete newClauses;
  }

  /**
   * Get all the active clauses in the database.
   * 
   * @param allClauses Active clauses are retrieved into this Array.
   * @param ignoreActivePreds If true, active preds are ignored; this means
   * all unsatisfied clauses are retrieved.
   */
  void getActiveClauses(Array<GroundClause*> &allClauses,
                        bool const & ignoreActivePreds)
  {
    getActiveClauses(NULL, allClauses, true, ignoreActivePreds);
  }
  
  int getNumActiveAtoms()
  {
    return activeAtoms_; 
  }

  /**
   * Selects one atom in each block in the domain and adds it to the block
   * here and sets it to true.
   */
  void addOneAtomToEachBlock()
  {
    assert(lazy_);
      // For each block: select one to set to true
    for (int i = 0; i < blocks_->size(); i++)
    {
        // If evidence atom exists, then all others are false
      if ((*blockEvidence_)[i])
      {
          // If first argument is -1, then all are set to false
        setOthersInBlockToFalse(-1, i);
        continue;
      }

        // Assumption is initLazyBlocks has been called
        // Pick one ground pred from the block in the domain
      const Array<Predicate*>* block = domain_->getPredBlock(i);

      int chosen = random() % block->size();
      Predicate* pred = (*block)[chosen];
      GroundPredicate* groundPred = new GroundPredicate(pred);

        // If chosen pred is not yet present, then add it, otherwise delete it
      int index = gndPredHashArray_.find(groundPred);
      if (index < 0)
      {
          // Pred not yet present
        index = gndPredHashArray_.append(groundPred);
        (*blocks_)[i].append(index);
        chosen = (*blocks_)[i].size() - 1;
          // addNewClauses adds the predicate to the state and updates
          // info arrays
        bool initial = false;
        addNewClauses(initial);
      }
      else
      {
        delete groundPred;
        chosen = (*blocks_)[i].find(index);
      }
      setValueOfAtom(index + 1, true);
      setOthersInBlockToFalse(chosen, i);
    }
  }

  /**
   * Initializes the block structures for the lazy version.
   */
  void initLazyBlocks()
  {
    assert(lazy_);
    blocks_ = new Array<Array<int> >;
    blocks_->growToSize(domain_->getNumPredBlocks());
    blockEvidence_ = new Array<bool>(*(domain_->getBlockEvidenceArray()));
  }

  /**
   * Fills the blocks with the predicates in the domain blocks.
   */
  void fillLazyBlocks()
  {
    assert(lazy_);
    const Array<Array<Predicate*>*>* blocks = domain_->getPredBlocks();
    for (int i = 0; i < blocks->size(); i++)
    {
      if (vsdebug) cout << "Block " << i << endl;
      Array<Predicate*>* block = (*blocks)[i];
      for (int j = 0; j < block->size(); j++)
      {
        Predicate* pred = (*block)[j];
        if (vsdebug)
        {
          cout << "\tPred: ";
          pred->printWithStrVar(cout, domain_);
          cout << endl;
        }
          // Add all non-evid preds in blocks to the state
        if (domain_->getDB()->getEvidenceStatus(pred))
          continue;
        GroundPredicate* groundPred = new GroundPredicate(pred);

          // Add pred if not yet present, otherwise delete it
        int index = gndPredHashArray_.find(groundPred);
        if (index < 0)
          index = gndPredHashArray_.append(groundPred);
        else
          delete groundPred;

          // Append the atom to the block if not yet there
        if (!(*blocks_)[i].contains(index))
          (*blocks_)[i].append(index);
      }
    }
      // addNewClauses adds the predicates to the state and updates info arrays
    bool initial = true;
    addNewClauses(initial);
  }

  ////////////// END: Lazy Functions //////////////

  
 private:
 
  /**
   * Sets the hard clause weight to the sum of all soft clause weights in
   * the domain (not in the MRF) plus a constant (10). This is then the hard
   * clause weight when no soft clauses are present.
   */
  void setHardClauseWeight()
  {
      // Soft weights are summed up to determine hard weight
    long double sumSoftWts = 0.0;
      // Determine hard clause weight
    int clauseCnt = mln_->getNumClauses();    
      // Sum up the soft weights of all grounded clauses
    for (int i = 0; i < clauseCnt; i++)
    {
      Clause* fclause = (Clause *) mln_->getClause(i);
        // Skip hard clauses
      if (fclause->isHardClause()) continue;
        // Weight could be negative
      long double wt = abs(fclause->getWt());
      long double numGndings = fclause->getNumGroundings(domain_);
      sumSoftWts += wt*numGndings;
    }
    assert(sumSoftWts >= 0);
      // Add constant so weight isn't zero if no soft clauses present
    hardWt_ = sumSoftWts + 10.0;
    cout << "Set hard weight to " << hardWt_ << endl;
  }

  /**
   * Replaces the old index of an atom with a new one in all ground clauses
   * in which the old index occurs.
   * 
   * @param oldIdx Old index of atom.
   * @param newIdx New index of atom.
   */
  void replaceAtomIndexInAllClauses(const int& oldIdx, const int& newIdx)
  {
    Array<int> gndClauseIndexes;

    gndClauseIndexes = getNegOccurenceArray(oldIdx + 1);
    for (int i = 0; i < gndClauseIndexes.size(); i++)
    {
        // Atom appears neg. in these clauses, so it appears as -(oldIdx + 1)
      if ((*gndClauses_)[gndClauseIndexes[i]])
        (*gndClauses_)[gndClauseIndexes[i]]->changeGndPredIndex(-(oldIdx + 1),
                                                                -(newIdx + 1));
    }

    gndClauseIndexes = getPosOccurenceArray(oldIdx + 1);
    for (int i = 0; i < gndClauseIndexes.size(); i++)
    {
        // Atom appears pos. in these clauses, so it appears as (oldIdx + 1)
      if ((*gndClauses_)[gndClauseIndexes[i]])
        (*gndClauses_)[gndClauseIndexes[i]]->changeGndPredIndex(oldIdx + 1,
                                                                newIdx + 1);
    }
  }

 private:

    // If true, this is a lazy variable state, else eager.
  bool lazy_;

    // Weight used for hard clauses (sum of soft weights + constant)
  long double hardWt_;
  
    // mln and domain are used to build MRF in eager state and to
    // retrieve active atoms in lazy state.
  MLN* mln_;
  Domain* domain_;

    // Eager version: Pointer to gndPreds_ and gndClauses_ in MRF
    // Lazy version: Holds active atoms and clauses
  Array<GroundPredicate*>* gndPreds_;
  Array<GroundClause*>* gndClauses_;
  
    // Predicates corresponding to the groundings of the unknown non-evidence
    // predicates
  GroundPredicateHashArray* unePreds_;

    // Predicates corresponding to the groundings of the known non-evidence
    // predicates
  GroundPredicateHashArray* knePreds_;
    // Actual truth values of ground known non-evidence preds
  Array<TruthValue>* knePredValues_;  
  

    ////////// BEGIN: Information used only by lazy version //////////
    // Number of distinct atoms in the first set of unsatisfied clauses
  int baseNumAtoms_;
    // If true, atoms are not deactivated when mem. is full
  bool noApprox_;
    // Indicates whether deactivation of atoms has taken place yet
  bool haveDeactivated_;
    // Max. amount of memory to use
  int memLimit_;
    // Max. amount of clauses memory can hold
  int clauseLimit_;
    ////////// END: Information used only by lazy version //////////


    ////////// BEGIN: Information used only by eager version //////////
    // MRF is used with eager states. If lazy, this stays NULL.
  MRF* mrf_;
    ////////// END: Information used only by eager version //////////

    // Holds the new active clauses
  Array<GroundClause*> newClauses_;
    // Holds the new gnd preds
  Array<GroundPredicate*> newPreds_;
    // Holds the ground predicates in a hash array.
    // Fast access is needed for comparing preds when activating clauses.
  GroundPredicateHashArray gndPredHashArray_;

    // Clauses to be satisfied
    // Indexed as clause_[clause_num][literal_num]
  Array<Array<int> > clause_;
    // Cost of each clause (can be negative)
  Array<long double> clauseCost_;
    // Highest cost of false clause
  long double highestCost_;
    // If true, more than one clause has highest cost
  bool eqHighest_;
    // Number of clauses with highest cost
  int numHighest_;
    // Clauses which are pos. and unsatisfied or neg. and satisfied
  Array<int> falseClause_;
    // Where each clause is listed in falseClause_
  Array<int> whereFalse_;
    // Number of true literals in each clause
  Array<int> numTrueLits_;
    // watch1_[c] contains the id of the first atom which c is watching
  Array<int> watch1_;
    // watch2_[c] contains the id of the second atom which c is watching
  Array<int> watch2_;
    // Which clauses are satisfied by fixed atoms
  Array<bool> isSatisfied_;
    // Clauses which are not to be considered
  Array<bool> deadClause_;
    // Use threshold to exclude clauses from the state?
  bool useThreshold_;
    // Pre-computed thresholds for each clause
  Array<long double> threshold_;

    // Holds the index of clauses in which each literal occurs
    // Indexed as occurence_[2*abs(lit) - (lit > 0)][occurence_num]
  Array<Array<int> > occurence_;

    // Current assigment of atoms
  Array<bool> atom_;
    // Cost of clauses which would become satisfied by flipping each atom
  Array<long double> makeCost_;
    // Cost of clauses which would become unsatisfied by flipping each atom
  Array<long double> breakCost_;
    // Indicates if an atom is fixed to a value (0 = not fixed, -1 = false,
    // 1 = true)
  Array<int> fixedAtom_;

    // Assigment of atoms producing lowest cost so far
  Array<bool> lowAtom_;
    // Cost of false clauses in the currently best state
  long double lowCost_;
    // Number of false clauses in the currently best state
  int lowBad_;

    // Current no. of unsatisfied clauses
  int numFalseClauses_;
    // Cost associated with the number of false clauses
  long double costOfFalseClauses_;
  
    // For block inference: blocks_ and blockEvidence_
    // All atom indices in (*blocks_)[i] are to be treated as in one block
  Array<Array<int> >* blocks_;
    // (*blockEvidence_)[i] states whether block i has true evidence and
    // thus all should be false
  Array<bool >* blockEvidence_;

    // Number of active atoms in state.  
  int activeAtoms_;
  
};

#endif /*VARIABLESTATE_H_*/
