//OnReplaceDialogCommand.cpp
#include"OnReplaceDialogCommand.h"
#include"NotePadForm.h"
#include"ReplaceDialog.h"

OnReplaceDialogCommand::OnReplaceDialogCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
	this->isChecked = false;
	this->notePadForm->isUndo = false;
	this->notePadForm->isThereSelected = false;
	this->firstX = 0;
	this->firstY = 0;
	this->endX = 0;
	this->endY = 0;
}

OnReplaceDialogCommand::~OnReplaceDialogCommand() {
}

void OnReplaceDialogCommand::Execute() {
	if (this->notePadForm->replaceDialog == NULL) {
		this->notePadForm->replaceDialog = new ReplaceDialog(this->notePadForm);
	}
	this->notePadForm->replaceDialog->Create(NULL, FR_DOWN, this->notePadForm);
}

void OnReplaceDialogCommand::Undo() {

}

void OnReplaceDialogCommand::Redo() {

}

Long OnReplaceDialogCommand::GetCurrentX() {
	return this->currentX;
}

Long OnReplaceDialogCommand::GetCurrentY() {
	return this->currentY;
}

string OnReplaceDialogCommand::GetCharacterForAll() {
	return this->character;
}

void OnReplaceDialogCommand::SetCheck() {
	this->isChecked = true;
}

bool OnReplaceDialogCommand::GetChecked() {
	return this->isChecked;
}







