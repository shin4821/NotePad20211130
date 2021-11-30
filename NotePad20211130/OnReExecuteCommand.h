//OnReExecuteCommand.h (21.09.05 Ãß°¡)
#ifndef _ONREEXECUTECOMMAND_H
#define _ONREEXECUTECOMMAND_H
#include"Command.h"

class OnReExecuteCommand :public Command {
public:
	OnReExecuteCommand(NotePadForm* notePadForm);
	virtual ~OnReExecuteCommand();
	virtual void Execute();
};

#endif//_ONREEXECUTECOMMAND_H
