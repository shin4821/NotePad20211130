//PageLeft.h
#ifndef _PAGELEFT_H
#define _PAGELEFT_H
#include"MoveAction.h"

class NotePadForm;
class PageLeft :public MoveAction {
public:
	PageLeft(NotePadForm* notePadForm);
	virtual ~PageLeft();
	virtual void KeyDown();
};


#endif//_LINERIGHT_H