//Caret.cpp
#include"Caret.h"
#include"Location.h"
#include"NotePadForm.h"

Caret::Caret(Location* location) {
	this->x = 0;
	this->y = 0;
	this->location = location;
}

Caret::Caret(Location* location, int x, int y) {
	this->x = x;
	this->y = y;
	this->location = location;
}

Caret::~Caret() {
}

Long Caret::GetX() {
	return this->x;
}

Long Caret::GetY() {
	return this->y;
}

void Caret::Move(int x, int y) {
	CPoint caretPoint;

	caretPoint.x = x;
	caretPoint.y = y;

	this->location->notePadForm->SetCaretPos(caretPoint);
	this->location->notePadForm->ShowCaret();

}













