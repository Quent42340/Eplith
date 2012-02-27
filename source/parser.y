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

using namespace std;

extern FILE* yyin;

void yyerror(string str) {
	extern int yylineno;
	extern char* yytext;
	cerr << "Error: " << str << " at symbol \"" << yytext << "\" at line " << yylineno << endl;
	exit(1);
}

extern "C" {
int yywrap() {
	return 1;
} 
}

int main(int argc, char* argv[]) {
	Variable::initNullVar();
	
	yyin = fopen(argv[1], "r");
	
	if(yyin == NULL) {
		fprintf(stderr, "Error: File %s doesn't exist.\n", argv[1]);
		exit(0);
	}
	
	yyparse();
	
	fclose(yyin);
	
	return 0;
} 

%}

%union {
	int iValue;
	char* sValue;
	Expression *exp;
}

%token <iValue> INTEGER
%token <sValue> VARNAME
%token <sValue> STRING

%token DEBUG
%token vNULL

%token STRA
%token EQI EQS

%token WHILE IF PRINT
%token TRUE FALSE
%token END
%nonassoc IFX
%nonassoc ELSE

%left GE LE EQ NE '>' '<'
%left '+' '-'
%left '*' '/'
%left NEG
%right '^'

%start program

%type <exp> expr var assign stmt stmt_list

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
	| PRINT '(' expr ')' ';' { $$ = new PrintExpression($3); }
	| WHILE '(' expr ')' stmt { ; }
	| IF '(' expr ')' stmt %prec IFX { ; }
	| IF '(' expr ')' stmt ELSE stmt { ; }
	| '{' stmt_list '}' { $$ = $2; }
	;

stmt_list:
	  stmt { $$ = $1; }
	| stmt_list stmt { ; }
	;

expr:
	  INTEGER { $$ = new IntExpression($1); }
	| STRING { $$ = new StrExpression($1); }
	| TRUE { $$ = new BoolExpression(true); }
	| FALSE { $$ = new BoolExpression(false); }
	| var { $$ = $1; }
	| expr '+' expr { $$ = new OpExpression($1, '+', $3); }
	| expr '-' expr { $$ = new OpExpression($1, '-', $3); }
	| expr '*' expr { $$ = new OpExpression($1, '*', $3); }
	| expr '/' expr { $$ = new OpExpression($1, '/', $3); }
	| '-' expr %prec NEG { $$ = new OpExpression($2, NEG); }
	| expr '^' expr { $$ = new OpExpression($1, '^', $3); }
	| '(' expr ')' { $$ = $2; }
	| expr '<' expr { $$ = new OpExpression($1, '<', $3); }
	| expr '>' expr { $$ = new OpExpression($1, '>', $3); }
	| expr GE expr { $$ = new OpExpression($1, GE, $3); }
	| expr LE expr { $$ = new OpExpression($1, LE, $3); }
	| expr EQ expr { $$ = new OpExpression($1, EQ, $3); }
	| expr NE expr { $$ = new OpExpression($1, NE, $3); }
	;

assign:
	VARNAME '=' expr { $$ = new AssignExpression(string($1), $3); }
	;

var:
	VARNAME { $$ = new VarExpression(string($1)); }
	;
%%

