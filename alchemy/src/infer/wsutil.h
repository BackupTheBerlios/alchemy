
#ifndef _WALKSAT_UTIL_H_
#define _WALKSAT_UTIL_H_

#define BLOCKSIZE  2000000	// size of block to malloc each time 

#define BIG 100000000

#define MAXLENGTH 1000          /* maximum number of literals which can be in any clause */

#define RANDOM 0
#define PRODUCTSUM 1
#define RECIPROCAL 2
#define ADDITIVE 3
#define BEST 4
#define EXPONENTIAL 5
#define TABU 6

#define NOVALUE -1

#define INIT_PARTIAL 1


class WSUtil
{
 public:
  static int makeNegatedLiteral(const int& lit)  { return lit | 0x80000000; }
  static int isLiteralNegated(const int& lit)    { return lit & 0x80000000; }
  static int getLiteral(const int& lit)          { return lit & 0x7FFFFFFF; }  
    //static int negationOf(const int& lit)          { return lit ^ 0x80000000; }

};

#endif

