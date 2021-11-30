//OnSelectAllCommand.cpp
#include"OnSelectAllCommand.h"
#include"NotePadForm.h"
#include"Note.h"
#include"SelectWords.h"

OnSelectAllCommand::OnSelectAllCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
}

OnSelectAllCommand::~OnSelectAllCommand() {
}

void OnSelectAllCommand::Execute() {
	int firstX;
	int firstY;
	int endX;
	int endY;
	Glyph* row;

	//1. 전체 선택 눌렀을 때 
	//1.1. 처음 좌표와 끝 좌표를 찾는다.
    endY = this->notePadForm->note->GetLength() - 1;
	row = this->notePadForm->note->GetChild(endY);
	endX = row->GetLength();

	//1.2. 구한 좌표를 현재 좌표로 설정한다.
	this->notePadForm->selectWords->firstX = 0;
	this->notePadForm->selectWords->firstY = 0;
	this->notePadForm->selectWords->endX = endX;
	this->notePadForm->selectWords->endY = endY;

	//1.3. endX, endY로 현재 좌표를 설정한다.
	this->notePadForm->note->MoveCurrent(endY);
	row->MoveCurrent(endX);


	// Scroll 업데이트 할 때의 조건을 설정한다.
	this->notePadForm->isJumpOverForPrevious = false;
	this->notePadForm->isJumpOverForNext = false;
	this->notePadForm->isDoubleByte = false;
	this->notePadForm->isUp = false;
	this->notePadForm->isDown = false;


	this->notePadForm->Notify(); //Observer Pattern 이후
	this->notePadForm->Invalidate();
}
