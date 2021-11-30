//PageRight.h
#ifndef _PAGERIGHT_H
#define _PAGERIGHT_H
#include"MoveAction.h"

class NotePadForm;
class PageRight :public MoveAction {
public:
	PageRight(NotePadForm* notePadForm);
	virtual ~PageRight();
	virtual void KeyDown();
};


#endif//_LINERIGHT_H