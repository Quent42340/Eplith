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

using namespace std;

vector<Function*> Function::funcs;

Function::Function(string name, vector<VarExpression*> *args, vector<Expression*> *stmts, boost::any returnValue) {
	m_address = getPtrAddr((void*)this);
	m_name = name;
	m_args = args;
	m_stmts = new vector<Expression*>;
	for(unsigned int i = 0 ; i < stmts->size() ; i++) {
		m_stmts->push_back((*stmts)[i]);
	}
	m_ret = Variable::vars[0]->value();
	
#ifdef FUNC_DEBUG
	cout << "Func name: " << m_name << " | Address: " << m_address << endl;
#endif
	
	funcs.push_back(this);
}

Function::~Function() {
	delete m_args;
	delete m_stmts;
	delete m_ret;
}

Function* Function::findByName(std::string name) {
	for(unsigned int i = funcs.size() - 1 ; i >= 0 ; i--) {
		if(funcs[i]->name() == name) {
			return funcs[i];
		}
	}
	return 0;
}

bool Function::exists(std::string name) {
	for(unsigned int i = 0 ; i < funcs.size() ; i++) {
		if(funcs[i]->name() == name) {
			return true;
		}
	}
	return false;
}

void Function::doFunc(vector<Expression*> *args) {
	if(m_args->size() != args->size()) yyerror("Unexpected number of arguments given");
	for(unsigned int i = 0 ; i < m_args->size() ; i++) {
		m_vars.push_back(new Variable((*m_args)[i]->varName(), new Value(*(*args)[i]->evaluate())));
	}
	int oldlineno = yylineno;
	for(unsigned int i = 0 ; i < m_stmts->size() ; i++) {
		yylineno = (*m_stmts)[i]->line();
		(*m_stmts)[i]->doExp();
		if(Expression::signal == sRETURN) {
			Expression::signal = sNONE;
			m_ret = (*m_stmts)[i]->evaluate();
			break;
		}
	}
	yylineno = oldlineno;
	for(unsigned int i = m_vars.size() - 1 ; m_vars.size() != 0 ; i--) {
		delete m_vars[i];
		m_vars.pop_back();
	}
}

