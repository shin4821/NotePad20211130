//EndDocKeyAction.h
#ifndef _ENDDOCKEYACTION_H
#define _ENDDOCKEYACTIO_H
#include"KeyAction.h"

class EndDocKeyAction :public KeyAction {
public:
	EndDocKeyAction(NotePadForm* notePadForm);
	virtual ~EndDocKeyAction();
	virtual void KeyDown();

};

#endif//_ENDDOCKEYACTIO_H