#ifndef SPLIT_H_
#define SPLIT_H_

#include <vector>
#include <string>
using namespace std;



class Split {

public:
    Split();
    vector<string> split(const string & path, char c);//splits a string by a token returns vector of splited string

};
#endif
