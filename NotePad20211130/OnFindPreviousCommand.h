//OnFindPreviousCommand.h (21.09.14. Ãß°¡)
#ifndef _ONFINDPREVIOUSCOMMAND_H
#define _ONFINDPREVIOUSCOMMAND_H
#include"Command.h"

class NotePadForm;
class OnFindPreviousCommand :public Command {
public:
	OnFindPreviousCommand(NotePadForm* notePadForm);
	virtual ~OnFindPreviousCommand();
	virtual void Execute();

};

#endif//_ONFINDPREVIOUSCOMMAND_H
