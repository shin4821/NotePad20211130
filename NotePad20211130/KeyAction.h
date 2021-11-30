//KeyAction.h
#ifndef _KEYACTION_H
#define _KEYACTION_H
#include<afxwin.h>

class NotePadForm;
class KeyAction {
public:
	KeyAction(NotePadForm* notePadForm);
	virtual ~KeyAction() = 0;
	virtual void KeyDown();

protected:  //protected:
	NotePadForm* notePadForm;
};

#endif//_KEYACTION_H








