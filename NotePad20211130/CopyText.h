//CopyText.h
#ifndef _COPYTEXT_H
#define _COPYTEXT_H
#include"KeyAction.h"

class NotePadForm;
class CopyText : public KeyAction {
public:
	CopyText(NotePadForm* notePadForm);
	virtual ~CopyText();
	virtual void KeyDown();
};

#endif//_COPYTEXT_H














