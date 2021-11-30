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

	//2. end Ű ��ư ������ ��
	//2.0. ���� note���� ���� row ���Ѵ�.

	//2.1. ���� note�� current Ȯ���Ѵ�.
	currentY = this->notePadForm->note->GetCurrent();
	noteLength = this->notePadForm->note->GetLength();

	//2.2. ���� note�� row�� ã�´�.
	this->notePadForm->row = this->notePadForm->note->GetChild(currentY);

	//(21.11.11.�߰�) 2.3. �ڵ������� �Ǿ��ִ� ���,
	if (this->notePadForm->isLineChangeButtonClicked == true) {
		//2.3.1. ���� ĳ���� ��ġ�� ���Ѵ�.
		currentX = this->notePadForm->row->GetCurrent();
		//2.3.2. x��ǥ�� length���� ���� ���, length ��°�� �̵��Ѵ�.
		length = this->notePadForm->row->GetLength();
		if (currentX < length) {
			this->notePadForm->row->MoveCurrent(length);
		}
		//2.3.3. x��ǥ�� length���� ũ�ų� ���� y++��ǥ�� noteLength���� ���� ���,
		else {
			currentY++;
			if (currentY < noteLength) {
				//2.3.3.1. currentY++��° ���� ã�´�.
				row = this->notePadForm->note->GetChild(currentY);

				//2.3.3.2. currentY++��° ���� DummyRow�̸� �ش����� length��°�� �̵��Ѵ�.
				if (dynamic_cast<DummyRow*>(row)) {
					this->notePadForm->note->MoveCurrent(currentY);
					length = row->GetLength();
					row->MoveCurrent(length);
				}
			}
		}
	}
	//2.4. �ڵ������� �ȵǾ��ִ� ���,
	else {

		//2.4.1. ���� row���� Last�� ȣ���Ѵ�.
		this->notePadForm->row->Last();
		currentX = this->notePadForm->row->GetCurrent();
		currentX++;
		this->notePadForm->row->MoveCurrent(currentX);
	}




	//2.4. ĳ���� �̵��Ѵ�.
	this->notePadForm->isOnImeCharForMove = FALSE;
	this->notePadForm->isFirstComposingForMove = FALSE;

	// Scroll ������Ʈ �� ���� ������ �����Ѵ�.
	this->notePadForm->isJumpOverForPrevious = FALSE;
	this->notePadForm->isJumpOverForNext = FALSE;
	this->notePadForm->isUp = FALSE;
	this->notePadForm->isDown = FALSE;
	this->notePadForm->isDoubleByte = FALSE;
	
	this->notePadForm->Notify();
}

















