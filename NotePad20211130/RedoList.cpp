//RedoList.cpp
#include"RedoList.h"
#include"Command.h"

RedoList::RedoList() {
	this->length = 0;
	this->current = 0;
}

RedoList::~RedoList() {

	//��ü command* ���ش�.
	LinkedList<Command*>::Node* node;
	LinkedList<Command*>::Node* previous = 0;
	Command* command;

	//1. �� �տ� �����͸� ã�´�.
	int length = this->redoList.GetLength();
	node = this->redoList.First();

	while (node != previous) {
		command = node->GetObject();
		delete command;
		previous = node;
		node = this->redoList.Next();
	}

}

void RedoList::Push(Command* command) { //����Ʈ�� �� �տ� ������ �߰�

	this->redoList.AppendFromHead(command);
	this->length++;
}

Command* RedoList::Pop() {
	LinkedList<Command*>::Node* node;
	Command* temp = 0;

	//1. �� �տ� �����͸� ã�´�.
	node = this->redoList.First();

	//2. �����Ͱ� ������ 
	if (node != 0) {
		//2.1. �����͸� ����Ѵ�.
		temp = node->GetObject(); //&

		//2.2. �ش� �����͸� �����.
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

	//1. �� �տ� �����͸� ã�´�.
	node = this->redoList.First();

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


void RedoList::DeleteAll() {

	//��� �����͸� �����.
	this->redoList.DeleteAllItems();
}

Long RedoList::GetLength() {
	return this->redoList.GetLength();
}

void RedoList::Check() {
	LinkedList<Command*>::Node* node;
	LinkedList<Command*>::Node* previous = 0;
	Command* command;

	//1. �� �տ� �����͸� ã�´�.
	int length = this->redoList.GetLength();
	node = this->redoList.First();

	while (node != previous) {
		command = node->GetObject();
		previous = node;
		node = this->redoList.Next();
	}
}
