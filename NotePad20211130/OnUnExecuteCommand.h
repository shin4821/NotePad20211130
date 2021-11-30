//OnUnExecuteCommand.h
#ifndef _ONUNEXECUTECOMMAND_H
#define _ONUNEXECUTECOMMAND_H
#include"Command.h"

class NotePadForm;
class OnUnExecuteCommand : public Command {
public:
	OnUnExecuteCommand(NotePadForm* notePadForm);
	virtual ~OnUnExecuteCommand();
	virtual void Execute();

};

#endif//__ONUNEXECUTECOMMAND_H
