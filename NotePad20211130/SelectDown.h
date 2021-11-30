//SelectDown.h
#ifndef _SELECTDOWN_H
#define _SELECTDOWN_H
#include"KeyAction.h"

class NotePadForm;
class SelectDown : public KeyAction {
public:
	SelectDown(NotePadForm* notePadForm);
	virtual ~SelectDown();
	virtual void KeyDown();

};

#endif//_SELECTDOWN_H