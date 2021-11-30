//Subject.h (FOR Observe Pattern)
#ifndef _SUBJECT_H
#define _SUBJECT_H
#include<afxwin.h>
#include"Array.h"
typedef signed long int Long;

class Observer;
class Subject {
public:
	Subject(Long capacity = 256);
	virtual ~Subject()=0; //추상클래스
	Long Attach(Observer* observer);
	Long Detach(Observer* observer);  //Long index
	void Notify();


public:
	Array<Observer*>observers; //21.07.20 _ FOR Observe Pattern
	Long capacity;
	Long length;

};

int CompareObservers(void* one, void* other);


#endif//_SUBJECT_H







