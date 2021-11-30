//PageLeft.cpp
#include"PageLeft.h"
#include"NotePadForm.h"
#include"Thumb.h"
#include"Scroll.h"
#include"Location.h"

PageLeft::PageLeft(NotePadForm* notePadForm)
	:MoveAction(notePadForm) {
}
PageLeft::~PageLeft() {
}
void PageLeft::KeyDown() {
	this->notePadForm->thumb->PageLeft();


#if 0
	this->notePadForm->isMoveActionVertical = FALSE;
	this->notePadForm->isMoveActionHorizon = TRUE;
	this->notePadForm->isAppend = FALSE;
	this->notePadForm->isInsert = FALSE;
	this->notePadForm->isDeleteAction = FALSE;

	this->notePadForm->Notify();
#endif
	this->notePadForm->ScrollWindow(this->notePadForm->scroll->siHorizon.nPos, 0, NULL, NULL);
	this->notePadForm->UpdateWindow();
	this->notePadForm->location->MoveCaret();
	this->notePadForm->Invalidate();

}
