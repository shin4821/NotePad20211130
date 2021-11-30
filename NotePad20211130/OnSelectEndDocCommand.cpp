//OnSelectEndDocCommand.cpp
#include"OnSelectEndDocCommand.h"
#include"NotePadForm.h"
#include"KeyAction.h"
#include"FactoryProduct.h"


OnSelectEndDocCommand::OnSelectEndDocCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
}

OnSelectEndDocCommand::~OnSelectEndDocCommand() {
}

void OnSelectEndDocCommand::Execute() {
	KeyAction* keyAction = NULL;
	FactoryProduct factoryProduct(this->notePadForm);
	UINT nChar = this->notePadForm->nChar;

	keyAction = factoryProduct.Make(nChar, VK_CONTROL, VK_SHIFT);
	keyAction->KeyDown();
}


