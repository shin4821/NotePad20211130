//UndoList.cpp
#include"UndoList.h"
#include"Command.h"

UndoList::UndoList() {
	this->length = 0;
	this->current = 0;
}

UndoList::~UndoList() {

	//전체 command* 없앤다.
	LinkedList<Command*>::Node* node;
	LinkedList<Command*>::Node* previous = 0;
	Command* command;

	//1. 맨 앞에 데이터를 찾는다.
	int length = this->undoList.GetLength();
	node = this->undoList.First();

	while (node != previous) {
		command = node->GetObject();
		delete command;
		previous = node;
		node = this->undoList.Next();
	}
}

void UndoList::Push(Command* command) { //리스트의 맨 앞에 데이터 추가

	this->undoList.AppendFromHead(command);
	this->length++;

#if 0
	LinkedList<Command*>::Node* node = this->undoList.First();
	while (node != NULL) {
		node = this->undoList.Next();
	}
#endif
}

Command* UndoList::Pop() { //Command*
	LinkedList<Command*>::Node* node;
	Command* temp = 0;

	//1. 맨 앞에 데이터를 찾는다.
	node = this->undoList.First();

	//2. 데이터가 있으면 
	if (node != 0) {
		//2.1. 데이터를 출력한다.
		temp = node->GetObject(); //&

		//2.2. 해당 데이터를 지운다.
		this->undoList.Delete(node);
		this->length--;

#if 0
		//2.3. 맨 처음 데이터를 출력한다.
		node = this->undoList.First();

		//2.4. 데이터가 있으면 해당 데이터를 출력한다.
		if (node != 0) {
			this->current = &node->GetObject();
		}
		else {
			this->current = 0;
		}
#endif
	}

	return temp;
}

bool UndoList::IsEmpty() {
	bool isEmpty = true;

	Long length = this->undoList.GetLength();
	if (length > 0) {
		isEmpty = false;
	}

	return isEmpty;
}

int UndoList::Size() {
	return this->undoList.GetLength();
}

Command* UndoList::Top() {
	LinkedList<Command*>::Node* node;
	Command* command = 0;

	//1. 맨 앞에 데이터를 찾는다.
	node = this->undoList.First();

	//2. 데이터가 있으면 해당 데이터를 출력한다.
	if (node != 0) {
		//this->current = &node->GetObject();
		command = node->GetObject();
	}

	//3. 데이터가 없으면 0을 출력한다.
	else {
		this->current = 0;
	}

	//return *this->current;
	return command;
}


Long UndoList::GetLength() {

	return this->undoList.GetLength();
}

void UndoList::Check() {
	LinkedList<Command*>::Node* node;
	LinkedList<Command*>::Node* previous = 0;
	Command* command;

	//1. 맨 앞에 데이터를 찾는다.
	int length = this->undoList.GetLength();
	node = this->undoList.First();

	while (node != previous) {
		command = node->GetObject();
		previous = node;
		node = this->undoList.Next();
	}
}

void UndoList::DeleteAll() {

	//모든 데이터를 지운다.
	this->undoList.DeleteAllItems();
}

