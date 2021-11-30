//OnNextCommand.cpp
#include"OnNextCommand.h"
#include"NotePadForm.h"
#include"KeyAction.h"
#include"FactoryProduct.h"


OnNextCommand::OnNextCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
}

OnNextCommand::~OnNextCommand() {
}

void OnNextCommand::Execute() {

	KeyAction* keyAction = NULL;
	FactoryProduct factoryProduct(this->notePadForm);
	UINT nChar = this->notePadForm->nChar;

	keyAction = factoryProduct.Make(nChar, NULL, NULL);
	keyAction->KeyDown();
}

