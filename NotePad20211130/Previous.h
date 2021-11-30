//Previous.h
#ifndef _PREVIOUS_H
#define _PREVIOUS_H
#include"KeyAction.h"
typedef signed long int Long;

class NotePadForm;
class Previous :public KeyAction {
public:
	Previous(NotePadForm* notePadForm);
	virtual ~Previous();
	virtual void KeyDown();
	


};

#endif//_PREVIOUS_H
