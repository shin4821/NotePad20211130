//FirstDoc.cpp
#include"FirstDoc.h"
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

FirstDoc::FirstDoc(NotePadForm* notePadForm)
	:KeyAction(notePadForm) {
}

FirstDoc::~FirstDoc() {
}

void FirstDoc::KeyDown() {
	//2. Ctrl+ home 키 버튼 눌렀을 때
	//2.0. 현재 note에서 First를 호출한다.
	this->notePadForm->note->First();
	//2.1. 현재 note의 current 확인한다.
	Long currentY = this->notePadForm->note->GetCurrent();
	//2.2. 현재 note의 row를 찾는다.
	this->notePadForm->row = this->notePadForm->note->GetChild(currentY);
	//2.3. 현재 row에서 First를 호출한다.
	this->notePadForm->row->First();

	//2.4. 캐럿을 이동한다.
	this->notePadForm->isOnImeCharForMove = FALSE;
	this->notePadForm->isFirstComposingForMove = FALSE;
#if 0
	// firstXY, endXY를 초기화한다.
	this->notePadForm->selectWords->firstX = 0;
	this->notePadForm->selectWords->firstY = 0;
	this->notePadForm->selectWords->endX = 0;
	this->notePadForm->selectWords->endY = 0;
#endif

	// Scroll 업데이트 할 때의 조건을 설정한다.
	this->notePadForm->isJumpOverForPrevious = FALSE;
	this->notePadForm->isJumpOverForNext = FALSE;
	this->notePadForm->isUp = FALSE;
	this->notePadForm->isDown = FALSE;
	this->notePadForm->isDoubleByte = FALSE;


	this->notePadForm->Notify();
}














