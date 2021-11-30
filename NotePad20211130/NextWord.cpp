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

	//2. 오른쪽 키 버튼 눌렀을 때

	//(21.08.04 추가) 2.1. 현재 Y좌표를 구한다.
	Long beforeCurrentY = this->notePadForm->note->GetCurrent();
	

	//(21.11.22.추가) 2.2. 현재 자동개행인 경우, 
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		//2.2.1. 자동개행 전의 캐럿의 위치를 반환받는다.
		this->notePadForm->lineChange->FindCaretBeforeLineChange(&currentX, &currentY);

		//2.2.2. 자동개행을 풀어준다.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

		//2.2.3. 캐럿을 옮겨준다.
		this->notePadForm->note->MoveCurrent(currentY);
		row = this->notePadForm->note->GetChild(currentY);
		row->MoveCurrent(currentX);
	}

	//2.3. 현재 note에서 Next 출력한다.
	this->notePadForm->note->NextWord();


	//2.4. 캐럿을 이동한다.
	this->notePadForm->isOnImeCharForMove = FALSE;
	this->notePadForm->isFirstComposingForMove = FALSE;

    //2.5. 현재 좌표를 확인한다.
	currentY = this->notePadForm->note->GetCurrent();
	row = this->notePadForm->note->GetChild(currentY);
	currentX = row->GetCurrent();

	//(21.11.22.추가) 2.6. 현재 자동개행인 경우, 
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		//2.6.1. 자동개행 해준다.
		this->notePadForm->lineChange->LineChangeButtonClicked();

		//2.6.2. 자동개행 후 이동할 캐럿의 위치를 구한다.
		this->notePadForm->lineChange->FindCaretAfterLineChange(currentX, currentY);
	}

	//2.7. Scroll 업데이트 할 때의 조건을 설정한다.
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

