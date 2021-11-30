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

//1. Ctrl+ C 키 버튼 눌렀을 때

    //1.0. 해당 범위가 있을 경우, 해당 범위를 str에 옮긴다.
    Long firstY = this->notePadForm->selectWords->GetFirstY();
    Long firstX = this->notePadForm->selectWords->GetFirstX();
    Long endY = this->notePadForm->selectWords->GetEndY();
    Long endX = this->notePadForm->selectWords->GetEndX();

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
            content = CString(content_.c_str());

            str.Append(content);

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
        if (i < endY && nextRow!=NULL && !dynamic_cast<DummyRow*>(nextRow)) {  //i < endY && !dynamic_cast<DummyRow*>(row)
            str.Append("\n");
        }
        //*************************************************************************************

        i++;
    }

    //1.1. 클립보드 핸들을 연다.  
    if (this->notePadForm->OpenClipboard()) {//OpenClipboard(this->notePadForm->GetSafeHwnd())

        //1. 클립보드를 비운다.
        EmptyClipboard();

        //2. 클립보드에 넣을 메모리를 할당한다.(메모리 주소를 얻는다)
        hGlobal = GlobalAlloc(GHND | GMEM_SHARE, str.GetLength()+1);

        //3. 해당 핸들에 문자열을 대입한다.
        PSTR pGlobal = (PSTR)GlobalLock(hGlobal);
        _tcscpy(pGlobal, (LPCTSTR)str);
        GlobalUnlock(hGlobal);

        //4. 클립보드에 데이터를 넣는다. 
        SetClipboardData(CF_TEXT, hGlobal);

        //5. 클립보드를 닫는다.
        CloseClipboard();
    }
}















