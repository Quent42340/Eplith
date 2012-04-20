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
		MathExpression(Expression *exp = 0, Expression *exp2 = 0) { m_exp = exp; m_exp2 = exp2; }
		~MathExpression() { delete m_exp; if(m_exp2) delete m_exp2; }
		
		Value *evaluate() { return T::eval(m_exp, m_exp2); }
		void doExp() {}
		
	private:
		Expression *m_exp;
		Expression *m_exp2;
};

double Math_deg(double x) { return x/RADIANS_PER_DEGREE; }
double Math_rad(double x) { return x*RADIANS_PER_DEGREE; }
double Math_rand() { return rand() % RAND_MAX / RAND_MAX; }
double Math_randomseed(double x) { srand(x); rand(); }

EPb_initMathStruct(Cos, cos);
EPb_initMathStruct(Sin, sin);
EPb_initMathStruct(Tan, tan);
EPb_initMathStruct(aCos, acos);
EPb_initMathStruct(aSin, asin);
EPb_initMathStruct(aTan, atan);
EPb_initMathStruct2(aTan2, atan2);
EPb_initMathStruct(Cosh, cosh);
EPb_initMathStruct(Sinh, sinh);
EPb_initMathStruct(Tanh, tanh);
EPb_initMathStruct(Sqrt, sqrt);
EPb_initMathStruct(Exp, exp);
EPb_initMathStruct2(Ldexp, ldexp);
EPb_initMathStruct(Log, log);
EPb_initMathStruct(Log10, log10);
EPb_initMathStruct(Ceil, ceil);
EPb_initMathStruct(Floor, floor);
EPb_initMathStruct(Abs, fabs);
EPb_initMathStruct2(Fmod, fmod);
EPb_initMathStruct(Deg, Math_deg);
EPb_initMathStruct(Rad, Math_rad);
EPb_initMathStruct0(Rand, Math_rand);
EPb_initMathStruct(Randomseed, Math_randomseed);

void EPblib_initMath() {
	map<string, Value*> Math_elements;
	
	EPb_initElemFunc(Math_elements, Cos, cos);
	EPb_initElemFunc(Math_elements, Sin, sin);
	EPb_initElemFunc(Math_elements, Tan, tan);
	EPb_initElemFunc(Math_elements, aCos, acos);
	EPb_initElemFunc(Math_elements, aSin, asin);
	EPb_initElemFunc(Math_elements, aTan, atan);
	EPb_initElemFunc(Math_elements, aTan2, atan2);
	EPb_initElemFunc(Math_elements, Cosh, cosh);
	EPb_initElemFunc(Math_elements, Sinh, sinh);
	EPb_initElemFunc(Math_elements, Tanh, tanh);
	EPb_initElemFunc(Math_elements, Sqrt, sqrt);
	EPb_initElemFunc(Math_elements, Exp, exp);
	EPb_initElemFunc(Math_elements, Ldexp, ldexp);
	EPb_initElemFunc(Math_elements, Log, log);
	EPb_initElemFunc(Math_elements, Log10, log10);
	EPb_initElemFunc(Math_elements, Ceil, ceil);
	EPb_initElemFunc(Math_elements, Floor, floor);
	EPb_initElemFunc(Math_elements, Abs, abs);
	EPb_initElemFunc(Math_elements, Fmod, fmod);
	EPb_initElemFunc(Math_elements, Deg, deg);
	EPb_initElemFunc(Math_elements, Rad, rad);
	EPb_initElemFunc(Math_elements, Rand, rand);
	EPb_initElemFunc(Math_elements, Randomseed, randomseed);
	
	Math_elements.insert(Math_elements.end(), pair<string, Value*>("pi", new Value(PI)));
	
	Variable *Math = new Variable("Math", new Value(Math_elements));
}

