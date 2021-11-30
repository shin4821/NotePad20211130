//SelectEndRow.h
#ifndef _SELECTENDROW_H
#define _SELECTENDROW_H
#include"KeyAction.h"

class NotePadForm;
class SelectEndRow : public KeyAction {
public:
	SelectEndRow(NotePadForm* notePadForm);
	virtual ~SelectEndRow();
	virtual void KeyDown();

};

#endif//_SELECTENDROW_H