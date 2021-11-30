//OnPreviousCommand.cpp
#include"OnPreviousCommand.h"
#include"NotePadForm.h"
#include"KeyAction.h"
#include"FactoryProduct.h"


OnPreviousCommand::OnPreviousCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
}

OnPreviousCommand::~OnPreviousCommand() {
}

void OnPreviousCommand::Execute() {
	KeyAction* keyAction = NULL;
	FactoryProduct factoryProduct(this->notePadForm);
	UINT nChar = this->notePadForm->nChar;

	keyAction = factoryProduct.Make(nChar, NULL, NULL);
	keyAction->KeyDown();
}


