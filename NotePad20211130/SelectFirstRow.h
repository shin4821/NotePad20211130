//SelectFirstRow.h
#ifndef _SELECTFIRSTROW_H
#define _SELECTFIRSTROW_H
#include"KeyAction.h"

class NotePadForm;
class SelectFirstRow : public KeyAction {
public:
	SelectFirstRow(NotePadForm* notePadForm);
	virtual ~SelectFirstRow();
	virtual void KeyDown();

};

#endif//_SELECTFIRSTROW_H