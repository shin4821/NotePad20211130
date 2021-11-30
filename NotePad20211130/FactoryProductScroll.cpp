//FactoryProductScroll.cpp
#include"FactoryProductScroll.h"
#include"LineRight.h"
#include"LineLeft.h"
#include"PageLeft.h"
#include"PageRight.h"
#include"LineUp.h"
#include"LineDown.h"
#include"PageUp.h"
#include"PageDown.h"
#include"ThumbTrackHorizon.h"
#include"ThumbTrackVertical.h"
#include"Top.h"

FactoryProductScroll::FactoryProductScroll(NotePadForm* notePadForm) {
	this->notePadForm = notePadForm;
}

FactoryProductScroll::~FactoryProductScroll() {
}

MoveAction* FactoryProductScroll::MakeHorizon(UINT nSBCode) {
	MoveAction* MoveAction = 0;

	if (nSBCode == SB_LINERIGHT) {
		MoveAction = new LineRight(this->notePadForm);
	}
	else if (nSBCode == SB_LINELEFT) {
		MoveAction = new LineLeft(this->notePadForm);
	}
	else if (nSBCode == SB_PAGELEFT) {
		MoveAction = new PageLeft(this->notePadForm);
	}
	else if (nSBCode == SB_PAGERIGHT) {
		MoveAction = new PageRight(this->notePadForm);
	}
	else if (nSBCode == SB_THUMBTRACK) {
		MoveAction = new ThumbTrackHorizon(this->notePadForm);
	}




	return MoveAction;
}

MoveAction* FactoryProductScroll::MakeVertical(UINT nSBCode) {
	MoveAction* MoveAction = 0;

    if (nSBCode == SB_LINEUP) {
		MoveAction = new LineUp(this->notePadForm);
	}
	else if (nSBCode == SB_LINEDOWN) {
		MoveAction = new LineDown(this->notePadForm);
	}
	else if (nSBCode == SB_PAGEUP) {
		MoveAction = new PageUp(this->notePadForm);
	}
	else if (nSBCode == SB_PAGEDOWN) {
		MoveAction = new PageDown(this->notePadForm);
	}
	else if (nSBCode == SB_THUMBTRACK) {
		MoveAction = new ThumbTrackVertical(this->notePadForm);
	}
	else if (nSBCode == SB_TOP) {
		MoveAction = new Top(this->notePadForm);
	}



	return MoveAction;
}













