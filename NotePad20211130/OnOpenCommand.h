//OnOpenCommand.h (20.09.07 Ãß°¡)
#ifndef _ONOPENCOMMAND_H
#define _ONOPENCOMMAND_H
#include"Command.h"

class NotePadForm;
class OnOpenCommand :public Command {
public:
	OnOpenCommand(NotePadForm* notePadForm);
	virtual ~OnOpenCommand();
	virtual void Execute();

};

#endif//_ONOPENCOMMAND_H
