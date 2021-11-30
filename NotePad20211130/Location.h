//Location.h
#ifndef _LOCATION_H
#define _LOCATION_H
#include<afxwin.h>
#include"Observer.h"
typedef signed long int Long;

class Caret;
class GetTextSize;
class NotePadForm;

class Location: public Observer {
public:
	Location(NotePadForm* notePadForm);
	virtual ~Location();
	//void MoveCaret(char(*character), CDC* pdc, CString character_);
	void MoveCaret();
	//void CalculateX(char(*character));
	Long GetX();
	Long GetY();
	virtual void Update(); //21.07.20 _ FOR Observe Pattern


public:
	Caret* caret;
	//GetTextSize* getTextSize;
	NotePadForm* notePadForm;



};

#endif//_LOCATION_H
