//OnFindNextCommand.h (21.09.16. Ãß°¡)
#ifndef _ONFINDNEXTCOMMAND_H
#define _ONFINDNEXTCOMMAND_H
#include"Command.h"

class NotePadForm;
class OnFindNextCommand :public Command {
public:
	OnFindNextCommand(NotePadForm* notePadForm);
	virtual ~OnFindNextCommand();
	virtual void Execute();

};

#endif//_ONFINDNEXTCOMMAND_H


