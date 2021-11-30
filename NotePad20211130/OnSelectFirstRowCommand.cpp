//OnSelectFirstRowCommand.cpp
#include"OnSelectFirstRowCommand.h"
#include"NotePadForm.h"
#include"KeyAction.h"
#include"FactoryProduct.h"


OnSelectFirstRowCommand::OnSelectFirstRowCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
}

OnSelectFirstRowCommand::~OnSelectFirstRowCommand() {

}

void OnSelectFirstRowCommand::Execute() {
	KeyAction* keyAction = NULL;
	FactoryProduct factoryProduct(this->notePadForm);
	UINT nChar = this->notePadForm->nChar;

	keyAction = factoryProduct.Make(nChar, VK_SHIFT, NULL);
	keyAction->KeyDown();
}

