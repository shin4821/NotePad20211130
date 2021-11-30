//FactoryProduct.h
#ifndef _FACTORYPRODUCT_H
#define _FACTORYPRODUCT_H
#include<afxwin.h>

class NotePadForm;
class KeyAction;
class FactoryProduct {
public:
	FactoryProduct(NotePadForm* notePadForm);
	~FactoryProduct();
	KeyAction* Make(UINT nCharFirst, UINT nCharSecond, UINT nCharThird);

private:
	NotePadForm* notePadForm;
};

#endif//_FACTORYPRODUCT_H

