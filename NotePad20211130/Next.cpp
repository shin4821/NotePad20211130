//Next.cpp
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
#include<string>
using namespace std;
#pragma warning(disable:4996)

Next::Next(NotePadForm* notePadForm) 
	:KeyAction(notePadForm){
	
}
Next::~Next() {
}
void Next::KeyDown() {

	Glyph* character;
	Glyph* nextRow;
	Glyph* row;
	string strCharacter;
	char chaCharacter[3];
	Long caretX = 0;
	Long pageY;
	int length;
	int noteLength;
	Long beforeCurrentY = this->notePadForm->note->GetCurrent();
	int strSizeY = this->notePadForm->getTextSize->GetX(131);
	int changedNPos;

	//1. ���� note���� Next ����Ѵ�.
	this->notePadForm->note->Next();

	//2. ���� ��ǥ�� Ȯ���Ѵ�.
	Long currentY = this->notePadForm->note->GetCurrent();
	row = this->notePadForm->note->GetChild(currentY);
	Long currentX = row->GetCurrent();

	//(21.11.15.�߰�) �ڵ� ����� �ٿ� ���� �¿� ����Ű�� ���� �ణ �̵� ���� ������
	//3. ���� �ڵ������� ���,
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
		//3.1. ���� ĳ���� x��ǥ�� row�� length�̰�, �� �������� NULL�� �ƴϰ� DummyRow�� ���,
		length = row->GetLength();
		noteLength = this->notePadForm->note->GetLength();
		nextRow = this->notePadForm->note->GetChild(currentY + 1);

		if (currentY + 1 < noteLength && currentX == length && dynamic_cast<DummyRow*>(nextRow)) {
			//�� �������� 0��°�� ĳ���� �̵��Ѵ�.
			this->notePadForm->note->MoveCurrent(currentY + 1);
			nextRow->MoveCurrent(0);
		}
	}

	//(21.11.23.�߰�)4. ���� ��ǥ�� ���� windowHeight ���� �ȿ� ����,
	int firstTextY = this->notePadForm->firstTextY;
	int endTextY = this->notePadForm->endTextY;
	currentY = this->notePadForm->note->GetCurrent();

	if (beforeCurrentY >= firstTextY && beforeCurrentY <= endTextY) {
		//4.1. ĳ���� �̵��Ѵ�.
		this->notePadForm->isOnImeCharForMove = FALSE;
		this->notePadForm->isFirstComposingForMove = FALSE;

		// Scroll ������Ʈ �� ���� ������ �����Ѵ�.
		this->notePadForm->isJumpOverForNext = TRUE;
		this->notePadForm->isJumpOverForPrevious = FALSE;
		this->notePadForm->isUp = FALSE;
		this->notePadForm->isDown = FALSE;
		this->notePadForm->isDoubleByte = FALSE;

		if (beforeCurrentY < currentY) {
			this->notePadForm->isJumpOverForNext = FALSE;
		}
		this->notePadForm->Notify();
	}
	//(21.11.23.�߰�) 5.���� ��ǥ�� firstTextY���� ���� �ִ� ���,
	else if(beforeCurrentY < firstTextY){

		//5.1. 0���� Y��ǥ-1������ nPos�� �������ش�.
		changedNPos = strSizeY * currentY;
		this->notePadForm->scroll->siVertical.nPos = changedNPos;

		//5.2. nPos�� 0���� ������ 0���� ġȯ�Ѵ�.
		if (changedNPos < 0) {
			this->notePadForm->scroll->siVertical.nPos = 0;
		}

		//5.3. ��ũ�� ������ set�Ѵ�.
		this->notePadForm->SetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical, TRUE);

		this->notePadForm->ScrollWindow(0, this->notePadForm->scroll->siVertical.nPos, NULL, NULL);
		this->notePadForm->location->MoveCaret();
		this->notePadForm->UpdateWindow();
		this->notePadForm->Invalidate();

	}
	//(21.11.23.�߰�) 6.���� ��ǥ�� endTextY���� �ؿ� �ִ� ���,
	else if (beforeCurrentY > endTextY) {

		//6.1. ���� y��ǥ�� firstTextY���� ���̸� ���Ѵ�.
		int difference = endTextY - firstTextY;
		int changedCurrentY= currentY - difference;
		changedNPos = strSizeY * changedCurrentY;

		//6.2. nPos�� �������ش�.
		this->notePadForm->scroll->siVertical.nPos = changedNPos;

		//6.3. ��ũ�� ������ set�Ѵ�.
		this->notePadForm->SetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical, TRUE);

		this->notePadForm->ScrollWindow(0, this->notePadForm->scroll->siVertical.nPos, NULL, NULL);
		this->notePadForm->location->MoveCaret();
		this->notePadForm->UpdateWindow();
		this->notePadForm->Invalidate();
	}



}
