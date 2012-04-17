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
#include "EPb_io.h"

using namespace std;

class GetsExpression : public Expression {
	public:
		GetsExpression() { m_str = new char; }
		~GetsExpression() {}
		
		void doExp() {
			gets(m_str);
		}
		
		Value *evaluate() {
			doExp();
			return new Value(m_str);
		}
		
	private:
		char *m_str;
};

void EPblib_initIO() {
	// puts
	Expression::scopes++;
	vector<VarExpression*> *putsArgs = new vector<VarExpression*>;
	putsArgs->push_back(new VarExpression("str"));
	vector<Expression*> *putsStmts = new vector<Expression*>;
	putsStmts->push_back(new PrintExpression(new OpExpression((*putsArgs)[0], '+', new StrExpression("\n"))));
	Expression::scopes--;
	Variable *puts = new Variable("puts", new Value(new Function(putsArgs, putsStmts)));
	
	// print
	Expression::scopes++;
	vector<VarExpression*> *printArgs = new vector<VarExpression*>;
	printArgs->push_back(new VarExpression("str"));
	vector<Expression*> *printStmts = new vector<Expression*>;
	printStmts->push_back(new PrintExpression((*printArgs)[0]));
	Expression::scopes--;
	Variable *print = new Variable("print", new Value(new Function(printArgs, printStmts)));
	
	// gets
	beginScope(stFUNC);
	vector<VarExpression*> *getsArgs = new vector<VarExpression*>;
	vector<Expression*> *getsStmts = new vector<Expression*>;
	getsStmts->push_back(new ReturnExpression(new GetsExpression()));
	endScope();
	Variable *gets = new Variable("gets", new Value(new Function(getsArgs, getsStmts)));
}

