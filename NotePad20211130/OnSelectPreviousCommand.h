//OnSelectPreviousCommand.h
#ifndef _ONSELECTPREVIOUSCOMMAND_H
#define _ONSELECTPREVIOUSCOMMAND_H
#include"Command.h"

class NotePadForm;
class OnSelectPreviousCommand :public Command {
public:
	OnSelectPreviousCommand(NotePadForm* notePadForm);
	virtual ~OnSelectPreviousCommand();
	virtual void Execute();
};

#endif//_ONSELECTPREVIOUSCOMMAND_H
