//OnPreviousWordCommand.cpp
#include"OnPreviousWordCommand.h"
#include"NotePadForm.h"
#include"KeyAction.h"
#include"FactoryProduct.h"

OnPreviousWordCommand::OnPreviousWordCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
}

OnPreviousWordCommand::~OnPreviousWordCommand() {
}

void OnPreviousWordCommand::Execute() {

	KeyAction* keyAction = NULL;
	FactoryProduct factoryProduct(this->notePadForm);
	UINT nChar = this->notePadForm->nChar;

	keyAction = factoryProduct.Make(nChar, VK_CONTROL, NULL);
	keyAction->KeyDown();
}

