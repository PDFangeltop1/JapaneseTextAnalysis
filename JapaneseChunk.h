#include <iostream>
#include <vector>
#include <string>
using namespace std;
class Morph{
public:
    string surface,base,pos,pos1;
    void output()
    {
        cout<<"surface -> "<<surface << " base -> "<<base<< " pos -> "<<pos<<" pos1 -> "<<pos1<<endl;
    }
};
class JapaneseChunk{
    vector<Morph>M;
    int Num,Dst;
    vector<int> *Srcs;
    bool flag;
public:
    JapaneseChunk();
  //  JapaneseChunk(vector<int> *S);
  //  JapaneseChunk(vector<Morph> x,int n,int d,vector<int> *S);
    JapaneseChunk& operator=(JapaneseChunk &x);
    ~JapaneseChunk();
    int getNum();
    void setNum(int N);
    int getDst();
    void setDst(int D);
    vector<int>* getSrcs();
    void AddToMorph(Morph x);
    vector<Morph>& getMorph();
    string getMorphString();
    string getPartMorphString();
    string getMorphStringToNoun();
    bool nounChunk();
    bool getFlag(){return flag;}
    void setFlag(bool x)
    {
        flag = x;
    }
};
void AnalyseInput(vector<string> &Input,ofstream &Fout);
int toNumber(string str,int idx);
