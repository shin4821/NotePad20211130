//NextWord.cpp
#include"NextWord.h"
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

NextWord::NextWord(NotePadForm* notePadForm)
	:KeyAction(notePadForm) {

}
NextWord::~NextWord() {
}
void NextWord::KeyDown() {
	Glyph* character;
	string strCharacter;
	char chaCharacter[3];
	Long caretX = 0;
	Long pageY;
	int currentX;
	int currentY;
	Glyph* row;

	//2. ������ Ű ��ư ������ ��

	//(21.08.04 �߰�) 2.1. ���� Y��ǥ�� ���Ѵ�.
	Long beforeCurrentY = this->notePadForm->note->GetCurrent();
	

	//(21.11.22.�߰�) 2.2. ���� �ڵ������� ���, 
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		//2.2.1. �ڵ����� ���� ĳ���� ��ġ�� ��ȯ�޴´�.
		this->notePadForm->lineChange->FindCaretBeforeLineChange(&currentX, &currentY);

		//2.2.2. �ڵ������� Ǯ���ش�.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

		//2.2.3. ĳ���� �Ű��ش�.
		this->notePadForm->note->MoveCurrent(currentY);
		row = this->notePadForm->note->GetChild(currentY);
		row->MoveCurrent(currentX);
	}

	//2.3. ���� note���� Next ����Ѵ�.
	this->notePadForm->note->NextWord();


	//2.4. ĳ���� �̵��Ѵ�.
	this->notePadForm->isOnImeCharForMove = FALSE;
	this->notePadForm->isFirstComposingForMove = FALSE;

    //2.5. ���� ��ǥ�� Ȯ���Ѵ�.
	currentY = this->notePadForm->note->GetCurrent();
	row = this->notePadForm->note->GetChild(currentY);
	currentX = row->GetCurrent();

	//(21.11.22.�߰�) 2.6. ���� �ڵ������� ���, 
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		//2.6.1. �ڵ����� ���ش�.
		this->notePadForm->lineChange->LineChangeButtonClicked();

		//2.6.2. �ڵ����� �� �̵��� ĳ���� ��ġ�� ���Ѵ�.
		this->notePadForm->lineChange->FindCaretAfterLineChange(currentX, currentY);
	}

	//2.7. Scroll ������Ʈ �� ���� ������ �����Ѵ�.
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

