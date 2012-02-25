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
#include "statement.h"
#include "variable.h"

using namespace std;

extern FILE* yyin;

void yyerror(string str) {
	extern int yylineno;
	extern char* yytext;
	cerr << "Error:" << str << " at symbol \"" << yytext << "\" at line " << yylineno << endl;
}

extern "C" {
int yywrap() {
	return 1;
} 
}

Variable* vars;

int main(int argc, char* argv[]) {
	yyin = fopen(argv[1], "r");
	
	if(yyin == NULL) {
		fprintf(stderr, "Error: File %s doesn't exist.\n", argv[1]);
		exit(0);
	}
	
	vars = (Variable*)calloc(65535, sizeof(Variable));
	
	yyparse();
	
	free(vars);
	
	fclose(yyin);
	
	return 0;
} 

%}

%union {
	int iValue;
	char* sValue;
	Variable *var;
	Statement *stmt;
}

%token <iValue> INTEGER
%token <sValue> WORD
%token <sValue> STRING

%token DEBUG
%token vNULL

%token STRA
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

/*
%type <nPtr> stmt nbr str assign stmt_list
%type <var> ivar svar
*/

%%
program:
	function { ; }
	;

function: /* empty */
	| function stmt { ; }
	;

stmt:
	  ';' { ; }
	| assign ';' { ; }
	| PRINT '(' nbr ')' ';' { ; }
	| PRINT '(' str ')' ';' { ; }
	| WHILE '(' nbr ')' stmt { ; }
	| IF '(' nbr ')' stmt %prec IFX { ; }
	| IF '(' nbr ')' stmt ELSE stmt { ; }
	| '{' stmt_list '}' { ; }
	;

stmt_list:
	  stmt { ; }
	| stmt_list stmt { ; }
	;

nbr:
	  INTEGER { ; }
	| ivar { ; }
	| nbr '+' nbr { ; }
	| nbr '-' nbr { ; }
	| nbr '*' nbr { ; }
	| nbr '/' nbr { ; }
	| '-' nbr %prec NEG { ; }
	| nbr '^' nbr { ; }
	| '(' nbr ')' { ; }
	| nbr '<' nbr { ; }
	| nbr '>' nbr { ; }
	| nbr GE nbr { ; }
	| nbr LE nbr { ; }
	| nbr EQ nbr { ; }
	| nbr NE nbr { ; }
	;

str:
	  STRING { ; }
	| svar { ; }
	| str '+' str { ; }
	;

ivar:
	INTV WORD ')' { ; }
	;

svar:
	STRV WORD ')' { ; }
	;

assign:
	  INTV WORD ')' '=' nbr { ; }
	| STRV WORD ')' '=' str { ; }
	;
%%

