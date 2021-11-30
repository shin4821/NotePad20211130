//SelectNextWord.h
#ifndef _SELECTNEXTWORD_H
#define _SELECTNEXTWORD_H
#include"KeyAction.h"

class NotePadForm;
class SelectNextWord : public KeyAction {
public:
	SelectNextWord(NotePadForm* notePadForm);
	virtual ~SelectNextWord();
	virtual void KeyDown();

};

#endif//_SELECTNEXTWORD_H