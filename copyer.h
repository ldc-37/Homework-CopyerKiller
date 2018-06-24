#pragma once
#include <string>
#include <vector>

#define MAX_FUNC_NUM 100
#define KEYWORD_NUM 7 //same as length of (const string KeyWord[])

using namespace std;

struct FunctionInfo
{
    int id;
    string name;
    int varNum;
    int keywordNum;
};

const string TypeName[] = {"void", "int", "double", "float", "char", "int*", "char*"};
const string KeyWord[] = { "for", "if", "else", "do", "while", "break", "continue" };

enum keyword_set { for_, if_, else_, do_, while_, break_, continue_ } Keyword; //@todo

int CollectFuncInfo (ifstream&, FunctionInfo (&)[MAX_FUNC_NUM]);
int CountVariableNum (ifstream&);
int CountKeywordNum (ifstream&);
string PickFuncName (ifstream&);
void ShowStatistic (int funcNum, FunctionInfo (&)[MAX_FUNC_NUM]);
void ShowAnalysis (FunctionInfo (&)[MAX_FUNC_NUM], FunctionInfo (&)[MAX_FUNC_NUM], int, int);


template <typename T>
bool InArray (const T arr[], const T content, const int len);

bool InVector (const vector<int> vec, const int content);
vector<int> VecIntersection (const vector<int> vecA, const vector<int> vecB);
vector<int> VecUnion (const vector<int> vecA, const vector<int> vecB);