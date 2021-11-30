//Observer.h
#ifndef _OBSERVER_H
#define _OBSERVER_H
#include<afxwin.h>
typedef signed long int Long;

class Observer {
public:
	Observer();
	virtual ~Observer() = 0;
	virtual void Update()=0;
};

#endif//_OBSERVER_H


