//SelectEndDoc.h
#ifndef _SELECTENDDOC_H
#define _SELECTENDDOC_H
#include"KeyAction.h"

class NotePadForm;
class SelectEndDoc : public KeyAction {
public:
	SelectEndDoc(NotePadForm* notePadForm);
	virtual ~SelectEndDoc();
	virtual void KeyDown();

};

#endif//_SELECTENDDOC_H