//OnSelectFirstDocCommand.cpp
#include"OnSelectFirstDocCommand.h"
#include"NotePadForm.h"
#include"KeyAction.h"
#include"FactoryProduct.h"

OnSelectFirstDocCommand::OnSelectFirstDocCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
}

OnSelectFirstDocCommand::~OnSelectFirstDocCommand() {
}

void OnSelectFirstDocCommand::Execute() {
	KeyAction* keyAction = NULL;
	FactoryProduct factoryProduct(this->notePadForm);
	UINT nChar = this->notePadForm->nChar;

	keyAction = factoryProduct.Make(nChar, VK_CONTROL, VK_SHIFT);
	keyAction->KeyDown();
}













