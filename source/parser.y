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
}

%token <iValue> INTEGER
%token <sValue> WORD
%token <sValue> STRING
%token DEBUG
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

%type <iValue> exp
%type <sValue> str
%type <var> ivar svar assign

%%
program:/* empty */
		| program instr
		;

instr:	';'
		| DEBUG { debug = true; }
		| exp ';' { printf("\t%d\n", $1); }
		| assign ';' {
			if(debug) {
				if(!strcmp(var_type($1), VAR_INT)) {
					printf("Var name: %s | Value: %d | Nb of vars: %d\n", var_name($1), var_iValue($1), sizeVars);
				}
				else if(!strcmp(var_type($1), VAR_CHAR)) {
					printf("Var name: %s | Value: %s | Nb of vars: %d\n", var_name($1), var_sValue($1), sizeVars);
				} else {
					yyerror("Unexpected type");
				}
			}
		}
		;

exp:	INTEGER { $$ = $1; }
		| ivar { $$ = var_iValue($1); }
		| exp '+' exp { $$ = $1 + $3; }
		| exp '-' exp { $$ = $1 - $3; }
		| exp '*' exp { $$ = $1 * $3; }
		| exp '/' exp { $$ = $1 / $3; }
		| '-' exp %prec NEG { $$ = -$2; }
		| exp '^' exp { $$ = pow($1, $3); }
		| '(' exp ')' { $$ = $2; }
		| exp '<' exp { $$ = ($1 < $3); }
		| exp '>' exp { $$ = ($1 > $3); }
		| exp GE exp { $$ = ($1 >= $3); }
		| exp LE exp { $$ = ($1 <= $3); }
		| exp EQ exp { $$ = ($1 == $3); }
		| exp NE exp { $$ = ($1 != $3); }
		;

str:	STRING { $$ = $1; }
		| svar { $$ = var_sValue($1); }
		| str '+' str { $$ = strcat($1, $3); }
		;

ivar:	INTV WORD ')' { $$ = var_find($2); }
		;

svar:	STRV WORD ')' { $$ = var_find($2); }
		;

assign:	INTV WORD ')' '=' exp { $$ = iVar($2, $5); }
		| STRV WORD ')' '=' str { $$ = sVar($2, $5); }
		;

%%

