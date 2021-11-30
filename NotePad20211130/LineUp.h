//LineUp.h
#ifndef _LINEUP_H
#define _LINEUP_H
#include"MoveAction.h"

class NotePadForm;
class LineUp :public MoveAction {
public:
	LineUp(NotePadForm* notePadForm);
	virtual ~LineUp();
	virtual void KeyDown();
};


#endif//_LINEUP_H