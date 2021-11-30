//GetTextSizeForDpi.h
#ifndef _GETTEXTSIZEFORDPI_H
#define _GETTEXTSIZEFORDPI_H
#include"Array.h"

class Glyph;
class NotePadForm;
class GetTextSizeForDpi {
public:
	GetTextSizeForDpi(NotePadForm* notePadForm, Long capacity = 256);
	~GetTextSizeForDpi();
	int GetX(int asciiCode);
	void CalculateX(char(*character));
	Long totalCalculateX(Long endX, Glyph* row);

private:
	Array<int> asciis;
	Long capacity;
	Long length;

public:
	NotePadForm* notePadForm;
};

#endif //_GETTEXTSIZEFORDPI_H