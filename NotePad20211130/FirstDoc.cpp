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
	//2. Ctrl+ home Ű ��ư ������ ��
	//2.0. ���� note���� First�� ȣ���Ѵ�.
	this->notePadForm->note->First();
	//2.1. ���� note�� current Ȯ���Ѵ�.
	Long currentY = this->notePadForm->note->GetCurrent();
	//2.2. ���� note�� row�� ã�´�.
	this->notePadForm->row = this->notePadForm->note->GetChild(currentY);
	//2.3. ���� row���� First�� ȣ���Ѵ�.
	this->notePadForm->row->First();

	//2.4. ĳ���� �̵��Ѵ�.
	this->notePadForm->isOnImeCharForMove = FALSE;
	this->notePadForm->isFirstComposingForMove = FALSE;
#if 0
	// firstXY, endXY�� �ʱ�ȭ�Ѵ�.
	this->notePadForm->selectWords->firstX = 0;
	this->notePadForm->selectWords->firstY = 0;
	this->notePadForm->selectWords->endX = 0;
	this->notePadForm->selectWords->endY = 0;
#endif

	// Scroll ������Ʈ �� ���� ������ �����Ѵ�.
	this->notePadForm->isJumpOverForPrevious = FALSE;
	this->notePadForm->isJumpOverForNext = FALSE;
	this->notePadForm->isUp = FALSE;
	this->notePadForm->isDown = FALSE;
	this->notePadForm->isDoubleByte = FALSE;


	this->notePadForm->Notify();
}














