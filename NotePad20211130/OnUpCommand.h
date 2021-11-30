//OnUpCommand.h
#ifndef _ONUPCOMMAND_H
#define _ONUPCOMMAND_H
#include"Command.h"

class NotePadForm;
class OnUpCommand :public Command {
public:
	OnUpCommand(NotePadForm* notePadForm);
	virtual ~OnUpCommand();
	virtual void Execute();

};

#endif//_ONUPCOMMAND_H









