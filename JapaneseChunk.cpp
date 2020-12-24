#include "JapaneseChunk.h"
#include "JapaneseTermExtract.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;
JapaneseChunk::JapaneseChunk()
{
    Srcs = new vector<int>;
    Num = -1;
    Dst = -1;
    flag = false;
}
//JapaneseChunk::JapaneseChunk(vector<int> *X)
//{
//    Srcs = X;
//}
//JapaneseChunk::JapaneseChunk(vector<Morph> x,int n,int d,vector<int> *S)
//{
//    M = x;
//    Num = n;
//    Dst = d;
//    *Srcs = *S;
//}

bool is_Jcomma(string Noun)
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
bool is_Comma(string Noun)
{
    // if(is_Digit(Noun))return true;
    if(is_Jcomma(Noun))return true;
    if(Noun == ""|| Noun == "."||Noun == "(" || Noun == ")" || Noun == "\"" || Noun == "%" || Noun == "。"
            ||  Noun == "#" || Noun == "," || Noun == ":" || Noun == ";"    || Noun == "\'"|| Noun == "*"
            || Noun == "-" || Noun == "&" || Noun == "\\" || Noun == "/")return true;
    return false;
}
JapaneseChunk::~JapaneseChunk()
{
    M.clear();
    delete Srcs;
}
JapaneseChunk& JapaneseChunk::operator=(JapaneseChunk& x)
{
    M = x.getMorph();
    Num = x.getNum();
    Dst  = x.getDst();
    flag = x.getFlag();
    *Srcs = vector<int>(x.getSrcs()->begin(),x.getSrcs()->end());
    return *this;
}
int JapaneseChunk::getDst()
{
    return Dst;
}
int JapaneseChunk::getNum()
{
    return Num;
}
vector<Morph>& JapaneseChunk::getMorph()
{
    return M;
}
vector<int>* JapaneseChunk::getSrcs()
{
    return Srcs;
}
void JapaneseChunk::setNum(int N)
{
    Num = N;
}
void JapaneseChunk::setDst(int D)
{
    Dst = D;
}
void JapaneseChunk::AddToMorph(Morph x)
{
    M.push_back(x);
}
string JapaneseChunk::getPartMorphString()
{
    string anwser = "";
    for(vector<Morph>::iterator it = M.begin(); it != M.end(); it ++)
    {
        int flag = 0;
        if(it->pos == "名詞")flag = 1;
        else
        {
            if(it->pos == "助詞" && it->surface == "の")
            {
                if((it+1) != M.end() && (it+1)->pos == "名詞")flag = 1;
            }
            else if(it->pos == "接頭詞")
            {
                if((it+1) != M.end() && (it+1)->pos == "名詞")flag = 1;
            }
        }
        if(flag)anwser += it->surface;
    }
    return anwser;
}
string JapaneseChunk::getMorphStringToNoun()
{
    string anwser = "";
    vector<Morph>::iterator id;
    for(vector<Morph>::iterator it = M.begin(); it != M.end(); it ++)if(it->pos == "名詞")id = it;
    ++id;
    for(vector<Morph>::iterator it = M.begin(); it != id; it ++)anwser += it->surface;
    return anwser;
}
string JapaneseChunk::getMorphString()
{
    string anwser = "";
    for(vector<Morph>::iterator it = M.begin(); it != M.end(); it ++)anwser += it->surface;
    return anwser;
}
bool JapaneseChunk::nounChunk()
{
    for(vector<Morph>::iterator it = M.begin(); it != M.end(); it ++)
    {
        if(it->pos == "名詞" && it->pos1 != "非自立" && it->pos1 != "代名詞" && it->pos1 != "副詞可能")
        {
            if(it->pos1 != "サ変接続")return 1;
            else
            {
                if((it+1) == M.end())return 1;
                else
                {
                    if((it+1)->pos != "動詞")return 1;
                }
            }
        }
    }
    return 0;
}
int toNumber(string str, int idx)
{
    string::size_type length;
    length = str.size() - idx;
    int num  = 0, Neg = 1;
    for(string::size_type i = 0 ; i< length; i ++)
    {
        if(str[i] == '-')Neg = -1;
        else num = num*10 + (str[i] - '0');
    }
    return num*Neg;
}
bool withOutComma(vector<Morph> &XX)
{
    for(vector<Morph>::iterator it = XX.begin(); it != XX.end(); it ++)
    {
        if(it != XX.begin() && (it+1) != XX.end() && is_Comma(it->surface))return 0;
    }
    return 1;
}
void PickOutNounChunk(vector<JapaneseChunk*> &ChunkList,JapaneseChunk *X,ofstream &Fout)
{
    string anwser = "";
    bool flag = 0;
    if(X->getFlag())return;
    if(X->getDst() != -1 && ChunkList[X->getDst()]->getDst() != -1 && X->getDst() == X->getNum()+1 )//linked four phrase
    {
        int ID;
        ID = ChunkList[X->getDst()]->getDst();
        if(ID != -1 && ID == X->getDst() + 1)
        {
            int ID1 = ChunkList[ID]->getDst();
            if(ID1 != -1 && ID1 == ID + 1 &&ChunkList[ID1]->nounChunk())
            {
                vector<Morph>XX;
                for(vector<Morph>::iterator it = X->getMorph().begin(); it != X->getMorph().end(); it ++)XX.push_back(*it);
                for(vector<Morph>::iterator it = ChunkList[X->getDst()]->getMorph().begin(); it != ChunkList[X->getDst()]->getMorph().end(); it ++)XX.push_back(*it);
                for(vector<Morph>::iterator it = ChunkList[ID]->getMorph().begin(); it != ChunkList[ID]->getMorph().end(); it ++)XX.push_back(*it);
                vector<Morph>::iterator id;
                for(vector<Morph>::iterator it = ChunkList[ID1]->getMorph().begin(); it != ChunkList[ID1]->getMorph().end(); it ++)if(it->pos == "名詞")id = it;
                ++id;
                for(vector<Morph>::iterator it = ChunkList[ID1]->getMorph().begin(); it != id; it ++)XX.push_back(*it);
                if(withOutComma(XX))
                {
                    anwser += X->getMorphString();
                    anwser += ChunkList[X->getDst()]->getMorphString();
                    anwser += ChunkList[ID]->getMorphString();
                    anwser += ChunkList[ID1]->getMorphStringToNoun();
                    ChunkList[X->getDst()]->setFlag(true);
                    ChunkList[ID]->setFlag(true);
                    ChunkList[ID1]->setFlag(true);
                    Fout<<anwser<<endl;
                    flag = 1;
                }
            }
        }
    }
    if(flag == 0 && X->getDst() != -1 &&ChunkList[X->getDst()]->getDst() != -1) //A -> C , B->C , C->D
    {
        if(ChunkList[X->getNum()+1]->getDst() == X->getDst() && X->getDst() == X->getNum()+2)
        {
            int ID = ChunkList[X->getDst()]->getDst();
            if(ID != -1 && ChunkList[ID]->nounChunk() && ID == X->getDst() + 1)
            {
                vector<Morph>XX;
                for(vector<Morph>::iterator it = X->getMorph().begin(); it != X->getMorph().end(); it ++)XX.push_back(*it);
                for(vector<Morph>::iterator it = ChunkList[X->getNum()+1]->getMorph().begin(); it != ChunkList[X->getNum()+1]->getMorph().end(); it ++)XX.push_back(*it);
                for(vector<Morph>::iterator it = ChunkList[X->getDst()]->getMorph().begin(); it != ChunkList[X->getDst()]->getMorph().end(); it ++)XX.push_back(*it);
                vector<Morph>::iterator id;
                for(vector<Morph>::iterator it = ChunkList[ID]->getMorph().begin(); it != ChunkList[ID]->getMorph().end(); it ++)if(it->pos == "名詞")id = it;
                ++id;
                for(vector<Morph>::iterator it = ChunkList[ID]->getMorph().begin(); it != id; it ++)XX.push_back(*it);
                if(withOutComma(XX))
                {
                    anwser += X->getMorphString();
                    anwser += ChunkList[X->getNum()+1]->getMorphString();
                    anwser += ChunkList[X->getDst()]->getMorphString();
                    anwser += ChunkList[ID]->getMorphStringToNoun();
                    ChunkList[X->getNum()+1]->setFlag(true);
                    ChunkList[X->getDst()]->setFlag(true);
                    ChunkList[ID]->setFlag(true);
                    Fout<<anwser<<endl;
                    flag = 1;
                }
            }
        }
    }
    if(flag == 0 && X->getDst() != -1 && ChunkList[X->getDst()]->getDst() != -1 && X->getDst() == X->getNum()+1 ) //linked three phrase
    {
        int ID;
        ID = ChunkList[X->getDst()]->getDst();
        if(ID != -1 && ChunkList[ID]->nounChunk() && ID == X->getDst() + 1)
        {
            vector<Morph>XX;
            for(vector<Morph>::iterator it = X->getMorph().begin(); it != X->getMorph().end(); it ++)XX.push_back(*it);
            for(vector<Morph>::iterator it = ChunkList[X->getDst()]->getMorph().begin(); it != ChunkList[X->getDst()]->getMorph().end(); it ++)XX.push_back(*it);
            vector<Morph>::iterator id;
            for(vector<Morph>::iterator it = ChunkList[ID]->getMorph().begin(); it != ChunkList[ID]->getMorph().end(); it ++)if(it->pos == "名詞")id = it;
            ++id;
            for(vector<Morph>::iterator it = ChunkList[ID]->getMorph().begin(); it != id; it ++)XX.push_back(*it);
            if(withOutComma(XX))
            {
                anwser += X->getMorphString();
                anwser += ChunkList[X->getDst()]->getMorphString();
                anwser += ChunkList[ID]->getMorphStringToNoun();
                ChunkList[X->getDst()]->setFlag(true);
                ChunkList[ID]->setFlag(true);
                Fout<<anwser<<endl;
                flag = 1;
            }
        }
    }
    if(flag == 0 && X->getDst() != -1&&ChunkList[X->getDst()]->nounChunk()) //A -> C  B ->C
    {
        if(ChunkList[X->getNum()+1]->getDst() == X->getDst() && X->getDst() == X->getNum()+2)
        {
            vector<Morph>XX;
            for(vector<Morph>::iterator it = X->getMorph().begin(); it != X->getMorph().end(); it ++)XX.push_back(*it);
            for(vector<Morph>::iterator it = ChunkList[X->getNum()+1]->getMorph().begin(); it != ChunkList[X->getNum()+1]->getMorph().end(); it ++)XX.push_back(*it);
            vector<Morph>::iterator id;
            for(vector<Morph>::iterator it = ChunkList[X->getDst()]->getMorph().begin(); it != ChunkList[X->getDst()]->getMorph().end(); it ++)if(it->pos == "名詞")id = it;
            ++id;
            for(vector<Morph>::iterator it = ChunkList[X->getDst()]->getMorph().begin(); it != id; it ++)XX.push_back(*it);
            if(withOutComma(XX))
            {
                anwser += X->getMorphString();
                anwser += ChunkList[X->getNum()+1]->getMorphString();
                anwser += ChunkList[X->getDst()]->getMorphStringToNoun();
                ChunkList[X->getNum()+1]->setFlag(true);
                ChunkList[X->getDst()]->setFlag(true);
                Fout<<anwser<<endl;
                flag = 1;
            }
        }
    }
    if(flag ==0 && X->getDst() != -1 &&ChunkList[X->getDst()]->nounChunk() && X->getDst() == X->getNum()+1)  //linked two phrase
    {
        vector<Morph>XX;
        for(vector<Morph>::iterator it = X->getMorph().begin(); it != X->getMorph().end(); it ++)XX.push_back(*it);
        vector<Morph>::iterator id;
        for(vector<Morph>::iterator it = ChunkList[X->getDst()]->getMorph().begin(); it != ChunkList[X->getDst()]->getMorph().end(); it ++)if(it->pos == "名詞")id = it;
        ++id;
        for(vector<Morph>::iterator it = ChunkList[X->getDst()]->getMorph().begin(); it != id; it ++)XX.push_back(*it);
        if(withOutComma(XX))
        {
            anwser += X->getMorphString();
            anwser += ChunkList[X->getDst()]->getMorphStringToNoun();
            ChunkList[X->getDst()]->setFlag(true);
            Fout<<anwser<<endl;
            flag = 1;
        }
    }
    if(flag == 0 && X->nounChunk()) // one phrase
    {
        Fout<<X->getPartMorphString()<<endl;
    }
}
void AddToChunkList(vector<JapaneseChunk*> &ChunkList, JapaneseChunk *chunk)
{
    for(vector<JapaneseChunk*>::iterator i = ChunkList.begin(); i != ChunkList.end(); i++)
    {
        if((*i)->getDst() == -1)continue;
        if((*i)->getDst() == chunk->getNum())
        {
            chunk->getSrcs()->push_back((*i)->getNum());
        }
    }
    ChunkList.push_back(chunk);
}
void AnalyseInput(vector<string>&Input,ofstream &Fout)
{
    vector<string>L;
    JapaneseTermExtract R;
    vector<JapaneseChunk*>ChunkList;
    JapaneseChunk *chunk;
    chunk = new JapaneseChunk;
    for(vector<string>::iterator it = Input.begin(); it != Input.end(); it ++)
    {
        R.split(*it,' ',L);
        if(*L.begin() == "*")
        {
            vector<string>::iterator i = L.begin();
            int Num = toNumber(*(i+1),0);
            int Dst = toNumber(*(i+2),1);
            if(Num != 0)
            {
                AddToChunkList(ChunkList,chunk);
                chunk = new JapaneseChunk;
            }
            chunk->setDst(Dst);
            chunk->setNum(Num);
        }
        else
        {
            L.clear();
            R.split(*it,'\t',L);
            Morph m;
            m.surface = L[0];
            string tmp = L[1];
            L.clear();
            R.split(tmp,',',L);
            m.pos = L[0];
            m.pos1 = L[1];
            m.base = L[6];
            chunk->AddToMorph(m);
        }
        L.clear();
    }
    AddToChunkList(ChunkList,chunk);
    for(vector<JapaneseChunk*>::iterator it = ChunkList.begin(); it != ChunkList.end(); it ++)
    {
        PickOutNounChunk(ChunkList,*it,Fout);
    }
    for(vector<JapaneseChunk*>::iterator it = ChunkList.begin(); it != ChunkList.end(); it ++)(*it)->~JapaneseChunk();
    ChunkList.clear();
}
