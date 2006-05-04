#ifndef HASHINT_H_NOV_10_2005
#define HASHINT_H_NOV_10_2005

using namespace __gnu_cxx;
#include "hasharray.h"

class HashInt
{
 public:
  size_t operator()(const int& i) const { return hash<int>()(i); }
};

class EqualInt
{
 public:
  bool operator()(const int& i1, const int& i2) const { return (i1 == i2); }
};

typedef HashArray<int, HashInt, EqualInt> IntHashArray;


#endif
