//NextWord.h
#ifndef _NEXTWORD_H
#define _NEXTWORD_H
#include"KeyAction.h"

class NextWord :public KeyAction {
public:
	NextWord(NotePadForm* notePadForm);
	virtual ~NextWord();
	virtual void KeyDown();
};

#endif //_NEXTWORD_H


