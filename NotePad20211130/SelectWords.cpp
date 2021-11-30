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

	//1. ������ x�� �Է¹޴´�.
	//2. ������ x���� �۰ų� ���� ���� �ݺ��Ѵ�.

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

	//1. ������ y�� �Է¹޴´�.
	//2. currentY�� ������ y���� �۰ų� �������� �ݺ��Ѵ�.
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

string SelectWords::GetSelectedWord() { //���� ���õ� �ܾ ����Ѵ�.
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

    //1.0. �ش� ������ ���� ���, �ش� ������ str�� �ű��.
    Long firstY = GetFirstY();
    Long firstX = GetFirstX();
    Long endY = GetEndY();
    Long endX = GetEndX();

    //��ǥ�� isBigger���� Ȯ���Ѵ�.
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

    // isBigger�� �ƴϸ� ��ǥ�� ���� �ٲ۴�.
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

        //1. j�� �����Ѵ�.
        if (i <= firstY) {
            j = firstX;
        }
        else {
            j = 0;
        }

        //2. limitX �� �����Ѵ�.
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

        // nextRow�� Row�̸� ���๮�ڸ� ���Ѵ�. **********************************************
        if (i + 1 < this->notePadForm->note->GetLength()) {
            nextRow = this->notePadForm->note->GetChild(i + 1);
        }
        else {
            nextRow = NULL;
        }
        // �� ���� ���̸� ���๮�ڸ� ���Ѵ�. (�������� �ƴϰ�, DummyRow�� �ƴ� ���,)
        if (i < endY && nextRow != NULL && !dynamic_cast<DummyRow*>(nextRow)) {  //i < endY && !dynamic_cast<DummyRow*>(row)
            selectedWord.append("\n");
        }
        //*************************************************************************************

        i++;
    }

    return selectedWord;
}
















