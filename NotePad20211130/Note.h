//Note.h
#ifndef _NOTE_H
#define _NOTE_H
#include"Composite.h"
#include<string>
using namespace std;
typedef signed long int Long;

class Visitor;
class Note :public Composite {
public:
	Note(Long capacith = 256);
	virtual ~Note();
	Note& operator=(const Note& source); //ġȯ������
	Note(const Note& source); //���������

	virtual Glyph* Clone();//���������Լ�
	virtual string GetContent(); //���������Լ�
	virtual void Accept(Visitor* visitor); //���������Լ�
	virtual void AcceptForSelect(Visitor* visitor);

};

#endif//_NOTE_H
