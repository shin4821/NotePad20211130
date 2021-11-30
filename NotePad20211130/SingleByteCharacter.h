//SingleByteCharacter.h
#ifndef _SINGLEBYTECHARACTER_H
#define _SINGLEBYTECHARACTER_H
#include"Character.h"
#include<string>

using namespace std;

class SingleByteCharacter :public Character {
public:
	SingleByteCharacter();
	virtual ~SingleByteCharacter();
	SingleByteCharacter(char content);
	char& GetContentt() const;
	bool IsEqual(const SingleByteCharacter& other);
	bool IsNotEqual(const SingleByteCharacter& other);
	bool operator==(const SingleByteCharacter& other);
	bool operator!=(const SingleByteCharacter& other);
	SingleByteCharacter(const SingleByteCharacter& source);//복사생성자
	SingleByteCharacter& operator=(const SingleByteCharacter& source); //치환연산자

	virtual Glyph* Clone();//순수가상함수
	virtual string GetContent(); //순수가상함수
	virtual void Accept(Visitor* visitor); //순수가상함수
	virtual void AcceptForSelect(Visitor* visitor);

private:
	char content;
};

inline char& SingleByteCharacter::GetContentt() const{
	return const_cast<char&>(this->content);
}







#endif//_SINGLEBYTECHARACTER_H

