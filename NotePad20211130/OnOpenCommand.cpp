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

	//3. 열기버튼 클릭했을 때
	 //3.1. 현재 메모장의 타이틀을 구한다.
	this->notePadForm->GetWindowText(title);
	title.Replace("*", "");
	CString titleWithTxt;
	titleWithTxt.Format("%s%s", title, ".txt");


	//3.2. 해당 파일이 존재하고, 내용 변화가 있으면,
	CFileFind pFind;
	Glyph* row = this->notePadForm->note->GetChild(0);


	ret = pFind.FindFile(titleWithTxt);
	if (ret == TRUE && this->notePadForm->isChanged == TRUE) {
		//3.2.1. 메세지를 띄운다.
		str.Format("%s%s%s", "변경내용을 ", title, "에 저장하시겠습니까?");
		ret_ = SaveMessageBox(this->notePadForm->GetSafeHwnd(), str, "메모장", MB_YESNOCANCEL);
		//ret_ = MessageBox(str, "메모장", MB_YESNOCANCEL);

		//3.2.2. YES를 누를 경우, 저장한다.
		if (ret_ == IDYES) {
			this->notePadForm->note->Save((LPCTSTR)titleWithTxt);
		}
	}

	//3.3. 해당 파일이 존재하지 않고 글자가 하나라도 있으면,
	else if (ret == FALSE && (row->GetLength() > 0 || this->notePadForm->note->GetLength() > 1)) {
		//3.3.1. 메세지를 띄운다.
		str.Format("%s%s%s", "변경내용을 ", title, "에 저장하시겠습니까?");
		ret_ = SaveMessageBox(this->notePadForm->GetSafeHwnd(), str, "메모장", MB_YESNOCANCEL);

		//3.3.2. YES를 누를 경우,
		if (ret_ == IDYES) {
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

						this->notePadForm->note->Save((LPCTSTR)fileName);

						//입력한 이름으로 변경한다
						this->notePadForm->SetWindowText(fileNameWithouttxt);
					}
				}
				//2.4.4. 파일 이름이 중복되지 않으면,
				else {
					this->notePadForm->note->Save((LPCTSTR)fileName);
					//입력한 이름으로 변경한다.
					this->notePadForm->SetWindowText(fileNameWithouttxt);
				}
			}
			//2.5. 사용자가 파일 선택 안했다면 strPathName 초기화하고 저장X.
			else {
				strPathName = "";
			}
		}
	}


	//3.4. 필터를 만든다.
	LPCTSTR fileFilter_(_T("모든파일|*.*|텍스트문서(*.txt)|*.txt||"));

	//3.5. 공통대화상자(열기) 만든다.
	CFileDialog dlg_(TRUE, _T("txt"), _T("*.txt"), OFN_HIDEREADONLY, fileFilter_, NULL, 0, NULL);

	//3.6. 공통대화상자 띄운다.
	//3.7. 사용자가 열 파일을 정상적으로 선택 하였을 때
	if (IDOK == dlg_.DoModal()) {

		this->notePadForm->isOpen = TRUE;
		this->notePadForm->isOpenChanged = FALSE;

		//3.7.1. 선택한 파일의 이름을 갖고 온다.
		fileName = dlg_.GetFileName();
		fileNameWithouttxt = dlg_.GetFileTitle();

		//3.7.2. 현재 note를 삭제한다.
		if (this->notePadForm->note != NULL) {
			delete this->notePadForm->note;
		}
		//3.7.3. 새로운 노트를 만든다.
		this->notePadForm->note = new Note();
		//3.7.4. 새로운 줄을 만든다.
		this->notePadForm->row = new Row();
		//3.7.5. 노트에 줄을 Add한다.
		this->notePadForm->note->Add(this->notePadForm->row);

		//3.7.6. 해당 파일을 Load한다.
		this->notePadForm->note->Load((LPCTSTR)fileName);

		//(21.11.15.추가) 현재 자동개행이 눌려있을 경우, 자동개행을 시켜준다.
		if(this->notePadForm->isLineChangeButtonClicked == TRUE)
		this->notePadForm->lineChange->LineChangeButtonClicked();

		//3.7.7. 해당 파일의 캡션을 바꾼다.
		this->notePadForm->SetWindowText(fileNameWithouttxt);


		//1.5. UndoList, RedoList 초기화한다.
		this->notePadForm->commandStory->undoList->DeleteAll();
		this->notePadForm->commandStory->redoList->DeleteAll();


		//3.7.8. 글자 변화를 초기화한다.
		this->notePadForm->isChanged = FALSE;
	}
	this->notePadForm->Invalidate();

	//Caret과 Scroll을 업데이트 해준다. *************************************

	//Caret의 위치를 초기화한다.
	this->notePadForm->note->MoveCurrent(0);
	row = this->notePadForm->note->GetChild(0);
	row->MoveCurrent(0);

    // Scroll 업데이트 할 때의 조건을 설정한다.
	this->notePadForm->isJumpOverForPrevious = false;
	this->notePadForm->isJumpOverForNext = false;
	this->notePadForm->isUp = false;
	this->notePadForm->isDown = false;
	this->notePadForm->isDoubleByte = false;


	this->notePadForm->Notify();
	//***********************************************************************

}
