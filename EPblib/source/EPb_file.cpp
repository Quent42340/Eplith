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
#include "EPb_file.h"

using namespace std;

int File_close(EPb_args *args) {
	File *f = EPb_getFile((*args)[0]);
	return (int)fclose(f->f);
}

File *File_open(EPb_args *args) {
	string filename = EPb_getStr((*args)[0]);
	string mode = EPb_getStr((*args)[1]);
	File *f = new File;
	f->f = fopen(filename.c_str(), mode.c_str());
	f->fname = filename.c_str();
	return f;
}

string File_getc(EPb_args *args) {
	File *f = EPb_getFile((*args)[0]);
	std::ostringstream oss;
	oss << (char)fgetc(f->f);
	return oss.str();
}

string File_EOF() {
	std::ostringstream oss;
	oss << (char)EOF;
	return oss.str();
}

string File_getline(EPb_args *args) {
	File *f = EPb_getFile((*args)[0]);
	std::ostringstream oss;
	char c = fgetc(f->f);
	if(c == EOF) { return File_EOF(); }
	while(c != '\n') {
		oss << c;
		c = fgetc(f->f);
	}
	return oss.str();
}

EPb_initStruct(Close,, File_close, 1, "f");
EPb_initStruct(Open, File_open,, 2, "filename", "mode");
EPb_initStruct(Getc, File_getc,, 1, "f");
EPb_initStruct(Getline, File_getline,, 1, "f");

void EPblib_initFile() {
	map<string, Value*> File_elements;
	
	EPb_initElemFunc(File_elements, Close, close);
	EPb_initElemFunc(File_elements, Open, open);
	EPb_initElemFunc(File_elements, Getc, getc);
	EPb_initElemFunc(File_elements, Getline, getline);
	
	File_elements.insert(File_elements.end(), pair<string, Value*>("EOF", new Value(File_EOF())));
	
	Variable *File = new Variable("File", new Value(File_elements));
}

