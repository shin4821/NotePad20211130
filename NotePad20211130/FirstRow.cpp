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


	//2. Home Ű ��ư ������ ��
	//2.0. ���� note���� ���� row ���Ѵ�.

	//2.1. ���� note�� current Ȯ���Ѵ�.
	currentY = this->notePadForm->note->GetCurrent();
	//2.2. ���� note�� row�� ã�´�.
	row = this->notePadForm->note->GetChild(currentY);
	currentX = this->notePadForm->row->GetCurrent();


	//(21.11.12.�߰�) 2.3. ���� �ڵ����� �Ǿ�������,
	if (this->notePadForm->isLineChangeButtonClicked == true) {
		//2.3.1. x��ǥ�� 0���� ū ���, 0��°�� �̵��Ѵ�.
		if (currentX > 0) {
			row->First();	
		}
		//2.3.2. x��ǥ�� 0���� ũ�� �ʰ� DummyRow�̸�,
		else if (dynamic_cast<DummyRow*>(row)) {
			//2.3.2.1. ���� ���� �� ���� ã�´�.
			currentY--;
			row = this->notePadForm->note->GetChild(currentY);
			//2.3.2.2. �� ���� 0��°�� �̵��Ѵ�.
			this->notePadForm->note->MoveCurrent(currentY);
			row->First();

		}
	}
	//2.4. �ڵ����� �Ǿ����� ������,���� row���� First�� ȣ���Ѵ�.
	else {
		row->First();	
	}


	//2.4. ĳ���� �̵��Ѵ�.
	this->notePadForm->isOnImeCharForMove = false;
	this->notePadForm->isFirstComposingForMove = false;

	// Scroll ������Ʈ �� ���� ������ �����Ѵ�.
	this->notePadForm->isJumpOverForPrevious = false;
	this->notePadForm->isJumpOverForNext = false;
	this->notePadForm->isDoubleByte = false;
	this->notePadForm->isUp = false;
	this->notePadForm->isDown = false;

	this->notePadForm->Notify();
	this->notePadForm->Invalidate();
}








