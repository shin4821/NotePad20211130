//SelectNext.h
#ifndef _SELECTNEXT_H
#define _SELECTNEXT_H
#include"KeyAction.h"

class NotePadForm;
class SelectNext: public KeyAction {
public:
	SelectNext(NotePadForm* notePadForm);
	virtual ~SelectNext();
	virtual void KeyDown();

};

#endif//_SELECTTEXT_H
