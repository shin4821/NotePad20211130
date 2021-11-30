//WindowText.cpp
#include"WindowText.h"
#include"NotePadForm.h"
#include"Note.h"

WindowText::WindowText(NotePadForm* notePadForm) {
	this->notePadForm = notePadForm;
}

WindowText::~WindowText() {

}

void WindowText::UpdateWindowText() {

	CString title;
	int rowLength;
	int noteLength;
	Glyph* testRow;

	this->notePadForm->GetWindowText(title);

	//(21.11.10.추가) 파일을 열지 않았을 경우, 편집여부를 알려주는 이펙트를 추가한다.
	if (this->notePadForm->isOpen == false) {
		
		if (title.Find("*") == -1) {
			title.Insert(0, "*");
			this->notePadForm->SetWindowText(title);
		}


		//현재 노트의 length가 0인 경우, 편집 이펙트를 지운다.
		noteLength = this->notePadForm->note->GetLength();
		if (noteLength <= 1) {

			testRow = this->notePadForm->note->GetChild(0);
			rowLength = testRow->GetLength();

			if (rowLength <= 0) {
				if (title.Find("*") == 0) {
					title.Replace("*", "");
					this->notePadForm->SetWindowText(title);
				}
			}
		}
	}
	//(21.11.10.추가) 파일을 열었을 경우, 편집여부를 알려주는 이펙트를 추가한다.
	else if(this->notePadForm->isOpenChanged == true) {
		
		if (title.Find("*") == -1) {
			title.Insert(0, "*");
			this->notePadForm->SetWindowText(title);
		}
	}




}

void WindowText::Update() {

	UpdateWindowText();

}








