/* This code is based on the MaxWalkSat package of Kautz et al. */
#ifndef _LAZY_WALKSAT_H_
#define _LAZY_WALKSAT_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <signal.h>

#include "freestoremanager.h"

#include "array.h"
#include "lwinfo.h"
#include "wsutil.h"
#include "maxwalksatparams.h"
#include "samplesatparams.h"

	/* Atoms start at 1 */
	/* Not a is recorded as -1 * a */
	/* One dimensional arrays are statically allocated. */
	/* Two dimensional arrays are dynamically allocated in */
	/* the second dimension only.  */
	/* (Arrays are not entirely dynamically allocated, because */
	/* doing so slows execution by 25% on SGI workstations.) */

class LazyWalksat
{
	 
 int numatom;
 int basenumatom;  // number of distinct atoms in the first set of unsatisfied clauses
 int numclause;
 int numliterals;

 int atommem;    //current allocation of memory for atoms
 int clausemem;  //current allocation of memory for clauses

 int saRatio;		// saRatio/100: percentage of SA steps
 int temperature;	// temperature/100: fixed temperature for SA step
 bool latesa;

 int ** clause;			/* clauses to be satisfied */
				/* indexed as clause[clause_num][literal_num] */
 int * cost;			/* cost of each clause */
 int * size;			/* length of each clause */
 int * falseclause;			/* clauses which are false */
 int * wherefalse;		/* where each clause is listed in false */
 int * numtruelit;		/* number of true literals in each clause */

 int ** occurence;	/* where each literal occurs */
				               /* indexed as occurence[literal+MAXATOM][occurence_num] */

 int * numoccurence;	    /* number of times each literal occurs */
 int * newnumoccurence;	/* number of times each literal occurs in the newly added clause set*/
 int * occurencemem;	    /* memory allocated (in terms of number of occurences) 
								   which can be stored for this literal */


 int * atom;		/* value of each atom */ 
 int * initatom; /* initial value of each atom */

 int * lowatom;

 int * changed;		/* step at which atom was last flipped */

 int * breakcost;	/* the cost of clauses that introduces if var is flipped */

 int numfalse;			/* number of false clauses */
 int costofnumfalse;		/* cost associated with the number of false clauses */


 bool costexpected;       /*indicate whether cost is expected from the input*/
 bool abort_flag;

 int heuristic;			/* heuristic to be used */
 int numerator;			/* make random flip with numerator/denominator frequency */
 int denominator;		
 int tabu_length;		/* length of tabu list */

 long int numflip;		/* number of changes so far */
 long int numnullflip;		/*  number of times a clause was picked, but no  */
				/*  variable from it was flipped  */

 int highestcost;               /*The highest cost of a clause violation*/
 int eqhighest;	/*Is there a clause violated with the highest cost*/
 int numhighest;      /*Number of violated clauses with the highest cost*/

 bool hard; /* if true never break a highest cost clause */

 bool noApprox; /* If set to true, atoms are not deactivated when mem. is full */

 FreeStoreManager *clauseFSMgr;  //free store manager
 int clauseFreeStore; //out of the memory allocated, how much is free for usage
 int *clauseStorePtr; //free storage ptr

 FreeStoreManager *atomFSMgr;  //free store manager
 int atomFreeStore; //out of the memory allocated, how much is free for usage
 int *atomStorePtr; //free storage ptr

 Array<Array<int> *> allClauses;
 Array<int> allClauseWeights;
 Array<Array<int> *> newClauses;
 Array<int> newClauseWeights;

    // Holds the initial set of active clauses for MC-SAT
 Array<IntClause *> supersetClauses_;

    // Holds atoms which have been fixed due to unit prop.
 //Array<int> fixedAtoms_;
 bool * fixedAtoms_;
 int maxFixedAtoms;

 LWInfo *lwInfo;

	// Max. amount of memory to use
 int memLimit;
 	// Max. amount of clauses memory can hold
 int clauseLimit;
 	// Indicates whether deactivation of atoms has taken place yet
 bool haveDeactivated;
 	// Initial number of clauses
 int initClauseCount;
    // Initial number of atoms
 int initAtomCount;

public:

 LazyWalksat(LWInfo *lwInfo, int memLimit); 

 ~LazyWalksat();
 
 /* perform the lazy samplesat inference */ 
 bool sample(const MaxWalksatParams* const & mwsParams,
 			 const SampleSatParams& sampleSatParams,
             const bool& initial);

 /* perform the lazy walksat inference */ 
 void infer(const MaxWalksatParams* const & params,
 			const int& numSolutions, const bool&  includeUnsatSolutions,
        	Array<Array<bool>*>& solutions, Array<int>& numBad,
        	const bool& initial);
        	
 int getNumInitClauses();
 
 int getNumInitAtoms();

 int getNumClauses();
 
 void resetSampleSat();
 
 void randomizeActiveAtoms();
 
private:

 /* initialize - add the initial set of clauses*/
 void init();

 /* initialize a new run */
 void initrun(void);                 

 /* changes an atom to make the given clause true */
 void fix(int tofix);                 

 /* added by Parag - add clauses*/
 void addNewClauses(bool initial);

 /* Deactivate clauses to save memory */
 void trimClauses();

 /* Using this temporarily until all old arrays converted to type Array */
 void removeAllNullFromArray(int array[]);

/* Initialize breakcost in the following: */
 void initializeBreakCost(int startclause); 

 /* update the structures */

 /*initialize the clause memory*/
 void initializeClauseMemory();

 /*initialize the atom memory */
 void initializeAtomMemory();

 /*  allocate memory for clauses */
 void allocateClauseMemory(int allocCount);
		 
 /*  allocate memory for clauses */
 void allocateAtomMemory(int allocCount);

 /* delete memory allocated for clauses */
 void deleteClauseMemory();

 /* delete memory allocated for atoms */
 void deleteAtomMemory();    

 //get the total of the weights of elements in the
 //clauseWeights array
 int getTotalWeight(Array<int> &clauseWeights);

 /* changes the assignment of the given literal */
 void flipatom(int toflip);           

 int selecthigh(int);   /*find the int'th clause with the highest cost*/

 double elapsed_seconds(void);
 int countunsat(void);
 void countlowunsatcost( int *, int *);
 void scanone(int argc, char *argv[], int i, int *varptr);

 int pickrandom(int *numbreak,int clausesize, int tofix);
 int pickproductsum(int *numbreak,int clausesize, int tofix);
 int pickreciprocal(int *numbreak,int clausesize, int tofix);
 int pickadditive(int *numbreak,int clausesize, int tofix);
 int pickbest(int *numbreak,int clausesize, int tofix);
 int picktabu(int *numbreak,int clausesize, int tofix);
 int pickexponential(int *numbreak,int clausesize, int tofix);
 int pickzero(int *numbreak,int clausesize);
 int pickweight(int *weight,int clausesize);

 bool simAnnealing();
 
 void undoFixedAtoms();
 void initFixedAtoms(Array<Array<int> *> &clauses,
                     Array<int> &clauseWeights);

 void print_false_clauses_cost(long int lowbad);
 void print_low_assign(long int lowbad);

 void printClause(int clause);

 void save_low_assign(void);

 long super(int i);

};

#endif
