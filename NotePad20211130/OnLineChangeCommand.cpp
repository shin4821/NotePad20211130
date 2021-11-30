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


	//0. 현재 캐럿의 위치를 확인한다.
	currentY = this->notePadForm->note->GetCurrent();
	row = this->notePadForm->note->GetChild(currentY);
	currentX = row->GetCurrent();

	//1. 현재 FALSE인데 자동 개행 버튼 눌렀을 경우 TRUE로 바꿔준다.
	if (this->notePadForm->isLineChangeButtonClicked == FALSE) {
		this->notePadForm->isLineChangeButtonClicked = TRUE;

		//1.1. 자동줄바꿈에 체크한다.
		CMenu* cMenu = this->notePadForm->GetMenu();
		cMenu->CheckMenuItem(40014, MF_CHECKED);

		//1.2. 레지스트리에 등록한다.
		//AfxGetApp()->WriteProfileInt("NotePadSection", "IsLineChanged", 1);
		this->notePadForm->pApp->WriteProfileInt("NotePadSection", "IsLineChanged", 1);

	}
	else if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
		this->notePadForm->isLineChangeButtonClicked = FALSE;

		//2.1. 자동줄바꿈에 언체크한다.
		CMenu* cMenu = this->notePadForm->GetMenu();
		cMenu->CheckMenuItem(40014, MF_UNCHECKED);

		//2.2. 레지스트리에 등록한다.
		this->notePadForm->pApp->WriteProfileInt("NotePadSection", "IsLineChanged", 0);


	}

//#if 0
	//2. 자동개행 버튼 눌렀을 때(ON)
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		//2.2. 자동개행 시킨다.
		this->notePadForm->lineChange->LineChangeButtonClicked();

		//2.1. 현재 캐럿의 위치를 자동개행 후의 위치로 변경한다.
		this->notePadForm->lineChange->FindCaretAfterLineChange(currentX, currentY, &currentX, &currentY);

		//2.2. 현재 선택된 단어가 있으면, 위치를 자동개행 후의 위치로 변경한다.
		if (firstX > 0 || firstY > 0 || endX > 0 || endY > 0) {

			this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
			this->notePadForm->lineChange->FindCaretAfterLineChange(endX, endY, &endX, &endY);
		}
	}

	//3. 자동개행 버튼 취소했을 때(OFF)
	else if (this->notePadForm->isLineChangeButtonClicked == FALSE) {

		//3.1. 자동개행 전의 캐럿의 위치를 반환받는다.
		this->notePadForm->lineChange->FindCaretBeforeLineChange(currentX, currentY, &currentX, &currentY);

		//3.2. 현재 선택된 단어가 있으면, 위치를 자동개행 후의 위치로 변경한다.
		if (firstX > 0 || firstY > 0 || endX > 0 || endY > 0) {
			this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
			this->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);
		}

		//3.3. 자동개행 취소한다.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

	}
//#endif

	//4. 반환받은 캐럿의 위치를 현재의 위치로 옮긴다.
	this->notePadForm->note->MoveCurrent(currentY);
	row = this->notePadForm->note->GetChild(currentY);
	row->MoveCurrent(currentX);

	this->notePadForm->selectWords->firstX = firstX;
	this->notePadForm->selectWords->firstY = firstY;
	this->notePadForm->selectWords->endX = endX;
	this->notePadForm->selectWords->endY = endY;

#if 0
	// Scroll 업데이트 할 때의 조건을 설정한다.
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
