//OnCharCommand.h
//(21.08.31. Ãß°¡)

#ifndef _ONCHARCOMMAND_H
#define _ONCHARCOMMAND_H
#include"Command.h"

class NotePadForm;
class OnCharCommand : public Command {
public:
	OnCharCommand(NotePadForm* notePadForm); //, OnChar* onChar
	virtual ~OnCharCommand();
	virtual void Execute();
	virtual void Undo();
	virtual void Redo();
	virtual bool GetChecked();
	virtual Long GetCurrentX();
	virtual Long GetCurrentY();
	virtual void SetCheck();
	virtual bool GetLineChanged();
	virtual char GetCharacter();

private:
	bool isChecked;
	bool isLineChanged;
	bool isLineChangeButtonClicked;
	Long currentX;
	Long currentY;
	Long beforeCurrentX;
	Long beforeCurrentY;
	char character[2];
	bool isSelected;
	int windowWidth;


};

#endif//_ONCHARCOMMAND_H
