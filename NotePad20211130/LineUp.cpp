//LineUp.cpp
#include"LineUp.h"
#include"NotePadForm.h"
#include"Thumb.h"
#include"Scroll.h"
#include"Location.h"

LineUp::LineUp(NotePadForm* notePadForm)
	:MoveAction(notePadForm) {
}
LineUp::~LineUp() {
}
void LineUp::KeyDown() {
	this->notePadForm->thumb->LineUp();

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