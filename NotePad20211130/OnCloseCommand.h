//OnCloseCommand.h (21.09.08 Ãß°¡)
#ifndef _ONCLOSECOMMAND_H
#define _ONCLOSECOMMAND_H
#include"Command.h"

class NotePadForm;
class OnCloseCommand :public Command {
public:
	OnCloseCommand(NotePadForm* notePadForm);
	virtual ~OnCloseCommand();
	virtual void Execute();
};

#endif//_ONCLOSEBUTTONCLICKEDCOMMAND_H


