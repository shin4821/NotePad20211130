//SelectUp.h
#ifndef _SELECTUP_H
#define _SELECTUP_H
#include"KeyAction.h"

class NotePadForm;
class SelectUp : public KeyAction {
public:
	SelectUp(NotePadForm* notePadForm);
	virtual ~SelectUp();
	virtual void KeyDown();

};

#endif//_SELECTTEXT_H