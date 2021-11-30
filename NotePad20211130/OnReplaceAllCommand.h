//OnReplaceAllCommand.h
#ifndef _ONREPLACEALLCOMMAND_H
#define _ONREPLACEALLCOMMAND_H
#include"Command.h"

class NotePadForm;
class OnReplaceAllCommand :public Command {
public:
	OnReplaceAllCommand(NotePadForm* notePadForm);
	~OnReplaceAllCommand();
	virtual void Execute();
	virtual void Undo();
	virtual void Redo();
	virtual Long GetCurrentX();
	virtual Long GetCurrentY();
	virtual string GetCharacterForAll();
	virtual void SetCheck();
	virtual bool GetChecked();
	virtual bool GetIsReplaced();
	virtual bool GetIsFirst();
	virtual void SetIsFirst();

private:
	Long currentX;
	Long currentY;
	string findStr;
	string replaceStr;
	bool isChecked;
	bool isReplaced;
	int firstX;
	int firstY;
	int beforeEndX;
	int beforeEndY;
	int afterEndX;
	int afterEndY;
	bool isLineChangeButtonClicked; //(21.10.01 추가)
	int windowWidth; //(21.10.01 추가)
	//bool isFirst;
	bool isReplacedForMessage;

public:
	bool isFirst;
};

#endif//__ONREPLACEALLCOMMAND_H

