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
#ifndef EPBLIB_TOOLS_H
#define EPBLIB_TOOLS_H

std::vector<VarExpression*> *EPblib_args(int nbArgs, ...);
std::vector<Expression*> *EPblib_stmts(int nbStmts, ...);

std::string EPb_valToStr(Value *v);

#define EPb_getValStr(val) val->value<string>()

#define EPb_getInt(exp) (int)getNumVal(exp->evaluate())
#define EPb_getStr(exp) exp->evaluate()->value<string>()
#define EPb_getArray(exp) exp->evaluate()->value< map<string, Value*> >()

#define EPb_getArrayPtr(exp) exp->evaluate()->valuePtr< map<string, Value*> >()

#define EPb_initSSM(Struct) std::vector<VarExpression*> *Struct::args = 0; \
						std::vector<Expression*> *Struct::stmts = 0;

#define EPb_initFunc(Struct, name) Struct::init(); \
							   Variable *name = new Variable(#name, new Value(new Function(Struct::args, Struct::stmts)));

#define EPb_initElemFunc(table, Struct, name) Struct::init(); \
										  Value *name = new Value(new Function(Struct::args, Struct::stmts)); \
										  table.insert(table.end(), pair<string, Value*>(#name, name));

#endif // EPBLIB_TOOLS_H
