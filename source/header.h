/*---------------------------------------------------------------------------------

    WordSphere Intepreter
    Copyright (C) 2012 Quent42340 <quent42340@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

---------------------------------------------------------------------------------*/
#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>

// Booleans for C
typedef int bool;
#define true 1
#define false 0

#define DBG(n) { printf("\t%d\n", n); }

#define VAR_INT "int"
#define VAR_CHAR "char"
#define VAR_VOID "void"

typedef struct {
	char* name;
	int id;
	
	char* type;
	
	union {
		int iValue;
		char* sValue;
	};
} bVar;

extern bVar* vars;
extern int sizeVars;

bVar iVar(char* name, int value);
bVar sVar(char* name, char* value);

char* var_name(bVar var);
char* var_type(bVar var);

int var_iValue(bVar var);
char* var_sValue(bVar var);

bVar var_findByName(char* name);
bVar var_findByID(int id);

typedef enum { typeInt, typeStr, typeVar, typeOpr } nodeEnum;

/* operators */
typedef struct {
	int oper;                   /* operator */
	int nops;                   /* number of operands */
	struct nodeTypeTag **op;	/* operands */
} oprNodeType;

typedef struct nodeTypeTag {
	nodeEnum type;              /* type of node */
	
	union {
		int	val;				/* integer values */
		char* str;				/* string values */
		bVar var;				/* variable */
		oprNodeType opr;        /* operators */
	};
} nodeType;

typedef union {
	int i;
	char* s;
	bVar v;
} sType;

sType ex(nodeType* p);
void freeNode(nodeType* p);

nodeType* opr(int oper, int nops, ...);
nodeType* cInt(int value);
nodeType* cStr(char* string);
nodeType* cVar(bVar var);

#endif // HEADER_H
