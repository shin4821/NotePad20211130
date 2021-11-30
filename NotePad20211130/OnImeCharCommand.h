//OnImeCharCommand.h (21.09.06 Ãß°¡)
#ifndef _ONIMECHARCOMMAND_H
#define _ONIMECHARCOMMAND_H
#include"Command.h"

class NotePadForm;
class OnImeCharCommand :public Command {
public:
	OnImeCharCommand(NotePadForm* notePadForm);
	virtual ~OnImeCharCommand();
	virtual void Execute();
	virtual void Undo();
	virtual void Redo();
	virtual bool GetChecked();
	virtual Long GetCurrentX();
	virtual Long GetCurrentY();
	virtual void SetCheck();
	virtual bool GetLineChanged();
    virtual string GetCharacterForDoubleByte();

private:
	bool isChecked;
	bool isLineChanged;
	Long currentX;
	Long currentY;
	Long beforeCurrentX;
	Long beforeCurrentY;
	string character;
	bool isRedo;
	bool isLineChangeButtonClicked;
	int windowWidth;

};

#endif//_ONIMECHARCOMMAND_H

