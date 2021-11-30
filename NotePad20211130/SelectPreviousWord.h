//SelectPreviousWord.h
#ifndef _SELECTPREVIOUSWORD_H
#define _SELECTPREVIOUSWORD_H
#include"KeyAction.h"

class NotePadForm;
class SelectPreviousWord : public KeyAction {
public:
	SelectPreviousWord(NotePadForm* notePadForm);
	virtual ~SelectPreviousWord();
	virtual void KeyDown();

};

#endif//_SELECTPREVIOUSWORD_H