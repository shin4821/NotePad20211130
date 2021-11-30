//OnNewCreateCommand.h
#ifndef _ONNEWCREATECOMMAND_H
#define _ONNEWCREATECOMMAND_H
#include"Command.h"

class NotePadForm;
class OnNewCreateCommand :public Command{
public:
	OnNewCreateCommand(NotePadForm* notePadForm);
	virtual ~OnNewCreateCommand();
	virtual void Execute();
};

#endif//_ONNEWCREATECOMMAND_H
