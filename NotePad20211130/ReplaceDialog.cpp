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

	//(21.11.04. �߰�) ����ڰ� ��ü �ܾ ��ġ��ų�� ���θ� Ȯ���Ѵ�.
	this->notePadForm->findSameText->searchAround = ((CButton*)GetDlgItem(IDC_BUTTON_SEARCHAROUND))->GetCheck();


	// �ش� command�� �����Ѵ�.
	command = factoryProductCommand.Make(id);

	//�ش� command�� �����Ѵ�.
	if (dynamic_cast<OnReplaceAllCommand*>(command)) {

		//2. ���� �ܾ �ִ� ���� �ݺ��Ѵ�.
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

			//replace�� �ܾ ������ ������ command�� history list�� �߰��Ѵ�.
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
		//(21.10.29.�߰�) ���� Undolist�� ��ü�׸��� ���캻��.
		this->notePadForm->commandStory->undoList->Check();
	}
	else {
		command->Execute();
	}

	

	//���� ã�Ⱑ �ƴ� ���,
	if (id != 40040 && command->GetIsReplaced() == true) {
		//������ command�� history list�� �߰��Ѵ�.
		this->notePadForm->commandStory->Pile(command);
	}



	Invalidate();
}


ReplaceDialog::ReplaceDialog(NotePadForm* notePadForm) {
	this->notePadForm = notePadForm;

}

void ReplaceDialog::OnFindTextChange() {

	//1. ���� �ܾ ���س��´�.
	this->notePadForm->findSameText->beforeFindStr = this->notePadForm->findSameText->findStr;

	//2. ������ ã�� ������ Ȯ���Ѵ�.
	GetDlgItem(IDC_EDIT_FINDTEXT)->GetWindowText(this->notePadForm->findSameText->findStr);


	//(21.09.15 �߰�) 3. ã�� ������ NULL�̸�, 
	if (this->notePadForm->findSameText->findStr == "\0") {
		//3.1. ã�� ��ư�� ��Ȱ��ȭ ��Ų��.
		GetDlgItem(IDC_BUTTON_FINDNEXT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_REPLACE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_REPLACEALL)->EnableWindow(FALSE);
	}
	else { //4. ã�� ������ NULL�� �ƴϸ�,
		//4.1. ã�� ��ư�� Ȱ��ȭ ��Ų��.
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

	//(21.09.16 �߰�) 1. ���� ���õ� �ܾ �ִ� ���, 
	if (this->notePadForm->selectWords->GetFirstX() > 0 || this->notePadForm->selectWords->GetFirstY() > 0 ||
		this->notePadForm->selectWords->GetEndX() > 0 || this->notePadForm->selectWords->GetEndY() > 0) {

		//1.1. ���õ� �ܾ ���Ѵ�.
		strCharacter = this->notePadForm->selectWords->GetSelectedWord();
		this->notePadForm->findSameText->findStr = CString(strCharacter.c_str());

		//1.2. �ش� �ܾ "ã�� ����"�� ���´�.
	}

	//2. ���õ� ������ ���� ���, �� �� �ܾ �־��ش�.
	else {
		strCharacter = this->notePadForm->findSameText->beforeFindStr;
		this->notePadForm->findSameText->findStr = CString(strCharacter.c_str());
	}


	BOOL ret = CFindReplaceDialog::Create(FALSE, (LPCTSTR)this->notePadForm->findSameText->findStr,
		"", dwFlags, pParentWnd);


	//2. FindSameText�� ��� �ɼǿ� ����, ��� �ɼ��� �������ش�.
	//2.1. ����ڰ� �˻� ���ڿ��� ��/�ҹ��ڸ� ��Ȯ�� ��ġ��Ű���� Ȯ���Ѵ�. (����:TRUE, �ȱ���:FALSE)
	if (this->notePadForm->findSameText->matchCase == TRUE) {
		((CButton*)GetDlgItem(IDC_BUTTON_MATCHCASE))->SetCheck(BST_CHECKED);
	}
	else {
		((CButton*)GetDlgItem(IDC_BUTTON_MATCHCASE))->SetCheck(BST_UNCHECKED);
	}

	//2.2. ����ڰ� ��ü �ܾ ��ġ��ų�� ���θ� Ȯ���Ѵ�.
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

void ReplaceDialog::OnFineNextButtonClicked() { //OnFindNextCommand���� ó���˴ϴ�. 

	//1. "���� ã��"��ư�� ������ ���,

		//1.0. ���� �ܾ ���س��´�.
	this->notePadForm->findSameText->beforeFindStr = this->notePadForm->findSameText->findStr;

	//1.1. ������ ã�� ������ Ȯ���Ѵ�.
	GetDlgItem(IDC_EDIT_FINDTEXT)->GetWindowText(this->notePadForm->findSameText->findStr);

	//1.2. ����ڰ� �˻� ���ڿ��� ��/�ҹ��ڸ� ��Ȯ�� ��ġ��Ű���� Ȯ���Ѵ�. (����:TRUE, �ȱ���:FALSE)
	this->notePadForm->findSameText->matchCase = ((CButton*)GetDlgItem(IDC_BUTTON_MATCHCASE))->GetCheck();


	//1.3. ����ڰ� �˻��� �Ʒ��� �������� �����ϴ��� Ȯ���Ѵ� (�Ʒ���:TRUE, ����:FALSE)
	this->notePadForm->findSameText->searchDown = TRUE;


	//1.4. ����ڰ� ��ü �ܾ ��ġ��ų�� ���θ� Ȯ���Ѵ�.
	this->notePadForm->findSameText->searchAround = ((CButton*)GetDlgItem(IDC_BUTTON_SEARCHAROUND))->GetCheck();

	//1.5. FindSameTextŬ������ SearchText�� ȣ���Ѵ�.
	this->notePadForm->findSameText->SearchText();

	//1.6. ���� �ܾ ������ �޼����ڽ��� ����Ѵ�. (""��(��) ã�� �� �����ϴ�.)
	if (this->notePadForm->selectWords->firstX <= 0 && this->notePadForm->selectWords->firstY <= 0 &&
		this->notePadForm->selectWords->endX <= 0 && this->notePadForm->selectWords->endY <= 0) {

		CString str;
		str.Format("\"%s\"��(��) ã�� �� �����ϴ�.", this->notePadForm->findSameText->findStr);
		MessageBox(str, "�޸���", MB_OK);
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

void ReplaceDialog::OnReplaceButtonClicked() { //OnReplaceCommand���� ó���˴ϴ�.

	//1. "�ٲٴ�" ��ư�� ������ ���,
	this->notePadForm->findSameText->exception = FALSE;

	//1.0. �ٲ� ������ Ȯ���Ѵ�.
	GetDlgItem(IDC_EDIT_REPLACETEXT)->GetWindowText(this->notePadForm->findSameText->replaceStr);

	//1.1. ã�� ������ Ȯ���Ѵ�.
	GetDlgItem(IDC_EDIT_FINDTEXT)->GetWindowText(this->notePadForm->findSameText->findStr);

	//1.2. ����ڰ� �˻� ���ڿ��� ��/�ҹ��ڸ� ��Ȯ�� ��ġ��Ű���� Ȯ���Ѵ�. (����:TRUE, �ȱ���:FALSE)
	this->notePadForm->findSameText->matchCase = ((CButton*)GetDlgItem(IDC_BUTTON_MATCHCASE))->GetCheck();

	//1.3. ����ڰ� �˻��� �Ʒ��� �������� �����ϴ��� Ȯ���Ѵ� (�Ʒ���:TRUE, ����:FALSE)
	this->notePadForm->findSameText->searchDown = TRUE;

	//1.4. ����ڰ� ��ü �ܾ ��ġ��ų�� ���θ� Ȯ���Ѵ�.
	this->notePadForm->findSameText->searchAround = ((CButton*)GetDlgItem(IDC_BUTTON_SEARCHAROUND))->GetCheck();



#if 0
	//1.5. FindSameTextŬ������ ReplaceAndSearchText�� ȣ���Ѵ�.
	this->notePadForm->findSameText->ReplaceAndSearchText();


	//1.6. ���� �ܾ ������ �޼����ڽ��� ����Ѵ�. (""��(��) ã�� �� �����ϴ�.)
	if ((this->notePadForm->selectWords->firstX <= 0 && this->notePadForm->selectWords->firstY <= 0 &&
		this->notePadForm->selectWords->endX <= 0 && this->notePadForm->selectWords->endY <= 0)
		|| this->notePadForm->findSameText->exception == TRUE) {

		CString str;
		str.Format("\"%s\"��(��) ã�� �� �����ϴ�.", (LPCTSTR)this->notePadForm->findSameText->findStr);
		MessageBox(str, "�޸���", MB_OK);
	}

	//1.7. ��ũ�� �̵����� Ȯ���Ѵ�. ******************************************************************
	this->notePadForm->isJumpOverForPrevious = FALSE;
	this->notePadForm->isJumpOverForNext = FALSE;
	this->notePadForm->isUp = FALSE;
	this->notePadForm->isDown = FALSE;
	this->notePadForm->isDoubleByte = FALSE;

	this->notePadForm->Notify(); // ���� ��ũ�� ���� �ۿ� �ܾ ���� ���
	this->notePadForm->Invalidate();
	//**************************************************************************************************
#endif
}

void ReplaceDialog::OnReplaceAllButtonClicked() { //OnReplaceAllCommand���� ó���˴ϴ�.
	BOOL isFirst = TRUE;

	//1. "��� �ٲٴ�" ��ư�� ������ ���,

	//1.0. �ٲ� ������ Ȯ���Ѵ�.
	GetDlgItem(IDC_EDIT_REPLACETEXT)->GetWindowText(this->notePadForm->findSameText->replaceStr);

	//1.1. ã�� ������ Ȯ���Ѵ�.
	GetDlgItem(IDC_EDIT_FINDTEXT)->GetWindowText(this->notePadForm->findSameText->findStr);

	//1.2. ����ڰ� �˻� ���ڿ��� ��/�ҹ��ڸ� ��Ȯ�� ��ġ��Ű���� Ȯ���Ѵ�. (����:TRUE, �ȱ���:FALSE)
	this->notePadForm->findSameText->matchCase = ((CButton*)GetDlgItem(IDC_BUTTON_MATCHCASE))->GetCheck();

	//1.3. ����ڰ� �˻��� �Ʒ��� �������� �����ϴ��� Ȯ���Ѵ� (�Ʒ���:TRUE, ����:FALSE)
	this->notePadForm->findSameText->searchDown = TRUE;

	//1.4. ����ڰ� ��ü �ܾ ��ġ��ų�� ���θ� Ȯ���Ѵ�.
	this->notePadForm->findSameText->searchAround = TRUE;

	//1.5. FindSameTextŬ������ ReplaceAndSearchText�� ȣ���Ѵ�.
	//1.5.1. ���� Ŀ���� ��ġ�� (0,0)���� �������ش�.
	this->notePadForm->note->MoveCurrent(0);
	Glyph* row = this->notePadForm->note->GetChild(0);
	row->MoveCurrent(0);

	//1.5.2. ���� �ܾ �ִ� ���� �ݺ��Ѵ�.

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

	//1.5.3. ���� ĳ���� ��ġ�� �ʱ�ȭ�Ѵ�.
	this->notePadForm->note->MoveCurrent(0);
	row = this->notePadForm->note->GetChild(0);
	row->MoveCurrent(0);

#if 0
	//1.6. ���� �ܾ ������ �޼����ڽ��� ����Ѵ�. (""��(��) ã�� �� �����ϴ�.)
	if ((this->notePadForm->selectWords->firstX <= 0 && this->notePadForm->selectWords->firstY <= 0 &&
		this->notePadForm->selectWords->endX <= 0 && this->notePadForm->selectWords->endY <= 0)
		|| this->notePadForm->findSameText->exception == TRUE) {

		CString str;
		str.Format("\"%s\"��(��) ã�� �� �����ϴ�.", (LPCTSTR)this->notePadForm->findSameText->findStr);
		MessageBox(str, "�޸���", MB_OK);
	}
#endif

	//1.7. ��ũ�� �̵����� Ȯ���Ѵ�. ******************************************************************
	this->notePadForm->isJumpOverForPrevious = FALSE;
	this->notePadForm->isJumpOverForNext = FALSE;
	this->notePadForm->isUp = FALSE;
	this->notePadForm->isDown = FALSE;
	this->notePadForm->isDoubleByte = FALSE;

	this->notePadForm->Notify(); // ���� ��ũ�� ���� �ۿ� �ܾ ���� ���
	this->notePadForm->Invalidate();
	//**************************************************************************************************
}



void ReplaceDialog::OnClose() {

	// �� ��ư�� ��ȭ�� �ɼ��� �����Ѵ�.
	this->notePadForm->findSameText->matchCase = ((CButton*)GetDlgItem(IDC_BUTTON_MATCHCASE))->GetCheck();
	this->notePadForm->findSameText->searchAround = ((CButton*)GetDlgItem(IDC_BUTTON_SEARCHAROUND))->GetCheck();

	//�ݱ��ư�� Ŭ������ ��, �����츦 �����Ѵ�.
	this->notePadForm->replaceDialog = NULL;
	EndDialog(0);
}

void ReplaceDialog::OnCancel() {

	// �� ��ư�� ��ȭ�� �ɼ��� �����Ѵ�.
	this->notePadForm->findSameText->matchCase = ((CButton*)GetDlgItem(IDC_BUTTON_MATCHCASE))->GetCheck();
	this->notePadForm->findSameText->searchAround = ((CButton*)GetDlgItem(IDC_BUTTON_SEARCHAROUND))->GetCheck();

	//�ݱ��ư�� Ŭ������ ��, �����츦 �����Ѵ�.
	this->notePadForm->replaceDialog = NULL;
	EndDialog(0);

}

BOOL ReplaceDialog::PreTranslateMessage(MSG* pMsg) {
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_ESCAPE) {

			// �� ��ư�� ��ȭ�� �ɼ��� �����Ѵ�.
			this->notePadForm->findSameText->matchCase = ((CButton*)GetDlgItem(IDC_BUTTON_MATCHCASE))->GetCheck();
			this->notePadForm->findSameText->searchAround = ((CButton*)GetDlgItem(IDC_BUTTON_SEARCHAROUND))->GetCheck();

			//�ݱ��ư�� Ŭ������ ��, �����츦 �����Ѵ�.
			this->notePadForm->replaceDialog = NULL;
			EndDialog(0);
			return TRUE;
		}
	}

	return 0;
}


