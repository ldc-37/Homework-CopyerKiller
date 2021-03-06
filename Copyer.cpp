#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <vector> //store intersection and union
#include <iomanip> //set precision

#include "copyer.h"

using namespace std;

int main()
{
    cout << "Copyright (c) 2017-2018 李定成(221701331). All Rights Reserved." << endl;
#ifdef _DEBUG
    FunctionInfo finfo[100];
    string path = "C:\\Users\\asus\\Desktop\\test1.c";
    ifstream file;
    file.open (path);
    cout << "File: " << path << endl;
    int funcNum = CollectFuncInfo (file, finfo);
    cout << "FunctionNum:" << funcNum << endl;
    for (int i = 0; i < funcNum; ++i) {
        cout << "in function [" << finfo[i].name << "]:" << endl;
        cout << "number of keyword: " << finfo[i].keywordNum << endl;
        cout << "number of variable: " << finfo[i].varNum << endl;
        cout << endl;
    }
    cout << endl;
    system ("pause");
    exit (0);
#endif //_DEBUG

    int funcNumA, funcNumB;
    FunctionInfo finfoA[100];
    FunctionInfo finfoB[100];
    string fpath1, fpath2;
    cout << "请自行确认文件正确性！" << endl << endl;
    cout << "请拖入第一个文件并按确认键" << endl;
    cin >> fpath1;
    cout << "请拖入第二个文件并按确认键" << endl;
    cin >> fpath2;
    ifstream file1, file2;
    file1.open (fpath1, ios::in);
    file2.open (fpath2, ios::in);
    if (!file1.good () || !file2.good ()) {
        cout << "Cannot Open File!!!" << endl;
        system ("pause");
        exit (-1);
    }
    funcNumA = CollectFuncInfo (file1, finfoA);
    funcNumB = CollectFuncInfo (file2, finfoB);
    system ("cls");
    cout << "=========================Data=======================" << endl;
    cout << "File path 1: " << fpath1 << endl << endl;
    cout << "FunctionNum: " << funcNumA << endl << endl;
    ShowStatistic (funcNumA, finfoA);
    cout << "----------------------------------------------------" << endl;
    cout << "File path 2: " << fpath2 << endl << endl;
    cout << "FunctionNum: " << funcNumB << endl << endl;
    ShowStatistic (funcNumB, finfoB);
    cout << "=======================Analysis=====================" << endl;
    ShowAnalysis (finfoA, finfoB, funcNumA, funcNumB);
    cout << endl;
    system ("pause");
    file1.close ();
    file2.close ();
    return 0;
}

int CollectFuncInfo (ifstream& ifs, FunctionInfo (&finfo)[MAX_FUNC_NUM])
{
    int funcNum = 0;
    string strtmp;
    char chartmp;
    while (1) {
        ifs >> strtmp;
        if (ifs.eof ()) break;
        if (strtmp.substr (0, 2) == "//") { //ignore comments
            ifs.seekg (-1, ios::cur);
            getline (ifs, strtmp);
        }
        else if (strtmp.substr (0, 2) == "/*") { //ignore comments
            while (1) {
                ifs >> chartmp;
                if (chartmp == '*') {
                    ifs >> chartmp;
                    if (chartmp == '/') break;
                }
            }
        }
        else if (strtmp.length () >=3 && strtmp.length () <= 6 && 
            InArray<string> (TypeName, strtmp, 7)) { //catch like:int XXX
            auto typeNamePos = ifs.tellg ();
            while (1) {
                ifs >> chartmp;
                if (chartmp == ')') { //it's a function head
                    ifs >> chartmp;
                    if (chartmp == '{') { //is definition
                        //go inside and count the number of this func's variables，etc.
                        finfo[funcNum].id = funcNum + 1;
                        auto startPos = ifs.tellg ();
                        ifs.seekg (typeNamePos);
                        finfo[funcNum].name = PickFuncName (ifs);
                        ifs.seekg (startPos);
                        finfo[funcNum].varNum = CountVariableNum (ifs);
                        ifs.clear ();
                        ifs.seekg (startPos);
                        finfo[funcNum].keywordNum = CountKeywordNum (ifs);
                        funcNum++;
                        break;
                    }
                    else if (chartmp == ';') { //is declearation
                        break;
                    }
                    else {
                        cout << "Seems syntax error: Lack of \';\'" << endl;
                        getline (ifs, strtmp);
                        cout << "Relevant sentence:" << strtmp << endl;
                        system ("pause");
                        return -1;
                    }
                }
                if (chartmp == ';') { //end of line
                    break;
                }
            }
        }
    }
    return funcNum;
}

int CountVariableNum (ifstream& ifs)
{
    int varNum = 0, braceNum = 1;//the number of non-match brace
    string strtmp;
    char chartmp;
    auto startPos = ifs.tellg ();
    while (braceNum) { // calc the func's length
        ifs >> chartmp;
        if (ifs.eof ()) {
            cout << "Unexpected EOF!Lack of brace." << endl;
            system ("pause");
            return -1;
        }
        if (chartmp == '{') braceNum++;
        if (chartmp == '}') braceNum--;
    }
    auto endPos = ifs.tellg ();
    ifs.seekg (startPos); //go back 
    while (ifs.tellg () < endPos && !ifs.eof ()) {
        ifs >> strtmp;
        if (strtmp.substr (0, 2) == "//") { //ignore comments
            getline (ifs, strtmp);
        }
        if (strtmp.length () >= 3 && strtmp.length () <= 6 && InArray<string> (TypeName, strtmp, 7)) { //catch like:int XXX
            while (1) {
                ifs >> chartmp;
                if (chartmp == '(') { //it's a function
                    break;
                }
                else if (chartmp == ',') { //many variables, continue
                    varNum++;
                }
                else if (chartmp == ';') { //single variable
                    varNum++;
                    break;
                }
            }
        }
    }
    return varNum;
}

int CountKeywordNum (ifstream& ifs)
{
    int keywordNum = 0, braceNum = 1;//the number of non-match brace
    string strtmp;
    char chartmp;
    auto startPos = ifs.tellg ();
    while (braceNum) { // calc the func's length
        ifs >> chartmp;
        if (ifs.eof ()) {
            cout << "Unexpected EOF!Lack of brace." << endl;
            system ("pause");
            return -1;
        }
        if (chartmp == '{') braceNum++;
        if (chartmp == '}') braceNum--;
    }
    auto endPos = ifs.tellg ();
    ifs.seekg (startPos); //go back 
    while (ifs.tellg () < endPos && !ifs.eof ()) {
        ifs >> strtmp;
        for (int i = 0; i < KEYWORD_NUM; ++i) {
            auto idx = strtmp.find (KeyWord[i]);
            if (idx != string::npos) { //detect valid keyword
                keywordNum++;
            }
        }
    }
    return keywordNum;
}

string PickFuncName (ifstream& ifs)
{
    string name;
    char tmp;
    ifs >> tmp;
    while (tmp != ' ' && tmp != '(' ) {
        name += tmp;
        ifs >> tmp;
    }
    return name;
}

void ShowStatistic (int funcNum, FunctionInfo (&finfo)[MAX_FUNC_NUM])
{
    for (int i = 0; i < funcNum; ++i) {
        cout << "In function [" << finfo[i].name << "]:" << endl;
        cout << "Keywords: " << finfo[i].keywordNum << 
            " ; Variables: " << finfo[i].varNum << endl << endl;
    }
}

void ShowAnalysis (FunctionInfo (&finfoA)[MAX_FUNC_NUM], FunctionInfo (&finfoB)[MAX_FUNC_NUM], int numA, int numB)
{
    vector<int> keywordA, keywordB, varA, varB;
    for (int i = 0; i < numA; ++i) {
        varA.push_back (finfoA[i].varNum);
        keywordA.push_back (finfoA[i].keywordNum);
    }
    for (int i = 0; i < numB; ++i) {
        varB.push_back (finfoB[i].varNum);
        keywordB.push_back (finfoB[i].keywordNum);
    }
    vector<int> varNumU = VecUnion (varA, varB);
    vector<int> keyNumU = VecUnion (keywordA, keywordB);
    vector<int> varNumI = VecIntersection (varA, varB);
    vector<int> keyNumI = VecIntersection (keywordA, keywordB);

    float varSimilar = (float)varNumI.size () / varNumU.size ();
    float keySimilar = (float)keyNumI.size () / keyNumU.size ();

    cout << "各函数关键词数量相似度：" << setprecision (4) << keySimilar * 100 << endl;
    cout << "各函数变量数量相似度：" << setprecision (4) << varSimilar * 100 << endl;
    double totalSimilar = 25 + 0.5 * (varSimilar * 100 + keySimilar * 100) / 2.0 + (numA == numB ? 10 : 0);
    cout << endl << "=>总相似度（25~85）：" << setprecision (4) << totalSimilar << endl;
}

template <typename T>
bool InArray (const T arr[], const T content, const int len)
{
    for (int i = 0; i < len; ++i) {
        if (content == arr[i]) {
            return true;
        }
    }
    return false;
}

bool InVector (const vector<int> vec, const int content)
{
    if (vec.size() == 0) {
        return false;
    }
    for (size_t i = 0; i < vec.size (); ++i) {
        if (content == vec[i]) {
            return true;
        }
    }
    return false;
}

vector<int> VecUnion (const vector<int> vecA, const vector<int> vecB)
{
    vector<int> union_;
    for (size_t i = 0; i < vecA.size (); ++i) {
        if (!InVector(union_, vecA[i])) {
            union_.push_back (vecA[i]);
        }
    }
    for (size_t i = 0; i < vecB.size (); ++i) {
        if (!InVector (union_, vecB[i])) {
            union_.push_back (vecB[i]);
        }
    }
    return union_;
}

vector<int> VecIntersection (const vector<int> vecA, const vector<int> vecB)
{
    vector<int> intersection;
    for (size_t i = 0; i < vecA.size (); ++i) {
        if (InVector (vecB, vecA[i]) && !InVector (intersection, vecA[i])) {
            intersection.push_back (vecA[i]);
        }
    }
    return intersection;
}