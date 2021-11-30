//OnPrintCommand.h
#ifndef _ONPRINTCOMMAND_H
#define _ONPRINTCOMMAND_H
#include"Command.h"
typedef signed long int Long;

class NotePadForm;
class OnPrintCommand :public Command {
public:
	OnPrintCommand(NotePadForm* notePadForm);
	virtual ~OnPrintCommand();
	virtual void Execute();


};

#endif//_ONPRINTCOMMAND_H
