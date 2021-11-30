//EndDocKeyAction.cpp
#include"EndDocKeyAction.h"
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


EndDocKeyAction::EndDocKeyAction(NotePadForm* notePadForm)
	:KeyAction(notePadForm) {
}
EndDocKeyAction::~EndDocKeyAction() {
}
void EndDocKeyAction::KeyDown() {
	Glyph* character;
	int caretX = 0;
	int length;
	string strCharacter;
	char chaCharacter[3];


	//21.08.04 추가
	Long beforeCurrentY = this->notePadForm->note->GetCurrent();


	//2. Ctrl+ home 키 버튼 눌렀을 때
	//2.0. 현재 note에서 First를 호출한다.
	this->notePadForm->note->Last();

	//2.1. 현재 note의 current 확인한다.
	Long currentY = this->notePadForm->note->GetCurrent();
	//2.2. 현재 note의 row를 찾는다.
	this->notePadForm->row = this->notePadForm->note->GetChild(currentY);
	//2.3. 현재 row에서 First를 호출한다.
	this->notePadForm->row->Last();
	Long currentX = this->notePadForm->row->GetCurrent();
	currentX++;
	this->notePadForm->row->MoveCurrent(currentX);

#if 0
	int i = 0;
	while (i < currentX) {
		//2.6.1. i번째 문자를 얻는다.
		character = this->notePadForm->row->GetChild(i);
		strCharacter = character->GetContent();
		strcpy(chaCharacter, strCharacter.c_str());

		if (chaCharacter[0] >= 0 && chaCharacter[0] < 128) {
			chaCharacter[1] = '\0';
		}
		else {
			chaCharacter[2] = '\0';
		}

		//2.6.2. Location의 CalculateX를 호출한다.
		this->notePadForm->getTextSize->CalculateX(chaCharacter);
		//CalculateX(chaCharacter);
		i++;
	}

	caretX = this->notePadForm->location->caret->GetX();
#endif



//#if 0
	Long totalCaretX = this->notePadForm->getTextSize->totalCalculateX(currentX, this->notePadForm->row);

#if 0
	if (totalCaretX > this->notePadForm->scroll->windowWidth) {
		this->notePadForm->scroll->siHorizon.nPos = totalCaretX - this->notePadForm->scroll->windowWidth;
	}
#endif

	//(21.11.24.추가)2.2.1. Down후 캐럿까지의 물리적 너비가 바꾸기 전 Horizon의 nPos보다 작은 경우,
	if (totalCaretX < this->notePadForm->scroll->siHorizon.nPos) {
		//2.2.1.1. Down후 캐럿까지의 물리적 너비에서 윈도우 3분의 1만큼 뺀다.
		this->notePadForm->scroll->siHorizon.nPos = totalCaretX - this->notePadForm->scroll->windowWidth / 3;
		if (this->notePadForm->scroll->siHorizon.nPos < 0) {
			this->notePadForm->scroll->siHorizon.nPos = 0;
		}
	}

	// 스크롤 정보를 set한다.
	this->notePadForm->SetScrollInfo(SB_HORZ, &this->notePadForm->scroll->siHorizon, TRUE);
//#endif

	//2.4. 캐럿을 옮긴다.
	this->notePadForm->isOnImeCharForMove = FALSE;
	this->notePadForm->isFirstComposingForMove = FALSE;


	// Scroll 업데이트 할 때의 조건을 설정한다.
	this->notePadForm->isJumpOverForPrevious = FALSE;
	this->notePadForm->isJumpOverForNext = FALSE;
	this->notePadForm->isUp = FALSE;
	this->notePadForm->isDown = FALSE;
	this->notePadForm->isDoubleByte = FALSE;
	this->notePadForm->isEndDoc = TRUE;


	this->notePadForm->Notify();
}