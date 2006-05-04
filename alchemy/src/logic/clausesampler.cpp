#include "clausesampler.h"
#include "clause.h"

  //clause must have more than one predicate and been canonicalized
double ClauseSampler::estimateNumTrueGroundings(Clause* const& clause, 
                                          const Predicate*const& flippedGndPred,
                                                const Domain* const& domain,
                                                double numSamples)
{
  Array<VarsGroundedType*>* vgtArr = new Array<VarsGroundedType*>;
  clause->createVarIdToVarsGroundedType(domain, vgtArr);

  Database* db = domain->getDB();
  TrueFalseGroundingsStore* tfGndingsStore 
    = domain->getTrueFalseGroundingsStore();
  
  int numPreds = clause->getNumPredicates();
  assert(numPreds > 1);
  if (numSamples <= 0) numSamples = computeNumSamples(numPreds);
  
   // marks out the range of numbers corresponding to  |Si|/sum_i |Si|
  Array<float> corrProbLimit(numPreds);
  double sumTrueGnds;
  Array<double> numTrueGndsPerPos;
  getProbInfo(clause, domain->getDB(), flippedGndPred, vgtArr, tfGndingsStore, 
              corrProbLimit, sumTrueGnds, numTrueGndsPerPos);
  
  double estX = 0, x = 0, x2 = 0, tmp, ave, stddev, sampNeeded;
  int i, numj;
  Array<int>* samp;
  Array<Array<int>*> samples;
  Array<double> sampleWt;
  
  int s = 0;
  if (sumTrueGnds > 0)
  {
    for (; s < numSamples; s++) // numSamples == numTrials
    {
      i = choosePredPos(corrProbLimit);
      if (i < 0) continue;
      samp = chooseSample(clause, vgtArr, domain, i, flippedGndPred);
      numj = testSampleMembership(clause, vgtArr, db, *samp, sumTrueGnds);
      
      assert(numj > 0);
      tmp = sumTrueGnds / (double)numj;
      estX += tmp;
      x += tmp;
      x2 += tmp*tmp;
      delete samp;
      
      if (s > 0 && s % NUM_SAMPLES_TEST_CONV == 0)
      {
        ave = x/(s+1);
        stddev = sqrt(x2/(s+1) - ave*ave);
        sampNeeded = getNumSamplesNeeded(stddev, 1.0-delta_, epsilon_*ave);
        if (s+1 > sampNeeded) { s++; break; }
        x = 0;
        x2 = 0;
      }
    }
  }

  clause->deleteVarIdToVarsGroundedType(vgtArr);

  if (sumTrueGnds == 0) return 0; 
  return  estX/s;
}


void 
ClauseSampler::getNumGroundingWithEachPredGrounded(const Clause* const & clause,
                                            Array<double>& gndingsWithPredGnded,
                                         const Array<VarsGroundedType*>& vgtArr)
{
  double totalNumGndings = 1;
  for (int i = 1; i < vgtArr.size(); i++)
  {
    if (vgtArr[i] == NULL) continue;
    totalNumGndings *= vgtArr[i]->numGndings;
  }


  Array<bool> varSeen; varSeen.growToSize(vgtArr.size(),false);
  for (int i = 0; i < clause->getNumPredicates(); i++)
  {
    double numGndings = 1;
    memset((void*)varSeen.getItems(), false, varSeen.size()*sizeof(bool));
    Predicate* pred = clause->getPredicate(i);
    for (int j = 0; j < pred->getNumTerms(); j++)
    {
      const Term* t = pred->getTerm(j);
      int varId = -(t->getId());
      if (varId > 0) // if term is a variable
      {
        if (!varSeen[varId])
        {
          varSeen[varId] = true;
          numGndings *= vgtArr[varId]->numGndings;
        }
      }
    }
    gndingsWithPredGnded.append(totalNumGndings/numGndings);
  }
}

  
void ClauseSampler::getProbInfo(const Clause* const & clause, 
                                const Database* const & db, 
                                const Predicate* const & flippedGndPred,
                                const Array<VarsGroundedType*>* const & vgtArr, 
                                TrueFalseGroundingsStore* const& tfGndingsStore,
                                Array<float>& corrProbLimit,double& sumTrueGnds,
                                Array<double>& numTrueGndsPerPos)
{
  Predicate* pred;
  int numGndings;   
  double si;    
  sumTrueGnds = 0;
  Array<double> gndingsWithPredGnded;
  getNumGroundingWithEachPredGrounded(clause, gndingsWithPredGnded, *vgtArr);
  
  for (int i = 0; i < clause->getNumPredicates(); i++)
  {
    pred = (Predicate*) clause->getPredicate(i);
    numGndings = tfGndingsStore->getNumTrueLiteralGroundings(pred, 
                                                             flippedGndPred);
    si = numGndings * gndingsWithPredGnded[i];
    numTrueGndsPerPos.append(si);
    sumTrueGnds += si;
  }
  
  float p = 0;
  for (int i = 0; i < clause->getNumPredicates(); i++)
  {
    if (sumTrueGnds > 0)
    {
      p += numTrueGndsPerPos[i]/sumTrueGnds;
      corrProbLimit.append(p);
    }
    else
      corrProbLimit.append(0.0);
  }
}


Array<int>* ClauseSampler::chooseSample(const Clause* const & clause, 
                                 const Array<VarsGroundedType*>* const & vgtArr,
                                        const Domain* const & domain, 
                                        const int& predPos,
                                        const Predicate* const & flippedGndPred)
{
  const Predicate* pred = clause->getPredicate(predPos);
  TrueFalseGroundingsStore* tfGndingsStore 
    = domain->getTrueFalseGroundingsStore();
  Predicate* rtSensePred 
    = tfGndingsStore->getRandomTrueLiteralGrounding(pred, flippedGndPred);
  
  if (!rtSensePred) 
  {
    cout << "ERROR: in ClauseSampler::chooseSample, no grounding found for ";
    pred->printAsInt(cout); cout << endl;
    exit(-1);
  }

    // create a sample
  Array<int>* samp = new Array<int>;
  samp->growToSize(vgtArr->size(), -1);
  
    // the sample takes the constants of rtSensePred
  for (int i = 0; i < pred->getNumTerms(); i++)
  {
    int constId = rtSensePred->getTerm(i)->getId(); assert(constId >= 0);
    int varId = pred->getTerm(i)->getId(); 
    if (varId < 0) (*samp)[-varId] = constId;
    else           assert(constId == varId);
  }
  
  for (int i = 1; i < vgtArr->size(); i++)
  {
      //if var -1 has been gnded or it has been set to rtSensePred's constant
    if ((*vgtArr)[i] == NULL || (*samp)[i] >= 0) continue; 
    const Array<int>* constants 
      = domain->getConstantsByType((*vgtArr)[i]->typeId);
    assert(constants->size() > 0);
    int idx = random_.randomOneOf(constants->size());
      (*samp)[i] = (*constants)[idx];
  }
  
  delete rtSensePred;
  return samp;
}


void ClauseSampler::groundClause(const Array<VarsGroundedType*>& vgtArr, 
                                 const Array<int>& samp)
{
  assert(samp.size() == vgtArr.size());
  for (int i = 1; i < vgtArr.size(); i++)
  {
    if (vgtArr[i] == NULL) continue; 
    Array<Term*>& vars = vgtArr[i]->vars;
    for (int j = 0 ; j < vars.size(); j++) vars[j]->setId(samp[i]);
  }
}


int ClauseSampler::testSampleMembership(Clause* const & clause, 
                                        Array<VarsGroundedType*>* const& vgtArr,
                                        const Database* const & db, 
                                        const Array<int>& samp,
                                        const double& sumTrueGnds)
{
  groundClause(*vgtArr, samp);
  int numj = 0;
  for (int i = 0; i < clause->getNumPredicates(); i++)
  {
    TruthValue tv = db->getValue(clause->getPredicate(i));
    bool sense = clause->getPredicate(i)->getSense();
    if (Database::sameTruthValueAndSense(tv,sense)) numj++;
  }
  clause->restoreVars(vgtArr);
  return numj;
}

