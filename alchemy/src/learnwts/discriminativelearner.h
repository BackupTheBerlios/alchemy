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
#ifndef DISCRIMINATIVE_LEARNER_H_OCT_30_2005
#define DISCRIMINATIVE_LEARNER_H_OCT_30_2005

#include "infer.h"
#include "clause.h"
#include "timer.h"
#include "indextranslator.h"
#include "maxwalksat.h"

const bool dldebug = false;
const double EPSILON = .00001;

#ifndef PI
#define PI 3.14159265
#endif

double dotprod(const double* v1, const double* v2, int dim) 
{
  double total = 0.0;
  for (int i = 0; i < dim; i++) 
    total += v1[i] * v2[i];
  return total;
}

double dotprod(const double* v1, const Array<double>& v2, int dim) 
{
  double total = 0.0;
  for (int i = 0; i < dim; i++) 
    total += v1[i] * v2[i];
  return total;
}

double dotprod(const Array<double>& v1, const Array<double>& v2, int dim) 
{
  double total = 0.0;
  for (int i = 0; i < dim; i++) 
  {
    total += v1[i] * v2[i];
  }
  return total;
}

double vlen(const double* v1, int dim) 
{ return sqrt(dotprod(v1,v1,dim)); }
double vlen(const Array<double>& v1, int dim) 
{ return sqrt(dotprod(v1,v1,dim)); }

bool isOutputIter(int iter)
{
  while (iter % 10 == 0) 
    iter /= 10;

  if (iter == 1 || iter == 2 || iter == 5)
    return true;
  else
    return false;
}



/**
 * Discriminative learning algorithms (see "Discriminative Training of Markov
 * Logic Networks", Singla and Domingos, 2005 and "Efficient Weight Learning for
 * Markov Logic Networks", Lowd and Domingos, 2007).
 */
class DiscriminativeLearner
{
 public:

  /**
   * Constructor. Various variables are initialized, relevant clauses are
   * determined and weights and inference procedures are initialized.
   * 
   * @param inferences Array of inference procedures to be used for inference
   * in each domain.
   * @param nonEvidPredNames Names of non-evidence predicates. This is used to
   * determine the relevant clauses.
   * @param idxTrans IndexTranslator needed when multiple dbs are used and they
   * don't line up.
   * @param lazyInference If true, lazy inference is used.
   * @param withEM If true, EM is used to fill in missing values.
   * @param rescaleGradient If true, use per-weight learning rates
   * @param method Determines how direction and step size are chosen
   * @param lambda Initial value of lambda for SMD or CG
   * @param preconditionCG Whether or not to use a preconditioner with
   * scaled conjugate gradient
   * @param maxLambda Maximum value of lambda for CG
   */
  DiscriminativeLearner(const Array<Inference*>& inferences,
                        const StringHashArray& nonEvidPredNames,
                        IndexTranslator* const & idxTrans,
                        const bool& lazyInference, const bool& withEM,
                        const bool& rescaleGradient, const int& method,
                        const double& lambda, const bool& preconditionCG,
                        const double& maxLambda)
    : domainCnt_(inferences.size()), idxTrans_(idxTrans),
      lazyInference_(lazyInference), rescaleGradient_(rescaleGradient),
      method_(method),
      // HACK: for now, we'll use the SMD lambda value for CG, even
      // though the two represent *very* different things!
      cg_lambda_(lambda), preconditionCG_(preconditionCG),
      maxBacktracks_(1000), backtrackCount_(0),
      cg_max_lambda_(maxLambda), withEM_(withEM) 
  { 
    cout << endl << "Constructing discriminative learner..." << endl << endl;

    inferences_.append(inferences);
    logOddsPerDomain_.growToSize(domainCnt_);
    clauseCntPerDomain_.growToSize(domainCnt_);
    
    for (int i = 0; i < domainCnt_; i++)
    {
      clauseCntPerDomain_[i] =
        inferences_[i]->getState()->getMLN()->getNumClauses();
      logOddsPerDomain_[i].growToSize(clauseCntPerDomain_[i], 0);
    }

    totalTrueCnts_.growToSize(domainCnt_);
    defaultTrueCnts_.growToSize(domainCnt_);
    relevantClausesPerDomain_.growToSize(domainCnt_);
    //relevantClausesFormulas_ is set in findRelevantClausesFormulas()

    findRelevantClauses(nonEvidPredNames);
    findRelevantClausesFormulas();

      // Initialize the clause wts for lazy version
    if (lazyInference_)
    {
      findCountsInitializeWtsAndSetNonEvidPredsToUnknownInDB(nonEvidPredNames);
    
      for (int i = 0; i < domainCnt_; i++)
      {
        const MLN* mln = inferences_[i]->getState()->getMLN();
        Array<double>& logOdds = logOddsPerDomain_[i];
        assert(mln->getNumClauses() == logOdds.size());
        for (int j = 0; j < mln->getNumClauses(); j++)
          ((Clause*) mln->getClause(j))->setWt(logOdds[j]);
      }
    }
      // Initialize the clause wts for eager version
    else
    {      
      initializeWts(nonEvidPredNames);
    }
    
      // Initialize the inference / state
    for (int i = 0; i < inferences_.size(); i++)
      inferences_[i]->init();
  }

  ~DiscriminativeLearner() 
  {
    for (int i = 0; i < trainTrueCnts_.size(); i++)
      delete[] trainTrueCnts_[i];
  }

    // set the prior means and std devs.
  void setMeansStdDevs(const int& arrSize, const double* const & priorMeans, 
                       const double* const & priorStdDevs) 
  {
    if (arrSize < 0) 
    {
      usePrior_ = false;
      priorMeans_ = NULL;
      priorStdDevs_ = NULL;
    } 
    else 
    {
      //cout << "arr size = " << arrSize<<", clause count = "<<clauseCnt_<<endl;
      usePrior_ = true;
      priorMeans_ = priorMeans;
      priorStdDevs_ = priorStdDevs;

      //cout << "\t\t Mean \t\t Std Deviation" << endl;
      //for (int i = 0; i < arrSize; i++) 
      //  cout << i << "\t\t" << priorMeans_[i]<<"\t\t"<<priorStdDevs_[i]<<endl;
    }
  }


  void setMLNWeights(double* const& weights)
  {
      // If there is one db or the clauses for multiple databases line up
    if (idxTrans_ == NULL)
    {
      int clauseCnt = clauseCntPerDomain_[0];
      for (int i = 0; i < domainCnt_; i++)
      {
        Array<bool>& relevantClauses = relevantClausesPerDomain_[i];
        assert(clauseCntPerDomain_[i] == clauseCnt);
        const MLN* mln = inferences_[i]->getState()->getMLN();
        
        for (int j = 0; j < clauseCnt; j++) 
        {
          Clause* c = (Clause*) mln->getClause(j);
          if (relevantClauses[j]) c->setWt(weights[j]);
          else                    c->setWt(0);
        }
      }
    }
    else
    {   // The clauses for multiple databases do not line up
      Array<Array<double> >* wtsPerDomain = idxTrans_->getWtsPerDomain();
      const Array<Array<Array<IdxDiv>*> >* cIdxToCFIdxsPerDomain 
        = idxTrans_->getClauseIdxToClauseFormulaIdxsPerDomain();
      
      for (int i = 0; i < domainCnt_; i++)
      {
        Array<double>& wts = (*wtsPerDomain)[i];
        memset((double*)wts.getItems(), 0, wts.size()*sizeof(double));

          //map clause/formula weights to clause weights
        for (int j = 0; j < wts.size(); j++)
        {
          Array<IdxDiv>* idxDivs = (*cIdxToCFIdxsPerDomain)[i][j];          
          for (int k = 0; k < idxDivs->size(); k++)
            wts[j] += weights[ (*idxDivs)[k].idx ] / (*idxDivs)[k].div;
        }
      }
      
      for (int i = 0; i < domainCnt_; i++)
      {
        Array<bool>& relevantClauses = relevantClausesPerDomain_[i];
        int clauseCnt = clauseCntPerDomain_[i];
        Array<double>& wts = (*wtsPerDomain)[i];
        assert(wts.size() == clauseCnt);
        const MLN* mln = inferences_[i]->getState()->getMLN();

        for (int j = 0; j < clauseCnt; j++)
        {
          Clause* c = (Clause*) mln->getClause(j);
          if(relevantClauses[j]) c->setWt(wts[j]);
          else                   c->setWt(0);
        }
      }
    }
  }


  void setLogOddsWeights(double* weights, int numWeights)
  {
      // If there is one db or the clauses for multiple databases line up
    if (idxTrans_ == NULL)
    {
      for (int i = 0; i < domainCnt_; i++)
      {
        Array<double>& logOdds = logOddsPerDomain_[i];
        assert(numWeights == logOdds.size());
        for (int j = 0; j < logOdds.size(); j++) weights[j] += logOdds[j];
      }
    }
    else
    { //the clauses for multiple databases do not line up
      const Array<Array<Array<IdxDiv>*> >* cIdxToCFIdxsPerDomain 
        = idxTrans_->getClauseIdxToClauseFormulaIdxsPerDomain();

      Array<int> numLogOdds; 
      Array<double> wtsForDomain;
      numLogOdds.growToSize(numWeights);
      wtsForDomain.growToSize(numWeights);
    
      for (int i = 0; i < domainCnt_; i++)
      {
        memset((int*)numLogOdds.getItems(), 0, numLogOdds.size()*sizeof(int));
        memset((double*)wtsForDomain.getItems(), 0,
               wtsForDomain.size()*sizeof(double));

        Array<double>& logOdds = logOddsPerDomain_[i];
      
          // Map the each log odds of a clause to the weight of a
          // clause/formula
        for (int j = 0; j < logOdds.size(); j++)
        {
          Array<IdxDiv>* idxDivs =(*cIdxToCFIdxsPerDomain)[i][j];          
          for (int k = 0; k < idxDivs->size(); k++)
          {
            wtsForDomain[ (*idxDivs)[k].idx ] += logOdds[j];
            numLogOdds[ (*idxDivs)[k].idx ]++;
          }
        }

        for (int j = 0; j < numWeights; j++)
          if (numLogOdds[j] > 0) weights[j] += wtsForDomain[j]/numLogOdds[j];  
      }
    }

    for (int i = 0; i < numWeights; i++)
      weights[i] /= domainCnt_;
  }


  void getVariance(Array<double>& variance, int numWeights)
  {
    // Compute variance and gradient for each clause
    for (int clauseno = 0; clauseno < numWeights; clauseno++)
    {
      variance[clauseno] = 0.0;

      // Takes into account the prior
      if (usePrior_)
      {
         double sd = priorStdDevs_[clauseno];
         variance[clauseno] = 1.0/(sd * sd);
      }

      // Sum variance over all domains
      for (int i = 0; i < domainCnt_; i++)
      {
        const Array<double>* trueCnts   
            = inferences_[i]->getClauseTrueCnts();
        const Array<double>* trueSqCnts 
            = inferences_[i]->getClauseTrueSqCnts();
        int totalSamples = inferences_[i]->getNumSamples();

        double x   = (*trueCnts)[clauseno];
        double xsq = (*trueSqCnts)[clauseno];
        double n   = totalSamples;

        // Add variance for this domain
        variance[clauseno] += xsq/n - (x/n)*(x/n);
      }
      
      if (dldebug)
        cout << clauseno << ": " << variance[clauseno] << endl;
    }
  }


  const Array<double>* getHessianVectorProduct(const Array<double>& d)
  {
    // Compute Hessian vector product using counts from all inferences
    const Array<double>* Hd = inferences_[0]->getHessianVectorProduct(d);
    for (int i = 1; i < domainCnt_; i++) 
    {
      const Array<double>* Hd_i = inferences_[i]->getHessianVectorProduct(d);
      for (int j = 0; j < Hd->size(); j++) 
        (*Hd)[j] += (*Hd_i)[j];
      delete Hd_i;
    }

    return Hd;
  }


  double computeQuadraticStepLength(double* gradient, const Array<double>& d, 
          const Array<double>* Hd, double lambda)
  {
    int numWeights = d.size();

    // Compute step length using trust region approach
    double dHd = dotprod(d, (*Hd), numWeights);
    double dd = dotprod(d, d, numWeights);
    double dg = dotprod(gradient, d, numWeights);
    double alpha = -dg/(dHd + lambda * dd);

      // Debug messages; can be helpful for diagnosing what is going on
    if (dldebug)
    {
      cout << "dHd = " << dHd << endl;
      cout << "dd = " << dd << endl;
      cout << "dg = " << dg << endl;
      cout << "alpha = " << alpha << endl;
    }

      // Because the problem is convex, the Hessian should always
      // be positive definite, and so alpha should always be non-negative.
      // Since our Hessian is approximate, we may end up with a negative
      // alpha.  In these cases, we used to make alpha zero (i.e., take a 
      // step of size zero), but now we return the negative alpha and
      // let the caller deal with it.
    if (alpha < 0.0) 
    {
      //alpha = 0.0;
      if (dldebug)
      {
        cout << "Alpha < 0!  Bad direction or Hessian." << endl;
        //cout << "Flipped alpha!  Setting step length to zero." << endl;
      }
    }

    return alpha;
  }


    // learn the weights
  void learnWeights(double* const & weights, const int& numWeights,
                    const int& maxIter, const double& maxSec,
                    const double& learningRate, const double& momentum,
                    bool initWithLogOdds, const int& mwsMaxSubsequentSteps, 
                    bool periodicMLNs)
  {
    Timer timer;
    double begSec = timer.time();

    //cout << "Learning weights discriminatively... " << endl;
    memset(weights, 0, numWeights*sizeof(double));

    double* averageWeights = new double[numWeights];
    double* gradient = new double[numWeights];

      // For predicting and assessing changes to the gradient
    const Array<double>* delta_pred = NULL;
    const Array<double>* Hd = NULL;

      // For all methods
    double alpha = 1;
    Array<double> d(numWeights, 0.0);
    double oldgradient[numWeights];
    Array<double> oldd(numWeights, 0.0);

    //double pre_backtrack_lambda = cg_lambda_;

      // Set the initial weight to the average log odds across domains/databases
    if (initWithLogOdds)
      setLogOddsWeights(weights, numWeights);

      // Initialize averageWeights
    memcpy(averageWeights, weights, numWeights*sizeof(double));

      // Make sure to save all counts, for computing gradient
    for (int i = 0; i < domainCnt_; i++)
      inferences_[i]->saveAllCounts(true);

      // For DN method
    Array<double> currVariance(numWeights, 0);

      // Used in CG
    Array<double> oldweights(numWeights, 0.0);

      // True if we're backtracking -- prevents us from making a forward
      // step if we've already committed to reverting our weights.
    bool backtracked = false;


    for (int iter = 1; iter <= maxIter; iter++) 
    {
      cout << endl << "Iteration " << iter << " : " << endl << endl;

      double totalsec = timer.time() - begSec;
      int hour = (int)totalsec/3600;
      int min = (int)(totalsec - 3600*hour)/60;
      int sec = (int)(totalsec - 3600*hour - 60*min);

        // Print time so far
      cout << "Elapsed time: " << totalsec << "s";
      if (hour > 0)
        cout << " (" << hour << "h " << min << "m " << sec << "s)";
      else if (min > 0)
        cout << " (" << min << "m " << sec << "s)";
      cout << endl;

      if (maxSec > 0 && totalsec > maxSec)
      {
        cout << "Time limit exceeded.  Stopping learning." << endl;
        break;
      }

        // In 3rd iteration, we want to tell MWS to perform subsequentSteps
        // (Iter. 1 is random assigment if initial weights are 0)
      if (iter == 3)
      {
        for (int i = 0; i < inferences_.size(); i++)
        {
            // Check if using MWS
          if (MaxWalkSat* mws = dynamic_cast<MaxWalkSat*>(inferences_[i]))
          {
            mws->setMaxSteps(mwsMaxSubsequentSteps);
          }
        }
      }

      cout << "Getting gradient..." << endl;
      getGradient(weights, gradient, numWeights);

      double realdist = 1.0;
      double preddist = 1.0;
      if (iter > 1 && delta_pred != NULL && !backtracked)
      {
        Array<double> dist(numWeights, 0.0);
        for (int i = 0; i < numWeights; i++)
          dist[i] = weights[i] - oldweights[i];

        // Below are four different methods for estimating
        // the real and predicted change in the function value.
        // These values are used to size of the trust region
        // by updating lambda.
#if 1
        // Predicted change is quadratic approximation
        Array<double> avgPred(numWeights);
        avgPred.growToSize(numWeights);
        for (int i = 0; i < numWeights; i++)
          avgPred[i] = oldgradient[i] + (*delta_pred)[i]/2.0;
        preddist = dotprod(avgPred, dist, numWeights);

        // Real change is lower bound on actual change
        realdist = dotprod(gradient, dist, numWeights);
#elif 0
        // Actual distance is a lower bound on the change 
        // in our function: new gradient * dist
        // Predicted change adopts the same form, but uses
        // the predicted gradient instead: pred gradient * dist
        Array<double> predgradient(numWeights, 0.0);
        for (int i = 0; i < numWeights; i++)
          predgradient[i] = oldgradient[i] + (*delta_pred)[i];

        preddist = dotprod(predgradient, dist, numWeights);
        realdist = dotprod(gradient, dist, numWeights);
#elif 0
        // Act as if we're optimizing the dot product of the
        // gradient with the search direction instead of
        // the function value itself.  In practice, this doesn't work
        // well.
        Array<double> delta_actual(numWeights, 0.0);
        for (int i = 0; i < numWeights; i++)
          delta_actual[i] = gradient[i] - oldgradient[i];

        preddist = dotprod(*delta_pred, dist, numWeights);
        realdist = dotprod(delta_actual, dist, numWeights);
#else
        // Assume quadratic behavior of function
        // If this test passes, then either our quadratic approx is
        // good, or we were strangely lucky.  In practice, this isn't
        // very stable, since our function doesn't act quadratically.
        Array<double> avgPred(numWeights, 0.0);
        Array<double> avgActual(numWeights, 0.0);
        for (int i = 0; i < numWeights; i++)
        {
          avgPred[i] = oldgradient[i] + (*delta_pred)[i]/2.0;
          avgActual[i] = (oldgradient[i] + gradient[i])/2.0;
        }

        preddist = dotprod(avgPred, dist, numWeights);
        realdist = dotprod(avgActual, dist, numWeights);
#endif

          // LOG: Print out the trust-region diagnostics.
        cout << "Pred*dist = " << preddist << endl;
        cout << "Real*dist = " << realdist << endl;
        if (preddist > 0)
          cout << "Distratio = " << realdist/preddist << endl;
      }

      if (iter > 1 && (method_ == CG || method_ == DN))
      {

        // Adjust lambda using technique of (Fletcher, 1987)
        double delta = realdist/preddist;

        // TEMP HACK DEBUG!  
        // Disable all updates to lambda in a hackish way.
        //delta = 0.5;

        if (!backtracked && preddist == 0)
          cg_lambda_ /= 4;

#if 0
        // In general, overshooting the predicted gain in
        // function value is a good thing.  However, since we're
        // doing everything approximately, we may wish to adjust
        // lambda so that our quadratic approximation is always
        // close.  These update formulas penalize overshooting.
        if (delta > 0.75 && delta < 1.333)
          cg_lambda_ /= 2;
        if (delta < 0.25 || delta > 4.0)
        {
          if (cg_lambda_ * 4 > cg_max_lambda_)
            cg_lambda_ = cg_max_lambda_;
          else
            cg_lambda_ *= 4;
        }
#else
        if (!backtracked && preddist != 0 && delta > 0.75)
          cg_lambda_ /= 2;
        if (delta < 0.25)
        {
          if (cg_lambda_ * 4 > cg_max_lambda_)
            cg_lambda_ = cg_max_lambda_;
          else
            cg_lambda_ *= 4;
        }
#endif
        cout << "Lambda = " << cg_lambda_ << endl;

        if (delta < 0.0 && backtrackCount_ < maxBacktracks_)
        {
          cout << "Backtracking!" << endl;
          iter--;
#if 0
          double oldalpha = alpha;
          alpha = computeQuadraticStepLength(oldgradient, d, Hd, cg_lambda_);
          for (int i = 0; i < numWeights; i++)
          {
            weights[i] = oldweights[i] + alpha * d[i];
            (*delta_pred)[i] *= alpha/oldalpha;
          }
#else
          for (int i = 0; i < numWeights; i++)
            weights[i] = oldweights[i];

#endif
          for (int i = 0; i < domainCnt_; i++) 
            inferences_[i]->restoreCnts();

          backtracked = true;
          backtrackCount_++;
        }
        else
        {
          backtracked = false;
          backtrackCount_ = 0;
        }
      }

      if (!backtracked)
      {
        for (int i = 0; i < domainCnt_; i++)
          inferences_[i]->saveCnts();
      }

        // Scaled conjugate gradient: compute conjugate gradient direction
        //     using Polak-Ribiere and step size using Hessian matrix.
        //     Optional preconditioner (inverse diagonalized Hessian)
      if (method_ == CG && !backtracked)
      {
        // Precond stores the diagonal entries of preconditioning matrix
        Array<double> precond(numWeights, 1.0);

        // If preconditioning, use the inverse diagonalized Hessian
        if (preconditionCG_)
        {
          Array<double> variance(numWeights, 0.0);
          getVariance(variance, numWeights);

          for (int clauseno = 0; clauseno < numWeights; clauseno++)
            precond[clauseno] = 1.0/variance[clauseno];
            //precond[clauseno] = 1.0/(variance[clauseno] + 1.0);
        }

        double beta = 0.0;

          // Compute beta using Polak-Ribiere form:
          //   beta = g_j+1 (g_j+1 - g_j) / (g_j g_j)
          // Preconditioned:
          //   beta = g_j+1 M-1 (g_j+1 - g_j) / (g_j M-1 g_j)
        double beta_num = 0.0;
        double beta_denom = 0.0;
        
        if (iter > 1)
        {
          for (int i = 0; i < numWeights; i++)
          {
            beta_num   += gradient[i] * precond[i] 
                * (gradient[i] - oldgradient[i]);
            beta_denom += oldgradient[i] * precond[i] * oldgradient[i];
          }
          beta = beta_num/beta_denom;
        }
        else
          beta = 0.0;

        if (dldebug)
          cout << "Beta = " << beta << endl;
        
          // Compute new direction
        for (int w = 0; w < numWeights; w++)
          d[w] = -precond[w]*gradient[w] + beta*oldd[w];

        delete Hd;
        Hd = getHessianVectorProduct(d);
        alpha = computeQuadraticStepLength(gradient, d, Hd, cg_lambda_);

        // HACK DEBUG -- if this isn't working, ignore the conjugacy
        // criteria since it sometimes messes us up.
        if (alpha < 0.0)
        {
          for (int w = 0; w < numWeights; w++)
            d[w] = -precond[w]*gradient[w];

          delete Hd;
          Hd = getHessianVectorProduct(d);
          alpha = computeQuadraticStepLength(gradient, d, Hd, cg_lambda_);
        }
      }

        // Diagonal Newton: divide gradient by clause variance,
        //     and use Hessian to pick step length
      if (method_ == DN && !backtracked)
      {
        Array<double> variance(numWeights, 0.0);
        getVariance(variance, numWeights);

        for (int w = 0; w < numWeights; w++) 
        {
          if (fabs(gradient[w]) < 0.00000001)
            gradient[w] = 0;
          d[w] = -gradient[w]/variance[w];
        }

        delete Hd;
        Hd = getHessianVectorProduct(d);
        alpha = computeQuadraticStepLength(gradient, d, Hd, cg_lambda_);
        // alpha = learningRate; (Alternate method for choosing step length.)
      } 

      if (method_ == SIMPLE)
      {
          // Search direction is just the gradient
        for (int w = 0; w < numWeights; w++)
          d[w] = -gradient[w];

          // Step length scalar is the learning rate
        alpha = learningRate;
      }

      if (!backtracked && alpha <= 0.0)
      {
        // If alpha is negative, then either the direction or the
        // Hessian is in error.  We call this a backtrack so that
        // we can gather more samples while keeping the old samples.
        backtracked = true;
        iter--;
      }

      if (!backtracked)
      {
          // Compute total weight change
        Array<double> wchange(numWeights, 0.0);
        for (int w = 0; w < numWeights; w++) 
          wchange[w] = d[w] * alpha + (weights[w] - oldweights[w]) * momentum;

        // Convergence criteria for 2nd order methods: 
        // Stop when the maximum predicted improvement in log likelihood 
        // is very small.
        const double MIN_LL_CHANGE = 0.0001;
        double maxchange = -dotprod(gradient, wchange, numWeights);
        cout << "Maximum estimated improvement: " << maxchange << endl;
        if ((method_ == CG || method_ == DN) &&
                maxchange < MIN_LL_CHANGE) {
            cout << "Upper bound is less than " << MIN_LL_CHANGE
                << "; halting learning." << endl;
            break;
        }

        // Save weights, gradient, and direction and adjust the weights
        for (int w = 0; w < numWeights; w++)
        {
          oldweights[w] = weights[w];
          oldd[w] = d[w];
          oldgradient[w] = gradient[w];

          weights[w] += wchange[w];
          averageWeights[w] = (iter * averageWeights[w] + weights[w])/(iter + 1);
        }

        // Predict the next change to the gradient using the Hessian
        delete delta_pred;
        delta_pred = getHessianVectorProduct(wchange);

        // Reset counts.  
        // (If we're backtracking, we DO NOT want to
        // reset counts, since we're restoring the old counts. 
        // That's why this is handled here.)
        for (int i = 0; i < domainCnt_; i++) 
          inferences_[i]->resetCnts();
      }

      bool print = !backtracked && (!periodicMLNs || isOutputIter(iter));
      if (print)
      {
        cout << "Weights:\tNew\t\tAverage\n";
        for (int w = 0; w < numWeights; w++)
          cout << w << ":\t\t" << weights[w] << "\t\t"
               << averageWeights[w] << endl; 
      }

      // done with an iteration
    }
    
    cout << endl << "Learned Weights : " << endl;
    for (int w = 0; w < numWeights; w++) 
    {
        // If using MWS, then assign average weights to MLN
      if (dynamic_cast<MaxWalkSat*>(inferences_[0]))
      {
        weights[w] = averageWeights[w];
      }
      cout << w << ":" << weights[w] << endl;
    }

    delete [] averageWeights;
    delete [] gradient;
    
    resetDBs();
  }
 
 
 private:
 
  /**
   * Resets the values of non-evidence predicates as they were before learning.
   */
  void resetDBs() 
  {
    if (!lazyInference_)
    {
      for (int i = 0; i < domainCnt_; i++) 
      {
        VariableState* state = inferences_[i]->getState();
        Database* db = state->getDomain()->getDB();
          // Change known NE to original values
        const GroundPredicateHashArray* knePreds = state->getKnePreds();
        const Array<TruthValue>* knePredValues = state->getKnePredValues();      
        db->setValuesToGivenValues(knePreds, knePredValues);
          // Set unknown NE back to UKNOWN
        const GroundPredicateHashArray* unePreds = state->getUnePreds();
        for (int predno = 0; predno < unePreds->size(); predno++) 
          db->setValue((*unePreds)[predno], UNKNOWN);
      }
    }
  }

  /**
   * Assign true to the elements in the relevantClauses_ bool array 
   * corresponding to indices of clauses which would be relevant for list of 
   * non-evidence predicates.
   */
  void findRelevantClauses(const StringHashArray& nonEvidPredNames) 
  {
    for (int d = 0; d < domainCnt_; d++)
    {
      int clauseCnt = clauseCntPerDomain_[d];
      Array<bool>& relevantClauses = relevantClausesPerDomain_[d];
      relevantClauses.growToSize(clauseCnt);
      memset((bool*)relevantClauses.getItems(), false, 
             relevantClauses.size()*sizeof(bool));
      const Domain* domain = inferences_[d]->getState()->getDomain();
      const MLN* mln = inferences_[d]->getState()->getMLN();
    
      const Array<IndexClause*>* indclauses;
      const Clause* clause;
      int predid, clauseid;
      for (int i = 0; i < nonEvidPredNames.size(); i++)
      {
        predid = domain->getPredicateId(nonEvidPredNames[i].c_str());
        //cout << "finding the relevant clauses for predid = " << predid 
        //     << " in domain " << d << endl;
        indclauses = mln->getClausesContainingPred(predid);
        if (indclauses) 
        {
          for (int j = 0; j < indclauses->size(); j++) 
          {
            clause = (*indclauses)[j]->clause;			
            clauseid = mln->findClauseIdx(clause);
            
            // If clause is external to this mln, then it stays irrelevant
            if (!mln->isExternalClause(clauseid)) relevantClauses[clauseid] = true;
          }
        }
      }    
    }
  }

  
  void findRelevantClausesFormulas()
  {
    if (idxTrans_ == NULL)
    {
      Array<bool>& relevantClauses = relevantClausesPerDomain_[0];
      relevantClausesFormulas_.growToSize(relevantClauses.size());
      for (int i = 0; i < relevantClauses.size(); i++)
        relevantClausesFormulas_[i] = relevantClauses[i];
    }
    else
    {
      idxTrans_->setRelevantClausesFormulas(relevantClausesFormulas_,
                                            relevantClausesPerDomain_[0]);
      cout << "Relevant clauses/formulas:" << endl;
      idxTrans_->printRelevantClausesFormulas(cout, relevantClausesFormulas_);
      cout << endl;
    }
  }


  /**
   * Calculate true/false/unknown counts for all clauses for the given domain.
   * 
   * @param trueCnt Number of true groundings for each clause is stored here.
   * @param falseCnt Number of false groundings for each clause is stored here.
   * @param domainIdx Index of domain where the groundings are counted.
   * @param hasUnknownPreds If true, the domain has predicates with unknown
   * truth values. Otherwise it contains only predicates with known values.
   */
  void calculateCounts(Array<double>& trueCnt, Array<double>& falseCnt,
                       const int& domainIdx, const bool& hasUnknownPreds) 
  {
    Clause* clause;
    double tmpUnknownCnt;
    int clauseCnt = clauseCntPerDomain_[domainIdx];
    Array<bool>& relevantClauses = relevantClausesPerDomain_[domainIdx];
    const MLN* mln = inferences_[domainIdx]->getState()->getMLN();
    const Domain* domain = inferences_[domainIdx]->getState()->getDomain();

    for (int clauseno = 0; clauseno < clauseCnt; clauseno++) 
    {
      if (!relevantClauses[clauseno]) 
      {
        continue;
        //cout << "\n\nthis is an irrelevant clause.." << endl;
      }
      clause = (Clause*) mln->getClause(clauseno);
      clause->getNumTrueFalseUnknownGroundings(domain, domain->getDB(), 
                                               hasUnknownPreds,
                                               trueCnt[clauseno],
                                               falseCnt[clauseno],
                                               tmpUnknownCnt);
      assert(hasUnknownPreds || (tmpUnknownCnt==0));
    }
  }


  void initializeWts(const StringHashArray& nonEvidPredNames)
  {
    cout << "Initializing weights ..." << endl;
    Array<double *> trainFalseCnts;
    trainTrueCnts_.growToSize(domainCnt_);
    trainFalseCnts.growToSize(domainCnt_);

    for (int i = 0; i < domainCnt_; i++)
    {
      if (dldebug) cout << "Domain " << i << endl;
      int clauseCnt = clauseCntPerDomain_[i];
      if (dldebug) cout << "Clause count: " << clauseCnt << endl;
      VariableState* state = inferences_[i]->getState();
      const GroundPredicateHashArray* unePreds = state->getUnePreds();
      const GroundPredicateHashArray* knePreds = state->getKnePreds();

      trainTrueCnts_[i] = new double[clauseCnt];
      trainFalseCnts[i] = new double[clauseCnt];

      if (dldebug)
      {
        cout << "Unknown non-evid preds: " << unePreds->size() << endl;
        cout << "Known non-evid preds: " << knePreds->size() << endl;
      }
      int totalPreds = unePreds->size() + knePreds->size();
        // Used to store gnd preds to be ignored in the count because they are
        // UNKNOWN
      Array<bool>* unknownPred = new Array<bool>;
      unknownPred->growToSize(totalPreds, false);
      for (int predno = 0; predno < totalPreds; predno++) 
      {
        GroundPredicate* p;
        if (predno < unePreds->size())
          p = (*unePreds)[predno];
        else
          p = (*knePreds)[predno - unePreds->size()];
        TruthValue tv = state->getDomain()->getDB()->getValue(p);

        //assert(tv != UNKNOWN);
        //bool activate = true;
        bool activate = false;
        if (tv == TRUE)
        {
          state->setValueOfAtom(predno + 1, true, activate, -1);
          p->setTruthValue(true);
        }
        else
        {
          state->setValueOfAtom(predno + 1, false, activate, -1);
          p->setTruthValue(false);
            // Can have unknown truth values when using EM. We want to ignore
            // these when performing the counts
          if (tv == UNKNOWN)
          {
            (*unknownPred)[predno] = true;
          }
        }
      }

      state->initMakeBreakCostWatch();
      //cout<<"getting true cnts => "<<endl;
      state->getNumClauseGndingsWithUnknown(trainTrueCnts_[i], clauseCnt, true,
                                            unknownPred);
      //cout<<endl;
      //cout<<"getting false cnts => "<<endl;
      state->getNumClauseGndingsWithUnknown(trainFalseCnts[i], clauseCnt, false,
                                            unknownPred);
      delete unknownPred;
      if (dldebug)
      {
        for (int clauseno = 0; clauseno < clauseCnt; clauseno++)
        {
          cout << clauseno << " : tc = " << trainTrueCnts_[i][clauseno]
               << " ** fc = " << trainFalseCnts[i][clauseno] << endl;
        }
      }
    }

    combinedTrueCnts_.growToSize(clauseCntPerDomain_[0]);
    combinedTotalCnts_.growToSize(clauseCntPerDomain_[0]);

    double tc,fc;
    cout << "List of CNF Clauses : " << endl;
    for (int clauseno = 0; clauseno < clauseCntPerDomain_[0]; clauseno++)
    {
      if (!relevantClausesPerDomain_[0][clauseno])
      {
        for (int i = 0; i < domainCnt_; i++)
        {
          Array<double>& logOdds = logOddsPerDomain_[i];
          logOdds[clauseno] = 0.0;
        }
        continue;
      }
      //cout << endl << endl;
      cout << clauseno << ":";
      const Clause* clause =
        inferences_[0]->getState()->getMLN()->getClause(clauseno);
      //cout << (*fncArr)[clauseno]->formula <<endl;
      clause->print(cout, inferences_[0]->getState()->getDomain());
      cout << endl;
      
      tc = 0.0; fc = 0.0;
      for (int i = 0; i < domainCnt_;i++)
      {
        tc += trainTrueCnts_[i][clauseno];
        fc += trainFalseCnts[i][clauseno];
      }
	
      //cout << "true count  = " << tc << endl;
      //cout << "false count = " << fc << endl;
	
      double weight = 0.0;
      double totalCnt = tc + fc;

      int nonEvidPreds = 0;
      for (int i = 0; i < clause->getNumPredicates(); i++)
      {
          const char* predName 
              = clause->getPredicate(i)->getTemplate()->getName();
          if (nonEvidPredNames.contains(predName)) 
            nonEvidPreds++;
      }

      combinedTrueCnts_[clauseno]  = (tc > 0) ? tc : 1;
      combinedTotalCnts_[clauseno] = (totalCnt > 0) ? totalCnt : 1;
		
      if (totalCnt == 0 || nonEvidPreds == 0) 
      {
        //cout << "NOTE: Total count is 0 for clause " << clauseno << endl;
        weight = EPSILON;
      } 
      else 
      {
        if (fc == 0.0)
          fc = 0.00001;
        if (tc == 0.0)
          tc = 0.00001;

        double priorodds = (pow(2.0, nonEvidPreds) - 1.0)/1.0;

        weight = log(tc/fc) - log(priorodds);

          /*
        double prob =  tc / (tc+fc);
        if (prob == 0) prob = 0.00001;
        if (prob == 1) prob = 0.99999;
        weight = log(prob/(1-prob));
          //if weight exactly equals 0, make it small non zero, so that clause  
          //is not ignored during the construction of the MRF
        //if(weight == 0) weight = 0.0001;
          //commented above - make sure all weights are positive in the
          //beginning
        //if(weight < EPSILON) weight = EPSILON;
        if (abs(weight) < EPSILON) weight = EPSILON;
          //cout << "Prob " << prob << " becomes weight of " << weight << endl;
          */
      }
      for (int i = 0; i < domainCnt_; i++) 
      {
      	Array<double>& logOdds = logOddsPerDomain_[i];
        logOdds[clauseno] = weight;
      }
    }
    cout << endl;
    
    for (int i = 0; i < trainFalseCnts.size(); i++)
      delete[] trainFalseCnts[i];
  }

  /**
   * Finds the training counts and intialize the weights for the lazy version.
   * True and false groundings have to be counted for each first-order clause
   * (this is stored in each grounding while building the mrf in the eager
   * version).
   * 
   * @param nonEvidPredNames List of non-evidence predicates.
   */
  void findCountsInitializeWtsAndSetNonEvidPredsToUnknownInDB(
                                       const StringHashArray& nonEvidPredNames)
  {
    bool hasUnknownPreds;
    Array<Array<double> > totalFalseCnts; 
    Array<Array<double> > defaultFalseCnts;
    totalFalseCnts.growToSize(domainCnt_);
    defaultFalseCnts.growToSize(domainCnt_);
    
    Array<Predicate*> gpreds;
    Array<Predicate*> ppreds;
    Array<TruthValue> gpredValues;
    Array<TruthValue> tmpValues;

    for (int i = 0; i < domainCnt_; i++) 
    {
      const Domain* domain = inferences_[i]->getState()->getDomain();
      int clauseCnt = clauseCntPerDomain_[i];
      domain->getDB()->setPerformingInference(false);

      //cout << endl << "Getting the counts for the domain " << i << endl;
      gpreds.clear();
      gpredValues.clear();
      tmpValues.clear();
      for (int predno = 0; predno < nonEvidPredNames.size(); predno++) 
      {
        ppreds.clear();
        int predid = domain->getPredicateId(nonEvidPredNames[predno].c_str());
        Predicate::createAllGroundings(predid, domain, ppreds);
        //cout<<"size of gnd for pred " << predid << " = "<<ppreds.size()<<endl;
        gpreds.append(ppreds);
      }
      
      domain->getDB()->alterTruthValue(&gpreds, UNKNOWN, FALSE, &gpredValues);
	  
      //cout <<"size of unknown set for domain "<<i<<" = "<<gpreds.size()<<endl;
      //cout << "size of the values " << i << " = " << gpredValues.size()<<endl;
	
      hasUnknownPreds = false;
      
      Array<double>& trueCnt = totalTrueCnts_[i];
      Array<double>& falseCnt = totalFalseCnts[i];
      trueCnt.growToSize(clauseCnt);
      falseCnt.growToSize(clauseCnt);
      calculateCounts(trueCnt, falseCnt, i, hasUnknownPreds);

      //cout << "got the total counts..\n\n\n" << endl;
      
      hasUnknownPreds = true;

      domain->getDB()->setValuesToUnknown(&gpreds, &tmpValues);

      Array<double>& dTrueCnt = defaultTrueCnts_[i];
      Array<double>& dFalseCnt = defaultFalseCnts[i];
      dTrueCnt.growToSize(clauseCnt);
      dFalseCnt.growToSize(clauseCnt);
      calculateCounts(dTrueCnt, dFalseCnt, i, hasUnknownPreds);

      //commented out: no need to revert the grounded non-evidence predicates
      //               to their initial values because we want to set ALL of
      //               them to UNKNOWN
      //assert(gpreds.size() == gpredValues.size());
      //domain->getDB()->setValuesToGivenValues(&gpreds, &gpredValues);
	  
      //cout << "the ground predicates are :" << endl;
      for (int predno = 0; predno < gpreds.size(); predno++) 
        delete gpreds[predno];

      domain->getDB()->setPerformingInference(true);
    }
    //cout << endl << endl;
    //cout << "got the default counts..." << endl;     
    for (int clauseno = 0; clauseno < clauseCntPerDomain_[0]; clauseno++) 
    {
      double tc = 0;
      double fc = 0;
      for (int i = 0; i < domainCnt_; i++) 
      {
      	Array<bool>& relevantClauses = relevantClausesPerDomain_[i];
      	Array<double>& logOdds = logOddsPerDomain_[i];
      
        if (!relevantClauses[clauseno]) { logOdds[clauseno] = 0; continue; }
        tc += totalTrueCnts_[i][clauseno] - defaultTrueCnts_[i][clauseno];
        fc += totalFalseCnts[i][clauseno] - defaultFalseCnts[i][clauseno];

        if (dldebug)
          cout << clauseno << " : tc = " << tc << " ** fc = "<< fc <<endl;      
      }
      
      double weight = 0.0;

      if ((tc + fc) == 0) 
      {
        //cout << "NOTE: Total count is 0 for clause " << clauseno << endl;
      } 
      else 
      {
        double prob = tc / (tc+fc);
        if (prob == 0) prob = 0.00001;
        if (prob == 1) prob = 0.99999;
        weight = log(prob / (1-prob));
            //if weight exactly equals 0, make it small non zero, so that clause
            //is not ignored during the construction of the MRF
        //if (weight == 0) weight = 0.0001;
        if (abs(weight) < EPSILON) weight = EPSILON;
          //cout << "Prob " << prob << " becomes weight of " << weight << endl;
      }
      
      	// Set logOdds in all domains to the weight calculated
      for(int i = 0; i < domainCnt_; i++) 
      { 
      	Array<double>& logOdds = logOddsPerDomain_[i];
        logOdds[clauseno] = weight;
      }
    }
  }
 
  
  /**
   * Runs inference using the current set of parameters.
   */
  void infer() 
  {
    //const int INIT_FREQUENCY = 5;
    //static int inferCount = 0;

    for (int i = 0; i < domainCnt_; i++) 
    {
      VariableState* state = inferences_[i]->getState();
      state->setGndClausesWtsToSumOfParentWts();
      // MWS: Search is started from state at end of last iteration
      state->init();
      inferences_[i]->infer();
      state->saveLowStateToGndPreds();
    }
  }

  /**
   * Infers values for predicates with unknown truth values and uses these
   * values to compute the training counts.
   */
  void fillInMissingValues()
  {
    assert(withEM_);
    cout << "Filling in missing data ..." << endl;
      // Get values of initial unknown preds by producing MAP state of
      // unknown preds given known evidence and non-evidence preds (VPEM)
    Array<Array<TruthValue> > ueValues;
    ueValues.growToSize(domainCnt_);
    for (int i = 0; i < domainCnt_; i++)
    {
      VariableState* state = inferences_[i]->getState();
      const Domain* domain = state->getDomain();
      const GroundPredicateHashArray* knePreds = state->getKnePreds();
      const Array<TruthValue>* knePredValues = state->getKnePredValues();

        // Mark known non-evidence preds as evidence
      domain->getDB()->setValuesToGivenValues(knePreds, knePredValues);

        // Infer missing values
      state->setGndClausesWtsToSumOfParentWts();
        // MWS: Search is started from state at end of last iteration
      state->init();
      inferences_[i]->infer();
      state->saveLowStateToGndPreds();

      if (dldebug)
      {
        cout << "Inferred following values: " << endl;
        inferences_[i]->printProbabilities(cout);
      }

        // Compute counts
      if (lazyInference_)
      {
        Array<double>& trueCnt = totalTrueCnts_[i];
        Array<double> falseCnt;
        bool hasUnknownPreds = false;
        falseCnt.growToSize(trueCnt.size());
        calculateCounts(trueCnt, falseCnt, i, hasUnknownPreds);
      }
      else
      {
        int clauseCnt = clauseCntPerDomain_[i];
        state->initMakeBreakCostWatch();
        //cout<<"getting true cnts => "<<endl;
        const Array<double>* clauseTrueCnts =
          inferences_[i]->getClauseTrueCnts();
        assert(clauseTrueCnts->size() == clauseCnt);
        int numSamples = inferences_[i]->getNumSamples();
        for (int j = 0; j < clauseCnt; j++)
          trainTrueCnts_[i][j] = (*clauseTrueCnts)[j]/numSamples;
      }

        // Set evidence values back
      //assert(uePreds.size() == ueValues[i].size());
      //domain->getDB()->setValuesToGivenValues(&uePreds, &ueValues[i]);
        // Set non-evidence values to unknown
      Array<TruthValue> tmpValues;
      tmpValues.growToSize(knePreds->size());
      domain->getDB()->setValuesToUnknown(knePreds, &tmpValues);
    }
    cout << "Done filling in missing data" << endl;    
  }

  
  // Get the gradient of the negative log likelihood for one domain
  void getGradientForDomain(double* const & gradient, const int& domainIdx)
  {
    Array<bool>& relevantClauses = relevantClausesPerDomain_[domainIdx];
    int clauseCnt = clauseCntPerDomain_[domainIdx];
    double* trainCnts = NULL;
    double* inferredCnts = NULL;
    double* clauseTrainCnts = new double[clauseCnt]; 
    double* clauseInferredCnts = new double[clauseCnt];
    double trainCnt, inferredCnt;
    Array<double>& totalTrueCnts = totalTrueCnts_[domainIdx];
    Array<double>& defaultTrueCnts = defaultTrueCnts_[domainIdx];    
    const MLN* mln = inferences_[domainIdx]->getState()->getMLN();
    const Domain* domain = inferences_[domainIdx]->getState()->getDomain();

    memset(clauseTrainCnts, 0, clauseCnt*sizeof(double));
    memset(clauseInferredCnts, 0, clauseCnt*sizeof(double));

    if (!lazyInference_)
    {
      if (!inferredCnts) inferredCnts = new double[clauseCnt];

      const Array<double>* clauseTrueCnts =
        inferences_[domainIdx]->getClauseTrueCnts();
      assert(clauseTrueCnts->size() == clauseCnt);
      int numSamples = inferences_[domainIdx]->getNumSamples();
      for (int i = 0; i < clauseCnt; i++)
        inferredCnts[i] = (*clauseTrueCnts)[i]/numSamples;
      trainCnts = trainTrueCnts_[domainIdx];

      // DEBUG
      cout << "numSamples = " << numSamples << endl;
    }
      //loop over all the training examples
    //cout << "\t\ttrain count\t\t\t\tinferred count" << endl << endl;
    for (int clauseno = 0; clauseno < clauseCnt; clauseno++) 
    {
      if (!relevantClauses[clauseno]) continue;
      
      if (lazyInference_)
      {
      	Clause* clause = (Clause*) mln->getClause(clauseno);

      	trainCnt = totalTrueCnts[clauseno];
      	inferredCnt =
          clause->getNumTrueGroundings(domain, domain->getDB(), false);
      	trainCnt -= defaultTrueCnts[clauseno];
      	inferredCnt -= defaultTrueCnts[clauseno];
      
      	clauseTrainCnts[clauseno] += trainCnt;
      	clauseInferredCnts[clauseno] += inferredCnt;
      }
      else
      {
        clauseTrainCnts[clauseno] += trainCnts[clauseno];
        clauseInferredCnts[clauseno] += inferredCnts[clauseno];
      }
      //cout << clauseno << ":\t\t" <<trainCnt<<"\t\t\t\t"<<inferredCnt<<endl;
    }

    if (dldebug)
    {
      cout << "net counts : " << endl;
      cout << "\t\ttrain count\t\t\t\tinferred count" << endl << endl;
    }

    for (int clauseno = 0; clauseno < clauseCnt; clauseno++) 
    {
      if (!relevantClauses[clauseno]) continue;
      
      if (dldebug)
        cout << clauseno << ":\t\t" << clauseTrainCnts[clauseno] << "\t\t\t\t"
             << clauseInferredCnts[clauseno] << endl;
        // NOTE: rescaling has been implemented to affect the gradient, 
        // but not the Hessian.
      if (rescaleGradient_ && clauseTrainCnts[clauseno] > 0)
      {
        gradient[clauseno] -= 
          (clauseTrainCnts[clauseno] - clauseInferredCnts[clauseno])
            / clauseTrainCnts[clauseno];
      }
      else
      {
        gradient[clauseno] -= clauseTrainCnts[clauseno] - 
                            clauseInferredCnts[clauseno];
      }
    }

    delete[] inferredCnts;
    delete[] clauseTrainCnts;
    delete[] clauseInferredCnts;
  }


    // Get the gradient of the negative log likelihood
  void getGradient(double* const & weights, double* const & gradient,
                   const int numWts) 
  {
    // Set the weights and run inference
    
    setMLNWeights(weights);
    
    //cout << "New Weights = **** " << endl << endl;
    //for (int i = 0; i < numWts; i++) cout << i << " : " << weights[i] << endl;

    if (withEM_) fillInMissingValues();
    cout << "Running inference ..." << endl;
    infer();
    cout << "Done with inference" << endl;

      // Compute the gradient
    memset(gradient, 0, numWts*sizeof(double));

      // There is one DB or the clauses of multiple DBs line up
    if (idxTrans_ == NULL)
    {
      for (int i = 0; i < domainCnt_; i++) 
      {		  
        //cout << "For domain number " << i << endl << endl; 
        getGradientForDomain(gradient, i);        
      }
    }
    else
    {
        // The clauses for multiple databases do not line up
      Array<Array<double> >* gradsPerDomain = idxTrans_->getGradsPerDomain();
      const Array<Array<Array<IdxDiv>*> >* cIdxToCFIdxsPerDomain 
        = idxTrans_->getClauseIdxToClauseFormulaIdxsPerDomain();
     
      for (int i = 0; i < domainCnt_; i++) 
      {		  
        //cout << "For domain number " << i << endl << endl; 

        Array<double>& grads = (*gradsPerDomain)[i];
        memset((double*)grads.getItems(), 0, grads.size()*sizeof(double));
        
        getGradientForDomain((double*)grads.getItems(), i);
        
          // map clause gradient to clause/formula gradients
        assert(grads.size() == clauseCntPerDomain_[i]);
        for (int j = 0; j < grads.size(); j++)
        {
          Array<IdxDiv>* idxDivs = (*cIdxToCFIdxsPerDomain)[i][j];          
          for (int k = 0; k < idxDivs->size(); k++)
            gradient[ (*idxDivs)[k].idx ] += grads[j] / (*idxDivs)[k].div;
        }
      }
    }

      // Add the deriative of the prior 
    if (usePrior_) 
    {
	  for (int i = 0; i < numWts; i++) 
      {
        if (!relevantClausesFormulas_[i]) continue;
        double sd = priorStdDevs_[i];
        double priorDerivative = (weights[i]-priorMeans_[i])/(sd*sd);
        //cout << i << " : " << "gradient : " << gradient[i]
        //     << "  prior gradient : " << priorDerivative;
        gradient[i] += priorDerivative; 
	    //cout << "  net gradient : " << gradient[i] << endl; 
      }
    }
  }

 public:
    // Different gradient descent methods
    // SIMPLE is Voted Perceptron
  const static int SIMPLE = 0;
  const static int DN = 2;
  const static int CG = 3;


 private:
  int domainCnt_;
  Array<Array<double> > logOddsPerDomain_;
  Array<int> clauseCntPerDomain_;

	// Used in lazy version
  Array<Array<double> > totalTrueCnts_; 
  Array<Array<double> > defaultTrueCnts_;

  Array<Array<bool> > relevantClausesPerDomain_;
  Array<bool> relevantClausesFormulas_;

	// Used to compute cnts from mrf
  Array<double*> trainTrueCnts_;
      
  Array<double>  combinedTrueCnts_;
  Array<double>  combinedTotalCnts_;

  bool usePrior_;
  const double* priorMeans_, * priorStdDevs_; 

  IndexTranslator* idxTrans_; //not owned by object; don't delete
  
  bool lazyInference_;
  bool isQueryEvidence_;
  bool rescaleGradient_;
  int  method_;
  double cg_lambda_;
  bool preconditionCG_;

  int maxBacktracks_;
  int backtrackCount_;
  double cg_max_lambda_;

  Array<Inference*> inferences_;
  
    // Using EM to fill in missing values?
  bool withEM_;
};


#endif
