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
%{
#include "header.h"
#include "expression.h"
#include "function.h"
#include "parser.tab.hpp"

using namespace std;

char stringBuf[1024];
char* stringBufPtr;

%}

%option yylineno

%x str

%%
	/* Strings */
\"							{ BEGIN str; stringBufPtr = stringBuf; }
<str>\"						{ *stringBufPtr = '\0'; yylval.sValue = strdup(stringBuf); BEGIN 0; return STRING; }
<str>\n						{ yyerror("Newline in string\n"); }
<str>\\n					{ *stringBufPtr++ = '\n'; }
<str>\\t					{ *stringBufPtr++ = '\t'; }
<str>\\r					{ *stringBufPtr++ = '\r'; }
<str>\\b					{ *stringBufPtr++ = '\b'; }
<str>\\f					{ *stringBufPtr++ = '\f'; }
<str>\\(.|\n)				{ *stringBufPtr++ = yytext[1]; }
<str>[^\\\n\"]+				{ char* yPtr = yytext; while(*yPtr) { *stringBufPtr++ = *yPtr++; } }

	/* Keywords */
"while"						Expression::scopes++; return WHILE;
"do"						return DO;
"if"						Expression::scopes++; return IF;
"else"						return ELSE;
"print"						return PRINT;
"function"					return FUNCTION;
"for"						Expression::scopes++; return FOR;
"->"						return TO;
^"#debug"[ \t\n]*$			return DEBUG;

	/* Booleans */
"true"						return TRUE;
"false"						return FALSE;

	/* Variable name */
[a-zA-Z]([0-9]|[a-zA-Z])*	yylval.sValue = strdup(yytext); return NAME;

	/* Integer */
[0-9]+						yylval.iValue = atoi(yytext); return INTEGER;

	/* Operators */
[-()<>=+*/!{}.;^]			return *yytext;
">="						return GE;
"<="						return LE;
"=="						return EQ;
"!="						return NE;

	/* Comments */
#!(.|\n)+!#					;	/* Ignore comments */
#.+$						;	/* Ignore comments */
	
	/* Whitespace */
[ \t\n]+					;	/* Ignore whitespace */
	
	/* Other */
.							yyerror("Unknown character");
%%

