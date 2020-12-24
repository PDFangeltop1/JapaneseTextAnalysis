#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <fstream>
#include "JapaneseChunk.h"
#include "JapaneseTermExtract.h"
#include "cabocha.h"

using namespace std;
////#include <stdio.h>
////
////int main ()
////{
////  char buffer [50];
////  int n, a=5, b=3;
////  n=sprintf (buffer, "%d plus %d is %d", a, b, a+b);
////  printf ("[%s] is a string %d chars long\n",buffer,n);
////  return 0;
////}
//void f(string F, vector<string>& Sentence)
//{
//    CaboCha::Parser *parser = CaboCha::createParser("");
//    const char *p = F.c_str();
//    const CaboCha::Tree *tree = parser->parse(p);
//    size_t cid = 0;
//    for(size_t i = 0; i < tree->token_size(); i++)
//    {
//        const CaboCha::Token *token = tree->token(i);
//        if(token->chunk)
//        {
//            char Tag[5] = "*";
//            char buffer[100];
//            int n;
//            sprintf(buffer,"%s %d %d",Tag,cid++,token->chunk->link);
//
//
//        }
//    }
//}
int main()
{
    JapaneseTermExtract R;
    ifstream F_in("cabocha_out");
    ofstream Fout;
    Fout.open("noun.txt");
    string line;
    vector<string> List;
    vector<string> Sentence;
    int count = 0;
    while(getline(F_in,line))
    {
        R.split(line,' ',List);
        if(*List.begin() == "EOS")
        {
            AnalyseInput(Sentence,Fout);
            Sentence.clear();
        }
        else Sentence.push_back(line);
        List.clear();
    }
    return 0;
}
