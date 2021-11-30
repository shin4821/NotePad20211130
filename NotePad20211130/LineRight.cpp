//LineRight.cpp
#include"LineRight.h"
#include"NotePadForm.h"
#include"Thumb.h"
#include"Scroll.h"
#include"Location.h"

LineRight::LineRight(NotePadForm* notePadForm)
	:MoveAction(notePadForm) {
}
LineRight::~LineRight() {
}
void LineRight::KeyDown() {
	this->notePadForm->thumb->LineRight();

#if 0
	this->notePadForm->isMoveActionVertical = FALSE;
	this->notePadForm->isMoveActionHorizon = TRUE;
	this->notePadForm->isAppend = FALSE;
	this->notePadForm->isInsert = FALSE;
	this->notePadForm->isDeleteAction = FALSE;

	this->notePadForm->Notify();
#endif

//#if 0
	this->notePadForm->ScrollWindow(this->notePadForm->scroll->siHorizon.nPos, 0, NULL, NULL);
	this->notePadForm->UpdateWindow();
	this->notePadForm->location->MoveCaret();
	this->notePadForm->Invalidate();
//#endif
}













