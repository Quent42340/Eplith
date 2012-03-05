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
	m_hexMode = false;
}

Expression::~Expression() {
}

IntExpression::IntExpression(int value, bool hexMode) {
	m_value = value;
	m_hexMode = hexMode;
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
	if(m_exp1->hexMode() || ((m_exp2) ? m_exp2->hexMode() : 0)) m_hexMode = true;
	Value *val = m_exp1->evaluate();
	if(m_oper == NEG) {
		return new Value(-val->value<int>(), m_hexMode);
	}
	else if(m_oper == POS) {
		return new Value(+val->value<int>(), m_hexMode);
	}
	else if(m_oper == NOT) {
		return new Value(!val->value<bool>());
	}
	else if(m_oper == BNOT) {
		return new Value(~val->value<int>());
	} else {
		Value *val2 = m_exp2->evaluate();
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
						if(m_hexMode) out << "0x" << hex;
						out << val->value<int>();
						if(m_hexMode) out << dec;
					}
					out << ends;
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
						if(m_hexMode) out2 << "0x" << hex;
						out2 << val2->value<int>();
						if(m_hexMode) out2 << dec;
					}
					out2 << ends;
					tmp2 = out2.str();
				} else {
					tmp2 = val2->value<string>();
				}
				return new Value(typeStr, tmp + tmp2);
			} else {
				return new Value(typeInt, val->value<int>() + val2->value<int>(), m_hexMode);
			}
		} else {
			if((val->type() != typeInt) || (val2->type() != typeInt)) yyerror("Operation not available with these type");
			switch(m_oper) {
				case '-':	 return new Value(val->value<int>() - val2->value<int>(), m_hexMode);
				case '*':	 return new Value(val->value<int>() * val2->value<int>(), m_hexMode);
				case '/':	 return new Value(val->value<int>() / val2->value<int>(), m_hexMode);
				case '^':	 return new Value((int)pow((double)val->value<int>(), (double)val2->value<int>()), m_hexMode);
				case '%':	 return new Value(val->value<int>() % val2->value<int>());
				case '<':	 return new Value(val->value<int>() < val2->value<int>());
				case '>':	 return new Value(val->value<int>() > val2->value<int>());
				case GE:	 return new Value(val->value<int>() >= val2->value<int>());
				case LE:	 return new Value(val->value<int>() <= val2->value<int>());
				case EQ:	 return new Value(((val->any()->type() == typeid(int)) ? (bool)val->value<int>() : val->value<bool>())
										   == ((val2->any()->type() == typeid(int)) ? (bool)val2->value<int>() : val2->value<bool>()));
				case NE:	 return new Value(((val->any()->type() == typeid(int)) ? (bool)val->value<int>() : val->value<bool>())
										   != ((val2->any()->type() == typeid(int)) ? (bool)val2->value<int>() : val2->value<bool>()));
				case AND:	 return new Value(((val->any()->type() == typeid(int)) ? (bool)val->value<int>() : val->value<bool>())
										   && ((val2->any()->type() == typeid(int)) ? (bool)val2->value<int>() : val2->value<bool>()));
				case OR:	 return new Value(((val->any()->type() == typeid(int)) ? (bool)val->value<int>() : val->value<bool>())
										   || ((val2->any()->type() == typeid(int)) ? (bool)val2->value<int>() : val2->value<bool>()));
				case BAND:	 return new Value(val->value<int>() & val2->value<int>(), m_hexMode);
				case BOR:	 return new Value(val->value<int>() | val2->value<int>(), m_hexMode);
				case XOR:	 return new Value(val->value<int>() ^ val2->value<int>(), m_hexMode);
				case LSHIFT: return new Value(val->value<int>() << val2->value<int>(), m_hexMode);
				case RSHIFT: return new Value(val->value<int>() >> val2->value<int>(), m_hexMode);
				default:	 yyerror("Unexpected operator"); break;
			}
		}
	}
}

VarExpression::VarExpression(string varName) {
	m_varName = varName;
	doThings();
}

VarExpression::~VarExpression() {
	delete m_var;
}

AssignExpression::AssignExpression(string varName, Expression *valExp, int op) {
	m_varName = varName;
	m_valExp = valExp;
	m_op = op;
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
		if(m_op == -1) {
			m_var->value(m_valExp->evaluate());
		} else {
			Value *val = m_var->value();
			Value *val2 = m_valExp->evaluate();
			if(m_op == ADD) {
				bool pb;
				if((val->type() != typeInt) || (val2->type() != typeInt)) {
					string tmp, tmp2;
					if(val->type() == typeInt) {
						stringstream out;
						if(val->any()->type() == typeid(bool)) {
							pb = *boost::any_cast<bool>(val->any());
							out << ((pb) ? "true" : "false");
						} else {
							if(m_hexMode) out << "0x" << hex;
							out << val->value<int>();
							if(m_hexMode) out << dec;
						}
						out << ends;
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
							if(m_hexMode) out2 << "0x" << hex;
							out2 << val2->value<int>();
							if(m_hexMode) out2 << dec;
						}
						out2 << ends;
						tmp2 = out2.str();
					} else {
						tmp2 = val2->value<string>();
					}
					m_var->value()->value<string>(tmp + tmp2);
				} else {
					m_var->value()->value<int>(val->value<int>() + val2->value<int>());
					m_var->value()->hexMode(true);
				}
			} else { 
				switch(m_op) {
					case SUB:	  m_var->value()->value<int>(val->value<int>() - val2->value<int>()); break;
					case MUL:	  m_var->value()->value<int>(val->value<int>() * val2->value<int>()); break;
					case DIV:	  m_var->value()->value<int>(val->value<int>() / val2->value<int>()); break;
					case EXP:	  m_var->value()->value<int>((int)pow((double)val->value<int>(), (double)val->value<int>())); break;
					case MOD:	  m_var->value()->value<int>(val->value<int>() % val2->value<int>()); break;
					case BAND_EQ: m_var->value()->value<int>(val->value<int>() & val2->value<int>()); break;
					case BOR_EQ:  m_var->value()->value<int>(val->value<int>() | val2->value<int>()); break;
					case XOR_EQ:  m_var->value()->value<int>(val->value<int>() ^ val2->value<int>()); break;
					default:	  yyerror("Unexpected operator"); break;
				}
			}
		}
	} else {
		if(m_op != -1) yyerror("Variable undeclared");
		m_var = new Variable(m_varName, m_valExp->evaluate());
	}
}

CrExpression::CrExpression(Expression *varExp, int op, bool after) {
	m_varExp = (VarExpression*)varExp;
	m_op = op;
	m_after = after;
	if(m_varExp->getVar()->value()->type() != typeInt) yyerror("Incrementation / decrementation unavailable with these type");
	doThings();
}

CrExpression::~CrExpression() {
	delete m_varExp;
}

Value* CrExpression::evaluate() {
	Value *val = new Value(*m_varExp->getVar()->value());
	switch(m_op) {
		case INCR: {
			Value *val2 = m_varExp->getVar()->value()->valIncr();
			if(m_after) return val;
			else return val2;
		}
		case DECR: {
			Value *val2 = m_varExp->getVar()->value()->valDecr();
			if(m_after) return val;
			else return val2;
		}
		default: yyerror("Unexpected operator");
	}
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
	Value *ifExpVal = m_ifExp->evaluate();
	if(((ifExpVal->any()->type() == typeid(int)) ? (bool)ifExpVal->value<int>() : ifExpVal->value<bool>())) {
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
	Value *whileExpVal = m_whileExp->evaluate();
	while(((whileExpVal->any()->type() == typeid(int)) ? (bool)whileExpVal->value<int>() : whileExpVal->value<bool>())) {
		for(unsigned int i = 0 ; i < m_statements->size() ; i++) {
			(*m_statements)[i]->doExp();
		}
	}
}

ForExpression::ForExpression(Expression *varExp, std::vector<Expression*> *statements, Expression *toExp, Expression *stepExp) {
	m_varExp = (AssignExpression*)varExp;
	m_toExp = toExp;
	m_stepExp = stepExp;
	m_statements = statements;
	doThings(true);
	scopes--;
}

ForExpression::~ForExpression() {
	delete m_varExp;
	delete m_toExp;
	delete m_stepExp;
	delete m_statements;
}

void ForExpression::doExp() {
	if(m_stepExp == 0) m_stepExp = new IntExpression(1);
	m_varExp->doExp();
	Value *vi = new Value((int)-1);
	for(unsigned int i = m_varExp->evaluate()->value<int>() ; i <= m_toExp->evaluate()->value<int>() ; i += m_stepExp->evaluate()->value<int>()) {
		vi->value<int>(i);
		m_varExp->getVar()->value(vi);
		for(unsigned int j = 0 ; j < m_statements->size() ; j++) {
			(*m_statements)[j]->doExp();
		}
	}
	delete vi;
}

