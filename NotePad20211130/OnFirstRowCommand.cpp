//OnFirstRowCommand.cpp
#include"OnFirstRowCommand.h"
#include"NotePadForm.h"
#include"KeyAction.h"
#include"FactoryProduct.h"

OnFirstRowCommand::OnFirstRowCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
}

OnFirstRowCommand::~OnFirstRowCommand() {
}

void OnFirstRowCommand::Execute() {

	KeyAction* keyAction = NULL;
	FactoryProduct factoryProduct(this->notePadForm);
	UINT nChar = this->notePadForm->nChar;

	keyAction = factoryProduct.Make(nChar, NULL, NULL);
	keyAction->KeyDown();

}










