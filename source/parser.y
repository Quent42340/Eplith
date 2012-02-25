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
#include "variable.h"
#include "function.h"

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

int main(int argc, char* argv[]) {
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
	Variable *var;
	Statement *stmt;
	Function *func;
	IntValue *intValue;
	StrValue *strValue;
	NullValue *nullValue;
	Value* value;
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

%type <value> expr
%type <var> ivar svar assign
%type <stmt> stmt stmt_list

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
	| PRINT '(' expr ')' ';' { Value::print($3); }
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
	  INTEGER { $$ = new IntValue($1); }
	| STRING { $$ = new StrValue($1); }
	| ivar { $$ = new IntValue($1); }
	| svar { $$ = new StrValue($1); }
	| expr '+' expr { $$ = Value::add($1, $3); /*$$ = IntValue::op($1, '+', $3);*/ }
	| expr '-' expr { $$ = IntValue::op($1, '-', $3); }
	| expr '*' expr { $$ = IntValue::op($1, '*', $3); }
	| expr '/' expr { $$ = IntValue::op($1, '/', $3); }
	| '-' expr %prec NEG { $$ = IntValue::op($2, NEG); }
	| expr '^' expr { $$ = IntValue::op($1, '^', $3); }
	| '(' expr ')' { $$ = $2; }
	| expr '<' expr { $$ = IntValue::op($1, '<', $3); }
	| expr '>' expr { $$ = IntValue::op($1, '>', $3); }
	| expr GE expr { $$ = IntValue::op($1, GE, $3); }
	| expr LE expr { $$ = IntValue::op($1, LE, $3); }
	| expr EQ expr { $$ = IntValue::op($1, EQ, $3); }
	| expr NE expr { $$ = IntValue::op($1, NE, $3); }
	;

assign:
	  INTV WORD ')' '=' expr { $$ = new Variable(string($2), $5); }
	| STRV WORD ')' '=' expr { $$ = new Variable(string($2), $5); }
	;

ivar:
	INTV WORD ')' { $$ = Variable::findByName(string($2)); }
	;

svar:
	STRV WORD ')' { $$ = Variable::findByName(string($2)); }
	;
%%

