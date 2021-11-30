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

	//(21.11.10.�߰�) ������ ���� �ʾ��� ���, �������θ� �˷��ִ� ����Ʈ�� �߰��Ѵ�.
	if (this->notePadForm->isOpen == false) {
		
		if (title.Find("*") == -1) {
			title.Insert(0, "*");
			this->notePadForm->SetWindowText(title);
		}


		//���� ��Ʈ�� length�� 0�� ���, ���� ����Ʈ�� �����.
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
	//(21.11.10.�߰�) ������ ������ ���, �������θ� �˷��ִ� ����Ʈ�� �߰��Ѵ�.
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








