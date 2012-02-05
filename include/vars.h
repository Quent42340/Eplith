#ifndef VARS_H
#define VARS_H

#include "attributes.h"

typedef struct {
	char* name;
	DataRange dr;
} uInt;

extern std::vector<uInt> vars;

char* catchVarName();
uInt catchVar();
uInt& findVar(char* name);

#endif // VARS_H
