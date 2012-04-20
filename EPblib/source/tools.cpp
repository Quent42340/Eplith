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
#include <EPlib.h>

using namespace std;

vector<VarExpression*> *EPblib_args(int nbArgs, ...) {
	vector<VarExpression*> *args = new vector<VarExpression*>;
	if(!nbArgs) return args;
	
	va_list vargs;
	va_start(vargs, nbArgs);
	char *s;
	for(int i = 0 ; i < nbArgs ; i++) {
		s = va_arg(vargs, char*);
		args->push_back(new VarExpression(s));
	}
	va_end(vargs);
	
	return args;
}

vector<Expression*> *EPblib_stmts(int nbStmts, ...) {
	vector<Expression*> *stmts = new vector<Expression*>;
	if(!nbStmts) return stmts;
	
	va_list vargs;
	va_start(vargs, nbStmts);
	Expression *exp;
	for(int i = 0 ; i < nbStmts ; i++) {
		exp = va_arg(vargs, Expression*);
		stmts->push_back(exp);
	}
	va_end(vargs);
	
	return stmts;
}

