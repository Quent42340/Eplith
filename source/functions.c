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
#include "header.h"
#include "parser.tab.h"

using namespace std;

bVar* vars;
int sizeVars = 0;

bVar iVar(char* name, int value) {
	bVar var;
	
	var.name = name;
	var.id = sizeVars;
	var.type = typeInt;
	var.iValue = value;
	
	printf("Var name: %s | Value: %d | ID: %d | Nb of vars: %d\n", var.name, var.iValue, var.id, sizeVars);
	
	vars[sizeVars] = var;
	sizeVars++;
	
	return var;
}

bVar sVar(char* name, char* value) {
	bVar var;
	
	var.name = name;
	var.id = sizeVars;
	var.type = typeStr;
	var.sValue = value;
	
	printf("Var name: %s | Value: %s | ID: %d | Nb of vars: %d\n", var.name, var.sValue, var.id, sizeVars);
	
	vars[sizeVars] = var;
	sizeVars++;
	
	return var;
}

char* var_name(bVar var) { return var.name; }
int var_id(bVar var) { return var.id; }
nodeEnum var_type(bVar var) { return var.type; }
int var_iValue(bVar var) { return var.iValue; }
char* var_sValue(bVar var) { return var.sValue; }

bVar bVarNULL = {"NULL", -1, typeVoid, -1};

bVar var_findByName(char* name) {
	int i;
	
	for(i = 0 ; i < sizeVars ; i++) {
		if(!strcmp(vars[i].name, name)) {
			return vars[i];
		}
	}
	
	return bVarNULL;
}

bVar var_findById(int id) {
	int i;
	
	for(i = 0 ; i < sizeVars ; i++) {
		if(vars[i].id == id) {
			return vars[i];
		}
	}
	
	return bVarNULL;
}

/**
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.
 */
char* itoa(int value, char* result, int base) {
	// check that the base if valid
	if (base < 2 || base > 36) { *result = '\0'; return result; }
	
	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;
	
	do {
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
	} while ( value );
	
	// Apply negative sign
	if (tmp_value < 0) *ptr++ = '-';
	*ptr-- = '\0';
	while(ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr--= *ptr1;
		*ptr1++ = tmp_char;
	}
	return result;
}

void aprintf(sType t) {
	if(t.t == typeStr) {
		printf("%s", t.s);
	}
	else if(t.t == typeInt) {
		printf("%d", t.i);
	} else {
		yyerror("Unexpected argument given to print function");
	}
}

sType ex(nodeType* p) {
	sType ret;
	int i;
	if(!p) { ret.i = 0; ret.t = typeInt; return ret; }
	switch(p->type) {
		case typeInt: {
			ret.t = typeInt;
			ret.i = p->val; return ret;
		}
		case typeStr: {
			ret.t = typeStr;
			ret.s = p->str; return ret;
		}
		case typeVar: {
			if(p->var.type == typeInt) {
				ret.t = typeInt; ret.i = p->var.iValue;
			}
			else if(p->var.type == typeStr) {
				ret.t = typeStr; ret.s = p->var.sValue;
			} else {
				yyerror("Unexistant variable given to print function");
			}
			return ret;
		}
		case typeOpr:
			switch(p->opr.oper) {
				case WHILE:	{
					while(ex(p->opr.op[0]).i) {
						ex(p->opr.op[1]);
						ret.i = 0; ret.t = typeInt; return ret;
					}
				}
				case IF: {
					if(ex(p->opr.op[0]).i) {
						ex(p->opr.op[1]);
					}
					else if(p->opr.nops > 2) {
						ex(p->opr.op[2]);
					}
					ret.i = 0; ret.t = typeInt; return ret;
				}
				case PRINT: {
					cout << "truc";
					if(ret.t == typeInt) {
						cout << ex(p->opr.op[0]).s;
					}
					else if(ret.t == typeStr) {
						cout << ex(p->opr.op[0]).i;
					}
					else if(ret.t == typeVar) {
						if(ret.v.type == typeInt) {
							cout << ex(p->opr.op[0]).v.iValue;
						}
						else if(ret.v.type == typeStr) {
							cout << ex(p->opr.op[0]).v.sValue;
						} else {
							yyerror("Unexistant variable given to print function");
						}
					} else {
						yyerror("Unexpected argument given to print function");
					}
					cout << endl;
					ret.i = 0; ret.t = typeInt; return ret;
				}
				case ';':		ex(p->opr.op[0]); ret = ex(p->opr.op[1]); return ret;
				case EQI:		ret.v = iVar(p->opr.op[0]->str, ex(p->opr.op[1]).i); ret.t = typeVar; return ret;
				case EQS:		ret.v = sVar(p->opr.op[0]->str, ex(p->opr.op[1]).s); ret.t = typeVar; return ret;
				case NEG:		ret.i = -ex(p->opr.op[0]).i; ret.t = typeInt; return ret;
				case '+':		ret.i = ex(p->opr.op[0]).i + ex(p->opr.op[1]).i; ret.t = typeInt; return ret;
				case STRA:		ret.s = strcat(ex(p->opr.op[0]).s, ex(p->opr.op[1]).s); ret.t = typeStr; return ret;
				case '-':		ret.i = ex(p->opr.op[0]).i - ex(p->opr.op[1]).i; ret.t = typeInt; return ret;
				case '*':		ret.i = ex(p->opr.op[0]).i * ex(p->opr.op[1]).i; ret.t = typeInt; return ret;
				case '/':		ret.i = ex(p->opr.op[0]).i / ex(p->opr.op[1]).i; ret.t = typeInt; return ret;
				case '<':		ret.i = ex(p->opr.op[0]).i < ex(p->opr.op[1]).i; ret.t = typeInt; return ret;
				case '>':		ret.i = ex(p->opr.op[0]).i > ex(p->opr.op[1]).i; ret.t = typeInt; return ret;
				case GE:		ret.i = ex(p->opr.op[0]).i >= ex(p->opr.op[1]).i; ret.t = typeInt; return ret;
				case LE:		ret.i = ex(p->opr.op[0]).i <= ex(p->opr.op[1]).i; ret.t = typeInt; return ret;
				case NE:		ret.i = ex(p->opr.op[0]).i != ex(p->opr.op[1]).i; ret.t = typeInt; return ret;
				case EQ:		ret.i = ex(p->opr.op[0]).i == ex(p->opr.op[1]).i; ret.t = typeInt; return ret;
				case '^':		ret.i = pow(ex(p->opr.op[0]).i, ex(p->opr.op[1]).i); ret.t = typeInt; return ret;
			}
    }
    ret.i = 0; ret.t = typeInt; return ret;
}

void freeNode(nodeType* p) {
	int i;
	
	if(!p) return;
	if(p->type == typeOpr) {
		for (i = 0; i < p->opr.nops; i++)
			freeNode(p->opr.op[i]);
		
		free(p->opr.op);
	}
	
	free(p);
}

nodeType* opr(int oper, int nops, ...) {
	va_list ap;
	nodeType* p;
	int i;
	
	/* allocate node */
	if((p = (nodeType*)malloc(sizeof(nodeType))) == NULL) {
		yyerror("Out of memory");
	}
	
	if((p->opr.op = (nodeType**)malloc(nops * sizeof(nodeType))) == NULL) {
		yyerror("Out of memory");
	}
	
	/* copy information */
	p->type = typeOpr;
	p->opr.oper = oper;
	p->opr.nops = nops;
	
	va_start(ap, nops);
	
	for(i = 0 ; i < nops ; i++) {
		p->opr.op[i] = va_arg(ap, nodeType*);
	}
	
	va_end(ap);
	
	return p;
}

nodeType* cInt(int value) {
	nodeType *p;
	
	/* allocate node */
	if((p = (nodeType*)malloc(sizeof(nodeType))) == NULL)
		yyerror("Out of memory");
	
	/* copy information */
	p->type = typeInt;
	p->val = value;
	
	return p;
}

nodeType* cStr(char* string) {
	nodeType *p;
	
	/* allocate node */
	if((p = (nodeType*)malloc(sizeof(nodeType))) == NULL)
		yyerror("Out of memory");
	
	/* copy information */
	p->type = typeStr;
	p->str = string;
	
	return p;
}

nodeType* cVar(bVar var) {
	nodeType *p;
	
	/* allocate node */
	if((p = (nodeType*)malloc(sizeof(nodeType))) == NULL)
		yyerror("Out of memory");
	
	/* copy information */
	p->type = typeVar;
	p->var = var;
	
	return p;
}

