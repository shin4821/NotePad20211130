//DeleteKeyAction.h
#ifndef _DELETEKEYACTION_H
#define _DELETEKEYACTION_H
#include"KeyAction.h"

class DeleteKeyAction :public KeyAction {
public:
	DeleteKeyAction(NotePadForm* notePadForm);
	virtual ~DeleteKeyAction();
	virtual void KeyDown();
};

#endif//_DELETEKEYACTION_H

