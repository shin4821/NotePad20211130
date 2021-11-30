//Top.h
#ifndef _TOP_H
#define _TOP_H
#include"MoveAction.h"

class NotePadForm;
class Top :public MoveAction {
public:
	Top(NotePadForm* notePadForm);
	virtual ~Top();
	virtual void KeyDown();
};


#endif//_TOP_H