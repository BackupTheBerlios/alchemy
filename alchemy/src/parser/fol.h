#ifndef FOL_H_JUN_21_2005
#define FOL_H_JUN_21_2005

#include "mln.h"
#include "domain.h"
#include "database.h"

  // Caller is responsible for deleting mln, dom, db and fileName if required.
  // This function reads the .mln, .kb and .db files. The db param will be
  // set to point to a Database object.
bool runYYParser(MLN* const & mln, Domain* const & domain,
                 const char* const & fileName, 
                 const bool& allPredsExceptQueriesAreClosedWorld,
                 const StringHashArray* const & openWorldPredNames,
                 const StringHashArray* const & queryPredNames,
                 const bool& addUnitClauses, const bool& warnDuplicates,
                 const double& defaultWt, const bool& mustHaveWtOrFullStop,
                 const Domain* const & domains, const bool& lazyInference,
                 const bool& flipWtsOfFlippedClause);

#endif
