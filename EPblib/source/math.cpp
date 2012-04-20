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

typedef enum {
	mCOS,
	mSIN,
	mTAN,
	mACOS,
	mASIN,
	mATAN,
	mATAN2,
	mCOSH,
	mSINH,
	mTANH,
	mSQRT,
	mEXP,
	mFREXP,
	mLDEXP,
	mLOG,
	mLOG10,
	mCEIL,
	mFLOOR,
	mFABS,
	mFMOD
} MathFunc;

class MathExpression : public Expression {
	public:
		MathExpression(Expression *exp, MathFunc func) { m_exp = exp; m_exp2 = 0; m_func = func; }
		MathExpression(Expression *exp, Expression *exp2, MathFunc func) { m_exp = exp; m_exp2 = exp2; m_func = func; }
		~MathExpression() { delete m_exp; if(m_exp2) delete m_exp2; }
		
		Value *evaluate() {
			switch(m_func) {
				case mCOS:
					return new Value(cos(getNumVal(m_exp->evaluate())));
				case mSIN:
					return new Value(sin(getNumVal(m_exp->evaluate())));
				case mTAN:
					return new Value(tan(getNumVal(m_exp->evaluate())));
				case mACOS:
					return new Value(acos(getNumVal(m_exp->evaluate())));
				case mASIN:
					return new Value(asin(getNumVal(m_exp->evaluate())));
				case mATAN:
					return new Value(atan(getNumVal(m_exp->evaluate())));
				case mATAN2:
					if(!m_exp2) yyerror("Unexpected error");
					return new Value(atan2(getNumVal(m_exp->evaluate()), getNumVal(m_exp2->evaluate())));
				case mCOSH:
					return new Value(cosh(getNumVal(m_exp->evaluate())));
				case mSINH:
					return new Value(sinh(getNumVal(m_exp->evaluate())));
				case mTANH:
					return new Value(tanh(getNumVal(m_exp->evaluate())));
				case mSQRT:
					return new Value(sqrt(getNumVal(m_exp->evaluate())));
				case mEXP:
					return new Value(exp(getNumVal(m_exp->evaluate())));
//				case mFREXP:
//					return new Value(exp(getNumVal(m_exp->evaluate())));
				case mLDEXP:
					if(!m_exp2) yyerror("Unexpected error");
					return new Value(ldexp(getNumVal(m_exp->evaluate()), getNumVal(m_exp2->evaluate())));
				case mLOG:
					return new Value(log(getNumVal(m_exp->evaluate())));
				case mLOG10:
					return new Value(log10(getNumVal(m_exp->evaluate())));
//				case mMODF:
//					return new Value(modf(getNumVal(m_exp->evaluate())));
				case mCEIL:
					return new Value(ceil(getNumVal(m_exp->evaluate())));
				case mFLOOR:
					return new Value(floor(getNumVal(m_exp->evaluate())));
				case mFABS:
					return new Value(fabs(getNumVal(m_exp->evaluate())));
				case mFMOD:
					return new Value(fmod(getNumVal(m_exp->evaluate()), getNumVal(m_exp2->evaluate())));
				default:
					yyerror("Unexpected error");
			}
		}
		void doExp() {}
		
	private:
		Expression *m_exp;
		Expression *m_exp2;
		MathFunc m_func;
};

void EPblib_initMath() {
	/*map<string, Value*> Math_elements;
	
	// Math.cos
	beginScope(stFUNC);
	vector<VarExpression*> *cosArgs = new vector<VarExpression*>;
	cosArgs->push_back(new VarExpression("x"));
	vector<Expression*> *cosStmts = new vector<Expression*>;
	cosStmts->push_back(new ReturnExpression(new MathExpression((*cosArgs)[0], mCOS)));
	endScope();
	Value *cos = new Value(new Function(cosArgs, cosStmts));
	
	// Math.sin
	beginScope(stFUNC);
	vector<VarExpression*> *sinArgs = new vector<VarExpression*>;
	sinArgs->push_back(new VarExpression("x"));
	vector<Expression*> *sinStmts = new vector<Expression*>;
	sinStmts->push_back(new ReturnExpression(new MathExpression((*sinArgs)[0], mSIN)));
	endScope();
	Value *sin = new Value(new Function(sinArgs, sinStmts));
	
	// Math.tan
	beginScope(stFUNC);
	vector<VarExpression*> *tanArgs = new vector<VarExpression*>;
	tanArgs->push_back(new VarExpression("x"));
	vector<Expression*> *tanStmts = new vector<Expression*>;
	tanStmts->push_back(new ReturnExpression(new MathExpression((*tanArgs)[0], mTAN)));
	endScope();
	Value *tan = new Value(new Function(tanArgs, tanStmts));
	
	// Math.acos
	beginScope(stFUNC);
	vector<VarExpression*> *acosArgs = new vector<VarExpression*>;
	acosArgs->push_back(new VarExpression("x"));
	vector<Expression*> *acosStmts = new vector<Expression*>;
	acosStmts->push_back(new ReturnExpression(new MathExpression((*acosArgs)[0], mACOS)));
	endScope();
	Value *acos = new Value(new Function(acosArgs, acosStmts));
	
	// Math.cos
	beginScope(stFUNC);
	vector<VarExpression*> *cosArgs = new vector<VarExpression*>;
	cosArgs->push_back(new VarExpression("x"));
	vector<Expression*> *cosStmts = new vector<Expression*>;
	cosStmts->push_back(new ReturnExpression(new MathExpression((*cosArgs)[0], mCOS)));
	endScope();
	Value *cos = new Value(new Function(cosArgs, cosStmts));
	
	// Math.cos
	beginScope(stFUNC);
	vector<VarExpression*> *cosArgs = new vector<VarExpression*>;
	cosArgs->push_back(new VarExpression("x"));
	vector<Expression*> *cosStmts = new vector<Expression*>;
	cosStmts->push_back(new ReturnExpression(new MathExpression((*cosArgs)[0], mCOS)));
	endScope();
	Value *cos = new Value(new Function(cosArgs, cosStmts));
	
	// Math.cos
	beginScope(stFUNC);
	vector<VarExpression*> *cosArgs = new vector<VarExpression*>;
	cosArgs->push_back(new VarExpression("x"));
	vector<Expression*> *cosStmts = new vector<Expression*>;
	cosStmts->push_back(new ReturnExpression(new MathExpression((*cosArgs)[0], mCOS)));
	endScope();
	Value *cos = new Value(new Function(cosArgs, cosStmts));
	
	// Math.cos
	beginScope(stFUNC);
	vector<VarExpression*> *cosArgs = new vector<VarExpression*>;
	cosArgs->push_back(new VarExpression("x"));
	vector<Expression*> *cosStmts = new vector<Expression*>;
	cosStmts->push_back(new ReturnExpression(new MathExpression((*cosArgs)[0], mCOS)));
	endScope();
	Value *cos = new Value(new Function(cosArgs, cosStmts));
	
	// Math.cos
	beginScope(stFUNC);
	vector<VarExpression*> *cosArgs = new vector<VarExpression*>;
	cosArgs->push_back(new VarExpression("x"));
	vector<Expression*> *cosStmts = new vector<Expression*>;
	cosStmts->push_back(new ReturnExpression(new MathExpression((*cosArgs)[0], mCOS)));
	endScope();
	Value *cos = new Value(new Function(cosArgs, cosStmts));
	
	Math_elements.insert(Math_elements.end(), pair<string, Value*>("cos", cos));
	Math_elements.insert(Math_elements.end(), pair<string, Value*>("sin", sin));
	Math_elements.insert(Math_elements.end(), pair<string, Value*>("tan", tan));
	Math_elements.insert(Math_elements.end(), pair<string, Value*>("acos", acos));
	Math_elements.insert(Math_elements.end(), pair<string, Value*>("asin", asin));
	Math_elements.insert(Math_elements.end(), pair<string, Value*>("atan", atan));
	Math_elements.insert(Math_elements.end(), pair<string, Value*>("atan2", atan2));
	Math_elements.insert(Math_elements.end(), pair<string, Value*>("cosh", cosh));
	Math_elements.insert(Math_elements.end(), pair<string, Value*>("sinh", sinh));
	Math_elements.insert(Math_elements.end(), pair<string, Value*>("tanh", tanh));
	Math_elements.insert(Math_elements.end(), pair<string, Value*>("sqrt", sqrt));
	Math_elements.insert(Math_elements.end(), pair<string, Value*>("exp", exp));
	Math_elements.insert(Math_elements.end(), pair<string, Value*>("frexp", frexp));
	Math_elements.insert(Math_elements.end(), pair<string, Value*>("ldexp", ldexp));
	Math_elements.insert(Math_elements.end(), pair<string, Value*>("log", log));
	Math_elements.insert(Math_elements.end(), pair<string, Value*>("log10", log10));
	Math_elements.insert(Math_elements.end(), pair<string, Value*>("ceil", ceil));
	Math_elements.insert(Math_elements.end(), pair<string, Value*>("floor", floor));
	Math_elements.insert(Math_elements.end(), pair<string, Value*>("fabs", fabs));
	Math_elements.insert(Math_elements.end(), pair<string, Value*>("fmod", fmod));
	Math_elements.insert(Math_elements.end(), pair<string, Value*>("pi", new Value(PI)));
	
	Variable *Math = new Variable("Math", new Value(Math_elements));
	*/
}

