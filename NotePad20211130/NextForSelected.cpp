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

	//2. ������ Ű ��ư ������ ��

	//21.08.04 �߰�
	Long beforeCurrentY = this->notePadForm->note->GetCurrent();


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
	this->notePadForm->note->MoveCurrent(endY);
	row = this->notePadForm->note->GetChild(endY);
	row->MoveCurrent(endX);

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


	// ���� currentX������ �ʺ� ���Ѵ�.
	//2.1. ���� note�� current Ȯ���Ѵ�.
	Long currentY = this->notePadForm->note->GetCurrent();
	//2.2. ���� note�� row�� ã�´�.
	row = this->notePadForm->note->GetChild(currentY);
	//2.3. ���� row�� current Ȯ���Ѵ�.
	Long currentX = row->GetCurrent();

	//(21.11.15.�߰�) �ڵ� ����� �ٿ� ���� �¿� ����Ű�� ���� �ణ �̵� ���� ������
	//2.4. ���� �ڵ������� ���,
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
		//2.4.1. ���� ĳ���� x��ǥ�� row�� length�̰�, �� �������� NULL�� �ƴϰ� DummyRow�� ���,
		length = row->GetLength();
		noteLength = this->notePadForm->note->GetLength();
		nextRow = this->notePadForm->note->GetChild(currentY + 1);

		if (currentY + 1 < noteLength && currentX == length && dynamic_cast<DummyRow*>(nextRow)) {
			//�� �������� 0��°�� ĳ���� �̵��Ѵ�.
			this->notePadForm->note->MoveCurrent(currentY + 1);
			nextRow->MoveCurrent(0);
		}
	}

	//2.4. ĳ���� �̵��Ѵ�.
	this->notePadForm->isOnImeCharForMove = FALSE;
	this->notePadForm->isFirstComposingForMove = FALSE;




	// Scroll ������Ʈ �� ���� ������ �����Ѵ�.
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
