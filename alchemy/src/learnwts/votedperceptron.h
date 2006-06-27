#ifndef VOTED_PERCEPTRON_H_OCT_30_2005
#define VOTED_PERCEPTRON_H_OCT_30_2005

#include "infer.h"
#include "clause.h"
#include "timer.h"
#include "indextranslator.h"
#include "lazywalksat.h"

const double EPSILON=.00001;
const int numChain = 1;
const int chainIdx = 0;

class VotedPerceptron 
{
 public:
  VotedPerceptron(const Array<MLN*>& mlns, const Array<Domain*>& domains,
                  const string& nonEvidPredsStr, const int& maxSteps,
                  const int& tries, const int& targetWt, const bool& hard,
                  IndexTranslator* const & idxTrans, const int& memLimit,
                  const bool& lazyInference)
    : domainCnt_(domains.size()), idxTrans_(idxTrans)
  { 
    cout << endl << "Constructing voted perceptron..." << endl << endl;
    //cout << "domain count = " << domainCnt_ << endl;

    assert(mlns.size() == domains.size());
    domains_.append(domains);
    mlns_.append(mlns);

    logOddsPerDomain_.growToSize(domainCnt_);
    clauseCntPerDomain_.growToSize(domainCnt_);
    
    for (int i = 0; i < domainCnt_; i++)
    {
      clauseCntPerDomain_[i] = mlns[i]->getNumClauses();
      logOddsPerDomain_[i].growToSize(clauseCntPerDomain_[i], 0);

      //cout<< "number of clauses in domain " << i << " = "
      //    << clauseCntPerDomain_[i] <<endl;
    }

	memLimit_ = memLimit;
	lazyInference_ = lazyInference;
	lazyReset_ = true;
	lwinfo = NULL;
	lw = NULL;
	    
	  ///////////////////////// determine if MaxWalkSat can be instantiated /////////////////////	
  	unsigned long pages;
    unsigned long bytesPerPage;
  	unsigned long kilobytes;

  	pages = sysconf(_SC_PHYS_PAGES);
  	bytesPerPage = sysconf(_SC_PAGESIZE);
  	kilobytes = pages*(bytesPerPage/1024);
	  // If memory limit was given by user, use it
  	if (memLimit_ == -1)
      memLimit_ = kilobytes;
    
  	if (!lazyInference_)
  	{
    	// Compute upper bound on memory needed
      long double memNeeded = 0;
	  for (int i = 0; i < domainCnt_; i++)
	  {
	  	long double numConstants = domains_[i]->getNumConstants();
        int highestArity = domains_[i]->getHighestPredicateArity();
        
          // Upper bound on memory needed = (size of predicate) * (constants ^ arity)
      	memNeeded += pow(numConstants, highestArity)*
                     (((double)(sizeof(Predicate) + sizeof(Array<Term*>) + sizeof(Array<int>) +
                        		highestArity * (sizeof(Term*) + sizeof(int)))) / 1024.0);
	  }

	  printf("Approximate memory needed for MaxWalkSat: %12lu kbytes\n", (unsigned long)memNeeded);      
	  
      if (memNeeded > memLimit_)
      {
	  	printf("Memory needed exceeds memory available (%d kbytes), using LazySat\n", memLimit_);      
    	lazyInference_ = true;
      }
      else
      {
      	//Remove evidence atoms structure from DBs
      	for (int i = 0; i < domainCnt_; i++)
	  	{
	  	  domains_[i]->getDB()->setLazyFlag(false);
	  	}
      }
  	}
    
    totalTrueCnts_.growToSize(domainCnt_);
    defaultTrueCnts_.growToSize(domainCnt_);
    relevantClausesPerDomain_.growToSize(domainCnt_);
    gndPredsPerDomain_.growToSize(domainCnt_);
    gndPredValuesPerDomain_.growToSize(domainCnt_);
    equivalentGndPredsPerDomain_.growToSize(domainCnt_);
    mrfs_.growToSize(domainCnt_);
    //relevantClausesFormulas_ is set in findRelevantClausesFormulas()
    
    StringHashArray nonEvidPredNames;
    extractPredNames(nonEvidPredsStr, NULL, nonEvidPredNames);//defined in infer.h
    findRelevantClauses(nonEvidPredNames);
    findRelevantClausesFormulas();

	if (lazyInference_)
	{
      findCountsInitializeWtsAndSetNonEvidPredsToUnknownInDB(nonEvidPredNames);
    
      for (int i = 0; i < mlns_.size(); i++)
      {
      	MLN* mln = mlns_[i];
      	Array<double>& logOdds = logOddsPerDomain_[i];
      	assert(mln->getNumClauses() == logOdds.size());
      	for (int j = 0; j < mln->getNumClauses(); j++)
          ((Clause*) mln->getClause(j))->setWt(logOdds[j]);
 	  }
	}
	else // !lazyInference_
	{
	  Array<int> allPredGndingsAreNonEvid;
      bool markHardGndClauses = false;
      bool trackParentClauseWts = true;

	  GroundPredicateHashArray knownPreds;
      Array<Predicate *> nonEvidPreds;
	  Array<TruthValue> nonEvidPredValues;
	  for(int i = 0; i < domainCnt_; i++) 
      {
        Domain* domain = domains_[i];
      	MLN* mln = mlns_[i];
      	GroundPredicateHashArray& gndPreds = gndPredsPerDomain_[i];
      	Array<Predicate*>& equivalentGndPreds = equivalentGndPredsPerDomain_[i];
      	Array<TruthValue>& gndPredValues = gndPredValuesPerDomain_[i];
      	MRF*& mrf = mrfs_[i];

	  	  //need to set some dummy weight
      	for (int j = 0; j < mln->getNumClauses(); j++)
          ((Clause*) mln->getClause(j))->setWt(1);

        knownPreds.clear();
        readPredValuesAndSetToUnknown(nonEvidPredNames, domain, nonEvidPreds,
        							  nonEvidPredValues, false);
	 
		cout<<"size of non evid preds = "<<nonEvidPreds.size()<<endl;
      	allPredGndingsAreNonEvid.growToSize(domain->getNumPredicates(), false);

      	  //defined in infer.h
	    createComLineQueryPreds(nonEvidPredsStr, domain, domain->getDB(), 
        	                    &gndPreds, &knownPreds, 
            	                &allPredGndingsAreNonEvid);
      	  //cout << "created the com line query .." << endl;

	    cout << endl << "constructing ground MRF for domain " << i << "..."<<endl;
    	mrf = new MRF(&gndPreds, &allPredGndingsAreNonEvid, domain, 
        		      domain->getDB(), mln, markHardGndClauses, 
            	      trackParentClauseWts, -1);
      	  //cout << "created mrf ..." << endl;
      	mrf->deleteGndClausesIntArrReps();
      	mrf->deleteGndPredsGndClauseSets();      
	  
	  	  //revert to older values
	  	domains_[i]->getDB()->setValuesToGivenValues(&nonEvidPreds,&nonEvidPredValues);
	  	for(int nepredno = 0; nepredno < nonEvidPreds.size(); nepredno++) 
          delete nonEvidPreds[nepredno];
        
	  	equivalentGndPreds.growToSize(gndPreds.size(), NULL);
      	gndPredValues.growToSize(gndPreds.size(), FALSE);

      	for (int predno = 0; predno < gndPreds.size(); predno++) 
      	{        
          Predicate* p = gndPreds[predno]->createEquivalentPredicate(domain);
          equivalentGndPreds[predno] = p;
          gndPredValues[predno] = domain->getDB()->getValue(p);
      	}
      	for (int i = 0; i < knownPreds.size(); i++) delete knownPreds[i];
      }
      cout << endl << "done constructing ground MRFs" << endl << endl;
      
      	//now initialize the clause wts
	  initializeWts();
    }

      //intialize the maxwalksat parameters
    wsparams_ = NULL;
    wsparams_ = new MaxWalksatParams;
    wsparams_->maxSteps = maxSteps;
    wsparams_->tries = tries;
    wsparams_->targetWt = targetWt;
    wsparams_->hard = hard;
    wsparams_->lazyGnded = false;
  	wsparams_->lazyNoApprox = false;
  }


  ~VotedPerceptron() 
  {
    if (!lazyInference_)
    {
      for(int i = 0; i < mrfs_.size(); i++) delete mrfs_[i];

	  for(int i = 0; i < equivalentGndPredsPerDomain_.size(); i++)
    	equivalentGndPredsPerDomain_[i].deleteItemsAndClear();
    }
    
    delete wsparams_;
  }


    // set the prior means and std devs.
  void setMeansStdDevs(const int& arrSize, const double* const & priorMeans, 
                       const double* const & priorStdDevs) 
  {
    if(arrSize < 0) 
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


    // learn the weights
  void learnWeights(double* const & weights, const int& numWeights,
                    const int& maxIter, const double& learningRate) 
  {
    //cout << "Learning weights discriminatively... " << endl;
    memset(weights, 0, numWeights*sizeof(double));

    double* averageWeights = new double[numWeights];
    double* gradient = new double[numWeights];

	  //set the initial weight to the average log odds across domains/databases

      //if there is one db or the clauses for multiple databases line up
    if (idxTrans_ == NULL)
    {
      for (int i = 0; i < domains_.size(); i++)
      {
        Array<double>& logOdds = logOddsPerDomain_[i];
        assert(numWeights == logOdds.size());
        for (int j = 0; j < logOdds.size(); j++) weights[j] += logOdds[j];
      }
    }
    else
    {   //the clauses for multiple databases do not line up
      const Array<Array<Array<IdxDiv>*> >* cIdxToCFIdxsPerDomain 
        = idxTrans_->getClauseIdxToClauseFormulaIdxsPerDomain();

  	  Array<int> numLogOdds; 
      Array<double> wtsForDomain;
      numLogOdds.growToSize(numWeights);
      wtsForDomain.growToSize(numWeights);
      
      for (int i = 0; i < domains_.size(); i++)
      {
        memset((int*)numLogOdds.getItems(), 0, numLogOdds.size()*sizeof(int));
        memset((double*)wtsForDomain.getItems(), 0, 
      	 	   wtsForDomain.size()*sizeof(double));

        Array<double>& logOdds = logOddsPerDomain_[i];
        
          //map the each log odds of a clause to the weight of a clause/formula
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
    {      
      weights[i] /= domains_.size();
      averageWeights[i] = weights[i];
    }

    for (int iter = 1; iter <= maxIter; iter++) 
    {
      cout << endl << "Iteration " << iter << " : " << endl << endl;
      cout << "Getting the gradient.. " << endl;
      getGradient(weights, gradient, numWeights);
      cout << endl; 

        // add gradient to weights
      for (int w = 0; w < numWeights; w++) 
      {
        double wchange = gradient[w] * learningRate;
        cout << "clause/formula " << w << ": wtChange = " << wchange;
        cout << "  oldWt = " << weights[w];
        double oldWt = weights[w];
        weights[w] += gradient[w] * learningRate;
        cout << "  newWt = " << weights[w];
          // If weight of one clause goes from neg. to pos. or vice versa, then reset lazysat
        if ((oldWt < 0 && weights[w] > 0) || (oldWt > 0 && weights[w] < 0))
          lazyReset_ = true;
        averageWeights[w] = (iter * averageWeights[w] + weights[w])/(iter + 1);
        cout << "  averageWt = " << averageWeights[w] << endl;
      }
      // done with an iteration
    }
    
    cout << endl << "Learned Weights : " << endl;
    for (int w = 0; w < numWeights; w++) 
    {
      weights[w] = averageWeights[w];
      cout << w << ":" << weights[w] << endl;
    }

    delete [] averageWeights;
    delete [] gradient;
    
    resetDBs();
  }
 
 
 private: 
    //reset the values of non-evidence predicates
  void resetDBs() 
  {
    for(int i = 0; i < domainCnt_; i++) 
    {
      Database* db = domains_[i]->getDB();
	    db->setValuesToGivenValues(&(equivalentGndPredsPerDomain_[i]), 
                                   &(gndPredValuesPerDomain_[i]));
    }
  }

    // assign true to the elements in the relevantClauses_ bool array 
    // corresponding to indices of clauses which would be relevant for list of 
    // non-evidence predicates 
  void findRelevantClauses(const StringHashArray& nonEvidPredNames) 
  {
    for (int d = 0; d < domainCnt_; d++)
    {
      int clauseCnt = clauseCntPerDomain_[d];
      Array<bool>& relevantClauses = relevantClausesPerDomain_[d];
      relevantClauses.growToSize(clauseCnt);
      memset((bool*)relevantClauses.getItems(), false, 
             relevantClauses.size()*sizeof(bool));
      Domain* domain = domains_[d];
      MLN* mln = mlns_[d];
    
      const Array<IndexClause*>* indclauses;
      const Clause* clause;
      int predid, clauseid;
      for(int i = 0;i < nonEvidPredNames.size(); i++)
      {
        predid = domain->getPredicateId(nonEvidPredNames[i].c_str());
        //cout << "finding the relevant clauses for predid = " << predid 
        //     << " in domain " << d << endl;
        indclauses = mln->getClausesContainingPred(predid);
        if (indclauses) 
        {
          for(int j = 0; j < indclauses->size(); j++) 
          {
            clause = (*indclauses)[j]->clause;			
            clauseid = mln->findClauseIdx(clause);
            relevantClauses[clauseid] = true;
            //cout << clauseid << " ";
          }
          //cout<<endl;
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


    // Calculate true/false/unknown counts for all clauses for the given domain 
  void calculateCounts(Array<double>& trueCnt, Array<double>& falseCnt,
                       const int& domainIdx, const bool& hasUnknownPreds) 
  {
    Clause* clause;
    double tmpUnknownCnt;
    int clauseCnt = clauseCntPerDomain_[domainIdx];
    Array<bool>& relevantClauses = relevantClausesPerDomain_[domainIdx];
    MLN* mln = mlns_[domainIdx];
    Domain* domain = domains_[domainIdx];

    for (int clauseno = 0; clauseno < clauseCnt; clauseno++) 
    {
      if(!relevantClauses[clauseno]) 
      {
        continue;
        //cout << "\n\nthis is an irrelevant clause.." << endl;
	  }
      //cout << clauseno << ": ";
      clause = (Clause*) mln->getClause(clauseno);
      clause->getNumTrueFalseUnknownGroundings(domain, domain->getDB(), 
                                               hasUnknownPreds,
                                               trueCnt[clauseno],
                                               falseCnt[clauseno],
                                               tmpUnknownCnt);
      //cout << "true = " << trueCnt[clauseno] << " ";
      //cout << "false = " << falseCnt[clauseno] << " ";
      //cout << "unknown = " << tmpUnknownCnt << endl;

	  assert(hasUnknownPreds || (tmpUnknownCnt==0));
    }
  }


  void initializeWts()
  { 
  	Array<double *> trainFalseCnts;
  	trainTrueCnts_.growToSize(domainCnt_);
  	trainFalseCnts.growToSize(domainCnt_);
  
  	for(int i = 0; i < domainCnt_; i++)
  	{
      int clauseCnt = clauseCntPerDomain_[i];
      GroundPredicateHashArray& gndPreds = gndPredsPerDomain_[i];
	  Array<Predicate*>& equivalentGndPreds = equivalentGndPredsPerDomain_[i];
	  
      //Array<TruthValue>& gndPredValues = gndPredValuesPerDomain_[i];

      mrfs_[i]->initGndPredsTruthValues(numChain);
	
	  trainTrueCnts_[i] = new double[clauseCnt];
	  trainFalseCnts[i] = new double[clauseCnt];
	  for (int predno = 0; predno < gndPreds.size(); predno++) 
      {
        Predicate *p = equivalentGndPreds[predno];
        TruthValue tv = domains_[i]->getDB()->getValue(p);
		assert(tv != UNKNOWN);
		 
		if(tv == TRUE)
		  gndPreds[predno]->setTruthValue(chainIdx,true);
		else
		  gndPreds[predno]->setTruthValue(chainIdx,false);
	  }
   
      mrfs_[i]->initNumSatLiterals(numChain);	
      //cout<<"getting true cnts => "<<endl;
	  mrfs_[i]->getNumClauseGndings(trainTrueCnts_[i], clauseCnt, true, chainIdx, domains_[i]);
	  //cout<<endl;
      //cout<<"getting false cnts => "<<endl;
	  mrfs_[i]->getNumClauseGndings(trainFalseCnts[i], clauseCnt, false, chainIdx, domains_[i]);
    
	  //for (int clauseno=0; clauseno < clauseCnt; clauseno++) 
      //{
      	//if(!relevantClauses[clauseno]) 
      	  //continue;
      	//cout<<clauseno<<" : tc = "<<trainTrueCnts_[i][clauseno]
			          //<<" ** fc = "<<trainFalseCnts[i][clauseno]<<endl;      
	  //}
	
   /*
	bool hasUnknownPreds = false;
    calculateCounts(trainTrueCnts_[i],trainFalseCnts[i],i,hasUnknownPreds);
  */
	
	}  
			
  	double tc,fc;
  	cout << "List of CNF Clauses : " << endl;
  	for (int clauseno = 0; clauseno < clauseCntPerDomain_[0]; clauseno++) 
  	{
      if(!relevantClausesPerDomain_[0][clauseno]) 
      {
      	for(int i = 0; i < domainCnt_; i++) 
      	{
		  Array<double>& logOdds = logOddsPerDomain_[i];
          logOdds[clauseno] = 0.0;
        }
      	continue;
      }
      //cout << endl << endl;  
	  cout << clauseno << ":";
	  const Clause* clause = mlns_[0]->getClause(clauseno);	
      //cout << (*fncArr)[clauseno]->formula <<endl;
      clause->print(cout, domains_[0]); 
      cout << endl;
      
	  tc = 0.0; fc = 0.0; 
      for(int i = 0; i < domainCnt_;i++) 
      {
		tc += trainTrueCnts_[i][clauseno]; 
        fc += trainFalseCnts[i][clauseno];
	  }
		
      //cout << "true count  = " << tc << endl;
      //cout << "false count = " << fc << endl;
		
      double weight = 0.0;
      double totalCnt = tc + fc;
		
      if(totalCnt == 0) 
      {
        //cout << "NOTE: Total count is 0 for clause " << clauseno << endl;
      } 
      else 
      {
        double prob =  tc/ (tc+fc);
        if (prob == 0) prob = 0.00001;
        if (prob == 1) prob = 0.99999;
        weight = log(prob/(1-prob));
          //if weight exactly equals 0, make it small non zero, so that clause  
          //is not ignored during the construction of the MRF
        //if(weight == 0) weight = 0.0001;
		//commented above - make sure all weights are positive in the
		//beginning
        //if(weight < EPSILON) weight = EPSILON;
        if(abs(weight) < EPSILON) weight = EPSILON;
        
		//cout << "Prob " << prob << " becomes weight of " << weight << endl;
      }
      for(int i = 0; i < domainCnt_; i++) 
      {
      	Array<double>& logOdds = logOddsPerDomain_[i];
        logOdds[clauseno] = weight;
      }
    }
    cout << endl;
    
	for (int i = 0; i < trainFalseCnts.size(); i++)
      delete[] trainFalseCnts[i];
  }



    //Find the training counts and intialize the weights 
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

    for(int i = 0; i < domainCnt_; i++) 
    {
      Domain* domain = domains_[i];
      int clauseCnt = clauseCntPerDomain_[i];

      //cout << endl << "Getting the counts for the domain " << i << endl;
      gpreds.clear();
      gpredValues.clear();
      tmpValues.clear();
      for(int predno = 0; predno < nonEvidPredNames.size(); predno++) 
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

      //cout<< "size of unknown set for domain " << i << " = " 
      //    << gpreds.size() << endl;
      //cout<< "size of corresponding value set " << i <<" = "
      //    << gpredValues.size() << endl;

      //commented out: no need to revert the grounded non-evidence predicates to
      //               their initial values because we want to set ALL of them
      //               to UNKNOWN	  
      //assert(gpreds.size() == gpredValues.size());
      //domain->getDB()->setValuesToGivenValues(&gpreds, &gpredValues);
	  
      //cout << "the ground predicates are :" << endl;
      for(int predno = 0; predno < gpreds.size(); predno++) 
        delete gpreds[predno];
    }
    //cout << endl << endl;
    //cout << "got the default counts..." << endl;
  
       
    //currently, we use the log odds to initialize the clause weights
/*
    for(int i = 0; i < domainCnt_; i++) 
    {
      Array<bool>& relevantClauses = relevantClausesPerDomain_[i];
      int clauseCnt = clauseCntPerDomain_[i];
      Domain* domain = domains_[i];
      MLN* mln = mlns_[i];
      Array<double>& logOdds = logOddsPerDomain_[i];
      
      cout << "List of relevant CNF Clauses : " << endl;
      for (int clauseno = 0; clauseno < clauseCnt; clauseno++) 
      {
        if(!relevantClauses[clauseno]) { logOdds[clauseno] = 0; continue; }
        //cout << endl << endl;  
        cout << clauseno << ": ";
        const Clause* clause = mln->getClause(clauseno);	
        clause->printWithoutWtWithStrVar(cout, domain); cout << endl;

		double tc = totalTrueCnts_[i][clauseno] - defaultTrueCnts_[i][clauseno];
        double fc = totalFalseCnts[i][clauseno] - defaultFalseCnts[i][clauseno];
		
        //cout << "true count  = " << tc << endl;
        //cout << "false count = " << fc << endl;
		
        double weight = 0.0;
		
        if((tc + fc) == 0) 
        {
          //cout << "NOTE: Total count is 0 for clause " << clauseno << endl;
        } 
        else 
        {
          double prob =  tc/ (tc+fc);
          if (prob == 0) prob = 0.00001;
          if (prob == 1) prob = 0.99999;
          weight = log(prob/(1-prob));
            //if weight exactly equals 0, make it small non zero, so that clause
            //is not ignored during the construction of the MRF
          if (weight == 0) weight = 0.0001;
          //cout << "Prob " << prob << " becomes weight of " << weight << endl;
        }
        
        logOdds[clauseno] = weight;
      }
      cout << endl;
    }
    */
     
    for (int clauseno = 0; clauseno < clauseCntPerDomain_[0]; clauseno++) 
    {
	  double tc = 0;
	  double fc = 0;
	  for(int i = 0; i < domainCnt_; i++) 
      {
      	Array<bool>& relevantClauses = relevantClausesPerDomain_[i];
      	Array<double>& logOdds = logOddsPerDomain_[i];
      
        if(!relevantClauses[clauseno]) { logOdds[clauseno] = 0; continue; }

		tc += totalTrueCnts_[i][clauseno] - defaultTrueCnts_[i][clauseno];
        fc += totalFalseCnts[i][clauseno] - defaultFalseCnts[i][clauseno];
		
        //cout << "true count  = " << tc << endl;
        //cout << "false count = " << fc << endl;
      }
      
      double weight = 0.0;
		
      if((tc + fc) == 0) 
      {
        //cout << "NOTE: Total count is 0 for clause " << clauseno << endl;
      } 
      else 
      {
        double prob =  tc/ (tc+fc);
        if (prob == 0) prob = 0.00001;
        if (prob == 1) prob = 0.99999;
        weight = log(prob/(1-prob));
            //if weight exactly equals 0, make it small non zero, so that clause
            //is not ignored during the construction of the MRF
        if (weight == 0) weight = 0.0001;
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
 
  
  //run maxwalksat using the current set of parameters  
  void walksatInfer() 
  {
    double hardClauseWt = -1;
    bool initIfMaxWalksatFails = false;
    
    for(int i = 0; i < domainCnt_; i++) 
    {
      Domain* domain = domains_[i];
	  MLN* mln = mlns_[i];
      
	  if (lazyInference_)
	  {
	  	domain->getDB()->setPerformingInference(true);
	  	if (lazyReset_)
	  	{
	  	  domain->getDB()->resetActiveStatus();
	  	  domain->getDB()->resetDeactivatedStatus();
  		  if (lwinfo)
  		  {
  		  	//lwinfo->setAllInactive();
  		  	delete lwinfo;
  		  }
  		  if (lw) delete lw;
	  	  lwinfo = new LWInfo(mln, domain);
		  lw = new LazyWalksat(lwinfo, memLimit_);
	  	}
  		int numSolutions = 1;
  		bool includeUnsatSolutions = true;
  		Array<Array<bool>*> solutions;
  		Array<int> numBad;
    
  		  // run LazyWalksat
  		lw->infer(wsparams_, numSolutions, includeUnsatSolutions,
  				  solutions, numBad, lazyReset_);
  		  //If the number of clauses in mem. has doubled, then start lw over
  		if (lw->getNumClauses() > 2 * lw->getNumInitClauses())
  		  lazyReset_ = true;
  		else
  		  lazyReset_ = false;
	  }
	  else // !lazyInference_
	  {
	    GroundPredicateHashArray& gndPreds = gndPredsPerDomain_[i];
    	Array<Predicate*>& equivalentGndPreds = equivalentGndPredsPerDomain_[i];
      	MRF* mrf = mrfs_[i];
      	mrf->initGndPredsTruthValues(numChain);
      	mrf->setGndClausesWtsToSumOfParentWts();
      	if (mrf->runMaxWalksat(chainIdx, initIfMaxWalksatFails, wsparams_,
        	                   hardClauseWt)) 
      	{
	    	//cout << "maxwalksat successfully completed.." << endl;
          for(int predno = 0; predno < gndPreds.size(); predno++) 
          {
          	TruthValue tv 
              = gndPreds[predno]->getTruthValue(chainIdx) ? TRUE : FALSE;
          	Predicate* p = equivalentGndPreds[predno];
          	domain->getDB()->setValue(p, tv);
          }
      	}
      	else 
      	{
	      cout << "maxwalksat did not successfully complete" << endl;
	      exit(-1);
      	}
	  }
    }
  }


  void getGradientForDomain(double* const & gradient, const int& domainIdx)
  {
    Array<bool>& relevantClauses = relevantClausesPerDomain_[domainIdx];
    int clauseCnt = clauseCntPerDomain_[domainIdx];
    double *trainCnts, *inferredCnts;
    trainCnts = NULL;
    inferredCnts = NULL;
    double* clauseTrainCnts = new double[clauseCnt]; 
    double* clauseInferredCnts = new double[clauseCnt];
    double trainCnt, inferredCnt;
    Array<double>& totalTrueCnts = totalTrueCnts_[domainIdx];
    Array<double>& defaultTrueCnts = defaultTrueCnts_[domainIdx];    
    MLN* mln = mlns_[domainIdx];
    Domain* domain = domains_[domainIdx];

    memset(clauseTrainCnts, 0, clauseCnt*sizeof(double));
    memset(clauseInferredCnts, 0, clauseCnt*sizeof(double));
	
	if (!lazyInference_)
	{
	  if(!inferredCnts) inferredCnts = new double[clauseCnt];
      mrfs_[domainIdx]->initNumSatLiterals(numChain);	
      mrfs_[domainIdx]->getNumClauseGndings(inferredCnts, clauseCnt, true, chainIdx, domain);
	  trainCnts = trainTrueCnts_[domainIdx];
	} 
      //loop over all the training examples
    //cout << "\t\ttrain count\t\t\t\tinferred count" << endl << endl;
    for (int clauseno = 0; clauseno < clauseCnt; clauseno++) 
    {
      if(!relevantClauses[clauseno]) continue;
      
      if (lazyInference_)
      {
      	Clause* clause = (Clause*) mln->getClause(clauseno);			       

      	trainCnt = totalTrueCnts[clauseno];
      	inferredCnt = clause->getNumTrueGroundings(domain, domain->getDB(),false);
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
	 
    //cout << "net counts : " << endl;
    //cout << "\t\ttrain count\t\t\t\tinferred count" << endl << endl;
	for (int clauseno = 0; clauseno < clauseCnt; clauseno++) 
    {
      if(!relevantClauses[clauseno]) continue;
      //cout << clauseno << ":\t\t" << clauseTrainCnts[clauseno] << "\t\t\t\t"
      //     << clauseInferredCnts[clauseno] << endl;
      gradient[clauseno] += clauseTrainCnts[clauseno] - 
                            clauseInferredCnts[clauseno];
	}
	 
	delete[] clauseTrainCnts;  
	delete[] clauseInferredCnts;
  }


    // Get the gradient 
  void getGradient(double* const & weights, double* const & gradient,
                   const int numWts) 
  {
    //set the weights and run WalkSat
    
    //cout << "New Weights = **** " << endl << endl;
    
      //if there is one db or the clauses for multiple databases line up
    if (idxTrans_ == NULL)
    {
      int clauseCnt = clauseCntPerDomain_[0];
      for (int i = 0; i < domains_.size(); i++)
      {
        Array<bool>& relevantClauses = relevantClausesPerDomain_[i];
        assert(clauseCntPerDomain_[i] == clauseCnt);
        MLN* mln = mlns_[i];
        
        for (int j = 0; j < clauseCnt; j++) 
        {
          Clause* c = (Clause*) mln->getClause(j);
          if(relevantClauses[j]) c->setWt(weights[j]);
          else                   c->setWt(0);
        }
      }
    }
    else
    {   //the clauses for multiple databases do not line up
      Array<Array<double> >* wtsPerDomain = idxTrans_->getWtsPerDomain();
      const Array<Array<Array<IdxDiv>*> >* cIdxToCFIdxsPerDomain 
        = idxTrans_->getClauseIdxToClauseFormulaIdxsPerDomain();
      
      for (int i = 0; i < domains_.size(); i++)
      {
        Array<double>& wts = (*wtsPerDomain)[i];
        memset((double*)wts.getItems(), 0, wts.size()*sizeof(double));

          //map clause/formula weights to clause weights
        for (int j = 0; j < wts.size(); j++)
        {
          Array<IdxDiv>* idxDivs =(*cIdxToCFIdxsPerDomain)[i][j];          
          for (int k = 0; k < idxDivs->size(); k++)
            wts[j] += weights[ (*idxDivs)[k].idx ] / (*idxDivs)[k].div;
        }
      }
      

      for (int i = 0; i < domains_.size(); i++)
      {
        Array<bool>& relevantClauses = relevantClausesPerDomain_[i];
        int clauseCnt = clauseCntPerDomain_[i];
        Array<double>& wts = (*wtsPerDomain)[i];
        assert(wts.size() == clauseCnt);
        MLN* mln = mlns_[i];

        for (int j = 0; j < clauseCnt; j++)
        {
          Clause* c = (Clause*) mln->getClause(j);
          if(relevantClauses[j]) c->setWt(wts[j]);
          else                   c->setWt(0);
        }
      }
    }
    //for (int i = 0; i < numWts; i++) cout << i << " : " << weights[i] << endl;

    walksatInfer();

    //compute the gradient
    memset(gradient, 0, numWts*sizeof(double));

      //there is one DB or the clauses of multiple DBs line up
    if (idxTrans_ == NULL)
    {
      for(int i = 0; i < domainCnt_; i++) 
      {		  
        //cout << "For domain number " << i << endl << endl; 
        getGradientForDomain(gradient, i);        
      }
    }
    else
    {    
        //the clauses for multiple databases do not line up
      Array<Array<double> >* gradsPerDomain = idxTrans_->getGradsPerDomain();
      const Array<Array<Array<IdxDiv>*> >* cIdxToCFIdxsPerDomain 
        = idxTrans_->getClauseIdxToClauseFormulaIdxsPerDomain();
     
      for(int i = 0; i < domainCnt_; i++) 
      {		  
        //cout << "For domain number " << i << endl << endl; 

        Array<double>& grads = (*gradsPerDomain)[i];
        memset((double*)grads.getItems(), 0, grads.size()*sizeof(double));
        
        getGradientForDomain((double*)grads.getItems(), i);
        
          // map clause gradient to clause/formula gradients
        assert(grads.size() == clauseCntPerDomain_[i]);
        for (int j = 0; j < grads.size(); j++)
        {
          Array<IdxDiv>* idxDivs =(*cIdxToCFIdxsPerDomain)[i][j];          
          for (int k = 0; k < idxDivs->size(); k++)
            gradient[ (*idxDivs)[k].idx ] += grads[j] / (*idxDivs)[k].div;
        }
      }
    }
      
    
      // add the deriative of the prior 
    if(usePrior_) 
    {
	  for (int i = 0; i < numWts; i++) 
      {
        if(!relevantClausesFormulas_[i]) continue;
        double priorDerivative = -(weights[i]-priorMeans_[i])/
                                 (priorStdDevs_[i]*priorStdDevs_[i]);
        //cout << i << " : " << "gradient : " << gradient[i]
        //     << "  prior gradient : " << priorDerivative;
        gradient[i] += priorDerivative; 
	      //cout << "  net gradient : " << gradient[i] << endl; 
      }
    }
  }



 private:
  int domainCnt_;
  Array<Domain*> domains_;  
  Array<MLN*> mlns_;
  Array<Array<double> > logOddsPerDomain_;
  Array<int> clauseCntPerDomain_;
  Array<Array<double> > totalTrueCnts_; 
  Array<Array<double> > defaultTrueCnts_;
  Array<Array<bool> > relevantClausesPerDomain_;
  Array<bool> relevantClausesFormulas_;

    //predicates corresponding to the groundings of the non-evidence predicates
  Array<GroundPredicateHashArray> gndPredsPerDomain_;
   
   //actual truth values of ground preds
  Array<Array<TruthValue> > gndPredValuesPerDomain_;  
   
    //equivalent predicates are the ground non-evidence predicates represented 
    //in the original Predicate representation
  Array<Array<Predicate*> > equivalentGndPredsPerDomain_;
  Array<MRF*> mrfs_;

	// Used to compute cnts from mrf
  Array<double*> trainTrueCnts_;

  bool usePrior_;
  const double* priorMeans_, * priorStdDevs_; 

  MaxWalksatParams* wsparams_;
  IndexTranslator* idxTrans_; //not owned by object; don't delete
  
  int memLimit_;
  bool lazyInference_;
  bool lazyReset_;
  
  LWInfo* lwinfo;
  LazyWalksat* lw;
  
};


#endif
