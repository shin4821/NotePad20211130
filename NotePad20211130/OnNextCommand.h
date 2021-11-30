//OnNextCommand.h
#ifndef _ONNEXTCOMMAND_H
#define _ONNEXTCOMMAND_H
#include"Command.h"

class NotePadForm;
class OnNextCommand :public Command {
public:
	OnNextCommand(NotePadForm* notePadForm);
	virtual ~OnNextCommand();
	virtual void Execute();
};

#endif//_ONNEXTCOMMAND_H

