//OnNextWordCommand.cpp
#include"OnNextWordCommand.h"
#include"NotePadForm.h"
#include"KeyAction.h"
#include"FactoryProduct.h"

OnNextWordCommand::OnNextWordCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
}

OnNextWordCommand::~OnNextWordCommand() {
}

void OnNextWordCommand::Execute() {
	KeyAction* keyAction = NULL;
	FactoryProduct factoryProduct(this->notePadForm);
	UINT nChar = this->notePadForm->nChar;

	keyAction = factoryProduct.Make(nChar, VK_CONTROL, NULL);
	keyAction->KeyDown();
}









