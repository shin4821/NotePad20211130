//FisrtDoc.h
#ifndef _FIRSTDOC_H
#define _FIRSTDOC_H
#include"KeyAction.h"

class FirstDoc :public KeyAction {
public:
	FirstDoc(NotePadForm* notePadForm);
	virtual ~FirstDoc();
	virtual void KeyDown();

};

#endif//_FIRSTDOC_H

