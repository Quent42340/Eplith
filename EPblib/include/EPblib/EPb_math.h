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
#ifndef EPBLIB_MATH_H
#define EPBLIB_MATH_H

#define initMathStruct0(Struct, name) \
	struct Struct { \
		static vector<VarExpression*> *args; \
		static vector<Expression*> *stmts; \
		static inline Value *eval(...) { return new Value(name()); } \
		static void init() { \
			beginScope(stFUNC); \
			args = EPblib_args(0); \
			stmts = EPblib_stmts(1, new ReturnExpression(new MathExpression<Struct>())); \
			endScope(); \
		} \
	}; \
	initSSM(Struct);

#define initMathStruct(Struct, name) \
	struct Struct { \
		static vector<VarExpression*> *args; \
		static vector<Expression*> *stmts; \
		static inline Value *eval(Expression *exp_, ...) { return new Value(name(getNumVal(exp_->evaluate()))); } \
		static void init() { \
			beginScope(stFUNC); \
			args = EPblib_args(1, "x"); \
			stmts = EPblib_stmts(1, new ReturnExpression(new MathExpression<Struct>((*args)[0]))); \
			endScope(); \
		} \
	}; \
	initSSM(Struct);

#define initMathStruct2(Struct, name) \
	struct Struct { \
		static vector<VarExpression*> *args; \
		static vector<Expression*> *stmts; \
		static inline Value *eval(Expression *exp1, Expression *exp2) { return new Value(name(getNumVal(exp1->evaluate()), getNumVal(exp2->evaluate()))); } \
		static void init() { \
			beginScope(stFUNC); \
			args = EPblib_args(2, "m", "n"); \
			stmts = EPblib_stmts(1, new ReturnExpression(new MathExpression<Struct>((*args)[0], (*args)[1]))); \
			endScope(); \
		} \
	}; \
	initSSM(Struct);

void EPblib_initMath();

#endif // EPBLIB_MATH_H
