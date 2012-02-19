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

extern FILE* yyin;

void yyerror(const char *str) {
	extern int yylineno;
	extern char* yytext;
	fprintf(stderr, "Error: %s at symbol \"%s\" at line %d\n", str, yytext, yylineno);
}

int yywrap() {
	return 1;
} 
 
void main(int argc, char* argv[]) {
	yyin = fopen(argv[1], "r");
	
	if(yyin == NULL) {
		fprintf(stderr, "Error: File %s doesn't exist.\n", argv[1]);
		exit(0);
	}
	
	vars = (bVar*)calloc(65535, sizeof(bVar));
	
	yyparse();
	
	free(vars);
	
	fclose(yyin);
} 

%}

%union {
	int iValue;
	char* sValue;
	bVar var;
	nodeType* nPtr;
}

%token <iValue> INTEGER
%token <sValue> WORD
%token <sValue> STRING

%token DEBUG
%token vNULL

%token STRA EXPA
%token EQI EQS

%token INTV STRV

%token WHILE IF PRINT
%token END
%nonassoc IFX
%nonassoc ELSE

%left GE LE EQ NE '>' '<'
%left '+' '-'
%left '*' '/'
%left NEG
%right '^'

%start program

%type <nPtr> stmt nbr str assign stmt_list exp
%type <var> ivar svar

%%
program:
	function { exit(0); }
	;

function: /* empty */
	| function stmt { ex($2); freeNode($2); }
	;

stmt:
	  ';' { $$ = opr(';', 2, NULL, NULL); }
	| assign ';' { $$ = $1; }
	| PRINT '(' exp ')' ';' { $$ = opr(PRINT, 1, $3); }
	| WHILE '(' nbr ')' stmt { $$ = opr(WHILE, 2, $3, $5); }
	| IF '(' nbr ')' stmt %prec IFX { $$ = opr(IF, 2, $3, $5); }
	| IF '(' nbr ')' stmt ELSE stmt { $$ = opr(IF, 3, $3, $5, $7); }
	| '{' stmt_list '}' { $$ = $2; }
	;

stmt_list:
	  stmt { $$ = $1; }
	| stmt_list stmt { $$ = opr(';', 2, $1, $2); }
	;

exp:
	nbr { $$ = $1; }
	| str { $$ = $1; }
	| exp '+' exp { $$ = opr(EXPA, 2, $1, $3); }
	;

nbr:
	  INTEGER { $$ = cInt($1); }
	| ivar { $$ = cVar($1); }
	| nbr '+' nbr { $$ = opr('+', 2, $1, $3); }
	| nbr '-' nbr { $$ = opr('-', 2, $1, $3); }
	| nbr '*' nbr { $$ = opr('*', 2, $1, $3); }
	| nbr '/' nbr { $$ = opr('/', 2, $1, $3); }
	| '-' nbr %prec NEG { $$ = opr(NEG, 1, $2); }
	| nbr '^' nbr { $$ = opr('+', 2, $1, $3); }
	| '(' nbr ')' { $$ = $2; }
	| nbr '<' nbr { $$ = opr('<', 2, $1, $3); }
	| nbr '>' nbr { $$ = opr('>', 2, $1, $3); }
	| nbr GE nbr { $$ = opr(GE, 2, $1, $3); }
	| nbr LE nbr { $$ = opr(LE, 2, $1, $3); }
	| nbr EQ nbr { $$ = opr(EQ, 2, $1, $3); }
	| nbr NE nbr { $$ = opr(NE, 2, $1, $3); }
	;

str:
	  STRING { $$ = cStr($1); }
	| svar { $$ = cVar($1); }
	| str '+' str { $$ = opr(STRA, 2, $1, $3); }
	;

ivar:
	INTV WORD ')' { $$ = var_findByName($2); }
	;

svar:
	STRV WORD ')' { $$ = var_findByName($2); }
	;

assign:
	  INTV WORD ')' '=' nbr { $$ = opr(EQI, 2, cStr($2), $5); }
	| STRV WORD ')' '=' str { $$ = opr(EQS, 2, cStr($2), $5); }
	;
%%

