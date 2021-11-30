//OnSelectEndDocCommand.h
#ifndef _ONSELECTENDDOCCOMMAND_H
#define _ONSELECTENDDOCCOMMAND_H
#include"Command.h"

class NotePadForm;
class OnSelectEndDocCommand :public Command {
public:
	OnSelectEndDocCommand(NotePadForm* notePadForm);
	~OnSelectEndDocCommand();
	virtual void Execute();
};

#endif//_ONSELECTENDDOCCOMMAND_H