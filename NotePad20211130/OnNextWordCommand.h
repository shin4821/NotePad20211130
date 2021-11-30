//OnNextWordCommand.h
#ifndef _ONNEXTWORDCOMMAND_H
#define _ONNEXTWORDCOMMAND_H
#include"Command.h"

class NotePadForm;
class OnNextWordCommand :public Command {
public:
	OnNextWordCommand(NotePadForm* notePadForm);
	virtual ~OnNextWordCommand();
	virtual void Execute();

};

#endif//_ONNEXTWORDCOMMAND_H