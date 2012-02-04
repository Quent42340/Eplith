#ifndef GLOBALVARS_H
#define GLOBALVARS_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <cstdlib>

#define MEM_SIZE 0xFFFF

inline bool isd(char c) { return (c >= '0' && c <= '9'); }
inline bool ish(char c) { return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || isd(c)); }

extern bool HEX, DEBUG;

extern std::vector<unsigned char> memory;

extern std::string sourceCode;

extern unsigned int i;

#endif // GLOBALVARS_H

