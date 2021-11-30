//OnSelectFirstRowCommand.h
#ifndef _ONSELECTFIRSTROWCOMMAND_H
#define _ONSELECTFIRSTROWCOMMAND_H
#include"Command.h"

class NotePadForm;
class OnSelectFirstRowCommand :public Command {
public:
	OnSelectFirstRowCommand(NotePadForm* notePadForm);
	virtual ~OnSelectFirstRowCommand();
	virtual void Execute();
};

#endif//_ONSELECTFIRSTROWCOMMAND_H
