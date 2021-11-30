//FirstRow.cpp
#include"FirstRow.h"
#include"NotePadForm.h"
#include"Glyph.h"
#include"Composite.h"
#include"Character.h"
#include"Note.h"
#include"Row.h"
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
#include"SelectWords.h"
#include"DummyRow.h"
#include<string>
using namespace std;
#pragma warning(disable:4996)

FirstRow::FirstRow(NotePadForm* notePadForm)
	:KeyAction(notePadForm) {
}
FirstRow::~FirstRow() {
}
void FirstRow::KeyDown() {
	Long currentY;
	Long currentX;
	Glyph* row;


	//2. Home 키 버튼 눌렀을 때
	//2.0. 현재 note에서 현재 row 구한다.

	//2.1. 현재 note의 current 확인한다.
	currentY = this->notePadForm->note->GetCurrent();
	//2.2. 현재 note의 row를 찾는다.
	row = this->notePadForm->note->GetChild(currentY);
	currentX = this->notePadForm->row->GetCurrent();


	//(21.11.12.추가) 2.3. 현재 자동개행 되어있으면,
	if (this->notePadForm->isLineChangeButtonClicked == true) {
		//2.3.1. x좌표가 0보다 큰 경우, 0번째로 이동한다.
		if (currentX > 0) {
			row->First();	
		}
		//2.3.2. x좌표가 0보다 크지 않고 DummyRow이면,
		else if (dynamic_cast<DummyRow*>(row)) {
			//2.3.2.1. 현재 줄의 전 줄을 찾는다.
			currentY--;
			row = this->notePadForm->note->GetChild(currentY);
			//2.3.2.2. 전 줄의 0번째로 이동한다.
			this->notePadForm->note->MoveCurrent(currentY);
			row->First();

		}
	}
	//2.4. 자동개행 되어있지 않으면,현재 row에서 First를 호출한다.
	else {
		row->First();	
	}


	//2.4. 캐럿을 이동한다.
	this->notePadForm->isOnImeCharForMove = false;
	this->notePadForm->isFirstComposingForMove = false;

	// Scroll 업데이트 할 때의 조건을 설정한다.
	this->notePadForm->isJumpOverForPrevious = false;
	this->notePadForm->isJumpOverForNext = false;
	this->notePadForm->isDoubleByte = false;
	this->notePadForm->isUp = false;
	this->notePadForm->isDown = false;

	this->notePadForm->Notify();
	this->notePadForm->Invalidate();
}








