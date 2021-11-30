//ThumbTrackVertical.h
#ifndef _THUMBTRACKVERTICAL_H
#define _THUMBTRACKVERTICAL_H
#include"MoveAction.h"

class NotePadForm;
class ThumbTrackVertical :public MoveAction {
public:
	ThumbTrackVertical(NotePadForm* notePadForm);
	virtual ~ThumbTrackVertical();
	virtual void KeyDown();
};


#endif//_THUMBTRACKVERTICAL_H