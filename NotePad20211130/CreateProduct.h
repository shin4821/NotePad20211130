//CreateProduct.h (Factory Method Pattern : Concreate Create(����Ŭ����))
#ifndef _CREATEPRODUCT_H
#define _CREATEPRODUCT_H
#include"CreateGlyph.h"

class CreateProduct :public CreateGlyph {
public:
	CreateProduct();
	virtual ~CreateProduct();
	CreateProduct(const CreateProduct& source); //���������
	CreateProduct& operator=(const CreateProduct& source); //ġȯ������
	virtual Glyph* Make(char(*character));
};

#endif//_CREATEPRODUCT_H


