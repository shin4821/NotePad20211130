//OnUpCommand.cpp
#include"OnUpCommand.h"
#include"NotePadForm.h"
#include"KeyAction.h"
#include"FactoryProduct.h"

OnUpCommand::OnUpCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
}

OnUpCommand::~OnUpCommand() {
}

void OnUpCommand::Execute() {
	KeyAction* keyAction = NULL;
	FactoryProduct factoryProduct(this->notePadForm);
	UINT nChar = this->notePadForm->nChar;

	keyAction = factoryProduct.Make(nChar, NULL, NULL);
	keyAction->KeyDown();
}











