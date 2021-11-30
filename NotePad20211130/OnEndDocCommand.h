//OnEndDocCommand.h
#ifndef _ONENDDOCCOMMAND_H
#define _ONENDDOCCOMMAND_H
#include"Command.h"

class NotePadForm;
class OnEndDocCommand :public Command {
public:
	OnEndDocCommand(NotePadForm* notePadForm);
	virtual ~OnEndDocCommand();
	virtual void Execute();

};

#endif//_ONENDDOCCOMMAND_H
