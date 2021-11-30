//OnSelectPreviousWordCommand.cpp
#include"OnSelectPreviousWordCommand.h"
#include"NotePadForm.h"
#include"KeyAction.h"
#include"FactoryProduct.h"

OnSelectPreviousWordCommand::OnSelectPreviousWordCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
}

OnSelectPreviousWordCommand::~OnSelectPreviousWordCommand() {
}

void OnSelectPreviousWordCommand::Execute() {
	KeyAction* keyAction = NULL;
	FactoryProduct factoryProduct(this->notePadForm);
	UINT nChar = this->notePadForm->nChar;

	keyAction = factoryProduct.Make(nChar, VK_CONTROL, VK_SHIFT);
	keyAction->KeyDown();

}










