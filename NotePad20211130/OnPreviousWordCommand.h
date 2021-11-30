//OnPreviousWordCommand.h
#ifndef _ONPREVIOUSWORDCOMMAND_H
#define _ONPREVIOUSWORDCOMMAND_H
#include"Command.h"

class NotePadForm;
class OnPreviousWordCommand :public Command {
public:
	OnPreviousWordCommand(NotePadForm* notePadForm);
	virtual ~OnPreviousWordCommand();
	virtual void Execute();

};

#endif//_ONPREVIOUSWORDCOMMAND_H
