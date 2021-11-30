//OnSelectNextCommand.h
#ifndef _ONSELECTNEXTCOMMAND_H
#define _ONSELECTNEXTCOMMAND_H
#include"Command.h"

class NotePadForm;
class OnSelectNextCommand :public Command {
public:
	OnSelectNextCommand(NotePadForm* notePadForm);
	virtual ~OnSelectNextCommand();
	virtual void Execute();
};

#endif//_ONSELECTNEXTCOMMAND_H