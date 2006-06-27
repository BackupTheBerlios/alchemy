#ifndef CLAUSEHELPER_H_JUN_26_2005
#define CLAUSEHELPER_H_JUN_26_2005

struct VarsGroundedType
{
  VarsGroundedType() : isGrounded(false), typeId(-1), numGndings(-1) {}
  VarsGroundedType(const VarsGroundedType& vgt)
  {
    vars.copyFrom(vgt.vars);
    isGrounded = vgt.isGrounded;
    typeId = vgt.typeId;
    numGndings = vgt.numGndings;
  }

  Array<Term*> vars;  //addresses of variables with the same id
  bool isGrounded;   //indicate whether the variable has been grounded
  int typeId;        //type of the variable
  int numGndings;    //number of groundings of the variable
};


struct LitIdxVarIdsGndings
{
  unsigned int litIdx; //index of a literal in a clause
  Array<int> varIds;   //vars in literal (preceding pred's vars are grounded)
  ArraysAccessor<int> varGndings; //groundings of vars
  Array<Predicate*> subseqGndLits; //subsequent literals that are grounded
  bool litUnseen;  //this literal has not been seen
  Array<Predicate*> bannedPreds; // used when obtaining unknown clauses
};


typedef hash_set<string, HashString, EqualString> FormulaStringSet;


struct CacheCount
{
  CacheCount(const int& gg, const int& cc, const double& ccnt) :
    g(gg), c(cc), cnt(ccnt) {}
  ~CacheCount() {}
  int g; // gth grounding of predicate
  int c; // cth flip combination of the grounding (when in a block)
  double cnt; //counts
};


  // Operations that can be applied to a clause. Used to set AuxClauseData.op.
enum { OP_NONE = 0, OP_ADD = 1, OP_REMOVE = 2, OP_REPLACE = 3, 
       OP_REPLACE_ADDPRED = 4, OP_REPLACE_REMPRED = 5 };


  //NOTE: ensure that the AuxClauseData's member are copied in Clause's 
  //      copy constructor and at the beginning of StructLearn::run()
struct AuxClauseData
{
  AuxClauseData() : gain(0), constTermPtrs(NULL), op(OP_NONE), 
                    removedClauseIdx(-1), cache(NULL), removedPredIdx(-1) {}

  AuxClauseData(const double& ggain, const int& oop, 
                const int& rremovedClauseIdx)
    : gain(ggain), constTermPtrs(NULL), op(oop), 
      removedClauseIdx(rremovedClauseIdx), cache(NULL), removedPredIdx(-1) {}

  AuxClauseData(const double& ggain, const int& oop, 
                const int& rremovedClauseIdx, const string& prevClause,
                const string& addedPred, const int& remPredIdx)
    : gain(ggain), constTermPtrs(NULL), op(oop), 
      removedClauseIdx(rremovedClauseIdx), cache(NULL), 
      prevClauseStr(prevClause), addedPredStr(addedPred), 
      removedPredIdx(remPredIdx) {}

  ~AuxClauseData() 
  { 
    if (constTermPtrs) delete constTermPtrs; 
    if (cache) deleteCache();
  }


  void deleteCache()
  {
    if (cache)
    {
      for (int i = 0; i < cache->size(); i++) //for each domain
      {
        Array<Array<CacheCount*>*>* aacc = (*cache)[i];
        for (int j = 0; j < aacc->size(); j++) //for each predicate
        {
          Array<CacheCount*>* ccArr = (*aacc)[j];
          if (ccArr == NULL) continue;
          ccArr->deleteItemsAndClear();
        }
        aacc->deleteItemsAndClear();
      }
      cache->deleteItemsAndClear(); 
      delete cache; 
      cache = NULL;
    }
  }

  
  void compress()
  {
    if (constTermPtrs) constTermPtrs->compress();
    if (cache)
    {
      for (int i = 0; i < cache->size(); i++) //for each domain
      {
        Array<Array<CacheCount*>*>* aacc = (*cache)[i];
        for (int j = 0; j < aacc->size(); j++) //for each predicate
        {
          Array<CacheCount*>* ccArr = (*aacc)[j];
          if (ccArr == NULL) continue;
          ccArr->compress();
        }
        aacc->compress();
      }
      cache->compress();
    }
    
  }

    //approximate size in MB, mainly due to cache
  double sizeMB() const
  {
    double sz = fixedSizeB_;
    if (cache)
    {
      for (int i = 0; i < cache->size(); i++)
        for (int j = 0; j < (*cache)[i]->size(); j++)
        {
          if((*(*cache)[i])[j] == NULL) continue;
          sz += (*(*cache)[i])[j]->size() * (sizeof(CacheCount) +
                                             sizeof(CacheCount*));
        }
    }
    if (constTermPtrs) sz += constTermPtrs->size() * sizeof(Term*);
    return sz/1000000.0;
  }


  static void computeFixedSizeB() { fixedSizeB_ = sizeof(AuxClauseData); }


  void reset()
  {
    gain = 0;
    if (constTermPtrs) constTermPtrs->clear();
    op = OP_NONE;
    removedClauseIdx = -1;
    deleteCache(); cache = NULL;
    prevClauseStr = "";
    addedPredStr = "";
    removedPredIdx = -1;
  }

  double gain;
  Array<Term*>* constTermPtrs;
  int op;
  int removedClauseIdx;

    //cache[d][p] is an array of CacheCounts for predicate p in domain d
  Array<Array<Array<CacheCount*>*>*>* cache; 

  string prevClauseStr;
  string addedPredStr;
  int removedPredIdx;

  static double fixedSizeB_;
};


#endif
