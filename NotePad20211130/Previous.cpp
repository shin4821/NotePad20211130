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
	char characterMove[3]; //Observe Pattern 때문에.
	int newNPos;
	int changedNPos;

	//1. 왼쪽 키 버튼 눌렀을 때
		//1.1. 커서 크기가 한글의 두께가 아니면, 현재 note에서 Previous 호출한다.
		DoubleByteCaretWidth = this->notePadForm->getTextSize->GetX(128);

		//1.2. Previous 호출 전 캐럿Y의 좌표를 구해놓는다.
		Long beforeCurrentY = this->notePadForm->note->GetCurrent();

		//(21.11.15.추가)1.3. Previous 이동하기 전 줄이 DummyRow인지 확인한다.
		beforeRow = this->notePadForm->note->GetChild(beforeCurrentY);

		//1.3. Previous를 호출한다.
		if (this->notePadForm->caretWidth != DoubleByteCaretWidth) {
			this->notePadForm->note->Previous();
		}

		//1.2. 현재 note의 current 확인한다.
		currentY = this->notePadForm->note->GetCurrent();
		//1.3. 현재 note의 row를 찾는다.
		row = this->notePadForm->note->GetChild(currentY);
		//1.4. 현재 row의 current 확인한다.
		currentX = row->GetCurrent();


		//1.4. 자동개행일 경우,
		if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

			length = row->GetLength();

			//1.4.1. 현재 캐럿의 x좌표가 row의 length이고, 이동하기 전 줄이 DummyRow인 경우,
			if (dynamic_cast<DummyRow*>(beforeRow) && currentX == length) {
				//현재 줄의 endX-1로 이동한다.
				currentX = length - 1;
				row->MoveCurrent(currentX);
			}
		}

		if (this->notePadForm->caretWidth == DoubleByteCaretWidth) {//현재 커서크기가 글자크기면 x를 1씩 뺀다.
			currentX--;
			row->MoveCurrent(currentX);
		}

		//(21.11.23.추가)4. 현재 좌표가 현재 windowHeight 범위 안에 들어가면,
		int firstTextY = this->notePadForm->firstTextY;
		int endTextY = this->notePadForm->endTextY;
		currentY = this->notePadForm->note->GetCurrent();

		if (beforeCurrentY >= firstTextY && beforeCurrentY <= endTextY) {
			//2.4. 캐럿을 이동한다.
			this->notePadForm->isOnImeCharForMove = FALSE;
			this->notePadForm->isFirstComposingForMove = FALSE;

			// Scroll 업데이트 할 때의 조건을 설정한다.
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
		//(21.11.23.추가) 5.현재 좌표가 firstTextY보다 위에 있는 경우,
		else if (beforeCurrentY < firstTextY) {
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
			int changedCurrentY = currentY - difference;
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























