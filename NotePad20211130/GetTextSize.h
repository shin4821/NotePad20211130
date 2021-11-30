//GetTextSize.h
#ifndef _GETTEXTSIZE_H
#define _GETTEXTSIZE_H
#include"Array.h"
#include<string>
using namespace std;
typedef signed long int Long;

class Glyph;
class NotePadForm;
class GetTextSize{
public:
	GetTextSize(NotePadForm* notePadForm, Long capacity = 256);
	~GetTextSize();
	GetTextSize(const GetTextSize& source); //복사생성자
	GetTextSize& operator=(const GetTextSize& source); //치환연산자
	int GetX(int asciiCode);
	void CalculateX(char(*character));
	Long totalCalculateX(Long endX, Glyph* row);
	Long totalCalculateEndX();
	int totalCalculateX(string character);
	Long GetMouseY(Long y);
	Long GetMouseX(Long x, Glyph* row);
	Long GetLeftX(Long currentX, Glyph* row);
	Long GetRightX(Long currentX, Glyph* row);
	Long GetLargestWidth();

private:
	Array<int> asciis;
	Long capacity;
	Long length;

public:
	NotePadForm* notePadForm;

};

#endif//_GETTEXTSIZE_H

