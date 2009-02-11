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
#include "bpfactor.h"
 
/*****************************************************************************/
// Functions for class BPFactor
/*****************************************************************************/

//the contribution of the factor itself
void BPFactor::initFactorMesssages()
{
  Predicate *pred;
  int numPreds = clause_->getNumPredicates();
  int stateCnt = (int)pow(2.0, numPreds);
  factorMsgs_ = new double[stateCnt];
  bool isSatisfied;
  for (int state = 0; state < stateCnt; state++)
  {
    isSatisfied = false;
    for (int predno = 0; predno < numPreds; predno++)
    {
      pred = clause_->getPredicate(predno);
      bool predBit = state & (1<<predno);
      if (pred->getSense() == predBit)
      {
        isSatisfied = true;
        break;
      }
    }
                   
    if(isSatisfied)
    {
      factorMsgs_[state] = clause_->getWt();
    }
    else
    {
      factorMsgs_[state] = 0;
    }
  }
}

  //find the outgoing message for the given inpPredIndex
double* BPFactor::multiplyMessagesAndSumOut(int inpPredIndex)
{
  int numPreds = clause_->getNumPredicates();
  int stateCnt = (int)pow(2.0, numPreds);
  double * prodMsgs = new double[stateCnt];
             
  BPNode *node;
  double *gndNodeCnts = new double[numPreds];

  // This is done to handle the case when BP is run
  // on supernodes/superfeatures which have not yet
  // reached an equilibrium state
  for (int predno = 0; predno < numPreds; predno++)
    gndNodeCnts[predno] = 0;
             
  for (int lno = 0; lno < links_->size(); lno++)
  {
    int predIndex = (*links_)[lno]->getPredIndex();
    node = (*links_)[lno]->getNode();
    gndNodeCnts[predIndex] += node->getGroundNodeCount();
  }

    //initialize the product
  for (int state = 0; state < stateCnt; state++)
    prodMsgs[state] = factorMsgs_[state];

  for (int lno = 0; lno < links_->size(); lno++)
  {
    int predIndex = (*links_)[lno]->getPredIndex();
    if (predIndex == inpPredIndex)
      continue;

    node = (*links_)[lno]->getNode();
      //wt must be equal to 1 in equilibrium. This step 
      //averages (weighted) out the messages from various 
      //supernodes (at this predIndex position).
    assert(gndNodeCnts[predIndex] != 0);
    double wt = node->getGroundNodeCount()/gndNodeCnts[predIndex];

    for (int state = 0; state < stateCnt; state++)
    {
      bool predBit = state & (1<<predIndex);
      if (predBit)
        prodMsgs[state] += (*msgsArr_)[lno][1]*wt;
      else
        prodMsgs[state] += (*msgsArr_)[lno][0]*wt;
    }
  }

    //caller is responsible for deleting it
  double *outMsgs = new double[2];    
  double maxMsgs[2];
  maxMsgs[0] = maxMsgs[1] = 0;
  bool firstTime[2];
  firstTime[0] = firstTime[1] = true;
             
    //now find the max messages 
  for (int state = 0; state < stateCnt; state++)
  {
    bool predBit = state & (1<<inpPredIndex);
    if (predBit && (maxMsgs[1] < prodMsgs[state] || firstTime[1]))
    {
      firstTime[1] = false;
      maxMsgs[1] = prodMsgs[state];
    }

    if (!predBit && (maxMsgs[0] < prodMsgs[state] || firstTime[0]))
    {
      firstTime[0] = false;
      maxMsgs[0] = prodMsgs[state];
    }
  }

  outMsgs[0] = outMsgs[1] = 0;
  for (int state = 0; state < stateCnt; state++)
  {
    bool predBit = state & (1<<inpPredIndex);
    if (predBit) 
      outMsgs[1] += expl(prodMsgs[state] - maxMsgs[1]);
    else
      outMsgs[0] += expl(prodMsgs[state] - maxMsgs[0]);
  }
  outMsgs[1] = maxMsgs[1] + logl(outMsgs[1]);
  outMsgs[0] = maxMsgs[0] + logl(outMsgs[0]);
  outMsgs[1] = outMsgs[1] - outMsgs[0];
  outMsgs[0] = 0;
  delete [] prodMsgs;
  delete [] gndNodeCnts;
  return outMsgs;
}

  //send Message on all the links
void BPFactor::sendMessage()
{
  double *outMsgs = NULL;
  BPLink *link;
  BPNode *node;
  int predIndex;
  for (int lindex = 0; lindex < links_->size(); lindex++)
  {
    link = (*links_)[lindex];
    node = link->getNode();
    predIndex = link->getPredIndex();

    outMsgs = multiplyMessagesAndSumOut(predIndex);
      //Assumes pass by value copy of the messages
    node->receiveMessage(outMsgs, link);
    delete [] outMsgs;
  }
}

  //update the stored msgs and update the msgProduct
void BPFactor::moveToNextStep()
{
  double * msgs;
  double * nextMsgs;

    //store the current messages in prevMessages array and
    //reinitialize the current message array
  for (int lindex = 0;lindex < links_->size(); lindex++)
  {
    msgs = (*msgsArr_)[lindex];
    nextMsgs = (*nextMsgsArr_)[lindex];
    for (int i = 0; i < 2; i++)
    {
      msgs[i] = nextMsgs[i];
      nextMsgs[i] = 0;
    }
  }
}

