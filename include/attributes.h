#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

typedef struct {
	int tab[2];
	bool sized;
	bool alone;
	int length;
} Attr;

typedef struct {
	Attr addr;
	int value;
} DataRange;

extern std::vector<unsigned char> mem;

int catchAttr();
Attr catchAttrs();
int catchValue(bool onlyValue = false);
char* catchString();
void setDataRangeValue(DataRange &dr, int value, bool noErrors = false);
int getDataRangeValue(DataRange &dr);
DataRange catchDataRange(bool onlyDR = false);

#endif // ATTRIBUTES_H
