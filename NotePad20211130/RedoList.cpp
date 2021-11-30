//RedoList.cpp
#include"RedoList.h"
#include"Command.h"

RedoList::RedoList() {
	this->length = 0;
	this->current = 0;
}

RedoList::~RedoList() {

	//전체 command* 없앤다.
	LinkedList<Command*>::Node* node;
	LinkedList<Command*>::Node* previous = 0;
	Command* command;

	//1. 맨 앞에 데이터를 찾는다.
	int length = this->redoList.GetLength();
	node = this->redoList.First();

	while (node != previous) {
		command = node->GetObject();
		delete command;
		previous = node;
		node = this->redoList.Next();
	}

}

void RedoList::Push(Command* command) { //리스트의 맨 앞에 데이터 추가

	this->redoList.AppendFromHead(command);
	this->length++;
}

Command* RedoList::Pop() {
	LinkedList<Command*>::Node* node;
	Command* temp = 0;

	//1. 맨 앞에 데이터를 찾는다.
	node = this->redoList.First();

	//2. 데이터가 있으면 
	if (node != 0) {
		//2.1. 데이터를 출력한다.
		temp = node->GetObject(); //&

		//2.2. 해당 데이터를 지운다.
		this->redoList.Delete(node);
		this->length--;
	}

	return temp;
}

bool RedoList::IsEmpty() {
	bool isEmpty = true;

	Long length = this->redoList.GetLength();
	if (length > 0) {
		isEmpty = false;
	}

	return isEmpty;
}

int RedoList::Size() {
	return this->redoList.GetLength();
}

Command* RedoList::Top() {
	LinkedList<Command*>::Node* node;
	Command* command = 0;

	//1. 맨 앞에 데이터를 찾는다.
	node = this->redoList.First();

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


void RedoList::DeleteAll() {

	//모든 데이터를 지운다.
	this->redoList.DeleteAllItems();
}

Long RedoList::GetLength() {
	return this->redoList.GetLength();
}

void RedoList::Check() {
	LinkedList<Command*>::Node* node;
	LinkedList<Command*>::Node* previous = 0;
	Command* command;

	//1. 맨 앞에 데이터를 찾는다.
	int length = this->redoList.GetLength();
	node = this->redoList.First();

	while (node != previous) {
		command = node->GetObject();
		previous = node;
		node = this->redoList.Next();
	}
}
