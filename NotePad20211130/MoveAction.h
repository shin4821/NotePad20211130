//MoveAction.h
#ifndef _MOVEACTION_H
#define _MOVEACTION_H
#include<afxwin.h>

class NotePadForm;
class MoveAction {
public:
	MoveAction(NotePadForm* notePadForm);
	virtual ~MoveAction()=0;
	virtual void KeyDown();

protected:
	NotePadForm* notePadForm;
};

#endif//_MOVEACTION_H
