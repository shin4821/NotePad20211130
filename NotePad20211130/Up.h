//Up.h
#ifndef _UP_H
#define _UP_H
#include"KeyAction.h"

class Up :public KeyAction {
public:
	Up(NotePadForm* notePadForm);
	virtual ~Up();
	virtual void KeyDown();
};

#endif //_UP_H

