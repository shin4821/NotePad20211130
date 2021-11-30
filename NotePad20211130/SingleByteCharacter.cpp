//SingleByteCharacter.cpp
#include"SingleByteCharacter.h"
#include"Visitor.h"

SingleByteCharacter::SingleByteCharacter() {
	this->content=' ';
}

SingleByteCharacter::SingleByteCharacter(char content) {
	this->content = content;
}

SingleByteCharacter::~SingleByteCharacter() {
}

bool SingleByteCharacter::IsEqual(const SingleByteCharacter& other) {
	bool ret = false;

	if (this->content == other.content) {
		ret = true;
	}

	return ret;
}

bool SingleByteCharacter::IsNotEqual(const SingleByteCharacter& other) {
	bool ret = false;

	if (this->content != other.content) {
		ret = true;
	}

	return ret;
}

bool SingleByteCharacter::operator==(const SingleByteCharacter& other) {
	bool ret = false;

	if (this->content == other.content) {
		ret = true;
	}

	return ret;
}

bool SingleByteCharacter::operator!=(const SingleByteCharacter& other) {
	bool ret = false;

	if (this->content != other.content) {
		ret = true;
	}

	return ret;
}

SingleByteCharacter::SingleByteCharacter(const SingleByteCharacter& source) {//���������
	this->content = source.content;
}

SingleByteCharacter& SingleByteCharacter::operator=(const SingleByteCharacter& source) {//ġȯ������
	this->content = source.content;

	return *this;
}

Glyph* SingleByteCharacter::Clone() {

	return new SingleByteCharacter(*this);
}

string SingleByteCharacter::GetContent() {
	string content;

	content = this->content;

	return content;
}


void SingleByteCharacter::Accept(Visitor* visitor) {

	visitor->VisitCharacter(this);
}

void SingleByteCharacter::AcceptForSelect(Visitor* visitor) {
}

#if 0
#include<iostream>
using namespace std;
int main(int argc, char* argv[]) {
	bool ret;

	SingleByteCharacter singleByteCharacter('A');
	cout << singleByteCharacter.GetContent() << endl;


	SingleByteCharacter singleByteCharacter1('B');

	cout << singleByteCharacter1.GetContent() << endl;

	SingleByteCharacter singleByteCharacter2('A');

	cout << singleByteCharacter2.GetContent() << endl;


	ret = singleByteCharacter.IsEqual(singleByteCharacter1);
	if (ret == true) {
		cout << "�����ϴ�." << endl;
	}
	else {
		cout << "�ٸ��ϴ�." << endl;
	}

	ret = singleByteCharacter.IsEqual(singleByteCharacter2);
	if (ret == true) {
		cout << "�����ϴ�." << endl;
	}
	else {
		cout << "�ٸ��ϴ�." << endl;
	}

	if (singleByteCharacter == singleByteCharacter2) {
		cout << "�����ϴ�." << endl;
	}

	SingleByteCharacter singleByteCharacter3(singleByteCharacter1);
	cout << singleByteCharacter3.GetContent() << endl;

	singleByteCharacter = singleByteCharacter1;
	cout << singleByteCharacter.GetContent() << endl;


	return 0;
}

#endif
