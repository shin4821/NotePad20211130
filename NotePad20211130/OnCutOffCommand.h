//OnCutOffCommand.h (21.09.14. Ãß°¡)
#ifndef _ONCUTOFFCOMMAND_H
#define _ONCUTOFFCOMMAND_H
#include"Command.h"

class NotePadForm;
class OnCutOffCommand :public Command {
public:
	OnCutOffCommand(NotePadForm* notePadForm);
	virtual ~OnCutOffCommand();
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
	int firstX;
	int firstY;
	int endX;
	int endY;
	bool isLineChangeButtonClicked;
	int windowWidth;
	string character;
};

#endif//_ONCUTOFFCOMMAND_H




