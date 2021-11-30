//OnFindPreviousCommand.cpp
#include"OnFindPreviousCommand.h"
#include"NotePadForm.h"
#include"SelectWords.h"
#include"FindSameText.h"
#include"FindDialog.h"
#include"resource.h"

OnFindPreviousCommand::OnFindPreviousCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
}

OnFindPreviousCommand::~OnFindPreviousCommand() {
}

void OnFindPreviousCommand::Execute() {
	//1.3. 찾는 단어가 없으면 메세지박스를 출력한다. (""을(를) 찾을 수 없습니다.)
	if (this->notePadForm->findSameText->findStr == "") {
#if 0
		CString str = "찾는 내용이 없습니다.";
		this->notePadForm->MessageBox(str, "메모장", MB_OK);
#endif
		this->notePadForm->SendMessage(WM_COMMAND, 40039);
	}
	else {
		//1.4. 검색 방향을 위쪽으로 고정한다.
		this->notePadForm->findSameText->searchDown = FALSE;

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
		this->notePadForm->isUp = FALSE;
		this->notePadForm->isDown = FALSE;
		this->notePadForm->isDoubleByte = FALSE;
		//**************************************************************************************************

		this->notePadForm->Notify(); // 만약 스크롤 범위 밖에 단어가 있을 경우
		this->notePadForm->Invalidate();
	}

}











