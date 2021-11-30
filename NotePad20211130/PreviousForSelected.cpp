//PreviousForSelected.cpp
#include"PreviousForSelected.h"
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
#include"LineChange.h"
#include<string>
using namespace std;
#pragma warning(disable:4996)

PreviousForSelected::PreviousForSelected(NotePadForm* notePadForm)
	:KeyAction(notePadForm) {
	//this->notePadForm = notePadForm;
}
PreviousForSelected::~PreviousForSelected() {

}
void PreviousForSelected::KeyDown() {

	int currentY;
	int currentX;
	Glyph* character;
	Glyph* beforeRow;
	Glyph* row;
	Glyph* nextRow;
	Long i = 0;
	int strSizeY;
	string strCharacter;
	char chaCharacter[3];
	Long caretX;
	Long caretY;
	Long DoubleByteCaretWidth;
	Long length;
	char characterMove[3]; //Observe Pattern 때문에.
	int newNPos;

	//1. 왼쪽 키 버튼 눌렀을 때
		//1.1. 커서 크기가 한글의 두께가 아니면, 현재 note에서 Previous 호출한다.
	DoubleByteCaretWidth = this->notePadForm->getTextSize->GetX(128);

	//1.2. Previous 호출 전 캐럿Y의 좌표를 구해놓는다.
	Long beforeCurrentY = this->notePadForm->note->GetCurrent();

	//(21.11.15.추가)1.3. Previous 이동하기 전 줄이 DummyRow인지 확인한다.
	beforeRow = this->notePadForm->note->GetChild(beforeCurrentY);

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
	this->notePadForm->note->MoveCurrent(firstY);
	row = this->notePadForm->note->GetChild(firstY);
	row->MoveCurrent(firstX);

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


	//1.2. 현재 note의 current 확인한다.
	currentY = this->notePadForm->note->GetCurrent();
	//1.3. 현재 note의 row를 찾는다.
	row = this->notePadForm->note->GetChild(currentY);
	//1.4. 현재 row의 current 확인한다.
	currentX = row->GetCurrent();


	//1.4. 자동개행일 경우,
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		length = row->GetLength();

		//1.4.1. 현재 캐럿의 x좌표가 row의 length이고, 이동하기 전 줄이 DummyRow인 경우,
		if (dynamic_cast<DummyRow*>(beforeRow) && currentX == length) {
			//현재 줄의 endX-1로 이동한다.
			currentX = length - 1;
			row->MoveCurrent(currentX);
		}
	}

	if (this->notePadForm->caretWidth == DoubleByteCaretWidth) {//현재 커서크기가 글자크기면 x를 1씩 뺀다.
		currentX--;
		row->MoveCurrent(currentX);
	}

	//2.4. 캐럿을 이동한다.
	this->notePadForm->isOnImeCharForMove = FALSE;
	this->notePadForm->isFirstComposingForMove = FALSE;

	//-------------------------------------------------------------------------------------------------

	// Scroll 업데이트 할 때의 조건을 설정한다.
	this->notePadForm->isJumpOverForNext = FALSE;
	this->notePadForm->isJumpOverForPrevious = TRUE;
	this->notePadForm->isUp = FALSE;
	this->notePadForm->isDown = FALSE;
	this->notePadForm->isDoubleByte = FALSE;
	if (beforeCurrentY > currentY) {
		this->notePadForm->isJumpOverForPrevious = FALSE;
	}

	this->notePadForm->Notify();

}

