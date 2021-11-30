//OnTimeAndDateCommand.h
#ifndef _ONTIMEANDDATECOMMAND_H
#define _ONTIMEANDDATECOMMAND_H
#include"Command.h"

class NotePadForm;
class OnTimeAndDateCommand : public Command {
public:
	OnTimeAndDateCommand(NotePadForm* notePadForm);
	virtual ~OnTimeAndDateCommand();
	virtual void Execute();
	virtual void Undo();
	virtual void Redo();
	virtual void SetCheck();
	virtual bool GetChecked();
	virtual Long GetCurrentX();
	virtual Long GetCurrentY();
	virtual char GetCharacter();

private:
	int firstX;
	int firstY;
	int endX;
	int endY;
	int currentX;
	int currentY;
	bool isChecked;
	string totalDate;
	bool isLineChangeButtonClicked;
	int windowWidth;
};

#endif//_ONTIMEANDDATECOMMAND_H
