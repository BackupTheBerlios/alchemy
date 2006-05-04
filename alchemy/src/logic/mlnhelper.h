#ifndef MLNHELPER_H_NOV_4_2005
#define MLNHELPER_H_NOV_4_2005

//auxiliary data structures used by MLN


  //index is the index of an IndexClause object in an array.
struct IndexClause 
{
  IndexClause(const int& iindex, Clause* const & cclause)
    : index(iindex), clause(cclause) {}
  ~IndexClause() {}
  int index; 
  Clause* clause; 
};

class HashIndexClause
{
 public:
  size_t operator()(IndexClause* const& ic) const 
  { return ic->clause->hashCode(); }
};

class EqualIndexClause
{
 public:
  bool operator()(IndexClause* const & ic1, IndexClause* const & ic2) const
  { return ic1->clause->same(ic2->clause); }
};

typedef HashArray<IndexClause*, HashIndexClause, EqualIndexClause> 
  IndexClauseHashArray;

/////////////////////////////////////////////////////////////////////////////


  //Used to quickly access a clause in predIdToClausesMap_. predId is an index
  //of predIdToClausesMap_, while clauseIndex is an index of the clause array 
  //predIdToClausesMap_[predId];
struct PredIdClauseIndex 
{
  PredIdClauseIndex(const int& ppredId, int* const & cclauseIndex)
    : predId(ppredId), clauseIndex(cclauseIndex) {}
  ~PredIdClauseIndex() {}
  int predId; 
  int* clauseIndex; 
};

  //Used to quickly access a clause in formAndClauses_. formulaIndex an index of
  //formAndClauses_ and clauseIndex is an index of forAndClauses_[formulaIndex].
struct FormulaClauseIndexes 
{ 
  FormulaClauseIndexes(int* const & fformulaIndex, int* const & cclauseIndex)
    : formulaIndex(fformulaIndex), clauseIndex(cclauseIndex) {}
  ~FormulaClauseIndexes() {}
  int* formulaIndex; 
  int* clauseIndex; 
};

  //Each MLNClauseInfo corresponds to a clause. index is the index of 
  //MLNClauseInfo in clauseInfos_. It should be the same as the index of its 
  //corresponding clause in clauses_. 
  //predIdsClauseIndexes and formulaClauseIndexes help one to quickly find the 
  //occurrences of the clause in predIdToClausesMap_, and formAndClauses_.
struct MLNClauseInfo
{
  MLNClauseInfo(const int& iindex, const double& ppriorMean) 
    : index(iindex), priorMean(ppriorMean) {}
  MLNClauseInfo(const int& iindex) : index(iindex), priorMean(0) {}

  ~MLNClauseInfo() { predIdsClauseIndexes.deleteItemsAndClear();
                     formulaClauseIndexes.deleteItemsAndClear(); }
  void compress() { predIdsClauseIndexes.compress(); 
                    formulaClauseIndexes.compress(); }
  int index;
  Array<PredIdClauseIndex*> predIdsClauseIndexes;
  Array<FormulaClauseIndexes*> formulaClauseIndexes;
  double priorMean;
};

/////////////////////////////////////////////////////////////////////////////

struct FormulaAndClauses 
{ 
  FormulaAndClauses(const string& fformula, const int& iindex, 
                    const bool& hhasExist)
    : formula(fformula), indexClauses(new IndexClauseHashArray), index(iindex), 
      hasExist(hhasExist), numPreds(-1), isHard(false), priorMean(0), wt(0),
      isExistUnique(false) {}
  ~FormulaAndClauses() { indexClauses->deleteItemsAndClear();
                         delete indexClauses; }
  string formula; 
  IndexClauseHashArray* indexClauses; 
  int index; 
  bool hasExist;
  int numPreds;
  bool isHard;
  double priorMean;
  double wt;
  bool isExistUnique; //contains existentially and uniquely quant. vars
};

class HashFormulaAndClauses
{
 public:
  size_t operator()(const FormulaAndClauses* const& f) const
  { return hash<char const *>()(f->formula.c_str()); }
};

class EqualHashFormulaAndClauses
{
 public:
  bool operator()(const FormulaAndClauses* const & f1, 
                  const FormulaAndClauses* const & f2) const
  { return (f1->formula.compare(f2->formula)==0); }
};

typedef HashArray<FormulaAndClauses*, HashFormulaAndClauses, 
  EqualHashFormulaAndClauses> FormulaAndClausesArray;


#endif
