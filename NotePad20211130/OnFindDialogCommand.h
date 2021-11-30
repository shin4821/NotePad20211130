//OnFindDialogCommand.h (21.09.15. Ãß°¡)
#ifndef _OnFindDialogCommand_H
#define _OnFindDialogCommand_H
#include"Command.h"

class NotePadForm;
class OnFindDialogCommand :public Command {
public:
	OnFindDialogCommand(NotePadForm* notePadForm);
	virtual ~OnFindDialogCommand();
	void virtual Execute();
};

#endif //_OnFindDialogCommand_H

