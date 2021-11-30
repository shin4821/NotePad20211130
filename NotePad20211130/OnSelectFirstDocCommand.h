//OnSelectFirstDocCommand.h
#ifndef _ONSELECTFIRSTDOCCOMMAND_H
#define _ONSELECTFIRSTDOCCOMMAND_H
#include"Command.h"

class NotePadForm;
class OnSelectFirstDocCommand :public Command {
public:
	OnSelectFirstDocCommand(NotePadForm* notePadForm);
	virtual ~OnSelectFirstDocCommand();
	virtual void Execute();
};

#endif//_ONSELECTFIRSTDOCCOMMAND_H