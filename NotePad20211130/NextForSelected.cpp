//NextForSelected.cpp
#include"NextForSelected.h"
#include"NotePadForm.h"
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
#include"SelectWords.h"
#include<string>
using namespace std;
#pragma warning(disable:4996)

NextForSelected::NextForSelected(NotePadForm* notePadForm)
	:KeyAction(notePadForm) {

}
NextForSelected::~NextForSelected() {
}
void NextForSelected::KeyDown() {

	Glyph* character;
	Glyph* nextRow;
	Glyph* row;
	string strCharacter;
	char chaCharacter[3];
	Long caretX = 0;
	Long pageY;
	int length;
	int noteLength;

	//2. 오른쪽 키 버튼 눌렀을 때

	//21.08.04 추가
	Long beforeCurrentY = this->notePadForm->note->GetCurrent();


	//(21.11.22.추가)1.4. 현재 선택된 단어가 있는지 확인한다. *********************************************
	Long firstY = this->notePadForm->selectWords->GetFirstY();
	Long firstX = this->notePadForm->selectWords->GetFirstX();
	Long endY = this->notePadForm->selectWords->GetEndY();
	Long endX = this->notePadForm->selectWords->GetEndX();

	BOOL isBigger = TRUE;

	//1.4.1. 좌표가 isBigger인지 확인한다.
	if (firstY < endY) {
		isBigger = TRUE;
	}
	else if (firstY > endY) {
		isBigger = FALSE;
	}
	else if (firstY == endY) {
		if (firstX < endX) {
			isBigger = TRUE;
		}
		else if (firstX > endX) {
			isBigger = FALSE;
		}
	}

	//1.4.2. isBigger이 아니면 좌표를 서로 바꾼다.
	if (isBigger == FALSE) {
		int startX = firstX;
		int startY = firstY;
		int lastX = endX;
		int lastY = endY;

		firstX = lastX;
		firstY = lastY;
		endX = startX;
		endY = startY;
	}

	//1.4.3. firstX, firstY로 좌표를 이동한다.
	this->notePadForm->note->MoveCurrent(endY);
	row = this->notePadForm->note->GetChild(endY);
	row->MoveCurrent(endX);

	//1.4.4. 선택된 좌표를 초기화한다.
	this->notePadForm->selectWords->firstX = 0;
	this->notePadForm->selectWords->firstY = 0;
	this->notePadForm->selectWords->endX = 0;
	this->notePadForm->selectWords->endY = 0;

	//1. Shift 키에서 손을 뗐으면, 드래그 중을 false로 설정한다.
	this->notePadForm->selectWords->isDragging = FALSE;
	this->notePadForm->isShiftClicked = FALSE;
	this->notePadForm->isShifting = FALSE;

	this->notePadForm->ShiftX = 0;
	this->notePadForm->ShiftY = 0;
	//*************************************************************************************************


	// 현재 currentX까지의 너비를 구한다.
	//2.1. 현재 note의 current 확인한다.
	Long currentY = this->notePadForm->note->GetCurrent();
	//2.2. 현재 note의 row를 찾는다.
	row = this->notePadForm->note->GetChild(currentY);
	//2.3. 현재 row의 current 확인한다.
	Long currentX = row->GetCurrent();

	//(21.11.15.추가) 자동 개행된 줄에 대해 좌우 방향키에 의한 행간 이동 오류 방지용
	//2.4. 현재 자동개행인 경우,
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
		//2.4.1. 현재 캐럿의 x좌표가 row의 length이고, 그 다음줄이 NULL이 아니고 DummyRow인 경우,
		length = row->GetLength();
		noteLength = this->notePadForm->note->GetLength();
		nextRow = this->notePadForm->note->GetChild(currentY + 1);

		if (currentY + 1 < noteLength && currentX == length && dynamic_cast<DummyRow*>(nextRow)) {
			//그 다음줄의 0번째로 캐럿을 이동한다.
			this->notePadForm->note->MoveCurrent(currentY + 1);
			nextRow->MoveCurrent(0);
		}
	}

	//2.4. 캐럿을 이동한다.
	this->notePadForm->isOnImeCharForMove = FALSE;
	this->notePadForm->isFirstComposingForMove = FALSE;




	// Scroll 업데이트 할 때의 조건을 설정한다.
	this->notePadForm->isJumpOverForPrevious = FALSE;
	this->notePadForm->isJumpOverForNext = TRUE;
	this->notePadForm->isUp = FALSE;
	this->notePadForm->isDown = FALSE;
	this->notePadForm->isDoubleByte = FALSE;

	if (beforeCurrentY < currentY) {
		this->notePadForm->isJumpOverForNext = FALSE;
	}
	this->notePadForm->Notify();
}
