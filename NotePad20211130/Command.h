//Command.h
//(21.08.31.) 기존 연산들을 Command Pattern 적용하여 정리 시작
#ifndef _COMMAND_H
#define _COMMAND_H
#include<string>
typedef signed long int Long;
using namespace std;

class NotePadForm;
class Command {
public:
	Command(NotePadForm* notePadForm);
	virtual ~Command() = 0;//순수가상함수
	virtual void Execute();
	virtual void Undo();
	virtual void Redo();
	virtual Long Add(Command* command);
	virtual void Remove(Command* command);
	virtual bool GetChecked();
	virtual Long GetCurrentX();
	virtual Long GetCurrentY();
	virtual bool GetLineChanged();
	virtual void SetCheck();
	virtual char GetCharacter();
	//virtual WPARAM GetCharacterFor2Byte();
	virtual string GetCharacterForDoubleByte();
	virtual string GetCharacterForAll(); 
	virtual bool GetIsReplaced();
	virtual Long GetEndX();
	virtual void UnCheck();
	virtual bool GetIsFirst(); //OnReplaceAllCommand 전용
	virtual void SetIsFirst(); //OnReplaceAllCommand 전용

protected:
	NotePadForm* notePadForm;
#if 0
	bool isChecked;
	Long column;
#endif

};

#endif//_COMMAND_H
