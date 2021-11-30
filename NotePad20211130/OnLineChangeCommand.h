//OnLineChangeCommand.h (21.09.08 Ãß°¡)
#ifndef _ONLINECHANGECOMMAND_H
#define _ONLINECHANGECOMMAND_H
#include"Command.h"

class NotePadForm;
class OnLineChangeCommand :public Command {
public:
	OnLineChangeCommand(NotePadForm* notePadForm);
	virtual ~OnLineChangeCommand();
	virtual void Execute();
};

#endif //_ONLINECHANGECOMMAND_H
