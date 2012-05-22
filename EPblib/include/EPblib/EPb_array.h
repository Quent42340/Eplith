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
#ifndef EPBLIB_ARRAY_H
#define EPBLIB_ARRAY_H

#define EPb_initArrayStructS(Struct, val) \
	struct Struct { \
		static vector<VarExpression*> *args; \
		static vector<Expression*> *stmts; \
		static inline void exec(...) {} \
		static inline Value *eval(Expression *exp, ...) {
			return new Value(val);
		} \
		static void init() { \
			beginScope(stFUNC); \
			args = EPblib_args(1, "t"); \
			stmts = EPblib_stmts(1, new ReturnExpression(new ArrayFExpression<Struct>((*args)[0]))); \
			endScope(); \
		} \
	}; \
	EPb_initSSM(Struct);

#define EPb_initArrayStruct(Struct, val) \
	struct Struct { \
		static vector<VarExpression*> *args; \
		static vector<Expression*> *stmts; \
		static inline void exec(...) {} \
		static inline Value *eval(Expression *exp, ...) { return new Value(val); } \
		static void init() { \
			beginScope(stFUNC); \
			args = EPblib_args(1, "t"); \
			stmts = EPblib_stmts(1, new ReturnExpression(new ArrayFExpression<Struct>((*args)[0]))); \
			endScope(); \
		} \
	}; \
	EPb_initSSM(Struct);

#define EPb_initArrayStruct2(Struct, val) \
	struct Struct { \
		static vector<VarExpression*> *args; \
		static vector<Expression*> *stmts; \
		static inline void exec(...) {} \
		static inline Value *eval(Expression *exp, Expression *exp2) { return new Value(val); } \
		static void init() { \
			beginScope(stFUNC); \
			args = EPblib_args(2, "t", "n"); \
			stmts = EPblib_stmts(1, new ReturnExpression(new ArrayFExpression<Struct>((*args)[0], (*args)[1]))); \
			endScope(); \
		} \
	}; \
	EPb_initSSM(Struct);

#define EPb_initArrayStructA(Struct, action) \
	struct Struct { \
		static vector<VarExpression*> *args; \
		static vector<Expression*> *stmts; \
		static inline void exec(Expression *exp, ...) { action; } \
		static inline Value *eval(Expression *exp, ...) { exec(exp, exp2); return new Value(); } \
		static void init() { \
			beginScope(stFUNC); \
			args = EPblib_args(1, "t"); \
			stmts = EPblib_stmts(1, new ReturnExpression(new ArrayFExpression<Struct>((*args)[0]))); \
			endScope(); \
		} \
	}; \
	EPb_initSSM(Struct);

#define EPb_initArrayStructA2(Struct, action) \
	struct Struct { \
		static vector<VarExpression*> *args; \
		static vector<Expression*> *stmts; \
		static inline void exec(Expression *exp, Expression *exp2) { action; } \
		static inline Value *eval(Expression *exp, Expression *exp2) { exec(exp, exp2); return new Value(); } \
		static void init() { \
			beginScope(stFUNC); \
			args = EPblib_args(2, "t", "n"); \
			stmts = EPblib_stmts(1, new ReturnExpression(new ArrayFExpression<Struct>((*args)[0], (*args)[1]))); \
			endScope(); \
		} \
	}; \
	EPb_initSSM(Struct);

void EPblib_initArray();

#endif // EPBLIB_ARRAY_H
