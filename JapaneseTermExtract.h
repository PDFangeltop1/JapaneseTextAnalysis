//#include "mecab.h"
#include <map>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

class JapaneseTermExtract{
 public:
   bool Recognize(string st, map<string,double> &N_imp);
   void split(string st, char delimiter, vector<string> &A);
   string join(string delimiter, vector<string> &A);
   JapaneseTermExtract();
 private:
  int LR;
 // MeCab::Tagger  *tagger;
  bool is_Digit(string Noun);
  bool is_Jcomma(string Noun);
  bool is_Comma(string Noun);
  void Add_to_hash(vector<string> &N, map<string,int> &Hash, int must);
  bool all_alphabet(string word);
  void modify_noun_list(map<string, double> &N_imp);
  void calc_imp_by_Hash_PP(const map<string,int> &Cmp_noun_list, map<string,double> &N_imp);
  void calc_imp_by_Hash(const map<string,int> &Cmp_noun_list,const int &LR, map<string,double> & N_imp);
};
