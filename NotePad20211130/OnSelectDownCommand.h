//OnSelectDownCommand.h
#ifndef _ONSELECTDOWNCOMMAND_H
#define _ONSELECTDOWNCOMMAND_H
#include"Command.h"

class NotePadForm;
class OnSelectDownCommand :public Command {
public:
	OnSelectDownCommand(NotePadForm* notePadForm);
	virtual ~OnSelectDownCommand();
	virtual void Execute();
};

#endif//_ONSELECTDOWNCOMMAND_H

