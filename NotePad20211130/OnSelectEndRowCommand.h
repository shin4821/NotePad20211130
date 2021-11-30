//OnSelectEndRowCommand.h
#ifndef _ONSELECTENDROWCOMMAND_H
#define _ONSELECTENDROWCOMMAND_H
#include"Command.h"

class NotePadForm;
class OnSelectEndRowCommand :public Command {
public:
	OnSelectEndRowCommand(NotePadForm* notePadForm);
	virtual ~OnSelectEndRowCommand();
	virtual void Execute();
};

#endif//_ONSELECTENDROWCOMMAND_H


