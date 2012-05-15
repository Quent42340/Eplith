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
#ifndef EPLIB_FUNCTION_H
#define EPLIB_FUNCTION_H

#include <vector>
#include <boost/any.hpp>
#include "variable.h"

class Expression;
class VarExpression;

class Function {
	public:
		Function(const Function &f);
		Function(std::vector<VarExpression*> *args, std::vector<Expression*> *stmts, boost::any returnValue = -1);
		~Function();
		
		std::vector<VarExpression*> *args() const { return m_args; }
		
		Value *ret() { return m_ret; }
		
		void colon(bool colon) { m_colon = colon; }
		void mainElement(Value *mainElement) { m_mainElement = mainElement; }
		
		void doFunc(std::vector<Expression*> *args);
		
	private:
		std::vector<VarExpression*> *m_args;
		std::vector<Variable*> *m_vars;
		std::vector<Expression*> *m_stmts;
		Value *m_ret;
		bool m_colon;
		Value *m_mainElement;
};

#endif // EPLIB_FUNCTION_H
