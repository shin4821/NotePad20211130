//EndRow.cpp
#include"EndRow.h"
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

EndRow::EndRow(NotePadForm* notePadForm)
	:KeyAction(notePadForm) {
}
EndRow::~EndRow() {
}
void EndRow::KeyDown() {
	string strCharacter;
	char chaCharacter[3];
	Glyph* character;
	Long currentY;
	Long currentX;
	Long length;
	Long noteLength;
	Glyph* row;

	//2. end 키 버튼 눌렀을 때
	//2.0. 현재 note에서 현재 row 구한다.

	//2.1. 현재 note의 current 확인한다.
	currentY = this->notePadForm->note->GetCurrent();
	noteLength = this->notePadForm->note->GetLength();

	//2.2. 현재 note의 row를 찾는다.
	this->notePadForm->row = this->notePadForm->note->GetChild(currentY);

	//(21.11.11.추가) 2.3. 자동개행이 되어있는 경우,
	if (this->notePadForm->isLineChangeButtonClicked == true) {
		//2.3.1. 현재 캐럿의 위치를 구한다.
		currentX = this->notePadForm->row->GetCurrent();
		//2.3.2. x좌표가 length보다 작은 경우, length 번째로 이동한다.
		length = this->notePadForm->row->GetLength();
		if (currentX < length) {
			this->notePadForm->row->MoveCurrent(length);
		}
		//2.3.3. x좌표가 length보다 크거나 같고 y++좌표가 noteLength보다 작은 경우,
		else {
			currentY++;
			if (currentY < noteLength) {
				//2.3.3.1. currentY++번째 줄을 찾는다.
				row = this->notePadForm->note->GetChild(currentY);

				//2.3.3.2. currentY++번째 줄이 DummyRow이면 해당줄의 length번째로 이동한다.
				if (dynamic_cast<DummyRow*>(row)) {
					this->notePadForm->note->MoveCurrent(currentY);
					length = row->GetLength();
					row->MoveCurrent(length);
				}
			}
		}
	}
	//2.4. 자동개행이 안되어있는 경우,
	else {

		//2.4.1. 현재 row에서 Last를 호출한다.
		this->notePadForm->row->Last();
		currentX = this->notePadForm->row->GetCurrent();
		currentX++;
		this->notePadForm->row->MoveCurrent(currentX);
	}




	//2.4. 캐럿을 이동한다.
	this->notePadForm->isOnImeCharForMove = FALSE;
	this->notePadForm->isFirstComposingForMove = FALSE;

	// Scroll 업데이트 할 때의 조건을 설정한다.
	this->notePadForm->isJumpOverForPrevious = FALSE;
	this->notePadForm->isJumpOverForNext = FALSE;
	this->notePadForm->isUp = FALSE;
	this->notePadForm->isDown = FALSE;
	this->notePadForm->isDoubleByte = FALSE;
	
	this->notePadForm->Notify();
}

















