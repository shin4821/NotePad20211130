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

	//3. 열기버튼 클릭했을 때
	 //3.1. 현재 메모장의 타이틀을 구한다.
	this->notePadForm->GetWindowText(title);
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
		//ret_ = MessageBox(str, "메모장", MB_YESNOCANCEL);//MessageBox(str, "메모장", MB_YESNOCANCEL);
		ret_ = SaveMessageBox(this->notePadForm->GetSafeHwnd(), str, "메모장", MB_YESNOCANCEL); //MessageBox(str, "메모장", MB_YESNOCANCEL);

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

