//OnEndDocCommand.cpp
#include"OnEndDocCommand.h"
#include"NotePadForm.h"
#include"KeyAction.h"
#include"FactoryProduct.h"

OnEndDocCommand::OnEndDocCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
}

OnEndDocCommand::~OnEndDocCommand() {
}

void OnEndDocCommand::Execute() {
	KeyAction* keyAction = NULL;
	FactoryProduct factoryProduct(this->notePadForm);
	UINT nChar = this->notePadForm->nChar;

	keyAction = factoryProduct.Make(nChar, VK_CONTROL, NULL);
	keyAction->KeyDown();
}




