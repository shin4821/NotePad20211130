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

	//1. ��ü ���� ������ �� 
	//1.1. ó�� ��ǥ�� �� ��ǥ�� ã�´�.
    endY = this->notePadForm->note->GetLength() - 1;
	row = this->notePadForm->note->GetChild(endY);
	endX = row->GetLength();

	//1.2. ���� ��ǥ�� ���� ��ǥ�� �����Ѵ�.
	this->notePadForm->selectWords->firstX = 0;
	this->notePadForm->selectWords->firstY = 0;
	this->notePadForm->selectWords->endX = endX;
	this->notePadForm->selectWords->endY = endY;

	//1.3. endX, endY�� ���� ��ǥ�� �����Ѵ�.
	this->notePadForm->note->MoveCurrent(endY);
	row->MoveCurrent(endX);


	// Scroll ������Ʈ �� ���� ������ �����Ѵ�.
	this->notePadForm->isJumpOverForPrevious = false;
	this->notePadForm->isJumpOverForNext = false;
	this->notePadForm->isDoubleByte = false;
	this->notePadForm->isUp = false;
	this->notePadForm->isDown = false;


	this->notePadForm->Notify(); //Observer Pattern ����
	this->notePadForm->Invalidate();
}
