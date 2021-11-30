//UndoList.h 
//(21.09.02 추가)
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
	Command* Top(); //제일 상단의 값을 꺼내지 않고 리턴만.
	Long GetLength();
	void Check();
	void DeleteAll();

private:
	LinkedList<Command*> undoList;
	Long length;
	Command** current;

};

#endif//_UNDOLIST_H
