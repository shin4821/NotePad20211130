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
	char characterMove[3]; //Observe Pattern 때문에.
	int newNPos;

	//1. 왼쪽 키 버튼 눌렀을 때

	//(21.08.03 추가)1.1. 현재 Y좌표를 구한다.
	Long beforeCurrentY = this->notePadForm->note->GetCurrent();

	//(21.11.22.추가) 1.2. 현재 자동개행인 경우, 
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		//1.2.1. 자동개행 전의 캐럿의 위치를 반환받는다.
		this->notePadForm->lineChange->FindCaretBeforeLineChange(&currentX, &currentY);

		//1.2.2. 자동개행을 풀어준다.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

		//1.2.3. 캐럿을 옮겨준다.
		this->notePadForm->note->MoveCurrent(currentY);
		row = this->notePadForm->note->GetChild(currentY);
		row->MoveCurrent(currentX);
	}

	//1.3. 현재 note에서 Next 출력한다.
	this->notePadForm->note->PreviousWord();

	//1.4. 현재 좌표를 확인한다.
	currentY = this->notePadForm->note->GetCurrent();
	row = this->notePadForm->note->GetChild(currentY);
	currentX = row->GetCurrent();

	//(21.11.22.추가) 1.5. 현재 자동개행인 경우, 
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		//1.5.1. 자동개행 해준다.
		this->notePadForm->lineChange->LineChangeButtonClicked();

		//1.5.2. 자동개행 후 이동할 캐럿의 위치를 구한다.
		this->notePadForm->lineChange->FindCaretAfterLineChange(currentX, currentY);
	}
	
	//1.6. Scroll 업데이트 할 때의 조건을 설정한다.
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

















