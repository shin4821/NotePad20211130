//OnVScrollCommand.h(21.09.08 Ãß°¡)
#ifndef _ONVSCROLLCOMMAND_H
#define _ONVSCROLLCOMMAND_H
#include"Command.h"

class NotePadForm;
class OnVScrollCommand :public Command {
public:
	OnVScrollCommand(NotePadForm* notePadForm);
	~OnVScrollCommand();
	virtual void Execute();
};

#endif//_ONVSCROLLCOMMAND_H