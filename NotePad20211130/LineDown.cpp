//LineUp.cpp
#include"LineDown.h"
#include"NotePadForm.h"
#include"Thumb.h"
#include"Scroll.h"
#include"Location.h"


LineDown::LineDown(NotePadForm* notePadForm)
	:MoveAction(notePadForm) {
}
LineDown::~LineDown() {
}
void LineDown::KeyDown() {
	this->notePadForm->thumb->LineDown();

#if 0
	this->notePadForm->isMoveActionVertical = TRUE;
	this->notePadForm->isMoveActionHorizon = FALSE;
	this->notePadForm->isAppend = FALSE;
	this->notePadForm->isInsert = FALSE;
	this->notePadForm->isDeleteAction = FALSE;

	this->notePadForm->Notify();
#endif

	this->notePadForm->ScrollWindow(0, this->notePadForm->scroll->siVertical.nPos, NULL, NULL);
	this->notePadForm->UpdateWindow();
	this->notePadForm->location->MoveCaret();
	this->notePadForm->Invalidate();
}