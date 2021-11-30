//Thumb.h
#ifndef _THUMB_H
#define _THUMB_H
typedef signed long int Long;

class NotePadForm;
class Thumb {
public:
	Thumb(NotePadForm* notePadForm);
	~Thumb();
	void LineRight();
	void LineLeft();
	void PageLeft();
	void PageRight();
	void LineUp();
	void LineDown();
	void PageUp();
	void PageDown();
	void PageUpForMouse();
	void PageDownForMouse();
	void ThumbTrackHorizon();
	void ThumbTrackVertical();
	void Top();
	//void MoveThumbAppend(Long horizonRangeValue, Long verticalRangeValue, Long x, Long y);
	//void MoveThumbInsert(char(*character), Long size);

private:
	NotePadForm* notePadForm;
};

#endif//_THUMB_H

