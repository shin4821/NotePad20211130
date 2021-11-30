//CopyText.cpp
#include"CopyText.h"
#include"NotePadForm.h"
#include"SelectWords.h"
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
#include<string>
using namespace std;
#pragma warning(disable:4996)

CopyText::CopyText(NotePadForm* notePadForm)
	:KeyAction(notePadForm) {
}

CopyText::~CopyText() {
}

void CopyText::KeyDown() {
    HANDLE hClipboard;
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
    HGLOBAL hGlobal;

//1. Ctrl+ C Ű ��ư ������ ��

    //1.0. �ش� ������ ���� ���, �ش� ������ str�� �ű��.
    Long firstY = this->notePadForm->selectWords->GetFirstY();
    Long firstX = this->notePadForm->selectWords->GetFirstX();
    Long endY = this->notePadForm->selectWords->GetEndY();
    Long endX = this->notePadForm->selectWords->GetEndX();

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
            content = CString(content_.c_str());

            str.Append(content);

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
        if (i < endY && nextRow!=NULL && !dynamic_cast<DummyRow*>(nextRow)) {  //i < endY && !dynamic_cast<DummyRow*>(row)
            str.Append("\n");
        }
        //*************************************************************************************

        i++;
    }

    //1.1. Ŭ������ �ڵ��� ����.  
    if (this->notePadForm->OpenClipboard()) {//OpenClipboard(this->notePadForm->GetSafeHwnd())

        //1. Ŭ�����带 ����.
        EmptyClipboard();

        //2. Ŭ�����忡 ���� �޸𸮸� �Ҵ��Ѵ�.(�޸� �ּҸ� ��´�)
        hGlobal = GlobalAlloc(GHND | GMEM_SHARE, str.GetLength()+1);

        //3. �ش� �ڵ鿡 ���ڿ��� �����Ѵ�.
        PSTR pGlobal = (PSTR)GlobalLock(hGlobal);
        _tcscpy(pGlobal, (LPCTSTR)str);
        GlobalUnlock(hGlobal);

        //4. Ŭ�����忡 �����͸� �ִ´�. 
        SetClipboardData(CF_TEXT, hGlobal);

        //5. Ŭ�����带 �ݴ´�.
        CloseClipboard();
    }
}















