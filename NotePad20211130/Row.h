//Row.h
#ifndef _ROW_H
#define _ROW_H
#include"Composite.h"
#include<string>
using namespace std;
typedef signed long int Long;

class Visitor;
class Row :public Composite {
public:
	Row(Long capacity = 256);
	virtual ~Row();
	Row& operator=(const Row& source); //치환연산자
	Row(const Row& source); //복사생성자

	virtual Glyph* Clone();//순수가상함수
	virtual string GetContent(); //순수가상함수
	virtual void Accept(Visitor* visitor); //순수가상함수
	virtual void AcceptForSelect(Visitor* visitor);

};



#endif//_ROW_H

