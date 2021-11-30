//OnSelectPreviousWordCommand.h
#ifndef _ONSELECTPREVIOUSWORDCOMMAND_H
#define _ONSELECTPREVIOUSWORDCOMMAND_H
#include"Command.h"

class NotePadForm;
class OnSelectPreviousWordCommand :public Command {
public:
	OnSelectPreviousWordCommand(NotePadForm* notePadForm);
	virtual ~OnSelectPreviousWordCommand();
	virtual void Execute();
};

#endif//_ONSELECTPREVIOUSWORDCOMMAND_H