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
#include "parser.tab.h"
//"@("[a-z]+")"								printf("INT VAR\n");
//"$("[a-z]+")"								printf("STR VAR\n");
//"&("[0-9a-fA-F]+((";"|":")[0-9a-fA-F]+)?")"	printf("DATA RANGE\n");
%}

%%
"while"						return WHILE;
"if"						return IF;
"else"						return ELSE;
"print"						return PRINT;
[a-zA-Z]([0-9]|[a-zA-Z])*	yylval.sValue = strdup(yytext); return WORD;
\"(\\.|[^\\"])*\"			yylval.sValue =	strtok(strdup(yytext), "\""); return STRING;
[0-9]+						yylval.iValue = atoi(yytext); return INTEGER;
[-()<>=+*/{}.;^]			return *yytext;
"@("						return INTV;
"$("						return STRV;
">="						return GE;
"<="						return LE;
"=="						return EQ;
"!="						return NE;
^"#debug"[ \t\n]*$			return DEBUG;
#!(.|\n)+!#					;	/* ignore comments */
#.+$						;	/* ignore comments */
[ \t\n]+					;	/* ignore whitespace */
.							yyerror("Unknown character");
%%

