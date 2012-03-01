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
	std::vector<Expression*> *list;
}

%token <iValue> INTEGER
%token <sValue> NAME
%token <sValue> STRING

%token DEBUG
%token vNULL

%token STRA
%token EQI EQS

%token DO WHILE IF PRINT FOR TO
%token TRUE FALSE
%token FUNCTION
%token END
%nonassoc IFX
%nonassoc ELSE

%left GE LE EQ NE '>' '<'
%left '+' '-'
%left '*' '/' '%'
%left NEG
%left NOT
%right '^'
%right '='

%start program

%type <exp> exp var assign assignExpVal stmt
%type <list> exp_list stmt_list stmts

%%
program:
	instruction { ; }
	;

instruction: /* empty */
	| instruction stmt { ; }
	;

stmt:
	  ';' { ; }
	| assign ';' { ; }
	| PRINT '(' exp ')' ';' { $$ = new PrintExpression($3); }
	| WHILE '(' exp ')' stmts { $$ = new WhileExpression($3, $5); }
	| DO stmts WHILE '(' exp ')' ';' { $$ = new WhileExpression($5, $2); }
	| IF '(' exp ')' stmts %prec IFX { $$ = new IfExpression($3, $5); }
	| IF '(' exp ')' stmts ELSE stmts { $$ = new IfExpression($3, $5, $7); }
	| FOR '(' assign TO exp ';' exp ')' stmts { $$ = new ForExpression($3, $9, $5, $7);  }
	| FOR '(' assign TO exp ')' stmts { $$ = new ForExpression($3, $7, $5);  }
	| NAME '(' exp_list ')' ';' { $$ = new CallExpression(string($1), $3); }
	;

stmts:
	  stmt { vector<Expression*> *v = new vector<Expression*>;
			 v->push_back($1);
			 $$ = v; }
	| '{' stmt_list '}' { $$ = $2; }

stmt_list:
	  stmt { vector<Expression*> *v = new vector<Expression*>;
			 v->push_back($1);
			 $$ = v; }
	| stmt_list stmt { vector<Expression*> *v = $1;
						   v->push_back($2);
						   $$ = v; }
	;

exp_list:
	  exp { vector<Expression*> *v = new vector<Expression*>;
			v->push_back($1);
			$$ = v; }
	| exp_list ',' exp { vector<Expression*> *v = $1;
						 v->push_back($3);
						 $$ = v; }

exp:
	  INTEGER { $$ = new IntExpression($1); }
	| STRING { $$ = new StrExpression($1); }
	| TRUE { $$ = new BoolExpression(true); }
	| FALSE { $$ = new BoolExpression(false); }
	| var { $$ = $1; }
	| exp '+' exp { $$ = new OpExpression($1, '+', $3); }
	| exp '-' exp { $$ = new OpExpression($1, '-', $3); }
	| exp '*' exp { $$ = new OpExpression($1, '*', $3); }
	| exp '/' exp { $$ = new OpExpression($1, '/', $3); }
	| '-' exp %prec NEG { $$ = new OpExpression($2, NEG); }
	| '!' exp %prec NOT { $$ = new OpExpression($2, NOT); }
	| exp '^' exp { $$ = new OpExpression($1, '^', $3); }
	| exp '%' exp { $$ = new OpExpression($1, '%', $3); }
	| '(' exp ')' { $$ = $2; }
	| exp '<' exp { $$ = new OpExpression($1, '<', $3); }
	| exp '>' exp { $$ = new OpExpression($1, '>', $3); }
	| exp GE exp { $$ = new OpExpression($1, GE, $3); }
	| exp LE exp { $$ = new OpExpression($1, LE, $3); }
	| exp EQ exp { $$ = new OpExpression($1, EQ, $3); }
	| exp NE exp { $$ = new OpExpression($1, NE, $3); }
	;

assign:
	NAME '=' assignExpVal { $$ = new AssignExpression(string($1), $3); }
	;

assignExpVal:
	exp { $$ = $1; }
	| assign { $$ = $1; }
	;

var:
	NAME { $$ = new VarExpression(string($1)); }
	;
%%


