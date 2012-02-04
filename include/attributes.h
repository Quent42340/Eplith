#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

typedef struct {
	int tab[2];
	bool sized;
	int length;
} Attr;

typedef struct {
	Attr addr;
	int value;
} DataRange;

extern std::vector<unsigned char> mem;

int catchAttr();
Attr catchAttrs();
int catchValue();
char* catchString();
DataRange catchDataRange();


#endif // ATTRIBUTES_H
