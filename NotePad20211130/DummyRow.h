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
	DummyRow& operator=(const DummyRow& source); //치환연산자
	DummyRow(const DummyRow& source); //복사생성자

	virtual Glyph* Clone();//순수가상함수
	virtual string GetContent(); //순수가상함수

};



#endif//_ROW_H