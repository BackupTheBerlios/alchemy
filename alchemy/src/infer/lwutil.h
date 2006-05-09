#ifndef _LW_UTIL_H_
#define _LW_UTIL_H_

//#define WSCALE 10
#define WEIGHT_EPSILON .0001

class LWUtil {

public:

static int mySign(int x)
{
  return (x>0)?1:0; 
}

static void setAllActive(Domain * const & domain)
{
  Array<Predicate*> gpreds;
  Predicate *gpred;

  for(int predno = 0; predno < domain->getNumPredicates(); predno++)
  {
	gpreds.clear();
	PredicateTemplate *ptemplate = (PredicateTemplate *)domain->getPredicateTemplate(predno);	
	if(ptemplate->isEqualPredWithType())
	  continue;
	Predicate::createAllGroundings(predno, domain, gpreds);
		
	for (int i = 0; i < gpreds.size(); i++) 
    {
	  gpred = gpreds[i];
	  bool evidence = domain->getDB()->getEvidenceStatus(gpred);
	  if(!evidence) 
	    domain->getDB()->setActiveStatus(gpred,true);
	  delete gpred;
	}
  }
}

};

#endif

