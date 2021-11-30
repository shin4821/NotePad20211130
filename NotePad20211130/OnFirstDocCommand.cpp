//OnFirstDocCommand.cpp
#include"OnFirstDocCommand.h"
#include"NotePadForm.h"
#include"KeyAction.h"
#include"FactoryProduct.h"
#include"Scroll.h"

OnFirstDocCommand::OnFirstDocCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
}

OnFirstDocCommand::~OnFirstDocCommand() {

}

void OnFirstDocCommand::Execute() {
	KeyAction* keyAction = NULL;
	FactoryProduct factoryProduct(this->notePadForm);
	UINT nChar = this->notePadForm->nChar;

	this->notePadForm->scroll->siVertical.nPos = 0;
	this->notePadForm->SetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical, TRUE);

	keyAction = factoryProduct.Make(nChar, VK_CONTROL, NULL);
	keyAction->KeyDown();
}

