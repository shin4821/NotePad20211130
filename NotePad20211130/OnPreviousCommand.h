//OnPreviousCommand.h
#ifndef _ONPREVIOUSCOMMAND_H
#define _ONPREVIOUSCOMMAND_H
#include"Command.h"

class NotePadForm;
class OnPreviousCommand :public Command {
public:
	OnPreviousCommand(NotePadForm* notePadForm);
	virtual ~OnPreviousCommand();
	virtual void Execute();
};

#endif//_ONPREVIOUSCOMMAND_H
