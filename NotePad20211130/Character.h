//Character.h
#ifndef _CHARACTER_H
#define _CHARACTER_H
#include"Glyph.h"

class Character:public Glyph {
public:
	Character();
	virtual ~Character() = 0;
	Character(const Character& source); //복사생성자
	Character& operator=(const Character& source); // 치환연산자, 추상메소드


};

#endif//_CHARACTER_H


