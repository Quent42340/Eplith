#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <cstdlib>

using namespace std;

#define MEM_SIZE 0xFFFF

inline bool isd(char c) { return (c >= '0' && c <= '9'); }
inline bool ish(char c) { return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || isd(c)); }

bool HEX, DEBUG;

vector<unsigned char> memory(MEM_SIZE, 0);

string sourceCode;

unsigned int i;

#include "errors.h"
#include "attributes.h"
#include "operators.h"
#include "vars.h"

#endif // MAIN_H
