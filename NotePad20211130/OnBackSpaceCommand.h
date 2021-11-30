//OnBackSpaceCommand.h (21.09.08 Ãß°¡)
#ifndef _ONBACKSPACECOMMAND_H
#define _ONBACKSPACECOMMAND_H
#include"Command.h"

class NotePadForm;
class OnBackSpaceCommand :public Command {
public:
	OnBackSpaceCommand(NotePadForm* notePadForm);
	virtual ~OnBackSpaceCommand();
	virtual void Execute();
	virtual void Undo();
	virtual void Redo();
	virtual Long GetCurrentX();
	virtual Long GetCurrentY();
	virtual string GetCharacterForAll();
	virtual void SetCheck();
	virtual bool GetChecked();

private:
	Long currentX;
	Long currentY;
	string character;
	bool isChecked;
	bool isLineChangeButtonClicked;
	int firstX;
	int firstY;
	int endX;
	int endY;
	int windowWidth;

};
#endif //_ONBACKSPACECOMMAND_H

