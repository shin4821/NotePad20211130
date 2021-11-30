//FirstRow.h
#ifndef _FIRSTROW_H
#define _FIRSTROW_H
#include"KeyAction.h"

class FirstRow :public KeyAction {
public:
	FirstRow(NotePadForm* notePadForm);
	virtual ~FirstRow();
	virtual void KeyDown();
};

#endif //_FIRSTROW_H

