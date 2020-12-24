#include "JapaneseTermExtract.h"
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>
#include <cmath>
using namespace std;
struct node{
  int a,b;
};
JapaneseTermExtract::JapaneseTermExtract(){
//  tagger = MeCab::createTagger("");
  LR = 1;
}
bool JapaneseTermExtract::is_Digit(string Noun){
  if(Noun == "1" ||
     Noun == "2" ||
     Noun == "3" ||
     Noun == "4" ||
     Noun == "5" ||
     Noun == "6" ||
     Noun == "7" ||
     Noun == "8" ||
     Noun == "9" ||
     Noun == "0" ||
     Noun == "０" ||
     Noun == "１" ||
     Noun == "２" ||
     Noun == "３" ||
     Noun == "４" ||
     Noun == "５" ||
     Noun == "６" ||
     Noun == "７" ||
     Noun == "８" ||
     Noun == "９" )return true;
  return false;
}
bool JapaneseTermExtract::is_Jcomma(string Noun){
      if(Noun == "【" || Noun == "】" ||
            Noun == "。" || Noun == "，" ||
            Noun == "．" || Noun == "：" ||
            Noun == "？" || Noun == "，" ||
            Noun == "―" || Noun == "‐" ||
            Noun == "／" || Noun == "℃" ||
            Noun == "（" || Noun == "）" ||
            Noun == "「" || Noun == "」" ||
            Noun == "±" || Noun == "°" ||
            Noun == "、" || Noun == "％" )return true;
    return false;
}
bool JapaneseTermExtract::is_Comma(string Noun){
   // if(is_Digit(Noun))return true;
    if(is_Jcomma(Noun))return true;
    if(Noun == ""|| Noun == "."||Noun == "(" || Noun == ")" || Noun == "\"" || Noun == "%" || Noun == "。"
            ||  Noun == "#" || Noun == "," || Noun == ":" || Noun == ";"    || Noun == "\'"|| Noun == "*"
            || Noun == "-" || Noun == "&" || Noun == "\\" || Noun == "/")return true;
    return false;
}
void JapaneseTermExtract::split(string st,char delimiter, vector<string> &A){
    string :: size_type found = st.find(delimiter);
    string :: size_type pre = 0;
    while(found != std::string::npos)
    {
        if(found > pre)
        {
            A.push_back(st.substr(pre,found-pre));
            pre = found + 1;
            found = st.find(delimiter,found+1);
        }
    }
    A.push_back(st.substr(pre));
}
string JapaneseTermExtract::join(string delimiter, vector<string> &A){
    string anwser = "";
    vector<string>::iterator it = A.begin();
    while(is_Comma(*it))
    {
        it++;
        if(it == A.end())break;
    }
    if(it != A.end())anwser = anwser + *(it++);
    for(; it != A.end(); it ++)
    {
        if(is_Comma(*it))continue;
        anwser = anwser + delimiter;
        anwser = anwser + *it;
    }
    return anwser;
}
void JapaneseTermExtract::Add_to_hash(vector<string> &N, map<string, int> &Hash,int must){

    if(N.size() > 0)
    {
      vector<string>::iterator it = N.begin();
      string prefix = *it;
      if(prefix == "前記" || prefix == "後記" || prefix == "上記" || prefix == "前述"
	 ||prefix == "上述"||prefix == "該当"||prefix == "後述")
	{
	  N.erase(it);
	 }
    }
    if(N.size() > 0)
      {
	vector<string>::iterator it = N.begin();
	string prefix = *it;
        if(prefix == "本" || prefix == "各" || prefix == "当"
	   ||prefix == "全" || prefix == "該")N.erase(it);
      }
    if(N.empty())return;
    string _end = N.back();
    if ( _end == "など"  || _end == "ら"   || _end == "上" || _end == "内"    || _end == "型"   || _end == "間"       ||
            _end == "中"    || _end == "毎"   || _end == "等"  || _end == " " || _end == "下" || must)
    {
        N.pop_back();
    }
    if(!N.empty())
    {
        string tmp = join(" ",N);
        if(tmp != "")Hash[tmp] ++;
    }
    N.clear();
}
bool JapaneseTermExtract::all_alphabet(string word){
    int len = word.size();
    for(int i = 0; i < len ; i ++)
    {
        string st = "";
        st += word[i];
        if((word[i] >= 'a'&& word[i] <= 'z') ||(word[i] >= 'A'&& word[i] <= 'Z') || is_Comma(st));
        else return 0;
    }
    return 1;
}
bool myFunction(pair<string,double> A, pair<string,double> B){
  return A.second > B.second;
}
void JapaneseTermExtract::modify_noun_list(map<string,double> &N_imp){
    vector<pair<string,double> > myVec(N_imp.begin(),N_imp.end());
    sort(myVec.begin(),myVec.end(),&myFunction);
    N_imp.clear();
    for(vector<pair<string,double> >::iterator it = myVec.begin() ; it != myVec.end(); it ++)
    {
        string word;
        vector<string>Noun_list;
        split(it->first,' ',Noun_list);
        word = join("",Noun_list);
        if(all_alphabet(word))continue;
        N_imp[word] = it->second;
    }
}
void JapaneseTermExtract::calc_imp_by_Hash_PP(const map<string, int> &Cmp_noun_list, map<string, double> &N_imp){
    map<string,node>Stat;  //Stat[a][A] A后面出现了多少词， Stat[b][A] A前面出现了多少词
    map<string,double>Stat_PP;  //单名词A的熵
    map<string, map<string,int> >Pre; //Pre[A][B]：对于A来说，B出现在A前面多少次
    map<string, map<string,int> >Post;//Post[B][A]： 对于B来说 A出现在B后面多少次
    //  map<string,double>N_imp; //N_imp[X]：单名词或复合名词X的重要度
    map<string,int>::const_iterator it;
    for(it = Cmp_noun_list.begin(); it != Cmp_noun_list.end(); it ++)
    {
        vector<string> Noun_list;
        split(it->first,' ',Noun_list);
        if(Noun_list.size() > 1)
        {
            for(vector<string>::iterator i = Noun_list.begin(); i != Noun_list.end(); i++)
            {
                if(*i == Noun_list.back())continue;
                Stat[*i].a += it->second;
                Stat[*(i+1)].b += it->second;
                Post[*i][*(i+1)] ++;   //?
                Pre[*(i+1)][*i] ++;  //?
                for(map<string,node>::iterator j = Stat.begin(); j != Stat.end(); j++)
                {
                    double entropy = 0;
                    double work;
                    if(Stat[j->first].a != 0)
                    {
                        map<string,int>::iterator postJ;
                        for(postJ = Post[j->first].begin(); postJ != Post[j->first].end(); postJ++)
                        {
                            if(postJ->second != 0)
                            {
                                work = postJ->second*1.0/(Stat[j->first].a + 1);
                                entropy = entropy - work*log(work);
                            }
                        }
                        //930s 89s
                    }
                    if(Stat[j->first].b != 0)
                    {
                        map<string,int>::iterator preJ;
                        for(preJ = Pre[j->first].begin(); preJ != Pre[j->first].end(); preJ++)
                        {
                            if(preJ->second != 0)
                            {
                                work = preJ->second*1.0/(Stat[j->first].b + 1);
                                entropy = entropy - work*log(work);
                            }
                        }
                    }
                    Stat_PP[j->first] = entropy;
                }
            }
        }
    }
    double imp = 0;
    int count = 0;
    for(it = Cmp_noun_list.begin(); it != Cmp_noun_list.end(); it ++)
    {
        vector<string> Noun_list;
        split(it->first,' ',Noun_list);
        for(vector<string>::iterator i = Noun_list.begin(); i != Noun_list.end(); i++)
        {
            imp += Stat_PP[*i];
            count ++;
        }
        if(count == 0)count ++;
        imp = imp/(2*count);
        imp += log(it->second+1);
        imp = imp/log(2.0);
        N_imp[it->first] = imp;
        imp = 0;
        count = 0;
    }
    modify_noun_list(N_imp);
}

void JapaneseTermExtract::calc_imp_by_Hash(const map<string,int> &Cmp_noun_list,const int &LR, map<string,double> &N_imp)
{
    map<string,int> Comb;
    map<string,int>Stat[2];
    map<string,int>::const_iterator it;
    for(it = Cmp_noun_list.begin(); it != Cmp_noun_list.end(); it ++)
    {
        vector<string> Noun_list;
        split(it->first,' ',Noun_list);
        if(Noun_list.size() > 1)
        {
            for(vector<string>::iterator i = Noun_list.begin(); i!= Noun_list.end(); i ++)
            {
                if(*i == Noun_list.back())continue;
                string Combine_Pre_Post = *i + *(i+1);
                int first_comb = 0;
                if(Comb[Combine_Pre_Post] == 0)
                {
                    Comb[Combine_Pre_Post] = 1;
                    first_comb = 1;
                }
                if(LR == 1)
                {
                    Stat[0][*i] += it->second;
                    Stat[1][*(i+1)] += it->second;
                }
                else if(LR == 2 && first_comb == 1)
                {
                    Stat[0][*i] ++;
                    Stat[1][*(i+1)] ++;
                }
            }
        }
    }
    double tmp = 1.0;
    int count = 0,pre, post;
    for(it = Cmp_noun_list.begin(); it != Cmp_noun_list.end(); it ++)
    {
        vector<string> Noun_list;
        split(it->first,' ',Noun_list);
        for(vector<string>::iterator i = Noun_list.begin(); i != Noun_list.end(); i++)
        {
            if(is_Comma(*i))continue;
            pre = Stat[0][*i];
            post = Stat[1][*i];
            tmp = tmp * (pre+1)*(post+1);
            count ++;
        }
        if(count == 0)count ++;
        tmp = pow(tmp,1.0/(2*count));
        tmp = tmp*it->second;
        N_imp[it->first] = tmp;
        count = 0;
        tmp = 1;
    }
    modify_noun_list(N_imp);
}
bool JapaneseTermExtract::Recognize(string input, map<string, double> &N_imp){
//    const char *st = input.c_str();
//    const char *result = tagger->parse(st);
//    string line = result;
    string line;
    vector<string>allInput;
    vector<string>Noun_Complex;
    map<string,int>Cmp_noun_list;
    int must = 0;

    split(line,'\n',allInput);
    for(vector<string>:: iterator ii = allInput.begin(); ii != allInput.end(); ii ++)
    {
        line = *ii;
        vector<string> Split_blank;
        split(line,'\t',Split_blank);
        vector<string>:: iterator i = Split_blank.begin();
        string Noun = *i;
        if(Noun == "EOS" || Noun == "")continue;
        string _remain = *(++i);
        vector<string> Split_comma;
        split(_remain,',',Split_comma);
        i = Split_comma.begin();
        string Part_of_speech = *i;
        string c1 = *(++i);
        string c2 = *(++i);

        if((Part_of_speech == "名詞" && c1 == "一般")                       ||
                (Part_of_speech == "名詞" && c1 == "サ変接続")                   ||
                (Part_of_speech == "名詞" && c1 == "接尾" && c2 == "一般")     ||
                (Part_of_speech == "名詞" && c1 == "接尾" && c2 == "サ変接続") ||
                (Part_of_speech == "名詞" && c1 == "固有名詞")                   ||
                (Part_of_speech == "記号" && c1 == "アルファベット")
          )
        {
            Noun_Complex.push_back(Noun);
            must = 0;
            continue;
        }
        else if((Part_of_speech == "名詞" && c1 == "形容動詞語幹")||(Part_of_speech == "名詞" && c1 == "ナイ形容詞語幹"))
        {
            Noun_Complex.push_back(Noun);
            must = 1;
            continue;
        }
        else if(Part_of_speech == "名詞" && c1 == "接尾" && c2 == "形容動詞語幹")
        {
            Noun_Complex.push_back(Noun);
            must = 1;
            continue;
        }
        else if(Part_of_speech == "動詞")Noun_Complex.clear();
        else
        {
            if(must == 0 && !Noun_Complex.empty())
            {
                Add_to_hash(Noun_Complex, Cmp_noun_list,must);
            }
        }
        if(must)Noun_Complex.clear();
        must = 0;
    }
    if(LR == 1)calc_imp_by_Hash(Cmp_noun_list, LR, N_imp);
    else calc_imp_by_Hash_PP(Cmp_noun_list,N_imp);
    return 1;
}
