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
#include <EPlib.h>
#include "EPb_tools.h"
#include "EPb_io.h"

using namespace std;

int IO_close(EPb_args *args) {
	File *f = EPb_getFile((*args)[0]);
	return (int)fclose(f->f);
}

File *IO_open(EPb_args *args) {
	string filename = EPb_getStr((*args)[0]);
	string mode = EPb_getStr((*args)[1]);
	File *f = new File;
	f->f = fopen(filename.c_str(), mode.c_str());
	f->fname = filename.c_str();
	return f;
}

string IO_getc(EPb_args *args) {
	File *f = EPb_getFile((*args)[0]);
	std::ostringstream oss;
	oss << (char)fgetc(f->f);
	return oss.str();
}

string IO_EOF() {
	std::ostringstream oss;
	oss << (char)EOF;
	return oss.str();
}

string IO_getline(EPb_args *args) {
	File *f = EPb_getFile((*args)[0]);
	std::ostringstream oss;
	char c = fgetc(f->f);
	if(c == EOF) return IO_EOF();
	while(c != '\n') {
		oss << c;
		c = fgetc(f->f);
	}
	return oss.str();
}

void *IO_flush(EPb_args *args) {
	std::cout.flush();
	return NULL;
}

EPb_initStruct(Close,, IO_close, 1, "f");
EPb_initStruct(Open, IO_open,, 2, "filename", "mode");
EPb_initStruct(Getc, IO_getc,, 1, "f");
EPb_initStruct(Getline, IO_getline,, 1, "f");
EPb_initStruct(Flush,, IO_flush, 0, "");

void EPblib_initIO() {
	map<string, Value*> IO_elements;
	
	EPb_initElemFunc(IO_elements, Close, close);
	EPb_initElemFunc(IO_elements, Open, open);
	EPb_initElemFunc(IO_elements, Getc, getc);
	EPb_initElemFunc(IO_elements, Getline, getline);
	
	IO_elements.insert(IO_elements.end(), pair<string, Value*>("EOF", new Value(IO_EOF())));
	
	Variable *IO = new Variable("IO", new Value(IO_elements));
}

