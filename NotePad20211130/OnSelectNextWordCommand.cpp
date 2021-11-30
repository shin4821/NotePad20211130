//OnSelectNextWordCommand.cpp
#include"OnSelectNextWordCommand.h"
#include"NotePadForm.h"
#include"KeyAction.h"
#include"FactoryProduct.h"

OnSelectNextWordCommand::OnSelectNextWordCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
}

OnSelectNextWordCommand::~OnSelectNextWordCommand() {
}

void OnSelectNextWordCommand::Execute() {

	KeyAction* keyAction = NULL;
	FactoryProduct factoryProduct(this->notePadForm);
	UINT nChar = this->notePadForm->nChar;

	keyAction = factoryProduct.Make(nChar, VK_CONTROL, VK_SHIFT);
	keyAction->KeyDown();
}



