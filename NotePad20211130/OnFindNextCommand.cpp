//OnFindNextCommand.cpp
#include"OnFindNextCommand.h"
#include"NotePadForm.h"
#include"SelectWords.h"
#include"FindSameText.h"
#include"FindDialog.h"
#include"resource.h"

OnFindNextCommand::OnFindNextCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
}

OnFindNextCommand::~OnFindNextCommand() {
}

void OnFindNextCommand::Execute() {
	HWND hDlg = GetForegroundWindow();
	HWND hWnd = FindWindow(NULL, "찾기");

	//1. "다음 찾기"버튼을 눌렀을 경우,


#if 0
	 //(21.09.15 추가) 1.2. 찾는 내용이 NULL이면 
	if (this->notePadForm->findSameText->findStr == "") {

        //1.2.1. 현재 찾기 창이 띄어져 있으면 닫는다.
		//if (GetDlgItem(IDD_FINDINGFORM) != NULL && IDD_FINDINGFORM->IsWindowVisible() == TRUE) {
		if (hDlg == hWnd) {
			this->notePadForm->findDialog = NULL;
			this->notePadForm->findDialog->EndDialog(0);
		}
		//1.2.2. 찾기 창을 띄운다.
		else {
			if (this->notePadForm->findDialog == NULL) {
				this->notePadForm->findDialog = new FindDialog(this->notePadForm);
			}
			this->notePadForm->findDialog->Create(NULL, FR_DOWN, this->notePadForm);
		}

	}
#endif

#if 0
	     //1.0. 이전 단어를 구해놓는다.
	     this->notePadForm->findSameText->beforeFindStr = this->notePadForm->findSameText->findStr;

	     //1.1. 기재한 찾을 내용을 확인한다.
	     this->notePadForm->findDialog->GetDlgItem(IDC_EDIT_FINDTEXT)->GetWindowText(this->notePadForm->findSameText->findStr);

		//1.2. 사용자가 검색 문자열의 대/소문자를 정확히 일치시키는지 확인한다. (구분:TRUE, 안구분:FALSE)
		this->notePadForm->findSameText->matchCase = ((CButton*)this->notePadForm->findDialog->GetDlgItem(IDC_BUTTON_MATCHCASE))->GetCheck();

		//1.3. 사용자가 검색을 아래쪽 방향으로 진행하는지 확인한다 (아랫쪽:TRUE, 위쪽:FALSE)
		this->notePadForm->findSameText->searchDown = ((CButton*)this->notePadForm->findDialog->GetDlgItem(IDC_BUTTON_DOWN))->GetCheck();

		//1.4. 사용자가 전체 단어만 일치시킬지 여부를 확인한다.
		this->notePadForm->findSameText->searchAround = ((CButton*)this->notePadForm->findDialog->GetDlgItem(IDC_BUTTON_SEARCHAROUND))->GetCheck();
#endif
		//1.3. 찾는 단어가 없으면 메세지박스를 출력한다. (""을(를) 찾을 수 없습니다.)
		if (this->notePadForm->findSameText->findStr == "") {
#if 0
			CString str = "찾는 내용이 없습니다.";
			this->notePadForm->MessageBox(str, "메모장", MB_OK);
#endif
			this->notePadForm->SendMessage(WM_COMMAND, 40039);
		}
		else {
			//1.4. 검색 방향을 아래쪽으로 고정한다.
			this->notePadForm->findSameText->searchDown = TRUE;

			//1.5. FindSameText클래스의 SearchText를 호출한다.
			this->notePadForm->findSameText->SearchText();

			//1.6. 같은 단어가 없으면 메세지박스를 출력한다. (""을(를) 찾을 수 없습니다.)
			if (this->notePadForm->selectWords->firstX <= 0 && this->notePadForm->selectWords->firstY <= 0 &&
				this->notePadForm->selectWords->endX <= 0 && this->notePadForm->selectWords->endY <= 0) {

				CString str;

				if (this->notePadForm->findSameText->findStr != "") {
					str.Format("\"%s\"을(를) 찾을 수 없습니다.", this->notePadForm->findSameText->findStr);
				}
				else {
					str = "찾는 내용이 없습니다.";
				}

				this->notePadForm->MessageBox(str, "메모장", MB_OK);
			}

			//1.7. 스크롤 이동여부 확인한다. ******************************************************************
			this->notePadForm->isJumpOverForPrevious = FALSE;
			this->notePadForm->isJumpOverForNext = FALSE;
			this->notePadForm->isDoubleByte = FALSE;
			//**************************************************************************************************

			this->notePadForm->Notify(); // 만약 스크롤 범위 밖에 단어가 있을 경우
			this->notePadForm->Invalidate();
		
		}
}


