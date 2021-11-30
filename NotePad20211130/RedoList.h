//RedoList.h 
//(21.09.02 �߰�)
#ifndef _REDOLIST_H
#define _REDOLIST_H
#include"LinkedList.h"

class Command;
class RedoList {
public:
	RedoList();
	~RedoList();
	void Push(Command* command);
	Command* Pop();
	bool IsEmpty();
	int Size();
	Command* Top(); //���� ����� ���� ������ �ʰ� ���ϸ�.
	void DeleteAll();
	Long GetLength();
	void Check();

private:
	LinkedList<Command*> redoList;
	Long length;
	Command** current;

};

#endif//_UNDOLIST_H
