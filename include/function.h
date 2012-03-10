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
#ifndef FUNCTION_H
#define FUNCTION_H

#include <vector>
#include <boost/any.hpp>

class Expression;
class VarExpression;
class Variable;

class Function {
	public:
		Function(std::string name, std::vector<VarExpression*> *args, std::vector<Expression*> *stmts, boost::any returnValue = -1);
		~Function();
		
		std::string name() const { return m_name; }
		std::string address() const { return m_address; }
		
		void doFunc(std::vector<Expression*> *args);
		
		static Function* findByName(std::string name);
		static bool exists(std::string name);
		
		static std::vector<Function*> funcs;
		
	private:
		std::string m_name;
		std::string m_address;
		std::vector<VarExpression*> *m_args;
		std::vector<Variable*> m_vars;
		std::vector<Expression*> *m_stmts;
};

#endif // FUNCTION_H
