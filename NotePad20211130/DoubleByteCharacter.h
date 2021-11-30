//DoubleByteCharacter.h
#ifndef _DOUBLEBYTECHARACTER_H
#define _DOUBLEBYTECHARACTER_H
#include"Character.h"
#include<string>
using namespace std;

class Visitor;
class DoubleByteCharacter :public Character {
public:
	DoubleByteCharacter();
	DoubleByteCharacter(char(*content));
	virtual ~DoubleByteCharacter();
	//char* GetContent() const;
	bool IsEqual(const DoubleByteCharacter& other);
	bool IsNotEqual(const DoubleByteCharacter& other);
	bool operator==(const DoubleByteCharacter& other);
	bool operator!=(const DoubleByteCharacter& other);
	DoubleByteCharacter(const DoubleByteCharacter& source);
	DoubleByteCharacter& operator=(const DoubleByteCharacter& source);

	virtual Glyph* Clone();//���������Լ�
	virtual string GetContent(); //���������Լ�
	virtual void Accept(Visitor* visitor); //���������Լ�
	virtual void AcceptForSelect(Visitor* visitor);


private:
	char content[2];
};

#if 0
inline char* DoubleByteCharacter::GetContent() const {
	return const_cast<char*>(this->content);

}
#endif


#endif//_DOUBLEBYTECHARACTER_H

