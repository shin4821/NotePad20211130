//OnNewCreateCommand.cpp
#include"OnNewCreateCommand.h"
#include"NotePadForm.h"
#include"Note.h"
#include"Row.h"
#include"UndoList.h"
#include"RedoList.h"
#include"CommandStory.h"
#include"SelectWords.h"
#include"LineChange.h"
#include<afxdlgs.h> //CFileDialog


OnNewCreateCommand::OnNewCreateCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
}

OnNewCreateCommand::~OnNewCreateCommand() {
}

void OnNewCreateCommand::Execute() {

	CString str;
	BOOL ret;
	int ret_;
	CString title;
	CString title_;
	CString strPathName;
	CString fileName;
	CString fileNameWithouttxt;
	Glyph* row = NULL;
	BOOL isClose = TRUE;
	BOOL isSave = FALSE;


	//1. '���� �����'�� ������ ���,

	//1.6. ���� length�� 0���� Ŭ ���, ���� ���θ� �����.*****************************
	//(21.11.10.�߰�) ���� ��Ʈ�� length�� 0�� ���, ���� ����Ʈ�� �����.
	int noteLength = this->notePadForm->note->GetLength();
	
	if (noteLength > 1) {
		isSave = TRUE;
	}
	else if (noteLength <= 1) {
		Glyph* testRow = this->notePadForm->note->GetChild(0);
		int rowLength = testRow->GetLength();

		if (rowLength > 0) {
			isSave = TRUE;
		}
	}

	if (isSave == TRUE) {

		//3.1. ���� �޸����� Ÿ��Ʋ�� ���Ѵ�.
		this->notePadForm->GetWindowText(title);

		// title�� *�� ���ԵǾ������� �����Ѵ�.
		if (title.Find("*") == 0) {
			title.Replace("*", "");
		}

		CString titleWithTxt;
		titleWithTxt.Format("%s%s", title, ".txt");

		//3.3.1. �޼����� ����.
		str.Format("%s%s%s", "���泻���� ", title, "�� �����Ͻðڽ��ϱ�?");
		//ret_ = MessageBox(str, "�޸���", MB_YESNOCANCEL);//MessageBox(str, "�޸���", MB_YESNOCANCEL);
		ret_ = SaveMessageBox(this->notePadForm->GetSafeHwnd(), str, "�޸���", MB_YESNOCANCEL); //MessageBox(str, "�޸���", MB_YESNOCANCEL);

		//3.3.2. YES�� ���� ���,
		if (ret_ == IDYES) {

			CFileFind pFind;
			BOOL isThereFile = pFind.FindFile(titleWithTxt);

			//1.2. �ش� �̸��� ������ �����, ������ ���Ÿ� �Ѵ�.(�����ȭ����x)
			if (isThereFile == TRUE) {
				//(21.11.01.�߰�) �ڵ������� �Ǿ������� �ڵ������� Ǯ���ش�.
				if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
					this->notePadForm->lineChange->LineChangeButtonNotClicked();
				}

				//(21.11.22.�߰�) ���� ��Ƽ ǥ�ð� ������ �����ش�.
				if (title.Find("*") == 0) {
					title.Replace("*", "");
					this->notePadForm->SetWindowText(title);
				}

				this->notePadForm->note->Save((LPCTSTR)titleWithTxt);
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
					fileNameWithouttxt = dlg.GetFileTitle();

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

							//�����Ѵ�.
							this->notePadForm->note->Save((LPCTSTR)fileName);
							//�Է��� �̸����� �����Ѵ�
							this->notePadForm->SetWindowText(fileNameWithouttxt);

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

						//�����Ѵ�.
						this->notePadForm->note->Save((LPCTSTR)fileName);
						//�Է��� �̸����� �����Ѵ�
						this->notePadForm->SetWindowText(fileNameWithouttxt);

						//(21.11.01.�߰�) �ڵ������� �Ǿ������� �ٽ� �ڵ����� ���ش�..
						if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
							this->notePadForm->lineChange->LineChangeButtonClicked();
						}
					}
				}
				//2.5. ����ڰ� ���� ���� ���ߴٸ� strPathName �ʱ�ȭ�ϰ� ����X.
				else {
					strPathName = "";
				}

			}
		}
		else if (ret_ == IDCANCEL) {
			isClose = FALSE;
		}
	}
	//*************************************************************************************
//#endif


	if(isClose == TRUE){

		this->notePadForm->isOpen = FALSE;
		this->notePadForm->isOpenChanged = FALSE;

		//1. ���õ� ������ �ʱ�ȭ�Ѵ�.
		this->notePadForm->selectWords->firstX = 0;
		this->notePadForm->selectWords->firstY = 0;
		this->notePadForm->selectWords->endX = 0;
		this->notePadForm->selectWords->endY = 0;


		//1.1. note�� �����.
		if (this->notePadForm->note != 0) {
			delete this->notePadForm->note;
			this->notePadForm->note = 0;
		}

		//1.2. note�� ���� �����.
		this->notePadForm->note = new Note();

		//1.3. row�� ���� �����,
		this->notePadForm->row = new Row();
		this->notePadForm->note->Add(this->notePadForm->row);

		//1.4. ĳ���� ��ġ�� �ʱ�ȭ�Ѵ�.
		this->notePadForm->note->MoveCurrent(0);
		this->notePadForm->row->MoveCurrent(0);

		//1.5. UndoList, RedoList �ʱ�ȭ�Ѵ�.
		this->notePadForm->commandStory->undoList->DeleteAll();
		this->notePadForm->commandStory->redoList->DeleteAll();

		//1.6. "�׽�Ʈ��"���� ���ϸ��� �����Ѵ�
		this->notePadForm->SetWindowText("�׽�Ʈ��");


		this->notePadForm->Notify();
	
	}
}

