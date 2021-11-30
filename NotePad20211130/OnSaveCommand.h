//OnSaveButtonClickedCommand.h (21.09.07 Ãß°¡)
#ifndef _ONSAVECOMMAND_H
#define _ONSAVECOMMAND_H
#include"Command.h"

class NotePadForm;
class OnSaveCommand :public Command {
public:
	OnSaveCommand(NotePadForm* notePadForm);
	virtual ~OnSaveCommand();
	virtual void Execute();

};

#endif //_ONSAVECOMMAND_H

