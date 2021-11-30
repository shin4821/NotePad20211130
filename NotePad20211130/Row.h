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
	Row& operator=(const Row& source); //ġȯ������
	Row(const Row& source); //���������

	virtual Glyph* Clone();//���������Լ�
	virtual string GetContent(); //���������Լ�
	virtual void Accept(Visitor* visitor); //���������Լ�
	virtual void AcceptForSelect(Visitor* visitor);

};



#endif//_ROW_H

