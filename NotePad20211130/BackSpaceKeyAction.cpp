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


	//0. BackSpace�� ������ ��
	 //21.08.05 �߰�
	 this->notePadForm->scroll->beforeCurrentY = this->notePadForm->note->GetCurrent();
	 Long beforeCurrentY = this->notePadForm->note->GetCurrent();

	  //1. ���� ĳ���� ��ǥ�� ���Ѵ�.
	int currentY = this->notePadForm->note->GetCurrent();

	//2. ���� row�� ���Ѵ�.
	row = this->notePadForm->note->GetChild(currentY);
	int currentX = row->GetCurrent();

	//(21.08.19 �߰�) �����ϴ� ���� ******************************************************
	//3. firstX,Y, endX,Y�� ���Ѵ�.
	int firstY = this->notePadForm->selectWords->GetFirstY();
	int firstX = this->notePadForm->selectWords->GetFirstX();
	int endY = this->notePadForm->selectWords->GetEndY();
	int endX = this->notePadForm->selectWords->GetEndX();


	int startX = firstX;
	int startY = firstY;
	int lastX = endX;
	int lastY = endY;

	//3.1. isBigger�� true���� Ȯ���Ѵ�. 
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

	//3.2. isBigger�� �ƴϸ� ��ǥ�� ���� �ٲ۴�.

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

	//4. �ڵ������ư�� ������ ���,
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		//4.1. �ڵ����� �� firstXY, endXY�� ��ǥ�� ���Ѵ�.
		this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
		this->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);
		this->notePadForm->lineChange->FindCaretBeforeLineChange(currentX, currentY, &currentX, &currentY);

		//4.2. �ڵ������ ������ ��ģ��.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

		//4.3. ���� row�� ���Ѵ�.
		row = this->notePadForm->note->GetChild(currentY);
	}

	//5. ���õ� ������ ���� ���,
	if (firstX <= 0 && firstY <= 0 && endX <= 0 && endY <= 0) {

		//5.1. currentY�� 0���� ũ��, currentX�� 0�� ���,
		if (currentY > 0 && currentX <= 0) {
			length = row->GetLength();
			rowUp = this->notePadForm->note->GetChild(currentY - 1);
			lengthUp = rowUp->GetLength();

			//5.1.1. �ش� row�� length�� 0���� ū ���,
			if (length > 0) {
				//5.1.1.1. length��ŭ �ݺ��Ѵ�.
				while (i < length) {
					//1. currentY�� row�� ���ڸ� �д´�.
					character = row->GetChild(i);
					//2. currentY-1�� row�� ���ڸ� add�Ѵ�.
					rowUp->Add(character);
					i++;
				}
				//5.1.1.2. ĳ���� Y�� currentY-1,X�� lengthUp���� �ű��.
				changedY = currentY - 1;
				changedX = lengthUp;

				//5.1.1.3. currentY�� row�� �����Ѵ�.
				this->notePadForm->note->Remove(currentY);
			}
			//5.1.2. �ش� row�� length�� 0�� ���ų� ���� ���,
			else {
				//5.1.2.1. �ش� row�� �����Ѵ�.
				this->notePadForm->note->Remove(currentY);
				//5.1.2.2. ĳ���� Y�� currentY-1, X�� lengthUp���� �̵��Ѵ�.
				changedY = currentY - 1;
				changedX = lengthUp;
			}
		}
		//5.2. currentX�� 0���� ū ���,
		else if (currentX > 0) {
			//5.2.1. currentX-1��° ���ڸ� �����.
			row->Remove(currentX - 1);

			//5.2.2. currentX-1��°�� ĳ���� �̵��Ѵ�.
			changedY = currentY;
			changedX = currentX - 1;
		}
	}

	//6. ���õ� ������ ���� ���,
	else {
		//1.3. firstY, endY�� ���� ���,
		if (firstY == endY) {
			//1.3.1. endX�� firstX�� ������ ���Ѵ�.
			difference = endX - firstX;

			//1.3.2. ������ 0���� ū ���� firstX��° ���ڸ� �����ش�.
			while (difference > 0) {
				row->Remove(firstX);
				difference--;
			}
		}

		//1.4. endY�� firstY���� �� ū ���,
		else if (endY > firstY) {

			//1.4.1. �ٿ������ �ϴ� ������ Ȯ���Ѵ�.
			endRow = this->notePadForm->note->GetChild(endY);
			if (firstX > 0 && endX < endRow->GetLength()) {
				isAppend = TRUE;
			}

			//1.4.2. firstY�� endY���� �۰ų� ���� ���� �ݺ��Ѵ�.
			i = firstY;
			while (i <= endY) {
				row = this->notePadForm->note->GetChild(i);

				//1.4.2.1. j�� ���Ѵ�.
				if (i <= firstY) {
					j = firstX;
				}
				else {
					j = 0;
				}

				//1.4.2.2. limitX �� �����Ѵ�.
				if (i >= endY) {
					limitX = endX;
				}
				else {
					limitX = row->GetLength();
				}

				difference = limitX - j;

				//1.4.2.3. difference�� 0���� ū ���� �ݺ��Ѵ�.
				while (difference > 0) {
					row->Remove(j);
					difference--;
				}

				//1.4.2.4. row�� length�� 0���� �۰ų� ���� ���,�ش� ���� �����.
				if (row->GetLength() <= 0) {

					//�ƿ� ������ ���̸� �����ϸ� �ȵȴ�. �� �̳��� �ٸ� ***************************
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

			//1.4.3. isAppend�� true�̸�. 
			if (isAppend == TRUE) {
				row = this->notePadForm->note->GetChild(firstY);
				endRow = this->notePadForm->note->GetChild(firstY + 1);
				i = 0;

				//1.4.3.1. firstY�� row�� �������´�.
				while (i < endRow->GetLength()) {
					character = endRow->GetChild(i);
					row->Add(character);
					i++;
				}
				//1.4.3.2. firstY+1�� row�� �����.
				this->notePadForm->note->Remove(firstY + 1);
			}
		}

		//1.5. ĳ���� �ű� ��ǥ�� ���Ѵ�.
		changedY = firstY;
		changedX = firstX;
		//changedX--;

	}
	
	//7. �ڵ������� ������ ���,
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		//7.1. ��ģ ������ �ٽ� �ڵ������Ѵ�. (ON)
		this->notePadForm->lineChange->LineChangeButtonClicked();

		//7.2. �ڵ����� �� �̵��� ĳ���� ��ġ�� ���Ѵ�.
		this->notePadForm->lineChange->FindCaretAfterLineChange(changedX, changedY);
	}
	else {
		this->notePadForm->note->MoveCurrent(changedY);
		this->notePadForm->row = this->notePadForm->note->GetChild(changedY);
		this->notePadForm->row->MoveCurrent(changedX);
	}

	//1.6. ĳ���� �̵��Ѵ�.
	this->notePadForm->isOnImeCharForMove = FALSE;
	this->notePadForm->isFirstComposingForMove = FALSE;


	//2. firstXY, endXY�� �ʱ�ȭ�Ѵ�. ****************************
	this->notePadForm->selectWords->firstX = 0;
	this->notePadForm->selectWords->firstY = 0;
	this->notePadForm->selectWords->endX = 0;
	this->notePadForm->selectWords->endY = 0;
	//************************************************************


	// Scroll ������Ʈ �� ���� ������ �����Ѵ�.
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













