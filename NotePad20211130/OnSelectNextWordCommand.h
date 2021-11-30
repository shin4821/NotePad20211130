//OnSelectNextWordCommand.h
#ifndef _ONSELECTNEXTWORDCOMMAND_H
#define _ONSELECTNEXTWORDCOMMAND_H
#include"Command.h"

class NotePadForm;
class OnSelectNextWordCommand :public Command {
public:
	OnSelectNextWordCommand(NotePadForm* notePadForm);
	virtual ~OnSelectNextWordCommand();
	virtual void Execute();
};

#endif//_ONSELECTNEXTWORDCOMMAND_H

