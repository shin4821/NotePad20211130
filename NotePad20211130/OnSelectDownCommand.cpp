//OnSelectDownCommand.cpp
#include"OnSelectDownCommand.h"
#include"NotePadForm.h"
#include"KeyAction.h"
#include"FactoryProduct.h"

OnSelectDownCommand::OnSelectDownCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
}

OnSelectDownCommand::~OnSelectDownCommand() {
}

void OnSelectDownCommand::Execute() {
	KeyAction* keyAction = NULL;
	FactoryProduct factoryProduct(this->notePadForm);
	UINT nChar = this->notePadForm->nChar;

	keyAction = factoryProduct.Make(nChar, VK_SHIFT, NULL);
	keyAction->KeyDown();
}






