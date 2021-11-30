//Previous.cpp
#include"Previous.h"
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

Previous::Previous(NotePadForm* notePadForm) 
	:KeyAction(notePadForm){
	//this->notePadForm = notePadForm;
}
Previous::~Previous() {

}
void Previous::KeyDown() {

	int currentY;
	int currentX;
	Glyph* character;
	Glyph* beforeRow;
	Glyph* row;
	Glyph* nextRow;
	Long i = 0;
	int strSizeY = this->notePadForm->getTextSize->GetX(131);
	string strCharacter;
	char chaCharacter[3];
	Long caretX;
	Long caretY;
	Long DoubleByteCaretWidth;
	Long length;
	char characterMove[3]; //Observe Pattern ������.
	int newNPos;
	int changedNPos;

	//1. ���� Ű ��ư ������ ��
		//1.1. Ŀ�� ũ�Ⱑ �ѱ��� �β��� �ƴϸ�, ���� note���� Previous ȣ���Ѵ�.
		DoubleByteCaretWidth = this->notePadForm->getTextSize->GetX(128);

		//1.2. Previous ȣ�� �� ĳ��Y�� ��ǥ�� ���س��´�.
		Long beforeCurrentY = this->notePadForm->note->GetCurrent();

		//(21.11.15.�߰�)1.3. Previous �̵��ϱ� �� ���� DummyRow���� Ȯ���Ѵ�.
		beforeRow = this->notePadForm->note->GetChild(beforeCurrentY);

		//1.3. Previous�� ȣ���Ѵ�.
		if (this->notePadForm->caretWidth != DoubleByteCaretWidth) {
			this->notePadForm->note->Previous();
		}

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

		//(21.11.23.�߰�)4. ���� ��ǥ�� ���� windowHeight ���� �ȿ� ����,
		int firstTextY = this->notePadForm->firstTextY;
		int endTextY = this->notePadForm->endTextY;
		currentY = this->notePadForm->note->GetCurrent();

		if (beforeCurrentY >= firstTextY && beforeCurrentY <= endTextY) {
			//2.4. ĳ���� �̵��Ѵ�.
			this->notePadForm->isOnImeCharForMove = FALSE;
			this->notePadForm->isFirstComposingForMove = FALSE;

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
		//(21.11.23.�߰�) 5.���� ��ǥ�� firstTextY���� ���� �ִ� ���,
		else if (beforeCurrentY < firstTextY) {
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
			int changedCurrentY = currentY - difference;
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























