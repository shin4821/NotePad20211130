//FactoryProductScroll.h
#ifndef _FACTORYPRODUCTSCROLL_H
#define _FACTORYPRODUCTSCROLL_H
#include<afxwin.h>

class NotePadForm;
class MoveAction;
class FactoryProductScroll {
public:
	FactoryProductScroll(NotePadForm* notePadForm);
	~FactoryProductScroll();
	MoveAction* MakeHorizon(UINT nSBCode);
	MoveAction* MakeVertical(UINT nSBCode);

private:
	NotePadForm* notePadForm;
};

#endif //_FACTORYPRODUCTSCROLL_H
