//PreviousForSelected.h
#ifndef _PREVIOUSFORSELECTED_H
#define _PREVIOUSFORSELECTED_H
#include"KeyAction.h"
typedef signed long int Long;

class NotePadForm;
class PreviousForSelected :public KeyAction {
public:
	PreviousForSelected(NotePadForm* notePadForm);
	virtual ~PreviousForSelected();
	virtual void KeyDown();
};

#endif//_PREVIOUSFORSELECTED_H
