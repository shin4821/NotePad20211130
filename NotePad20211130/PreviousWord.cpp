//PreviousWord.cpp
#include"PreviousWord.h"
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
#include"LineChange.h"
#include<string>
using namespace std;
#pragma warning(disable:4996)


PreviousWord::PreviousWord(NotePadForm* notePadForm)
	:KeyAction(notePadForm){
	this->notePadForm = notePadForm;
}

PreviousWord::~PreviousWord() {

}

void PreviousWord::KeyDown() {
	int currentY;
	int currentX;
	Glyph* character;
	Glyph* row;
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

	//(21.08.03 �߰�)1.1. ���� Y��ǥ�� ���Ѵ�.
	Long beforeCurrentY = this->notePadForm->note->GetCurrent();

	//(21.11.22.�߰�) 1.2. ���� �ڵ������� ���, 
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		//1.2.1. �ڵ����� ���� ĳ���� ��ġ�� ��ȯ�޴´�.
		this->notePadForm->lineChange->FindCaretBeforeLineChange(&currentX, &currentY);

		//1.2.2. �ڵ������� Ǯ���ش�.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

		//1.2.3. ĳ���� �Ű��ش�.
		this->notePadForm->note->MoveCurrent(currentY);
		row = this->notePadForm->note->GetChild(currentY);
		row->MoveCurrent(currentX);
	}

	//1.3. ���� note���� Next ����Ѵ�.
	this->notePadForm->note->PreviousWord();

	//1.4. ���� ��ǥ�� Ȯ���Ѵ�.
	currentY = this->notePadForm->note->GetCurrent();
	row = this->notePadForm->note->GetChild(currentY);
	currentX = row->GetCurrent();

	//(21.11.22.�߰�) 1.5. ���� �ڵ������� ���, 
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		//1.5.1. �ڵ����� ���ش�.
		this->notePadForm->lineChange->LineChangeButtonClicked();

		//1.5.2. �ڵ����� �� �̵��� ĳ���� ��ġ�� ���Ѵ�.
		this->notePadForm->lineChange->FindCaretAfterLineChange(currentX, currentY);
	}
	
	//1.6. Scroll ������Ʈ �� ���� ������ �����Ѵ�.
	this->notePadForm->isOnImeCharForMove = FALSE;
	this->notePadForm->isFirstComposingForMove = FALSE;

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

















