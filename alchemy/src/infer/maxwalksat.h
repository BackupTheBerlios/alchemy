#ifndef MAXWALKSAT_H_OCT_1_2005
#define MAXWALKSAT_H_OCT_1_2005

#include <cfloat>
#include <unistd.h>
#include <sys/time.h>
#include <cstring>
#include <sstream>
#include <fstream>
#include "array.h"
#include "lazywalksat.h"

struct MaxWalksatData
{
  Array<Array<int>*> clauses;
  Array<double> clauseWts;
  int numGndPreds;
  double hardClauseWt;
};


  //Wrapper class for Max Walksat
class MaxWalksat
{
 public:
  MaxWalksat() {}

    // Caller is responsible for deleting the contents of solutions
  void sample(const MaxWalksatData* const & data, const int& identifier,
              Array<Array<bool>*>& solutions, const MaxWalksatParams* const & params)
  {
    const Array<Array<int>*>& clauses = data->clauses;
    const Array<double>& clauseWts = data->clauseWts;
    int numGndPreds = data->numGndPreds;
    int numSolutions = 1;
    bool includeUnsatSolutions = true;
    //int maxSteps = 1000000;
    int maxSteps = params->maxSteps;
    int tries = params->tries;
    int targetWt = params->targetWt;
    bool hard = params->hard;
    int seed = params->seed;

      // get the file name
    string cnfFile, resultsFile;
    getWorkingFileNames(identifier, cnfFile, resultsFile);
	bool ok = true;
    Array<int> numBad;

      // prepare the input file for Max Walksat
    ok = prepareCNFFile(cnfFile, clauses, clauseWts, numGndPreds);
    if (!ok) return;

	// run MaxWalksat
    char targetWtParam[20]; targetWtParam[0] = '\0';
    if (targetWt != INT_MAX) sprintf(targetWtParam, "-targetcost %d", targetWt);
    
    char hardParam[20]; hardParam[0] = '\0';
    if (hard) sprintf(hardParam, "-hard");

    char command[1000];
    sprintf(command, 
            "%s -withcost -tries %d -numsol %d -cutoff %d -seed %d %s %s -low < %s > %s",
            maxWalksatExe_, tries, numSolutions, maxSteps, seed, targetWtParam, 
            hardParam, cnfFile.c_str(), resultsFile.c_str());
    system(command);

      // read the results
    ok = readResults(resultsFile, numGndPreds, includeUnsatSolutions, 
                     solutions, numBad);
	if (!ok) return;

    unlink(cnfFile.c_str());
    unlink(resultsFile.c_str());
  }


  static int makeNegatedLiteral(const int& lit)  { return lit | 0x80000000; }
  static int isLiteralNegated(const int& lit)    { return lit & 0x80000000; }
  static int getLiteral(const int& lit)          { return lit & 0x7FFFFFFF; }  
  static int negationOf(const int& lit)          { return lit ^ 0x80000000; }


 private:
    //caller is responsible for deleting the return file names
  void getWorkingFileNames(const int& i, string& cnfFile, string& resultsFile)
  {
    struct timeval tv;
    gettimeofday(&tv,0);
    int sec  = tv.tv_sec;
    int usec = tv.tv_usec;

    char buf[80];
    sprintf(buf, "./walksattmp%d.%d.%d.%d", i, getpid(), sec, usec);
    cnfFile.clear();
    cnfFile.append(buf);

    sprintf(buf, "%s.res", cnfFile.c_str());
    resultsFile.clear();
    resultsFile.append(buf);
  }


  bool prepareCNFFile(const string& cnfFile, const Array<Array<int>*>& clauses, 
                      const Array<double>& clauseWts, const int& numGndPreds) 
  {
    ofstream out(cnfFile.c_str());
    if (!out.good())
    {
      cout << "Error in MaxWalksat: could not open " << cnfFile << " for output"
           << endl;
      return false;
    }

      // These variables are numbered 0..numVars-1. 
      // We will output them as 1..numVars
    out << "p wcnf " << numGndPreds << " " << clauses.size() << endl;
    for (int i = 0; i < clauses.size(); i++) 
    {
      out << int(clauseWts[i]) << " ";
      for (int j = 0; j < clauses[i]->size(); j++) 
      {
        int gndPredId = getLiteral( (*(clauses[i]))[j] );
        bool negated = isLiteralNegated( (*(clauses[i]))[j] );
        if (negated) out << "-";
        out << gndPredId+1 << " ";
      }
      out << "0" << endl;
    }
    out << endl;

    out.close();
    return true;
  }
  

    // numBad is the reported number of bad. I think that's num of unsat clauses
    // Caller is responsible for deleting the contents of solutions
  bool readResults(const string& resultsFile, const int& numGndPreds, 
                   const bool&  includeUnsatSolutions,
                   Array<Array<bool>*>& solutions, Array<int>& numBad) 
  {
    ifstream in(resultsFile.c_str());

    if (!in.good())
    {
      cout << "Error in MaxWalksat: could not open " << resultsFile 
           << " for input" << endl;
      return false;
    }    
      // Each solution begins with "Begin assign with lowest # bad = 0\n" 
      // and ends with "End assign\n". The "# bad" is 0 for sat example, 
      // non-zero for one that doesn't sat
    string buffer;
    while(getline(in, buffer)) 
    {
      if (buffer.compare(0, 12, "Begin assign") == 0)
      {
        int numbad;
        sscanf(buffer.c_str(), "Begin assign with lowest # bad = %d", &numbad);
        if (includeUnsatSolutions || numbad == 0) 
        {
          // concat all lines until reach 'end assign'
          Array<bool>* solution = new Array<bool>(numGndPreds);
          for (int i = 0; i < numGndPreds; i++)  solution->append(false);
          
          while (getline(in, buffer)) 
          {
            if (buffer.compare(0, 10, "End assign") == 0) break;
            else 
            {
              istringstream x(buffer);
              int lit;
              while (x >> lit) (*solution)[lit-1] = true;
            }
          }
          solutions.append(solution);
          numBad.append(numbad);
        }
      }
    }
    
    in.close(); 
    return true;
  }

 private:
  static char* maxWalksatExe_;
  
};


#endif
