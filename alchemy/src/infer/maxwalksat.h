/* This code is based on the MaxWalkSat package of Kautz et al. */
#ifndef MAXWALKSAT_H_OCT_1_2005
#define MAXWALKSAT_H_OCT_1_2005

#include <cfloat>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <cstring>
#include <sstream>
#include <fstream>
#include "array.h"
#include "timer.h"
#include "util.h"
#include "wsutil.h"
#include "maxwalksatparams.h"

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
  MaxWalksat(const Array<Array<int> >* const & blocks,
             const Array<bool>* const & blockEvidence)
  {
    costexpected = false;   
    highestcost = 1;               
    eqhighest = 0;    
    numhighest = 0;      
    hard = 0;
    
    blocks_ = new Array<Array<int> >(*blocks);
    blockEvidence_ = new Array<bool>(*blockEvidence);
  
      // Init to not in block
    inBlock = false;
    flipInBlock = NOVALUE;
  }

  ~MaxWalksat()
  {
    free(occurence);
    free(numoccurence);
    free(atom);
    free(lowatom);
    free(changed);
    free(breakcost);
    
    free(clause);
    free(cost);
    free(size);
    free(falseclause);
    free(lowfalse);
    free(wherefalse);
    free(numtruelit);
     
    for(int i = 0; i < storePtrArray.size(); i++)
      free(storePtrArray[i]);
      
    for (int i = 0; i < blocks_->size(); i++)
      (*blocks_)[i].clearAndCompress();
    delete blocks_;
    
    delete blockEvidence_; 
  }

  // Caller is responsible for deleting the contents of solutions
  void sample(const MaxWalksatData* const & data, const int& identifier,
              Array<Array<bool>*>& solutions,
              const MaxWalksatParams* const & params,
              const Array<Array<int> >* const & blocks,
              const Array<bool>* const & blockEvidence)
  {
    const Array<Array<int>*>& clauses = data->clauses;
    const Array<double>& clauseWts = data->clauseWts;
    int numGndPreds = data->numGndPreds;
    int numSolutions = 1;
    bool includeUnsatSolutions = true;
    int maxSteps = params->maxSteps;
    int tries = params->tries;
    int targetWt = params->targetWt;
    bool hard = params->hard;
	
    char targetWtParam[20]; targetWtParam[0] = '\0';
    if (targetWt != INT_MAX) sprintf(targetWtParam, "-targetcost %d", targetWt);
    
    char hardParam[20]; hardParam[0] = '\0';
    if (hard) sprintf(hardParam, "-hard");

      // run Max Walksat
    Array<int> numBad;
    string cnfFile, resultsFile;
    string commandStr;

	Array<char *> argArray;
    argArray.append("null"); 
	argArray.append("-withcost"); 
	argArray.append("-tries");
    argArray.append((char *)(Util::intToString(tries).c_str()));
	argArray.append("-numsol");
    argArray.append((char *)(Util::intToString(numSolutions).c_str()));
	argArray.append("-cutoff");
    argArray.append((char *)(Util::intToString(maxSteps).c_str()));
	 
	if(strcmp(targetWtParam,"")) 
	  argArray.append(targetWtParam);
	 
	if(strcmp(hardParam,"")) 
	  argArray.append(hardParam);
	 
	argArray.append("-low");   
	 
	int argc = argArray.size();
	char **argv = (char **)argArray.getItems();
    Array<int> truePreds;
	int numbad = 0;
    
    for (int i = 0; i < blocks_->size(); i++)
      (*blocks_)[i].clearAndCompress();
    delete blocks_;
    delete blockEvidence_;
  
    blocks_ = new Array<Array<int> >(*blocks);
    blockEvidence_ = new Array<bool>(*blockEvidence);
  
	//for(int i = 0; i < argc; i++) 
	//  cout << argv[i] << " ";
	//cout<<endl;
	 
	infer(argc, argv, numGndPreds, clauses, clauseWts, truePreds, numbad);
    bool ok = readResults(truePreds, numbad, numGndPreds, includeUnsatSolutions, 
                          solutions, numBad);
    if (!ok) return;
  }
 
 
  int infer(int argc, char *argv[], int atomCnt,
            const Array<Array<int>*>& clauseArray, 
            const Array<double>& wtArray, Array<int>& trueAtomArray,
            int & numbad)
  {
    int i;          /* loop counter */
    int k;          /* another loop counter */
    
    int numrun = 10;
    int cutoff = 100000;
    int base_cutoff = 100000;
    bool printonlysol = false;
    //bool printsolcnf = false;
    bool printfalse = false;
    bool printlow = false;
    bool initoptions = false;
    bool superlinear = false;
    //int printtrace = 10000;
    int printtrace = 0;
    long int totalflip = 0; /* total number of flips in all tries so far */
    long int totalsuccessflip = 0; /* total number of flips in all tries which succeeded so far */
    int numtry = 0;     /* total attempts at solutions */
    int numsuccesstry = 0;  /* total found solutions */
    int numsol = 1;         /* stop after this many tries succeeds */
    int seed;           /* seed for random */
    struct timeval tv;
    struct timezone tzp;
    
    int prior_setuptime=0;
    double setuptime;
    double expertime;
    double totaltime;
    
    long int lowbad;        /* lowest number of bad clauses during try */
    long int lowcost;       /* lowest cost of bad clauses during try */
    int targetcost = 0;     /* the cost at which the program terminates*/
    long x;
    long integer_sum_x = 0;
    double sum_x = 0.0;
    double sum_x_squared = 0.0;
    double mean_x=-1;
    double second_moment_x;
    double variance_x=-1;
    double std_dev_x=-1;
    double std_error_mean_x=-1;
    double seconds_per_flip;
    int r;
    int sum_r = 0;
    double sum_r_squared = 0.0;
    double mean_r = -1;
    double variance_r;
    double std_dev_r;
    double std_error_mean_r;
    int worst_cost, computed_cost;

    gettimeofday(&tv,&tzp);
    seed = (( tv.tv_sec & 0177 ) * 1000000) + tv.tv_usec;
    heuristic = BEST;
    numerator = NOVALUE;
    denominator = 100;

    for (i = 1; i < argc; i++)
    {
      if (strcmp(argv[i],"-withcost") == 0)
        costexpected = true;
      else if (strcmp(argv[i],"-seed") == 0)
        scanone(argc, argv, ++i, &seed);
      else if (strcmp(argv[i],"-targetcost") == 0)
        scanone(argc, argv, ++i, &targetcost);
      else if (strcmp(argv[i],"-cutoff") == 0)
        scanone(argc, argv, ++i, &cutoff);
      else if (strcmp(argv[i],"-random") == 0)
        heuristic = RANDOM;
      else if (strcmp(argv[i],"-productsum") == 0)
        heuristic = PRODUCTSUM;
      else if (strcmp(argv[i],"-reciprocal") == 0)
        heuristic = RECIPROCAL;
      else if (strcmp(argv[i],"-additive") == 0)
        heuristic = ADDITIVE;
      else if (strcmp(argv[i],"-exponential") == 0)
        heuristic = EXPONENTIAL;
      else if (strcmp(argv[i],"-best") == 0)
        heuristic = BEST;
      else if (strcmp(argv[i],"-noise") == 0)
      {
        scanone(argc,argv,++i,&numerator);
        scanone(argc,argv,++i,&denominator);
      }
      else if (strcmp(argv[i],"-super") == 0)
        superlinear = true;
      else if (strcmp(argv[i],"-tries") == 0)
        scanone(argc,argv,++i,&numrun);
      else if (strcmp(argv[i],"-tabu") == 0)
      {
        scanone(argc,argv,++i,&tabu_length);
        heuristic = TABU;
      }
      else if (strcmp(argv[i],"-low") == 0)
        printlow = true;
      else if (strcmp(argv[i],"-sol") == 0)
      {
        printonlysol = true;
        printlow = true;
      }
      else if (strcmp(argv[i],"-bad") == 0)
        printfalse = true;
      else if (strcmp(argv[i],"-hard") == 0)
        hard = true;
      else if (strcmp(argv[i],"-numsol") == 0)
        scanone(argc, argv, ++i, &numsol);
      else if (strcmp(argv[i],"-trace") == 0)
        scanone(argc, argv, ++i, &printtrace);
      //to incorporate the time taken in setting up the walksat clauses etc.
      else if (strcmp(argv[i],"-setuptime") == 0)
        scanone(argc, argv, ++i, &prior_setuptime);
      else 
      {
        fprintf(stderr, "Bad argument %s\n", argv[i]);
        fprintf(stderr, "General parameters:\n");
        fprintf(stderr, "  -seed N -cutoff N -tries N\n");
        fprintf(stderr, "  -numsol N = stop after finding N solutions\n");
        fprintf(stderr, "  -withcost = input is a set of weighted clauses\n");
        fprintf(stderr, "  -targetcost N = find assignments of cost <= N (MAXSAT)\n");
        fprintf(stderr, "  -hard = never break a highest-cost clause\n");
        fprintf(stderr, "Heuristics:\n");
        fprintf(stderr, "  -noise N M -best -super -tabu N\n");
        fprintf(stderr, "  -productsum -reciprocal -additive -exponential\n");
        fprintf(stderr, "Printing:\n");
        fprintf(stderr, "  -trace N = print statistics every N flips\n");
        fprintf(stderr, "  -sol = print assignments where cost < target\n");
        fprintf(stderr, "  -low = print lowest assignment each try\n");
        fprintf(stderr, "  -bad = print unsat clauses each try\n");
        fprintf(stderr, "  -solcnf = print sat assign in cnf format, and exit\n");
        fprintf(stderr, "  -setuptime S = time taken for setting up the clauses for walksat\n");
        exit(-1);
      }
    }
        
    base_cutoff = cutoff;
    if (numerator==NOVALUE)
    {
      if (heuristic==BEST)
        numerator = 50;
      else
        numerator = 0;
    }

    srandom(seed);

    if (printtrace)
    {
      printf("maxwalksat version 20\n");
      printf("seed = %i\n",seed);
      printf("cutoff = %i\n",cutoff);
      printf("tries = %i\n",numrun);
      printf("numsol = %i\n",numsol);
      printf("targetcost = %i\n",targetcost);

      printf("heuristic = ");

      switch(heuristic)
      {
      case RANDOM:
        printf("random");
        break;
      case PRODUCTSUM:
        printf("productsum");
        break;
      case RECIPROCAL:
        printf("reciprocal");
        break;
      case ADDITIVE:
        printf("additive");
        break;
      case BEST:
        printf("best");
        break;
      case EXPONENTIAL:
        printf("exponential");
        break;
      case TABU:
        printf("tabu %d", tabu_length);
        break;
      }
    
      if (numerator > 0)
      {
        printf(", noise %d / %d", numerator, denominator);
      }

      if (superlinear)
        printf(", super");

      printf(", trace %d", printtrace);

      printf("\n");
      printf("Prior set up time = %d \n", prior_setuptime);
    }
    
    expertime = 0;
    elapsed_seconds();
    initprob(atomCnt, clauseArray, wtArray);
    setuptime = prior_setuptime +  elapsed_seconds();
    
    if (printtrace)
    {
      if (costexpected) printf("clauses contain explicit costs\n");
      else printf("clauses all assigned default cost of 1\n");

      if(hard)
        printf("hard option is set \n");
      else
        printf("hard option is not set \n");

    
      printf("numatom = %i, numclause = %i, numliterals = %i\n",numatom,numclause,numliterals);
    
      printf("                                           average             average       mean              standard\n");
      printf("    lowest     worst    number                when                over      flips                 error\n");
      printf("      cost    clause    #unsat    #flips     model   success       all      until        std         of\n");
      printf("  this try  this try  this try  this try     found      rate     tries     assign        dev       mean\n");
    }
    abort_flag = false;
    numnullflip = 0;
    x = 0; r = 0;
    lowcost = BIG;
    
    for(k = 0; k < numrun; k++)
    {
      if (printtrace) printf("performing the Run %d \n", k);
      init(initoptions);
      lowbad = numfalse; 
      lowcost = costofnumfalse;
      save_low_assign();
      numflip = 0;

      if (superlinear) cutoff = base_cutoff * super(r+1);
    
      if (printtrace)
      {
        printf("in the beginning, costofnumfalse = %d \n",costofnumfalse); 
        printf("cutoff = %d \n",cutoff);
        printf("numflip = %li \n",numflip);
        printf("numfalse = %d \n",numfalse);
        printf("numclause = %d \n",numclause);
      
        printf("costofnumfalse \t  numfalse \t numflip \t numclause \n");
      }
      while((numflip < cutoff) && (costofnumfalse > targetcost))
      {
        //printf("flip number .. %d \n",numflip);
        if (printtrace && (numflip % printtrace == 0))
        {
          printf("%10i \t %10i \t %10li \t %10i\n", costofnumfalse,numfalse,numflip,numclause);
        }
        numflip++;

        if (hard && (eqhighest) && (highestcost!=1))
          fix(selecthigh(1 + random()%numhighest));
        else
          fix(falseclause[random()%numfalse]);
         
        if (costofnumfalse < lowcost)
        {
          lowcost = costofnumfalse;
          lowbad = numfalse;
          save_low_assign();       
        }

        if(numflip % 100000 == 0 && printtrace)
        {
          expertime += elapsed_seconds();     
          totaltime = setuptime + expertime;
          printf("Averaging for %li flips \n",(totalflip+numflip));
          printf("expertime - average flips per second = %10li\n", (long)((totalflip+numflip)/expertime));
          printf("totaltime - average flips per second = %10li\n", (long)((totalflip+numflip)/totaltime));
          printf("numatom = %d , numclause = %d \n",numatom,numclause);
          printf("\n");
        }
      }

      if(printtrace)
      {
        printf("Inside the run %d \n",k);
        printf("numatom = %d , numclause = %d \n",numatom,numclause);
      }
      
      numtry++;
      totalflip += numflip;
      x += numflip;
      r++;
      
        // If succesful try
      if(costofnumfalse <= targetcost)
      {
        numsuccesstry++;
        totalsuccessflip += numflip;
        integer_sum_x += x;
        sum_x = (double) integer_sum_x;
        sum_x_squared += ((double)x)*((double)x);
        mean_x = sum_x / numsuccesstry;
        if (numsuccesstry > 1)
        {
          second_moment_x = sum_x_squared / numsuccesstry;
          variance_x = second_moment_x - (mean_x * mean_x);
            /* Adjustment for small small sample size */
          variance_x = (variance_x * numsuccesstry)/(numsuccesstry - 1);
          std_dev_x = sqrt(variance_x);
          std_error_mean_x = std_dev_x / sqrt((double)numsuccesstry);
        }
        sum_r += r;
        mean_r = ((double)sum_r)/numsuccesstry;
        sum_r_squared += ((double)r)*((double)r);
        x = 0;
        r = 0;
      }

      countlowunsatcost(&computed_cost, &worst_cost);
      if(lowcost != computed_cost)
      {
        fprintf(stderr, "Program error, verification of assignment cost fails!\n");
        exit(-1);
      }
/*      
      if(numsuccesstry == 0)
        printf("%10li%10i%10li%10li         *         0         *          *          *          *\n",
               lowcost,worst_cost,lowbad,numflip);
      else if (numsuccesstry == 1)
        printf("%10li%10i%10li%10li%10li%10i%10li %10.1f          *          *\n",
               lowcost,worst_cost,lowbad,numflip,totalsuccessflip/numsuccesstry,
               (numsuccesstry*100)/numtry,totalflip/numsuccesstry,
               mean_x);
      else
        printf("%10li%10i%10li%10li%10li%10i%10li %10.1f %10.1f %10.1f\n",
               lowcost,worst_cost,lowbad,numflip,totalsuccessflip/numsuccesstry,
               (numsuccesstry*100)/numtry,totalflip/numsuccesstry,
               mean_x, std_dev_x, std_error_mean_x);

      if (numfalse>0 && printfalse)
        print_false_clauses_cost(lowbad);

      if (printlow && (!printonlysol || costofnumfalse <= targetcost))
        print_low_assign(lowcost);
*/
      
      extract_low_assign(trueAtomArray);
      numbad = lowcost;
      
      if (numsuccesstry >= numsol) break;
      if (abort_flag) break;
    }
    
    expertime += elapsed_seconds();
    totaltime = setuptime + expertime;
    seconds_per_flip = expertime / totalflip;

    if(printtrace)
    {
      printf("\nset up seconds = %f\n", setuptime);
      printf("\nwalksat run seconds = %f\n", expertime);
      printf("\ntotal elapsed seconds = %f\n", totaltime);
    
      printf("Averaging for all the flips (%li) \n",totalflip);
      printf("expertime - average flips per second = %10li\n", (long)(totalflip/expertime));
      printf("totaltime - average flips per second = %10li\n", (long)(totalflip/totaltime));
      printf("\n");
    }
/*    
    if (heuristic == TABU)
      printf("proportion null flips = %f\n", ((double)numnullflip)/totalflip);
    printf("number of solutions found = %d\n", numsuccesstry);
*/    
    if (numsuccesstry > 0)
    {
/*
      printf("mean flips until assign = %f\n", mean_x);
      if (numsuccesstry > 1)
      {
        printf("  variance = %f\n", variance_x);
        printf("  standard deviation = %f\n", std_dev_x);
        printf("  standard error of mean = %f\n", std_error_mean_x);
      }
      printf("mean seconds until assign = %f\n", mean_x * seconds_per_flip);
      
      if (numsuccesstry > 1)
      {
        printf("  variance = %f\n", variance_x * seconds_per_flip * seconds_per_flip);
        printf("  standard deviation = %f\n", std_dev_x * seconds_per_flip);
        printf("  standard error of mean = %f\n", std_error_mean_x * seconds_per_flip);
      }
      printf("mean restarts until assign = %f\n", mean_r);
*/
      if (numsuccesstry > 1)
      {
        variance_r = (sum_r_squared / numsuccesstry) - (mean_r * mean_r);
        variance_r = (variance_r * numsuccesstry)/(numsuccesstry - 1);       
        std_dev_r = sqrt(variance_r);
        std_error_mean_r = std_dev_r / sqrt((double)numsuccesstry);
        //printf("  variance = %f\n", variance_r);
        //printf("  standard deviation = %f\n", std_dev_r);
        //printf("  standard error of mean = %f\n", std_error_mean_r);
      }
    }
/*
    if (numsuccesstry > 0)
    {
      printf("ASSIGNMENT ACHIEVING TARGET %i FOUND\n", targetcost);
      if(printsolcnf)
        for(i = 1;i < numatom+1; i++)
          printf("v %i\n", atom[i] == 1 ? i : -i);
    }
    else
      printf("ASSIGNMENT NOT FOUND\n");
*/    
    return 0;
  }
 
 private:
    
  // numBad is the reported number of unsat clauses
  // Caller is responsible for deleting the contents of solutions
  bool readResults(Array<int> truePreds, int numbad, const int& numGndPreds, 
                   const bool&  includeUnsatSolutions,
                   Array<Array<bool>*>& solutions, Array<int>& numBad) 
  {
    if (includeUnsatSolutions || numbad == 0) 
    {
      Array<bool>* solution = new Array<bool>(numGndPreds);
      for (int i = 0; i < numGndPreds; i++)
        solution->append(false);
      
      for (int i = 0; i < truePreds.size(); i++)  
		(*solution)[truePreds[i]-1] = true;
      
      solutions.append(solution);
      numBad.append(numbad);
    }
    return true;
  }
  
  
  
  long super(int i)
  {
    long power;
    int k;

    if (i <= 0)
    {
      fprintf(stderr, "bad argument super(%d)\n", i);
      exit(1);
    }
    /* let 2^k be the least power of 2 >= (i+1) */
    k = 1;
    power = 2;
    while (power < (i+1))
    {
      k += 1;
      power *= 2;
    }
    if (power == (i+1)) return (power/2);
    return (super(i - (power/2) + 1));
  }

  void handle_interrupt(int sig)
  {
    if (abort_flag) exit(-1);
    abort_flag = true;
  }


  void scanone(int argc, char *argv[], int i, int *varptr)
  {
    if (i >= argc || sscanf(argv[i],"%i",varptr) != 1)
    {
      fprintf(stderr, "Bad argument %s\n", i<argc ? argv[i] : argv[argc-1]);
      exit(-1);
    }
  }

  void init(int initoptions)
  {
    int i;          /* loop counter */
    int j;          /* another loop counter */
    int thetruelit = -1;

    for(i = 0; i < numclause; i++)
      numtruelit[i] = 0;
    numfalse = 0;
    costofnumfalse = 0;
    eqhighest = 0;
    numhighest = 0;

    for(i = 1; i < numatom + 1; i++)
    {
      changed[i] = -BIG;
      breakcost[i] = 0;
    }

      // For each block: select one to set to true
    for (int i = 0; i < blocks_->size(); i++)
    {
      Array<int> block = (*blocks_)[i];
      
        // If evidence atom exists, then all others are false
      if ((*blockEvidence_)[i])
      {
          // If 1st argument is -1, then all are set to false
        setOthersInBlockToFalse(-1, i);
        continue;
      }
      
      int chosen = random() % block.size();
      atom[block[chosen] + 1] = true;
      setOthersInBlockToFalse(chosen, i);
    }
      
      // Random tv for all not in blocks
    for(i = 1; i < numatom + 1; i++)
    {
      if (getBlock(i - 1) >= 0)
      {
        continue;
      }
      atom[i] = random() % 2;
    }

    /* Initialize breakcost in the following: */
    for(i = 0; i < numclause; i++)
    {
      for(j = 0; j < size[i]; j++)
      {
        if((clause[i][j] > 0) == atom[abs(clause[i][j])])
        {
          numtruelit[i]++;
          thetruelit = clause[i][j];
        }
      }
      
        // Unsatisfied positive-weighted clauses
      if(numtruelit[i] == 0 && cost[i] >= 0)
      {
        wherefalse[i] = numfalse;
        falseclause[numfalse] = i;
        ++numfalse;
        costofnumfalse += cost[i];
        if (highestcost == cost[i]) {eqhighest = true; numhighest++;}
      }
        // Satisfied negative-weighted clauses
      else if(numtruelit[i] > 0 && cost[i] < 0)
      {
        wherefalse[i] = numfalse;
        falseclause[numfalse] = i;
        ++numfalse;
        costofnumfalse += abs(cost[i]);
        if (highestcost == abs(cost[i])) {eqhighest = true; numhighest++;}
      }
        // Pos. clause satisfied by exactly one literal: note breakcost for that lit
      else if (numtruelit[i] == 1 && cost[i] >= 0)
      {
        breakcost[abs(thetruelit)] += cost[i];
      }
        // Neg. clause not satisfied: note breakcost for all lits
      else if (numtruelit[i] == 0 && cost[i] < 0)
      {
        for(int j = 0; j < size[i]; j++)
        {
          breakcost[abs(clause[i][j])] += abs(cost[i]);
        }
      }
    }
  }

  void print_false_clauses_cost(long int lowbad)
  {
    int i, j;
    bool bad;
    int lit, sign;

    printf("Unsatisfied clauses:\n");
    for (i = 0; i < numclause; i++)
    {
      bad = true;
      for (j = 0; j < size[i]; j++)
      {
        lit = clause[i][j];
        sign = lit > 0 ? 1 : 0;
        if ( lowatom[abs(lit)] == sign )
        {
          bad = false;
          break;
        }
      }
      
      if (bad)
      {
        printf("Cost %i ", cost[i]);
        for (j = 0; j < size[i]; j++)
        {
          printf("%d ", clause[i][j]);
        }
        printf("0\n");
      }
    }
    printf("End unsatisfied clauses\n");
  }


  void initprob(int atomCnt, const Array<Array<int>*> &clauseArray,
                const Array<double>& wtArray)
  {
    int i;          /* loop counter */
    int j;          /* another loop counter */
    int *storeptr = NULL;
    int freestore;
    int lit;
    
    numatom = atomCnt;
    numclause = clauseArray.size();
    //cout<<"came in to read the clauses.. "<<endl;
      
    occurence = (int **) malloc(sizeof(int *)*(2*numatom+1));  
    numoccurence = (int *) malloc(sizeof(int)*(2*numatom+1));
    atom = (int *) malloc(sizeof(int)*(numatom+1));
    lowatom = (int *) malloc(sizeof(int)*(numatom+1));
    changed = (int *) malloc(sizeof(int)*(numatom+1)); 
    breakcost = (int *) malloc(sizeof(int)*(numatom+1)); 

    
    clause = (int **) malloc(sizeof(int *)*(numclause+1));
    cost = (int *) malloc(sizeof(int)*(numclause+1));
    size = (int *) malloc(sizeof(int)*(numclause+1));
    falseclause = (int *) malloc(sizeof(int)*(numclause+1));
    lowfalse = (int *) malloc(sizeof(int)*(numclause+1));
    wherefalse = (int *) malloc(sizeof(int)*(numclause+1));
    numtruelit = (int *) malloc(sizeof(int)*(numclause+1));
    freestore = 0;
    numliterals = 0;

    for(i = 0; i < 2*numatom + 1; i++)
      numoccurence[i] = 0;
    
    for(i = 0; i < numclause; i++)
    {
      if (costexpected)
      {
          //cout<<"not assigning default cost. some problem.."<<endl;
        cost[i] = (int)wtArray[i];
      }
      else
      {
          /*the default cost of a clause violation is unit 1*/
        if (wtArray[i] >= 0)
          cost[i] = 1;
        else
          cost[i] = -1;
      }

      size[i] = 0;
      if (freestore < MAXLENGTH)
      {
        storeptr = (int *) malloc( sizeof(int) * BLOCKSIZE );
        storePtrArray.append(storeptr);
        freestore = BLOCKSIZE;
        fprintf(stderr,"allocating memory...\n");
      }
      clause[i] = storeptr;

      for(int litno = 0; litno < clauseArray[i]->size(); litno++)
      {
        size[i]++;
        if(size[i] > MAXLENGTH)
        {
          printf("ERROR - clause too long\n");
          exit(-1);
        }
          
          //Convert to a representation which starts indexing from 1 (instead of 0).
          //This operation is same as the one done while preparing CNF file (in maxwalksat.h)
          //for use the by oringial maxwalksat code
          //This is probably not the right place to do - should be
          //fixed sometime.
        int litId = WSUtil::getLiteral((*clauseArray[i])[litno]);
        bool negated = WSUtil::isLiteralNegated((*clauseArray[i])[litno]);
        if (negated) 
          lit = -(litId+1);
        else
          lit = litId+1;
          
        *(storeptr++) = lit; /* clause[i][size[i]] = j; */
        freestore--;
        numliterals++;
        numoccurence[lit+numatom]++;
      }
    }
    
    //cout<<"done with reading in clauses.. "<<endl;
    if(size[0] == 0)
    {
      fprintf(stderr,"ERROR - incorrect problem format or extraneous characters\n");
      exit(-1);
    }

    for(i = 0; i < 2*numatom + 1; i++)
    {
      if (freestore < numoccurence[i])
      {
        storeptr = (int *) malloc( sizeof(int) * BLOCKSIZE );
        storePtrArray.append(storeptr);
        freestore = BLOCKSIZE;
        fprintf(stderr,"allocating memory...\n");
      }
      occurence[i] = storeptr;
      freestore -= numoccurence[i];
      storeptr += numoccurence[i];
      numoccurence[i] = 0;
    }

    for(i = 0; i < numclause; i++)
    {
      for(j = 0; j < size[i]; j++)
      {
        occurence[clause[i][j]+numatom][numoccurence[clause[i][j]+numatom]] = i;
        numoccurence[clause[i][j]+numatom]++;
      }
    }
  }


  void fix(int tofix)
  {
      // If neg. clause, then flip all true literals
    if (cost[tofix] < 0)
    {
      if (numtruelit[tofix] > 0)
      {
        for(int i = 0; i < size[tofix]; i++)
        {
            //TODO: all atoms in block in this clause?
          int atm = abs(clause[tofix][i]);
            // true literal: flip it
          if((clause[tofix][i] > 0) == atom[atm])
          {
              // Flip actual atom
            flipatom(atm);
              // Check if in block
            int blockIdx = getBlock(atm - 1);
            if(blockIdx >= 0)
            {
                //Dealing with atom in a block
              Array<int> block = (*blocks_)[blockIdx];
              int chosen = -1;
    
                // 1->0: choose to flip the other randomly
              bool ok = false;
              while(!ok)
              {
                chosen = random() % block.size();
                if (block[chosen] + 1 != atm)
                  ok = true;
              }
    
              assert(chosen >= 0);
              assert(atom[block[chosen] + 1] == 0);
              flipatom(block[chosen] + 1);
            }
          }
        }
      }
        // No true lits: already fixed
      else
      {
        numnullflip++;
        return;
      }
    }
    else
    {
      int numbreak[MAXLENGTH];  // number of clauses changing
                                // each atoms would make false
      int i;          /* loop counter */
      int choice;
    
      for(i = 0; i < size[tofix]; i++)
      {
        numbreak[i] = breakcost[abs(clause[tofix][i])];
      }
        
      switch(heuristic)
      {
        case RANDOM:      
          choice = pickrandom(numbreak,size[tofix],tofix);
          break;
        case PRODUCTSUM:  
          choice = pickproductsum(numbreak,size[tofix],tofix);
          break;
        case RECIPROCAL:  
          choice = pickreciprocal(numbreak,size[tofix],tofix);
          break;
        case ADDITIVE:    
          choice = pickadditive(numbreak,size[tofix],tofix);
          break;
        case BEST:        
          choice = pickbest(numbreak,size[tofix],tofix);
          break;
        case EXPONENTIAL: 
          choice = pickexponential(numbreak,size[tofix],tofix);
          break;
        case TABU:        
          choice = picktabu(numbreak,size[tofix],tofix);
          break;
        default:          
          choice = pickbest(numbreak,size[tofix],tofix);
      }

      if (choice == NOVALUE)
      {
        numnullflip++;
          // set in block back to false
        inBlock = false;
        flipInBlock = NOVALUE;
      }
      else
      {
        flipatom(abs(clause[tofix][choice]));
        if (inBlock)
        {
          flipatom(flipInBlock);
        }
          // set in block back to false
        inBlock = false;
        flipInBlock = NOVALUE;
      }
    }
  }


  void flipatom(int toflip)
  {
    int i, j;           /* loop counter */
    int toenforce;      /* literal to enforce */
    register int cli;
    register int lit;
    int numocc;
    register int sz;
    register int * litptr;
    int * occptr;

    changed[toflip] = numflip;
    if(atom[toflip] > 0)
      toenforce = -toflip;
    else
      toenforce = toflip;
    atom[toflip] = 1-atom[toflip];
    
    numocc = numoccurence[numatom-toenforce];
    occptr = occurence[numatom-toenforce];
    
    
    for(i = 0; i < numocc ;i++)
    {
      cli = *(occptr++);

       // Neg. clause
      if (cost[cli] < 0)
      {
          // Became unsatisfied: Decrease num. of false clauses
        if (--numtruelit[cli] == 0)
        {
          numfalse--;
          costofnumfalse -= abs(cost[cli]);
          falseclause[wherefalse[cli]] = falseclause[numfalse];
          wherefalse[falseclause[numfalse]] = wherefalse[cli];
          
          if (abs(cost[cli]) == highestcost)
            { numhighest--; if (numhighest == 0) eqhighest = false; }
              
            /* Increment all atoms' breakcount */
          sz = size[cli];
          litptr = clause[cli];
          for(j = 0; j < sz; j++)
          {
            lit = *(litptr++);
            breakcost[abs(lit)] += abs(cost[cli]);
          }
        }
      }
        // Pos. clause
      else
      {
        if (--numtruelit[cli] == 0)
        {
          falseclause[numfalse] = cli;
          wherefalse[cli] = numfalse;
          numfalse++;
          costofnumfalse += cost[cli];
            // Decrement toflip's breakcost
          breakcost[toflip] -= cost[cli];
          if (cost[cli] == highestcost)
              { eqhighest = 1; numhighest++; }
        }
        else if (numtruelit[cli] == 1)
        {
          // Find the lit in this clause that makes it true, and inc its breakcount
          sz = size[cli];
          litptr = clause[cli];
          for (j = 0; j < sz; j++)
          {
            lit = *(litptr++);
            if((lit > 0) == atom[abs(lit)])
            {
              breakcost[abs(lit)] += cost[cli];
              break;
            }
          }
        }    
      }
    }
    
    numocc = numoccurence[numatom+toenforce];
    occptr = occurence[numatom+toenforce];
    
    for(i = 0; i < numocc; i++)
    {
      cli = *(occptr++);

        // Neg. clause
      if (cost[cli] < 0)
      {
          // Became satisfied: increment num. of false clauses
        if (++numtruelit[cli] == 1)
        {
          wherefalse[cli] = numfalse;
          falseclause[numfalse] = cli;
          ++numfalse;
          costofnumfalse += abs(cost[cli]);
          if (highestcost == abs(cost[cli])) {eqhighest = true; numhighest++;}

            // Decrement all atoms' breakcount
          sz = size[cli];
          litptr = clause[cli];
          for(j = 0; j < sz; j++)
          {
            lit = *(litptr++);
            breakcost[abs(lit)] -= abs(cost[cli]);
          }
        }
      }
        // Pos. clause
      else
      {    
        if (++numtruelit[cli] == 1)
        {
          numfalse--;
          costofnumfalse -= cost[cli];
          falseclause[wherefalse[cli]] = falseclause[numfalse];
          wherefalse[falseclause[numfalse]] = wherefalse[cli];
            // Increment toflip's breakcount
          breakcost[toflip] += cost[cli];
          if (cost[cli] == highestcost)
            { numhighest--; if (numhighest==0) eqhighest = 0; }
        }
        else if (numtruelit[cli] == 2)
        {
            // Find the lit in this clause other than toflip that makes it true,
            // and decrement its breakcount
          sz = size[cli];
          litptr = clause[cli];
          for (j = 0; j < sz; j++)
          {
            lit = *(litptr++);
            if( ((lit > 0) == atom[abs(lit)]) &&
                (toflip != abs(lit)) )
            {
              breakcost[abs(lit)] -= cost[cli];
              break;
            }
          }
        }
      }
    }
  }

  int pickrandom(int *numbreak, int clausesize, int tofix)
    /* returns a random number */
  {
    return(random()%clausesize);
  }

  int pickproductsum(int *numbreak, int clausesize, int tofix)
    /* weights each possibility by the */
    /* product of the product and sum of everything else */
  {
    int i;                             /* a loop counter */
    int weight[MAXLENGTH];             /* weights of each possibility */
    int tochange;                      /* value to return */
    int totalproduct = 1;              /* product of all numbreaks */
    int totalsum = 0;                  /* sum of all numbreaks */

    if(clausesize == 1)
      return(0);

    if((tochange = pickzero(numbreak,clausesize)) != NOVALUE)
      return(tochange);

    for(i = 0; i < clausesize; i++)
    {
      totalproduct *= numbreak[i];
      totalsum += numbreak[i];
    }

    for(i = 0; i < clausesize; i++)
    {
      weight[i] = (totalproduct/numbreak[i])*
                  (totalsum-numbreak[i]);
    }

    return(pickweight(weight,clausesize));
  }

  int pickreciprocal(int *numbreak,int clausesize, int tofix)
    /* weights each possibility by its reciprocal*/
  {
    int i;                             /* a loop counter */
    int weight[MAXLENGTH];             /* weights of each possibility */
    int tochange;                      /* value to return */
    int totalproduct = 1;              /* product of all numbreaks */

    if(clausesize == 1)
      return(0);

    if((tochange = pickzero(numbreak,clausesize)) != NOVALUE)
      return(tochange);

    for(i = 0; i < clausesize; i++)
      totalproduct *= numbreak[i];

    for(i = 0; i < clausesize; i++)
      weight[i] = (totalproduct/numbreak[i]);

    return(pickweight(weight,clausesize));
  }

  int pickadditive(int *numbreak,int clausesize, int tofix)
    /* weights each possibility by the sum of the others */
  {
    int i;                             /* a loop counter */
    int weight[MAXLENGTH];             /* weights of each possibility */
    int tochange;                      /* value to return */
    int totalsum = 0;                  /* sum of all numbreaks */

    if(clausesize == 1)
      return(0);

    if((tochange = pickzero(numbreak,clausesize)) != NOVALUE)
      return(tochange);

    for(i = 0; i < clausesize; i++)
      totalsum += numbreak[i];

    for(i = 0; i < clausesize; i++)
      weight[i] = (totalsum-numbreak[i]);

    return(pickweight(weight,clausesize));
  }

  int pickbest(int *numbreak, int clausesize, int tofix)
  {
    int i;          /* a loop counter */
    int best[MAXLENGTH];    /* best possibility so far */
    int numbest;        /* how many are tied for best */
    int bestvalue;      /* best value so far */
    register int var;

    Array<int> canNotFlip;
      // If var in candidateBlockedVars is chosen, then
      // corresponding var in othersToFlip is flipped as well
    Array<int> candidateBlockedVars;
    Array<int> othersToFlip;
 
    numbest = 0;
    bestvalue = BIG;

    for (i = 0; i < clausesize; i++)
    {
      var = abs(clause[tofix][i]);
      int otherToFlip = NOVALUE;
      int blockIdx = getBlock(var - 1);
      if (blockIdx == -1) // Atom not in a block
        continue;
      else // Atom is in a block
      {
          // If evidence atom exists or in block of size 1, then can not flip
        if ((*blockEvidence_)[blockIdx] || (*blocks_)[blockIdx].size() == 1)
        {
          numbreak[i] = INT_MAX;
          canNotFlip.append(var);
        }
        else
        {
          numbreak[i] = calculateNumbreak(var, otherToFlip);
          candidateBlockedVars.append(var);
          othersToFlip.append(otherToFlip);
        }
      }

      if (numbreak[i] <= bestvalue)
      { // Tied or better than previous best
        if (numbreak[i] < bestvalue) numbest = 0;
        bestvalue = numbreak[i];
        best[numbest++] = i;
      }
    }

      // Choose one of the best at random
      // (default if none of the following 2 cases occur)
    int toflip = (numbest == 0) ? random()%clausesize : best[random()%numbest];

      // 1. If at least 1 clause is broken by best,
      // then with prob. choose a random atom
    if (bestvalue > 0 && (random()%denominator < numerator))
    {
        // allow random breaks of hard clauses
      if (!hard || abs(cost[tofix]) >= highestcost)
        toflip = random()%clausesize;
        
        // only do a random walk breaking non-hard clauses
      numbest = 0;
      for (i = 0; i < clausesize; i++)
      {
        if (numbreak[i] < highestcost)
        {
          best[numbest++] = i;
        }
      }
      if (numbest==0) toflip = random()%clausesize;
    }
      // 2. Exactly one best atom
    else if (numbest == 1)
      toflip = best[0];

      // If atom can not be flipped, then null flip
    if (canNotFlip.contains(toflip)) toflip = NOVALUE;
    else
    { // If toflip is in block, then set other to flip
      int idx = candidateBlockedVars.find(abs(clause[tofix][toflip]));
      if (idx >= 0)
      {
        inBlock = true;
        flipInBlock = othersToFlip[idx];
      }
    }
    return toflip;
  }


  int picktabu(int *numbreak,int clausesize, int tofix)
  {
    int i;          /* a loop counter */
    int best[MAXLENGTH];    /* best possibility so far */
    int numbest;        /* how many are tied for best */
    int bestvalue;      /* best value so far */

    numbest = 0;
    bestvalue = BIG;

    if (numerator>0 && (random()%denominator < numerator))
    {
      for (i = 0; i < clausesize; i++)
      {
        if ((tabu_length < numflip - changed[abs(clause[tofix][i])]) ||
            numbreak[i]==0)
        {
          if (numbreak[i]==0) numbest=0;
          best[numbest++] = i;
        }
      }
    }
    else 
    {
      for (i = 0; i < clausesize; i++)
      {
        if (numbreak[i]<=bestvalue && 
            ((tabu_length < numflip - changed[abs(clause[tofix][i])]) ||
            numbreak[i]==0))
        {
          if (numbreak[i]<bestvalue) numbest=0;
          bestvalue = numbreak[i];
          best[numbest++] = i;
        }
      }
    }
    
    if (numbest == 0) return NOVALUE;
    if (numbest == 1) return best[0];
    return (best[random()%numbest]);
  }

  int pickexponential(int *numbreak,int clausesize, int tofix)
  {
    int i;                             /* a loop counter */
    int weight[MAXLENGTH];             /* weights of each possibility */
    int tochange;                      /* value to return */

    if(clausesize == 1)
      return(0);

    if((tochange = pickzero(numbreak,clausesize)) != NOVALUE)
      return(tochange);

    for(i = 0; i < clausesize; i++)
      weight[i] = 2*2*2*2*2*2*2*2*2*2*2*2*2*2;

    for(i = 0; i < clausesize; i++)
    {
      weight[i] >>= numbreak[i]; /* weight[i] = weight[i]/(2^numbreak[i]) */
    }

    return(pickweight(weight,clausesize));
  }

  int pickzero(int *numbreak,int clausesize)
  {
    int i;                /* loop counter */
    int numzero = 0;      /* number of zeros so far */
    int select;           /* random number */

    for(i = 0; i < clausesize; i++)
    {
      if(numbreak[i] == 0)
      numzero++;
    }

    if(numzero == 0)
      return(NOVALUE);

    select = random()%numzero;

    for(i = 0; select >= 0; i++)
    {
      if(numbreak[i] == 0)
        select--;
    }

    return(i-1);
  }

  int pickweight(int *weight, int clausesize)
  {
    int i;                /* loop counter */
    int total = 0;        /* sum of weights */
    int select;           /* random number less than total */
    int subtotal = 0;

    for(i = 0; i < clausesize; i++)
      total += weight[i];

    if(total == 0)
      return(random()%clausesize);

    select = random()%total;
    for(i = 0; subtotal <= select; i++)
      subtotal += weight[i];
    return(i-1);
  }

  void countlowunsatcost(int * unsatcostptr, int * worstcostptr)
  {
    int i, j, lit, sign, unsatcost, worstcost;
    bool bad;

    unsatcost = 0;
    worstcost = 0;
    
    for (i = 0; i < numclause; i++)
    {
        // Neg. clause
      if (cost[i] < 0)
        bad = false;
        // Pos. clause
      else
        bad = true;

      for (j = 0; j < size[i]; j++)
      {
        lit = clause[i][j];
        sign = lit > 0 ? 1 : 0;
        if ( lowatom[abs(lit)] == sign )
        {
            // Neg. clause
          if (cost[i] < 0)
            bad = true;
            // Pos. clause
          else
            bad = false;
          break;
        }
      }
      if (bad)
      {
        unsatcost += abs(cost[i]);
        if (abs(cost[i]) > worstcost) worstcost = abs(cost[i]);
      }
    }
    
    * unsatcostptr = unsatcost;
    * worstcostptr = worstcost;
    return;
  }


  double elapsed_seconds(void)
  {
    double answer;

    static struct rusage prog_rusage;
    static long prev_rusage_seconds = 0;
    static long prev_rusage_micro_seconds = 0;

    getrusage(0, &prog_rusage);
    answer = (double)(prog_rusage.ru_utime.tv_sec - prev_rusage_seconds) +
             ((double)(prog_rusage.ru_utime.tv_usec - prev_rusage_micro_seconds)) / 
             1000000.0 ;
    prev_rusage_seconds = prog_rusage.ru_utime.tv_sec ;
    prev_rusage_micro_seconds = prog_rusage.ru_utime.tv_usec ;
    return answer;
  }

    /* extract the true atoms in the best assignment */
  void extract_low_assign(Array<int>& trueAtomArray)
  {
    trueAtomArray.clear();
    for (int i = 1; i <= numatom; i++)
    {
      if (lowatom[i] > 0)
      {
        trueAtomArray.append(i);
      }
    }
  }

  void print_low_assign(long int lowbad)
  {
    int i, j;

    printf("Begin assign with lowest # bad = %li (atoms assigned true)\n", lowbad);
    j = 1;
    for (i = 1; i <= numatom; i++)
    {
      if (lowatom[i] > 0)
      {
        printf(" %d", i);
        if (j++ % 10 == 0) printf("\n");
      }
    }
    if ((j-1) % 10 != 0) printf("\n");
    printf("End assign\n");
  }

  void save_low_assign(void)
  {
    int i;

    for (i = 1; i <= numatom; i++)
      lowatom[i] = atom[i];
  }

  int selecthigh(int high)
  {
    int counter = 0;
    int i = 0;
    while ((i < numfalse) && (counter < high))
    {
      if (abs(cost[falseclause[i]]) == highestcost)
        counter++;
      i++;
    }
    /* fprintf(stderr, "The cost of the selected clause %i\n", cost[false[i-1]]);*/
    return(falseclause[i-1]);
  }
  
    // Sets the truth values of all atoms in the
    // block blockIdx except for the one with index atomIdx
  void setOthersInBlockToFalse(const int& atomIdx,
                               const int& blockIdx)
  {
    Array<int> block = (*blocks_)[blockIdx];
    for (int i = 0; i < block.size(); i++)
    {
      if (i != atomIdx)
        atom[block[i] + 1] = 0;
    }
  }
  
    // returns the index of the block which the atom
    // with index atomIdx is in. If not in any, returns -1
  int getBlock(const int& atomIdx)
  {
    for (int i = 0; i < blocks_->size(); i++)
    {
      int blockIdx = (*blocks_)[i].find(atomIdx);
      if (blockIdx >= 0)
        return i;
    }
    return -1;
  }
  
    // Calculates the breakcost by flipping atom
    // and stores idx of other atom to flip in block in flipInBlock
  int calculateNumbreak(const int& atomIdx, int& otherToFlip)
  {
    int blockIdx = getBlock(atomIdx - 1);
    assert(blockIdx >= 0);

      //Dealing with atom in a block
    Array<int> block = (*blocks_)[blockIdx];
    int chosen = -1;
    
      // 0->1: choose atom in block which is already 1
    if (atom[atomIdx] == 0)
    {
      for (int i = 0; i < block.size(); i++)
      {
        if (atom[block[i] + 1] == 1)
        {
          chosen = i;
          break;
        }
      }
    }
      // 1->0: choose to flip the other randomly
      // TODO: choose to flip the other with best breakcost
    else
    {
      bool ok = false;
      while(!ok)
      {
        chosen = random() % block.size();
        if (block[chosen] + 1 != atomIdx)
          ok = true;
      }
    }
    
    assert(chosen >= 0);
    otherToFlip = block[chosen] + 1;
    return breakcost[atomIdx] + breakcost[otherToFlip];    
  }
  
 private:
          
    /* Atoms start at 1 */
    /* Not a is recorded as -1 * a */
    /* One dimensional arrays are statically allocated. */
    /* Two dimensional arrays are dynamically allocated in */
    /* the second dimension only.  */
    /* (Arrays are not entirely dynamically allocated, because */
    /* doing so slows execution by 25% on SGI workstations.) */

  int numatom;
  int numclause;
  int numliterals;

  int ** clause;    /* clauses to be satisfied */
                    /* indexed as clause[clause_num][literal_num] */
  int * cost;       /* cost of each clause */
  int * size;         /* length of each clause */
  int * falseclause;          /* clauses which are false */
  int * lowfalse;

  int * wherefalse;       /* where each clause is listed in false */
  int * numtruelit;       /* number of true literals in each clause */

  int **occurence;    /* where each literal occurs */
                    /* indexed as occurence[literal+numatom][occurence_num] */

  int *numoccurence;  /* number of times each literal occurs */


  int *atom;      /* value of each atom */ 
  int *lowatom;

  int *changed;       /* step at which atom was last flipped */

  int *breakcost; /* the cost of clauses that introduces if var is flipped */

  int numfalse;           /* number of false clauses */
  int costofnumfalse;     /* cost associated with the number of false clauses */

  bool costexpected;   /*indicate whether cost is expected from the input*/
  bool abort_flag;

  int heuristic;          /* heuristic to be used */
  int numerator;          /* make random flip with numerator/denominator frequency */
  int denominator;        
  int tabu_length;        /* length of tabu list */

  long int numflip;       /* number of changes so far */
  long int numnullflip;       /*  number of times a clause was picked, but no  */
                /*  variable from it was flipped  */

  int highestcost;               /*The highest cost of a clause violation*/
  int eqhighest;  /*Is there a clause violated with the highest cost*/
  int numhighest;      /*Number of violated clauses with the highest cost*/

  bool hard; /* if true never break a highest cost clause */

  Array<int *> storePtrArray;
  
    // For block inference
  Array<Array<int> >* blocks_;
  Array<bool >* blockEvidence_;
    
    // Which other atom to flip in block
  bool inBlock;
  int flipInBlock;
    
};


#endif
