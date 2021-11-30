//PageUp.h
#ifndef _PAGEUP_H
#define _PAGEUP_H
#include"MoveAction.h"

class NotePadForm;
class PageUp :public MoveAction {
public:
	PageUp(NotePadForm* notePadForm);
	virtual ~PageUp();
	virtual void KeyDown();
};


#endif//_LINEUP_H