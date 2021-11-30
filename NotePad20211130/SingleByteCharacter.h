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
	SingleByteCharacter(const SingleByteCharacter& source);//���������
	SingleByteCharacter& operator=(const SingleByteCharacter& source); //ġȯ������

	virtual Glyph* Clone();//���������Լ�
	virtual string GetContent(); //���������Լ�
	virtual void Accept(Visitor* visitor); //���������Լ�
	virtual void AcceptForSelect(Visitor* visitor);

private:
	char content;
};

inline char& SingleByteCharacter::GetContentt() const{
	return const_cast<char&>(this->content);
}







#endif//_SINGLEBYTECHARACTER_H

