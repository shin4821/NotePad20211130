//BackSpaceKeyAction.cpp
#include"BackSpaceKeyAction.h"
#include"Next.h"
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

BackSpaceKeyAction::BackSpaceKeyAction(NotePadForm* notePadForm)
	:KeyAction(notePadForm) {
}

BackSpaceKeyAction::~BackSpaceKeyAction() {

}

void BackSpaceKeyAction::KeyDown() {

	Long i = 0;
	Long j;
	Glyph* character = NULL;
	Glyph* row;
	Glyph* rowUp;
	Glyph* dummyRow;
	Glyph* nextRow;
	Glyph* endRow;
	Long length;
	Long lengthUp;
	char chaCharacter[3];
	char characterMove[3];
	Long changedX = 0;
	Long changedY = 0;
	string strCharacter;
	Long caretX;
	int width;
	int x = 0;
	BOOL isBigger = TRUE;
  //BOOL isSelected = FALSE;
	BOOL isAppend = FALSE;
	BOOL isDummyRow = FALSE;
	int difference;
	int limitX;


	//0. BackSpace를 눌렀을 때
	 //21.08.05 추가
	 this->notePadForm->scroll->beforeCurrentY = this->notePadForm->note->GetCurrent();
	 Long beforeCurrentY = this->notePadForm->note->GetCurrent();

	  //1. 현재 캐럿의 좌표를 구한다.
	int currentY = this->notePadForm->note->GetCurrent();

	//2. 현재 row를 구한다.
	row = this->notePadForm->note->GetChild(currentY);
	int currentX = row->GetCurrent();

	//(21.08.19 추가) 선택하다 포함 ******************************************************
	//3. firstX,Y, endX,Y를 구한다.
	int firstY = this->notePadForm->selectWords->GetFirstY();
	int firstX = this->notePadForm->selectWords->GetFirstX();
	int endY = this->notePadForm->selectWords->GetEndY();
	int endX = this->notePadForm->selectWords->GetEndX();


	int startX = firstX;
	int startY = firstY;
	int lastX = endX;
	int lastY = endY;

	//3.1. isBigger이 true인지 확인한다. 
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

	//3.2. isBigger이 아니면 좌표를 서로 바꾼다.

	if (isBigger == FALSE) {
		startX = firstX;
		startY = firstY;
		lastX = endX;
		lastY = endY;

		firstX = lastX;
		firstY = lastY;
		endX = startX;
		endY = startY;
	}

	//************************************************************************************

	//4. 자동개행버튼이 눌려진 경우,
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		//4.1. 자동개행 전 firstXY, endXY의 좌표를 구한다.
		this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
		this->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);
		this->notePadForm->lineChange->FindCaretBeforeLineChange(currentX, currentY, &currentX, &currentY);

		//4.2. 자동개행된 문서를 펼친다.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

		//4.3. 현재 row를 구한다.
		row = this->notePadForm->note->GetChild(currentY);
	}

	//5. 선택된 범위가 없을 경우,
	if (firstX <= 0 && firstY <= 0 && endX <= 0 && endY <= 0) {

		//5.1. currentY가 0보다 크고, currentX가 0인 경우,
		if (currentY > 0 && currentX <= 0) {
			length = row->GetLength();
			rowUp = this->notePadForm->note->GetChild(currentY - 1);
			lengthUp = rowUp->GetLength();

			//5.1.1. 해당 row의 length가 0보다 큰 경우,
			if (length > 0) {
				//5.1.1.1. length만큼 반복한다.
				while (i < length) {
					//1. currentY의 row의 문자를 읽는다.
					character = row->GetChild(i);
					//2. currentY-1의 row에 문자를 add한다.
					rowUp->Add(character);
					i++;
				}
				//5.1.1.2. 캐럿의 Y를 currentY-1,X를 lengthUp으로 옮긴다.
				changedY = currentY - 1;
				changedX = lengthUp;

				//5.1.1.3. currentY의 row를 삭제한다.
				this->notePadForm->note->Remove(currentY);
			}
			//5.1.2. 해당 row의 length가 0과 같거나 작은 경우,
			else {
				//5.1.2.1. 해당 row를 삭제한다.
				this->notePadForm->note->Remove(currentY);
				//5.1.2.2. 캐럿의 Y를 currentY-1, X를 lengthUp으로 이동한다.
				changedY = currentY - 1;
				changedX = lengthUp;
			}
		}
		//5.2. currentX가 0보다 큰 경우,
		else if (currentX > 0) {
			//5.2.1. currentX-1번째 문자를 지운다.
			row->Remove(currentX - 1);

			//5.2.2. currentX-1번째로 캐럿을 이동한다.
			changedY = currentY;
			changedX = currentX - 1;
		}
	}

	//6. 선택된 범위가 있을 경우,
	else {
		//1.3. firstY, endY가 같은 경우,
		if (firstY == endY) {
			//1.3.1. endX와 firstX의 차액을 구한다.
			difference = endX - firstX;

			//1.3.2. 차액이 0보다 큰 동안 firstX번째 글자를 지워준다.
			while (difference > 0) {
				row->Remove(firstX);
				difference--;
			}
		}

		//1.4. endY가 firstY보다 더 큰 경우,
		else if (endY > firstY) {

			//1.4.1. 붙여적어야 하는 조건을 확인한다.
			endRow = this->notePadForm->note->GetChild(endY);
			if (firstX > 0 && endX < endRow->GetLength()) {
				isAppend = TRUE;
			}

			//1.4.2. firstY가 endY보다 작거나 같은 동안 반복한다.
			i = firstY;
			while (i <= endY) {
				row = this->notePadForm->note->GetChild(i);

				//1.4.2.1. j를 정한다.
				if (i <= firstY) {
					j = firstX;
				}
				else {
					j = 0;
				}

				//1.4.2.2. limitX 를 결정한다.
				if (i >= endY) {
					limitX = endX;
				}
				else {
					limitX = row->GetLength();
				}

				difference = limitX - j;

				//1.4.2.3. difference가 0보다 큰 동안 반복한다.
				while (difference > 0) {
					row->Remove(j);
					difference--;
				}

				//1.4.2.4. row의 length가 0보다 작거나 같은 경우,해당 줄을 지운다.
				if (row->GetLength() <= 0) {

					//아예 마지막 줄이면 삭제하면 안된다. 그 이내의 줄만 ***************************
					if (i < this->notePadForm->note->GetLength() - 1) {
						this->notePadForm->note->Remove(i);
					}
					//*******************************************************************************
					//this->notePadForm->note->Remove(i);
					endY--;
				}
				else {
					i++;
				}
			}

			//1.4.3. isAppend가 true이면. 
			if (isAppend == TRUE) {
				row = this->notePadForm->note->GetChild(firstY);
				endRow = this->notePadForm->note->GetChild(firstY + 1);
				i = 0;

				//1.4.3.1. firstY의 row에 붙혀적는다.
				while (i < endRow->GetLength()) {
					character = endRow->GetChild(i);
					row->Add(character);
					i++;
				}
				//1.4.3.2. firstY+1의 row를 지운다.
				this->notePadForm->note->Remove(firstY + 1);
			}
		}

		//1.5. 캐럿의 옮길 좌표를 정한다.
		changedY = firstY;
		changedX = firstX;
		//changedX--;

	}
	
	//7. 자동개행이 눌려진 경우,
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		//7.1. 펼친 문서를 다시 자동개행한다. (ON)
		this->notePadForm->lineChange->LineChangeButtonClicked();

		//7.2. 자동개행 후 이동할 캐럿의 위치를 구한다.
		this->notePadForm->lineChange->FindCaretAfterLineChange(changedX, changedY);
	}
	else {
		this->notePadForm->note->MoveCurrent(changedY);
		this->notePadForm->row = this->notePadForm->note->GetChild(changedY);
		this->notePadForm->row->MoveCurrent(changedX);
	}

	//1.6. 캐럿을 이동한다.
	this->notePadForm->isOnImeCharForMove = FALSE;
	this->notePadForm->isFirstComposingForMove = FALSE;


	//2. firstXY, endXY를 초기화한다. ****************************
	this->notePadForm->selectWords->firstX = 0;
	this->notePadForm->selectWords->firstY = 0;
	this->notePadForm->selectWords->endX = 0;
	this->notePadForm->selectWords->endY = 0;
	//************************************************************


	// Scroll 업데이트 할 때의 조건을 설정한다.
	this->notePadForm->isJumpOverForPrevious = TRUE;
	this->notePadForm->isJumpOverForNext = FALSE;
	this->notePadForm->isDoubleByte = FALSE;
	this->notePadForm->isUp = FALSE;
	this->notePadForm->isDown = FALSE;

	if (beforeCurrentY > currentY) {
		this->notePadForm->isJumpOverForPrevious = FALSE;
	}


	this->notePadForm->Notify();

	this->notePadForm->Invalidate();
}













