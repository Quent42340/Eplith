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
%x COMMENT

%%
	/* Strings */
\"							{ BEGIN str; stringBufPtr = stringBuf; }
<str>\"						{ *stringBufPtr = '\0'; yylval.sValue = strdup(stringBuf); BEGIN 0; return STRING; }
	/*<str>\n						{ yyerror("Newline in string\n"); }*/
<str>\\n					{ *stringBufPtr++ = '\n'; }
<str>\\t					{ *stringBufPtr++ = '\t'; }
<str>\\r					{ *stringBufPtr++ = '\r'; }
<str>\\b					{ *stringBufPtr++ = '\b'; }
<str>\\f					{ *stringBufPtr++ = '\f'; }
<str>\\(.|\n)				{ *stringBufPtr++ = yytext[1]; }
<str>[^\\\n\"]+				{ char* yPtr = yytext; while(*yPtr) { *stringBufPtr++ = *yPtr++; } }

	/* Keywords */
^"#debug"[ \t\n]*$			return DEBUG;
"while"						Expression::scopes++; return WHILE;
"do"						return DO;
"if"						Expression::scopes++; return IF;
"else"						return ELSE;
"print"						return PRINT;
"function"					return FUNCTION;
"for"						Expression::scopes++; return FOR;
("->"|"to")					return TO;
"dec"						return DEC;
"hex"						return HEX;
	/* TODO: "oct"						return OCT;*/
	/* TODO: "bin"						return BIN;*/

	/* Booleans */
"true"						return TRUE;
"false"						return FALSE;

	/* Variable name */
[a-zA-Z]([0-9]|[a-zA-Z])*	yylval.sValue = strdup(yytext); return NAME;

	/* Integer */
[0-9]+						yylval.iValue = atoi(yytext); return INTEGER;
"0x"[0-9a-fA-F]+			yylval.iValue = strtol(yytext, NULL, 16); return HEX_INT;

	/* Operators */
[-()<>=+*/%{}.;^]			return *yytext;
("!"|"not")					return NOT;
("&&"|"and")				return AND;
("||"|"or")					return OR;
("&"|"bitand")				return BAND;
("|"|"bitor")				return BOR;
("><"|">-<"|"xor")			return XOR;
("~"|"compl"|"bitnot")		return BNOT;
("<<"|"lshift")				return LSHIFT;
(">>"|"rshift")				return RSHIFT;
(">="|"geq_")				return GE;
("<="|"leq_")				return LE;
("=="|"eq_")				return EQ;
("!="|"not_eq")				return NE;
"+="						yylval.op = ADD; return ASSIGN_OP;
"-="						yylval.op = SUB; return ASSIGN_OP;
"*="						yylval.op = MUL; return ASSIGN_OP;
"/="						yylval.op = DIV; return ASSIGN_OP;
"^="						yylval.op = EXP; return ASSIGN_OP;
"%="						yylval.op = MOD; return ASSIGN_OP;
("&="|"and_eq")				yylval.op = BAND_EQ; return ASSIGN_OP;
("|="|"or_eq")				yylval.op = BOR_EQ; return ASSIGN_OP;
("\/="|"><="|"xor_eq")	 	yylval.op = XOR_EQ; return ASSIGN_OP;

	/* Comments */
#[^\[].*\n						;
"#["						BEGIN COMMENT;
<COMMENT>"]#"				BEGIN INITIAL;
<COMMENT>.|\n				;
<COMMENT><<EOF>>			yyerror("Unterminated comment");

	/* Whitespace */
[ \t\n]+					;	/* Ignore whitespace */
	
	/* Other */
.							yyerror("Unknown character");
%%

