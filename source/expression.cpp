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
#include "header.h"
#include "function.h"
#include "expression.h"
#include "parser.tab.hpp"

using namespace std;

int Expression::scopes = 0;

Expression::Expression() {
}

Expression::~Expression() {
}

IntExpression::IntExpression(int value) {
	m_value = value;
}

IntExpression::~IntExpression() {
}

StrExpression::StrExpression(string str) {
	m_str = str;
}

StrExpression::~StrExpression() {
}

BoolExpression::BoolExpression(bool value) {
	m_value = value;
}

BoolExpression::~BoolExpression() {
}

OpExpression::OpExpression(Expression *exp1, int oper, Expression *exp2) {
	m_exp1 = exp1;
	m_oper = oper;
	m_exp2 = exp2;
}

OpExpression::~OpExpression() {
	delete m_exp1;
	delete m_exp2;
}

Value* OpExpression::evaluate() {
	Value* val = m_exp1->evaluate();
	if(m_oper == NEG) {
		return new Value(-val->value<int>());
	}
	else if(m_oper == NOT) {
		return new Value(!val->value<bool>());
	} else {
		Value* val2 = m_exp2->evaluate();
		if(m_oper == '+') {
			bool pb;
			if((val->type() != typeInt) || (val2->type() != typeInt)) {
				string tmp, tmp2;
				if(val->type() == typeInt) {
					stringstream out;
					if(val->any()->type() == typeid(bool)) {
						pb = *boost::any_cast<bool>(val->any());
						out << ((pb) ? "true" : "false");
						} else {
						out << val->value<int>();
					}
					tmp = out.str();
				} else {
					tmp = val->value<string>();
				}
				if(val2->type() == typeInt) {
					stringstream out2;
					if(val2->any()->type() == typeid(bool)) {
						pb = *boost::any_cast<bool>(val2->any());
						out2 << ((pb) ? "true" : "false");
					} else {
						out2 << val2->value<int>();
					}
					tmp2 = out2.str();
				} else {
					tmp2 = val2->value<string>();
				}
				return new Value(typeStr, tmp + tmp2);
			} else {
				return new Value(typeInt, val->value<int>() + val2->value<int>());
			}
		} else {
			if((val->type() != typeInt) || (val2->type() != typeInt)) yyerror("Operation not available with these type");
			switch(m_oper) {
				case '-': return new Value(val->value<int>() - val2->value<int>());
				case '*': return new Value(val->value<int>() * val2->value<int>());
				case '/': return new Value(val->value<int>() / val2->value<int>());
				case '^': return new Value((int)pow(val->value<int>(), val2->value<int>()));
				case '<': return new Value(val->value<int>() < val2->value<int>());
				case '>': return new Value(val->value<int>() > val2->value<int>());
				case GE: return new Value(val->value<int>() >= val2->value<int>());
				case LE: return new Value(val->value<int>() <= val2->value<int>());
				case EQ: return new Value(val->value<int>() == val2->value<int>());
				case NE: return new Value(val->value<int>() != val2->value<int>());
			}
		}
	}
}

VarExpression::VarExpression(string varName) {
	m_var = Variable::findByName(varName);
}

VarExpression::~VarExpression() {
	delete m_var;
}

AssignExpression::AssignExpression(string varName, Expression *valExp) {
	m_varName = varName;
	m_valExp = valExp;
	doThings();
}

AssignExpression::~AssignExpression() {
	delete m_var;
	delete m_valExp;
}

Value* AssignExpression::evaluate() {
	return m_var->value();
}

void AssignExpression::doExp() {
	if(Variable::exists(m_varName)) {
		m_var = Variable::findByName(m_varName);
		m_var->value(m_valExp->evaluate());
	} else {
		m_var = new Variable(m_varName, m_valExp->evaluate());
	}
}

IfExpression::IfExpression(Expression *ifExp, vector<Expression*> *statements) {
	m_ifExp = ifExp;
	m_statements = statements;
	m_elseStatements = 0;
	doThings(true);
	scopes--;
}

IfExpression::IfExpression(Expression *ifExp, vector<Expression*> *statements, vector<Expression*> *elseStatements) {
	m_ifExp = ifExp;
	m_statements = statements;
	m_elseStatements = elseStatements;
	doThings(true);
	scopes--;
}

IfExpression::~IfExpression() {
	delete m_ifExp;
	delete m_statements;
	delete m_elseStatements;
}

void IfExpression::doExp() {
	if(m_ifExp->evaluate()->value<bool>()) {
		for(unsigned int i = 0 ; i < m_statements->size() ; i++) {
			(*m_statements)[i]->doExp();
		}
	} else {
		if(m_elseStatements != 0) {
			for(unsigned int i = 0 ; i < m_elseStatements->size() ; i++) {
				(*m_elseStatements)[i]->doExp();
			}
		}
	}
}

CallExpression::CallExpression(string funcName, vector<Expression*> *args) {
	m_funcName = funcName;
	m_func = Function::findByName(m_funcName);
	m_args = args;
}

CallExpression::~CallExpression() {
	delete m_func;
	delete m_args;
}

void CallExpression::doExp() {
	//m_func->doFuncInstr();
}

PrintExpression::PrintExpression(Expression *exp) {
	m_exp = exp;
	doThings();
}

PrintExpression::~PrintExpression() {
	delete m_exp;
}

WhileExpression::WhileExpression(Expression *whileExp, vector<Expression*> *statements) {
	m_whileExp = whileExp;
	m_statements = statements;
	doThings(true);
	scopes--;
}

WhileExpression::~WhileExpression() {
	delete m_whileExp;
	delete m_statements;
}

void WhileExpression::doExp() {
	while(m_whileExp->evaluate()->value<bool>()) {
		for(unsigned int i = 0 ; i < m_statements->size() ; i++) {
			(*m_statements)[i]->doExp();
		}
	}
}

ForExpression::ForExpression(Expression *varExp, std::vector<Expression*> *statements, Expression *fromExp, Expression *toExp, Expression *stepExp = 0);
	m_varExp = varExp;
	m_statements = statements;
	m_fromExp = fromExp;
	m_toExp = toExp;
	m_stepExp = stepExp;
	doThings(true);
	scopes--;
}

ForExpression::~ForExpression() {
	delete m_varExp;
	delete m_statements;
	delete m_fromExp;
	delete m_toExp;
	delete m_stepExp;
}

void ForExpression::doExp() {
	if(m_stepExp == 0) m_stepExp = new Value(0);
	for(unsigned int i = m_fromExp->evaluate()->value<int>() ; i <= m_fromExp->evaluate()->value<int>() ; i += m_stepExp->evaluate()->value<int>()) {
		for(unsigned int i = 0 ; i < m_statements->size() ; i++) {
			(*m_statements)[i]->doExp();
		}
	}
}

