//Down.h
#ifndef _DOWN_H
#define _DOWN_H
#include"KeyAction.h"

class Down :public KeyAction {
public:
	Down(NotePadForm* notePadForm);
	virtual ~Down();
	virtual void KeyDown();

};

#endif//_DOWN_H

