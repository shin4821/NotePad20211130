//ThumbTrackHorizon.h
#ifndef _THUMBTRACKHORIZON_H
#define _THUMBTRACKHORIZON_H
#include"MoveAction.h"

class NotePadForm;
class ThumbTrackHorizon :public MoveAction {
public:
	ThumbTrackHorizon(NotePadForm* notePadForm);
	virtual ~ThumbTrackHorizon();
	virtual void KeyDown();
};


#endif//_THUMBTRACKHORIZON_H