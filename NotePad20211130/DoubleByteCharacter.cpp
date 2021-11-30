//DoubleByteCharacter.cpp
#include"DoubleByteCharacter.h"
#include"Visitor.h"

DoubleByteCharacter::DoubleByteCharacter() {
	this->content[0] = ' ';
	this->content[1] = ' ';
}

DoubleByteCharacter::~DoubleByteCharacter() {
}

DoubleByteCharacter::DoubleByteCharacter(char(*content)) { // �迭 �����ͷ� ���´�.
	this->content[0] = content[0];
	this->content[1] = content[1];
}

bool DoubleByteCharacter::IsEqual(const DoubleByteCharacter& other) {
	bool ret = false;

	if (this->content[0] == other.content[0] && this->content[1] == this->content[1]) {
		ret = true;
	}

	return ret;
}

bool DoubleByteCharacter::IsNotEqual(const DoubleByteCharacter& other) {
	bool ret = false;

	if (this->content[0] != other.content[0] || this->content[1] != this->content[1]) {
		ret = true;
	}

	return ret;
}

bool DoubleByteCharacter::operator==(const DoubleByteCharacter& other) {
	bool ret = false;

	if (this->content[0] == other.content[0] && this->content[1] == this->content[1]) {
		ret = true;
	}

	return ret;
}

bool DoubleByteCharacter::operator!=(const DoubleByteCharacter& other) {
	bool ret = false;

	if (this->content[0] != other.content[0] || this->content[1] != this->content[1]) {
		ret = true;
	}

	return ret;
}

DoubleByteCharacter::DoubleByteCharacter(const DoubleByteCharacter& source) {
	this->content[0] = source.content[0];
	this->content[1] = source.content[1];
}

DoubleByteCharacter& DoubleByteCharacter::operator=(const DoubleByteCharacter& source) {
	this->content[0] = source.content[0];
	this->content[1] = source.content[1];

	return *this;
}

Glyph* DoubleByteCharacter::Clone() {
	return new DoubleByteCharacter(*this);
}

string DoubleByteCharacter::GetContent(){
	char content_[3];
	content_[0] = this->content[0];
	content_[1] = this->content[1];
	content_[2] = '\0';

	string content = content_;
	string contents;
	contents.append(content);
	//string content(content_);

	return contents;
}


void DoubleByteCharacter::Accept(Visitor* visitor) {

	visitor->VisitCharacter(this);
}

void DoubleByteCharacter::AcceptForSelect(Visitor* visitor) {
}


#if 0
#include<iostream>
using namespace std;
int main(int argc, char* argv[]) {
	bool ret;

	DoubleByteCharacter doubleByteCharacter("��"); //�ѱ��� 2byte�̹Ƿ� ""���� �ؾߵȴ�. �ڿ� �ι��� ��� ���������.
	cout << doubleByteCharacter.GetContent() << endl;


	DoubleByteCharacter doubleByteCharacter1("��");
	cout << doubleByteCharacter1.GetContent() << endl;

	DoubleByteCharacter doubleByteCharacter2("��"); //�ѱ��� 2byte�̹Ƿ� ""���� �ؾߵȴ�. �ڿ� �ι��� ��� ���������.
	cout << doubleByteCharacter2.GetContent() << endl;


	ret = doubleByteCharacter.IsEqual(doubleByteCharacter2);
	if (ret == true) {
		cout << "�����ϴ�." << endl;
	}

	ret = doubleByteCharacter.IsNotEqual(doubleByteCharacter1);
	if (ret == true) {
		cout << "�ٸ��ϴ�." << endl;
	}

	if (doubleByteCharacter == doubleByteCharacter2) {
		cout << "�����ϴ�." << endl;
	}

	if (doubleByteCharacter != doubleByteCharacter1) {
		cout << "�ٸ��ϴ�." << endl;
	}

	DoubleByteCharacter doubleByteCharacter3(doubleByteCharacter1);
	cout << doubleByteCharacter3.GetContent() << endl;

	doubleByteCharacter = doubleByteCharacter3;
	cout << doubleByteCharacter.GetContent() << endl;

		

	return 0;
}

#endif













