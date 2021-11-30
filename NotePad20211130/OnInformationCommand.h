//OnInformationCommand.h
#ifndef _ONINFORMATIONCOMMAND_H
#define _ONINFORMATIONCOMMAND_H
#include"Command.h"

class NotePadForm;
class OnInformationCommand:public Command {
public:
	OnInformationCommand(NotePadForm* notePadForm);
	virtual ~OnInformationCommand();
	virtual void Execute();
};

#endif//_ONINFORMATIONCOMMAND_H

