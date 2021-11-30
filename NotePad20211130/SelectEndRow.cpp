//SelectEndRow.cpp
#include"SelectEndRow.h"
#include"NotePadForm.h"
#include"EndRow.h"
#include"SelectWords.h"
#include"Glyph.h"
#include"Composite.h"
#include"Character.h"
#include"Note.h"
#include"Row.h"
#include"DummyRow.h"
#include"SingleByteCharacter.h"
#include"DoubleByteCharacter.h"
#include"CreateGlyph.h"
#include"CreateProduct.h"
#include"GetTextSize.h"
#include"Location.h"
#include"Font.h"
#include"Caret.h"
#include"Observer.h"
#include"Scroll.h"


SelectEndRow::SelectEndRow(NotePadForm* notePadForm)
	:KeyAction(notePadForm) {
}

SelectEndRow:: ~SelectEndRow() {
}

void SelectEndRow::KeyDown() {
	Long beforeCurrentX;
	Long beforeCurrentY;
	Long currentX;
	Long currentY;
	Glyph* row;


	//1. Up하기 전, 현재 좌표의 위치를 구한다.
	beforeCurrentY = this->notePadForm->note->GetCurrent();
	row = this->notePadForm->note->GetChild(beforeCurrentY);
	beforeCurrentX = row->GetCurrent();

	//2. 기존 Down 클래스의 keyDown 함수와 동일하게 만들어준다.
	KeyAction* keyAction = new EndRow(this->notePadForm);
	keyAction->KeyDown();

	//3. 처음 드래그 시작한 것이면,
	if (this->notePadForm->selectWords->isDragging == FALSE) {

		//3.1. SelectWords의 firstX,Y에 next하기 전 좌표를 적는다.
		this->notePadForm->selectWords->firstX = beforeCurrentX;
		this->notePadForm->selectWords->firstY = beforeCurrentY;
	}

	//4. Down  후의 좌표를 SelectWords의 endX,Y에 적는다.
	currentY = this->notePadForm->note->GetCurrent();
	row = this->notePadForm->note->GetChild(currentY);
	currentX = row->GetCurrent();

	this->notePadForm->selectWords->endX = currentX;
	this->notePadForm->selectWords->endY = currentY;

	//5. 드래그 중을 true로 바꾼다.
	this->notePadForm->selectWords->isDragging = TRUE;

}