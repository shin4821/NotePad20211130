//OnVScrollCommand.cpp
#include"OnVScrollCommand.h"
#include"NotePadForm.h"
#include"MoveAction.h"
#include"FactoryProductScroll.h"


OnVScrollCommand::OnVScrollCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
}

OnVScrollCommand::~OnVScrollCommand() {
}

void OnVScrollCommand::Execute() {

	MoveAction* MoveAction = NULL;
	FactoryProductScroll factoryProductScroll(this->notePadForm);
	UINT nSBCode = this->notePadForm->nSBCode;

	//1. SB_LINEUP를 클릭했을 때
	if (nSBCode == SB_LINEUP) {
		MoveAction = factoryProductScroll.MakeVertical(nSBCode);
		//MoveAction = this->factoryProductScroll->MakeVertical(nSBCode);
		MoveAction->KeyDown();
	}
	//2. SB_LINEDOWN을 클릭했을 때
	else if (nSBCode == SB_LINEDOWN) {
		MoveAction = factoryProductScroll.MakeVertical(nSBCode);
		//MoveAction = this->factoryProductScroll->MakeVertical(nSBCode);
		MoveAction->KeyDown();
	}
	//3. SB_PAGEUP을 클릭했을 때
	else if (nSBCode == SB_PAGEUP) {
		MoveAction = factoryProductScroll.MakeVertical(nSBCode);
		MoveAction->KeyDown();
	}
	//4. SB_PAGEDOWN을 클릭했을 때
	else if (nSBCode == SB_PAGEDOWN) {
		MoveAction = factoryProductScroll.MakeVertical(nSBCode);
		MoveAction->KeyDown();
	}
	else if (nSBCode == SB_THUMBTRACK) {
		MoveAction = factoryProductScroll.MakeVertical(nSBCode);
		MoveAction->KeyDown();
	}
	else if (nSBCode == SB_TOP) {
		MoveAction = factoryProductScroll.MakeVertical(nSBCode);
		MoveAction->KeyDown();
	}


#if 0
	//2. SB_LINELEFT를 클릭했을 때
	else if (nSBCode == SB_LINEDOWN) {
		MoveAction = factoryProductScroll.Make(nSBCode);
		MoveAction->KeyDown();
	}
	//3. SB_PAGELEFT를 클릭했을 때
	else if (nSBCode == SB_PAGEUP) {
		MoveAction = factoryProductScroll.Make(nSBCode);
		MoveAction->KeyDown();
	}
	//3. SB_PAGERIGHT를 클릭했을 때
	else if (nSBCode == SB_PAGEDOWN) {
		MoveAction = factoryProductScroll.Make(nSBCode);
		MoveAction->KeyDown();
	}
#endif


}












