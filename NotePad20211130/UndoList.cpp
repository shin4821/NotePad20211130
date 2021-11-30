//UndoList.cpp
#include"UndoList.h"
#include"Command.h"

UndoList::UndoList() {
	this->length = 0;
	this->current = 0;
}

UndoList::~UndoList() {

	//��ü command* ���ش�.
	LinkedList<Command*>::Node* node;
	LinkedList<Command*>::Node* previous = 0;
	Command* command;

	//1. �� �տ� �����͸� ã�´�.
	int length = this->undoList.GetLength();
	node = this->undoList.First();

	while (node != previous) {
		command = node->GetObject();
		delete command;
		previous = node;
		node = this->undoList.Next();
	}
}

void UndoList::Push(Command* command) { //����Ʈ�� �� �տ� ������ �߰�

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

	//1. �� �տ� �����͸� ã�´�.
	node = this->undoList.First();

	//2. �����Ͱ� ������ 
	if (node != 0) {
		//2.1. �����͸� ����Ѵ�.
		temp = node->GetObject(); //&

		//2.2. �ش� �����͸� �����.
		this->undoList.Delete(node);
		this->length--;

#if 0
		//2.3. �� ó�� �����͸� ����Ѵ�.
		node = this->undoList.First();

		//2.4. �����Ͱ� ������ �ش� �����͸� ����Ѵ�.
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

	//1. �� �տ� �����͸� ã�´�.
	node = this->undoList.First();

	//2. �����Ͱ� ������ �ش� �����͸� ����Ѵ�.
	if (node != 0) {
		//this->current = &node->GetObject();
		command = node->GetObject();
	}

	//3. �����Ͱ� ������ 0�� ����Ѵ�.
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

	//1. �� �տ� �����͸� ã�´�.
	int length = this->undoList.GetLength();
	node = this->undoList.First();

	while (node != previous) {
		command = node->GetObject();
		previous = node;
		node = this->undoList.Next();
	}
}

void UndoList::DeleteAll() {

	//��� �����͸� �����.
	this->undoList.DeleteAllItems();
}

