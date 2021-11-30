//LineLeft.h
#ifndef _LINELEFT_H
#define _LINELEFT_H
#include"MoveAction.h"

class NotePadForm;
class LineLeft :public MoveAction {
public:
	LineLeft(NotePadForm* notePadForm);
	virtual ~LineLeft();
	virtual void KeyDown();
};


#endif//_LINERIGHT_H

