//OnFirstRowCommand.h
#ifndef _ONFIRSTROWCOMMAND_H
#define _ONFIRSTROWCOMMAND_H
#include"Command.h"

class NotePadForm;
class OnFirstRowCommand :public Command {
public:
	OnFirstRowCommand(NotePadForm* notePadForm);
	virtual ~OnFirstRowCommand();
	virtual void Execute();

};

#endif//_ONFIRSTROWCOMMAND_H
