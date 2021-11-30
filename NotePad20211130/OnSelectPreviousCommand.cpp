//OnSelectPreviousCommand.cpp(21.09.08 Ãß°¡)
#include"OnSelectPreviousCommand.h"
#include"NotePadForm.h"
#include"KeyAction.h"
#include"FactoryProduct.h"

OnSelectPreviousCommand::OnSelectPreviousCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
}

OnSelectPreviousCommand::~OnSelectPreviousCommand() {

}

void OnSelectPreviousCommand::Execute() {

	KeyAction* keyAction = NULL;
	FactoryProduct factoryProduct(this->notePadForm);
	UINT nChar = this->notePadForm->nChar;

	keyAction = factoryProduct.Make(nChar, VK_SHIFT, NULL);
	keyAction->KeyDown();
}













