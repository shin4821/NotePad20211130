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
	HWND hWnd = FindWindow(NULL, "ã��");

	//1. "���� ã��"��ư�� ������ ���,


#if 0
	 //(21.09.15 �߰�) 1.2. ã�� ������ NULL�̸� 
	if (this->notePadForm->findSameText->findStr == "") {

        //1.2.1. ���� ã�� â�� ����� ������ �ݴ´�.
		//if (GetDlgItem(IDD_FINDINGFORM) != NULL && IDD_FINDINGFORM->IsWindowVisible() == TRUE) {
		if (hDlg == hWnd) {
			this->notePadForm->findDialog = NULL;
			this->notePadForm->findDialog->EndDialog(0);
		}
		//1.2.2. ã�� â�� ����.
		else {
			if (this->notePadForm->findDialog == NULL) {
				this->notePadForm->findDialog = new FindDialog(this->notePadForm);
			}
			this->notePadForm->findDialog->Create(NULL, FR_DOWN, this->notePadForm);
		}

	}
#endif

#if 0
	     //1.0. ���� �ܾ ���س��´�.
	     this->notePadForm->findSameText->beforeFindStr = this->notePadForm->findSameText->findStr;

	     //1.1. ������ ã�� ������ Ȯ���Ѵ�.
	     this->notePadForm->findDialog->GetDlgItem(IDC_EDIT_FINDTEXT)->GetWindowText(this->notePadForm->findSameText->findStr);

		//1.2. ����ڰ� �˻� ���ڿ��� ��/�ҹ��ڸ� ��Ȯ�� ��ġ��Ű���� Ȯ���Ѵ�. (����:TRUE, �ȱ���:FALSE)
		this->notePadForm->findSameText->matchCase = ((CButton*)this->notePadForm->findDialog->GetDlgItem(IDC_BUTTON_MATCHCASE))->GetCheck();

		//1.3. ����ڰ� �˻��� �Ʒ��� �������� �����ϴ��� Ȯ���Ѵ� (�Ʒ���:TRUE, ����:FALSE)
		this->notePadForm->findSameText->searchDown = ((CButton*)this->notePadForm->findDialog->GetDlgItem(IDC_BUTTON_DOWN))->GetCheck();

		//1.4. ����ڰ� ��ü �ܾ ��ġ��ų�� ���θ� Ȯ���Ѵ�.
		this->notePadForm->findSameText->searchAround = ((CButton*)this->notePadForm->findDialog->GetDlgItem(IDC_BUTTON_SEARCHAROUND))->GetCheck();
#endif
		//1.3. ã�� �ܾ ������ �޼����ڽ��� ����Ѵ�. (""��(��) ã�� �� �����ϴ�.)
		if (this->notePadForm->findSameText->findStr == "") {
#if 0
			CString str = "ã�� ������ �����ϴ�.";
			this->notePadForm->MessageBox(str, "�޸���", MB_OK);
#endif
			this->notePadForm->SendMessage(WM_COMMAND, 40039);
		}
		else {
			//1.4. �˻� ������ �Ʒ������� �����Ѵ�.
			this->notePadForm->findSameText->searchDown = TRUE;

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
			this->notePadForm->isDoubleByte = FALSE;
			//**************************************************************************************************

			this->notePadForm->Notify(); // ���� ��ũ�� ���� �ۿ� �ܾ ���� ���
			this->notePadForm->Invalidate();
		
		}
}


