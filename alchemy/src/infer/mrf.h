#ifndef MRF_H_SEP_23_2005
#define MRF_H_SEP_23_2005

#include <cstdlib>
#include <cfloat>
#include "random.h"
#include "timer.h"
#include "mln.h"
#include "groundpredicate.h"
#include "maxwalksat.h"
#include "convergencetest.h"
#include "gelmanconvergencetest.h"


enum WalksatType { NONE=0, MAXWALKSAT=1 };

struct GibbsParams
{
  int numChains;
  int burnMinSteps;
  int burnMaxSteps;
  double gamma;
  double epsilonFrac;
  int gibbsMinSteps;
  int gibbsMaxSteps;
  double fracConverged;
  WalksatType walksatType;
  int maxSeconds;
  int samplesPerConvergenceTest;
};

///////////////////////////////////////////////////////////////////////////
  //used when flip negative clause wts
struct LitWt { int lit; double wt; };

class HashLitWt
{
 public:
  int operator()(LitWt* const & lw) const  { return hash<int>()(lw->lit); }
};


class EqualLitWt
{
 public:
  bool operator()(LitWt* const & lw1, LitWt* const & lw2) const
     { return lw1->lit == lw2->lit; }
};


typedef HashArray<LitWt*, HashLitWt, EqualLitWt> LitWtHashArray;


///////////////////////////////////////////////////////////////////////////

  // used as parameter of addGndClause()
struct AddGroundClauseStruct
{
  AddGroundClauseStruct(const GroundPredicateSet* const & sseenPreds,
                        GroundPredicateSet* const & uunseenPreds,
                        Array<GroundPredicate*>* const & ggndPreds,
                        GroundPredicateToIntMap* const & ggndPredsMap,
                        const Array<int>* const & aallPredGndingsAreQueries,
                        GroundClauseSet* const & ggndClausesSet,
                        Array<GroundClause*>* const & ggndClauses,
                        const bool& mmarkHardGndClauses,
                        const double* const & pparentWtPtr,
						const int & cclauseId)
    : seenPreds(sseenPreds), unseenPreds(uunseenPreds), gndPreds(ggndPreds),
      gndPredsMap(ggndPredsMap), 
      allPredGndingsAreQueries(aallPredGndingsAreQueries),
      gndClausesSet(ggndClausesSet), gndClauses(ggndClauses),
      markHardGndClauses(mmarkHardGndClauses), parentWtPtr(pparentWtPtr),
	  clauseId(cclauseId) {}
  
  ~AddGroundClauseStruct() {}
  
  const GroundPredicateSet* seenPreds;
  GroundPredicateSet* unseenPreds;
  Array<GroundPredicate*>* gndPreds;
  GroundPredicateToIntMap* gndPredsMap;
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
      const bool& trackParentClauseWts)
  {
    random_.init(-1);
    cout << "creating mrf..." << endl; 
    Timer timer;
    GroundPredicateSet unseenPreds, seenPreds;
    GroundPredicateToIntMap gndPredsMap;
    GroundClauseSet gndClausesSet;
    gndPreds_ = new Array<GroundPredicate*>;
    gndClauses_ = new Array<GroundClause*>;

      //add GroundPredicates in queries to unseenPreds
    for (int i = 0; i < queries->size(); i++)
    {
      GroundPredicate* gp = (*queries)[i];
      unseenPreds.insert(gp);
      int gndPredIdx = gndPreds_->append(gp);
      assert(gndPredsMap.find(gp) == gndPredsMap.end());
      gndPredsMap[gp] = gndPredIdx;
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
		const int clauseId = mln->findClauseIdx(c);  
		assert(clauseId >= 0);
		
		  //ignore clause with zero weight
        if (c->getWt() == 0) continue;

          //add gnd clauses with unknown truth values to gndClauses_
        const double* parentWtPtr = (trackParentClauseWts) ? c->getWtPtr():NULL;
        AddGroundClauseStruct agc(&seenPreds, &unseenPreds, gndPreds_,
                                  &gndPredsMap, allPredGndingsAreQueries,
                                  &gndClausesSet, gndClauses_,
                                  markHardGndClauses, parentWtPtr,
								  clauseId);
       /* 
		cout<<"just before calling addunknown gnd clauses.."<<endl;
		cout<<this<<endl;
		c->print(cout,domain);
		cout<<endl<<endl;
		*/
		addUnknownGndClauses(pred,c,domain,db,genClausesForAllPredGndings,&agc);
      }

      //clauses with negative wts are flipped in prepareMaxWalksatData()

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
 
    //cout << "grounded predicates:" << endl;
    //for (int i = 0; i < gndPreds_->size(); i++)
    //{ cout << "\t"; (*gndPreds_)[i]->print(cout, domain); cout << endl; }
    //cout << "grounded clauses:" << endl;
    //for (int i = 0; i < gndClauses_->size(); i++)
    //{ cout << "\t"; (*gndClauses_)[i]->print(cout, domain); cout << endl; }

    for (int i = 0; i < gndPreds_->size(); i++) 
      (*gndPreds_)[i]->compress();
    gndPreds_->compress();
    gndClauses_->compress();

    cout <<"Time taken to construct MRF = ";Timer::printTime(cout,timer.time());
    cout << endl;
  }


    //Do not delete the clause and truncClause argument.
    //This function is tightly bound to Clause::createAndAddUnknownClause().
  static void addUnknownGndClause(const AddGroundClauseStruct* const & agcs, 
                                  const Clause* const & clause,
                                  const Clause* const & truncClause,
                                  const bool& isHardClause)
  {
    const GroundPredicateSet* seenPreds     = agcs->seenPreds;
    GroundPredicateSet*       unseenPreds   = agcs->unseenPreds;
    Array<GroundPredicate*>*  gndPreds      = agcs->gndPreds;
    GroundPredicateToIntMap*  gndPredsMap   = agcs->gndPredsMap; 
    const Array<int>* allGndingsAreQueries  = agcs->allPredGndingsAreQueries;
    GroundClauseSet*          gndClausesSet = agcs->gndClausesSet;
    Array<GroundClause*>*     gndClauses    = agcs->gndClauses;
    const bool markHardGndClauses           = agcs->markHardGndClauses;
    const double* parentWtPtr               = agcs->parentWtPtr;
    const int clauseId                      = agcs->clauseId;
	

    //check none of the grounded clause's predicates have been seen before
    //if any of them have been seen before, this clause has been created 
    //before (for that seen predicate), and can be ignored

      //check the untruncated ground clause whether any of its predicates
      //have been seen before
    GroundClause* gndClause = new GroundClause(clause);
    bool seenBefore = false;
    for (int j = 0; j < gndClause->getNumGroundPredicates(); j++)
    {
      GroundPredicate* gp = (GroundPredicate*) gndClause->getGroundPredicate(j);
      if (seenPreds->find(gp) !=  seenPreds->end() ||
          (allGndingsAreQueries && (*allGndingsAreQueries)[gp->getId()] > 1) )
      { 
        seenBefore = true;
        break;
      }
    }

    gndClause->deleteAllGndPreds();
    delete gndClause;
    if (seenBefore) return;

    gndClause = new GroundClause(truncClause);
    if (markHardGndClauses && isHardClause) gndClause->setWtToHardWt();
    assert(gndClause->getWt() != 0);


    GroundClauseSet::iterator iter = gndClausesSet->find(gndClause);
      //if the unknown clause is not in gndClauses
    if (iter == gndClausesSet->end())
    {
	  gndClausesSet->insert(gndClause);
      gndClauses->append(gndClause);
      //gndClause's wt is set when it was constructed
      if (parentWtPtr) 
      { 
        gndClause->appendParentWtPtr(parentWtPtr);
		gndClause->incrementClauseFrequency(clauseId,1); 
		
		assert(gndClause->getWt() == *parentWtPtr);
      }

        // add the unknown predicates of the clause to unseenPreds if 
        // the predicates are already not in it
      for (int j = 0; j < gndClause->getNumGroundPredicates(); j++)
      {
        GroundPredicate* gp =(GroundPredicate*)gndClause->getGroundPredicate(j);
        assert(seenPreds->find(gp) == seenPreds->end());
          // if the ground predicate is not in unseenPreds
        GroundPredicateSet::iterator it = unseenPreds->find(gp);
        if (it == unseenPreds->end())
        {
          //cout << "\tinserting into unseen pred: ";  
          //pred->print(cout, domain); cout << endl;
          unseenPreds->insert(gp);
          //commented out: done when gndClause is initialized
          //gp->appendGndClause(gndClause,predSense);              
          int gndPredIdx = gndPreds->append(gp);
          assert(gndPredsMap->find(gp) == gndPredsMap->end());
          (*gndPredsMap)[gp] = gndPredIdx;
          gndClause->setGroundPredicateIndex(j, gndPredIdx);
        }
        else
        {
          GroundPredicate* origGndPred = *it;
          gndClause->replaceGroundPredicate(j, origGndPred);
          bool predSense = gndClause->getGroundPredicateSense(j);
          bool ok = origGndPred->appendGndClause(gndClause, predSense);
          assert(ok); ok = true; // avoid compilation warning
          
            // origGndPred must already be in gndPreds
          int gndPredIdx = (*(gndPredsMap->find(origGndPred))).second;
          assert(gndPredIdx >= 0);
          gndClause->setGroundPredicateIndex(j, gndPredIdx);
        }
      }
    }
    else
    {  // gndClause has appeared before, so just accumulate its weight
      (*iter)->addWt(gndClause->getWt());

	  //Change as suggeted by STAN : Mar 8, 2006
      //if (parentWtPtr) gndClause->appendParentWtPtr(parentWtPtr);
	  if (parentWtPtr)
	  {
		   (*iter)->appendParentWtPtr(parentWtPtr);
           (*iter)->incrementClauseFrequency(clauseId,1); 
	  }

	  gndClause->deleteAllGndPreds();
      delete gndClause;
    }
  } //addUnknownGndClause()



  ~MRF()
  {
    for (int i = 0; i < gndClauses_->size(); i++)
      delete (*gndClauses_)[i];
    delete gndClauses_;    

    for (int i = 0; i < gndPreds_->size(); i++)
      if ((*gndPreds_)[i]) delete (*gndPreds_)[i];
    delete gndPreds_;
  }


  void deleteGndClausesIntArrReps() 
  {
    for (int i = 0; i < gndClauses_->size(); i++)
      (*gndClauses_)[i]->deleteIntArrRep();    
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


  const Array<GroundPredicate*>* getGndPreds() const { return gndPreds_; }

  const Array<GroundClause*>* getGndClauses() const { return gndClauses_; }

  void initGndPredsTruthValues(const int& numChains)
  {
    for (int i = 0; i < gndPreds_->size(); i++)
      (*gndPreds_)[i]->initTruthValues(numChains);
  }


    // If saveSapce is set to true, the gndPredSenses_ and gndPredIndexes_
    // fields of the GroundClauses in gndClauses_ will be deleted in the 
    // function. This means that the function can only be correctly called once.
    // Returns the number of samples drawn per predicate.
  int runGibbsSampling(const GibbsParams& gibbsParams, 
                       const MaxWalksatParams* const & maxWalksatParams,
                       const bool& saveSpace, const Domain* const & domain)
  {
    // this occurs when all query predicates don't appear in any UNKNOWN clause 
    if (gndClauses_->size()==0) return 0;
    Timer timer;
    double begSec;
      //extract Gibbs sampling parameters
    int numChains = gibbsParams.numChains;
    WalksatType walksatType = gibbsParams.walksatType;
    double gamma = gibbsParams.gamma;
    double epsilonFrac = gibbsParams.epsilonFrac;
    int samplesPerConvergenceTest = gibbsParams.samplesPerConvergenceTest;
    int burnMinSteps = gibbsParams.burnMinSteps;
    int burnMaxSteps = gibbsParams.burnMaxSteps;
    int gibbsMinSteps = gibbsParams.gibbsMinSteps;
    int gibbsMaxSteps = gibbsParams.gibbsMaxSteps;
    double fracConverged = gibbsParams.fracConverged;
    int maxSeconds = gibbsParams.maxSeconds;
    
    int numGndPreds = gndPreds_->size();

      //initialize gndPreds' truthValues & wts, and gndClauses' numSatLiterals
    for (int i = 0; i < gndPreds_->size(); i++)
      (*gndPreds_)[i]->initTruthValuesAndWts(numChains);

    begSec = timer.time();
    cout << "initializing ground predicates values "; 
    if (walksatType == MAXWALKSAT)
    {
      assert(maxWalksatParams);
      cout << "with MaxWalksat" << endl;
      maxWalksatInitGndPredsTruthValues(numChains, maxWalksatParams);
    }
    else
    {
      cout << "randomly " << endl;
      randomInitGndPredsTruthValues(numChains);
    }
    cout << "initialization took "; Timer::printTime(cout, timer.time()-begSec);
    cout << endl;

    for (int i = 0; i < gndClauses_->size(); i++)
      (*gndClauses_)[i]->initNumSatLiterals(numChains);


    if (saveSpace)
    {
        // Delete to save space. Can be deleted because gndPredSenses and 
        // gndPredIndexes are not required after being used by MaxWalksat
      for (int i = 0; i < gndClauses_->size(); i++)
      {
        (*gndClauses_)[i]->deleteGndPredSenses();
        (*gndClauses_)[i]->deleteGndPredIndexes();
      }
    }

      //initialize convergence test
    GelmanConvergenceTest** burnConvergenceTests;
    ConvergenceTest** gibbsConvergenceTests;
    initConvergenceTests(burnConvergenceTests, gibbsConvergenceTests, 
                         gamma, epsilonFrac, numGndPreds, numChains);

    int numSamplesPerPred = 0, isamp = -1;
    bool done      = false;
    bool burningIn = true;
  
    double secondsElapsed = 0;
    double startTimeSec = timer.time();
    double currentTimeSec;

    GroundPredicateHashArray affectedGndPreds;

    for (int i = 0; i < gndPreds_->size(); i++) 
      affectedGndPreds.append((*gndPreds_)[i], gndPreds_->size());
    for (int c = 0; c < numChains; c++) 
      updateWtsForAffectedGndPreds(affectedGndPreds, c);        
    affectedGndPreds.clear();


    cout << "running Gibbs sampling..." << endl;
      // start burn in and Gibbs sampling
    while(!done) 
    {
      ++isamp;

      if (isamp % samplesPerConvergenceTest == 0)
      { 
        currentTimeSec = timer.time();
        secondsElapsed = currentTimeSec-startTimeSec;
        cout << "Sample (per pred per chain) " << isamp << ", time elapsed = ";
        Timer::printTime(cout,secondsElapsed); cout << endl;
      }

        // for each chain, for each node, generate the node's new truth value
      for (int c = 0; c < numChains; c++) 
      {
        for (int i = 0; i < gndPreds_->size(); i++) 
        {
          bool newAssignment 
            = genTruthValueForProb((*gndPreds_)[i]->getProbability(c));

            // if gndPred is flipped, do updates & find all affected gndPreds
          if (newAssignment != (*gndPreds_)[i]->getTruthValue(c)) 
          {            
            (*gndPreds_)[i]->setTruthValue(c,newAssignment);
            affectedGndPreds.clear();
            gndPredFlippedUpdates(i, c, affectedGndPreds);
            updateWtsForAffectedGndPreds(affectedGndPreds, c);
          }

            // if in actual gibbs sampling phase, track the num of times
            // the ground predicate is set to true
          if (!burningIn && newAssignment) (*gndPreds_)[i]->incrementNumTrue();
        }
        if (!burningIn) numSamplesPerPred++;
      }

  
        // add current truth values to the convergence testers
      for (int i = 0; i < gndPreds_->size(); i++) 
      {        
        const bool* vals = (*gndPreds_)[i]->getTruthValues()->getItems();
          //WARNING: implicit cast from bool* to double*
        if (burningIn) burnConvergenceTests[i]->appendNewValues(vals);
        else           gibbsConvergenceTests[i]->appendNewValues(vals);
      }


      if (isamp % samplesPerConvergenceTest != 0) continue;      
      if (burningIn) 
      {
          // use convergence criteria stated in "Probability and Statistics",
          // DeGroot and Schervish
        bool burnConverged 
          = GelmanConvergenceTest::checkConvergenceOfAll(burnConvergenceTests,
                                                         gndPreds_->size(),
                                                         true);
        if (   (isamp >= burnMinSteps && burnConverged)
            || (burnMaxSteps >= 0 && isamp >= burnMaxSteps) 
            || (maxSeconds > 0 && secondsElapsed >= maxSeconds))
        {
          cout << "Done burning. " << isamp << " samples per pred per chain (" 
               << (burnConverged? "converged":"didn't converge") 
               <<" at total of " << numChains*isamp << " samples per pred)" 
               << endl;
          burningIn = false;
          isamp=0;          
        }
      }
      else 
      {  // going actual gibbs sampling
        bool gibbsConverged 
          = ConvergenceTest::checkConvergenceOfAtLeast(gibbsConvergenceTests, 
                                                       gndPreds_->size(), isamp,
                                                       fracConverged, true);
        if (   (isamp >= gibbsMinSteps && gibbsConverged) 
            || (gibbsMaxSteps >= 0 && isamp >= gibbsMaxSteps) 
            || (maxSeconds > 0 && secondsElapsed >= maxSeconds)) 
        {
          cout << "Done Gibbs sampling. " << isamp 
               << " samples per pred per chain ("
               << (gibbsConverged? "converged":"didn't converge") 
               <<" at total of " << numSamplesPerPred << " samples per pred)" 
               << endl;
          done = true;
        }
      }
      cout.flush();
    } // while (!done);    


    cout<< "Time taken for Gibbs sampling = "; 
    Timer::printTime(cout, timer.time()-startTimeSec); cout << endl;

      // update gndPreds_ probability that it is true
    for (int i = 0; i < gndPreds_->size(); i++)
      (*gndPreds_)[i]->setProbTrue((*gndPreds_)[i]->getNumTrue()
                                   / numSamplesPerPred);
    
    deleteConvergenceTests(burnConvergenceTests, gibbsConvergenceTests, 
                           numGndPreds);
    
    return numSamplesPerPred;
  }

    //returns true if MaxWalksat succeeded; else returns false
  bool runMaxWalksat(const int& chainIdx, const bool& randomInitIfFail,
                     const MaxWalksatParams* const & params, 
                     double& hardClauseWt)
  {
    cout << "running MaxWalksat..." << endl;

    //NOTE: using wall clock time since MaxWalksat is a separate process 
    //      use util/Timer instead when this is changed to our customized
    //      memory efficient/fast MaxWalksat
    struct timeval tv; struct timezone tzp;
    gettimeofday(&tv,&tzp);

    MaxWalksatData* data = prepareMaxWalksatData();
    hardClauseWt = data->hardClauseWt;
    MaxWalksat mws;
    Array<Array<bool>*> solutions;
    mws.sample(data, chainIdx, solutions, params);
    
    cout << "MaxWalksat sampling for chain " << chainIdx << "..." << endl;
    
    bool good = true;
    if (solutions.size() == 0) 
    {
      cout << "MaxWalksat was unable to find a solution." << endl;
      good = false;
    }
    else
    if (solutions.size() > 1) 
    {
      cout << "MaxWalksat unexpectedly returned multiple solutions." << endl;
      good = false;
    }

    if (gndPreds_->size() != data->numGndPreds) 
    {
      cout << "Error in MRF: gndPreds_->size() != data->numGndPreds." << endl;
      good = false;
    }

    if (solutions.size() == 1 &&
        gndPreds_->size() != solutions[0]->size()) 
    {
      cout << "Error in MRF: gndPreds_->size() != solutions[0]->size()." <<endl;
      good = false;
    }

 
    if (good)
    {
      Array<bool>* solution = solutions[0];
      for (int i = 0; i < solution->size(); i++)
        (*gndPreds_)[i]->setTruthValue(chainIdx, (*solution)[i] );
    }
    else 
    if (randomInitIfFail)
    {
      cout << "Randomly assigning initializing ground predicates' truth values."
           << endl;
      for (int i = 0; i < gndPreds_->size(); i++) 
        (*gndPreds_)[i]->setTruthValue(chainIdx, genTruthValueForProb(0.5) );
    }
    

    for (int i = 0; i < solutions.size(); i++)
      delete solutions[i];

    for (int i = 0; i < data->clauses.size(); i++)
      delete (data->clauses)[i];
    delete data;


    struct timeval tv2; struct timezone tzp2;
    gettimeofday(&tv2,&tzp2);
    cout << "Wall clock time is used because MaxWalksat is a separate process"
         << endl;
    cout << "MaxWalksat took "; Timer::printTime(cout, tv2.tv_sec-tv.tv_sec);
    cout << endl;

    return good;
  }


 private:
  double max(const double& a, const double& b) { if (a > b) return a; return b;}

  void addToLitWtArray(const int& lit, const double& wt, 
                       LitWtHashArray& litWtArr)
  {
    LitWt* litWt = new LitWt;
    litWt->lit = lit;
    litWt->wt  = wt;
    int a = litWtArr.find(litWt);
    if (a < 0)    litWtArr.append(litWt);
    else        { litWtArr[a]->wt += litWt->wt; delete litWt; } 
  }


  void storeFlippedGndPreds(const GroundClause* const & gndClause, 
                            const double& wt, LitWtHashArray& litWtArr)
  {
    int numGndPreds = gndClause->getNumGroundPredicates();
    double negWt = -wt/numGndPreds;
    for (int j = 0; j < numGndPreds; j++)
    {
      int idx = gndClause->getGroundPredicateIndex(j);
      int lit = (gndClause->getGroundPredicateSense(j)) ?
                MaxWalksat::makeNegatedLiteral(idx) : idx;
      addToLitWtArray(lit, negWt, litWtArr);
    }
  }


    //The caller is responsible for deleting returned  maxWalksatData.
  MaxWalksatData* prepareMaxWalksatData()
  {  
      //this is a reasonable number to prevent the external executable 
      //MaxWalksat from have having overflow errors
    int maxAllowedWt = 4000;

      // prepare the propositional theory for use in MaxWalksat
    MaxWalksatData* data = new MaxWalksatData;
    data->numGndPreds = gndPreds_->size();

      //used to hold lits in unit clauses (including those formed by flipping 
      //negative wt clauses)
    LitWtHashArray litWtArr;
    Array<GroundClause*> negWtClauses;
    Array<GroundClause*> hardGndClauses; //used to store hard ground clauses

      // process ground clauses except the hard ones
    for (int i = 0; i < gndClauses_->size(); i++) 
    {
      GroundClause* gndClause = (*gndClauses_)[i];

      if (gndClause->isHardClause()){hardGndClauses.append(gndClause);continue;}

      assert(!gndClause->isHardClause());
      double wt = gndClause->getWt();

        // if the wt is negative, make it positive, flip the gnd preds, and 
        // distribute the wt evenly among them
      if (wt < 0) { negWtClauses.append(gndClause); continue; }

      int numGndPreds = gndClause->getNumGroundPredicates();
      if (numGndPreds == 1)
      {
        int idx = gndClause->getGroundPredicateIndex(0);
        int lit = (gndClause->getGroundPredicateSense(0)) ?
                  idx : MaxWalksat::makeNegatedLiteral(idx);
        addToLitWtArray(lit, wt, litWtArr);
      }
      else
      {
        Array<int>* litClause = new Array<int>;
        litClause->growToSize(numGndPreds);
        for (int j = 0; j < numGndPreds; j++) 
        {
          int idx = gndClause->getGroundPredicateIndex(j);
          int lit = (gndClause->getGroundPredicateSense(j)) ?
                    idx : MaxWalksat::makeNegatedLiteral(idx);
          (*litClause)[j]= lit;
        }
        data->clauses.append(litClause);
        data->clauseWts.append(wt);
        //cout << wt << " " << litClause << endl;
      }
    }
      // flip the negative clause wts
    for (int i = 0; i < negWtClauses.size(); i++)
      storeFlippedGndPreds(negWtClauses[i], negWtClauses[i]->getWt(), litWtArr);

      // for each unit ground clause (including those formed by flipping 
      // neg clauses wts)
    for (int i = 0; i < litWtArr.size(); i++)
    {
      Array<int>* litClause = new Array<int>;
      litClause->growToSize(1);
      (*litClause)[0]= litWtArr[i]->lit;
      data->clauses.append(litClause);
      double wt = litWtArr[i]->wt;
      data->clauseWts.append(wt);
      delete litWtArr[i];
    }

      // find minimum wt among soft clauses
    double minWt = DBL_MAX;
    double maxWt = DBL_MIN;
    for(int i = 0; i < data->clauseWts.size(); i++)
    {
    	//cout << (data->clauseWts)[i] << " " << (data->clauses)[i] << endl;
      if ((data->clauseWts)[i] < minWt) minWt = (data->clauseWts)[i];
      else if ((data->clauseWts)[i] > maxWt) maxWt = (data->clauseWts)[i];
      assert(minWt >= 0);
      assert(maxWt >= 0);
    }
    
      //find out how much to scale weights by
    double scale = 1;
    if (maxWt > maxAllowedWt) scale = maxAllowedWt/maxWt;
    else
    {
      if (minWt < 10)
      {
        scale = 10/minWt;
        if (scale*maxWt > maxAllowedWt) scale = maxAllowedWt/maxWt;
      }
    }

      // find the sum of all the weights of soft clauses, scaling up if needed
    int sumSoftWts = 0;
    for(int i = 0; i < data->clauseWts.size(); i++)
    {
      double oldwt = (data->clauseWts)[i];
      int newwt = (int)(oldwt*scale + 0.5);
      (data->clauseWts)[i] = (double)newwt;
      sumSoftWts += newwt;
    }
    assert(sumSoftWts >= 0);
    
      // process the hard ground clauses
    int largeWt = (int)(sumSoftWts + 10*scale);
    data->hardClauseWt = (int)(largeWt/scale+0.5);
    
    for (int i = 0; i < hardGndClauses.size(); i++) 
    {
      GroundClause* gndClause = hardGndClauses[i];

      Array<int>* litClause = new Array<int>;
      litClause->growToSize(gndClause->getNumGroundPredicates());

      for (int j = 0; j < gndClause->getNumGroundPredicates(); j++) 
      {
        int idx = gndClause->getGroundPredicateIndex(j);
        int lit = (gndClause->getGroundPredicateSense(j)) ?
                  idx : MaxWalksat::makeNegatedLiteral(idx);
        (*litClause)[j]= lit;
      }
      data->clauses.append(litClause);
      data->clauseWts.append(largeWt);
    }    

    return data;
  }


  void maxWalksatInitGndPredsTruthValues(const int& numChains, 
                                         const MaxWalksatParams* const & params)
  { 
    double hardClauseWt;
    for (int c = 0; c < numChains; c++)  
      runMaxWalksat(c, true, params, hardClauseWt); 
  }


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


  void initConvergenceTests(GelmanConvergenceTest**& burnConvergenceTests,
                            ConvergenceTest**& gibbsConvergenceTests, 
                            const double& gamma, const double& epsilonFrac, 
                            const int& numGndPreds, const int& numChains)
  {
    burnConvergenceTests = new GelmanConvergenceTest*[numGndPreds];
    gibbsConvergenceTests = new ConvergenceTest*[numGndPreds];
    for (int i = 0; i < numGndPreds; i++) 
    {
      burnConvergenceTests[i]  = new GelmanConvergenceTest(numChains);
      gibbsConvergenceTests[i] = new ConvergenceTest(numChains, gamma,
                                                     epsilonFrac);
    }
  }

  
  void deleteConvergenceTests(GelmanConvergenceTest**& burnConvergenceTests,
                              ConvergenceTest**& gibbsConvergenceTests, 
                              const int& numGndPreds)
  {
    for (int i = 0; i < numGndPreds; i++) 
    {
      delete burnConvergenceTests[i];
      delete gibbsConvergenceTests[i];
    }
    delete [] burnConvergenceTests;
    delete [] gibbsConvergenceTests;
  }  


  bool genTruthValueForProb(const double& p)
  {
    if (p==1.0) return true;
    if (p==0.0) return false;
    return random_.random() <= p;    
  }


  void randomInitGndPredsTruthValues(const int& numChains)
  {
    for (int c = 0; c < numChains; c++)
      for (int i = 0; i < gndPreds_->size(); i++)
        (*gndPreds_)[i]->setTruthValue(c, genTruthValueForProb(0.5));
  }


  void gndPredFlippedUpdates(const int& gndPredIdx, const int& chainIdx,
                             GroundPredicateHashArray& affectedGndPreds)
  {
    GroundPredicate* gndPred = (*gndPreds_)[gndPredIdx];
    affectedGndPreds.append(gndPred, gndPreds_->size());
    assert(affectedGndPreds.size() <= gndPreds_->size());

    const Array<GroundClause*>* gndClauses = gndPred->getGndClauses();
    const Array<bool>* senseInGndClauses = gndPred->getSenseInGndClauses();
    assert(gndClauses->size() == senseInGndClauses->size());
    GroundClause* gndClause;

    for (int i = 0; i < gndClauses->size(); i++)
    {
      gndClause = (*gndClauses)[i];
      if (gndPred->getTruthValue(chainIdx) == (*senseInGndClauses)[i])
        gndClause->incrementNumSatLiterals(chainIdx);
      else
        gndClause->decrementNumSatLiterals(chainIdx);

      assert(gndClause->getNumSatLiterals(chainIdx)
             <= gndClause->getNumGroundPredicates());

      for (int j = 0; j < gndClause->getNumGroundPredicates(); j++)
      {
        affectedGndPreds.append((GroundPredicate*)
                                 gndClause->getGroundPredicate(j), 
                                 gndPreds_->size());
        assert(affectedGndPreds.size() <= gndPreds_->size());
      }
    }
  }


  void updateWtsForAffectedGndPreds(GroundPredicateHashArray& affectedGndPreds,
                                    const int& chainIdx)
  {
      // for each ground predicate whose MB has changed
    for (int g = 0; g < affectedGndPreds.size(); g++)
    {

      GroundPredicate* gndPred = affectedGndPreds[g];
      double wtIfNoChange = 0, wtIfInverted = 0, wt;
      const Array<GroundClause*>* gndClauses = gndPred->getGndClauses();
      const Array<bool>* senseInGndClauses = gndPred->getSenseInGndClauses();
      assert(gndClauses->size() == senseInGndClauses->size());

      for (int i = 0; i < gndClauses->size(); i++)
      {      
        wt = (*gndClauses)[i]->getWt();
        if ((*gndClauses)[i]->getNumSatLiterals(chainIdx) > 1) 
        {
            // some other literal is making it sat, so it doesn't matter
          wtIfNoChange += wt;
          wtIfInverted += wt;
        }
        else
        if ((*gndClauses)[i]->getNumSatLiterals(chainIdx) == 1) 
        {
          wtIfNoChange += wt;
            // if the current truth value is the same as its sense in gndClause
          if (gndPred->getTruthValue(chainIdx) == (*senseInGndClauses)[i]) 
          {
            // This gndPred is the only one making this function satisfied
          }
          else 
          {
              // Some other literal is making it satisfied
            wtIfInverted += wt;
          }
        }
        else
        if ((*gndClauses)[i]->getNumSatLiterals(chainIdx) == 0) 
        {
          // None satisfy, so when gndPred switch to its negative, it'll satisfy
          wtIfInverted += wt;
        }
      } // for each ground clause that gndPred appears in


      if (gndPred->getTruthValue(chainIdx)) 
      {
        gndPred->setWtWhenTrue(chainIdx, wtIfNoChange);
        gndPred->setWtWhenFalse(chainIdx, wtIfInverted);
      }
      else 
      {
        gndPred->setWtWhenFalse(chainIdx, wtIfNoChange);
        gndPred->setWtWhenTrue(chainIdx, wtIfInverted);
      }
    } // for each ground predicate whose MB has changed
  }

 public:

  
// initialize the structure to store the number of satisfied literals in
// each clause
void initNumSatLiterals(const int& numChains)
{
  for(int i=0;i<gndClauses_->size();i++)
  {
	(*gndClauses_)[i]->initNumSatLiterals(numChains);
  }
}
  
// get the number of clause gndings using the first order clause frequencies
void getNumClauseGndings(double numGndings[], int clauseCnt, bool tv,
						 int chainIdx, const Domain *domain)
{
  IntPairItr itr;
  IntPair *clauseFrequencies;
	 
	 //cout<<"ground clause frequency stats => "<<endl;
  for(int clauseno=0;clauseno<clauseCnt;clauseno++)
    numGndings[clauseno] = 0;

  for(int i=0;i<gndClauses_->size();i++)
  {
	GroundClause *gndClause = (*gndClauses_)[i];
	int satLitcnt = gndClause->getNumSatLiterals(chainIdx);
	if(tv == true && satLitcnt == 0)
	  continue;
	if(tv == false && satLitcnt > 0)
	  continue;
		  
	clauseFrequencies = gndClause->getClauseFrequencies();
	for(itr=clauseFrequencies->begin();itr != clauseFrequencies->end();itr++)
	{
	  int clauseno = itr->first;
	  int frequency = itr->second;
		   //cout<<"cno = "<<clauseno<<" ** f = "<<frequency<<" ,";
	  numGndings[clauseno] += frequency;
	}
	      //cout<<endl;
  }
	 /*
	 for(int clauseno=0;clauseno<clauseCnt;clauseno++)
		  cout<<"number of "<<tv<<" groundings for clause "<<clauseno
			   <<" = "<<numGndings[clauseno]<<endl;
     */
}

 private:
  Array<GroundPredicate*>* gndPreds_;
  Array<GroundClause*>* gndClauses_;
  Random random_;
};


#endif
