//Character.h
#ifndef _CHARACTER_H
#define _CHARACTER_H
#include"Glyph.h"

class Character:public Glyph {
public:
	Character();
	virtual ~Character() = 0;
	Character(const Character& source); //���������
	Character& operator=(const Character& source); // ġȯ������, �߻�޼ҵ�


};

#endif//_CHARACTER_H


