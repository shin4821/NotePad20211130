//OnGetHelpCommand.h
#ifndef _ONGETHELPCOMMAND_H
#define _ONGETHELPCOMMAND_H
#include"Command.h"

class NotePadForm;
class OnGetHelpCommand :public Command {
public:
	OnGetHelpCommand(NotePadForm* notePadForm);
	virtual ~OnGetHelpCommand();
	virtual void Execute();
};

#endif//_ONGETHELPCOMMAND_H
