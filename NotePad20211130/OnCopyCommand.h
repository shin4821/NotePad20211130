//OnCopyCommand.h (21.09.14. Ãß°¡)
#ifndef _ONCOPYCOMMAND_H
#define _ONCOPYCOMMAND_H
#include"Command.h"

class NotePadForm;
class OnCopyCommand : public Command {
public:
	OnCopyCommand(NotePadForm* notePadForm);
	virtual ~OnCopyCommand();
	virtual void Execute();
};

#endif//_ONCOPYCOMMAND_H



