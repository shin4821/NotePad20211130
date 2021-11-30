//OnReplaceCommand.h (21.09.16. �߰�)
#ifndef _ONREPLACECOMMAND_H
#define _ONREPLACECOMMAND_H
#include"Command.h"

class NotePadForm;
class OnReplaceCommand :public Command {
public:
	OnReplaceCommand(NotePadForm* notePadForm);
	~OnReplaceCommand();
	virtual void Execute();
	virtual void Undo();
	virtual void Redo();
	virtual Long GetCurrentX();
	virtual Long GetCurrentY();
	virtual string GetCharacterForAll();
	virtual void SetCheck();
	virtual bool GetChecked();
	virtual bool GetIsReplaced();

private:
	Long currentX;
	Long currentY;
	string findStr;
	string replaceStr;
	bool isChecked;
	int firstX;
	int firstY;
	int beforeEndX;
	int beforeEndY;
	int afterEndX;
	int afterEndY;
	bool isReplaced; //(21.09.19 �߰�)
	bool isLineChangeButtonClicked; //(21.10.01 �߰�)
	int windowWidth; //(21.10.01 �߰�)

	

};

#endif//_ONREPLACECOMMAND_H