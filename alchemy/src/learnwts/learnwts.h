#ifndef LEARNWTS_H_NOV_23_2005
#define LEARNWTS_H_NOV_23_2005

#include <sys/time.h>
#include "util.h"
#include "timer.h"
#include "fol.h"
#include "mln.h"
#include "indextranslator.h"


extern const char* ZZ_TMP_FILE_POSTFIX; //defined in fol.y
const bool DOMAINS_SHARE_DATA_STRUCT = true;

void extractFileNames(const char* const & namesStr, Array<string>& namesArray)
{
  if (namesStr == NULL) return;
  string s(namesStr);
  s = Util::trim(s);
  if (s.length() == 0) return;
  s.append(",");
  string::size_type cur = 0;
  string::size_type comma;
  string name;
  while (true)
  {
    comma = s.find(",", cur);
    if (comma == string::npos) return;
    name = s.substr(cur, comma-cur);
    namesArray.append(name);
    cur = comma+1;
  }
}


void createDomainAndMLN(Array<Domain*>& domains, Array<MLN*>& mlns,
                        const string& inMLNFile, ostringstream& constFiles,
                        ostringstream& funcFiles, ostringstream& dbFiles,
                        const StringHashArray* const & nonEvidPredNames,
                        const bool& addUnitClauses, const double& priorMean,
                        const bool& checkPlusTypes, const bool& mwsLazy)
{
  string::size_type bslash = inMLNFile.rfind("/");
  string tmp = (bslash == string::npos) ? 
               inMLNFile:inMLNFile.substr(bslash+1,inMLNFile.length()-bslash-1);
  char tmpInMLN[100];
  sprintf(tmpInMLN, "%s%s",  tmp.c_str(), ZZ_TMP_FILE_POSTFIX);

  ofstream out(tmpInMLN);
  ifstream in(inMLNFile.c_str());
  if (!out.good()) { cout<<"ERROR: failed to open "<<tmpInMLN <<endl; exit(-1);}
  if (!in.good())  { cout<<"ERROR: failed to open "<<inMLNFile<<endl; exit(-1);}

  string buffer;
  while(getline(in, buffer)) out << buffer << endl;
  in.close();

  out << constFiles.str() << endl << funcFiles.str() << endl 
      << dbFiles.str() << endl;
  out.close();
  
  // read the formulas from the input MLN
  Domain* domain = new Domain;
  MLN* mln = new MLN();
  
  bool allPredsExceptQueriesAreCW = true;
  bool warnAboutDupGndPreds = true;
  bool mustHaveWtOrFullStop = false;
  Domain* domain0 = (checkPlusTypes) ? domains[0] : NULL;

  bool ok = runYYParser(mln, domain, tmpInMLN, allPredsExceptQueriesAreCW, 
                        NULL, nonEvidPredNames, addUnitClauses, 
                        warnAboutDupGndPreds, priorMean, mustHaveWtOrFullStop,
                        domain0, mwsLazy);

  if (!ok) { unlink(tmpInMLN); exit(-1); }
  domains.append(domain);
  mlns.append(mln);
  unlink(tmpInMLN);
}


void createDomainsAndMLNs(Array<Domain*>& domains, Array<MLN*>& mlns, 
                          const bool& multipleDatabases,
                          const string& inMLNFile,
                          const Array<string>& constFilesArr,
                          const Array<string>& funcFilesArr,
                          const Array<string>& dbFilesArr,
                          const StringHashArray* const & nonEvidPredNames,
                          const bool& addUnitClauses, const double& priorMean,
                          const bool& mwsLazy)
{
  if (!multipleDatabases)
  {
    ostringstream constFilesStream, funcFilesStream, dbFilesStream;
    for (int i = 0; i < constFilesArr.size(); i++) 
      constFilesStream << "#include \"" << constFilesArr[i] << "\"" << endl;
    for (int i = 0; i < funcFilesArr.size(); i++) 
      funcFilesStream << "#include \"" << funcFilesArr[i] << "\"" << endl;
    for (int i = 0; i < dbFilesArr.size(); i++)    
      dbFilesStream << "#include \"" << dbFilesArr[i] << "\"" << endl;
    createDomainAndMLN(domains, mlns, inMLNFile, constFilesStream, 
                       funcFilesStream, dbFilesStream, nonEvidPredNames,
                       addUnitClauses, priorMean, false, mwsLazy);
  }
  else
  {   //if multiple databases
    for (int i = 0; i < dbFilesArr.size(); i++) // for each domain
    {
      cout << "parsing MLN and creating domain " << i << "..." << endl;
      ostringstream constFilesStream, funcFilesStream, dbFilesStream;
      if (constFilesArr.size() > 0)
        constFilesStream << "#include \"" << constFilesArr[i] << "\"" << endl;
      if (funcFilesArr.size() > 0)
        funcFilesStream << "#include \"" << funcFilesArr[i] << "\"" << endl;
      dbFilesStream    << "#include \"" << dbFilesArr[i]    << "\"" << endl;
      
      bool checkPlusTypes = (i > 0);

      createDomainAndMLN(domains, mlns, inMLNFile, constFilesStream,
                         funcFilesStream, dbFilesStream, nonEvidPredNames,
                         addUnitClauses, priorMean, checkPlusTypes, mwsLazy);

        // let the domains share data structures
      if (DOMAINS_SHARE_DATA_STRUCT && i > 0)
      {
        const ClauseHashArray* carr = mlns[i]->getClauses();
        for (int j = 0; j < carr->size(); j++)
        {
          Clause* c = (*carr)[j];
          for (int k = 0; k < c->getNumPredicates(); k++)
          {
            Predicate* p = c->getPredicate(k);
            const PredicateTemplate* t 
              = domains[0]->getPredicateTemplate(p->getName());
            assert(t);
            p->setTemplate((PredicateTemplate*)t);
          }
        }

        ((Domain*)domains[i])->replaceTypeDualMap((
                                         DualMap*)domains[0]->getTypeDualMap());
        ((Domain*)domains[i])->replaceStrToPredTemplateMapAndPredDualMap(
                  (StrToPredTemplateMap*) domains[0]->getStrToPredTemplateMap(),
                  (DualMap*) domains[0]->getPredDualMap());
        ((Domain*)domains[i])->replaceStrToFuncTemplateMapAndFuncDualMap(
                  (StrToFuncTemplateMap*) domains[0]->getStrToFuncTemplateMap(),
                  (DualMap*) domains[0]->getFuncDualMap());
        ((Domain*)domains[i])->replaceEqualPredTemplate(
                        (PredicateTemplate*)domains[0]->getEqualPredTemplate());
        ((Domain*)domains[i])->replaceFuncSet(
                                        (FunctionSet*)domains[0]->getFuncSet());
      }

    } // for each domain
  }

  //commented out: not true when there are domains with different constants
  //int numClauses = mlns[0]->getNumClauses();
  //for (int i = 1; i < mlns.size(); i++) 
  //  assert(mlns[i]->getNumClauses() == numClauses);
  //numClauses = 0; //avoid compilation warning
}


double getTimeSec(const bool& discLearn, Timer& timer)
{
  if (discLearn)
  {
      //using wall clock time since MaxWalksat is an external executable/process
    struct timeval tv; struct timezone tzp;
    gettimeofday(&tv,&tzp);
    return tv.tv_sec;
  }
  return timer.time();
}


void assignWtsAndOutputMLN(ostream& out, Array<MLN*>& mlns, 
                           Array<Domain*>& domains, const Array<double>& wts, 
                           IndexTranslator* const& indexTrans)
{
    //assign the optimal weights belonging to clauses (and none of those 
    //belonging to existentially quantified formulas) to the MLNs
  double* wwts = (double*) wts.getItems();
  indexTrans->assignNonTiedClauseWtsToMLNs(++wwts);


    // output the predicate declaration
  out << "//predicate declarations" << endl;
  domains[0]->printPredicateTemplates(out);
  out << endl;

  // output the function declarations
  out << "//function declarations" << endl;
  domains[0]->printFunctionTemplates(out);
  out << endl;

  mlns[0]->printMLNNonExistFormulas(out, domains[0]);

  const ClauseHashArray* clauseOrdering = indexTrans->getClauseOrdering();
  const StringHashArray* exFormOrdering = indexTrans->getExistFormulaOrdering();
  for (int i = 0; i < exFormOrdering->size(); i++)
  {
      // output the original formula and its weight
    out.width(0); out << "// "; out.width(6); 
    out << wts[1+clauseOrdering->size()+i] << "  " <<(*exFormOrdering)[i]<<endl;
    out << wts[1+clauseOrdering->size()+i] << "  " <<(*exFormOrdering)[i]<<endl;
    out << endl;
  }
}


void assignWtsAndOutputMLN(ostream& out, Array<MLN*>& mlns, 
                           Array<Domain*>& domains, const Array<double>& wts)
{
    // assign the optimal weights to the clauses in all MLNs
  for (int i = 0; i < mlns.size(); i++)
  {
    MLN* mln = mlns[i];
    const ClauseHashArray* clauses = mln->getClauses();
    for (int i= 0; i < clauses->size(); i++) 
      (*clauses)[i]->setWt(wts[i+1]);
  }

    // output the predicate declaration
  out << "//predicate declarations" << endl;
  domains[0]->printPredicateTemplates(out);
  out << endl;

  // output the function declarations
  out << "//function declarations" << endl;
  domains[0]->printFunctionTemplates(out);
  out << endl;
  mlns[0]->printMLN(out, domains[0]);
}


void deleteDomains(Array<Domain*>& domains)
{
  for (int i = 0; i < domains.size(); i++) 
  {
    if (DOMAINS_SHARE_DATA_STRUCT && i > 0)
    {
      ((Domain*)domains[i])->setTypeDualMap(NULL);
      ((Domain*)domains[i])->setStrToPredTemplateMapAndPredDualMap(NULL, NULL);
      ((Domain*)domains[i])->setStrToFuncTemplateMapAndFuncDualMap(NULL, NULL);
      ((Domain*)domains[i])->setEqualPredTemplate(NULL);
      ((Domain*)domains[i])->setFuncSet(NULL);
    }
    delete domains[i];
  }
}


#endif
