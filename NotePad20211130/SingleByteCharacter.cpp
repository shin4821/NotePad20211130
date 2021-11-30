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

SingleByteCharacter::SingleByteCharacter(const SingleByteCharacter& source) {//복사생성자
	this->content = source.content;
}

SingleByteCharacter& SingleByteCharacter::operator=(const SingleByteCharacter& source) {//치환연산자
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
		cout << "같습니다." << endl;
	}
	else {
		cout << "다릅니다." << endl;
	}

	ret = singleByteCharacter.IsEqual(singleByteCharacter2);
	if (ret == true) {
		cout << "같습니다." << endl;
	}
	else {
		cout << "다릅니다." << endl;
	}

	if (singleByteCharacter == singleByteCharacter2) {
		cout << "같습니다." << endl;
	}

	SingleByteCharacter singleByteCharacter3(singleByteCharacter1);
	cout << singleByteCharacter3.GetContent() << endl;

	singleByteCharacter = singleByteCharacter1;
	cout << singleByteCharacter.GetContent() << endl;


	return 0;
}

#endif
