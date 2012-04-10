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

using namespace std;

extern FILE* yyin;
string filename;

void error(string str, const char* file, unsigned int line) {
	cerr << "\33[0;31;01m" << filename << ":" << yylineno << ": Error: " << str << "\33[0m" << endl;
#ifdef EPLITH_DEBUG
	edbg(file << ":" << line);
#endif
	exit(1);
}

void warn(string str, const char* file, unsigned int line) {
	cerr << "\33[0;33;01m" << filename << ":" << yylineno << ": Warning: " << str << "\33[0m" << endl;
#ifdef EPLITH_DEBUG
	edbg(file << ":" << line);
#endif
}

extern "C" {
int yywrap() {
	return 1;
} 
}

int main(int argc, char* argv[]) {
	filename = "Eplith";
	
	if(argc < 2) {
		yyerror("Filename not given");
	}
	else if(argc > 2) {
		yyerror("Too much arguments");
	}
	
#ifdef TYPES_DEBUG
	edbg("Types: Int: " << typeInt << " | Float: " << typeFloat << " | String: " << typeStr << " | Array: " << typeArray << " | Function: " << typeFunc << " | Void: " << typeVoid);
#endif
	
	newFile(argv[1]);
	
	filename = argv[1];
	
	Variable::initNullVar();
	
	yyparse();
	
	return 0;
} 

%}

%union {
	int iValue;
	double fValue;
	char* sValue;
	std::string *str;
	Expression *exp;
	std::vector<Expression*> *list;
	std::vector<VarExpression*> *varList;
	std::vector<std::string> *elementIndex;
	int op;
	ElementExpression *element;
	std::multimap<std::string, Expression*> *elemList;
	std::pair<std::string, Expression*> *elem;
}

%token <iValue> INTEGER
%token <iValue> HEX_INT
%token <fValue> FLOAT
%token <sValue> NAME
%token <sValue> STRING

%token DEC HEX
%token OCT BIN
%token SCI

%token ADD SUB
%token MUL DIV
%token EXP MOD

%token <op> ASSIGN_OP

%token DEBUG
%token vNULL

%token BREAK CONTINUE
%token RETURN

%token STRA
%token EQI EQS

%token INCR_A DECR_A INCR_B DECR_B

%token DO WHILE IF PRINT FOR TO
%token TRUE FALSE
%token FUNCTION
%nonassoc IFX
%nonassoc ELSE

	/* See Cp19 for others */
%left '=' BAND_EQ BOR_EQ XOR_EQ
%left OR
%left AND
%left BOR
%left XOR
%left BAND
%left EQ NE
%left '<' LE '>' GE
%left LSHIFT RSHIFT
%left '+' '-'
%left '*' '/' '%'
%left '^'
%right NOT BNOT NEG POS INCR DECR

%start program

%type <exp> call exp var assign assignExpVal stmt cast integer func ufunc
%type <list> exp_list stmt_list stmts
%type <varList> var_list
%type <elementIndex> element_index
%type <element> element
%type <str> member index
%type <elemList> elem_list
%type <elem> elem

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
	| exp ';' { $$ = $1; if($$->type() == "CallExpression") $1->doThings(); }
	| BREAK ';' { $$ = new SignalExpression(sBREAK); }
	| CONTINUE ';' { $$ = new SignalExpression(sCONTINUE); }
	| RETURN exp ';' { $$ = new ReturnExpression($2); }
	| PRINT '(' exp ')' ';' { $$ = new PrintExpression($3); }
	| WHILE '(' exp ')' stmts { $$ = new WhileExpression($3, $5); }
	| DO stmts WHILE '(' exp ')' ';' { $$ = new WhileExpression($5, $2); }
	| IF '(' exp ')' stmts %prec IFX { $$ = new IfExpression($3, $5); }
	| IF '(' exp ')' stmts ELSE stmts { $$ = new IfExpression($3, $5, $7); }
	| FOR '(' assign TO exp ';' exp ')' stmts { $$ = new ForExpression($3, $9, $5, $7);  }
	| FOR '(' assign TO exp ')' stmts { $$ = new ForExpression($3, $7, $5);  }
	| func { $$ = $1; }
	;

func:
	FUNCTION NAME '(' var_list ')' stmts { $$ = new FuncExpression(string($2), $4, $6); }
	;

ufunc:
	FUNCTION '(' var_list ')' stmts { $$ = new FuncExpression("<<unamed>>", $3, $5); }
	;

stmts:
	stmt { vector<Expression*> *v = new vector<Expression*>;
		   v->push_back($1);
		   $$ = v; }
	| '{' stmt_list '}' { $$ = $2; }
	| '{' '}' { vector<Expression*> *v = new vector<Expression*>;
				$$ = v; }
	;

stmt_list:
	  stmt { vector<Expression*> *v = new vector<Expression*>;
			 v->push_back($1);
			 $$ = v; }
	| stmt_list stmt { vector<Expression*> *v = $1;
						   v->push_back($2);
						   $$ = v; }
	;

var_list:
	/* void */ { vector<VarExpression*> *v = new vector<VarExpression*>;
				 $$ = v; }
	| var { vector<VarExpression*> *v = new vector<VarExpression*>;
			v->push_back((VarExpression*)$1);
			$$ = v; }
	| var_list ',' var { vector<VarExpression*> *v = $1;
						 v->push_back((VarExpression*)$3);
						 $$ = v; }
	;

exp_list:
	/* void */ { vector<Expression*> *v = new vector<Expression*>;
				 $$ = v; }
	| exp { vector<Expression*> *v = new vector<Expression*>;
			v->push_back($1);
			$$ = v; }
	| exp_list ',' exp { vector<Expression*> *v = $1;
						 v->push_back($3);
						 $$ = v; }
	;

elem_list:
	/* void */ { multimap<string, Expression*> *m = new multimap<string, Expression*>;
				 $$ = m; }
	| elem { multimap<string, Expression*> *m = new multimap<string, Expression*>;
			 m->insert(m->end(), *$1);
			 $$ = m; }
	| elem_list ',' elem { multimap<string, Expression*> *m = $1;
						   m->insert(m->end(), *$3);
						   $$ = m; }
	;

elem:
	  exp { $$ = new pair<string, Expression*>("<<nothing>>", $1); }
	| NAME '=' exp { $$ = new pair<string, Expression*>(string($1), $3); }
	;

exp:
	  INTEGER { $$ = new IntExpression($1); }
	| HEX_INT { $$ = new IntExpression($1); }
	| FLOAT { $$ = new FloatExpression($1); }
	| STRING { $$ = new StrExpression($1); }
	| TRUE { $$ = new BoolExpression(true); }
	| FALSE { $$ = new BoolExpression(false); }
	| var { $$ = $1; }
	| INCR var { $$ = new CrExpression($2, INCR); }
	| DECR var { $$ = new CrExpression($2, DECR); }
	| var INCR { $$ = new CrExpression($1, INCR, true); }
	| var DECR { $$ = new CrExpression($1, DECR, true); }
	| cast { $$ = $1; }
	| exp '+' exp { $$ = new OpExpression($1, '+', $3); }
	| exp '-' exp { $$ = new OpExpression($1, '-', $3); }
	| exp '*' exp { $$ = new OpExpression($1, '*', $3); }
	| exp '/' exp { $$ = new OpExpression($1, '/', $3); }
	| '-' exp %prec NEG { $$ = new OpExpression($2, NEG); }
	| '+' exp %prec POS { $$ = new OpExpression($2, POS); }
	| exp '^' exp { $$ = new OpExpression($1, '^', $3); }
	| exp '%' exp { $$ = new OpExpression($1, '%', $3); }
	| exp '<' exp { $$ = new OpExpression($1, '<', $3); }
	| exp '>' exp { $$ = new OpExpression($1, '>', $3); }
	| NOT exp { $$ = new OpExpression($2, NOT); }
	| exp AND exp { $$ = new OpExpression($1, AND, $3); }
	| exp OR exp { $$ = new OpExpression($1, OR, $3); }
	| exp BAND exp { $$ = new OpExpression($1, BAND, $3); }
	| exp BOR exp { $$ = new OpExpression($1, BOR, $3); }
	| exp XOR exp { $$ = new OpExpression($1, XOR, $3); }
	| BNOT exp { $$ = new OpExpression($2, BNOT); }
	| exp LSHIFT exp { $$ = new OpExpression($1, LSHIFT, $3); }
	| exp RSHIFT exp { $$ = new OpExpression($1, RSHIFT, $3); }
	| exp GE exp { $$ = new OpExpression($1, GE, $3); }
	| exp LE exp { $$ = new OpExpression($1, LE, $3); }
	| exp EQ exp { $$ = new OpExpression($1, EQ, $3); }
	| exp NE exp { $$ = new OpExpression($1, NE, $3); }
	| '(' exp ')' { $$ = $2; }
	| call { $$ = $1; }
	| '{' elem_list '}' { $$ = new ArrayExpression($2); }
	| element { $$ = $1; }
	| ufunc { $$ = $1; }
	;

call:
	  NAME '(' exp_list ')' { $$ = new CallExpression(string($1), $3); }
	| element '(' exp_list ')' { $$ = new CallExpression($1, $3); }
	;

element:
	NAME element_index { $$ = new ElementExpression(string($1), $2); }
	;

element_index:
	element_index member { $1->insert($1->begin(), *$2); $$ = $1; }
	| member { vector<string> *v = new vector<string>;
					  v->insert(v->begin(), *$1);
					  $$ = v; }
	;

member:
	  '[' index ']' { $$ = new string(*$2); }
	| '.' NAME { $$ = new string($2); }
	| ':' NAME { $$ = new string(string("*") + $2); }

index:
	INTEGER { $$ = new string(itos($1)); }
	| STRING { $$ = new string($1); }
	| var { VarExpression *v = (VarExpression*)$1;
			if(isIndex(v->evaluate())) $$ = new string(getIndexVal(v->evaluate())); else yyerror("Requires an integer or string expression here."); }
	;

cast:
	  '(' HEX ')' integer { Expression::setHexMode($4, true); $$ = $4; }
	| '(' SCI ')' integer { Expression::setSciMode($4, true); $$ = $4; }
	;

integer:
	INTEGER { $$ = new IntExpression($1); }
	| HEX_INT { $$ = new IntExpression($1); }
	| FLOAT { $$ = new FloatExpression($1); }
	| var { VarExpression *v = (VarExpression*)$1;
			if(isNum(v->evaluate())) $$ = v; else yyerror("Requires a numeric expression here."); }
	;

assign:
	NAME '=' assignExpVal { $$ = new AssignExpression(string($1), $3); }
	| NAME ASSIGN_OP assignExpVal { $$ = new AssignExpression(string($1), $3, $2); }
	| element '=' assignExpVal { $$ = new AssignExpression($1, $3); }
	;

assignExpVal:
	exp { $$ = $1; }
	| assign { $$ = $1; }
	;

var:
	  NAME { $$ = new VarExpression(string($1)); }
	;
%%

