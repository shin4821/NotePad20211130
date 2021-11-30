//DummyRow.h
#ifndef _DUMMYROW_H
#define _DUMMYROW_H
#include"Row.h"
#include<string>
using namespace std;
typedef signed long int Long;


class DummyRow :public Row {
public:
	DummyRow(Long capacity = 256);
	virtual ~DummyRow();
	DummyRow& operator=(const DummyRow& source); //ġȯ������
	DummyRow(const DummyRow& source); //���������

	virtual Glyph* Clone();//���������Լ�
	virtual string GetContent(); //���������Լ�

};



#endif//_ROW_H