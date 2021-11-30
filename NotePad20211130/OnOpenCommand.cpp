//OnOpenCommand.cpp
#include"OnOpenCommand.h"
#include"NotePadForm.h"
#include"Note.h"
#include"Row.h"
#include"CommandStory.h"
#include"UndoList.h"
#include"RedoList.h"
#include"LineChange.h"
#include<afxdlgs.h> //CFileDialog

OnOpenCommand::OnOpenCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
}

OnOpenCommand::~OnOpenCommand() {

}

void OnOpenCommand::Execute() {

	CString str;
	BOOL ret;
	BOOL ret_;
	CString title;
	CString title_;
	CString strPathName;
	CString fileName;
	CString fileNameWithouttxt;

	//3. �����ư Ŭ������ ��
	 //3.1. ���� �޸����� Ÿ��Ʋ�� ���Ѵ�.
	this->notePadForm->GetWindowText(title);
	title.Replace("*", "");
	CString titleWithTxt;
	titleWithTxt.Format("%s%s", title, ".txt");


	//3.2. �ش� ������ �����ϰ�, ���� ��ȭ�� ������,
	CFileFind pFind;
	Glyph* row = this->notePadForm->note->GetChild(0);


	ret = pFind.FindFile(titleWithTxt);
	if (ret == TRUE && this->notePadForm->isChanged == TRUE) {
		//3.2.1. �޼����� ����.
		str.Format("%s%s%s", "���泻���� ", title, "�� �����Ͻðڽ��ϱ�?");
		ret_ = SaveMessageBox(this->notePadForm->GetSafeHwnd(), str, "�޸���", MB_YESNOCANCEL);
		//ret_ = MessageBox(str, "�޸���", MB_YESNOCANCEL);

		//3.2.2. YES�� ���� ���, �����Ѵ�.
		if (ret_ == IDYES) {
			this->notePadForm->note->Save((LPCTSTR)titleWithTxt);
		}
	}

	//3.3. �ش� ������ �������� �ʰ� ���ڰ� �ϳ��� ������,
	else if (ret == FALSE && (row->GetLength() > 0 || this->notePadForm->note->GetLength() > 1)) {
		//3.3.1. �޼����� ����.
		str.Format("%s%s%s", "���泻���� ", title, "�� �����Ͻðڽ��ϱ�?");
		ret_ = SaveMessageBox(this->notePadForm->GetSafeHwnd(), str, "�޸���", MB_YESNOCANCEL);

		//3.3.2. YES�� ���� ���,
		if (ret_ == IDYES) {
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
				fileNameWithouttxt = dlg.GetFileTitle();

				//MessageBox(title);

				//2.4.3. ���� �̸��� �ߺ��Ǹ� 
				if (PathFileExists(fileName)) {
					str.Format("%s%s%s", fileName, "��(��) �̹� �ֽ��ϴ�.\n", "�ٲٽðڽ��ϱ�?");
					ret = this->notePadForm->MessageBox(str, _T("�ٸ� �̸����� ���� Ȯ��"), MB_YESNO);

					if (ret == IDYES) {// Yes ������ ���, �ش� ���Ͽ� ������Ѵ�.
						//������ �ִ� �ش� �̸��� ������ �����Ѵ�.
						//remove(fileName);

						this->notePadForm->note->Save((LPCTSTR)fileName);

						//�Է��� �̸����� �����Ѵ�
						this->notePadForm->SetWindowText(fileNameWithouttxt);
					}
				}
				//2.4.4. ���� �̸��� �ߺ����� ������,
				else {
					this->notePadForm->note->Save((LPCTSTR)fileName);
					//�Է��� �̸����� �����Ѵ�.
					this->notePadForm->SetWindowText(fileNameWithouttxt);
				}
			}
			//2.5. ����ڰ� ���� ���� ���ߴٸ� strPathName �ʱ�ȭ�ϰ� ����X.
			else {
				strPathName = "";
			}
		}
	}


	//3.4. ���͸� �����.
	LPCTSTR fileFilter_(_T("�������|*.*|�ؽ�Ʈ����(*.txt)|*.txt||"));

	//3.5. �����ȭ����(����) �����.
	CFileDialog dlg_(TRUE, _T("txt"), _T("*.txt"), OFN_HIDEREADONLY, fileFilter_, NULL, 0, NULL);

	//3.6. �����ȭ���� ����.
	//3.7. ����ڰ� �� ������ ���������� ���� �Ͽ��� ��
	if (IDOK == dlg_.DoModal()) {

		this->notePadForm->isOpen = TRUE;
		this->notePadForm->isOpenChanged = FALSE;

		//3.7.1. ������ ������ �̸��� ���� �´�.
		fileName = dlg_.GetFileName();
		fileNameWithouttxt = dlg_.GetFileTitle();

		//3.7.2. ���� note�� �����Ѵ�.
		if (this->notePadForm->note != NULL) {
			delete this->notePadForm->note;
		}
		//3.7.3. ���ο� ��Ʈ�� �����.
		this->notePadForm->note = new Note();
		//3.7.4. ���ο� ���� �����.
		this->notePadForm->row = new Row();
		//3.7.5. ��Ʈ�� ���� Add�Ѵ�.
		this->notePadForm->note->Add(this->notePadForm->row);

		//3.7.6. �ش� ������ Load�Ѵ�.
		this->notePadForm->note->Load((LPCTSTR)fileName);

		//(21.11.15.�߰�) ���� �ڵ������� �������� ���, �ڵ������� �����ش�.
		if(this->notePadForm->isLineChangeButtonClicked == TRUE)
		this->notePadForm->lineChange->LineChangeButtonClicked();

		//3.7.7. �ش� ������ ĸ���� �ٲ۴�.
		this->notePadForm->SetWindowText(fileNameWithouttxt);


		//1.5. UndoList, RedoList �ʱ�ȭ�Ѵ�.
		this->notePadForm->commandStory->undoList->DeleteAll();
		this->notePadForm->commandStory->redoList->DeleteAll();


		//3.7.8. ���� ��ȭ�� �ʱ�ȭ�Ѵ�.
		this->notePadForm->isChanged = FALSE;
	}
	this->notePadForm->Invalidate();

	//Caret�� Scroll�� ������Ʈ ���ش�. *************************************

	//Caret�� ��ġ�� �ʱ�ȭ�Ѵ�.
	this->notePadForm->note->MoveCurrent(0);
	row = this->notePadForm->note->GetChild(0);
	row->MoveCurrent(0);

    // Scroll ������Ʈ �� ���� ������ �����Ѵ�.
	this->notePadForm->isJumpOverForPrevious = false;
	this->notePadForm->isJumpOverForNext = false;
	this->notePadForm->isUp = false;
	this->notePadForm->isDown = false;
	this->notePadForm->isDoubleByte = false;


	this->notePadForm->Notify();
	//***********************************************************************

}
