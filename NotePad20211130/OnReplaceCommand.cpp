//OnReplaceCommand.cpp
#include"OnReplaceCommand.h"
#include"NotePadForm.h"
#include"SelectWords.h"
#include"FindSameText.h"
#include"ReplaceDialog.h"
#include"Note.h"
#include"LineChange.h"
#include"FindAndReplace.h"
#include"CreateGlyph.h"
#include"CreateProduct.h"
#include"Scroll.h"
#include"KeyAction.h"
#include"FactoryProduct.h"
#include"resource.h"

OnReplaceCommand::OnReplaceCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
	this->isChecked = false;
	this->notePadForm->isUndo = false;
	this->notePadForm->isThereSelected = false;
	this->firstX = 0;
	this->firstY = 0;
	this->beforeEndX = 0;
	this->beforeEndY = 0;
	this->afterEndX = 0;
	this->afterEndY = 0;
	this->isReplaced = false;
	this->isLineChangeButtonClicked = false;
}

OnReplaceCommand::~OnReplaceCommand() {
}

void OnReplaceCommand::Execute() {

	//1. "바꾸다" 버튼을 눌렀을 경우,
	this->notePadForm->findSameText->exception = false;

	//1.0. 바꿀 내용을 확인한다.
	this->notePadForm->replaceDialog->GetDlgItem(IDC_EDIT_REPLACETEXT)->GetWindowText(this->notePadForm->findSameText->replaceStr);

	//1.1. 찾을 내용을 확인한다.
	this->notePadForm->replaceDialog->GetDlgItem(IDC_EDIT_FINDTEXT)->GetWindowText(this->notePadForm->findSameText->findStr);

	//1.2. 사용자가 검색 문자열의 대/소문자를 정확히 일치시키는지 확인한다. (구분:true, 안구분:false)
	this->notePadForm->findSameText->matchCase = 
		((CButton*)this->notePadForm->replaceDialog->GetDlgItem(IDC_BUTTON_MATCHCASE))->GetCheck();

	//1.3. 사용자가 검색을 아래쪽 방향으로 진행하는지 확인한다 (아랫쪽:true, 위쪽:false)
	this->notePadForm->findSameText->searchDown = true;

	//1.4. 사용자가 전체 단어만 일치시킬지 여부를 확인한다.
	this->notePadForm->findSameText->searchAround = 
		((CButton*)this->notePadForm->replaceDialog->GetDlgItem(IDC_BUTTON_SEARCHAROUND))->GetCheck();


	//1.5. FindSameText클래스의 ReplaceAndSearchText를 호출한다.
	this->notePadForm->findSameText->ReplaceAndSearchText();


	//(21.09.19 추가) 멤버 설정한다.
	this->firstX = this->notePadForm->selectWords->beforeFirstX;
	this->firstY = this->notePadForm->selectWords->beforeFirstY;
	this->beforeEndX = this->notePadForm->selectWords->beforeEndX;
	this->beforeEndY = this->notePadForm->selectWords->beforeEndY;
	this->afterEndX = this->notePadForm->selectWords->afterEndX;
	this->afterEndY = this->notePadForm->selectWords->afterEndY;
	this->replaceStr = this->notePadForm->findSameText->replaceStr;
	this->findStr = this->notePadForm->findSameText->findStr;
	this->isReplaced = this->notePadForm->selectWords->isReplaced;
	this->isLineChangeButtonClicked = this->notePadForm->selectWords->isLineChangeButtonClicked;
	this->windowWidth = this->notePadForm->selectWords->windowWidth;


	//1.6. 같은 단어가 없으면 메세지박스를 출력한다. (""을(를) 찾을 수 없습니다.)
	if ((this->notePadForm->selectWords->firstX <= 0 && this->notePadForm->selectWords->firstY <= 0 &&
		this->notePadForm->selectWords->endX <= 0 && this->notePadForm->selectWords->endY <= 0)
		|| this->notePadForm->findSameText->exception == true) {

		CString str;
		str.Format("\"%s\"을(를) 찾을 수 없습니다.", (LPCTSTR)this->notePadForm->findSameText->findStr);
		this->notePadForm->MessageBox(str, "메모장", MB_OK);
	}

	//1.7. 스크롤 이동여부 확인한다. ******************************************************************
	this->notePadForm->isJumpOverForPrevious = false;
	this->notePadForm->isJumpOverForNext = false;
	this->notePadForm->isDoubleByte = false;

	this->notePadForm->Notify(); // 만약 스크롤 범위 밖에 단어가 있을 경우
	this->notePadForm->Invalidate();
	//**************************************************************************************************
}

void OnReplaceCommand::Undo() {
	int difference;
	Glyph* row;
	int windowWidth = this->notePadForm->scroll->windowWidth; //현재 윈도우너비
	int changedX;
	int changedY;

	//0. firstX, firstY 까지 지운다.
	int firstX = this->firstX;
	int firstY = this->firstY;
	int endX = this->afterEndX;
	int endY = this->afterEndY;

	//1. (21.09.30 추가) 현재 자동개행버튼 상황과 적을때의 상황이 다르면,
	if (this->notePadForm->isLineChangeButtonClicked != this->isLineChangeButtonClicked) {

		//1.1. 현재 자동개행버튼이 눌려져 있으면, 자동개행 후의 좌표를 구한다. 
		if (this->notePadForm->isLineChangeButtonClicked == true) {
			this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
			this->notePadForm->lineChange->FindCaretAfterLineChange(endX, endY, &endX, &endY);
		}
		//1.2. 현재 자동개행버튼이 안눌려져 있으면, 자동개행 전의 좌표를 구한다.
		else {
			//1.2.1. 자동개행 시켜준다.
			this->notePadForm->lineChange->LineChangeButtonClicked(this->windowWidth);
			//1.2.2. 자동개행 전 좌표 구한다.
			this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
			this->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);

			//1.2.3. 자동개행 취소한다.
			this->notePadForm->lineChange->LineChangeButtonNotClicked();
		}
	}
	//2. 현재 자동개행버튼 상황과 적을때의 상황이 같지만, 자동개행일 때 윈도우 너비가 다른 경우,
	else if (this->isLineChangeButtonClicked == true && windowWidth != this->windowWidth) {
		//2.1. 자동개행된 문서를 펼친다.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

		//2.2. 자동개행 시켜준다.
		this->notePadForm->lineChange->LineChangeButtonClicked(this->windowWidth);

		//2.3. 자동개행 전 좌표를 구한다.
		this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
		this->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);

		//2.4. 자동개행된 문서를 펼친다.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

		//2.5. 펼친 문서를 현재 windowWidth만큼 자동개행한다.
		this->notePadForm->lineChange->LineChangeButtonClicked(windowWidth);

		//2.6. 자동개행 후 좌표를 구한다.
		this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
		this->notePadForm->lineChange->FindCaretAfterLineChange(endX, endY, &endX, &endY);
	}

	//2. 선택된 단어를 정한다.
	this->notePadForm->selectWords->firstX = firstX;
	this->notePadForm->selectWords->firstY = firstY;
	this->notePadForm->selectWords->endX = endX;
	this->notePadForm->selectWords->endY = endY;

	//3. endX, endY를 현재 위치로 설정해준다.
	this->notePadForm->note->MoveCurrent(endY);
	row = this->notePadForm->note->GetChild(endY);
	row->MoveCurrent(endX);

	KeyAction* keyAction = NULL;
	FactoryProduct factoryProduct(this->notePadForm);

	keyAction = factoryProduct.Make(VK_BACK, NULL, NULL);
	keyAction->KeyDown();

	//3. 현재 위치를 firstX,Y로 설정해준다.
	this->notePadForm->note->MoveCurrent(firstY);
	row = this->notePadForm->note->GetChild(firstY);
	row->MoveCurrent(firstX);

	//4. 자동개행버튼이 눌려진 경우,
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		//4.1. 자동개행 전 firstXY의 좌표를 구한다.
		this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);

		//4.2. 자동개행된 문서를 펼친다.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();
	}

	//5. 지운 위치에 findStr 적어준다.
	this->notePadForm->findSameText->findAndReplace->AddWordForUndo(firstX, firstY, this->findStr, &changedX, &changedY);

	//6. 자동개행이 눌려진 경우,
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		//6.1. 펼친 문서를 다시 자동개행한다. (ON)
		this->notePadForm->lineChange->LineChangeButtonClicked();

		//6.2. 자동개행 후 이동할 캐럿의 위치를 구한다.
		this->notePadForm->lineChange->FindCaretAfterLineChange(changedX, changedY, &changedX, &changedY);
	}

	//7. 해당 위치로 캐럿을 이동시킨다.
	this->notePadForm->note->MoveCurrent(changedY);
	row = this->notePadForm->note->GetChild(changedY);
	row->MoveCurrent(changedX);


	//5. 선택된 좌표를 초기화한다.
	this->notePadForm->selectWords->firstY = 0;
	this->notePadForm->selectWords->firstX = 0;
	this->notePadForm->selectWords->endY = 0;
	this->notePadForm->selectWords->endX = 0;

	//6. Scroll 업데이트 할 때의 조건을 설정한다.
	this->notePadForm->isJumpOverForPrevious = FALSE;
	this->notePadForm->isJumpOverForNext = FALSE;
	this->notePadForm->isDoubleByte = FALSE;
	this->notePadForm->isUp = TRUE;
	this->notePadForm->isDown = FALSE;

	this->notePadForm->Notify();

}

void OnReplaceCommand::Redo() {

	int difference;
	Glyph* row;
	int windowWidth = this->notePadForm->scroll->windowWidth; //현재 윈도우너비
	int changedX;
	int changedY;

	//0. firstX, firstY 까지 지운다.
	int firstX = this->firstX;
	int firstY = this->firstY;
	int endX = this->beforeEndX;
	int endY = this->beforeEndY;

	//1. (21.09.30 추가) 현재 자동개행버튼 상황과 적을때의 상황이 다르면,
	if (this->notePadForm->isLineChangeButtonClicked != this->isLineChangeButtonClicked) {

		//1.1. 현재 자동개행버튼이 눌려져 있으면, 자동개행 후의 좌표를 구한다. 
		if (this->notePadForm->isLineChangeButtonClicked == true) {
			this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
			this->notePadForm->lineChange->FindCaretAfterLineChange(endX, endY, &endX, &endY);
		}
		//1.2. 현재 자동개행버튼이 안눌려져 있으면, 자동개행 전의 좌표를 구한다.
		else {
			//1.2.1. 자동개행 시켜준다.
			this->notePadForm->lineChange->LineChangeButtonClicked(this->windowWidth);
			//1.2.2. 자동개행 전 좌표 구한다.
			this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
			this->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);

			//1.2.3. 자동개행 취소한다.
			this->notePadForm->lineChange->LineChangeButtonNotClicked();
		}
	}
	//2. 현재 자동개행버튼 상황과 적을때의 상황이 같지만, 자동개행일 때 윈도우 너비가 다른 경우,
	else if (this->isLineChangeButtonClicked == true && windowWidth != this->windowWidth) {
		//2.1. 자동개행된 문서를 펼친다.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

		//2.2. 자동개행 시켜준다.
		this->notePadForm->lineChange->LineChangeButtonClicked(this->windowWidth);

		//2.3. 자동개행 전 좌표를 구한다.
		this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
		this->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);

		//2.4. 자동개행된 문서를 펼친다.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

		//2.5. 펼친 문서를 현재 windowWidth만큼 자동개행한다.
		this->notePadForm->lineChange->LineChangeButtonClicked(windowWidth);

		//2.6. 자동개행 후 좌표를 구한다.
		this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
		this->notePadForm->lineChange->FindCaretAfterLineChange(endX, endY, &endX, &endY);
	}

	//2. 선택된 단어를 정한다.
	this->notePadForm->selectWords->firstX = firstX;
	this->notePadForm->selectWords->firstY = firstY;
	this->notePadForm->selectWords->endX = endX;
	this->notePadForm->selectWords->endY = endY;

	//3. endX, endY를 현재 위치로 설정해준다.
	this->notePadForm->note->MoveCurrent(endY);
	row = this->notePadForm->note->GetChild(endY);
	row->MoveCurrent(endX);

	KeyAction* keyAction = NULL;
	FactoryProduct factoryProduct(this->notePadForm);

	keyAction = factoryProduct.Make(VK_BACK, NULL, NULL);
	keyAction->KeyDown();

	//3. 현재 위치를 firstX,Y로 설정해준다.
	this->notePadForm->note->MoveCurrent(firstY);
	row = this->notePadForm->note->GetChild(firstY);
	row->MoveCurrent(firstX);

	//4. 자동개행버튼이 눌려진 경우,
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		//4.1. 자동개행 전 firstXY의 좌표를 구한다.
		this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);

		//4.2. 자동개행된 문서를 펼친다.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();
	}

	//5. 지운 위치에 findStr 적어준다.
	this->notePadForm->findSameText->findAndReplace->AddWordForUndo(firstX, firstY, this->replaceStr, &changedX, &changedY);

	//6. 자동개행이 눌려진 경우,
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		//6.1. 펼친 문서를 다시 자동개행한다. (ON)
		this->notePadForm->lineChange->LineChangeButtonClicked();

		//6.2. 자동개행 후 이동할 캐럿의 위치를 구한다.
		this->notePadForm->lineChange->FindCaretAfterLineChange(changedX, changedY, &changedX, &changedY);
	}

	//7. 해당 위치로 캐럿을 이동시킨다.
	this->notePadForm->note->MoveCurrent(changedY);
	row = this->notePadForm->note->GetChild(changedY);
	row->MoveCurrent(changedX);


	//5. 선택된 좌표를 초기화한다.
	this->notePadForm->selectWords->firstY = 0;
	this->notePadForm->selectWords->firstX = 0;
	this->notePadForm->selectWords->endY = 0;
	this->notePadForm->selectWords->endX = 0;

	//6. Scroll 업데이트 할 때의 조건을 설정한다.
	this->notePadForm->isJumpOverForPrevious = FALSE;
	this->notePadForm->isJumpOverForNext = FALSE;
	this->notePadForm->isDoubleByte = FALSE;
	this->notePadForm->isUp = TRUE;
	this->notePadForm->isDown = FALSE;

	this->notePadForm->Notify();
}

Long OnReplaceCommand::GetCurrentX() {
	return this->currentX;
}

Long OnReplaceCommand::GetCurrentY() {
	return this->currentY;
}

string OnReplaceCommand::GetCharacterForAll() {
	return this->findStr;
}

void OnReplaceCommand::SetCheck() {
	this->isChecked = true;
}

bool OnReplaceCommand::GetChecked() {
	return this->isChecked;
}

bool OnReplaceCommand::GetIsReplaced() {
	return this->isReplaced;
}




