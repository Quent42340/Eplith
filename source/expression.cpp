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
#include "header.h"
#include "function.h"
#include "expression.h"
#include "parser.tab.hpp"

using namespace std;

int Expression::scopes = 0;
Signal Expression::signal = sNONE;

Expression::Expression() {
	m_mode = noMode;
	m_line = yylineno;
}

Expression::~Expression() {
}

IntExpression::IntExpression(int value) {
	m_type = "IntExpression";
	m_value = value;
}

IntExpression::~IntExpression() {
}

FloatExpression::FloatExpression(double value) {
	m_type = "FloatExpression";
	m_value = value;
}

FloatExpression::~FloatExpression() {
}

StrExpression::StrExpression(string str) {
	m_type = "StrExpression";
	m_str = str;
}

StrExpression::~StrExpression() {
}

BoolExpression::BoolExpression(bool value) {
	m_type = "BoolExpression";
	m_value = value;
}

BoolExpression::~BoolExpression() {
}

ArrayExpression::ArrayExpression(multimap<string, Expression*> *elements) {
	m_type = "ArrayExpression";
	m_elements = elements;
}

ArrayExpression::~ArrayExpression() {
	delete m_elements;
}

Value* ArrayExpression::evaluate() {
	unsigned int a = 0;
	map<string, Value*> *mElements = new map<string, Value*>;
	for(multimap<string, Expression*>::iterator it = m_elements->begin() ; it != m_elements->end() ; it++) {
		if(it->first == "<<nothing>>") {
			mElements->insert(pair<string, Value*>(itos(a++), it->second->evaluate()));
		} else {
			mElements->insert(pair<string, Value*>(it->first, it->second->evaluate()));
		}
	}
	return new Value(*mElements);
}

ElementExpression::ElementExpression(string arrayName, vector<string> *index) {
	m_type = "ElementExpression";
	m_arrayName = arrayName;
	m_index = index;
}

ElementExpression::~ElementExpression() {
	delete m_index;
}

Value* ElementExpression::evaluate() {
	Variable *array = Variable::findByName(m_arrayName);
	if(array == 0) yyerror(string("Variable \"") + string(m_arrayName) + string("\" undefined"));
	if(array->value()->type() != typeArray) yyerror("Trying to access to an element of a non-array variable");
	//cout << array->value()->element(*m_index)->type() << endl;
	return array->value()->element(*m_index);
}

OpExpression::OpExpression(Expression *exp1, int oper, Expression *exp2) {
	m_type = "OpExpression";
	m_exp1 = exp1;
	m_oper = oper;
	m_exp2 = exp2;
}

OpExpression::~OpExpression() {
	delete m_exp1;
	delete m_exp2;
}

Value* OpExpression::evaluate() {
	if(m_exp1->hexMode() || ((m_exp2) ? m_exp2->hexMode() : 0)) m_mode = modeHex;
	if(m_exp1->sciMode() || ((m_exp2) ? m_exp2->sciMode() : 0)) m_mode = modeSci;
	Value *val = m_exp1->evaluate();
	Value *val2 = (m_exp2) ? m_exp2->evaluate() : 0;
	//cdbg("val type: " << val->type() << " | val2 type: " << val2->type());
	if(m_oper == '+') {
		bool pb;
		if(!isNum(val) || !isNum(val2)) {
			string tmp, tmp2;
			stringstream out, out2;
			val->print(out, m_mode);
			out << ends;
			tmp = out.str();
			val2->print(out2, m_mode);
			out2 << ends;
			tmp2 = out2.str();
			return new Value(tmp + tmp2);
		} else {
			return new Value(getNumVal(val) + getNumVal(val2), m_mode);
		}
	} else {
		switch(m_oper) {
			case NEG:	 return new Value(-getNumVal(val));
			case POS:	 return new Value(+getNumVal(val));
			case NOT:	 return new Value(!val->value<bool>());
			case BNOT:	 return new Value(~(int)getNumVal(val));
		}
		if(!isNum(val) || !isNum(val2)) yyerror("Operation not available with these type");
		switch(m_oper) {
			case '-':	 return new Value(getNumVal(val) - getNumVal(val2));
			case '*':	 return new Value(getNumVal(val) * getNumVal(val2));
			case '/':	 return new Value(getNumVal(val) / getNumVal(val2));
			case '^':	 return new Value(valPow(val, val2));
			case '%':	 return new Value((int)getNumVal(val) % (int)getNumVal(val2));
			case '<':	 return new Value(getNumVal(val) < getNumVal(val2));
			case '>':	 return new Value(getNumVal(val) > getNumVal(val2));
			case GE:	 return new Value(getNumVal(val) >= getNumVal(val2));
			case LE:	 return new Value(getNumVal(val) <= getNumVal(val2));
			case EQ:	 return new Value(getNumVal(val) == getNumVal(val2));
			case NE:	 return new Value(getNumVal(val) != getNumVal(val2));
			case AND:	 return new Value(valNumToBool(val) && valNumToBool(val2));
			case OR:	 return new Value(valNumToBool(val) || valNumToBool(val2));
			case BAND:	 return new Value((int)getNumVal(val) & (int)getNumVal(val2));
			case BOR:	 return new Value((int)getNumVal(val) | (int)getNumVal(val2));
			case XOR:	 return new Value((int)getNumVal(val) ^ (int)getNumVal(val2));
			case LSHIFT: return new Value((int)getNumVal(val) << (int)getNumVal(val2));
			case RSHIFT: return new Value((int)getNumVal(val) >> (int)getNumVal(val2));
			default:	 yyerror("Unexpected operator"); break;
		}
	}
}

VarExpression::VarExpression(string varName) {
	m_type = "VarExpression";
	m_varName = varName;
	doThings();
}

VarExpression::~VarExpression() {
	delete m_var;
}

Value* VarExpression::evaluate() {
	doExp();
	return m_var->value();
}

void VarExpression::doExp() {
	if(Variable::exists(m_varName)) {
		m_var = Variable::findByName(m_varName);
	}
	else if(Function::exists(m_varName)) {
		m_var = new Variable("<<unamed>>", new Value(Function::findByName(m_varName)), true);
	} else {
		yyerror(string("Variable or function '") + m_varName + "' not found.");
	}
}

AssignExpression::AssignExpression(string varName, Expression *valExp, int op) {
	m_type = "AssignExpression";
	m_varName = varName;
	m_valExp = valExp;
	m_op = op;
	m_element = 0;
	doThings();
}

AssignExpression::AssignExpression(ElementExpression *element, Expression *valExp, int op) {
	m_type = "AssignExpression";
	m_valExp = valExp;
	m_op = op;
	m_element = element;
	doThings();
}

AssignExpression::~AssignExpression() {
	delete m_var;
	delete m_valExp;
}

Value* AssignExpression::evaluate() {
	if(m_element != 0) m_element->evaluate();
	return (m_element == 0) ? m_var->value() : m_var->value()->element(*m_element->index());
}

void AssignExpression::doExp() {
	bool array = (m_element == 0) ? false : true;
	if(Variable::exists(m_varName) || array) {
		if(!array) m_var = Variable::findByName(m_varName);
		else m_var = Variable::findByName(m_element->arrayName());
		if(m_op == -1) {
			if(!array) m_var->value(m_valExp->evaluate());
			else {
				m_var->value()->element(m_element, m_valExp->evaluate());
			}
		} else {
			Value *val = m_var->value();
			Value *val2 = m_valExp->evaluate();
			if(m_op == ADD) {
				bool pb;
				if(!isNum(val) || !isNum(val2)) {
					string tmp, tmp2;
					stringstream out, out2;
					val->print(out, m_mode);
					out << ends;
					tmp = out.str();
					val2->print(out2, m_mode);
					out2 << ends;
					tmp2 = out2.str();
					m_var->value()->value<string>(tmp + tmp2);
				} else {
					setNumVal(m_var->value(), getNumVal(val) + getNumVal(val2));
					m_var->value()->hexMode(true);
				}
			} else { 
				switch(m_op) {
					case SUB:	  setNumVal(m_var->value(), getNumVal(val) - getNumVal(val2)); break;
					case MUL:	  setNumVal(m_var->value(), getNumVal(val) * getNumVal(val2)); break;
					case DIV:	  setNumVal(m_var->value(), getNumVal(val) / getNumVal(val2)); break;
					case EXP:	  setNumVal(m_var->value(), valPow(val, val2)); break;
					case MOD:	  setNumVal(m_var->value(), (int)getNumVal(val) % (int)getNumVal(val2)); break;
					case BAND_EQ: setNumVal(m_var->value(), (int)getNumVal(val) & (int)getNumVal(val2)); break;
					case BOR_EQ:  setNumVal(m_var->value(), (int)getNumVal(val) | (int)getNumVal(val2)); break;
					case XOR_EQ:  setNumVal(m_var->value(), (int)getNumVal(val) ^ (int)getNumVal(val2)); break;
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
	m_type = "CrExpression";
	m_varExp = (VarExpression*)varExp;
	m_op = op;
	m_after = after;
}

CrExpression::~CrExpression() {
	delete m_varExp;
	delete m_valA;
	delete m_valB;
}

void CrExpression::doExp() {
	if(m_varExp->getVar()->value()->type() != typeInt) yyerror("Incrementation / decrementation unavailable with these type");
	m_valB = new Value(*m_varExp->getVar()->value());
	if(m_op == INCR) {
		m_valA = m_varExp->getVar()->value()->valIncr();
	}
	else if(m_op == DECR) {
		m_valA = m_varExp->getVar()->value()->valDecr();
	} else {
		yyerror("Unexpected operator");
	}
}

Value* CrExpression::evaluate() {
	doExp();
	switch(m_op) {
		case INCR: {
			if(m_after) return m_valB;
			else return m_valA;
		}
		case DECR: {
			if(m_after) return m_valB;
			else return m_valA;
		}
		default: yyerror("Unexpected operator");
	}
}

IfExpression::IfExpression(Expression *ifExp, vector<Expression*> *statements, vector<Expression*> *elseStatements) {
	m_type = "IfExpression";
	m_ifExp = ifExp;
	m_statements = statements;
	m_elseStatements = elseStatements;
	doThings(true);
	endScope();
}

IfExpression::~IfExpression() {
	delete m_ifExp;
	delete m_statements;
	delete m_elseStatements;
}

unsigned int n = 0, m = 0;
Value* IfExpression::evaluate() {
	if(m_ifExp->evaluate()->intToBool()->value<bool>()) {
		return (*m_statements)[n]->evaluate();
	} else {
		if(m_elseStatements != 0) {
			return (*m_elseStatements)[m]->evaluate();
		} else {
			return new Value();
		}
	}
}

void IfExpression::doExp() {
	int oldlineno = yylineno;
	if(m_ifExp->evaluate()->intToBool()->value<bool>()) {
		for(n = 0 ; n < m_statements->size() ; n++) {
			yylineno = (*m_statements)[n]->line();
			(*m_statements)[n]->doExp();
		} n--;
	} else {
		if(m_elseStatements != 0) {
			for(m = 0 ; m < m_elseStatements->size() ; m++) {
				yylineno = (*m_elseStatements)[m]->line();
				(*m_elseStatements)[m]->doExp();
			} m--;
		}
	}
	yylineno = oldlineno;
}

FuncExpression::FuncExpression(string funcName, vector<VarExpression*> *args, vector<Expression*> *stmts) {
	m_type = "FuncExpression";
	m_funcName = funcName;
	m_args = args;
	m_stmts = stmts;
	doThings(true);
	endScope();
}

FuncExpression::~FuncExpression() {
	delete m_func;
	delete m_args;
	delete m_stmts;
}

void FuncExpression::doExp() {
	if(Function::exists(m_funcName) && m_funcName != "<<unamed>>") {
#ifdef WARNINGS
		yywarn("The function already exists, this declaration will overwrite it");
#endif
		m_func = new Function(m_funcName, m_args, m_stmts);
	} else {
		m_func = new Function(m_funcName, m_args, m_stmts);
	}
}

CallExpression::CallExpression(string funcName, vector<Expression*> *args) {
	m_type = "CallExpression";
	m_funcName = funcName;
	m_element = 0;
	m_args = args;
	m_init = false;
}

CallExpression::CallExpression(ElementExpression *element, vector<Expression*> *args) {
	m_type = "CallExpression";
	m_funcName = "<<unamed>>";
	m_element = element;
	m_args = args;
	m_init = false;
}

CallExpression::~CallExpression() {
	m_funcs.clear();
	delete m_args;
}

void CallExpression::initFunc() {
	Function *func = Function::findByName(m_funcName);
	if(func == 0) func = Variable::findByName(m_funcName)->value()->value<Function*>();
	if(m_element == 0) m_funcs.push_back(new Function(*func));
	else m_funcs.push_back(new Function(*m_element->evaluate()->value<Function*>()));
	if(m_funcs.back() == 0) yyerror("Function undefined");
	m_init = true;
}

Value* CallExpression::evaluate() {
	if(m_init) initFunc();
	doExp();
	Value* ret = new Value(*m_funcs.back()->ret());
	m_funcs.pop_back();
	return ret;
}

void CallExpression::doExp() {
	if(!m_init) initFunc();
	scopes++;
	m_funcs.back()->doFunc(m_args);
	endScope();
}

ReturnExpression::ReturnExpression(Expression *exp) {
	m_type = "ReturnExpression";
	m_exp = exp;
}

ReturnExpression::~ReturnExpression() {
	delete m_exp;
}

PrintExpression::PrintExpression(Expression *exp) {
	m_type = "PrintExpression";
	m_exp = exp;
	doThings();
}

PrintExpression::~PrintExpression() {
	delete m_exp;
}

SignalExpression::SignalExpression(Signal s) {
	m_signal = s;
	doThings();
}

void SignalExpression::doExp() {
	signal = m_signal;
}

WhileExpression::WhileExpression(Expression *whileExp, vector<Expression*> *statements) {
	m_type = "WhileExpression";
	m_whileExp = whileExp;
	m_statements = statements;
	doThings(true);
	endScope();
}

WhileExpression::~WhileExpression() {
	delete m_whileExp;
	delete m_statements;
}

void WhileExpression::doExp() {
	int oldlineno = yylineno;
	while(m_whileExp->evaluate()->intToBool()->value<bool>()) {
		for(unsigned int i = 0 ; i < m_statements->size() ; i++) {
			yylineno = (*m_statements)[i]->line();
			(*m_statements)[i]->doExp();
			if(signal == sBREAK || signal == sCONTINUE) break;
		}
		if(signal == sBREAK) {
			signal = sNONE;
			break;
		}
		else if(signal == sCONTINUE) {
			signal = sNONE;
			continue;
		}
	}
	yylineno = oldlineno;
}

ForExpression::ForExpression(Expression *varExp, std::vector<Expression*> *statements, Expression *toExp, Expression *stepExp) {
	m_type = "ForExpression";
	m_varExp = (AssignExpression*)varExp;
	m_toExp = toExp;
	m_stepExp = stepExp;
	m_statements = statements;
	doThings(true);
	endScope();
}

ForExpression::~ForExpression() {
	delete m_varExp;
	delete m_toExp;
	delete m_stepExp;
	delete m_statements;
}

void ForExpression::doExp() {
	if(m_stepExp == 0) m_stepExp = new IntExpression(1);
	Variable *var = m_varExp->getVar();
	Value *vi = new Value((int)-1);
	int oldlineno = yylineno;
	for(unsigned int i = m_varExp->evaluate()->value<int>() ; i <= m_toExp->evaluate()->value<int>() ; i += m_stepExp->evaluate()->value<int>()) {
		vi->value<int>(i);
		var->value(vi);
		for(unsigned int j = 0 ; j < m_statements->size() ; j++) {
			yylineno = (*m_statements)[j]->line();
			(*m_statements)[j]->doExp();
			if(signal == sBREAK || signal == sCONTINUE) break;
		}
		if(signal == sBREAK) {
			signal = sNONE;
			break;
		}
		else if(signal == sCONTINUE) {
			signal = sNONE;
			continue;
		}
	}
	yylineno = oldlineno;
	delete vi;
}

