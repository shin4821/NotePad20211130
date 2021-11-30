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
	Note& operator=(const Note& source); //치환연산자
	Note(const Note& source); //복사생성자

	virtual Glyph* Clone();//순수가상함수
	virtual string GetContent(); //순수가상함수
	virtual void Accept(Visitor* visitor); //순수가상함수
	virtual void AcceptForSelect(Visitor* visitor);

};

#endif//_NOTE_H
