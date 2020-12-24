#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <cmath>
#include <algorithm>


using namespace std;

class JapanesePackage{
 public:
  void split(string , char , vector<string>& );
  string join(string delimiter, vector<string>& A);
  bool is_Digit(string Noun);
  bool is_Jcomma(string Noun);
  bool is_Comma(string Noun);
  virtual ~JapanesePackage(){};
  virtual bool Recognize(string st,map<string, double>*NImp)=0;
};
