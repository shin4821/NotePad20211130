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
	//1.3. ã�� �ܾ ������ �޼����ڽ��� ����Ѵ�. (""��(��) ã�� �� �����ϴ�.)
	if (this->notePadForm->findSameText->findStr == "") {
#if 0
		CString str = "ã�� ������ �����ϴ�.";
		this->notePadForm->MessageBox(str, "�޸���", MB_OK);
#endif
		this->notePadForm->SendMessage(WM_COMMAND, 40039);
	}
	else {
		//1.4. �˻� ������ �������� �����Ѵ�.
		this->notePadForm->findSameText->searchDown = FALSE;

		//1.5. FindSameTextŬ������ SearchText�� ȣ���Ѵ�.
		this->notePadForm->findSameText->SearchText();

		//1.6. ���� �ܾ ������ �޼����ڽ��� ����Ѵ�. (""��(��) ã�� �� �����ϴ�.)
		if (this->notePadForm->selectWords->firstX <= 0 && this->notePadForm->selectWords->firstY <= 0 &&
			this->notePadForm->selectWords->endX <= 0 && this->notePadForm->selectWords->endY <= 0) {

			CString str;

			if (this->notePadForm->findSameText->findStr != "") {
				str.Format("\"%s\"��(��) ã�� �� �����ϴ�.", this->notePadForm->findSameText->findStr);
			}
			else {
				str = "ã�� ������ �����ϴ�.";
			}

			this->notePadForm->MessageBox(str, "�޸���", MB_OK);
		}

		//1.7. ��ũ�� �̵����� Ȯ���Ѵ�. ******************************************************************
		this->notePadForm->isJumpOverForPrevious = FALSE;
		this->notePadForm->isJumpOverForNext = FALSE;
		this->notePadForm->isUp = FALSE;
		this->notePadForm->isDown = FALSE;
		this->notePadForm->isDoubleByte = FALSE;
		//**************************************************************************************************

		this->notePadForm->Notify(); // ���� ��ũ�� ���� �ۿ� �ܾ ���� ���
		this->notePadForm->Invalidate();
	}

}











