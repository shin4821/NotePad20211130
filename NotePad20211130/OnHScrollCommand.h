//OnHScrollCommand.h(21.09.08 Ãß°¡)
#ifndef _ONHSCROLLCOMMAND_H
#define _ONHSCROLLCOMMAND_H
#include"Command.h"

class NotePadForm;
class OnHScrollCommand :public Command {
public:
	OnHScrollCommand(NotePadForm* notePadForm);
	~OnHScrollCommand();
	virtual void Execute();
};

#endif//_ONHSCROLLBUTTONCLICKEDCOMMAND_H

