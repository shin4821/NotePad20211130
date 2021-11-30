//UndoList.h 
//(21.09.02 �߰�)
#ifndef _UNDOLIST_H
#define _UNDOLIST_H
#include"LinkedList.h"

class Command;
class UndoList {
public:
	UndoList();
	~UndoList();
	void Push(Command* command);
	Command* Pop(); //Command*
	bool IsEmpty();
	int Size();
	Command* Top(); //���� ����� ���� ������ �ʰ� ���ϸ�.
	Long GetLength();
	void Check();
	void DeleteAll();

private:
	LinkedList<Command*> undoList;
	Long length;
	Command** current;

};

#endif//_UNDOLIST_H
