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

using namespace std;

Function::Function(const Function &f) {
	m_args = new vector<VarExpression*>(*f.m_args);
	m_vars = new vector<Variable*>(*f.m_vars);
	m_stmts = new vector<Expression*>(*f.m_stmts);
	m_ret = new Value(*f.m_ret);
	m_colon = f.m_colon;
	m_mainElement = (f.m_mainElement) ? new Value(*f.m_mainElement) : 0;
}

Function::Function(vector<VarExpression*> *args, vector<Expression*> *stmts, boost::any returnValue) {
	m_args = new vector<VarExpression*>(*args);
	m_stmts = new vector<Expression*>;
	m_vars = new vector<Variable*>;
	
	for(unsigned int i = 0 ; i < stmts->size() ; i++) {
		m_stmts->push_back((*stmts)[i]);
	}
	
	m_ret = new Value();
	m_colon = false;
	m_mainElement = 0;
}

Function::~Function() {
	delete m_args;
	delete m_vars;
	delete m_stmts;
	delete m_ret;
	if(m_mainElement) delete m_mainElement;
}

void Function::doFunc(vector<Expression*> *args) {
	if(m_colon) {
		if(m_args->size() != args->size() + 1) {
			yyerror(string("Unexpected number of arguments: ") + itos(args->size() + 1) + " given but " + itos(m_args->size()) + " required");
		}
	} else {
		if(m_args->size() != args->size()) {
			if(m_args->size() > args->size()) {
				for(unsigned int i = args->size() ; i < m_args->size() ; i++) {
					if(!(*m_args)[i]->value()) yyerror(string("Unexpected number of arguments: ") + itos(args->size()) + " given but " + itos(m_args->size()) + " required");
				}
			} else {
				yyerror(string("Unexpected number of arguments: ") + itos(args->size()) + " given but " + itos(m_args->size()) + " required");
			}
		}
	}
	
	for(unsigned int i = 0 ; i < m_args->size() ; i++) {
		if(m_colon && (*m_args)[i]->varName() == "self") {
			if(!m_mainElement) yyerror("Unexpected error");
			else m_vars->push_back(new Variable((*m_args)[i]->varName(), m_mainElement));
		} else {
			m_vars->push_back(new Variable((*m_args)[i]->varName(), (i < args->size()) ? (*args)[i]->evaluate() : (*m_args)[i]->value()));
		}
	}
	
	int oldlineno = yylineno;
	for(unsigned int i = 0 ; i < m_stmts->size() ; i++) {
		yylineno = (*m_stmts)[i]->line();
		(*m_stmts)[i]->doExp();
		if(Expression::signal == sRETURN) {
			Expression::signal = sNONE;
			m_ret = new Value(*(*m_stmts)[i]->evaluate());
			break;
		}
	}
	
	yylineno = oldlineno;
	for(unsigned int i = m_vars->size() - 1 ; m_vars->size() != 0 ; i--) {
		m_vars->pop_back();
	}
}

