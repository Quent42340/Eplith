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

typedef std::vector<VarExpression*> EPb_args;
typedef std::vector<Expression*> EPb_stmts;

template<class T>
class EPbExpression : public Expression {
	public:
		EPbExpression(EPb_args *args) { m_args = args; }
		~EPbExpression() { delete T::args; delete T::stmts; delete T::value; delete m_args; }
		
		Value *evaluate() { return T::eval(m_args); }
		void doExp() { T::exec(m_args); }
		
	private:
		EPb_args *m_args;
};

EPb_args *EPb_arguments(int nbArgs, ...);
EPb_stmts *EPb_statements(int nbStmts, ...);

#define EPb_checkArgsNbr(min, max) if(args->size() > max || args->size() < min) yyerror("Unexpected number of arguments");

#define EPb_initStruct(Struct, val, action, nb, ...) \
	struct Struct { \
		static EPb_args *args; \
		static EPb_stmts *stmts; \
		static Value *ret; \
		static inline void exec(EPb_args *args) { \
			if(string(#action) != "") { \
				ret = new Value(action(args)); \
			} \
		} \
		static inline Value *eval(EPb_args *args) { \
			exec(args); \
			if(string(#val) != "") { \
				return new Value(val(args)); \
			} else { \
				if(ret) { \
					return new Value(*ret); \
				} else { \
					return new Value(); \
				} \
			} \
		} \
		static void init() { \
			beginScope(stFUNC); \
			args = EPb_arguments(nb, ##__VA_ARGS__); \
			stmts = EPb_statements(1, new ReturnExpression(new EPbExpression<Struct>(args))); \
			endScope(); \
		} \
	}; \
	EPb_initSSM(Struct);

std::string EPb_valToStr(Value *v);

#define EPb_getValStr(val) val->value<string>()

#define EPb_getVal(exp) exp->evaluate()

// getInt is like that because of the float bug...
#define EPb_getInt(exp) (int)getNumVal(exp->evaluate())
#define EPb_getNum(exp) getNumVal(exp->evaluate())
#define EPb_getStr(exp) exp->evaluate()->value<string>()
#define EPb_getArray(exp) exp->evaluate()->value< map<string, Value*> >()
#define EPb_getFile(exp) exp->evaluate()->value<File*>()

#define EPb_getArrayPtr(exp) exp->evaluate()->valuePtr< map<string, Value*> >()

#define EPb_initSSM(Struct) EPb_args *Struct::args = 0; \
							EPb_stmts *Struct::stmts = 0; \
							Value *Struct::ret = 0;

#define EPb_initFunc(Struct, name) Struct::init(); \
								   Variable *name = new Variable(#name, new Value(new Function(Struct::args, Struct::stmts)));

#define EPb_initElemFunc(table, Struct, name) Struct::init(); \
											  Value *name = new Value(new Function(Struct::args, Struct::stmts)); \
											  table.insert(table.end(), pair<string, Value*>(#name, name));

#endif // EPBLIB_TOOLS_H
