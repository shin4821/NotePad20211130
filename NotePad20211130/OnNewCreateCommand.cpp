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


	//1. '새로 만들기'를 눌렀을 경우,

	//1.6. 현재 length가 0보다 클 경우, 저장 여부를 물어본다.*****************************
	//(21.11.10.추가) 현재 노트의 length가 0인 경우, 편집 이펙트를 지운다.
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

		//3.1. 현재 메모장의 타이틀을 구한다.
		this->notePadForm->GetWindowText(title);

		// title에 *가 포함되어있으면 제거한다.
		if (title.Find("*") == 0) {
			title.Replace("*", "");
		}

		CString titleWithTxt;
		titleWithTxt.Format("%s%s", title, ".txt");

		//3.3.1. 메세지를 띄운다.
		str.Format("%s%s%s", "변경내용을 ", title, "에 저장하시겠습니까?");
		//ret_ = MessageBox(str, "메모장", MB_YESNOCANCEL);//MessageBox(str, "메모장", MB_YESNOCANCEL);
		ret_ = SaveMessageBox(this->notePadForm->GetSafeHwnd(), str, "메모장", MB_YESNOCANCEL); //MessageBox(str, "메모장", MB_YESNOCANCEL);

		//3.3.2. YES를 누를 경우,
		if (ret_ == IDYES) {

			CFileFind pFind;
			BOOL isThereFile = pFind.FindFile(titleWithTxt);

			//1.2. 해당 이름의 파일을 열어보고, 있으면 갱신만 한다.(공통대화상자x)
			if (isThereFile == TRUE) {
				//(21.11.01.추가) 자동개행이 되어있으면 자동개행을 풀어준다.
				if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
					this->notePadForm->lineChange->LineChangeButtonNotClicked();
				}

				//(21.11.22.추가) 현재 더티 표시가 있으면 없애준다.
				if (title.Find("*") == 0) {
					title.Replace("*", "");
					this->notePadForm->SetWindowText(title);
				}

				this->notePadForm->note->Save((LPCTSTR)titleWithTxt);
				this->notePadForm->isChanged = FALSE;

				//(21.11.01.추가) 자동개행이 되어있으면 다시 자동개행 해준다..
				if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
					this->notePadForm->lineChange->LineChangeButtonClicked();
				}
			}
			//2. 현재 파일의 제목이 없으면,
			else {
				//2.1. 필터를 만든다.
				LPCTSTR fileFilter(_T("모든파일|*.*|텍스트문서(*.txt)|*.txt||"));

				//2.2. 공통대화상자(다른이름으로 저장)를 만든다.
				CFileDialog dlg(FALSE, _T("txt"), _T("*.txt"), OFN_HIDEREADONLY, fileFilter, NULL, 0, NULL);
				//OFN_HIDEREADONLY 한 개의 파일 선택할 경우

				//2.3. 공통대화상자(다른이름으로 저장)를 띄운다.
				//2.4. 사용자가 저장할 파일을 정상적으로 선택하였을때, 
				if (IDOK == dlg.DoModal()) {
					//2.4.1. strPathName에 지정할 파일의 경로 저장한다.
					strPathName = dlg.GetPathName();

					//2.4.2. 선택한 파일의 이름을 갖고 온다.
					fileName = dlg.GetFileName();//선택된 파일의 이름과 확장자
					fileNameWithouttxt = dlg.GetFileTitle();

					//MessageBox(title);

					//2.4.3. 파일 이름이 중복되면 
					if (PathFileExists(fileName)) {
						str.Format("%s%s%s", fileName, "이(가) 이미 있습니다.\n", "바꾸시겠습니까?");
						ret = this->notePadForm->MessageBox(str, _T("다른 이름으로 저장 확인"), MB_YESNO);

						if (ret == IDYES) {// Yes 눌렀을 경우, 해당 파일에 덮어쓰기한다.
							//기존에 있던 해당 이름의 파일을 삭제한다.
							//remove(fileName);

							//(21.11.01.추가) 자동개행이 되어있으면 자동개행을 풀어준다.
							if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
								this->notePadForm->lineChange->LineChangeButtonNotClicked();
							}

							//저장한다.
							this->notePadForm->note->Save((LPCTSTR)fileName);
							//입력한 이름으로 변경한다
							this->notePadForm->SetWindowText(fileNameWithouttxt);

							//(21.11.01.추가) 자동개행이 되어있으면 다시 자동개행 해준다..
							if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
								this->notePadForm->lineChange->LineChangeButtonClicked();
							}
						}
					}
					//2.4.4. 파일 이름이 중복되지 않으면,
					else {
						//(21.11.01.추가) 자동개행이 되어있으면 자동개행을 풀어준다.
						if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
							this->notePadForm->lineChange->LineChangeButtonNotClicked();
						}

						//저장한다.
						this->notePadForm->note->Save((LPCTSTR)fileName);
						//입력한 이름으로 변경한다
						this->notePadForm->SetWindowText(fileNameWithouttxt);

						//(21.11.01.추가) 자동개행이 되어있으면 다시 자동개행 해준다..
						if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
							this->notePadForm->lineChange->LineChangeButtonClicked();
						}
					}
				}
				//2.5. 사용자가 파일 선택 안했다면 strPathName 초기화하고 저장X.
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

		//1. 선택된 영역을 초기화한다.
		this->notePadForm->selectWords->firstX = 0;
		this->notePadForm->selectWords->firstY = 0;
		this->notePadForm->selectWords->endX = 0;
		this->notePadForm->selectWords->endY = 0;


		//1.1. note를 지운다.
		if (this->notePadForm->note != 0) {
			delete this->notePadForm->note;
			this->notePadForm->note = 0;
		}

		//1.2. note를 새로 만든다.
		this->notePadForm->note = new Note();

		//1.3. row를 새로 만든다,
		this->notePadForm->row = new Row();
		this->notePadForm->note->Add(this->notePadForm->row);

		//1.4. 캐럿의 위치를 초기화한다.
		this->notePadForm->note->MoveCurrent(0);
		this->notePadForm->row->MoveCurrent(0);

		//1.5. UndoList, RedoList 초기화한다.
		this->notePadForm->commandStory->undoList->DeleteAll();
		this->notePadForm->commandStory->redoList->DeleteAll();

		//1.6. "테스트용"으로 파일명을 변경한다
		this->notePadForm->SetWindowText("테스트용");


		this->notePadForm->Notify();
	
	}
}

