#ifndef MAXWALKSATPARAMS_H_MAR_21_2005
#define MAXWALKSATPARAMS_H_MAR_21_2005

struct MaxWalksatParams
{
  int maxSteps;
  int tries;
  int targetWt;
  bool hard;
  bool lazyGnded;
  bool lazyNoApprox;
  //int seed;
};

#endif
