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
#include "EPb_base.h"

using namespace std;

template<class T>
class IOExpression : public Expression {
	public:
		IOExpression(Expression *exp = 0) { if(!exp) m_str = new char; m_exp = exp; }
		~IOExpression() {}
		
		Value *evaluate() {	return T::eval(m_str); }
		void doExp() { T::exec(m_exp); }
		
	private:
		char *m_str;
		Expression *m_exp;
};

struct Print {
	static vector<VarExpression*> *args;
	static vector<Expression*> *stmts;
	static inline void exec(Expression *exp) { exp->evaluate()->print(); }
	static inline Value *eval(...) { return new Value(); }
	static void init() {
		beginScope(stFUNC);
		args = EPblib_args(1, "str");
		stmts = EPblib_stmts(1, new IOExpression<Print>((*args)[0]));
		endScope();
	}
};

struct Puts {
	static vector<VarExpression*> *args;
	static vector<Expression*> *stmts;
	static inline void exec(Expression *exp) { exp->evaluate()->print(); cout << endl; }
	static inline Value *eval(...) { return new Value(); }
	static void init() {
		beginScope(stFUNC);
		args = EPblib_args(1, "str");
		stmts = EPblib_stmts(1, new IOExpression<Puts>((*args)[0]));
		endScope();
	}
};

struct Gets {
	static vector<VarExpression*> *args;
	static vector<Expression*> *stmts;
	static inline void exec(...) {}
	static inline Value *eval(char *str) { gets(str); return new Value(str); }
	static void init() {
		beginScope(stFUNC);
		args = EPblib_args(0);
		stmts = EPblib_stmts(1, new ReturnExpression(new IOExpression<Gets>));
		endScope();
	}
};

struct TypeF {
	static vector<VarExpression*> *args;
	static vector<Expression*> *stmts;
	static inline void exec(...) {}
	static inline Value *eval(...) { return new Value((*args)[0]->evaluate()->typeToStr()); }
	static void init() {
		beginScope(stFUNC);
		args = EPblib_args(1, "var");
		stmts = EPblib_stmts(1, new ReturnExpression(new IOExpression<TypeF>));
		endScope();
	}
};

struct ToNumber {
	static vector<VarExpression*> *args;
	static vector<Expression*> *stmts;
	static inline void exec(...) {}
	static inline Value *eval(...) { return (*args)[0]->evaluate()->toNum(); }
	static void init() {
		beginScope(stFUNC);
		args = EPblib_args(1, "var");
		stmts = EPblib_stmts(1, new ReturnExpression(new IOExpression<ToNumber>));
		endScope();
	}
};

struct ToString {
	static vector<VarExpression*> *args;
	static vector<Expression*> *stmts;
	static inline void exec(...) {}
	static inline Value *eval(...) { return (*args)[0]->evaluate()->toStr(); }
	static void init() {
		beginScope(stFUNC);
		args = EPblib_args(1, "var");
		stmts = EPblib_stmts(1, new ReturnExpression(new IOExpression<ToString>));
		endScope();
	}
};

EPb_initSSM(Print);
EPb_initSSM(Puts);
EPb_initSSM(Gets);
EPb_initSSM(TypeF);
EPb_initSSM(ToNumber);
EPb_initSSM(ToString);

void EPblib_initBase() {
	EPb_initFunc(Print, print);
	EPb_initFunc(Puts, puts);
	EPb_initFunc(Gets, gets);
	EPb_initFunc(TypeF, type);
	EPb_initFunc(ToNumber, tonumber);
	EPb_initFunc(ToString, tostring);
	
	Variable *version = new Variable("_VERSION", new Value("0.1a"));
}

