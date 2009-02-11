/*
 * All of the documentation and software included in the
 * Alchemy Software is copyrighted by Stanley Kok, Parag
 * Singla, Matthew Richardson, Pedro Domingos, Marc
 * Sumner, Hoifung Poon, and Daniel Lowd.
 * 
 * Copyright [2004-08] Stanley Kok, Parag Singla, Matthew
 * Richardson, Pedro Domingos, Marc Sumner, Hoifung
 * Poon, and Daniel Lowd. All rights reserved.
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
 * Richardson, Pedro Domingos, Marc Sumner, Hoifung
 * Poon, and Daniel Lowd in the Department of Computer Science and
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
#ifndef BP_H_
#define BP_H_

#include "inference.h"
#include "bpparams.h"
#include "twowaymessage.h"
#include "superclause.h"
#include "bpfactor.h"
#include "bpnode.h"

const int bpdebug = false;

/**
 * Class for belief propagation algorithm.
 */
class BP : public Inference
{
 public:

  /**
   * Constructor: Constructs unit propagation and SampleSat.
   */
  BP(VariableState* state, long int seed, const bool& trackClauseTrueCnts,
     BPParams* bpParams, MLN* mln, Domain* domain)
    : Inference(state, seed, trackClauseTrueCnts)
  {
    Timer timer1;
    
    maxSteps_ = bpParams->maxSteps;
    maxSeconds_ = bpParams->maxSeconds;
    lifted_ = bpParams->lifted;
    convergenceThresh_ = bpParams->convergenceThresh;
    convergeRequiredItrCnt_ = bpParams->convergeRequiredItrCnt;
    implicitRep_ = bpParams->implicitRep;
    
    lidToTWMsg_ = new LinkIdToTwoWayMessageMap();
    superClausesArr_ = new Array<Array<SuperClause*>*>();
    factors_ = new Array<BPFactor*>();
    nodes_ = new Array<BPNode*>();
    mln_ = mln;
    domain_ = domain;
  }

  /**
   * Destructor.
   */
  ~BP()
  {
    delete lidToTWMsg_;
    delete superClausesArr_;
    delete factors_;
    delete nodes_;
  }
  
  /**
   * Initializes belief propagation.
   */
  void init()
  {
    Timer timer1;
    cout << "Initializing ";
    if (lifted_) cout << "Lifted ";
    cout << "Belief Propagation..." << endl;

    if (lifted_)
    {
      createSuper();
      createSuperNetwork();
    }
    else
    {
      createGround();
      createGroundNetwork();
    }
    
    if (bpdebug)
    {
      cout << "[BP.init] ";
      Timer::printTime(cout, timer1.time());
      cout << endl;
      timer1.reset();
    }
  }

  /**
   * Performs Belief Propagation inference.
   */
  void infer()
  {
    Timer timer1;

    double oldProbs[2];
    double newProbs[2];
    double diff;
    double maxDiff;
    int maxDiffNodeIndex;
    int convergeItrCnt = 0;
    bool converged = false;

    cout<<"factorcnt = "<<factors_->size()<<", nodecnt = "<<nodes_->size()<<endl;

    if (bpdebug)
    {
      cout << "factors:" << endl;
      for (int i = 0; i < factors_->size(); i++)
      {
        (*factors_)[i]->print(cout);
        cout << endl;
      }
      cout << "nodes:" << endl;
      for (int i = 0; i < nodes_->size(); i++)
      {
        (*nodes_)[i]->print(cout);
        cout << endl;
      }
    }
      // Pass around (send) the messages
    int itr;
    
      // Move to next step to transfer the message in the nextMsgsArr in the
      // beginning to the the msgsArr 
    for (itr = 1; itr <= maxSteps_; itr++)
    {
      if (bpdebug)
      {
        cout<<"*************************************"<<endl;
        cout<<"Performing Iteration "<<itr<<" of BP"<<endl;
        cout<<"*************************************"<<endl;
      }

      for (int i = 0; i < factors_->size(); i++)
      {
        if (bpdebug)
        {
          cout << "Sending messages for Factor: ";
          (*factors_)[i]->print(cout); cout << endl;
        }
        (*factors_)[i]->sendMessage();
      }
           
      for (int i = 0; i < nodes_->size(); i++)
      {
        if (bpdebug)
        {
          cout << "Sending messages for Node: ";
          (*nodes_)[i]->print(cout); cout << endl;
        }
        (*nodes_)[i]->sendMessage();
      }

      for (int i = 0; i < factors_->size(); i++)
      {
        (*factors_)[i]->moveToNextStep();
        if (bpdebug)
        {
          cout << "BP-Factor Iteration " << itr << " => ";
          (*factors_)[i]->print(cout); cout << endl;
        }
      }
          
      maxDiff = -1;
      maxDiffNodeIndex = -1;
      for (int i = 0; i < nodes_->size(); i++)
      {
        if (bpdebug)
        {
          cout<<"************************************"<<endl;
          cout<<"Node "<<i<<":"<<endl;
          cout<<"************************************"<<endl;
          cout<<"Getting Old Probabilities =>"<<endl; 
          cout<<endl;
          cout<<"Moving to next step "<<endl;
          cout<<endl;
          cout<<"Getting New Probabilities =>"<<endl; 
        }

        (*nodes_)[i]->getProbs(oldProbs);
        (*nodes_)[i]->moveToNextStep();
        (*nodes_)[i]->getProbs(newProbs);

        diff = abs(newProbs[1] - oldProbs[1]);

        if (bpdebug)
        {
          cout<<endl<<endl<<"Final Probs : "<<endl;
          cout<<"Node "<<i<<": probs["<<0<<"] = "<<newProbs[0]
              <<", probs["<<1<<"] = "<<newProbs[1]<<endl;
          cout<<"BP-Node Iteration "<<itr<<": "<<newProbs[0]
              <<"  probs["<<1<<"] = "<<newProbs[1]<<endl;
          cout<<" : => ";
          (*nodes_)[i]->print(cout)<<endl;
        }
        
        if (maxDiff < diff)
        {
          maxDiff = diff;
          maxDiffNodeIndex = i;
        }
      }
           
      cout<<"At Iteration "<<itr<<": MaxDiff = "<<maxDiff<<endl;
      cout<<endl;
           
        //check if BP has converged
      if (maxDiff < convergenceThresh_)
        convergeItrCnt++;
      else
        convergeItrCnt = 0;

        // Check if for N continuous iterations, maxDiff has been below the
        // threshold
      if (convergeItrCnt >= convergeRequiredItrCnt_)
      {
        converged = true;
        break;    
      }
    }
      
    if (converged)
    {
      cout << "Converged in " << itr << " Iterations " << endl;
    }
    else
    {
      cout << "Did not converge in " << maxSteps_ << " (max allowed) Iterations"
           << endl;
    }
  }

  /**
   * Prints the probabilities of each predicate to a stream.
   */
  void printProbabilities(ostream& out)
  {
    double probs[2];
    Array<int>* constants;
    Predicate* pred;
    int predId;
    BPNode* node;
    double exp;
    Domain* domain = domain_;
    for (int i = 0; i < nodes_->size(); i++)
    { 
      node = (*nodes_)[i];
      predId = node->getPredId();
      node->getProbs(probs);         
      exp = node->getExp();
      SuperPred * superPred = node->getSuperPred();

      if (superPred)
      {        
        for (int index = 0; index < superPred->getNumTuples(); index++)
        {
          constants = superPred->getConstantTuple(index);
          pred = domain->getPredicate(constants, predId);
          pred->printWithStrVar(out, domain);
          out << " " << probs[1] << endl;
          //out<<" "<<exp<<endl;
        }
      }
      else
      {
        constants = node->getConstants(); 
        assert(constants != NULL);
        pred = domain->getPredicate(constants, predId);
        pred->printWithStrVar(out, domain);
        out << " " << probs[1] << endl;
        //out<<" "<<exp<<endl;
      }
    }
  }

  /**
   * Puts the predicates whose probability has changed with respect to the
   * reference vector oldProbs by more than probDelta in string form and the
   * corresponding probabilities of each predicate in two vectors.
   * 
   * @param changedPreds Predicates whose probability have changed more than
   * probDelta are put here.
   * @param probs The probabilities corresponding to the predicates in
   * changedPreds are put here.
   * @param oldProbs Reference probabilities for checking for changes.
   * @param probDelta If probability of an atom has changed more than this
   * value, then it is considered to have changed.
   */
  void getChangedPreds(vector<string>& changedPreds, vector<float>& probs,
                       vector<float>& oldProbs, const float& probDelta)
  {
  }

  /**
   * Gets the probability of a ground predicate.
   * 
   * @param gndPred GroundPredicate whose probability is being retrieved.
   * @return Probability of gndPred if present in state, otherwise 0.
   */
  double getProbability(GroundPredicate* const& gndPred)
  {
    return 0.0;
  }

  /**
   * Prints each predicate with a probability of 0.5 or greater to a stream.
   */
  void printTruePreds(ostream& out)
  {
  }
  
  LinkIdToTwoWayMessageMap* getLinkIdToTwoWayMessageMap()
  {
    return lidToTWMsg_;
  }


 private:
 
 /**
  * Ground away the evidence from each clause. Then, create one 
  * superclause for each ground clause
  */
  void createGround()
  {
    MLN* mln = mln_;
    Domain* domain = domain_;
    Clause* mlnClause;

      //clause with all variables in it
    Clause *varClause;

    Array<Clause *> allClauses;
    Array<int> *mlnClauseTermIds;
  
    ClauseToSuperClauseMap *clauseToSuperClause;
    ClauseToSuperClauseMap::iterator clauseItr;
    int numClauses = mln->getNumClauses();
  
    PredicateTermToVariable *ptermToVar = NULL;
  
    double gndtime;
    Util::elapsed_seconds();
  
    clauseToSuperClause = new ClauseToSuperClauseMap();
    for (int i = 0; i < numClauses; i++)
    {
        //remove the unknown predicates
      mlnClause = (Clause*) mln->getClause(i);
      varClause = new Clause(*mlnClause);
      mlnClauseTermIds = varClause->updateToVarClause();
     
      mlnClause->getConstantTuples(domain, domain->getDB(), mlnClauseTermIds, 
                                   varClause, ptermToVar, clauseToSuperClause,
                                   false);

        //can delete the var Clause now
      delete varClause;
    }

    clauseToSuperClause = mergeSuperClauses(clauseToSuperClause);
    addSuperClauses(clauseToSuperClause);
  
    gndtime = Util::elapsed_seconds();

      //create the super preds  
    int totalGndClauseCnt = 0;
    for (int i = 0; i < superClausesArr_->size(); i++)
    {
      SuperClause *superClause = (*(*superClausesArr_)[i])[0];
      int gndCnt = superClause->getNumTuplesIncludingImplicit();
      totalGndClauseCnt += gndCnt;
    }
    cout<<"Total Number of Ground Clauses = "<<totalGndClauseCnt<<endl;
  }
 
 
  /**
   * Creates the Ground BP network
   */
  void createGroundNetwork()
  {
    Domain* domain = domain_;
    int domainPredCnt = domain->getNumPredicates();
    Array<IntArrayToIntMap*>* pconstantsToNodeIndexArr =
      new Array<IntArrayToIntMap*>();
    pconstantsToNodeIndexArr->growToSize(domainPredCnt);
     
      // Initialize the mappings from pred constants to the nodeIndices
    for(int i = 0; i < domainPredCnt; i++)
    {
      (*pconstantsToNodeIndexArr)[i] = new IntArrayToIntMap();
    }
     
    IntArrayToIntMap *pconstantsToNodeIndex;
    IntArrayToIntMap::iterator itr;
     
    Array<SuperClause*>* superClauses;
    SuperClause* superClause;

    Clause *clause;
    Predicate *pred;
    Array<int>* constants;
    Array<int>* pconstants;
    
    BPFactor* factor;
    BPNode* node;

    for (int arrIndex = 0; arrIndex < superClausesArr_->size(); arrIndex++)
    {
      superClauses = (*superClausesArr_)[arrIndex];
      for (int scindex = 0; scindex < superClauses->size(); scindex++)
      {
        superClause = (*superClauses)[scindex];
        clause = superClause->getClause();
        int numTuples = superClause->getNumTuples();

        for (int tindex = 0; tindex < numTuples; tindex++)
        {
            // Number of times this tuple appears in the superclause
            // (because of counting elimination)
          double tcnt = superClause->getTupleCount(tindex);
          constants = superClause->getConstantTuple(tindex);

          factor = new BPFactor(clause, NULL, constants, domain);     
          factors_->append(factor);
          for (int pindex = 0; pindex < clause->getNumPredicates(); pindex++)
          {
            pred = clause->getPredicate(pindex);
            pconstants = pred->getPredicateConstants(constants);
            
            int predId = domain->getPredicateId(pred->getName());
            pconstantsToNodeIndex = (*pconstantsToNodeIndexArr)[predId];
            itr = pconstantsToNodeIndex->find(pconstants);
                 
            if (itr == pconstantsToNodeIndex->end())
            {
              int nodeIndex = nodes_->size();
              (*pconstantsToNodeIndex)[pconstants] = nodeIndex;
                    
              node = new BPNode(predId, NULL, pconstants, domain);
              nodes_->append(node);
            }
            else
            {
              delete pconstants;
              int nodeIndex = itr->second;
              node = (*nodes_)[nodeIndex];
            }

              // Now, add the links to the bpnode/bpfactor
            int reverseNodeIndex = factor->getNumLinks();
              // Index where this factor would be stored in the list of nodes
            int reverseFactorIndex = node->getNumLinks();
            BPLink * link = new BPLink(node, factor, reverseNodeIndex,
                                       reverseFactorIndex, pindex, tcnt);
            node->addLink(link, NULL);
            factor->addLink(link, NULL);
          }
        }
      }
    }
     
      // clean up
    for (int predId = 0; predId < domainPredCnt; predId++)
    {
      pconstantsToNodeIndex = (*pconstantsToNodeIndexArr)[predId];
      pconstantsToNodeIndex->clear();
      delete pconstantsToNodeIndex;
    }
    delete pconstantsToNodeIndexArr;
  }

    //create the super clause/predicate network
  void createSuper()
  {
    MLN* mln = mln_;
    Domain* domain = domain_;
    Clause* mlnClause;

      //clause with all variables in it
    Clause *varClause;
    Array<Clause *> allClauses;
    Array<int> *mlnClauseTermIds;
       
    ClauseToSuperClauseMap *clauseToSuperClause;
    ClauseToSuperClauseMap::iterator clauseItr;

    int numClauses = mln->getNumClauses();
       
    PredicateTermToVariable *ptermToVar = NULL;
    double gndtime, bptime, setuptime;
    Util::elapsed_seconds();

    if (implicitRep_)
    {
      ptermToVar = getPredicateTermToVariableMap(mln, domain);
      getIndexedConstants(ptermToVar, mln, domain);
    }
      
    clauseToSuperClause = new ClauseToSuperClauseMap();
    for(int i=0;i<numClauses;i++)
    {
        //remove the unknown predicates
      mlnClause = (Clause*) mln->getClause(i);
      varClause = new Clause(*mlnClause);
      mlnClauseTermIds = varClause->updateToVarClause();
          
      mlnClause->getConstantTuples(domain, domain->getDB(), mlnClauseTermIds, 
                                   varClause, ptermToVar, clauseToSuperClause,
                                   implicitRep_);
          
        //can delete the var Clause now
      delete varClause;
    }
      
    clauseToSuperClause = mergeSuperClauses(clauseToSuperClause);
    addSuperClauses(clauseToSuperClause);

    gndtime = Util::elapsed_seconds();

    cout << endl << endl;
    cout << "*****************************************************************"
         << endl << endl;
    cout << "Now, starting the iterations of creating supernodes/superfeatures"
         << endl;
      //now create the super preds corresponding to the current set of
      //superclauses

    SuperClause *superClause;
    int totalTupleCnt = 0;
    int totalGndTupleCnt = 0;

    cout << "Counts in the beginning:" << endl;
    for (int i = 0; i < superClausesArr_->size(); i++)
    {
      superClause = (*(*superClausesArr_)[i])[0];
         
      int cnt = superClause->getNumTuples();
      int gndCnt = superClause->getNumTuplesIncludingImplicit();

      totalTupleCnt += cnt;
      totalGndTupleCnt += gndCnt;
    }
    cout << "Total Number of Ground Tuples = " << totalGndTupleCnt << endl;
    cout << "Total Number of Tuples Created = " << totalTupleCnt << endl;

    /*************************************************************************
     * Start the Iterations now */
    /*************************************************************************/
       
    int newSuperClauseCnt = getNumArrayArrayElements(*superClausesArr_);
    int superClauseCnt = newSuperClauseCnt;
    int itr = 1;
    cout << "********************************************************"
         << endl << endl;
    setuptime = 0;
    bptime = 0;
       
      // For creation of the Belief Propagation Network
    Array<BPFactor *> * factors = new Array<BPFactor *>();
    Array<BPNode *> * nodes = new Array<BPNode *>();

    while (newSuperClauseCnt != superClauseCnt || itr <= 2)
    {
      superClauseCnt = newSuperClauseCnt;
      cout << "***************************************************************"
           << endl;
      cout << "Iteration: " << itr << endl;
           
        //for iteration 1, superclauses have already been created
      if (itr > 1)
      {
        cout << "Creating Super Clauses.. " << endl;
        createSuperClauses(superClausesArr_, domain);
        newSuperClauseCnt = getNumArrayArrayElements(*superClausesArr_);
      }
       
      cout << "Creating New Super Preds.. " << endl;
      createSuperPreds(superClausesArr_, domain);
       
      cout << "Number of superclauses after this iteration is = "
           << newSuperClauseCnt << endl;
      itr++;
    }

    superClauseCnt = getNumArrayArrayElements(*superClausesArr_);
    cout << "***************************************************************"
         << endl;
    cout << "Total Number of Super Clauses = " << superClauseCnt << endl;
  
    int predCnt = domain->getNumPredicates();
    const PredicateTemplate *ptemplate;
    for (int predId = 0; predId < predCnt; predId++)
    {
      ptemplate = domain->getPredicateTemplate(predId);
      if (ptemplate->isEqualPredicateTemplate())
        continue;
      int cnt = SuperPred::getSuperPredCount(predId);
      if (cnt > 0)
      {
        cout<<"SuperPred count for pred: ";
        ptemplate->print(cout);
        cout << " = " << cnt << endl;
      }
    }

    for (int i = 0; i < nodes->size(); i++) delete (*nodes)[i];
    for (int i = 0; i < factors->size(); i++) delete (*factors)[i];
    nodes->clear();
    factors->clear();
  }


  /**
   * Create the Super BP network
   */
  void createSuperNetwork()
  {
    Domain* domain = domain_;
    Array<SuperPred*> * superPreds;
    Array<SuperClause*> *superClauses;
  
    SuperClause *superClause;
    SuperPred *superPred;

    BPFactor *factor;
    BPNode *node;
    Clause *clause;
    Array<int>* constants = NULL;

      //create the factor (superclause) nodes
    for (int arrIndex = 0; arrIndex < superClausesArr_->size(); arrIndex++)
    {
      superClauses = (*superClausesArr_)[arrIndex];
      for (int scindex = 0; scindex < superClauses->size(); scindex++)
      {
        superClause = (*superClauses)[scindex];
        clause = superClause->getClause();
        factor = new BPFactor(clause, superClause, constants, domain);
        factors_->append(factor);
      }
    }

      //create the variable (superpreds) nodes
    int predCnt = domain->getNumPredicates();
    for (int predId = 0;predId < predCnt; predId++)
    {
      superPreds = SuperPred::getSuperPreds(predId);
      for (int spindex = 0; spindex < superPreds->size(); spindex++)
      {
        superPred = (*superPreds)[spindex];
        node = new BPNode(predId, superPred, constants, domain);
        node->addFactors(factors_, getLinkIdToTwoWayMessageMap());
        nodes_->append(node);
      }
    }
  }
  
  /**
   *  Manipulate the link id to message map
   */
  void updateLinkIdToTwoWayMessageMap()
  {
    BPNode *node;
    BPLink *link;
    BPFactor *factor;
    double nodeToFactorMsgs[2];
    double factorToNodeMsgs[2];

    LinkId *lid;
    TwoWayMessage *tmsg;
    LinkIdToTwoWayMessageMap::iterator lidToTMsgItr;
     
      // Delete the old values
    Array<LinkId*> keysArr;
    for (lidToTMsgItr = lidToTWMsg_->begin();
         lidToTMsgItr != lidToTWMsg_->end();
         lidToTMsgItr++)
    {
      keysArr.append(lidToTMsgItr->first);
      tmsg = lidToTMsgItr->second;
      delete tmsg;
    }
                       
    for (int i = 0; i < keysArr.size(); i++)
    {
      delete keysArr[i];
    }
    lidToTWMsg_->clear();

      // Now populate
    for (int i = 0; i < nodes_->size(); i++)
    {
      node = (*nodes_)[i];
      for (int j = 0; j < node->getNumLinks(); j++)
      {
        link = node->getLink(j);
        factor = link->getFactor();
              
        int predId = node->getPredId();
        int superPredId = node->getSuperPredId();
        int superClauseId = factor->getSuperClauseId();
        int predIndex = link->getPredIndex(); 
              
        lid = new LinkId(predId, superPredId, superClauseId, predIndex);

        int reverseFactorIndex = link->getReverseFactorIndex();
        node->getMessage(reverseFactorIndex, nodeToFactorMsgs);

        int reverseNodeIndex = link->getReverseNodeIndex();
        factor->getMessage(reverseNodeIndex, factorToNodeMsgs);
 
        tmsg = new TwoWayMessage(nodeToFactorMsgs,factorToNodeMsgs);
        (*lidToTWMsg_)[lid] = tmsg;
      }
    }
  }
  
  
    //add the super clauses
  void addSuperClauses(ClauseToSuperClauseMap* const & clauseToSuperClause)
  {
    ClauseToSuperClauseMap::iterator clauseItr;
    Array<SuperClause *> * superClauses;
    SuperClause *superClause;
     
    for(clauseItr = clauseToSuperClause->begin();
        clauseItr != clauseToSuperClause->end(); 
        clauseItr++)
    {
      superClauses = new Array<SuperClause *>();
      superClausesArr_->append(superClauses);
      superClause = clauseItr->second;
      superClauses->append(superClause);
    }
  }
  
  
  ClauseToSuperClauseMap*
  mergeSuperClauses(ClauseToSuperClauseMap* const & clauseToSuperClause)
  {
    Domain* domain = domain_;
    ClauseToSuperClauseMap *mergedClauseToSuperClause =
      new ClauseToSuperClauseMap();
    SuperClause *superClause, *mergedSuperClause;
    Clause *keyClause;
    ClauseToSuperClauseMap::iterator itr, mergedItr;
    Array<int> * constants;
    double tcnt;
    for (itr = clauseToSuperClause->begin();
         itr != clauseToSuperClause->end();
         itr++)
    {
      superClause = itr->second;
      keyClause = superClause->getClause();
      mergedItr = mergedClauseToSuperClause->find(keyClause);
      if (mergedItr != mergedClauseToSuperClause->end())
      {
        mergedSuperClause = mergedItr->second;
        for (int tindex = 0; tindex < superClause->getNumTuples(); tindex++)
        {
          constants = superClause->getConstantTuple(tindex);
          tcnt = superClause->getTupleCount(tindex);
          mergedSuperClause->addNewConstantsAndIncrementCount(constants, tcnt);
          delete constants;
        }
        delete superClause;
      }
      else
      {
        (*mergedClauseToSuperClause)[keyClause] = superClause;
        keyClause->print(cout, domain);
        cout << endl;
      }
    }
    return mergedClauseToSuperClause;
  }
  
  /**
   * Creates the equivalence classes for the variables appearing in various
   * clauses.
   */
  PredicateTermToVariable* getPredicateTermToVariableMap(MLN * const & mln,
                                                         Domain* const & domain)
  {
    Clause *clause;
    Predicate *pred;
    const Term* term;
    Array<Variable *> *eqVars = new Array<Variable *>();
    int eqClassId = 0;
    Variable *var;
    const PredicateTemplate *ptemplate;
    const Array<int>* constants;

    for (int clauseno = 0; clauseno < mln->getNumClauses(); clauseno++)
    {
      clause = (Clause *)mln->getClause(clauseno);
      for (int predno = 0; predno < clause->getNumPredicates(); predno++)
      {
        pred = clause->getPredicate(predno);
        ptemplate = pred->getTemplate();
        for (int termno = 0; termno < pred->getNumTerms(); termno++)
        {
          term = pred->getTerm(termno);
          int varId = term->getId();
          int varTypeId = ptemplate->getTermTypeAsInt(termno);
          constants = domain->getConstantsByType(varTypeId); 
          var = new Variable(clause,varId,pred,termno,eqClassId,constants);
          eqVars->append(var);
          eqClassId++;
        }
      }
    }
    
    Variable  *var1, *var2;
    for (int i = 0; i < eqVars->size(); i++)
    {
      var1 = (*eqVars)[i];
      for (int j = i + 1; j < eqVars->size(); j++)
      {
        var2 = (*eqVars)[j];
        if (var1->same(var2))
        {
          var1->merge(var2); 
        }
      }
    }

      //now populate the map
    PredicateTermToVariable *ptermToVar = new PredicateTermToVariable();
    PredicateTermToVariable::iterator itr;
    PredicateTerm *pterm;
    Variable *tiedVar;
    int uniqueCnt = 0;
    for (int i = 0; i < eqVars->size(); i++)
    {
      var = (*eqVars)[i];
      if (var->isRepresentative())
      {
        uniqueCnt++;
        for (int j = 0; j < var->getNumTiedVariables(); j++)
        {
          tiedVar = var->getTiedVariable(j);
          int predId = tiedVar->getPredId();
          int termno = tiedVar->getTermno();
          pterm = new PredicateTerm(predId,termno);
          itr = ptermToVar->find(pterm);
          if (itr == ptermToVar->end())
          {
            (*ptermToVar)[pterm] = var;
          }
          else
          {
            delete pterm;
          }
        }
      }
    }

    cout << "size of PtermToVarMap is " << ptermToVar->size() << endl;
    cout << "count of Variable Eq Classes (Unique) is = " << uniqueCnt << endl;
    return ptermToVar;
  }


  void getIndexedConstants(PredicateTermToVariable * const & ptermToVar, 
                           MLN * const & mln, 
                           Domain * const & domain)
  {
    Predicate *pred, *gndPred;
    IntHashArray seenPredIds;
    const Clause *clause;
    const Term *term;
     
    Array<Predicate *> * indexedGndings;
    PredicateTerm *pterm;
    Database * db;
     
    int predId, termId, constantId;
    bool ignoreActivePreds = true;

    PredicateTermToVariable::iterator itr;
    Variable * var;

    indexedGndings = new Array<Predicate *>();
    db = domain->getDB();
    cout << "size of PtermToVarMap is " << ptermToVar->size() << endl;
     
    Clause *varClause;
    for (int clauseno = 0; clauseno < mln->getNumClauses(); clauseno++)
    {
      clause = mln->getClause(clauseno);    
      varClause = new Clause(*clause);
      varClause->updateToVarClause();

        //to make sure that we do not use clause
      clause = NULL;

      for (int predno = 0; predno < varClause->getNumPredicates(); predno++)
      {
        pred = varClause->getPredicate(predno);
        predId = pred->getId();

        if (seenPredIds.append(predId) < 0)
          continue;
        indexedGndings->clear();
          //Note: we assume that every predicate is indexable
        if(db->isClosedWorld(predId))
        {
            //precidate is closed world - rettrieve only true groundings
          db->getIndexedGndings(indexedGndings,pred,ignoreActivePreds,true);
        }
        else
        {
            //predicate is open world - retrieve both true and false groundings  
          db->getIndexedGndings(indexedGndings,pred,ignoreActivePreds,true);
          db->getIndexedGndings(indexedGndings,pred,ignoreActivePreds,false);
        }
        
        for (int gndno = 0; gndno < indexedGndings->size(); gndno++)
        {
          gndPred = (*indexedGndings)[gndno];

          for (int termno = 0; termno < gndPred->getNumTerms(); termno++)
          {
            pterm = new PredicateTerm(predId, termno);
            itr = ptermToVar->find(pterm);
            assert(itr != ptermToVar->end());
            var = itr->second;
            term = gndPred->getTerm(termno);
            constantId = term->getId();
            var->removeImplicit(constantId);
            delete pterm;
          }
          delete (*indexedGndings)[gndno];
        }
      }
      delete varClause;
    }
   
      //now explicitly handle the constants appearing in the clause
    for (int clauseno = 0; clauseno < mln->getNumClauses(); clauseno++)
    {
      clause = mln->getClause(clauseno);    
      for (int predno = 0; predno < clause->getNumPredicates(); predno++)
      {
        pred = clause->getPredicate(predno);
        predId = pred->getId();
        for (int termno = 0; termno < pred->getNumTerms(); termno++)
        {
          term = pred->getTerm(termno);
          termId = term->getId();
            // if it is a variable, nothing to do
          if (termId < 0) continue;
            // else, this constant also should be added the list of
            // indexed constants
          pterm = new PredicateTerm(predId, termno);
          itr = ptermToVar->find(pterm);
          assert(itr != ptermToVar->end());
          var = itr->second;
          var->removeImplicit(termId);
          delete pterm;
        }
      }
    }

    IntHashArray *seenEqClassIds = new IntHashArray();
    cout << "Implicit Set of constants are: " << endl;
    for (itr = ptermToVar->begin(); itr != ptermToVar->end(); itr++)
    {
      pterm = itr->first;
      var = itr->second;
      int eqClassId = var->getEqClassId();
      if (seenEqClassIds->find(eqClassId) >= 0)
        continue;
      seenEqClassIds->append(eqClassId);
      cout << "Implicit Constants for Eq class " << eqClassId << endl;
      cout << "Count =  " << var->getNumImplicitConstants() << " => " << endl;
      var->printImplicitConstants(cout, domain);
      cout << endl << endl << endl;
    }
    delete seenEqClassIds;
  }
  
 private:
    // Max. no. of BP iterations to perform
  int maxSteps_;
    // Max. no. of seconds BP should run
  int maxSeconds_;
    // Indicates if lifted inference will be run
  bool lifted_;
    // Maximum difference between probabilities must be less than this
    // in order to converge
  double convergenceThresh_;
    // Convergence must last this number of iterations
  int convergeRequiredItrCnt_;
    // Indicates if implicit representation is to be used
  bool implicitRep_;

  LinkIdToTwoWayMessageMap* lidToTWMsg_;

  Array<Array<SuperClause*>*>* superClausesArr_;
  
  /* For creation of the Belief Propagation Network */
  Array<BPFactor*>* factors_;
  Array<BPNode*>* nodes_;

  MLN* mln_;
  Domain* domain_;
};

#endif /*BP_H_*/
