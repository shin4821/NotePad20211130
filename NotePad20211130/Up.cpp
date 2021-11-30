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

	//1. 위쪽 방향키 눌렀을 때
    //1.1. 현재 캐럿의 좌표를 구한다.
	Long currentY = this->notePadForm->note->GetCurrent();
	Long beforeCurrentY = currentY;
	Glyph* row = this->notePadForm->note->GetChild(currentY);
	Long currentX = row->GetCurrent();

	//1.2. caret의 x를 0으로 초기화한다.
	this->notePadForm->location->caret->x = 0;

	if (currentY > 0) {
		//1.3.1. 현재 좌표까지의 너비를 계산한다.
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

		//1.3.2. y-1좌표에서 현재 너비보다 작거나 같을 동안 반복한다.
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
				while (upWidth < width && i < length) { // upWidth가 더 커진 상태에서 나오게된다.

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

		//1.3.3. y-1좌표가 현재 너비보다 작은 경우,
		if (upWidth < width && i < length) {
			//1. y-1좌표의 현재 x좌표에서 +1한 문자 너비를 구한다.
			//i++;
			character = this->notePadForm->row->GetChild(i);
			strCharacter = character->GetContent();
			strcpy(chaCharacter, strCharacter.c_str());
			//this->notePadForm->location->caret->x = 0;

			if (chaCharacter[0] >= 0 && chaCharacter[0] < 128) { //1바이트면,
				//chaCharacter[1] = '\0';
				ascii = chaCharacter[0];
				addWidth = this->notePadForm->getTextSize->GetX(ascii);

			}
			else {
				addWidth = this->notePadForm->getTextSize->GetX(128);
			}

			//2. y+1좌표 총 x너비 + 문자 반 너비보다 현재 너비가 더 크면, y+1좌표의 x좌표를 하나 더해준다.
			if (upWidth + (addWidth / 2) <= width) {
				i++;
				currentX = i; //좌표변경
				upWidth = upWidth + addWidth; //너비변경
			}
		}

		//1.3.4. currentX, currentY를 현재 current로 설정한다.
		this->notePadForm->y = currentY;
		this->notePadForm->note->MoveCurrent(currentY);
		this->notePadForm->x = currentX;
		this->notePadForm->row = this->notePadForm->note->GetChild(currentY);
		this->notePadForm->row->MoveCurrent(currentX);
	}

	//(21.11.23.추가)4. 현재 좌표가 현재 windowHeight 범위 안에 들어가면,
	int firstTextY = this->notePadForm->firstTextY;
	int endTextY = this->notePadForm->endTextY;

	if (beforeCurrentY >= firstTextY && beforeCurrentY <= endTextY) {
		//2.4. 캐럿을 이동한다.
		this->notePadForm->isOnImeCharForMove = FALSE;
		this->notePadForm->isFirstComposingForMove = FALSE;

		// Scroll 업데이트 할 때의 조건을 설정한다.
		this->notePadForm->isJumpOverForPrevious = FALSE;
		this->notePadForm->isJumpOverForNext = FALSE;
		this->notePadForm->isUp = TRUE;
		this->notePadForm->isDown = FALSE;
		this->notePadForm->isDoubleByte = FALSE;

		this->notePadForm->Notify();
	}
	//(21.11.23.추가) 2.현재 좌표가 firstTextY보다 위에 있는 경우,
	else if (beforeCurrentY < firstTextY) {

		//2.1. firstTextY의 -2까지 nPos 이동한다.
		changedY = firstTextY - 1;
		changedNPos = changedY * strSizeY;
		this->notePadForm->scroll->siVertical.nPos = changedNPos;

		//2.2. nPos가 0보다 작으면 0으로 치환한다.
		if (changedNPos < 0) {
			this->notePadForm->scroll->siVertical.nPos = 0;
			changedY = 0;
		}

		//2.3. 좌표를 이동시킨다.
		this->notePadForm->note->MoveCurrent(changedY);
		row = this->notePadForm->note->GetChild(changedY);
		length = row->GetLength();

		if (length >= currentX) {
			row->MoveCurrent(currentX);
		}
		else {
			row->MoveCurrent(length);
		}

		//5.3. 스크롤 정보를 set한다.
		this->notePadForm->SetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical, TRUE);

		this->notePadForm->ScrollWindow(0, this->notePadForm->scroll->siVertical.nPos, NULL, NULL);
		this->notePadForm->location->MoveCaret();
		this->notePadForm->UpdateWindow();
		this->notePadForm->Invalidate();
	}
	//(21.11.23.추가) 3.현재 좌표가 endTextY보다 밑에 있는 경우,
	else if (beforeCurrentY > endTextY) {

		//3.1. endTextY -1이 바닥이 되도록 nPos를 설정한다.
		changedY = endTextY + 1;
		this->notePadForm->scroll->siVertical.nPos += strSizeY;

		//3.2. 좌표를 이동해준다.
		this->notePadForm->note->MoveCurrent(changedY);
		row = this->notePadForm->note->GetChild(changedY);
		length = row->GetLength();

		if (length >= currentX) {
			row->MoveCurrent(currentX);
		}
		else {
			row->MoveCurrent(length);
		}

		//5.3. 스크롤 정보를 set한다.
		this->notePadForm->SetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical, TRUE);

		this->notePadForm->ScrollWindow(0, this->notePadForm->scroll->siVertical.nPos, NULL, NULL);
		this->notePadForm->location->MoveCaret();
		this->notePadForm->UpdateWindow();
		this->notePadForm->Invalidate();
	}

}















