//ThumbTrackVertical.cpp
#include"ThumbTrackVertical.h"
#include"NotePadForm.h"
#include"Thumb.h"
#include"Scroll.h"
#include"Location.h"

ThumbTrackVertical::ThumbTrackVertical(NotePadForm* notePadForm)
	:MoveAction(notePadForm) {
}
ThumbTrackVertical::~ThumbTrackVertical() {
}
void ThumbTrackVertical::KeyDown() {
	this->notePadForm->thumb->ThumbTrackVertical();

	this->notePadForm->ScrollWindow(0, this->notePadForm->scroll->siVertical.nPos, NULL, NULL);
	this->notePadForm->UpdateWindow();
	this->notePadForm->location->MoveCaret();
	this->notePadForm->Invalidate();
}