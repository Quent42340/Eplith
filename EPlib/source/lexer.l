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
%{
#include "header.h"
#include "expression.h"
#include "function.h"
#include "parser.tab.hpp"

using namespace std;

char stringBuf[1024];
char* stringBufPtr;

struct EFile {
		struct EFile *prev;
		YY_BUFFER_STATE bs;
		int lineno;
		char *filename;
		FILE *f;
} *currentFile = 0;

int newFile(char *filename);
int popFile();

%}

%option yylineno

%x str
%x COMMENT
%x IFILE

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
"while"						beginScope(stLOOP); return WHILE;
"do"						return DO;
"if"						beginScope(stOTHER); return IF;
"else"						return ELSE;
"function"					beginScope(stFUNC); return FUNCTION;
"for"						beginScope(stLOOP); return FOR;
"break"						return BREAK;
"continue"					return CONTINUE;
"return"					return RETURN;
"delete"					return DELETE;
"null"						return vNULL;
"global"					return GLOBAL;
("->"|"to")					return TO;
"hex"						return HEX;
	/* TODO: "oct"						return OCT;*/
	/* TODO: "bin"						return BIN;*/
"sci"						return SCI;

	/* Booleans */
"true"						return TRUE;
"false"						return FALSE;

	/* Variable name */
[a-zA-Z_][0-9a-zA-Z_]*		yylval.sValue = strdup(yytext); return NAME;

	/* Integer */
-?[0-9]+					yylval.iValue = atoi(yytext); return INTEGER;
"0x"[0-9a-fA-F]+			yylval.iValue = strtol(yytext, NULL, 16); return HEX_INT;

	/* Float */
-?[0-9]*"."[0-9]+ |
-?[0-9]*("."[0-9]+)?[eE][-+]?[0-9]+ yylval.fValue = atof(yytext); return FLOAT;

	/* Operators */
[-()<>=+*/%{}.:,;^]|"["|"]"	return *yytext;
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
("!="|"neq_")				return NE;
"+="						yylval.op = ADD; return ASSIGN_OP;
"-="						yylval.op = SUB; return ASSIGN_OP;
"*="						yylval.op = MUL; return ASSIGN_OP;
"/="						yylval.op = DIV; return ASSIGN_OP;
"^="						yylval.op = EXP; return ASSIGN_OP;
"%="						yylval.op = MOD; return ASSIGN_OP;
("&="|"and_eq")				yylval.op = BAND_EQ; return ASSIGN_OP;
("|="|"or_eq")				yylval.op = BOR_EQ; return ASSIGN_OP;
("\/="|"><="|"xor_eq")	 	yylval.op = XOR_EQ; return ASSIGN_OP;
"++"						return INCR;
"--"						return DECR;

	/* Comments */
"//"[^!].*\n				;
"/*"						BEGIN COMMENT;
<COMMENT>"*/"				BEGIN INITIAL;
<COMMENT>.|\n				;
<COMMENT><<EOF>>			yyerror("Unterminated comment");

	/* Whitespace */
[ \t\n]+					;	/* Ignore whitespace */
	
	/* Include */
^"-"[ \t]*include[ \t]*\" { BEGIN IFILE; }
<IFILE>[^ \t\n\"]+		  {
							{ int c;
							  while((c = yyinput()) && c != '\n') ;
							}
							if(!newFile(yytext)) yyterminate();
							BEGIN INITIAL;
						  }

<IFILE>.|\n				  { yyerror("Bad include line"); }
<<EOF>>					  { if(!popFile()) yyterminate(); }

	/* Other */
.							yyerror("Unknown character");
%%

int newFile(char *filename) {
	FILE *f = fopen(filename, "r");
	struct EFile *file = new struct EFile;
	
	if(!f) { yylineno--; yyerror(string("File '") + string(filename) + "' not found"); }
	if(!file) { yylineno--; yyerror("Memory allocation error"); }
	
	if(currentFile) currentFile->lineno = yylineno;
	file->prev = currentFile;
	
	file->bs = yy_create_buffer(f, YY_BUF_SIZE);
	file->f = f;
	file->filename = filename;
	yy_switch_to_buffer(file->bs);
	currentFile = file;
	yylineno = 1;
	return 1;
}

int popFile() {
	struct EFile *file = currentFile;
	struct EFile *prevFile;
	
	if(!file) return 0;
	
	fclose(file->f);
	yy_delete_buffer(file->bs);
	
	prevFile = file->prev;
	delete file;
	
	if(!prevFile) return 0;
	
	yy_switch_to_buffer(prevFile->bs);
	currentFile = prevFile;
	yylineno = currentFile->lineno;
	return 1; 
}

