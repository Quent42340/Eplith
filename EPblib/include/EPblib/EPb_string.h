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
#ifndef EPBLIB_STRING_H
#define EPBLIB_STRING_H

#define EPb_initStringStruct(Struct, val) \
	struct Struct { \
		static vector<VarExpression*> *args; \
		static vector<Expression*> *stmts; \
		static inline void exec(...) {} \
		static inline Value *eval(Expression *exp, ...) { return new Value(val); } \
		static void init() { \
			beginScope(stFUNC); \
			args = EPblib_args(1, "str"); \
			stmts = EPblib_stmts(1, new ReturnExpression(new StringExpression<Struct>((*args)[0]))); \
			endScope(); \
		} \
	}; \
	EPb_initSSM(Struct);

#define EPb_initStringStruct2(Struct, val) \
	struct Struct { \
		static vector<VarExpression*> *args; \
		static vector<Expression*> *stmts; \
		static inline void exec(...) {} \
		static inline Value *eval(Expression *exp, Expression *exp2) { return new Value(val); } \
		static void init() { \
			beginScope(stFUNC); \
			args = EPblib_args(2, "str", "n"); \
			stmts = EPblib_stmts(1, new ReturnExpression(new StringExpression<Struct>((*args)[0], (*args)[1]))); \
			endScope(); \
		} \
	}; \
	EPb_initSSM(Struct);

void EPblib_initString();

#endif // EPBLIB_STRING_H
