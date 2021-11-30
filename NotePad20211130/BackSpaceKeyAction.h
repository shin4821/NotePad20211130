//BackSpaceKeyAction.h
#ifndef _BACKSPACEKEYACTION_H
#define _BACKSPACEKEYACTION_H
#include"KeyAction.h"

class BackSpaceKeyAction :public KeyAction {
public:
	BackSpaceKeyAction(NotePadForm* notePadForm);
	virtual ~BackSpaceKeyAction();
	virtual void KeyDown();
};
#endif//_BACKSPACEKEYACTION_H
