//SelectWords.cpp
#include"Glyph.h"
#include"SelectWords.h"
#include"NotePadForm.h"
#include"GetTextSize.h"
#include"Scroll.h"
#include"LineChange.h"
#include"DummyRow.h"

SelectWords::SelectWords(NotePadForm* notePadForm) {
	this->notePadForm = notePadForm;
	this->isDragging = FALSE;
	this->firstX = 0;
	this->firstY = 0;
	this->endX = 0;
	this->endY = 0;
    this->beforeFirstX = 0;
    this->beforeFirstY = 0;
    this->beforeEndX = 0;
    this->beforeEndY = 0;
    this->afterEndX = 0;
    this->afterEndY = 0;
    this->isReplaced = FALSE;
    this->isLineChangeButtonClicked = FALSE;
}

SelectWords::~SelectWords() {
}

Long SelectWords::GetCurrentX(Long x, Glyph* row) {
	Long currentX = 0;
	Long caretX;
	int i = 0;

	//1. 물리적 x를 입력받는다.
	//2. 물리적 x보다 작거나 같은 동안 반복한다.

	caretX = this->notePadForm->getTextSize->totalCalculateX(i, row);
	i++;

	while (caretX < x) {
		caretX = this->notePadForm->getTextSize->totalCalculateX(i, row);
		i++;
	}
	i--;

	return i;
}

Long SelectWords::GetCurrentY(Long y) {
	int currentY = 0;
	int strSizeY = this->notePadForm->getTextSize->GetX(131);

	//1. 물리적 y를 입력받는다.
	//2. currentY가 물리적 y보다 작거나 같은동안 반복한다.
	while ((strSizeY * currentY) - this->notePadForm->scroll->siVertical.nPos < y) {
		currentY++;
	}

	return currentY;
}

int SelectWords::GetFirstX() {
	return this->firstX;
}

int SelectWords::GetFirstY() {
	return this->firstY;
}

int SelectWords::GetEndX() {
	return this->endX;
}

int SelectWords::GetEndY() {
	return this->endY;
}

string SelectWords::GetSelectedWord() { //현재 선택된 단어를 출력한다.
    BOOL isBigger = TRUE;
    CString str;
    Glyph* row;
    Glyph* character;
    Glyph* nextRow;
    int nSize;
    int limitX;
    int i;
    int j;
    string content_;
    CString content;
    string selectedWord;

    //1.0. 해당 범위가 있을 경우, 해당 범위를 str에 옮긴다.
    Long firstY = GetFirstY();
    Long firstX = GetFirstX();
    Long endY = GetEndY();
    Long endX = GetEndX();

    //좌표가 isBigger인지 확인한다.
    if (firstY < endY) {
        isBigger = TRUE;
    }
    else if (firstY > endY) {
        isBigger = FALSE;
    }
    else if (firstY == endY) {
        if (firstX < endX) {
            isBigger = TRUE;
        }
        else if (firstX > endX) {
            isBigger = FALSE;
        }
    }

    // isBigger이 아니면 좌표를 서로 바꾼다.
    if (isBigger == FALSE) {
        int startX = firstX;
        int startY = firstY;
        int lastX = endX;
        int lastY = endY;

        firstX = lastX;
        firstY = lastY;
        endX = startX;
        endY = startY;
    }

    i = firstY;
    while (i <= endY) {
        row = this->notePadForm->note->GetChild(i);
        //rowLength = row->GetLength();

        //1. j를 결정한다.
        if (i <= firstY) {
            j = firstX;
        }
        else {
            j = 0;
        }

        //2. limitX 를 결정한다.
        if (i >= endY) {
            //rowLength = endX;
            limitX = endX;
        }
        else {
            limitX = row->GetLength();
        }

        while (j < limitX) {
            character = row->GetChild(j);
            content_ = character->GetContent();
           
            selectedWord.append(content_);

            j++;
        }

        // nextRow가 Row이면 개행문자를 더한다. **********************************************
        if (i + 1 < this->notePadForm->note->GetLength()) {
            nextRow = this->notePadForm->note->GetChild(i + 1);
        }
        else {
            nextRow = NULL;
        }
        // 그 다음 줄이면 개행문자를 더한다. (마지막줄 아니고, DummyRow가 아닌 경우,)
        if (i < endY && nextRow != NULL && !dynamic_cast<DummyRow*>(nextRow)) {  //i < endY && !dynamic_cast<DummyRow*>(row)
            selectedWord.append("\n");
        }
        //*************************************************************************************

        i++;
    }

    return selectedWord;
}
















