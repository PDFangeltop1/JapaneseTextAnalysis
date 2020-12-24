#include <iostream>
#include <vector>
#include <stdio.h>
#include "cabocha.h"
using namespace std;

int main()
{
  vector<string> Sentence;
  CaboCha::Parser *parser = CaboCha::createParser("");
  char p[] = "検察は２４日、大韓航空の客室乗務本部の常務についても、前副社長をかばう証言をするよう乗務員らに強要した疑いなどで逮捕状を請求したほか、韓国政府が行った調査を巡り、大韓航空側に事前に情報を漏らした疑いで韓国国土交通省の調査官についても捜査を進めています.";
  const CaboCha::Tree *tree = parser->parse(p);
  size_t cid = 0;
  for(size_t i = 0; i < tree->token_size(); i++)
    {
      const CaboCha::Token *token = tree->token(i);
      string anwser = "";
      if(token->chunk){
	//cout<<"* "<<cid++<<' '<<token->chunk->link<<"D "<<token->chunk->head_pos<<"/"
	//  <<token->chunk->func_pos<<endl;
	char Tag[5] = "*";
	char buffer[100];
	sprintf(buffer,"%s %d %d",Tag,cid++,token->chunk->link);
	anwser  += buffer;
	Sentence.push_back(anwser);
	anwser  = "";
      }
      anwser += token->surface;
      anwser += '\t';
      anwser += token->feature;
      //cout<<token->surface<<'\t'<<token->feature;
	if(token->ne){
	  // cout<<'\t'<<token->ne;
	  anwser += '\t';
	  anwser += token->ne;
	}
	Sentence.push_back(anwser);
	//cout<<endl;
    }
  for(vector<string>::iterator it = Sentence.begin(); it != Sentence.end(); it ++)
    {
      cout<<*it<<endl;
    }
  return 0;
}

