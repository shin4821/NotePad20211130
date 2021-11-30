//EndRow.h
#ifndef _ENDROW_H
#define _ENDROW_H
#include"KeyAction.h"

class EndRow :public KeyAction {
public:
	EndRow(NotePadForm* notePadForm);
	virtual ~EndRow();
	virtual void KeyDown();

};

#endif//_ENDROW_H
