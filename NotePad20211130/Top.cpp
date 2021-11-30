//Top.cpp
#include"Top.h"
#include"NotePadForm.h"
#include"Thumb.h"

Top::Top(NotePadForm* notePadForm)
	:MoveAction(notePadForm) {
}
Top::~Top() {
}
void Top::KeyDown() {
	this->notePadForm->thumb->Top();
	
	// Scroll ������Ʈ �� ���� ������ �����Ѵ�.
	this->notePadForm->isJumpOverForPrevious = FALSE;
	this->notePadForm->isJumpOverForNext = FALSE;
	this->notePadForm->isUp = TRUE;
	this->notePadForm->isDown = FALSE;
	this->notePadForm->isDoubleByte = FALSE;
	
	this->notePadForm->Notify();
}