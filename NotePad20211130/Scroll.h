//Scroll.h
#ifndef _SCROLL_H
#define _SCROLL_H
#include"Observer.h"

class NotePadForm;
class Scroll :public Observer {
public:
	Scroll(NotePadForm* notePadForm);
	virtual ~Scroll();
	void MovePageAndThumb();
	void MovePage(); //21.08.27 추가.
	void Create(); //21.09.10 추가.
	virtual void Update();

public:
	NotePadForm* notePadForm;
	SCROLLINFO siHorizon;
    SCROLLINFO siVertical;
	//BOOL enableScrollBarX;
	//BOOL enableScrollBarY;
	int beforeX;
	int beforeY;
	int beforeWindowWidth;
	int windowWidth;
	int windowHeight;
	int beforePageX;
	int beforePageY;
	int beforeCurrentY;
};

#endif //_SCROLL_H
