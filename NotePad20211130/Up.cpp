//Up.cpp
#include"Up.h"
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
#include<string>
using namespace std;
#pragma warning(disable:4996)

Up::Up(NotePadForm* notePadForm)
	:KeyAction(notePadForm) {
}
Up::~Up() {
}
void Up::KeyDown() {
	Long i = 0;
	Glyph* character;
	string strCharacter;
	char chaCharacter[3];
	Long width = 0;
	Long upWidth = 0;
	Long previousUpWidth = 0;
	Long length = 0;
	Long addWidth = 0;
	Long difference = 0;
	int ascii;
	int newNPos;
	int strSizeY = this->notePadForm->getTextSize->GetX(131);
	int changedNPos;
	Long changedY;

	//1. ���� ����Ű ������ ��
    //1.1. ���� ĳ���� ��ǥ�� ���Ѵ�.
	Long currentY = this->notePadForm->note->GetCurrent();
	Long beforeCurrentY = currentY;
	Glyph* row = this->notePadForm->note->GetChild(currentY);
	Long currentX = row->GetCurrent();

	//1.2. caret�� x�� 0���� �ʱ�ȭ�Ѵ�.
	this->notePadForm->location->caret->x = 0;

	if (currentY > 0) {
		//1.3.1. ���� ��ǥ������ �ʺ� ����Ѵ�.
		while (i < currentX) {
			character = row->GetChild(i);
			strCharacter = character->GetContent();
			strcpy(chaCharacter, strCharacter.c_str());
			if (chaCharacter[0] >= 0 && chaCharacter[0] < 128) {
				chaCharacter[1] = '\0';
			}
			else {
				chaCharacter[2] = '\0';
			}
			//this->notePadForm->location->CalculateX(chaCharacter);
			this->notePadForm->getTextSize->CalculateX(chaCharacter);
			i++;
		}
		width = this->notePadForm->location->caret->GetX();

		//1.3.2. y-1��ǥ���� ���� �ʺ񺸴� �۰ų� ���� ���� �ݺ��Ѵ�.
		currentY--;
		this->notePadForm->location->caret->x = 0;
		this->notePadForm->row = this->notePadForm->note->GetChild(currentY);
		length = this->notePadForm->row->GetLength();
		i = 0;

		if (i < length) {
			character = this->notePadForm->row->GetChild(i);
			strCharacter = character->GetContent();
			strcpy(chaCharacter, strCharacter.c_str());

			if (chaCharacter[0] >= 0 && chaCharacter[0] < 128) {
				chaCharacter[1] = '\0';
			}
			else {
				chaCharacter[2] = '\0';
			}
			//this->notePadForm->location->CalculateX(chaCharacter);
			this->notePadForm->getTextSize->CalculateX(chaCharacter);
			upWidth = this->notePadForm->location->caret->GetX();



			if (upWidth < width && i < length) {
				i = 1;
				while (upWidth < width && i < length) { // upWidth�� �� Ŀ�� ���¿��� �����Եȴ�.

					previousUpWidth = upWidth;

					character = this->notePadForm->row->GetChild(i);
					strCharacter = character->GetContent();
					strcpy(chaCharacter, strCharacter.c_str());

					if (chaCharacter[0] >= 0 && chaCharacter[0] < 128) {
						chaCharacter[1] = '\0';
					}
					else {
						chaCharacter[2] = '\0';
					}
					//this->notePadForm->location->CalculateX(chaCharacter);
					this->notePadForm->getTextSize->CalculateX(chaCharacter);
					upWidth = this->notePadForm->location->caret->GetX();
					i++;
				}

				if (upWidth > width) {
					upWidth = previousUpWidth;
					i--;
				}
			}
			else if (upWidth != width) {
				upWidth = previousUpWidth;
			}
			else if (upWidth == width) {
				i = 1;
			}
		}

		currentX = i;

		//1.3.3. y-1��ǥ�� ���� �ʺ񺸴� ���� ���,
		if (upWidth < width && i < length) {
			//1. y-1��ǥ�� ���� x��ǥ���� +1�� ���� �ʺ� ���Ѵ�.
			//i++;
			character = this->notePadForm->row->GetChild(i);
			strCharacter = character->GetContent();
			strcpy(chaCharacter, strCharacter.c_str());
			//this->notePadForm->location->caret->x = 0;

			if (chaCharacter[0] >= 0 && chaCharacter[0] < 128) { //1����Ʈ��,
				//chaCharacter[1] = '\0';
				ascii = chaCharacter[0];
				addWidth = this->notePadForm->getTextSize->GetX(ascii);

			}
			else {
				addWidth = this->notePadForm->getTextSize->GetX(128);
			}

			//2. y+1��ǥ �� x�ʺ� + ���� �� �ʺ񺸴� ���� �ʺ� �� ũ��, y+1��ǥ�� x��ǥ�� �ϳ� �����ش�.
			if (upWidth + (addWidth / 2) <= width) {
				i++;
				currentX = i; //��ǥ����
				upWidth = upWidth + addWidth; //�ʺ񺯰�
			}
		}

		//1.3.4. currentX, currentY�� ���� current�� �����Ѵ�.
		this->notePadForm->y = currentY;
		this->notePadForm->note->MoveCurrent(currentY);
		this->notePadForm->x = currentX;
		this->notePadForm->row = this->notePadForm->note->GetChild(currentY);
		this->notePadForm->row->MoveCurrent(currentX);
	}

	//(21.11.23.�߰�)4. ���� ��ǥ�� ���� windowHeight ���� �ȿ� ����,
	int firstTextY = this->notePadForm->firstTextY;
	int endTextY = this->notePadForm->endTextY;

	if (beforeCurrentY >= firstTextY && beforeCurrentY <= endTextY) {
		//2.4. ĳ���� �̵��Ѵ�.
		this->notePadForm->isOnImeCharForMove = FALSE;
		this->notePadForm->isFirstComposingForMove = FALSE;

		// Scroll ������Ʈ �� ���� ������ �����Ѵ�.
		this->notePadForm->isJumpOverForPrevious = FALSE;
		this->notePadForm->isJumpOverForNext = FALSE;
		this->notePadForm->isUp = TRUE;
		this->notePadForm->isDown = FALSE;
		this->notePadForm->isDoubleByte = FALSE;

		this->notePadForm->Notify();
	}
	//(21.11.23.�߰�) 2.���� ��ǥ�� firstTextY���� ���� �ִ� ���,
	else if (beforeCurrentY < firstTextY) {

		//2.1. firstTextY�� -2���� nPos �̵��Ѵ�.
		changedY = firstTextY - 1;
		changedNPos = changedY * strSizeY;
		this->notePadForm->scroll->siVertical.nPos = changedNPos;

		//2.2. nPos�� 0���� ������ 0���� ġȯ�Ѵ�.
		if (changedNPos < 0) {
			this->notePadForm->scroll->siVertical.nPos = 0;
			changedY = 0;
		}

		//2.3. ��ǥ�� �̵���Ų��.
		this->notePadForm->note->MoveCurrent(changedY);
		row = this->notePadForm->note->GetChild(changedY);
		length = row->GetLength();

		if (length >= currentX) {
			row->MoveCurrent(currentX);
		}
		else {
			row->MoveCurrent(length);
		}

		//5.3. ��ũ�� ������ set�Ѵ�.
		this->notePadForm->SetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical, TRUE);

		this->notePadForm->ScrollWindow(0, this->notePadForm->scroll->siVertical.nPos, NULL, NULL);
		this->notePadForm->location->MoveCaret();
		this->notePadForm->UpdateWindow();
		this->notePadForm->Invalidate();
	}
	//(21.11.23.�߰�) 3.���� ��ǥ�� endTextY���� �ؿ� �ִ� ���,
	else if (beforeCurrentY > endTextY) {

		//3.1. endTextY -1�� �ٴ��� �ǵ��� nPos�� �����Ѵ�.
		changedY = endTextY + 1;
		this->notePadForm->scroll->siVertical.nPos += strSizeY;

		//3.2. ��ǥ�� �̵����ش�.
		this->notePadForm->note->MoveCurrent(changedY);
		row = this->notePadForm->note->GetChild(changedY);
		length = row->GetLength();

		if (length >= currentX) {
			row->MoveCurrent(currentX);
		}
		else {
			row->MoveCurrent(length);
		}

		//5.3. ��ũ�� ������ set�Ѵ�.
		this->notePadForm->SetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical, TRUE);

		this->notePadForm->ScrollWindow(0, this->notePadForm->scroll->siVertical.nPos, NULL, NULL);
		this->notePadForm->location->MoveCaret();
		this->notePadForm->UpdateWindow();
		this->notePadForm->Invalidate();
	}

}















