//SelectPrevious.h
#ifndef _SELECTPREVIOUS_H
#define _SELECTPREVIOUS_H
#include"KeyAction.h"

class NotePadForm;
class SelectPrevious : public KeyAction {
public:
	SelectPrevious(NotePadForm* notePadForm);
	virtual ~SelectPrevious();
	virtual void KeyDown();

};

#endif//_SELECTTEXT_H