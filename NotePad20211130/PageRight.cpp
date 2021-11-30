//PageRight.cpp
#include"PageRight.h"
#include"NotePadForm.h"
#include"Thumb.h"
#include"Scroll.h"
#include"Location.h"

PageRight::PageRight(NotePadForm* notePadForm)
	:MoveAction(notePadForm) {
}
PageRight::~PageRight() {
}
void PageRight::KeyDown() {
	this->notePadForm->thumb->PageRight();
	
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
