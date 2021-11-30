//OnPreviewCommand.h
#ifndef _ONPREVIEWCOMMAND_H
#define _ONPREVIEWCOMMAND_H
#include"Command.h"

class Preview;
class NotePadForm;
class OnPreviewCommand :public Command {
public:
	OnPreviewCommand(NotePadForm* notePadForm);
	virtual ~OnPreviewCommand();
	virtual void Execute();

};

#endif//_ONPREVIEWCOMMAND_H



