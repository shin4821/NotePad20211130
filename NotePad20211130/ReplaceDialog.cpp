//ReplaceDialog.cpp
#include"ReplaceDialog.h"
#include"NotePadForm.h"
#include"findSameText.h"
#include"SelectWords.h"
#include"FactoryProductCommand.h"
#include"CommandStory.h"
#include"Command.h"
#include"Scroll.h"
#include"Note.h"
#include"OnReplaceAllCommand.h"
#include"UndoList.h"
//#include"OnReplaceCommand.h"


BEGIN_MESSAGE_MAP(ReplaceDialog, CFindReplaceDialog)
	ON_COMMAND_RANGE(40040, 40045, OnMyCommand)
	ON_EN_CHANGE(IDC_EDIT_FINDTEXT, OnFindTextChange)
	ON_BN_CLICKED(IDC_BUTTON_FINDNEXT, OnFineNextButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_REPLACE, OnReplaceButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_REPLACEALL, OnReplaceAllButtonClicked)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


void ReplaceDialog::OnMyCommand(UINT id) {
	FactoryProductCommand factoryProductCommand(this->notePadForm);
	Command* command = NULL;
	BOOL isFirst = true;
	BOOL isFirstForList = true;
	int isReplaced = 0;
	Glyph* row;

	//(21.11.04. 추가) 사용자가 전체 단어만 일치시킬지 여부를 확인한다.
	this->notePadForm->findSameText->searchAround = ((CButton*)GetDlgItem(IDC_BUTTON_SEARCHAROUND))->GetCheck();


	// 해당 command를 생성한다.
	command = factoryProductCommand.Make(id);

	//해당 command를 실행한다.
	if (dynamic_cast<OnReplaceAllCommand*>(command)) {

		//2. 같은 단어가 있는 동안 반복한다.
		if (isFirst == true) {
			this->notePadForm->selectWords->firstX = 0;
			this->notePadForm->selectWords->firstY = 0;
			this->notePadForm->selectWords->endX = 0;
			this->notePadForm->selectWords->endY = 0;
		}
		
		row = this->notePadForm->note->GetChild(2);

		while (this->notePadForm->selectWords->firstX > 0 || this->notePadForm->selectWords->firstY > 0 ||
			this->notePadForm->selectWords->endX > 0 || this->notePadForm->selectWords->endY > 0 || 
			isFirst == TRUE) {

			command->Execute();

			//replace된 단어가 있으면 실행한 command를 history list에 추가한다.
			if (dynamic_cast<OnReplaceAllCommand*>(command)->GetIsReplaced() == true) {

				if (isFirstForList == false) {
					dynamic_cast<OnReplaceAllCommand*>(command)->isFirst = false;
				}

				this->notePadForm->commandStory->Pile(command);
				isFirstForList = false;
			}

			isFirst = false;

			isReplaced++;

			command = factoryProductCommand.Make(40045);
		}
		//(21.10.29.추가) 현재 Undolist의 전체항목을 살펴본다.
		this->notePadForm->commandStory->undoList->Check();
	}
	else {
		command->Execute();
	}

	

	//다음 찾기가 아닌 경우,
	if (id != 40040 && command->GetIsReplaced() == true) {
		//실행한 command를 history list에 추가한다.
		this->notePadForm->commandStory->Pile(command);
	}



	Invalidate();
}


ReplaceDialog::ReplaceDialog(NotePadForm* notePadForm) {
	this->notePadForm = notePadForm;

}

void ReplaceDialog::OnFindTextChange() {

	//1. 이전 단어를 구해놓는다.
	this->notePadForm->findSameText->beforeFindStr = this->notePadForm->findSameText->findStr;

	//2. 기재한 찾을 내용을 확인한다.
	GetDlgItem(IDC_EDIT_FINDTEXT)->GetWindowText(this->notePadForm->findSameText->findStr);


	//(21.09.15 추가) 3. 찾는 내용이 NULL이면, 
	if (this->notePadForm->findSameText->findStr == "\0") {
		//3.1. 찾기 버튼을 비활성화 시킨다.
		GetDlgItem(IDC_BUTTON_FINDNEXT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_REPLACE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_REPLACEALL)->EnableWindow(FALSE);
	}
	else { //4. 찾는 내용이 NULL이 아니면,
		//4.1. 찾기 버튼을 활성화 시킨다.
		GetDlgItem(IDC_BUTTON_FINDNEXT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_REPLACE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_REPLACEALL)->EnableWindow(TRUE);
	}

	this->notePadForm->findSameText->beforeFindStr = this->notePadForm->findSameText->findStr;
}

BOOL ReplaceDialog::Create(LPCTSTR lpszFindWhat, DWORD dwFlags, CWnd* pParentWnd) {

	string strCharacter;
	this->notePadForm = (NotePadForm*)pParentWnd;

	this->m_fr.Flags |= FR_ENABLETEMPLATE;
	this->m_fr.hInstance = AfxGetResourceHandle();
	this->m_fr.lpTemplateName = MAKEINTRESOURCE(ReplaceDialog::IDD);

	//(21.09.16 추가) 1. 현재 선택된 단어가 있는 경우, 
	if (this->notePadForm->selectWords->GetFirstX() > 0 || this->notePadForm->selectWords->GetFirstY() > 0 ||
		this->notePadForm->selectWords->GetEndX() > 0 || this->notePadForm->selectWords->GetEndY() > 0) {

		//1.1. 선택된 단어를 구한다.
		strCharacter = this->notePadForm->selectWords->GetSelectedWord();
		this->notePadForm->findSameText->findStr = CString(strCharacter.c_str());

		//1.2. 해당 단어를 "찾는 내용"에 적는다.
	}

	//2. 선택된 내용이 없는 경우, 그 전 단어를 넣어준다.
	else {
		strCharacter = this->notePadForm->findSameText->beforeFindStr;
		this->notePadForm->findSameText->findStr = CString(strCharacter.c_str());
	}


	BOOL ret = CFindReplaceDialog::Create(FALSE, (LPCTSTR)this->notePadForm->findSameText->findStr,
		"", dwFlags, pParentWnd);


	//2. FindSameText의 멤버 옵션에 따라, 멤버 옵션을 설정해준다.
	//2.1. 사용자가 검색 문자열의 대/소문자를 정확히 일치시키는지 확인한다. (구분:TRUE, 안구분:FALSE)
	if (this->notePadForm->findSameText->matchCase == TRUE) {
		((CButton*)GetDlgItem(IDC_BUTTON_MATCHCASE))->SetCheck(BST_CHECKED);
	}
	else {
		((CButton*)GetDlgItem(IDC_BUTTON_MATCHCASE))->SetCheck(BST_UNCHECKED);
	}

	//2.2. 사용자가 전체 단어만 일치시킬지 여부를 확인한다.
	if (this->notePadForm->findSameText->searchAround == TRUE) {
		((CButton*)GetDlgItem(IDC_BUTTON_SEARCHAROUND))->SetCheck(BST_CHECKED);
	}
	else {
		((CButton*)GetDlgItem(IDC_BUTTON_SEARCHAROUND))->SetCheck(BST_UNCHECKED);
	}



	return ret;

#if 0
	return CFindReplaceDialog::Create(TRUE, (LPCTSTR)this->notePadForm->findSameText->findStr,
		(LPCTSTR)this->notePadForm->findSameText->findStr, dwFlags, pParentWnd);
#endif
}

void ReplaceDialog::OnFineNextButtonClicked() { //OnFindNextCommand에서 처리됩니다. 

	//1. "다음 찾기"버튼을 눌렀을 경우,

		//1.0. 이전 단어를 구해놓는다.
	this->notePadForm->findSameText->beforeFindStr = this->notePadForm->findSameText->findStr;

	//1.1. 기재한 찾을 내용을 확인한다.
	GetDlgItem(IDC_EDIT_FINDTEXT)->GetWindowText(this->notePadForm->findSameText->findStr);

	//1.2. 사용자가 검색 문자열의 대/소문자를 정확히 일치시키는지 확인한다. (구분:TRUE, 안구분:FALSE)
	this->notePadForm->findSameText->matchCase = ((CButton*)GetDlgItem(IDC_BUTTON_MATCHCASE))->GetCheck();


	//1.3. 사용자가 검색을 아래쪽 방향으로 진행하는지 확인한다 (아랫쪽:TRUE, 위쪽:FALSE)
	this->notePadForm->findSameText->searchDown = TRUE;


	//1.4. 사용자가 전체 단어만 일치시킬지 여부를 확인한다.
	this->notePadForm->findSameText->searchAround = ((CButton*)GetDlgItem(IDC_BUTTON_SEARCHAROUND))->GetCheck();

	//1.5. FindSameText클래스의 SearchText를 호출한다.
	this->notePadForm->findSameText->SearchText();

	//1.6. 같은 단어가 없으면 메세지박스를 출력한다. (""을(를) 찾을 수 없습니다.)
	if (this->notePadForm->selectWords->firstX <= 0 && this->notePadForm->selectWords->firstY <= 0 &&
		this->notePadForm->selectWords->endX <= 0 && this->notePadForm->selectWords->endY <= 0) {

		CString str;
		str.Format("\"%s\"을(를) 찾을 수 없습니다.", this->notePadForm->findSameText->findStr);
		MessageBox(str, "메모장", MB_OK);
	}

	//1.7. 스크롤 이동여부 확인한다. ******************************************************************
	this->notePadForm->isJumpOverForPrevious = FALSE;
	this->notePadForm->isJumpOverForNext = FALSE;
	this->notePadForm->isUp = FALSE;
	this->notePadForm->isDown = FALSE;
	this->notePadForm->isDoubleByte = FALSE;

	//**************************************************************************************************

	this->notePadForm->Notify(); // 만약 스크롤 범위 밖에 단어가 있을 경우
	this->notePadForm->Invalidate();

}

void ReplaceDialog::OnReplaceButtonClicked() { //OnReplaceCommand에서 처리됩니다.

	//1. "바꾸다" 버튼을 눌렀을 경우,
	this->notePadForm->findSameText->exception = FALSE;

	//1.0. 바꿀 내용을 확인한다.
	GetDlgItem(IDC_EDIT_REPLACETEXT)->GetWindowText(this->notePadForm->findSameText->replaceStr);

	//1.1. 찾을 내용을 확인한다.
	GetDlgItem(IDC_EDIT_FINDTEXT)->GetWindowText(this->notePadForm->findSameText->findStr);

	//1.2. 사용자가 검색 문자열의 대/소문자를 정확히 일치시키는지 확인한다. (구분:TRUE, 안구분:FALSE)
	this->notePadForm->findSameText->matchCase = ((CButton*)GetDlgItem(IDC_BUTTON_MATCHCASE))->GetCheck();

	//1.3. 사용자가 검색을 아래쪽 방향으로 진행하는지 확인한다 (아랫쪽:TRUE, 위쪽:FALSE)
	this->notePadForm->findSameText->searchDown = TRUE;

	//1.4. 사용자가 전체 단어만 일치시킬지 여부를 확인한다.
	this->notePadForm->findSameText->searchAround = ((CButton*)GetDlgItem(IDC_BUTTON_SEARCHAROUND))->GetCheck();



#if 0
	//1.5. FindSameText클래스의 ReplaceAndSearchText를 호출한다.
	this->notePadForm->findSameText->ReplaceAndSearchText();


	//1.6. 같은 단어가 없으면 메세지박스를 출력한다. (""을(를) 찾을 수 없습니다.)
	if ((this->notePadForm->selectWords->firstX <= 0 && this->notePadForm->selectWords->firstY <= 0 &&
		this->notePadForm->selectWords->endX <= 0 && this->notePadForm->selectWords->endY <= 0)
		|| this->notePadForm->findSameText->exception == TRUE) {

		CString str;
		str.Format("\"%s\"을(를) 찾을 수 없습니다.", (LPCTSTR)this->notePadForm->findSameText->findStr);
		MessageBox(str, "메모장", MB_OK);
	}

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

void ReplaceDialog::OnReplaceAllButtonClicked() { //OnReplaceAllCommand에서 처리됩니다.
	BOOL isFirst = TRUE;

	//1. "모두 바꾸다" 버튼을 눌렀을 경우,

	//1.0. 바꿀 내용을 확인한다.
	GetDlgItem(IDC_EDIT_REPLACETEXT)->GetWindowText(this->notePadForm->findSameText->replaceStr);

	//1.1. 찾을 내용을 확인한다.
	GetDlgItem(IDC_EDIT_FINDTEXT)->GetWindowText(this->notePadForm->findSameText->findStr);

	//1.2. 사용자가 검색 문자열의 대/소문자를 정확히 일치시키는지 확인한다. (구분:TRUE, 안구분:FALSE)
	this->notePadForm->findSameText->matchCase = ((CButton*)GetDlgItem(IDC_BUTTON_MATCHCASE))->GetCheck();

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
}



void ReplaceDialog::OnClose() {

	// 각 버튼의 변화된 옵션을 저장한다.
	this->notePadForm->findSameText->matchCase = ((CButton*)GetDlgItem(IDC_BUTTON_MATCHCASE))->GetCheck();
	this->notePadForm->findSameText->searchAround = ((CButton*)GetDlgItem(IDC_BUTTON_SEARCHAROUND))->GetCheck();

	//닫기버튼을 클릭했을 때, 윈도우를 종료한다.
	this->notePadForm->replaceDialog = NULL;
	EndDialog(0);
}

void ReplaceDialog::OnCancel() {

	// 각 버튼의 변화된 옵션을 저장한다.
	this->notePadForm->findSameText->matchCase = ((CButton*)GetDlgItem(IDC_BUTTON_MATCHCASE))->GetCheck();
	this->notePadForm->findSameText->searchAround = ((CButton*)GetDlgItem(IDC_BUTTON_SEARCHAROUND))->GetCheck();

	//닫기버튼을 클릭했을 때, 윈도우를 종료한다.
	this->notePadForm->replaceDialog = NULL;
	EndDialog(0);

}

BOOL ReplaceDialog::PreTranslateMessage(MSG* pMsg) {
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_ESCAPE) {

			// 각 버튼의 변화된 옵션을 저장한다.
			this->notePadForm->findSameText->matchCase = ((CButton*)GetDlgItem(IDC_BUTTON_MATCHCASE))->GetCheck();
			this->notePadForm->findSameText->searchAround = ((CButton*)GetDlgItem(IDC_BUTTON_SEARCHAROUND))->GetCheck();

			//닫기버튼을 클릭했을 때, 윈도우를 종료한다.
			this->notePadForm->replaceDialog = NULL;
			EndDialog(0);
			return TRUE;
		}
	}

	return 0;
}


