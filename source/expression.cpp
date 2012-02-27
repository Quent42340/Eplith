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
		return new IntValue(-val->value<int>());
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
				case '-': return new IntValue(val->value<int>() - val2->value<int>());
				case '*': return new IntValue(val->value<int>() * val2->value<int>());
				case '/': return new IntValue(val->value<int>() / val2->value<int>());
				case '^': return new IntValue(pow(val->value<int>(), val2->value<int>()));
				case '<': return new BoolValue(val->value<int>() < val2->value<int>());
				case '>': return new BoolValue(val->value<int>() > val2->value<int>());
				case GE: return new BoolValue(val->value<int>() >= val2->value<int>());
				case LE: return new BoolValue(val->value<int>() <= val2->value<int>());
				case EQ: return new BoolValue(val->value<int>() == val2->value<int>());
				case NE: return new BoolValue(val->value<int>() != val2->value<int>());
			}
		}
	}
}

VarExpression::VarExpression(Variable *var) {
	m_var = var;
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
}

Value* AssignExpression::evaluate() {
	return m_var->value();
}

IfExpression::IfExpression(Expression *ifExp, Expression *thenExp) {
	m_ifExp = ifExp;
	m_thenExp = thenExp;
	m_elseExp = 0;
}

IfExpression::IfExpression(Expression *ifExp, Expression *thenExp, Expression *elseExp) {
	m_ifExp = ifExp;
	m_thenExp = thenExp;
	m_elseExp = elseExp;
}

IfExpression::~IfExpression() {
	delete m_ifExp;
	delete m_thenExp;
	delete m_elseExp;
}

Value* IfExpression::evaluate() {
	if(m_ifExp->evaluate()->value<bool>()) {
		return m_thenExp->evaluate();
	} else {
		if(m_elseExp != 0) {
			return m_elseExp->evaluate();
		}
	}
}

void IfExpression::doThings() {
	if(m_ifExp->evaluate()->value<bool>()) {
		m_thenExp->doThings();
	} else {
		if(m_elseExp != 0) {
			m_elseExp->doThings();
		}
	}
}

PrintExpression::PrintExpression(Expression *exp) {
	m_exp = exp;
	doThings();
}

PrintExpression::~PrintExpression() {
	delete m_exp;
}

