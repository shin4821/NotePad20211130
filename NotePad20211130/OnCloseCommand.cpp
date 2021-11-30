//OnCloseButtonClickedCommand.cpp 
#include"OnCloseCommand.h"
#include"NotePadForm.h"
#include"Note.h"
#include"Row.h"
#include"Subject.h"
#include"Observer.h"
#include"Scroll.h"
#include<afxdlgs.h> //CFileDialog

OnCloseCommand::OnCloseCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
}

OnCloseCommand::~OnCloseCommand() {
}

void OnCloseCommand::Execute() {
	
	this->notePadForm->PostMessage(WM_CLOSE);



#if 0
	CString str;
	BOOL ret;
	int ret_;
	CString title;
	CString title_;
	CString strPathName;
	CString fileName;
	CString fileNameWithouttxt;

	//3. �����ư Ŭ������ ��
	 //3.1. ���� �޸����� Ÿ��Ʋ�� ���Ѵ�.
	this->notePadForm->GetWindowText(title);
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
		//ret_ = MessageBox(str, "�޸���", MB_YESNOCANCEL);//MessageBox(str, "�޸���", MB_YESNOCANCEL);
		ret_ = SaveMessageBox(this->notePadForm->GetSafeHwnd(), str, "�޸���", MB_YESNOCANCEL); //MessageBox(str, "�޸���", MB_YESNOCANCEL);

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


	if (this->notePadForm->note != NULL) {
		delete this->notePadForm->note;
		this->notePadForm->note = NULL;
	}
	if (this->notePadForm->getTextSize != NULL) {
		delete this->notePadForm->getTextSize;
		this->notePadForm->getTextSize = NULL;

	}
	if (this->notePadForm->font != NULL) {
		delete this->notePadForm->font;
		this->notePadForm->font = NULL;

	}
	if (this->notePadForm->thumb != NULL) {
		delete this->notePadForm->thumb;
		this->notePadForm->thumb = NULL;
	}
	if (this->notePadForm->scroll != NULL) {
		dynamic_cast<Subject*>(this)->Detach(this->notePadForm->scroll);
		delete this->notePadForm->scroll;
		this->notePadForm->scroll = NULL;
	}

	if (this->notePadForm->findDialog != NULL) {
		delete this->notePadForm->findDialog;
		this->notePadForm->findDialog = NULL;
	}
#if 0
	if (this->factoryProductScroll != NULL) {
		delete this->factoryProductScroll;
		this->factoryProductScroll = NULL;

	}
#endif
	if (this->notePadForm->commandStory != NULL) {
		delete this->notePadForm->commandStory;
		this->notePadForm->commandStory = NULL;
	}

	//this->notePadForm->CFrameWnd::OnClose();
#endif
}

