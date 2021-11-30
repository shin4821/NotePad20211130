//OnReplaceDialogCommand.h (21.09.15. Ãß°¡)
#ifndef _OnReplaceDialogCommand_H
#define _OnReplaceDialogCommand_H
#include"Command.h"

class NotePadForm;
class OnReplaceDialogCommand :public Command {
public:
	OnReplaceDialogCommand(NotePadForm* notePadForm);
	~OnReplaceDialogCommand();
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
	int firstX;
	int firstY;
	int endX;
	int endY;
};

#endif//_OnReplaceDialogCommand_H
