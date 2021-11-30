//ThumbTrackHorizon.cpp
#include"ThumbTrackHorizon.h"
#include"NotePadForm.h"
#include"Thumb.h"
#include"Scroll.h"
#include"Location.h"


ThumbTrackHorizon::ThumbTrackHorizon(NotePadForm* notePadForm)
	:MoveAction(notePadForm) {
}
ThumbTrackHorizon::~ThumbTrackHorizon() {
}
void ThumbTrackHorizon::KeyDown() {
	this->notePadForm->thumb->ThumbTrackHorizon();

	this->notePadForm->ScrollWindow(this->notePadForm->scroll->siHorizon.nPos, 0, NULL, NULL);
	this->notePadForm->UpdateWindow();
	this->notePadForm->location->MoveCaret();
	this->notePadForm->Invalidate();
}