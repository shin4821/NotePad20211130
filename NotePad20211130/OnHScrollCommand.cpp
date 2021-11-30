//OnHScrollCommand.cpp
#include"NotePadForm.h"
#include"OnHScrollCommand.h"
#include"MoveAction.h"
#include"FactoryProductScroll.h"

OnHScrollCommand::OnHScrollCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
}

OnHScrollCommand::~OnHScrollCommand() {
}

void OnHScrollCommand::Execute() {

	MoveAction* MoveAction = NULL;

	FactoryProductScroll factoryProductScroll(this->notePadForm);

	UINT nSBCode = this->notePadForm->nSBCode;

	//1. SB_LINERIGHT�� Ŭ������ ��
	if (nSBCode == SB_LINELEFT) {
		MoveAction = factoryProductScroll.MakeHorizon(nSBCode);
		//MoveAction = this->factoryProductScroll->MakeHorizon(nSBCode);
		MoveAction->KeyDown();
	}
	//2. SB_LINELEFT�� Ŭ������ ��
	else if (nSBCode == SB_LINERIGHT) {
		MoveAction = factoryProductScroll.MakeHorizon(nSBCode);
		//MoveAction = this->factoryProductScroll->MakeHorizon(nSBCode);
		MoveAction->KeyDown();
	}
	//3. SB_PAGELEFT�� Ŭ������ ��
	else if (nSBCode == SB_PAGELEFT) {
		MoveAction = factoryProductScroll.MakeHorizon(nSBCode);
		//MoveAction = this->factoryProductScroll->MakeHorizon(nSBCode);
		MoveAction->KeyDown();
	}
	//3. SB_PAGERIGHT�� Ŭ������ ��
	else if (nSBCode == SB_PAGERIGHT) {
		MoveAction = factoryProductScroll.MakeHorizon(nSBCode);
		//MoveAction = this->factoryProductScroll->MakeHorizon(nSBCode);
		MoveAction->KeyDown();
	}
	else if (nSBCode == SB_THUMBTRACK) {
		MoveAction = factoryProductScroll.MakeHorizon(nSBCode);
		MoveAction->KeyDown();
	}
}














