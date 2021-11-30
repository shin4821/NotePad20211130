//CutOffText.h
#ifndef _CUTOFFTEXT_H
#define _CUTOFFTEXT_H
#include"KeyAction.h"

class NotePadForm;
class CutOffText : public KeyAction {
public:
	CutOffText(NotePadForm* notePadForm);
	virtual ~CutOffText();
	virtual void KeyDown();
};

#endif //_CUTOFFTEXT_H















