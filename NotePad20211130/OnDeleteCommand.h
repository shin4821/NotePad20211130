//OnDeleteCommand.h (21.09.15 Ãß°¡)
#ifndef _ONDELETECOMMAND_H
#define _ONDELETECOMMAND_H
#include"Command.h"

class NotePadForm;
class OnDeleteCommand :public Command {
public:
	OnDeleteCommand(NotePadForm* notePadForm);
	virtual ~OnDeleteCommand();
	virtual void Execute();
	virtual void Undo();
	virtual void Redo();
	virtual Long GetCurrentX();
	virtual Long GetCurrentY();
	virtual void SetCheck();
	virtual bool GetChecked();

private:
	Long currentX;
	Long currentY;
	bool isChecked;
	string character;
	int firstX;
	int firstY;
	int endX;
	int endY;
	bool isLineChangeButtonClicked;
	int windowWidth;
};

#endif//_ONDELETECOMMAND_H


