/*---------------------------------------------------------------------------------

    WordSphere Intepreter
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

class Expression {
	public:
		Expression();
		~Expression();
		
		virtual Value* evaluate() = 0;
		virtual void doExp() = 0;
		
		void doThings() { if(scopes == 0) doExp(); }
		void doThings(bool inScope) { if(scopes - 1 == 0) doExp(); }
		
		static int scopes;
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
		
		Value* evaluate() { return m_var->value(); };
		void doExp() {}
		
	private:
		Variable *m_var;
};

class AssignExpression : public Expression {
	public:
		AssignExpression(std::string varName, Expression *valExp);
		~AssignExpression();
		
		Value* evaluate();
		void doExp();
		
	private:
		std::string m_varName;
		Variable *m_var;
		Expression *m_valExp;
};

class IfExpression : public Expression {
	public:
		IfExpression(Expression *ifExp, std::vector<Expression*> *statements);
		IfExpression(Expression *ifExp, std::vector<Expression*> *statements, std::vector<Expression*> *elseStatements);
		~IfExpression();
		
		Value* evaluate() { return new Value(); }
		void doExp();
		
	private:
		Expression *m_ifExp;
		std::vector<Expression*> *m_statements;
		std::vector<Expression*> *m_elseStatements;
};

class CallExpression : public Expression {
	public:
		CallExpression(std::string funcName, std::vector<Expression*> *args);
		~CallExpression();
		
		Value* evaluate() { return new Value(); }
		void doExp();
		
	private:
		std::string m_funcName;
		Function *m_func;
		std::vector<Expression*> *m_args;
};

class PrintExpression : public Expression {
	public:
		PrintExpression(Expression *exp);
		~PrintExpression();

		Value* evaluate() { return new Value(m_exp->evaluate()->value<std::string>()); }
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

#endif // EXPRESSION_H
