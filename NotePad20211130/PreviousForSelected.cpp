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
	char characterMove[3]; //Observe Pattern ������.
	int newNPos;

	//1. ���� Ű ��ư ������ ��
		//1.1. Ŀ�� ũ�Ⱑ �ѱ��� �β��� �ƴϸ�, ���� note���� Previous ȣ���Ѵ�.
	DoubleByteCaretWidth = this->notePadForm->getTextSize->GetX(128);

	//1.2. Previous ȣ�� �� ĳ��Y�� ��ǥ�� ���س��´�.
	Long beforeCurrentY = this->notePadForm->note->GetCurrent();

	//(21.11.15.�߰�)1.3. Previous �̵��ϱ� �� ���� DummyRow���� Ȯ���Ѵ�.
	beforeRow = this->notePadForm->note->GetChild(beforeCurrentY);

	//(21.11.22.�߰�)1.4. ���� ���õ� �ܾ �ִ��� Ȯ���Ѵ�. *********************************************
	Long firstY = this->notePadForm->selectWords->GetFirstY();
	Long firstX = this->notePadForm->selectWords->GetFirstX();
	Long endY = this->notePadForm->selectWords->GetEndY();
	Long endX = this->notePadForm->selectWords->GetEndX();

	BOOL isBigger = TRUE;

	//1.4.1. ��ǥ�� isBigger���� Ȯ���Ѵ�.
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

	//1.4.2. isBigger�� �ƴϸ� ��ǥ�� ���� �ٲ۴�.
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

	//1.4.3. firstX, firstY�� ��ǥ�� �̵��Ѵ�.
	this->notePadForm->note->MoveCurrent(firstY);
	row = this->notePadForm->note->GetChild(firstY);
	row->MoveCurrent(firstX);

	//1.4.4. ���õ� ��ǥ�� �ʱ�ȭ�Ѵ�.
	this->notePadForm->selectWords->firstX = 0;
	this->notePadForm->selectWords->firstY = 0;
	this->notePadForm->selectWords->endX = 0;
	this->notePadForm->selectWords->endY = 0;

	//1. Shift Ű���� ���� ������, �巡�� ���� false�� �����Ѵ�.
	this->notePadForm->selectWords->isDragging = FALSE;
	this->notePadForm->isShiftClicked = FALSE;
	this->notePadForm->isShifting = FALSE;

	this->notePadForm->ShiftX = 0;
	this->notePadForm->ShiftY = 0;
	//*************************************************************************************************


	//1.2. ���� note�� current Ȯ���Ѵ�.
	currentY = this->notePadForm->note->GetCurrent();
	//1.3. ���� note�� row�� ã�´�.
	row = this->notePadForm->note->GetChild(currentY);
	//1.4. ���� row�� current Ȯ���Ѵ�.
	currentX = row->GetCurrent();


	//1.4. �ڵ������� ���,
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		length = row->GetLength();

		//1.4.1. ���� ĳ���� x��ǥ�� row�� length�̰�, �̵��ϱ� �� ���� DummyRow�� ���,
		if (dynamic_cast<DummyRow*>(beforeRow) && currentX == length) {
			//���� ���� endX-1�� �̵��Ѵ�.
			currentX = length - 1;
			row->MoveCurrent(currentX);
		}
	}

	if (this->notePadForm->caretWidth == DoubleByteCaretWidth) {//���� Ŀ��ũ�Ⱑ ����ũ��� x�� 1�� ����.
		currentX--;
		row->MoveCurrent(currentX);
	}

	//2.4. ĳ���� �̵��Ѵ�.
	this->notePadForm->isOnImeCharForMove = FALSE;
	this->notePadForm->isFirstComposingForMove = FALSE;

	//-------------------------------------------------------------------------------------------------

	// Scroll ������Ʈ �� ���� ������ �����Ѵ�.
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

