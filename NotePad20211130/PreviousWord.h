//PreviousWord.h
#ifndef _PREVIOUSWORD_H
#define _PREVIOUSWORD_H
#include"KeyAction.h"
typedef signed long int Long;

class PreviousWord :public KeyAction {
public:
	PreviousWord(NotePadForm* notePadForm);
	virtual ~PreviousWord();
	virtual void KeyDown();


};


#endif//_KEYACTION_H
