// stringToTokenVector.h
//
// author: Barbara Kreaseck for CPTG 244 course use
// updated: Nov 18, 2013
// ------------------------------------------------

#ifndef STRINGTOTOKENVECTOR_H
#define STRINGTOTOKENVECTOR_H

#include <string>
#include <vector>
#include <cstring> // brings in c's char string functions
#include "Token(2).h"
using namespace std;

vector<Token> stringToTokenVector(string str);
vector<Token> stringToTokenVector(char * cstr);

#endif
