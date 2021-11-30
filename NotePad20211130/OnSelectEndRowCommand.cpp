//OnSelectEndRowCommand.cpp
#include"OnSelectEndRowCommand.h"
#include"NotePadForm.h"
#include"KeyAction.h"
#include"FactoryProduct.h"

OnSelectEndRowCommand::OnSelectEndRowCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
}

OnSelectEndRowCommand::~OnSelectEndRowCommand() {

}

void OnSelectEndRowCommand::Execute() {
	KeyAction* keyAction = NULL;
	FactoryProduct factoryProduct(this->notePadForm);
	UINT nChar = this->notePadForm->nChar;

	keyAction = factoryProduct.Make(nChar, VK_SHIFT, NULL);
	keyAction->KeyDown();
}
