//OnFirstDocCommand.h
#ifndef _ONFIRSTDOCCOMMAND_H
#define _ONFIRSTDOCCOMMAND_H
#include"Command.h"

class NotePadForm;
class OnFirstDocCommand :public Command {
public:
	OnFirstDocCommand(NotePadForm* notePadForm);
	virtual ~OnFirstDocCommand();
	virtual void Execute();

};

#endif//_ONFIRSTDOCCOMMAND_H
