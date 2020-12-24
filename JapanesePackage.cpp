#include "JapanesePackage.h"
using namespace std;
////////////////////////////////////////JapanesePackage///////////////////////////////////////////////////////////
bool JapanesePackage::is_Digit(string Noun)
{
    if(Noun == "1" ||     Noun == "2" ||     Noun == "3" ||     Noun == "4" ||
     Noun == "5" ||     Noun == "6" ||     Noun == "7" ||     Noun == "8" ||
     Noun == "9" ||     Noun == "0" ||     Noun == "０" ||     Noun == "１" ||
     Noun == "２" ||     Noun == "３" ||     Noun == "４" ||     Noun == "５" ||
     Noun == "６" ||     Noun == "７" ||     Noun == "８" ||     Noun == "９" )return true;
  return false;
}
bool JapanesePackage::is_Jcomma(string Noun)
{
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
bool JapanesePackage::is_Comma(string Noun){
    //if(JapanesePackage::is_Digit(Noun))return true;
    if(JapanesePackage::is_Jcomma(Noun))return true;
    if(Noun == ""|| Noun == "."||Noun == "(" || Noun == ")"
            ||  Noun == "#" || Noun == "," || Noun == ":" || Noun == ";"
            || Noun == "-" || Noun == "&" || Noun == "\\" || Noun == "/")return true;
    return false;
}
void JapanesePackage::split(string st, char delimiter, vector<string>& A)
{
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


string JapanesePackage::join(string delimiter, vector<string>&A)
{
    string anwser = "";
    vector<string>::iterator it = A.begin();
    while(JapanesePackage::is_Comma(*it))
    {
        it++;
        if(it == A.end())break;
    }
    if(it != A.end())anwser = anwser + *(it++);
    for(; it != A.end(); it ++)
    {
        if(JapanesePackage::is_Comma(*it))continue;
        anwser = anwser + delimiter;
        anwser = anwser + *it;
    }
    return anwser;
}



