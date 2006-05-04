#ifndef INTERNALS_H_MAR_10_2006
#define INTERNALS_H_MAR_10_2006

#include <iostream>
#include <string>
#include <sstream>
using namespace std;

class Internals
{

  public:

   static int succ(int a) { return a+1; }

   static int plus(int a, int b) { return a+b; }

   static int minus(int a, int b) { return a-b; }

   static int times(int a, int b) { return a*b; }

   static int dividedBy(int a, int b) { return a/b; }

   static int mod(int a, int b) { return a%b; }

   static string concat(string a, string b)
   {
     return a.append(b);
   }

   static bool greaterThan(int a, int b) { return a > b; }

   static bool lessThan(int a, int b) { return a < b; }

   static bool greaterThanEq(int a, int b) { return a >= b; }

   static bool lessThanEq(int a, int b) { return a <= b; }

   static bool substr(string a, string b)
   {
     if (b.find(a, 0) == string::npos) return false;
     else return true;
   }

   // helper functions
   static bool stringToInt(const string &s, int &i)
   {
   	 istringstream iss(s);
  
  	 if (iss>>i)
       return true;
     else
       return false;
   }

   static string intToString(int i)
   {
     ostringstream oss;
     oss << i << flush;  
     return(oss.str());
   }

};

#endif
