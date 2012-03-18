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

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <cstddef>
#include <cstdarg>

#define WARNINGS
#define YYTEXT_DEBUG 0
#define FUNC_DEBUG
#define VAR_DEBUG
#define TYPES_DEBUG
//#define DELVAR_DEBUG
//#define SETVAR_DEBUG

extern int yylineno;
extern char* yytext;

static inline std::string getPtrAddr(void* ptr) {
	std::stringstream oss;
	std::string str;
	oss << ptr;
	str = oss.str(); 
	return str;
}

extern "C" {
	int yyparse(void);
	int yylex(void);
	int yywrap(void);
}

void yyerror(std::string str);
void yywarn(std::string str);

#define DBG(n) { printf("\t%d\n", n); }

#endif // HEADER_H

