#ifndef HASHSTRING_H_JUL_10_2004
#define HASHSTRING_H_JUL_10_2004

#include <string>
#include <ext/hash_set>
#include <ext/hash_map>
using namespace __gnu_cxx;
#include "hasharray.h"

class HashString
{
 public:
  size_t operator()(string const& str) const
  {
    return hash<char const *>()(str.c_str());
  }
};

class EqualString
{
 public:
  bool operator()(string const & s1, string const & s2) const
  {
    return (s1.compare(s2)==0);
  }
};

typedef hash_set<string, HashString, EqualString> StringSet;
typedef hash_map<string, int, HashString, EqualString> StringToIntMap;
typedef HashArray<string, HashString, EqualString> StringHashArray;


#endif
