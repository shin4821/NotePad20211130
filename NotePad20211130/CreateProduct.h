//CreateProduct.h (Factory Method Pattern : Concreate Create(하위클래스))
#ifndef _CREATEPRODUCT_H
#define _CREATEPRODUCT_H
#include"CreateGlyph.h"

class CreateProduct :public CreateGlyph {
public:
	CreateProduct();
	virtual ~CreateProduct();
	CreateProduct(const CreateProduct& source); //복사생성자
	CreateProduct& operator=(const CreateProduct& source); //치환연산자
	virtual Glyph* Make(char(*character));
};

#endif//_CREATEPRODUCT_H


