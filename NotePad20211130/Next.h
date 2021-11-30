//Next.h
#ifndef _NEXT_H
#define _NEXT_H
#include"KeyAction.h"
typedef signed long int Long;

class NotePadForm;
class Next :public KeyAction {
public:
	Next(NotePadForm* notePadForm);
	virtual ~Next();
	virtual void KeyDown();



};

#endif//_NEXT_H