//FactoryProduct.cpp
#include"NotePadForm.h"
#include"FactoryProduct.h"
#include"Previous.h"
#include"Next.h"
#include"PreviousWord.h"
#include"NextWord.h"
#include"FirstRow.h"
#include"EndRow.h"
#include"FirstDoc.h"
#include"EndDocKeyAction.h"
#include"Up.h"
#include"Down.h"
#include"BackSpaceKeyAction.h"
#include"DeleteKeyAction.h"
#include"SelectNext.h"
#include"SelectPrevious.h"
#include"SelectUp.h"
#include"SelectDown.h"
#include"SelectPreviousWord.h"
#include"SelectNextWord.h"
#include"SelectFirstRow.h"
#include"SelectEndRow.h"
#include"SelectFirstDoc.h"
#include"SelectEndDoc.h"
#include"CopyText.h"
#include"PasteText.h"
#include"CutOffText.h"
#include"SelectWords.h"
#include"PreviousForSelected.h"
#include"NextForSelected.h"

FactoryProduct::FactoryProduct(NotePadForm* notePadForm) {
	this->notePadForm = notePadForm;
}

FactoryProduct::~FactoryProduct() {
}

KeyAction* FactoryProduct::Make(UINT nCharFirst, UINT nCharSecond, UINT nCharThird) {
	KeyAction* keyAction = 0;
	BOOL isSelected = FALSE;

	//(21.11.22.추가)1.4. 현재 선택된 단어가 있는지 확인한다. *********************************************
	Long firstY = this->notePadForm->selectWords->GetFirstY();
	Long firstX = this->notePadForm->selectWords->GetFirstX();
	Long endY = this->notePadForm->selectWords->GetEndY();
	Long endX = this->notePadForm->selectWords->GetEndX();

	if (firstX > 0 || endX > 0) {
		isSelected = TRUE;
	}


	if (nCharFirst == VK_LEFT && nCharSecond==NULL && nCharThird == NULL && isSelected == FALSE) {
		keyAction = new Previous(this->notePadForm);
	}
	//(21.11.22.추가) 선택된 단어가 있으면, 
	else if (nCharFirst == VK_LEFT && nCharSecond == NULL && nCharThird == NULL && isSelected == TRUE) {
		keyAction = new PreviousForSelected(this->notePadForm);
	}

	//----------------------------------------------------------------------------------------------------
	//21.08.17 추가
	else if (nCharFirst == VK_LEFT && nCharSecond == VK_SHIFT && nCharThird == NULL) {
		keyAction = new SelectPrevious(this->notePadForm);
	}

	//21.08.17 추가
	else if (nCharFirst == VK_RIGHT && nCharSecond == VK_SHIFT && nCharThird == NULL) {
		keyAction = new SelectNext(this->notePadForm);
	}

	//21.08.18 추가
	else if ((nCharFirst == VK_LEFT && nCharSecond == VK_CONTROL && nCharThird == VK_SHIFT)) {
		keyAction = new SelectPreviousWord(this->notePadForm);
	}

	//21.08.18 추가
	else if ((nCharFirst == VK_RIGHT && nCharSecond == VK_CONTROL && nCharThird == VK_SHIFT)) {
		keyAction = new SelectNextWord(this->notePadForm);
	}

	//21.08.18 추가
	else if ((nCharFirst == VK_HOME && nCharSecond == VK_SHIFT && nCharThird == NULL)) {
		keyAction = new SelectFirstRow(this->notePadForm);
	}

	//21.08.18 추가
	else if ((nCharFirst == VK_END && nCharSecond == VK_SHIFT && nCharThird == NULL)) {
		keyAction = new SelectEndRow(this->notePadForm);
	}

	//21.08.18 추가
	else if ((nCharFirst == VK_HOME && nCharSecond == VK_CONTROL && nCharThird == VK_SHIFT)) {
		keyAction = new SelectFirstDoc(this->notePadForm);
	}

	//21.08.18 추가
	else if ((nCharFirst == VK_END && nCharSecond == VK_CONTROL && nCharThird == VK_SHIFT)) {
		keyAction = new SelectEndDoc(this->notePadForm);
	}

	//21.08.17 추가
	else if (nCharFirst == VK_UP && nCharSecond == VK_SHIFT && nCharThird == NULL) {
		keyAction = new SelectUp(this->notePadForm);
	}

	//21.08.17 추가
	else if (nCharFirst == VK_DOWN && nCharSecond == VK_SHIFT && nCharThird == NULL) {
		keyAction = new SelectDown(this->notePadForm);
	}
	//----------------------------------------------------------------------------------------------------

	else if (nCharFirst == VK_RIGHT && nCharSecond == NULL && nCharThird == NULL && isSelected == FALSE) {
		keyAction = new Next(this->notePadForm);
	}
	//(21.11.22.추가) 선택된 단어가 있으면, 
	else if (nCharFirst == VK_RIGHT && nCharSecond == NULL && nCharThird == NULL && isSelected == TRUE) {
		keyAction = new NextForSelected(this->notePadForm);
	}

	else if ((nCharFirst == VK_LEFT && nCharSecond == VK_CONTROL && nCharThird == NULL)) {
		keyAction = new PreviousWord(this->notePadForm);
	}


	else if ((nCharFirst == VK_RIGHT && nCharSecond == VK_CONTROL && nCharThird == NULL)) {
		keyAction = new NextWord(this->notePadForm);
	}

	else if (nCharFirst == VK_HOME && nCharSecond == NULL && nCharThird == NULL) {
		keyAction = new FirstRow(this->notePadForm);
	}

	else if (nCharFirst == VK_END && nCharSecond == NULL && nCharThird == NULL) {
		keyAction = new EndRow(this->notePadForm);
	}

	else if ((nCharFirst == VK_HOME && nCharSecond == VK_CONTROL && nCharThird == NULL)) {
		keyAction = new FirstDoc(this->notePadForm);
	}

	else if ((nCharFirst == VK_END && nCharSecond == VK_CONTROL && nCharThird == NULL)) {
		keyAction = new EndDocKeyAction(this->notePadForm);
	}
	else if ((nCharFirst == VK_UP && nCharSecond == NULL && nCharThird == NULL)) {
		keyAction = new Up(this->notePadForm);
	}

	else if ((nCharFirst == VK_DOWN && nCharSecond == NULL && nCharThird == NULL)) {
		keyAction = new Down(this->notePadForm);
	}

	else if ((nCharFirst == VK_BACK && nCharSecond == NULL && nCharThird == NULL)) {
		keyAction = new BackSpaceKeyAction(this->notePadForm);
	}
	else if ((nCharFirst == VK_DELETE && nCharSecond == NULL && nCharThird == NULL)) {
		keyAction = new DeleteKeyAction(this->notePadForm);
	}
	//(21.08.20) 복사하다 추가.
	else if ((nCharFirst == 0x43 && nCharSecond == VK_CONTROL && nCharThird == NULL)) {
		keyAction = new CopyText(this->notePadForm);
	}
	//(21.08.20) 붙혀넣다 추가.
	else if ((nCharFirst == 0x56 && nCharSecond == VK_CONTROL && nCharThird == NULL)) {
		keyAction = new PasteText(this->notePadForm);
	}
	//(21.08.24) 잘라내다 추가.
	else if ((nCharFirst == 0x58 && nCharSecond == VK_CONTROL && nCharThird == NULL)) {
		keyAction = new CutOffText(this->notePadForm);
	}



	return keyAction;
}











