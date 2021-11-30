//OnSaveCommand.cpp 
#include"OnSaveCommand.h"
#include"NotePadForm.h"
#include"Note.h"
#include"LineChange.h"
#include<afxdlgs.h> //CFileDialog

OnSaveCommand::OnSaveCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
}

OnSaveCommand::~OnSaveCommand() {

}

void OnSaveCommand::Execute() {

	FILE* file = 0;
	CString strPathName;
	CString fileName;
	string fileName_;
	CString str;
	int ret;
	CString title;
	CString fileNameWithoutTxt;

	//2. ���� ��ư�� Ŭ������ ��
	//1.1.���� �޸����� �̸��� Ȯ���Ѵ�. 
	this->notePadForm->GetWindowText(title);
	CString title_;
	title_.Format("%s%s", title, ".txt");
	title_.Replace("*", "");

	CFileFind pFind;
	BOOL ret_ = pFind.FindFile(title_);

	// ���� currentX, currentY  ���Ѵ�.
	Long currentY = this->notePadForm->note->GetCurrent();
	this->notePadForm->row = this->notePadForm->note->GetChild(currentY);
	Long currentX = this->notePadForm->row->GetCurrent();


	//1.2. �ش� �̸��� ������ �����, ������ ���Ÿ� �Ѵ�.(�����ȭ����x)
	if (ret_ == TRUE) {

		//(21.11.01.�߰�) �ڵ������� �Ǿ������� �ڵ������� Ǯ���ش�.
		if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
			this->notePadForm->lineChange->LineChangeButtonNotClicked();
		}

	    //(21.11.22.�߰�) ���� ��Ƽ ǥ�ð� ������ �����ش�.
		if (title.Find("*") == 0) {
			title.Replace("*", "");
			this->notePadForm->SetWindowText(title);
		}

		this->notePadForm->note->Save((LPCTSTR)title_);
		this->notePadForm->isChanged = FALSE;

		//(21.11.01.�߰�) �ڵ������� �Ǿ������� �ٽ� �ڵ����� ���ش�..
		if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
			this->notePadForm->lineChange->LineChangeButtonClicked();
		}

	}

	//2. ���� ������ ������ ������,
	else {
		//2.1. ���͸� �����.
		LPCTSTR fileFilter(_T("�������|*.*|�ؽ�Ʈ����(*.txt)|*.txt||"));

		//2.2. �����ȭ����(�ٸ��̸����� ����)�� �����.
		CFileDialog dlg(FALSE, _T("txt"), _T("*.txt"), OFN_HIDEREADONLY, fileFilter, NULL, 0, NULL);
		//OFN_HIDEREADONLY �� ���� ���� ������ ���

		//2.3. �����ȭ����(�ٸ��̸����� ����)�� ����.
		//2.4. ����ڰ� ������ ������ ���������� �����Ͽ�����, 
		if (IDOK == dlg.DoModal()) {


			//2.4.1. strPathName�� ������ ������ ��� �����Ѵ�.
			strPathName = dlg.GetPathName();

			//2.4.2. ������ ������ �̸��� ���� �´�.
			fileName = dlg.GetFileName();//���õ� ������ �̸��� Ȯ����
			fileNameWithoutTxt = dlg.GetFileTitle();

			fileName_ = (LPCTSTR)fileName;
			//MessageBox(title);

			//2.4.3. ���� �̸��� �ߺ��Ǹ� 
			if (PathFileExists(fileName)) {
				str.Format("%s%s%s", fileName, "��(��) �̹� �ֽ��ϴ�.\n", "�ٲٽðڽ��ϱ�?");
				ret = this->notePadForm->MessageBox(str, _T("�ٸ� �̸����� ���� Ȯ��"), MB_YESNO);

				if (ret == IDYES) {// Yes ������ ���, �ش� ���Ͽ� ������Ѵ�.
					//������ �ִ� �ش� �̸��� ������ �����Ѵ�.
					//remove(fileName);


					//(21.11.01.�߰�) �ڵ������� �Ǿ������� �ڵ������� Ǯ���ش�.
					if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
						this->notePadForm->lineChange->LineChangeButtonNotClicked();
					}

					this->notePadForm->note->Save(fileName_);
					//�Է��� �̸����� �����Ѵ�.
					this->notePadForm->SetWindowText(fileNameWithoutTxt);
					//rename("NotePad.txt", fileName);


					//(21.11.01.�߰�) �ڵ������� �Ǿ������� �ٽ� �ڵ����� ���ش�..
					if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
						this->notePadForm->lineChange->LineChangeButtonClicked();
					}

				}
			}
			//2.4.4. ���� �̸��� �ߺ����� ������,
			else {

				//(21.11.01.�߰�) �ڵ������� �Ǿ������� �ڵ������� Ǯ���ش�.
				if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
					this->notePadForm->lineChange->LineChangeButtonNotClicked();
				}

				this->notePadForm->note->Save(fileName_);
				//�Է��� �̸����� �����Ѵ�.
				this->notePadForm->SetWindowText(fileNameWithoutTxt);
				//rename("NotePad.txt", fileName); //fileName

				//(21.11.01.�߰�) �ڵ������� �Ǿ������� �ٽ� �ڵ����� ���ش�..
				if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
					this->notePadForm->lineChange->LineChangeButtonClicked();
				}
			}

			this->notePadForm->isChanged = FALSE;


		}
		//2.5. ����ڰ� ���� ���� ���ߴٸ� strPathName �ʱ�ȭ�ϰ� ����X.
		else {
			strPathName = "";
		}
	}

	//�ڵ������� ���, ���� �� ĳ������ �̵������ش�.
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
		this->notePadForm->note->MoveCurrent(currentY);
		this->notePadForm->row = this->notePadForm->note->GetChild(currentY);
		this->notePadForm->row->MoveCurrent(currentX);
	}


	this->notePadForm->isOpen = TRUE;
	this->notePadForm->isOpenChanged = FALSE;

	this->notePadForm->Invalidate();
	this->notePadForm->Notify();
}


