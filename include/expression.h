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
#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <boost/any.hpp>
#include "variable.h"

class Function;

typedef enum {
	sNONE,
	sRETURN,
	sBREAK,
	sCONTINUE
} Signal;

class Expression {
	public:	
		Expression();
		~Expression();
		
		virtual Value* evaluate() = 0;
		virtual void doExp() = 0;
		
		void doThings() { if(scopes == 0) doExp(); }
		void doThings(bool inScope) { if(scopes - 1 == 0) doExp(); }
		
		static int scopes;
		static Signal signal;
		
		bool hexMode() const { return m_mode == modeHex; }
		static void setHexMode(Expression *exp, bool h) { exp->m_mode = (h) ? modeHex : noMode; }
		
		bool sciMode() const { return m_mode == modeSci; }
		static void setSciMode(Expression *exp, bool s) { exp->m_mode = (s) ? modeSci : noMode; }
		
		Mode mode() const { return m_mode; }
		void mode(Mode m) { m_mode = m; }
		
		std::string type() const { return m_type; }
		void type(std::string t) { m_type = t; }
		
		int line() const { return m_line; }
		void line(int l) { m_line = l; }
		
	protected:
		Mode m_mode;
		int m_line;
		std::string m_type;
};

class IntExpression : public Expression {
	public:
		IntExpression(int value);
		~IntExpression();
		
		Value* evaluate() { return new Value(m_value); }
		void doExp() {}
	
	private:
		int m_value;
};

class FloatExpression : public Expression {
	public:
		FloatExpression(double value);
		~FloatExpression();
		
		Value* evaluate() { return new Value(m_value); }
		void doExp() {}
	
	private:
		double m_value;
};

class StrExpression : public Expression {
	public:
		StrExpression(std::string str);
		~StrExpression();
		
		Value* evaluate() { return new Value(m_str); }
		void doExp() {}
		
	private:
		std::string m_str;
};

class BoolExpression : public Expression {
	public:
		BoolExpression(bool value);
		~BoolExpression();
		
		Value* evaluate() { return new Value(m_value); }
		void doExp() {}
		
	private:
		bool m_value;
};

class ArrayExpression : public Expression {
	public:
		ArrayExpression(std::vector<Expression*> *elements);
		~ArrayExpression();

		Value *evaluate();
		void doExp() {}
		
	private:
		std::vector<Expression*> *m_elements;
};

class ElementExpression : public Expression {
	public:
		ElementExpression(std::string arrayName, std::vector<std::string> *index);
		~ElementExpression();

		Value *evaluate();
		void doExp() {}
		
		std::string arrayName() const { return m_arrayName; }
		std::vector<std::string>* index() { return m_index; }
		
	private:
		std::string m_arrayName;
		std::vector<std::string> *m_index;
};

class OpExpression : public Expression {
	public:
		OpExpression(Expression *exp1, int oper, Expression *exp2 = 0);
		~OpExpression();
		
		Value* evaluate();
		void doExp() {}
		
	private:
		int m_oper;
		Expression *m_exp1;
		Expression *m_exp2;
};

class VarExpression : public Expression {
	public:
		VarExpression(std::string varName);
		~VarExpression();
		
		Value* evaluate() { doExp(); return m_var->value(); }
		void doExp() { m_var = Variable::findByName(m_varName); }
		
		std::string varName() { return m_varName; }
		
		virtual Variable* getVar() { doExp(); return m_var; }
		
	private:
		std::string m_varName;
		Variable *m_var;
};

class AssignExpression : public Expression {
	public:
		AssignExpression(std::string varName, Expression *valExp, int op = -1);
		AssignExpression(ElementExpression *element, Expression *valExp, int op = -1);
		~AssignExpression();
		
		Value* evaluate();
		void doExp();
		
		virtual Variable* getVar() { doExp(); return m_var; }
		
	private:
		std::string m_varName;
		Variable *m_var;
		Expression *m_valExp;
		int m_op;
		ElementExpression *m_element;
};

class CrExpression : public Expression {
	public:
		CrExpression(Expression *varExp, int op, bool after = false);
		~CrExpression();
		
		Value* evaluate();
		void doExp();
		
	private:
		VarExpression *m_varExp;
		int m_op;
		bool m_after;
		Value *m_valA, *m_valB;
};

class IfExpression : public Expression {
	public:
		IfExpression(Expression *ifExp, std::vector<Expression*> *statements, std::vector<Expression*> *elseStatements = 0);
		~IfExpression();
		
		Value* evaluate();
		void doExp();
		
	private:
		Expression *m_ifExp;
		std::vector<Expression*> *m_statements;
		std::vector<Expression*> *m_elseStatements;
};

class FuncExpression : public Expression {
	public:
		FuncExpression(std::string funcName, std::vector<VarExpression*> *args, std::vector<Expression*> *stmts);
		~FuncExpression();
		
		Value* evaluate() { return new Value(); }
		void doExp();
		
	private:
		std::string m_funcName;
		Function *m_func;
		std::vector<VarExpression*> *m_args;
		std::vector<Expression*> *m_stmts;
};

class CallExpression : public Expression {
	public:
		CallExpression(std::string funcName, std::vector<Expression*> *args);
		~CallExpression();
		
		void initFunc();
		Value* evaluate();
		void doExp();
		
	private:
		std::string m_funcName;
		std::vector<Function*> m_funcs;
		std::vector<Expression*> *m_args;
		bool m_init;
};

class ReturnExpression : public Expression {
	public:
		ReturnExpression(Expression *exp);
		~ReturnExpression();
		
		Value* evaluate() { return m_exp->evaluate(); }
		void doExp() { signal = sRETURN; };
		
	private:
		Expression *m_exp;
};

class PrintExpression : public Expression {
	public:
		PrintExpression(Expression *exp);
		~PrintExpression();

		Value* evaluate() { Value *v = m_exp->evaluate(); v->mode(m_exp->mode()); return v; }
		void doExp() { this->evaluate()->print(); }

	private:
		Expression *m_exp;
};

class WhileExpression : public Expression {
	public:
		WhileExpression(Expression *whileExp, std::vector<Expression*> *statements);
		~WhileExpression();
		
		Value* evaluate() { return new Value(); };
		void doExp();
		
	private:
		Expression *m_whileExp;
		std::vector<Expression*> *m_statements;
};

class ForExpression : public Expression {
	public:
		ForExpression(Expression *varExp, std::vector<Expression*> *statements, Expression *toExp, Expression *stepExp = 0);
		~ForExpression();
		
		Value* evaluate() { return new Value(); };
		void doExp();
		
	private:
		AssignExpression *m_varExp;
		Expression *m_toExp;
		Expression *m_stepExp;
		std::vector<Expression*> *m_statements;
};

#endif // EXPRESSION_H
