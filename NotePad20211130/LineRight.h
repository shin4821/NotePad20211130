//LineRight.h
#ifndef _LINERIGHT_H
#define _LINERIGHT_H
#include"MoveAction.h"

class NotePadForm;
class LineRight :public MoveAction {
public:
	LineRight(NotePadForm* notePadForm);
	virtual ~LineRight();
	virtual void KeyDown();
};


#endif//_LINERIGHT_H

