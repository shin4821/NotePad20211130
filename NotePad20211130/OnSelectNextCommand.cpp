//OnSelectNextCommand.cpp
#include"OnSelectNextCommand.h"
#include"NotePadForm.h"
#include"KeyAction.h"
#include"FactoryProduct.h"

OnSelectNextCommand::OnSelectNextCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
}

OnSelectNextCommand::~OnSelectNextCommand() {
}

void OnSelectNextCommand::Execute() {

	KeyAction* keyAction = NULL;
	FactoryProduct factoryProduct(this->notePadForm);
	UINT nChar = this->notePadForm->nChar;

	keyAction = factoryProduct.Make(nChar, VK_SHIFT, NULL);
	keyAction->KeyDown();
}











