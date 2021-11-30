//OnEndRowCommand.cpp
#include"OnEndRowCommand.h"
#include"NotePadForm.h"
#include"KeyAction.h"
#include"FactoryProduct.h"


OnEndRowCommand::OnEndRowCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
}

OnEndRowCommand::~OnEndRowCommand() {
}

void OnEndRowCommand::Execute() {
	KeyAction* keyAction = NULL;
	FactoryProduct factoryProduct(this->notePadForm);
	UINT nChar = this->notePadForm->nChar;

	keyAction = factoryProduct.Make(nChar, NULL, NULL);
	keyAction->KeyDown();
}

