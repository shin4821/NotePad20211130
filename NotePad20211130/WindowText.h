//WindowText.h
#ifndef _WINDOWTEXT_H
#define _WINDOWTEXT_H
#include"Observer.h"

class NotePadForm;
class WindowText : public Observer {
public:
	WindowText(NotePadForm* notePadForm);
	virtual ~WindowText();
	void UpdateWindowText();
	virtual void Update();

private:
	NotePadForm* notePadForm;

};

#endif//_WINDOWTEXT_H


