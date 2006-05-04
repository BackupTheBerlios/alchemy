#ifndef _INFER_H_OCT_30_2005
#define _INFER_H_OCT_30_2005

#include "util.h"
#include "fol.h"
#include "mrf.h"


bool createComLineQueryPreds(const string& queryPredsStr,
                             const Domain* const & domain,
                             Database* const & evidenceDB,
                             GroundPredicateHashArray* const & queries,
                             GroundPredicateHashArray* const & knownQueries,
                             Array<int>* const & allPredGndingsAreQueries,
                             bool printToFile, ostream& out, bool amapPos)
{
  if (queryPredsStr.length() == 0) return true;
  string preds = Util::trim(queryPredsStr);

    //replace the comma between query predicates with '\n'
  int balparen = 0;
  for (unsigned int i = 0; i < preds.length(); i++)
  {
    if (preds.at(i)=='(')                     balparen++;
    else if (preds.at(i)==')')                balparen--;
    else if (preds.at(i)==',' && balparen==0) preds.at(i)='\n';
  }

  bool onlyPredName;
  bool ret = true;
  unsigned int cur;
  int termId, varIdCnt = 0;
  hash_map<string, int, HashString, EqualString> varToId;
  hash_map<string, int, HashString, EqualString>::iterator it;
  Array<VarsTypeId*>* vtiArr;
  string pred, predName, term;
  const PredicateTemplate* ptemplate;
  istringstream iss(preds);
  char delimit[2]; delimit[1] = '\0';

    // for each query pred on command line
  while (getline(iss, pred))
  {
    onlyPredName = false;
    varToId.clear();
    varIdCnt = 0;
    cur = 0;

      // get predicate name
    if (!Util::substr(pred,cur,predName,"(")) {predName=pred;onlyPredName=true;}
    
    ptemplate = domain->getPredicateTemplate(predName.c_str());
    if (ptemplate == NULL)
    {
      cout << "ERROR: Cannot find command line query predicate" << predName 
           << " in domain." << endl;
      ret = false;
      continue;
    }
    Predicate ppred(ptemplate);

    if (!onlyPredName) // if the terms of the query predicate are also specified
    {
        // get term name
      for (int i = 0; i < 2; i++)
      {       
        if (i == 0) delimit[0] = ',';
        else        delimit[0] = ')';
        while(Util::substr(pred, cur, term, delimit))
        {
          if (isupper(term.at(0))) // this is a constant
          {
            termId = domain->getConstantId(term.c_str());
            if (termId < 0) 
            {
              cout <<"ERROR: Cannot find constant "<<term<<" in database"<<endl;
              ret = false;
            }        
          }
          else
          {   // it is a variable        
            if ((it=varToId.find(term)) == varToId.end()) 
            {
              termId = --varIdCnt;
              varToId[term] = varIdCnt; 
            }
            else
              termId = (*it).second;
          }
          ppred.appendTerm(new Term(termId, (void*)&ppred, true));
        }
      }
    }
    else
    {   // if only the predicate name is specified
      (*allPredGndingsAreQueries)[ptemplate->getId()] = true;
      for (int i = 0; i < ptemplate->getNumTerms(); i++)
        ppred.appendTerm(new Term(--varIdCnt, (void*)&ppred, true));
    }  


    if (ppred.getNumTerms() != ptemplate->getNumTerms())
    {
      cout << "ERROR: " << predName << " requires " << ptemplate->getNumTerms()
           << " terms but given " << ppred.getNumTerms() << endl;
      ret = false;
    }
    if (!ret) continue;

    
    ///////////////////// create all groundings of predicate ///////////////
    vtiArr = NULL;
    ppred.createVarsTypeIdArr(vtiArr);

      // if a ground predicate was specified on command line
    if (vtiArr->size() <= 1)
    {
      assert(ppred.isGrounded());
      assert(!evidenceDB->isClosedWorld(ppred.getId()));
      TruthValue tv = evidenceDB->getValue(&ppred);              
      GroundPredicate* gndPred = new GroundPredicate(&ppred);
      
      // If just printing to file, then all values must be known
      if (printToFile) assert(tv != UNKNOWN);
      if (tv == UNKNOWN)
      {
        if (queries->append(gndPred) < 0) delete gndPred;
      }
      else
      {
          // If just printing to file
        if (printToFile)
        {
          if (amapPos) //if show postive ground query predicates only
      	  {
      		if (tv == TRUE)
      		{
      	  	  ppred.printWithStrVar(out, domain);
      	  	  out << endl;
      		}
      	  }
      	  else
      	  {
      		  //print all ground query predicates
	  		ppred.printWithStrVar(out, domain);
	  		out << " " << tv << endl;
      	  }
          delete gndPred;
        }
        else // Building queries
        {
          if (tv == TRUE) gndPred->setProbTrue(1);
          else            gndPred->setProbTrue(0);

          if (knownQueries->append(gndPred) < 0) delete gndPred;  
        }
      }      
    }
    else
    {
      ArraysAccessor<int> acc;
      for (int i = 1; i < vtiArr->size(); i++)
      {
        const Array<int>* cons=domain->getConstantsByType((*vtiArr)[i]->typeId);
        acc.appendArray(cons);
      } 

        // form all groundings of the predicate
      Array<int> constIds;
      while (acc.getNextCombination(constIds))
      {
        assert(constIds.size() == vtiArr->size()-1);
        for (int j = 0; j < constIds.size(); j++)
        {
          Array<Term*>& terms = (*vtiArr)[j+1]->vars;
          for (int k = 0; k < terms.size(); k++)
            terms[k]->setId(constIds[j]);
        }

        //at this point the predicate is grounded
      
        assert(!evidenceDB->isClosedWorld(ppred.getId()));

        TruthValue tv = evidenceDB->getValue(&ppred);        
        GroundPredicate* gndPred = new GroundPredicate(&ppred);

    	  // If just printing to file, then all values must be known
	    if (printToFile) assert(tv != UNKNOWN);
        if (tv == UNKNOWN)
        {
          if (queries->append(gndPred) < 0) delete gndPred;
        }
        else
        {
          	// If just printing to file
          if (printToFile)
          {
          	if (amapPos) //if show postive ground query predicates only
      	  	{
      		  if (tv == TRUE)
      		  {
      	  	  	ppred.printWithStrVar(out, domain);
      	  	  	out << endl;
      		  }
      	  	}
      	  	else
      	  	{
      		  	//print all ground query predicates
	  		  ppred.printWithStrVar(out, domain);
	  		  out << " " << tv << endl;
      	  	}
          	delete gndPred;
          }
          else // Building queries
          {
          	if (tv == TRUE) gndPred->setProbTrue(1);
          	else            gndPred->setProbTrue(0);
          	if (knownQueries->append(gndPred) < 0) delete gndPred;  
          }
        }        
      }
    }
      
    ppred.deleteVarsTypeIdArr(vtiArr);
  } // for each query pred on command line

  if (!printToFile)
  {
  	queries->compress();
  	knownQueries->compress();
  }
  
  return ret;
}

bool createComLineQueryPreds(const string& queryPredsStr,
                             const Domain* const & domain,
                             Database* const & evidenceDB,
                             GroundPredicateHashArray* const & queries,
                             GroundPredicateHashArray* const & knownQueries,
                             Array<int>* const & allPredGndingsAreQueries)
{
  return createComLineQueryPreds(queryPredsStr, domain, evidenceDB,
                             	 queries, knownQueries, allPredGndingsAreQueries,
                             	 false, cout, false);
}
  // note that preds is a copy
bool extractPredNames(string preds, const string* queryFile, 
                      StringHashArray& predNames)
{ 
  predNames.clear();

    // first extract the query pred names specified on command line
  string::size_type cur = 0, ws, ltparen;
  string qpred, predName;
  
  if (preds.length() > 0)
  {
    preds.append(" "); //terminate preds with a whitespace
    
      //replace the comma between query predicates with ' '
    int balparen = 0;
    for (unsigned int i = 0; i < preds.length(); i++)
    {
      if (preds.at(i) == '(')      balparen++;
      else if (preds.at(i) == ')') balparen--;
      else if (preds.at(i) == ',' && balparen == 0) preds.at(i) = ' ';
    }
    
    while (preds.at(cur) == ' ') cur++;
    while (true)
    {
      ws = preds.find(" ", cur);
      if (ws == string::npos) break;
      qpred = preds.substr(cur,ws-cur+1);
      cur = ws+1;
      while (cur < preds.length() && preds.at(cur) == ' ') cur++;
      ltparen = qpred.find("(",0);
      
      if (ltparen == string::npos) 
      { 
        ws = qpred.find(" ");
        if (ws != string::npos) qpred = qpred.substr(0,ws);
        predName = qpred; 
      }
      else
        predName = qpred.substr(0,ltparen);
      
      predNames.append(predName);
    }
  }

  if (queryFile == NULL || queryFile->length() == 0) return true;

    // next extract query predicates specified in query file  
  ifstream in((*queryFile).c_str());
  if (!in.good())
  {
    cout << "ERROR: unable to open " << *queryFile << endl;
    return false;
  }
  string buffer;
  while (getline(in, buffer))
  {
    cur = 0;
    while (cur < buffer.length() && buffer.at(cur) == ' ') cur++;
    ltparen = buffer.find("(", cur);
    if (ltparen == string::npos) continue;
    predName = buffer.substr(cur, ltparen-cur);
    predNames.append(predName);
  }

  in.close();
  return true;
}


typedef hash_map<string, const Array<const char*>*, HashString, EqualString> 
StringToStrArrayMap;


char getTruthValueFirstChar(const TruthValue& tv)
{
  if (tv == TRUE)    return 'T';
  if (tv == FALSE)   return 'F';
  if (tv == UNKNOWN) return 'U';
  assert(false);
  exit(-1);
  return '#'; //avoid compilation warning
}


void setsrand()
{
  struct timeval tv;
  struct timezone tzp;
  gettimeofday(&tv,&tzp);
  unsigned int seed = (( tv.tv_sec & 0177 ) * 1000000) + tv.tv_usec;
  srand(seed);
} 


  //copy srcFile to dstFile, & append '#include "dbFiles"' to latter
void copyFileAndAppendDbFile(const string& srcFile, string& dstFile, 
                             const Array<string>& dbFilesArr,
                             const Array<string>& constFilesArr,
                             const Array<string>& funcFilesArr)
{
  ofstream out(dstFile.c_str());
  ifstream in(srcFile.c_str());
  if (!out.good()) { cout<<"ERROR: failed to open "<<dstFile<<endl;exit(-1);}
  if (!in.good()) { cout<<"ERROR: failed to open "<<srcFile<<endl;exit(-1);}
  
  string buffer;
  while(getline(in, buffer)) out << buffer << endl;
  in.close();

  out << endl;
  for (int i = 0; i < constFilesArr.size(); i++) 
    out << "#include \"" << constFilesArr[i] << "\"" << endl;
  out << endl;
  for (int i = 0; i < funcFilesArr.size(); i++)  
    out << "#include \"" <<  funcFilesArr[i]  << "\"" << endl;
  out << endl;
  for (int i = 0; i < dbFilesArr.size(); i++)    
    out << "#include \"" << dbFilesArr[i] << "\"" << endl;
  out.close();
}


bool checkQueryPredsNotInClosedWorldPreds(const StringHashArray& qpredNames,
                                          const StringHashArray& cwPredNames)
{
  bool ok = true;
  for (int i = 0; i < qpredNames.size(); i++)
    if (cwPredNames.contains(qpredNames[i]))
    {
      cout << "ERROR: query predicate " << qpredNames[i] 
           << " cannot be specified as closed world" << endl; 
      ok = false;
    }
  return ok;
}


bool createQueryFilePreds(const string& queryFile, const Domain* const & domain,
                          Database* const & evidenceDB,
                          GroundPredicateHashArray* const &queries,
                          GroundPredicateHashArray* const &knownQueries,
                          bool printToFile, ostream& out, bool amapPos)
{
  if (queryFile.length() == 0) return true;

  bool ret = true;
  ifstream in(queryFile.c_str());
  unsigned int line = 0;
  unsigned int cur;
  int constId, predId;
  bool ok;
  string predStr, predName, constant;
  Array<int> constIds;
  const PredicateTemplate* ptemplate;

  while (getline(in, predStr))
  {
    line++;
    cur = 0;

      // get predicate name
    ok = Util::substr(predStr, cur, predName, "(");
    if (!ok) continue;

    predId = domain->getPredicateId(predName.c_str());
    ptemplate = domain->getPredicateTemplate(predId);

    if (predId < 0 || ptemplate == NULL)
    {
      cout << "ERROR: Cannot find " << predName << " in domain on line " 
           << line << " of query file " << queryFile << endl;
      ret = false;
      continue;
    }

      // get constant name
    constIds.clear();
    while (Util::substr(predStr, cur, constant, ","))
    {
      constId = domain->getConstantId(constant.c_str());
      constIds.append(constId);
      if (constId < 0)
      {
        cout << "ERROR: Cannot find " << constant << "in database on line " 
             << line << " of query file " << queryFile << endl;
        ret = false;
      }
    }

      // get constant name
    ok = Util::substr(predStr, cur, constant, ")"); 
    if (!ok)
    {
      cout << "ERROR: Failed to parse ground predicate on line " << line
           << " of query file " << queryFile << endl;
      ret = false;
      continue;
    }

    constId = domain->getConstantId(constant.c_str());
    constIds.append(constId);
    if (constId < 0)
    {
      cout << "ERROR: Cannot find " << constant << "in database on line " 
           << line << " of query file " << queryFile << endl;
      ret = false;
    }

    if (!ret) continue;
    ///////////////////////// done parsing ///////////////////////////////
    
      // create Predicate and set its truth value to UNKNOWN
    if (constIds.size() != ptemplate->getNumTerms())
    {
      cout << "ERROR: incorrect number of terms for " << predName 
           << ". Expected " << ptemplate->getNumTerms() << ", given " 
           << constIds.size() << endl;
      ret = false;
      continue;
    }
    
    Predicate pred(ptemplate);
    for (int i = 0; i < constIds.size(); i++)
    {
      if (pred.getTermTypeAsInt(i) != domain->getConstantTypeId(constIds[i]))
      {
        cout << "ERROR: wrong type for term " 
             << domain->getConstantName(constIds[i]) << " for predicate " 
             << predName  << " on line " << line << " of query file " 
             << queryFile << endl;
        ret = false;
        continue;
      }
      pred.appendTerm(new Term(constIds[i], (void*)&pred, true));
    }
    if (!ret) continue;

    assert(!evidenceDB->isClosedWorld(predId));

    TruthValue tv = evidenceDB->getValue(&pred);
    GroundPredicate* gndPred = new GroundPredicate(&pred);
    
    // If just printing to file, then all values must be known
    if (printToFile) assert(tv != UNKNOWN);
    if (tv == UNKNOWN)
    {
      if (queries->append(gndPred) < 0) delete gndPred;
    }
    else
    {
    	// If just printing to file
      if (printToFile)
      {
        if (amapPos) //if show postive ground query predicates only
    	{
      	  if (tv == TRUE)
      	  {
      	    pred.printWithStrVar(out, domain);
      	  	out << endl;
      	  }
        }
        else
        {
         	//print all ground query predicates
	  	  pred.printWithStrVar(out, domain);
	  	  out << " " << tv << endl;
      	}
        delete gndPred;
      }
      else // Building queries
      {
        if (tv == TRUE) gndPred->setProbTrue(1);
      	else            gndPred->setProbTrue(0);
      	if (knownQueries->append(gndPred) < 0) delete gndPred;
      }
    }
  } // while (getline(in, predStr))

  in.close();
  return ret;
}

bool createQueryFilePreds(const string& queryFile, const Domain* const & domain,
                          Database* const & evidenceDB,
                          GroundPredicateHashArray* const &queries,
                          GroundPredicateHashArray* const &knownQueries)
{
  return createQueryFilePreds(queryFile, domain, evidenceDB, queries,
                          	  knownQueries, false, cout, false);
}

void readPredValuesAndSetToUnknown( const StringHashArray& predNames,
		                             Domain *domain,
									 Array<Predicate *> &queryPreds,
									 Array<TruthValue> &queryPredValues,
									 bool isQueryEvidence)
 {
    Array<Predicate*> ppreds;

    //cout << endl << "Getting the counts for the domain " << i << endl;
    queryPreds.clear();
    queryPredValues.clear();
    for(int predno = 0; predno < predNames.size(); predno++) 
    {
      ppreds.clear();
      int predid = domain->getPredicateId(predNames[predno].c_str());
      Predicate::createAllGroundings(predid, domain, ppreds);
       //cout<<"size of gnd for pred " << predid << " = "<<ppreds.size()<<endl;
		for(int gpredno=0;gpredno<ppreds.size();gpredno++) {
		 Predicate *pred = ppreds[gpredno];
		 TruthValue tv = domain->getDB()->getValue(pred);
		 if(tv == UNKNOWN)
		  domain->getDB()->setValue(pred,FALSE);
		 
		 // if first order query pred grounndings are allowed to be evidence
		 // - we assume all the predicates not in db to be false
		 // evidence - need a better way code this.
		 if(isQueryEvidence && tv == UNKNOWN)
			  delete pred;
		 else
			  queryPreds.append(pred);
		}
	  }
	  //set all the query preds to unknown, reading in the TRUE/FALSE status 
	  //for verification at a later time
	  domain->getDB()->setValuesToUnknown(&queryPreds, &queryPredValues);
 }


void setPredsToGivenValues( const StringHashArray& predNames,
		                             Domain *domain,
									 Array<TruthValue> &gpredValues)
{
    Array<Predicate*> gpreds;
    Array<Predicate*> ppreds;
    Array<TruthValue> tmpValues;

    //cout << endl << "Getting the counts for the domain " << i << endl;
    gpreds.clear();
    tmpValues.clear();
    for(int predno = 0; predno < predNames.size(); predno++) 
    {
       ppreds.clear();
       int predid = domain->getPredicateId(predNames[predno].c_str());
       Predicate::createAllGroundings(predid, domain, ppreds);
       //cout<<"size of gnd for pred " << predid << " = "<<ppreds.size()<<endl;
       gpreds.append(ppreds);
    }
     
     
	domain->getDB()->setValuesToGivenValues(&gpreds, &gpredValues);
      
	for(int gpredno = 0; gpredno < gpreds.size(); gpredno++) 
      delete gpreds[gpredno];
 }


#endif
