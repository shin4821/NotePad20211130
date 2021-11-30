//OnDownCommand.h
#ifndef _ONDOWNCOMMAND_H
#define _ONDOWNCOMMAND_H
#include"Command.h"

class NotePadForm;
class OnDownCommand :public Command {
public:
	OnDownCommand(NotePadForm* notePadForm);
	virtual ~OnDownCommand();
	virtual void Execute();

};

#endif//_ONUPCOMMAND_H