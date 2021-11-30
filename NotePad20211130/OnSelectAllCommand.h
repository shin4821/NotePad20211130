//OnSelectAllCommand.h
#ifndef _ONSELECTALLCOMMAND_H
#define _ONSELECTALLCOMMAND_H
#include"Command.h"

class NotePadForm;
class OnSelectAllCommand :public Command {
public:
	OnSelectAllCommand(NotePadForm* notePadForm);
	virtual ~OnSelectAllCommand();
	virtual void Execute();
};

#endif//_ONSELECTALLCOMMAND_H
