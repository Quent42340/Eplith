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
#include "EPb_math.h"

#define PI 3.14159265

using namespace std;

class CosExpression : public Expression {
	public:
		CosExpression(Expression *exp) { m_exp = exp; }
		~CosExpression() { delete m_exp; }
		
		Value *evaluate() {	return new Value(cos(getNumVal(m_exp->evaluate()))); }
		void doExp() {}
		
	private:
		Expression *m_exp;
};

void EPblib_initMath() {
	map<string, Value*> Math_elements;
	
	// Math.cos
	beginScope(stFUNC);
	vector<VarExpression*> *cosArgs = new vector<VarExpression*>;
	cosArgs->push_back(new VarExpression("x"));
	vector<Expression*> *cosStmts = new vector<Expression*>;
	cosStmts->push_back(new ReturnExpression(new CosExpression((*cosArgs)[0])));
	endScope();
	Value *cos = new Value(new Function(cosArgs, cosStmts));
	
	Math_elements.insert(Math_elements.end(), pair<string, Value*>("cos", cos));
	Math_elements.insert(Math_elements.end(), pair<string, Value*>("pi", new Value(PI)));
	
	Variable *Math = new Variable("Math", new Value(Math_elements));
}

