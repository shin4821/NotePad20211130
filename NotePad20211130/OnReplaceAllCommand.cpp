//OnReplaceAllCommand.cpp
#include"OnReplaceAllCommand.h"
#include"NotePadForm.h"
#include"ReplaceDialog.h"
#include"FindSameText.h"
#include"SelectWords.h"
#include"Note.h"
#include"OnReplaceCommand.h"
#include"LineChange.h"
#include"Scroll.h"
#include"KeyAction.h"
#include"FactoryProduct.h"
#include"FindSameText.h"
#include"FindAndReplace.h"
#include"CommandStory.h"
#include"UndoList.h"

OnReplaceAllCommand::OnReplaceAllCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {

	this->isChecked = false;
	this->notePadForm->isUndo = false;
	this->notePadForm->isThereSelected = false;
	this->isReplaced = false;
	this->firstX = 0;
	this->firstY = 0;
	this->beforeEndX = 0;
	this->beforeEndY = 0;
	this->afterEndX = 0;
	this->afterEndY = 0;
	this->isLineChangeButtonClicked = false;
	this->isFirst = true;
	this->isReplacedForMessage = false;
}

OnReplaceAllCommand::~OnReplaceAllCommand() {
}

void OnReplaceAllCommand::Execute() {
	BOOL isFirst = true;
	int isReplaced = 0;

	//1. 현재 커서의 위치를 (0,0)으로 설정해준다.
	this->notePadForm->note->MoveCurrent(0);
	Glyph* row = this->notePadForm->note->GetChild(0);
	row->MoveCurrent(0);


	//2. 현재 설정한 옵션들을 확인한다. *******************************************************************
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
	//******************************************************************************************************

#if 0
	//2. 같은 단어가 있는 동안 반복한다.
	if (isFirst == true) {
		this->isFirst = true;
		this->notePadForm->selectWords->firstX = 0;
		this->notePadForm->selectWords->firstY = 0;
		this->notePadForm->selectWords->endX = 0;
		this->notePadForm->selectWords->endY = 0;
	}
#endif

	//FindSameText클래스의 ReplaceAndSearchText를 호출한다.
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

#if 0
	if (this->isReplacedForMessage == false && this->beforeEndX>0) {
		this->isReplacedForMessage = true;
	}

	if (this->isReplacedForMessage == true) {
		CString str;
		str.Format("\"%s\"을(를) 찾을 수 없습니다.", (LPCTSTR)this->notePadForm->findSameText->findStr);
		this->notePadForm->MessageBox(str, "메모장", MB_OK);
	}
#endif



#if 0

	while (this->notePadForm->selectWords->firstX > 0 || this->notePadForm->selectWords->firstY > 0 ||
		this->notePadForm->selectWords->endX > 0 || this->notePadForm->selectWords->endY > 0 || isFirst == TRUE) {

		//FindSameText클래스의 ReplaceAndSearchText를 호출한다.
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

		//하나라도 바뀐게 있으면 CommandStory에 쌓는다.
		if (this->beforeEndX > 0) {
			this->notePadForm->commandStory->Pile(this); //dynamic_cast<Command*>(this)
		}

		//(21.10.29.추가) 현재 Undolist의 전체항목을 살펴본다.
		this->notePadForm->commandStory->undoList->Check();


		isFirst = false;
		this->isFirst = false;
		isReplaced++;
	}

	//한 단어라도 바꼈는지 확인한다.
	if (isReplaced > 1) {
		this->isReplaced = true;
	}
	else {
		CString str;
		str.Format("\"%s\"을(를) 찾을 수 없습니다.", (LPCTSTR)this->notePadForm->findSameText->findStr);
		this->notePadForm->MessageBox(str, "메모장", MB_OK);
	}

#endif

	//1.7. 스크롤 이동여부 확인한다. ******************************************************************
	this->notePadForm->isJumpOverForPrevious = FALSE;
	this->notePadForm->isJumpOverForNext = FALSE;
	this->notePadForm->isUp = FALSE;
	this->notePadForm->isDown = FALSE;
	this->notePadForm->isDoubleByte = FALSE;

	this->notePadForm->Notify(); // 만약 스크롤 범위 밖에 단어가 있을 경우
	this->notePadForm->Invalidate();
	//**************************************************************************************************

#if 0
	BOOL isFirst = true;
	int isReplaced = 0;

	//1. "모두 바꾸다" 버튼을 눌렀을 경우,

	//1.0. 바꿀 내용을 확인한다.
	this->notePadForm->replaceDialog->GetDlgItem(IDC_EDIT_REPLACETEXT)->GetWindowText(this->notePadForm->findSameText->replaceStr);
	this->replaceStr = (LPCTSTR)this->notePadForm->findSameText->replaceStr;

	//1.1. 찾을 내용을 확인한다.
	this->notePadForm->replaceDialog->GetDlgItem(IDC_EDIT_FINDTEXT)->GetWindowText(this->notePadForm->findSameText->findStr);
	this->findStr = (LPCTSTR)this->notePadForm->findSameText->findStr;

	//1.2. 사용자가 검색 문자열의 대/소문자를 정확히 일치시키는지 확인한다. (구분:TRUE, 안구분:FALSE)
	this->notePadForm->findSameText->matchCase = ((CButton*)this->notePadForm->replaceDialog->GetDlgItem(IDC_BUTTON_MATCHCASE))->GetCheck();

	//1.3. 사용자가 검색을 아래쪽 방향으로 진행하는지 확인한다 (아랫쪽:TRUE, 위쪽:FALSE)
	this->notePadForm->findSameText->searchDown = TRUE;

	//1.4. 사용자가 전체 단어만 일치시킬지 여부를 확인한다.
	this->notePadForm->findSameText->searchAround = TRUE;

	//1.5. FindSameText클래스의 ReplaceAndSearchText를 호출한다.
	//1.5.1. 현재 커서의 위치를 (0,0)으로 설정해준다.
	this->notePadForm->note->MoveCurrent(0);
	Glyph* row = this->notePadForm->note->GetChild(0);
	row->MoveCurrent(0);

	//1.5.2. 같은 단어가 있는 동안 반복한다.

	if (isFirst == true) {
		this->notePadForm->selectWords->firstX = 0;
		this->notePadForm->selectWords->firstY = 0;
		this->notePadForm->selectWords->endX = 0;
		this->notePadForm->selectWords->endY = 0;
	}

	while (this->notePadForm->selectWords->firstX > 0 || this->notePadForm->selectWords->firstY > 0 ||
		this->notePadForm->selectWords->endX > 0 || this->notePadForm->selectWords->endY > 0 || isFirst == TRUE) {

		this->notePadForm->findSameText->ReplaceAllText();
		isFirst = false;

		isReplaced++;
		//this->isReplaced = TRUE;
	}

	//한 단어라도 바꼈는지 확인한다.
	//this->isReplaced = this->notePadForm->selectWords->isReplaced;

	if (isReplaced > 1) {
		this->isReplaced = true;
	}


	//1.5.3. 현재 캐럿의 위치를 초기화한다.
	this->notePadForm->note->MoveCurrent(0);
	row = this->notePadForm->note->GetChild(0);
	row->MoveCurrent(0);

#if 0
	//1.6. 같은 단어가 없으면 메세지박스를 출력한다. (""을(를) 찾을 수 없습니다.)
	if ((this->notePadForm->selectWords->firstX <= 0 && this->notePadForm->selectWords->firstY <= 0 &&
		this->notePadForm->selectWords->endX <= 0 && this->notePadForm->selectWords->endY <= 0)
		|| this->notePadForm->findSameText->exception == TRUE) {

		CString str;
		str.Format("\"%s\"을(를) 찾을 수 없습니다.", (LPCTSTR)this->notePadForm->findSameText->findStr);
		MessageBox(str, "메모장", MB_OK);
	}
#endif

	//1.7. 스크롤 이동여부 확인한다. ******************************************************************
	this->notePadForm->isJumpOverForPrevious = FALSE;
	this->notePadForm->isJumpOverForNext = FALSE;
	this->notePadForm->isUp = FALSE;
	this->notePadForm->isDown = FALSE;
	this->notePadForm->isDoubleByte = FALSE;

	this->notePadForm->Notify(); // 만약 스크롤 범위 밖에 단어가 있을 경우
	this->notePadForm->Invalidate();
	//**************************************************************************************************

#endif
}

void OnReplaceAllCommand::Undo() {

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

#if 0
	BOOL isFirst = TRUE;
	CString replaceStr = (CString)this->replaceStr.c_str();
	CString findStr = (CString)this->findStr.c_str();

	//1. "모두 바꾸다" 버튼을 눌렀을 경우,

	//1.0. 바꿀 내용을 확인한다.
	this->notePadForm->findSameText->replaceStr = findStr;

	//1.1. 찾을 내용을 확인한다.
	this->notePadForm->findSameText->findStr = replaceStr;

	//1.2. 사용자가 검색 문자열의 대/소문자를 정확히 일치시키는지 확인한다. (구분:TRUE, 안구분:FALSE)
	//this->notePadForm->findSameText->matchCase = ((CButton*)this->notePadForm->replaceDialog->GetDlgItem(IDC_BUTTON_MATCHCASE))->GetCheck();

	//1.3. 사용자가 검색을 아래쪽 방향으로 진행하는지 확인한다 (아랫쪽:TRUE, 위쪽:FALSE)
	this->notePadForm->findSameText->searchDown = TRUE;

	//1.4. 사용자가 전체 단어만 일치시킬지 여부를 확인한다.
	this->notePadForm->findSameText->searchAround = TRUE;

	//1.5. FindSameText클래스의 ReplaceAndSearchText를 호출한다.
	//1.5.1. 현재 커서의 위치를 (0,0)으로 설정해준다.
	this->notePadForm->note->MoveCurrent(0);
	Glyph* row = this->notePadForm->note->GetChild(0);
	row->MoveCurrent(0);

	//1.5.2. 같은 단어가 있는 동안 반복한다.

	if (isFirst == TRUE) {
		this->notePadForm->selectWords->firstX = 0;
		this->notePadForm->selectWords->firstY = 0;
		this->notePadForm->selectWords->endX = 0;
		this->notePadForm->selectWords->endY = 0;
	}

	while (this->notePadForm->selectWords->firstX > 0 || this->notePadForm->selectWords->firstY > 0 ||
		this->notePadForm->selectWords->endX > 0 || this->notePadForm->selectWords->endY > 0 || isFirst == TRUE) {

		this->notePadForm->findSameText->ReplaceAllText();
		isFirst = FALSE;
	}

	//1.5.3. 현재 캐럿의 위치를 초기화한다.
	this->notePadForm->note->MoveCurrent(0);
	row = this->notePadForm->note->GetChild(0);
	row->MoveCurrent(0);
#endif

}

void OnReplaceAllCommand::Redo() {

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


#if 0
	BOOL isFirst = TRUE;
	CString replaceStr = (CString)this->replaceStr.c_str();
	CString findStr = (CString)this->findStr.c_str();
#if 0
	CString replaceStr = this->notePadForm->findSameText->findStr;
	CString findStr = this->notePadForm->findSameText->replaceStr;
#endif

	//1. "모두 바꾸다" 버튼을 눌렀을 경우,

	//1.0. 바꿀 내용을 확인한다.
	this->notePadForm->findSameText->replaceStr = replaceStr;

	//1.1. 찾을 내용을 확인한다.
	this->notePadForm->findSameText->findStr = findStr;

	//1.2. 사용자가 검색 문자열의 대/소문자를 정확히 일치시키는지 확인한다. (구분:TRUE, 안구분:FALSE)
	//this->notePadForm->findSameText->matchCase = ((CButton*)this->notePadForm->replaceDialog->GetDlgItem(IDC_BUTTON_MATCHCASE))->GetCheck();

	//1.3. 사용자가 검색을 아래쪽 방향으로 진행하는지 확인한다 (아랫쪽:TRUE, 위쪽:FALSE)
	this->notePadForm->findSameText->searchDown = TRUE;

	//1.4. 사용자가 전체 단어만 일치시킬지 여부를 확인한다.
	this->notePadForm->findSameText->searchAround = TRUE;

	//1.5. FindSameText클래스의 ReplaceAndSearchText를 호출한다.
	//1.5.1. 현재 커서의 위치를 (0,0)으로 설정해준다.
	this->notePadForm->note->MoveCurrent(0);
	Glyph* row = this->notePadForm->note->GetChild(0);
	row->MoveCurrent(0);

	//1.5.2. 같은 단어가 있는 동안 반복한다.

	if (isFirst == TRUE) {
		this->notePadForm->selectWords->firstX = 0;
		this->notePadForm->selectWords->firstY = 0;
		this->notePadForm->selectWords->endX = 0;
		this->notePadForm->selectWords->endY = 0;
	}

	while (this->notePadForm->selectWords->firstX > 0 || this->notePadForm->selectWords->firstY > 0 ||
		this->notePadForm->selectWords->endX > 0 || this->notePadForm->selectWords->endY > 0 || isFirst == TRUE) {

		this->notePadForm->findSameText->ReplaceAllText();
		isFirst = FALSE;
	}

	//1.5.3. 현재 캐럿의 위치를 초기화한다.
	this->notePadForm->note->MoveCurrent(0);
	row = this->notePadForm->note->GetChild(0);
	row->MoveCurrent(0);
#endif
}


bool OnReplaceAllCommand::GetIsFirst() {
	return this->isFirst;
}

Long OnReplaceAllCommand::GetCurrentX() {
	return this->currentX;
}

Long OnReplaceAllCommand::GetCurrentY() {
	return this->currentY;
}

string OnReplaceAllCommand::GetCharacterForAll() {
	return this->findStr;
}

void OnReplaceAllCommand::SetCheck() {
	this->isChecked = true;
}

bool OnReplaceAllCommand::GetChecked() {
	return this->isChecked;
}

bool OnReplaceAllCommand::GetIsReplaced() {
	return this->isReplaced;
}

void OnReplaceAllCommand::SetIsFirst() {
	this->isFirst = true;
}





