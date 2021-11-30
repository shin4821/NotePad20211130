//LineDown.h
#ifndef _LINEDOWN_H
#define _LINEDOWN_H
#include"MoveAction.h"

class NotePadForm;
class LineDown :public MoveAction {
public:
	LineDown(NotePadForm* notePadForm);
	virtual ~LineDown();
	virtual void KeyDown();
};


#endif//_LINEDOWN_H