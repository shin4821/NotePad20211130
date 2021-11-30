//OnImeCompositionCommand.h (21.09.06 �߰�)
#ifndef _ONIMECOMPOSITIONCOMMAND_H
#define _ONIMECOMPOSITIONCOMMAND_H
#include"Command.h"

class NotePadForm;
class OnImeCompositionCommand : public Command {
public:
	OnImeCompositionCommand(NotePadForm* notePadForm);
	virtual ~OnImeCompositionCommand();
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
	//Long beforeCurrentX; �������� �Ŵ� ���� currentX, Y�� �ʿ��ϴ�. 
	//Long beforeCurrentY;
	string character;

};

#endif//_ONIMECOMPOSITIONCOMMAND_H












