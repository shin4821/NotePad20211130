//NextForSelected.h
#ifndef _NEXTFORSELECTED_H
#define _NEXTFORSELECTED_H
#include"KeyAction.h"
typedef signed long int Long;

class NotePadForm;
class NextForSelected :public KeyAction {
public:
	NextForSelected(NotePadForm* notePadForm);
	virtual ~NextForSelected();
	virtual void KeyDown();



};

#endif//_NEXTFORSELECTED_H