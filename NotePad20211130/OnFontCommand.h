//OnFontCommand.h (21.09.08 Ãß°¡)
#ifndef _ONFONTCOMMAND_H
#define _ONFONTCOMMAND_H
#include"Command.h"

class OnFontCommand :public Command {
public:
	OnFontCommand(NotePadForm* notePadForm);
	virtual ~OnFontCommand();
	virtual void Execute();
};

#endif//_ONFONTBUTTONCLICKECCOMMAND_H
