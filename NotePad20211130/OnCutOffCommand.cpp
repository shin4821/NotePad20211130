//OnCutOffCommand.cpp
#include"OnCutOffCommand.h"
#include"NotePadForm.h"
#include"SelectWords.h"
#include"Note.h"
#include"DummyRow.h"
#include"KeyAction.h"
#include"FactoryProduct.h"
#include"Scroll.h"
#include"LineChange.h"
#include<string>
using namespace std;
#pragma warning(disable:4996)

OnCutOffCommand::OnCutOffCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
	this->isChecked = false;
	this->firstX = 0;
	this->firstY = 0;
	this->endX = 0;
	this->endY = 0;
    this->isLineChangeButtonClicked = false;
}

OnCutOffCommand::~OnCutOffCommand() {
}

void OnCutOffCommand::Execute() {

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

    //(21.11.10.추가)
    if (this->notePadForm->isOpen == TRUE) {
        this->notePadForm->isOpenChanged = TRUE;
    }

    //1.0. 해당 범위가 있을 경우, 해당 범위를 str에 옮긴다.
    Long firstY = this->notePadForm->selectWords->GetFirstY();
    Long firstX = this->notePadForm->selectWords->GetFirstX();
    Long endY = this->notePadForm->selectWords->GetEndY();
    Long endX = this->notePadForm->selectWords->GetEndX();

    //(21.10.05 추가) 현재 자동개행버튼이 눌려져 있는지 확인한다.
    if (this->notePadForm->isLineChangeButtonClicked == true) {
        this->isLineChangeButtonClicked = true;
        this->windowWidth = this->notePadForm->scroll->windowWidth;
    }


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

    this->firstX = firstX;
    this->firstY = firstY;
    this->endX = endX;
    this->endY = endY;

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
        if (i < endY && nextRow != NULL && !dynamic_cast<DummyRow*>(nextRow)) {  //i < endY && !dynamic_cast<DummyRow*>(row)
            str.Append("\n");
        }
        //*************************************************************************************

        i++;
    }

    //(21.10.05 추가)
    this->character = (LPCTSTR)str;

    //1.1. 클립보드 핸들을 연다.  
    if (this->notePadForm->OpenClipboard()) {//OpenClipboard(this->notePadForm->GetSafeHwnd())

        //1. 클립보드를 비운다.
        EmptyClipboard();

        //2. 클립보드에 넣을 메모리를 할당한다.(메모리 주소를 얻는다)
        hGlobal = GlobalAlloc(GHND | GMEM_SHARE, str.GetLength() + 1);

        //3. 해당 핸들에 문자열을 대입한다.
        PSTR pGlobal = (PSTR)GlobalLock(hGlobal);
        _tcscpy(pGlobal, (LPCTSTR)str);
        GlobalUnlock(hGlobal);

        //4. 클립보드에 데이터를 넣는다. 
        SetClipboardData(CF_TEXT, hGlobal);

        //5. 클립보드를 닫는다.
        CloseClipboard();
    }

    //1.2. 선택된 범위를 삭제한다.
    KeyAction* keyAction = NULL;
    FactoryProduct factoryProduct(this->notePadForm);

    keyAction = factoryProduct.Make(VK_BACK, NULL, NULL);
    keyAction->KeyDown();

    //1.3. 현재 캐럿의 위치를 확인한다.
    this->currentY = this->notePadForm->note->GetCurrent();
    row = this->notePadForm->note->GetChild(this->currentY);
    this->currentX = row->GetCurrent();

}

void OnCutOffCommand::Undo() { //BackSpace의 Undo와 비슷

#if 0
    //0. 현재 캐럿의 위치를 설정한다.
    this->notePadForm->note->MoveCurrent(this->firstY);
    Glyph* row = this->notePadForm->note->GetChild(this->firstY);
    row->MoveCurrent(this->firstX);
#endif
    string temp;
    string doubleByteCharacter;
    char singleByteCharacter[2];
    singleByteCharacter[0] = this->character.at(0);
    int id;
    int i = 0;
    int currentX = this->currentX;
    int currentY = this->currentY;
    int firstX = this->firstX;
    int firstY = this->firstY;
    int endX = this->endX;
    int endY = this->endY;
    int windowWidth = this->notePadForm->scroll->windowWidth; //현재 윈도우너비


    //1. 현재 캐럿의 위치를 변경해준다.
        //1. (21.09.30 추가) 현재 자동개행버튼 상황과 적을때의 상황이 다르면,
    if (this->notePadForm->isLineChangeButtonClicked != this->isLineChangeButtonClicked) {

        //1.1. 현재 자동개행버튼이 눌려져 있으면, 자동개행 후의 좌표를 구한다. 
        if (this->notePadForm->isLineChangeButtonClicked == true) {
            this->notePadForm->lineChange->FindCaretAfterLineChange(currentX, currentY, &currentX, &currentY);
        }
        //1.2. 현재 자동개행버튼이 안눌려져 있으면, 자동개행 전의 좌표를 구한다.
        else {
            //1.2.1. 자동개행 시켜준다.
            this->notePadForm->lineChange->LineChangeButtonClicked(this->windowWidth);
            //1.2.2. 자동개행 전 좌표 구한다.
            this->notePadForm->lineChange->FindCaretBeforeLineChange(currentX, currentY, &currentX, &currentY);
            //1.2.3. 자동개행 취소한다.
            this->notePadForm->lineChange->LineChangeButtonNotClicked();
        }
    }
    //2. 현재 자동개행버튼 상황과 적을때의 상황이 같지만, 자동개행일 때 윈도우 너비가 다른 경우,
    else if (this->isLineChangeButtonClicked == true && windowWidth != this->windowWidth) {
        //2.1. 자동개행된 문서를 펼친다.
        this->notePadForm->lineChange->LineChangeButtonNotClicked();

        //2.2. 자동개행 시켜준다.
        this->notePadForm->lineChange->LineChangeButtonClicked(this->windowWidth);

        //2.3. 자동개행 전 좌표를 구한다.
        this->notePadForm->lineChange->FindCaretBeforeLineChange(currentX, currentY, &currentX, &currentY);

        //2.4. 자동개행된 문서를 펼친다.
        this->notePadForm->lineChange->LineChangeButtonNotClicked();

        //2.5. 펼친 문서를 현재 windowWidth만큼 자동개행한다.
        this->notePadForm->lineChange->LineChangeButtonClicked(windowWidth);

        //2.6. 자동개행 후 좌표를 구한다.
        this->notePadForm->lineChange->FindCaretAfterLineChange(currentX, currentY, &currentX, &currentY);
    }
#if 0
    else if (this->isLineChangeButtonClicked == true) {
        //자동개행 후 좌표를 구한다.
        this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
        this->notePadForm->lineChange->FindCaretAfterLineChange(endX, endY, &endX, &endY);
    }
#endif

    //3. 현재 캐럿의 위치를 변경해준다.
    this->notePadForm->note->MoveCurrent(currentY);
    Glyph* row = this->notePadForm->note->GetChild(currentY);
    row->MoveCurrent(currentX);
    this->notePadForm->isUndo = true;

#if 0
    //4. Paste를 실행한다.
    KeyAction* keyAction = NULL;
    FactoryProduct factoryProduct(this->notePadForm);

    keyAction = factoryProduct.Make(0x56, VK_CONTROL, NULL);
    keyAction->KeyDown();
#endif

    //4. character의 length만큼 반복한다.
    while (i < this->character.size()) {

        //4.1.  캐럿의 크기는 Undo시 3mm로 설정한다.
        this->notePadForm->isFirstComposing = false;

        //4.2. 1Byte인지 2Byte인지 확인한다.
        singleByteCharacter[0] = this->character.at(i);

        //4.3. 현재 character에 있는 문자가 1Byte이면, OnChar을 호출한다.
        if (singleByteCharacter[0] >= 0 && singleByteCharacter[0] <= 127) {

            this->notePadForm->selectWords->firstX = 0;
            this->notePadForm->selectWords->firstY = 0;
            this->notePadForm->selectWords->endX = 0;
            this->notePadForm->selectWords->endY = 0;


            //4.3.1. 개행문자이면 좌표를 currentY++, currentX=0 으로 변환해준다.
            this->notePadForm->nChar = singleByteCharacter[0];
            this->notePadForm->SendMessage(WM_COMMAND, 40001);
        }
        //4.4. 현재 character에 있는 문자가 2Byte이면, OnImeComposition 호출한다.
        else {
            doubleByteCharacter.clear();
            temp = this->character.at(i);
            doubleByteCharacter.append(temp);
            temp = this->character.at(i + 1);
            doubleByteCharacter.append(temp);

            this->notePadForm->selectWords->firstX = 0;
            this->notePadForm->selectWords->firstY = 0;
            this->notePadForm->selectWords->endX = 0;
            this->notePadForm->selectWords->endY = 0;


            this->notePadForm->letter = doubleByteCharacter;
            this->notePadForm->SendMessage(WM_COMMAND, 40005);
            i++;
        }
        i++;
    }


    //1. (21.11.04 추가) 현재 자동개행버튼 상황과 적을때의 상황이 다르면,
    if (this->notePadForm->isLineChangeButtonClicked != this->isLineChangeButtonClicked) {

        //1.1. 현재 자동개행버튼이 눌려져 있으면, 자동개행 후의 좌표를 구한다. 
        if (this->notePadForm->isLineChangeButtonClicked == true) {
            this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
            this->notePadForm->lineChange->FindCaretAfterLineChange(endX, endY, &endX, &endY);
        }
        //1.2. 현재 자동개행버튼이 안눌려져 있으면, 자동개행 전의 좌표를 구한다.
        else {
            //1.2.1. 자동개행 시켜준다.
            this->notePadForm->lineChange->LineChangeButtonClicked(this->windowWidth); //-25
            //1.2.2. 자동개행 전 좌표 구한다.
            this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
            this->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);
            //1.2.3. 자동개행 취소한다.
            this->notePadForm->lineChange->LineChangeButtonNotClicked();
        }
    }
    //2. 현재 자동개행버튼 상황과 적을때의 상황이 같지만, 자동개행일 때 윈도우 너비가 다른 경우,
    else if (this->isLineChangeButtonClicked == true && windowWidth != this->windowWidth) {
        //2.1. 자동개행된 문서를 펼친다.
        this->notePadForm->lineChange->LineChangeButtonNotClicked();

        //2.2. 자동개행 시켜준다.
        this->notePadForm->lineChange->LineChangeButtonClicked(this->windowWidth); //-25

        //2.3. 자동개행 전 좌표를 구한다.
        this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
        this->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);

        //2.4. 자동개행된 문서를 펼친다.
        this->notePadForm->lineChange->LineChangeButtonNotClicked();

        //2.5. 펼친 문서를 현재 windowWidth만큼 자동개행한다.
        this->notePadForm->lineChange->LineChangeButtonClicked(windowWidth);

        //2.6. 자동개행 후 좌표를 구한다.
        this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
        this->notePadForm->lineChange->FindCaretAfterLineChange(endX, endY, &endX, &endY);
    }

    this->windowWidth = this->notePadForm->scroll->windowWidth;
    this->isLineChangeButtonClicked = this->notePadForm->isLineChangeButtonClicked;

    //5. firstXY, endXY를 설정해준다.
    this->notePadForm->selectWords->firstX = firstX;
    this->notePadForm->selectWords->firstY = firstY;
    this->notePadForm->selectWords->endX = endX;
    this->notePadForm->selectWords->endY = endY;

    this->firstX = firstX;
    this->firstY = firstY;
    this->endX = endX;
    this->endY = endY;
    this->currentX = endX;
    this->currentY = endY;

#if 0
    //6. Undo 후 바뀐 캐럿의 위치를 적용한다.
    this->currentX = this->notePadForm->currentX;
    this->currentY = this->notePadForm->currentY;
#endif


    this->notePadForm->isUndo = false;

}


void OnCutOffCommand::Redo() { //BackSpace와 비슷, OnImeChar의 Undo와 비슷
    int currentX = this->currentX;
    int currentY = this->currentY;
    int firstX = this->firstX;
    int firstY = this->firstY;
    int endX = this->endX;
    int endY = this->endY;
    int windowWidth = this->notePadForm->scroll->windowWidth; //현재 윈도우너비


    //1. (21.11.04 추가) 현재 자동개행버튼 상황과 적을때의 상황이 다르면,
    if (this->notePadForm->isLineChangeButtonClicked != this->isLineChangeButtonClicked) {

        //1.1. 현재 자동개행버튼이 눌려져 있으면, 자동개행 후의 좌표를 구한다. 
        if (this->notePadForm->isLineChangeButtonClicked == true) {
            this->notePadForm->lineChange->FindCaretAfterLineChange(currentX, currentY, &currentX, &currentY);
            this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
            this->notePadForm->lineChange->FindCaretAfterLineChange(endX, endY, &endX, &endY);
        }
        //1.2. 현재 자동개행버튼이 안눌려져 있으면, 자동개행 전의 좌표를 구한다.
        else {
            //1.2.1. 자동개행 시켜준다.
            this->notePadForm->lineChange->LineChangeButtonClicked(this->windowWidth);
            //1.2.2. 자동개행 전 좌표 구한다.
            this->notePadForm->lineChange->FindCaretBeforeLineChange(currentX, currentY, &currentX, &currentY);
            this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
            this->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);
            //1.2.3. 자동개행 취소한다.
            this->notePadForm->lineChange->LineChangeButtonNotClicked();
        }
    }
    //2. 현재 자동개행버튼 상황과 적을때의 상황이 같지만, 자동개행일 때 윈도우 너비가 다른 경우,
    else if (this->isLineChangeButtonClicked == true && windowWidth != this->windowWidth) {
        //2.1. 자동개행된 문서를 펼친다.
        this->notePadForm->lineChange->LineChangeButtonNotClicked();

        //2.2. 자동개행 시켜준다.
        this->notePadForm->lineChange->LineChangeButtonClicked(this->windowWidth);

        //2.3. 자동개행 전 좌표를 구한다.
        this->notePadForm->lineChange->FindCaretBeforeLineChange(currentX, currentY, &currentX, &currentY);
        this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
        this->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);

        //2.4. 자동개행된 문서를 펼친다.
        this->notePadForm->lineChange->LineChangeButtonNotClicked();

        //2.5. 펼친 문서를 현재 windowWidth만큼 자동개행한다.
        this->notePadForm->lineChange->LineChangeButtonClicked(windowWidth);

        //2.6. 자동개행 후 좌표를 구한다.
        this->notePadForm->lineChange->FindCaretAfterLineChange(currentX, currentY, &currentX, &currentY);
        this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
        this->notePadForm->lineChange->FindCaretAfterLineChange(endX, endY, &endX, &endY);
    }

    //1. 현재 캐럿의 위치를 변경해준다.
    this->notePadForm->note->MoveCurrent(endY);
    Glyph* row = this->notePadForm->note->GetChild(endY);
    row->MoveCurrent(endX);

    //2. Execute 하기 전에 좌표를 다시 정해준다.
    this->notePadForm->selectWords->firstX = firstX;
    this->notePadForm->selectWords->firstY = firstY;
    this->notePadForm->selectWords->endX = endX;
    this->notePadForm->selectWords->endY = endY;

    this->firstX = firstX;
    this->firstY = firstY;
    this->endX = endX;
    this->endY = endY;


    //캐럿의 크기는 Undo시 3mm로 설정한다.
    this->notePadForm->isFirstComposingForMove = false;
    this->notePadForm->isOnImeCharForMove = false;

    //(21.10.01 추가)
    this->windowWidth = this->notePadForm->scroll->windowWidth;
    this->isLineChangeButtonClicked = this->notePadForm->isLineChangeButtonClicked;


    Execute();
}


Long OnCutOffCommand::GetCurrentX() {
	return this->currentX;
}

Long OnCutOffCommand::GetCurrentY() {
	return this->currentY;
}

void OnCutOffCommand::SetCheck() {
	this->isChecked = true;
}

bool OnCutOffCommand::GetChecked() {
	return this->isChecked;
}








