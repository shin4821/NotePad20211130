//CreateGlyph.h(Factory Method Pattern : Creator 상위(추상)클래스)
#ifndef _CREATEGLYPH_H
#define _CREATEGLYPH_H
#include"Glyph.h"


class CreateGlyph {
public:
	CreateGlyph();
	virtual ~CreateGlyph() = 0;//순수가상소멸자 
	CreateGlyph(const CreateGlyph& source); //복사생성자
	CreateGlyph& operator=(const CreateGlyph& source); //치환연산자
	virtual Glyph* Make(char(*character)) = 0; //순수가상함수

};

#endif//_CREATEGLYPH_H