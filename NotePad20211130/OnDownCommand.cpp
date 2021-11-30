//OnDownCommand.cpp
#include"OnDownCommand.h"
#include"NotePadForm.h"
#include"KeyAction.h"
#include"FactoryProduct.h"

OnDownCommand::OnDownCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
}

OnDownCommand::~OnDownCommand() {
}

void OnDownCommand::Execute() {
	KeyAction* keyAction = NULL;
	FactoryProduct factoryProduct(this->notePadForm);
	UINT nChar = this->notePadForm->nChar;

	keyAction = factoryProduct.Make(nChar, NULL, NULL);
	keyAction->KeyDown();
}
