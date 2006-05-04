#ifndef UTIL_H_OCT_17_2005
#define UTIL_H_OCT_17_2005

#include <string>
using namespace std;


class Util
{
 public:
	
  static bool substr(const string& pred, unsigned int& cur, string& outstr,
                     const char* const & delimiter)
  {
    outstr.clear();
    while (cur < pred.length() && isspace(pred.at(cur))) cur++;
    string::size_type dlt = pred.find(delimiter, cur);
    if (dlt == string::npos) return false;
    unsigned int cur2 = dlt-1;
    while (cur2 >= 0 && isspace(pred.at(cur2))) cur2--;
    if (cur2 < cur) return false;
    outstr = pred.substr(cur, cur2+1-cur);
    cur = dlt+1;
    return true;
  }


  static string trim(const string& str)
  {
    int lt = 0; 
    while (isspace(str.at(lt))) lt++;
    int rt = str.length()-1;
    while (isspace(str.at(rt))) rt--;
    return str.substr(lt, rt+1-lt);
  }
  
  /* this function converts an integer to a string */
  static string intToString(int i) {
	 ostringstream myStream;
	 myStream<<i;
	 return myStream.str();
  }

  /* this function converts a double to a string */
  static string doubleToString(double i) {
	 ostringstream myStream;
	 myStream<<i;
	 return myStream.str();
  }
  
};

#endif
