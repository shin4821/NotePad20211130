//OnSelectUpCommand.cpp
#include"OnSelectUpCommand.h"
#include"NotePadForm.h"
#include"KeyAction.h"
#include"FactoryProduct.h"

OnSelectUpCommand::OnSelectUpCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
}

OnSelectUpCommand::~OnSelectUpCommand() {
}

void OnSelectUpCommand::Execute() {
	KeyAction* keyAction = NULL;
	FactoryProduct factoryProduct(this->notePadForm);
	UINT nChar = this->notePadForm->nChar;

	keyAction = factoryProduct.Make(nChar, VK_SHIFT, NULL);
	keyAction->KeyDown();
}

