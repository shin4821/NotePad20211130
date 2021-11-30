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

//(21.09.01) �߰�
void FindDialog::OnMyCommand(UINT id) {
	FactoryProductCommand factoryProductCommand(this->notePadForm);
	Command* command = NULL;

	// �ش� command�� �����Ѵ�.
	command = factoryProductCommand.Make(id);

	//�ش� command�� �����Ѵ�.
	command->Execute();

#if 0
	//�ش� command�� undolist�� �״´�.
	//������� �Ǵ� ���ູ���� �ƴϸ�,
	if (id != 40040) {
		//������ command�� history list�� �߰��Ѵ�.
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

	//(21.09.15 �߰�) 1. ���� ���õ� �ܾ �ִ� ���, 
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

	ret = CFindReplaceDialog::Create(TRUE, (LPCTSTR)this->notePadForm->findSameText->findStr,
		(LPCTSTR)this->notePadForm->findSameText->findStr, dwFlags, pParentWnd);


	//1. FindSameText�� ��� �ɼǿ� ����, ��� �ɼ��� �������ش�.
    //1.1. ����ڰ� �˻� ���ڿ��� ��/�ҹ��ڸ� ��Ȯ�� ��ġ��Ű���� Ȯ���Ѵ�. (����:TRUE, �ȱ���:FALSE)

	if (this->notePadForm->findSameText->matchCase == TRUE) {
		((CButton*)GetDlgItem(IDC_BUTTON_MATCHCASE))->SetCheck(BST_CHECKED);
	}
	else {
		((CButton*)GetDlgItem(IDC_BUTTON_MATCHCASE))->SetCheck(BST_UNCHECKED);
	}


	//1.2. ����ڰ� ��ü �ܾ ��ġ��ų�� ���θ� Ȯ���Ѵ�.
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
	//1.0. ���� �ܾ ���س��´�.
	this->notePadForm->findSameText->beforeFindStr = this->notePadForm->findSameText->findStr;

	//1.1. ������ ã�� ������ Ȯ���Ѵ�.
	GetDlgItem(IDC_EDIT_FINDTEXT)->GetWindowText(this->notePadForm->findSameText->findStr);

	//1.2. ����ڰ� �˻� ���ڿ��� ��/�ҹ��ڸ� ��Ȯ�� ��ġ��Ű���� Ȯ���Ѵ�. (����:TRUE, �ȱ���:FALSE)
	this->notePadForm->findSameText->matchCase = ((CButton*)GetDlgItem(IDC_BUTTON_MATCHCASE))->GetCheck();

	//1.3. ����ڰ� �˻��� �Ʒ��� �������� �����ϴ��� Ȯ���Ѵ� (�Ʒ���:TRUE, ����:FALSE)
	this->notePadForm->findSameText->searchDown = ((CButton*)GetDlgItem(IDC_BUTTON_DOWN))->GetCheck();

	//1.4. ����ڰ� ��ü �ܾ ��ġ��ų�� ���θ� Ȯ���Ѵ�.
	this->notePadForm->findSameText->searchAround = ((CButton*)GetDlgItem(IDC_BUTTON_SEARCHAROUND))->GetCheck();


	//1.5. �˻��� �Ʒ��� �������� �����ϸ� "�����˻�"�� Ȱ��ȭ�Ѵ�,

	this->notePadForm->SendMessage(WM_COMMAND, 40040);

	//1.6. �˻��� ���� �������� �����ϸ� "�����˻�"�� Ȱ��ȭ�Ѵ�.
#endif


	//1. "���� ã��"��ư�� ������ ���,

	    //1.0. ���� �ܾ ���س��´�.
	    this->notePadForm->findSameText->beforeFindStr = this->notePadForm->findSameText->findStr;

        //1.1. ������ ã�� ������ Ȯ���Ѵ�.
	    GetDlgItem(IDC_EDIT_FINDTEXT)->GetWindowText(this->notePadForm->findSameText->findStr);

		//1.2. ����ڰ� �˻� ���ڿ��� ��/�ҹ��ڸ� ��Ȯ�� ��ġ��Ű���� Ȯ���Ѵ�. (����:TRUE, �ȱ���:FALSE)
		this->notePadForm->findSameText->matchCase = ((CButton*)GetDlgItem(IDC_BUTTON_MATCHCASE))->GetCheck();
		
		//1.3. ����ڰ� �˻��� �Ʒ��� �������� �����ϴ��� Ȯ���Ѵ� (�Ʒ���:TRUE, ����:FALSE)
		this->notePadForm->findSameText->searchDown = ((CButton*)GetDlgItem(IDC_BUTTON_DOWN))->GetCheck();

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

		//(21.11.04.�߰�) "����"�� �߰��������� isUp�� TRUE�� �����Ѵ�.
		if (this->notePadForm->findSameText->searchDown == FALSE) {
			this->notePadForm->isJumpOverForPrevious = FALSE;
			this->notePadForm->isJumpOverForNext = FALSE;
			//this->notePadForm->isUp = TRUE; //������ ��ġ�϶� �������� �ǹ����� ���� �ݴ�� ����
			//this->notePadForm->isDown = FALSE;
			this->notePadForm->isDoubleByte = FALSE;
		}
		else {
			//1.7. ��ũ�� �̵����� Ȯ���Ѵ�. ******************************************************************
			this->notePadForm->isJumpOverForPrevious = FALSE;
			this->notePadForm->isJumpOverForNext = FALSE;
			//this->notePadForm->isUp = FALSE; //������ ��ġ�϶� �ǹؿ��� ������ ���� �ݴ�� ����
			//this->notePadForm->isDown = FALSE;
			this->notePadForm->isDoubleByte = FALSE;
			//**************************************************************************************************
		}


	
	this->notePadForm->Notify(); // ���� ��ũ�� ���� �ۿ� �ܾ ���� ���
	this->notePadForm->Invalidate();

}

void FindDialog::OnFindTextChange() {

	//1. ���� �ܾ ���س��´�.
	this->notePadForm->findSameText->beforeFindStr = this->notePadForm->findSameText->findStr;

	//2. ������ ã�� ������ Ȯ���Ѵ�.
	GetDlgItem(IDC_EDIT_FINDTEXT)->GetWindowText(this->notePadForm->findSameText->findStr);

	//(21.09.15 �߰�) 3. ã�� ������ NULL�̸�, 
	if (this->notePadForm->findSameText->findStr == "\0") {
		//3.1. ã�� ��ư�� ��Ȱ��ȭ ��Ų��.
		GetDlgItem(IDC_BUTTON_FINDNEXT)->EnableWindow(FALSE);
	}
	else { //4. ã�� ������ NULL�� �ƴϸ�,
		//4.1. ã�� ��ư�� Ȱ��ȭ ��Ų��.
		GetDlgItem(IDC_BUTTON_FINDNEXT)->EnableWindow(TRUE);
	}

	this->notePadForm->findSameText->beforeFindStr = this->notePadForm->findSameText->findStr;
}


void FindDialog::OnClose() {

	// �� ��ư�� ��ȭ�� �ɼ��� �����Ѵ�.
	this->notePadForm->findSameText->matchCase = ((CButton*)GetDlgItem(IDC_BUTTON_MATCHCASE))->GetCheck();
	this->notePadForm->findSameText->searchAround = ((CButton*)GetDlgItem(IDC_BUTTON_SEARCHAROUND))->GetCheck();
	this->notePadForm->findSameText->searchDown = ((CButton*)GetDlgItem(IDC_BUTTON_DOWN))->GetCheck();

	//�ݱ��ư�� Ŭ������ ��, �����츦 �����Ѵ�.
	this->notePadForm->findDialog = NULL;
	EndDialog(0);
}

#if 0
void FindDialog::Oncancelmode() {
	// �� ��ư�� ��ȭ�� �ɼ��� �����Ѵ�.
	this->notePadForm->findSameText->matchCase = ((CButton*)GetDlgItem(IDC_BUTTON_MATCHCASE))->GetCheck();
	this->notePadForm->findSameText->searchAround = ((CButton*)GetDlgItem(IDC_BUTTON_SEARCHAROUND))->GetCheck();
	this->notePadForm->findSameText->searchDown = ((CButton*)GetDlgItem(IDC_BUTTON_DOWN))->GetCheck();

	//�ݱ��ư�� Ŭ������ ��, �����츦 �����Ѵ�.
	this->notePadForm->findDialog = NULL;
	EndDialog(0);
}
#endif

BOOL FindDialog::PreTranslateMessage(MSG* pMsg) {
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_ESCAPE) {

			// �� ��ư�� ��ȭ�� �ɼ��� �����Ѵ�.
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
	// �� ��ư�� ��ȭ�� �ɼ��� �����Ѵ�.
	this->notePadForm->findSameText->matchCase = ((CButton*)GetDlgItem(IDC_BUTTON_MATCHCASE))->GetCheck();
	this->notePadForm->findSameText->searchAround = ((CButton*)GetDlgItem(IDC_BUTTON_SEARCHAROUND))->GetCheck();
	this->notePadForm->findSameText->searchDown = ((CButton*)GetDlgItem(IDC_BUTTON_DOWN))->GetCheck();

	//�ݱ��ư�� Ŭ������ ��, �����츦 �����Ѵ�.
	this->notePadForm->findDialog = NULL;
	EndDialog(0);


}







