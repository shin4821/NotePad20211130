//OnEndRowCommand.h
#ifndef _ONENDROWCOMMAND_H
#define _ONENDROWCOMMAND_H
#include"Command.h"

class NotePadForm;
class OnEndRowCommand :public Command {
public:
	OnEndRowCommand(NotePadForm* notePadForm);
	virtual ~OnEndRowCommand();
	virtual void Execute();

};

#endif//_ONENDROWCOMMAND_H

