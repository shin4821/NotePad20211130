//PasteText.h
#ifndef _PASTETEXT_H
#define _PASTETEXT_H
#include"KeyAction.h"

class NotePadForm;
class PasteText : public KeyAction {
public:
	PasteText(NotePadForm* notePadForm);
	virtual ~PasteText();
	virtual void KeyDown();
};

#endif //_PASTETEXT_H
