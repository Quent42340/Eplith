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
#include "EPb_tools.h"
#include "EPb_math.h"

#undef PI
#define PI (3.14159265358979323846)
#define RADIANS_PER_DEGREE (PI/180.0)

using namespace std;

template<class T>
class MathExpression : public Expression {
	public:
		MathExpression() { m_exp = 0; m_exp2 = 0; }
		MathExpression(Expression *exp) { m_exp = exp; m_exp2 = 0; }
		MathExpression(Expression *exp, Expression *exp2) { m_exp = exp; m_exp2 = exp2; }
		~MathExpression() { delete m_exp; if(m_exp2) delete m_exp2; }
		
		Value *evaluate() { return T::eval(m_exp, m_exp2); }
		void doExp() {}
		
	private:
		Expression *m_exp;
		Expression *m_exp2;
};

double math_deg(double x) { return x/RADIANS_PER_DEGREE; }
double math_rad(double x) { return x*RADIANS_PER_DEGREE; }
double math_rand() { return rand() % RAND_MAX / RAND_MAX; }
double math_randomseed(double x) { srand(x); rand(); }

initMathStruct(Cos, cos);
initMathStruct(Sin, sin);
initMathStruct(Tan, tan);
initMathStruct(aCos, acos);
initMathStruct(aSin, asin);
initMathStruct(aTan, atan);
initMathStruct2(aTan2, atan2);
initMathStruct(Cosh, cosh);
initMathStruct(Sinh, sinh);
initMathStruct(Tanh, tanh);
initMathStruct(Sqrt, sqrt);
initMathStruct(Exp, exp);
initMathStruct2(Ldexp, ldexp);
initMathStruct(Log, log);
initMathStruct(Log10, log10);
initMathStruct(Ceil, ceil);
initMathStruct(Floor, floor);
initMathStruct(Abs, fabs);
initMathStruct2(Fmod, fmod);
initMathStruct(Deg, math_deg);
initMathStruct(Rad, math_rad);
initMathStruct0(Rand, math_rand);
initMathStruct(Randomseed, math_randomseed);

void EPblib_initMath() {
	map<string, Value*> Math_elements;
	
	initElemFunc(Math_elements, Cos, cos);
	initElemFunc(Math_elements, Sin, sin);
	initElemFunc(Math_elements, Tan, tan);
	initElemFunc(Math_elements, aCos, acos);
	initElemFunc(Math_elements, aSin, asin);
	initElemFunc(Math_elements, aTan, atan);
	initElemFunc(Math_elements, aTan2, atan2);
	initElemFunc(Math_elements, Cosh, cosh);
	initElemFunc(Math_elements, Sinh, sinh);
	initElemFunc(Math_elements, Tanh, tanh);
	initElemFunc(Math_elements, Sqrt, sqrt);
	initElemFunc(Math_elements, Exp, exp);
	initElemFunc(Math_elements, Ldexp, ldexp);
	initElemFunc(Math_elements, Log, log);
	initElemFunc(Math_elements, Log10, log10);
	initElemFunc(Math_elements, Ceil, ceil);
	initElemFunc(Math_elements, Floor, floor);
	initElemFunc(Math_elements, Abs, abs);
	initElemFunc(Math_elements, Fmod, fmod);
	initElemFunc(Math_elements, Deg, deg);
	initElemFunc(Math_elements, Rad, rad);
	initElemFunc(Math_elements, Rand, rand);
	initElemFunc(Math_elements, Randomseed, randomseed);
	
	Math_elements.insert(Math_elements.end(), pair<string, Value*>("pi", new Value(PI)));
	
	Variable *Math = new Variable("Math", new Value(Math_elements));
}

