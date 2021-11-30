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

	//2. 저장 버튼을 클릭했을 때
	//1.1.현재 메모장의 이름을 확인한다. 
	this->notePadForm->GetWindowText(title);
	CString title_;
	title_.Format("%s%s", title, ".txt");
	title_.Replace("*", "");

	CFileFind pFind;
	BOOL ret_ = pFind.FindFile(title_);

	// 현재 currentX, currentY  구한다.
	Long currentY = this->notePadForm->note->GetCurrent();
	this->notePadForm->row = this->notePadForm->note->GetChild(currentY);
	Long currentX = this->notePadForm->row->GetCurrent();


	//1.2. 해당 이름의 파일을 열어보고, 있으면 갱신만 한다.(공통대화상자x)
	if (ret_ == TRUE) {

		//(21.11.01.추가) 자동개행이 되어있으면 자동개행을 풀어준다.
		if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
			this->notePadForm->lineChange->LineChangeButtonNotClicked();
		}

	    //(21.11.22.추가) 현재 더티 표시가 있으면 없애준다.
		if (title.Find("*") == 0) {
			title.Replace("*", "");
			this->notePadForm->SetWindowText(title);
		}

		this->notePadForm->note->Save((LPCTSTR)title_);
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
			fileNameWithoutTxt = dlg.GetFileTitle();

			fileName_ = (LPCTSTR)fileName;
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

					this->notePadForm->note->Save(fileName_);
					//입력한 이름으로 변경한다.
					this->notePadForm->SetWindowText(fileNameWithoutTxt);
					//rename("NotePad.txt", fileName);


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

				this->notePadForm->note->Save(fileName_);
				//입력한 이름으로 변경한다.
				this->notePadForm->SetWindowText(fileNameWithoutTxt);
				//rename("NotePad.txt", fileName); //fileName

				//(21.11.01.추가) 자동개행이 되어있으면 다시 자동개행 해준다..
				if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
					this->notePadForm->lineChange->LineChangeButtonClicked();
				}
			}

			this->notePadForm->isChanged = FALSE;


		}
		//2.5. 사용자가 파일 선택 안했다면 strPathName 초기화하고 저장X.
		else {
			strPathName = "";
		}
	}

	//자동개행인 경우, 저장 전 캐럿으로 이동시켜준다.
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


