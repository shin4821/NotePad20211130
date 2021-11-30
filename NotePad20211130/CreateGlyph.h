//CreateGlyph.h(Factory Method Pattern : Creator ����(�߻�)Ŭ����)
#ifndef _CREATEGLYPH_H
#define _CREATEGLYPH_H
#include"Glyph.h"


class CreateGlyph {
public:
	CreateGlyph();
	virtual ~CreateGlyph() = 0;//��������Ҹ��� 
	CreateGlyph(const CreateGlyph& source); //���������
	CreateGlyph& operator=(const CreateGlyph& source); //ġȯ������
	virtual Glyph* Make(char(*character)) = 0; //���������Լ�

};

#endif//_CREATEGLYPH_H