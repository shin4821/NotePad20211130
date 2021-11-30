//FindDialog.cpp
#include"FindDialog.h"
#include"NotePadForm.h"
#include"FindSameText.h"
#include"SelectWords.h"
#include"Scroll.h"
#include"FactoryProductCommand.h"
#include"Command.h"

BEGIN_MESSAGE_MAP(FindDialog, CFindReplaceDialog)
	ON_COMMAND_RANGE(40042, 40042, OnMyCommand)
	ON_EN_CHANGE(IDC_EDIT_FINDTEXT, OnFindTextChange)
	ON_BN_CLICKED(IDC_BUTTON_FINDNEXT, OnFindNextButtonClicked)

	ON_BN_CLICKED(IDCANCEL, OnCancel)
	//ON_WM_CANCELMODE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

//(21.09.01) 추가
void FindDialog::OnMyCommand(UINT id) {
	FactoryProductCommand factoryProductCommand(this->notePadForm);
	Command* command = NULL;

	// 해당 command를 생성한다.
	command = factoryProductCommand.Make(id);

	//해당 command를 실행한다.
	command->Execute();

#if 0
	//해당 command를 undolist에 쌓는다.
	//실행취소 또는 실행복구가 아니면,
	if (id != 40040) {
		//실행한 command를 history list에 추가한다.
		this->commandStory->Pile(command);
	}
#endif

	Invalidate();

}

FindDialog::FindDialog(NotePadForm* notePadForm) {
	this->notePadForm = notePadForm;
}

BOOL FindDialog::Create(LPCTSTR lpszFindWhat,DWORD dwFlags, CWnd* pParentWnd ) {
	
	BOOL ret;
	string strCharacter;
	this->notePadForm = (NotePadForm*) pParentWnd;

	this->m_fr.Flags |= FR_ENABLETEMPLATE;
	this->m_fr.hInstance = AfxGetResourceHandle();
	this->m_fr.lpTemplateName = MAKEINTRESOURCE(FindDialog::IDD);

	//(21.09.15 추가) 1. 현재 선택된 단어가 있는 경우, 
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

	ret = CFindReplaceDialog::Create(TRUE, (LPCTSTR)this->notePadForm->findSameText->findStr,
		(LPCTSTR)this->notePadForm->findSameText->findStr, dwFlags, pParentWnd);


	//1. FindSameText의 멤버 옵션에 따라, 멤버 옵션을 설정해준다.
    //1.1. 사용자가 검색 문자열의 대/소문자를 정확히 일치시키는지 확인한다. (구분:TRUE, 안구분:FALSE)

	if (this->notePadForm->findSameText->matchCase == TRUE) {
		((CButton*)GetDlgItem(IDC_BUTTON_MATCHCASE))->SetCheck(BST_CHECKED);
	}
	else {
		((CButton*)GetDlgItem(IDC_BUTTON_MATCHCASE))->SetCheck(BST_UNCHECKED);
	}


	//1.2. 사용자가 전체 단어만 일치시킬지 여부를 확인한다.
	if (this->notePadForm->findSameText->searchAround == TRUE) {
		((CButton*)GetDlgItem(IDC_BUTTON_SEARCHAROUND))->SetCheck(BST_CHECKED);
	}
	else {
		((CButton*)GetDlgItem(IDC_BUTTON_SEARCHAROUND))->SetCheck(BST_UNCHECKED);
	}




	return ret;
}


void FindDialog::OnFindNextButtonClicked() {

#if 0
	//1.0. 이전 단어를 구해놓는다.
	this->notePadForm->findSameText->beforeFindStr = this->notePadForm->findSameText->findStr;

	//1.1. 기재한 찾을 내용을 확인한다.
	GetDlgItem(IDC_EDIT_FINDTEXT)->GetWindowText(this->notePadForm->findSameText->findStr);

	//1.2. 사용자가 검색 문자열의 대/소문자를 정확히 일치시키는지 확인한다. (구분:TRUE, 안구분:FALSE)
	this->notePadForm->findSameText->matchCase = ((CButton*)GetDlgItem(IDC_BUTTON_MATCHCASE))->GetCheck();

	//1.3. 사용자가 검색을 아래쪽 방향으로 진행하는지 확인한다 (아랫쪽:TRUE, 위쪽:FALSE)
	this->notePadForm->findSameText->searchDown = ((CButton*)GetDlgItem(IDC_BUTTON_DOWN))->GetCheck();

	//1.4. 사용자가 전체 단어만 일치시킬지 여부를 확인한다.
	this->notePadForm->findSameText->searchAround = ((CButton*)GetDlgItem(IDC_BUTTON_SEARCHAROUND))->GetCheck();


	//1.5. 검색을 아랫쪽 방향으로 진행하면 "다음검색"을 활성화한다,

	this->notePadForm->SendMessage(WM_COMMAND, 40040);

	//1.6. 검색을 위쪽 방향으로 진행하면 "이전검색"을 활성화한다.
#endif


	//1. "다음 찾기"버튼을 눌렀을 경우,

	    //1.0. 이전 단어를 구해놓는다.
	    this->notePadForm->findSameText->beforeFindStr = this->notePadForm->findSameText->findStr;

        //1.1. 기재한 찾을 내용을 확인한다.
	    GetDlgItem(IDC_EDIT_FINDTEXT)->GetWindowText(this->notePadForm->findSameText->findStr);

		//1.2. 사용자가 검색 문자열의 대/소문자를 정확히 일치시키는지 확인한다. (구분:TRUE, 안구분:FALSE)
		this->notePadForm->findSameText->matchCase = ((CButton*)GetDlgItem(IDC_BUTTON_MATCHCASE))->GetCheck();
		
		//1.3. 사용자가 검색을 아래쪽 방향으로 진행하는지 확인한다 (아랫쪽:TRUE, 위쪽:FALSE)
		this->notePadForm->findSameText->searchDown = ((CButton*)GetDlgItem(IDC_BUTTON_DOWN))->GetCheck();

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

		//(21.11.04.추가) "위로"가 추가되있으면 isUp을 TRUE로 변경한다.
		if (this->notePadForm->findSameText->searchDown == FALSE) {
			this->notePadForm->isJumpOverForPrevious = FALSE;
			this->notePadForm->isJumpOverForNext = FALSE;
			//this->notePadForm->isUp = TRUE; //주위에 배치일때 맨위에서 맨밑으로 갈때 반대로 설정
			//this->notePadForm->isDown = FALSE;
			this->notePadForm->isDoubleByte = FALSE;
		}
		else {
			//1.7. 스크롤 이동여부 확인한다. ******************************************************************
			this->notePadForm->isJumpOverForPrevious = FALSE;
			this->notePadForm->isJumpOverForNext = FALSE;
			//this->notePadForm->isUp = FALSE; //주위에 배치일때 맨밑에서 맨위로 갈때 반대로 설정
			//this->notePadForm->isDown = FALSE;
			this->notePadForm->isDoubleByte = FALSE;
			//**************************************************************************************************
		}


	
	this->notePadForm->Notify(); // 만약 스크롤 범위 밖에 단어가 있을 경우
	this->notePadForm->Invalidate();

}

void FindDialog::OnFindTextChange() {

	//1. 이전 단어를 구해놓는다.
	this->notePadForm->findSameText->beforeFindStr = this->notePadForm->findSameText->findStr;

	//2. 기재한 찾을 내용을 확인한다.
	GetDlgItem(IDC_EDIT_FINDTEXT)->GetWindowText(this->notePadForm->findSameText->findStr);

	//(21.09.15 추가) 3. 찾는 내용이 NULL이면, 
	if (this->notePadForm->findSameText->findStr == "\0") {
		//3.1. 찾기 버튼을 비활성화 시킨다.
		GetDlgItem(IDC_BUTTON_FINDNEXT)->EnableWindow(FALSE);
	}
	else { //4. 찾는 내용이 NULL이 아니면,
		//4.1. 찾기 버튼을 활성화 시킨다.
		GetDlgItem(IDC_BUTTON_FINDNEXT)->EnableWindow(TRUE);
	}

	this->notePadForm->findSameText->beforeFindStr = this->notePadForm->findSameText->findStr;
}


void FindDialog::OnClose() {

	// 각 버튼의 변화된 옵션을 저장한다.
	this->notePadForm->findSameText->matchCase = ((CButton*)GetDlgItem(IDC_BUTTON_MATCHCASE))->GetCheck();
	this->notePadForm->findSameText->searchAround = ((CButton*)GetDlgItem(IDC_BUTTON_SEARCHAROUND))->GetCheck();
	this->notePadForm->findSameText->searchDown = ((CButton*)GetDlgItem(IDC_BUTTON_DOWN))->GetCheck();

	//닫기버튼을 클릭했을 때, 윈도우를 종료한다.
	this->notePadForm->findDialog = NULL;
	EndDialog(0);
}

#if 0
void FindDialog::Oncancelmode() {
	// 각 버튼의 변화된 옵션을 저장한다.
	this->notePadForm->findSameText->matchCase = ((CButton*)GetDlgItem(IDC_BUTTON_MATCHCASE))->GetCheck();
	this->notePadForm->findSameText->searchAround = ((CButton*)GetDlgItem(IDC_BUTTON_SEARCHAROUND))->GetCheck();
	this->notePadForm->findSameText->searchDown = ((CButton*)GetDlgItem(IDC_BUTTON_DOWN))->GetCheck();

	//닫기버튼을 클릭했을 때, 윈도우를 종료한다.
	this->notePadForm->findDialog = NULL;
	EndDialog(0);
}
#endif

BOOL FindDialog::PreTranslateMessage(MSG* pMsg) {
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_ESCAPE) {

			// 각 버튼의 변화된 옵션을 저장한다.
			this->notePadForm->findSameText->matchCase = ((CButton*)GetDlgItem(IDC_BUTTON_MATCHCASE))->GetCheck();
			this->notePadForm->findSameText->searchAround = ((CButton*)GetDlgItem(IDC_BUTTON_SEARCHAROUND))->GetCheck();
			this->notePadForm->findSameText->searchDown = ((CButton*)GetDlgItem(IDC_BUTTON_DOWN))->GetCheck();

			this->notePadForm->findDialog = NULL;
			EndDialog(0);
			return TRUE;
		}
	}

	return 0;
}

void FindDialog::OnCancel() {
	// 각 버튼의 변화된 옵션을 저장한다.
	this->notePadForm->findSameText->matchCase = ((CButton*)GetDlgItem(IDC_BUTTON_MATCHCASE))->GetCheck();
	this->notePadForm->findSameText->searchAround = ((CButton*)GetDlgItem(IDC_BUTTON_SEARCHAROUND))->GetCheck();
	this->notePadForm->findSameText->searchDown = ((CButton*)GetDlgItem(IDC_BUTTON_DOWN))->GetCheck();

	//닫기버튼을 클릭했을 때, 윈도우를 종료한다.
	this->notePadForm->findDialog = NULL;
	EndDialog(0);


}







