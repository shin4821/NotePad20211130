//PageDown.cpp
#include"PageDown.h"
#include"NotePadForm.h"
#include"Thumb.h"
#include"Scroll.h"
#include"Location.h"
#include"Note.h"
#include"GetTextSize.h"

PageDown::PageDown(NotePadForm* notePadForm)
	:MoveAction(notePadForm) {
}
PageDown::~PageDown() {
}
void PageDown::KeyDown() {
	Long currentY;
	Long currentX;
	Long textOutY;
	Long changedY;
	Long changedX;
	Glyph* row;
	int difference;
	int beforeNPos = this->notePadForm->scroll->siVertical.nPos;
	int length;
	int strSizeY = this->notePadForm->getTextSize->GetX(131);
	int i = 0;


	//2. Ű����� �������� PageDown�� ȣ���Ѵ�.
	if (this->notePadForm->isKeyBoard == TRUE) {
		this->notePadForm->thumb->PageDown(); //ĳ�� �̵� ����
	}
	//2.1. ���콺�� �������� PageDownForMouse ȣ���Ѵ�.
	else {
		this->notePadForm->thumb->PageDownForMouse();//ĳ�� �̵� ������
	}



#if 0
	this->notePadForm->isJumpOverForPrevious = FALSE;
	this->notePadForm->isJumpOverForNext = FALSE;
	this->notePadForm->isDoubleByte = FALSE;
	this->notePadForm->isUp = FALSE;
	this->notePadForm->isDown = FALSE;
	
	this->notePadForm->Notify();
#endif


	this->notePadForm->ScrollWindow(0, this->notePadForm->scroll->siVertical.nPos, NULL, NULL);
	this->notePadForm->location->MoveCaret();
	this->notePadForm->UpdateWindow();
	this->notePadForm->Invalidate();

}