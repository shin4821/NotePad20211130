//OnFindDialogCommand.cpp
#include"OnFindDialogCommand.h"
#include"NotePadForm.h"
#include"FindDialog.h"

OnFindDialogCommand::OnFindDialogCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
}

OnFindDialogCommand::~OnFindDialogCommand() {
}

void OnFindDialogCommand::Execute() {

	if (this->notePadForm->findDialog == NULL) {
		this->notePadForm->findDialog = new FindDialog(this->notePadForm);
	}
	this->notePadForm->findDialog->Create(NULL, FR_DOWN, this->notePadForm);


}






