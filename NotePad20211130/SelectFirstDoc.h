//SelectFirstDoc.h
#ifndef _SELECTFIRSTDOC_H
#define _SELECTFIRSTDOC_H
#include"KeyAction.h"

class NotePadForm;
class SelectFirstDoc : public KeyAction {
public:
	SelectFirstDoc(NotePadForm* notePadForm);
	virtual ~SelectFirstDoc();
	virtual void KeyDown();

};

#endif//_SELECTENDROW_H