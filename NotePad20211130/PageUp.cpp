//PageUp.cpp
#include"PageUp.h"
#include"NotePadForm.h"
#include"Thumb.h"
#include"Scroll.h"
#include"Location.h"

PageUp::PageUp(NotePadForm* notePadForm)
	:MoveAction(notePadForm) {
}
PageUp::~PageUp() {
}
void PageUp::KeyDown() {

	//2. 키보드로 눌렀으면 PageUp을 호출한다.
	if (this->notePadForm->isKeyBoard == TRUE) {
		this->notePadForm->thumb->PageUp();
	}
	//2.1. 마우스로 눌렀으면 PageUpForMouse 호출한다.
	else {
		this->notePadForm->thumb->PageUpForMouse();//캐럿 이동 미포함
	}


#if 0
	this->notePadForm->isMoveActionVertical = TRUE; //TRUE
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