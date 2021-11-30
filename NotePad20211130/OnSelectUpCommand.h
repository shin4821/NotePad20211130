//OnSelectUpCommand.h
#ifndef _ONSELECTUPCOMMAND_H
#define _ONSELECTUPCOMMAND_H
#include"Command.h"

class NotePadForm;
class OnSelectUpCommand :public Command {
public:
	OnSelectUpCommand(NotePadForm* notePadForm);
	virtual ~OnSelectUpCommand();
	virtual void Execute();
};

#endif//_ONSELECTUPCOMMAND_H