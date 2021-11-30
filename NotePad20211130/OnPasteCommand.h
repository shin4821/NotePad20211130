//OnPasteCommand.h
#ifndef _ONPASTECOMMAND_H
#define _ONPASTECOMMAND_H
#include"Command.h"

class NotePadForm;
class OnPasteCommand :public Command {
public:
	OnPasteCommand(NotePadForm* notePadForm);
	virtual ~OnPasteCommand();
	virtual void Execute();
	virtual void Undo();
	virtual void Redo();
	virtual Long GetCurrentX();
	virtual Long GetCurrentY();
	virtual void SetCheck();
	virtual void UnCheck();
	virtual bool GetChecked();
	virtual Long GetEndX();

private:
	Long currentX;
	Long currentY;
	bool isChecked;
	int firstX;
	int firstY;
	int endX;
	int endY;
	int windowWidth;
	bool isLineChangeButtonClicked;
	string character;
	bool isRedo;
};

#endif//_ONPASTECOMMAND_H


