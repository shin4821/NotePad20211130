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

	//1. 현재 note에서 Next 출력한다.
	this->notePadForm->note->Next();

	//2. 현재 좌표를 확인한다.
	Long currentY = this->notePadForm->note->GetCurrent();
	row = this->notePadForm->note->GetChild(currentY);
	Long currentX = row->GetCurrent();

	//(21.11.15.추가) 자동 개행된 줄에 대해 좌우 방향키에 의한 행간 이동 오류 방지용
	//3. 현재 자동개행인 경우,
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
		//3.1. 현재 캐럿의 x좌표가 row의 length이고, 그 다음줄이 NULL이 아니고 DummyRow인 경우,
		length = row->GetLength();
		noteLength = this->notePadForm->note->GetLength();
		nextRow = this->notePadForm->note->GetChild(currentY + 1);

		if (currentY + 1 < noteLength && currentX == length && dynamic_cast<DummyRow*>(nextRow)) {
			//그 다음줄의 0번째로 캐럿을 이동한다.
			this->notePadForm->note->MoveCurrent(currentY + 1);
			nextRow->MoveCurrent(0);
		}
	}

	//(21.11.23.추가)4. 현재 좌표가 현재 windowHeight 범위 안에 들어가면,
	int firstTextY = this->notePadForm->firstTextY;
	int endTextY = this->notePadForm->endTextY;
	currentY = this->notePadForm->note->GetCurrent();

	if (beforeCurrentY >= firstTextY && beforeCurrentY <= endTextY) {
		//4.1. 캐럿을 이동한다.
		this->notePadForm->isOnImeCharForMove = FALSE;
		this->notePadForm->isFirstComposingForMove = FALSE;

		// Scroll 업데이트 할 때의 조건을 설정한다.
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
	//(21.11.23.추가) 5.현재 좌표가 firstTextY보다 위에 있는 경우,
	else if(beforeCurrentY < firstTextY){

		//5.1. 0에서 Y좌표-1까지로 nPos를 변경해준다.
		changedNPos = strSizeY * currentY;
		this->notePadForm->scroll->siVertical.nPos = changedNPos;

		//5.2. nPos가 0보다 작으면 0으로 치환한다.
		if (changedNPos < 0) {
			this->notePadForm->scroll->siVertical.nPos = 0;
		}

		//5.3. 스크롤 정보를 set한다.
		this->notePadForm->SetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical, TRUE);

		this->notePadForm->ScrollWindow(0, this->notePadForm->scroll->siVertical.nPos, NULL, NULL);
		this->notePadForm->location->MoveCaret();
		this->notePadForm->UpdateWindow();
		this->notePadForm->Invalidate();

	}
	//(21.11.23.추가) 6.현재 좌표가 endTextY보다 밑에 있는 경우,
	else if (beforeCurrentY > endTextY) {

		//6.1. 현재 y좌표와 firstTextY와의 차이를 구한다.
		int difference = endTextY - firstTextY;
		int changedCurrentY= currentY - difference;
		changedNPos = strSizeY * changedCurrentY;

		//6.2. nPos를 변경해준다.
		this->notePadForm->scroll->siVertical.nPos = changedNPos;

		//6.3. 스크롤 정보를 set한다.
		this->notePadForm->SetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical, TRUE);

		this->notePadForm->ScrollWindow(0, this->notePadForm->scroll->siVertical.nPos, NULL, NULL);
		this->notePadForm->location->MoveCaret();
		this->notePadForm->UpdateWindow();
		this->notePadForm->Invalidate();
	}



}
