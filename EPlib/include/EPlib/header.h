/*---------------------------------------------------------------------------------

    Eplith Intepreter
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
#ifndef EPLIB_HEADER_H
#define EPLIB_HEADER_H

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <cstddef>
#include <cstdarg>
#include <boost/any.hpp>

#define EPLITH_DEBUG
#define YYERROR_VERBOSE

#define WARNINGS

#define VAR_DEBUG
#define TYPES_DEBUG
//#define CALL_DEBUG
//#define DELVAR_DEBUG
//#define SETVAR_DEBUG

#define edbg(str) cout << "\33[0;36;01m" << str << "\33[0m" << endl;
#define edbg2(str, other, str2) cout << "\33[0;36;01m" << str; other; cout << str2 << "\33[0m" << endl;
#define cdbg(str) cout << "\33[0;35;01m" << str << "\t\t\t" << __FILE__ << ":" << __LINE__ << "\33[0m" << endl;
#define cdbg2(str, other, str2) cout << "\33[0;35;01m" << str; other; cout << str2 << "\t\t\t" << __FILE__ << ":" << __LINE__ << "\33[0m" << endl;

static inline std::string itos(int n) {
	std::ostringstream oss;
	oss << n;
	return oss.str();
}

static int *stoi(const char *c) {
	int *r = new int[2];
	r[1] = sscanf(c, "%d", &r[0]);
	return r;
}

extern int yylineno;
extern char* yytext;
extern std::string EP_filename;

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

#define yyerror(str) error(str, __FILE__, __LINE__)
#define yywarn(str) warn(str, __FILE__, __LINE__)

void error(std::string str, const char* file, unsigned int line);
void warn(std::string str, const char* file, unsigned int line);

#define DBG(n) { printf("\t%d\n", n); }

int newFile(char *filename);

#endif // EPLIB_HEADER_H

