//OnPreviewCommand.cpp
#include"OnPreviewCommand.h"
#include"NotePadForm.h"
#include"Preview.h"
#include"Scroll.h"


OnPreviewCommand::OnPreviewCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
}

OnPreviewCommand::~OnPreviewCommand() {
}

void OnPreviewCommand::Execute() {

	int width = this->notePadForm->scroll->windowWidth;
	int height = this->notePadForm->scroll->windowHeight;

	//1. �̸����� �����������츦 ����. (��ȭ���� �ƴ϶�)
	this->notePadForm->preview = new Preview(this->notePadForm);
	//Preview* preview = new Preview(this->notePadForm);
	

	this->notePadForm->preview->Create(NULL, "�̸�����");
	this->notePadForm->preview->ShowWindow(SW_SHOW);
	this->notePadForm->preview->UpdateWindow();

}





