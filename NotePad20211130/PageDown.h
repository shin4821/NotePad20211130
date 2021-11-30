//PageDown.h
#ifndef _PAGEDOWN_H
#define _PAGEDOWN_H
#include"MoveAction.h"

class NotePadForm;
class PageDown :public MoveAction {
public:
	PageDown(NotePadForm* notePadForm);
	virtual ~PageDown();
	virtual void KeyDown();
};


#endif//_LINEDOWN_H