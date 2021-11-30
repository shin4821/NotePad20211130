//OnLineChangeCommand.cpp
#include"OnLineChangeCommand.h"
#include"LineChange.h"
#include"NotePadForm.h"
#include"Note.h"
#include"Scroll.h"
#include"Location.h"
#include"SelectWords.h"
#include"resource.h"
#include"GetTextSize.h"


OnLineChangeCommand::OnLineChangeCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
}

OnLineChangeCommand::~OnLineChangeCommand() {

}

void OnLineChangeCommand::Execute() {

	int i = 0;
	int j;
	Glyph* row;
	Glyph* dummyRow;
	Glyph* nextRow;
	int location;
	BOOL ret;
	int currentX;
	int currentY;
	int firstX = this->notePadForm->selectWords->firstX;
	int firstY = this->notePadForm->selectWords->firstY;
	int endX = this->notePadForm->selectWords->endX;
	int endY = this->notePadForm->selectWords->endY;


	//0. ���� ĳ���� ��ġ�� Ȯ���Ѵ�.
	currentY = this->notePadForm->note->GetCurrent();
	row = this->notePadForm->note->GetChild(currentY);
	currentX = row->GetCurrent();

	//1. ���� FALSE�ε� �ڵ� ���� ��ư ������ ��� TRUE�� �ٲ��ش�.
	if (this->notePadForm->isLineChangeButtonClicked == FALSE) {
		this->notePadForm->isLineChangeButtonClicked = TRUE;

		//1.1. �ڵ��ٹٲ޿� üũ�Ѵ�.
		CMenu* cMenu = this->notePadForm->GetMenu();
		cMenu->CheckMenuItem(40014, MF_CHECKED);

		//1.2. ������Ʈ���� ����Ѵ�.
		//AfxGetApp()->WriteProfileInt("NotePadSection", "IsLineChanged", 1);
		this->notePadForm->pApp->WriteProfileInt("NotePadSection", "IsLineChanged", 1);

	}
	else if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
		this->notePadForm->isLineChangeButtonClicked = FALSE;

		//2.1. �ڵ��ٹٲ޿� ��üũ�Ѵ�.
		CMenu* cMenu = this->notePadForm->GetMenu();
		cMenu->CheckMenuItem(40014, MF_UNCHECKED);

		//2.2. ������Ʈ���� ����Ѵ�.
		this->notePadForm->pApp->WriteProfileInt("NotePadSection", "IsLineChanged", 0);


	}

//#if 0
	//2. �ڵ����� ��ư ������ ��(ON)
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		//2.2. �ڵ����� ��Ų��.
		this->notePadForm->lineChange->LineChangeButtonClicked();

		//2.1. ���� ĳ���� ��ġ�� �ڵ����� ���� ��ġ�� �����Ѵ�.
		this->notePadForm->lineChange->FindCaretAfterLineChange(currentX, currentY, &currentX, &currentY);

		//2.2. ���� ���õ� �ܾ ������, ��ġ�� �ڵ����� ���� ��ġ�� �����Ѵ�.
		if (firstX > 0 || firstY > 0 || endX > 0 || endY > 0) {

			this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
			this->notePadForm->lineChange->FindCaretAfterLineChange(endX, endY, &endX, &endY);
		}
	}

	//3. �ڵ����� ��ư ������� ��(OFF)
	else if (this->notePadForm->isLineChangeButtonClicked == FALSE) {

		//3.1. �ڵ����� ���� ĳ���� ��ġ�� ��ȯ�޴´�.
		this->notePadForm->lineChange->FindCaretBeforeLineChange(currentX, currentY, &currentX, &currentY);

		//3.2. ���� ���õ� �ܾ ������, ��ġ�� �ڵ����� ���� ��ġ�� �����Ѵ�.
		if (firstX > 0 || firstY > 0 || endX > 0 || endY > 0) {
			this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
			this->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);
		}

		//3.3. �ڵ����� ����Ѵ�.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

	}
//#endif

	//4. ��ȯ���� ĳ���� ��ġ�� ������ ��ġ�� �ű��.
	this->notePadForm->note->MoveCurrent(currentY);
	row = this->notePadForm->note->GetChild(currentY);
	row->MoveCurrent(currentX);

	this->notePadForm->selectWords->firstX = firstX;
	this->notePadForm->selectWords->firstY = firstY;
	this->notePadForm->selectWords->endX = endX;
	this->notePadForm->selectWords->endY = endY;

#if 0
	// Scroll ������Ʈ �� ���� ������ �����Ѵ�.
	this->notePadForm->isMoveActionHorizon = FALSE;
	this->notePadForm->isMoveActionVertical = FALSE;
	this->notePadForm->isAppend = FALSE;
	this->notePadForm->isInsert = FALSE;
	this->notePadForm->isDeleteAction = FALSE;

	this->notePadForm->Notify();

	this->notePadForm->Invalidate();
#endif


	this->notePadForm->isJumpOverForPrevious = FALSE;
	this->notePadForm->isJumpOverForNext = FALSE;
	this->notePadForm->isUp = FALSE;
	this->notePadForm->isDown = FALSE;
	this->notePadForm->isDoubleByte = FALSE;

	this->notePadForm->Notify();

	//this->notePadForm->Invalidate();


}
