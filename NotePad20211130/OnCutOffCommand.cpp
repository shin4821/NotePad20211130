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

    //1. Ctrl+ C Ű ��ư ������ ��

    //(21.11.10.�߰�)
    if (this->notePadForm->isOpen == TRUE) {
        this->notePadForm->isOpenChanged = TRUE;
    }

    //1.0. �ش� ������ ���� ���, �ش� ������ str�� �ű��.
    Long firstY = this->notePadForm->selectWords->GetFirstY();
    Long firstX = this->notePadForm->selectWords->GetFirstX();
    Long endY = this->notePadForm->selectWords->GetEndY();
    Long endX = this->notePadForm->selectWords->GetEndX();

    //(21.10.05 �߰�) ���� �ڵ������ư�� ������ �ִ��� Ȯ���Ѵ�.
    if (this->notePadForm->isLineChangeButtonClicked == true) {
        this->isLineChangeButtonClicked = true;
        this->windowWidth = this->notePadForm->scroll->windowWidth;
    }


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

    this->firstX = firstX;
    this->firstY = firstY;
    this->endX = endX;
    this->endY = endY;

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
        if (i < endY && nextRow != NULL && !dynamic_cast<DummyRow*>(nextRow)) {  //i < endY && !dynamic_cast<DummyRow*>(row)
            str.Append("\n");
        }
        //*************************************************************************************

        i++;
    }

    //(21.10.05 �߰�)
    this->character = (LPCTSTR)str;

    //1.1. Ŭ������ �ڵ��� ����.  
    if (this->notePadForm->OpenClipboard()) {//OpenClipboard(this->notePadForm->GetSafeHwnd())

        //1. Ŭ�����带 ����.
        EmptyClipboard();

        //2. Ŭ�����忡 ���� �޸𸮸� �Ҵ��Ѵ�.(�޸� �ּҸ� ��´�)
        hGlobal = GlobalAlloc(GHND | GMEM_SHARE, str.GetLength() + 1);

        //3. �ش� �ڵ鿡 ���ڿ��� �����Ѵ�.
        PSTR pGlobal = (PSTR)GlobalLock(hGlobal);
        _tcscpy(pGlobal, (LPCTSTR)str);
        GlobalUnlock(hGlobal);

        //4. Ŭ�����忡 �����͸� �ִ´�. 
        SetClipboardData(CF_TEXT, hGlobal);

        //5. Ŭ�����带 �ݴ´�.
        CloseClipboard();
    }

    //1.2. ���õ� ������ �����Ѵ�.
    KeyAction* keyAction = NULL;
    FactoryProduct factoryProduct(this->notePadForm);

    keyAction = factoryProduct.Make(VK_BACK, NULL, NULL);
    keyAction->KeyDown();

    //1.3. ���� ĳ���� ��ġ�� Ȯ���Ѵ�.
    this->currentY = this->notePadForm->note->GetCurrent();
    row = this->notePadForm->note->GetChild(this->currentY);
    this->currentX = row->GetCurrent();

}

void OnCutOffCommand::Undo() { //BackSpace�� Undo�� ���

#if 0
    //0. ���� ĳ���� ��ġ�� �����Ѵ�.
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
    int windowWidth = this->notePadForm->scroll->windowWidth; //���� ������ʺ�


    //1. ���� ĳ���� ��ġ�� �������ش�.
        //1. (21.09.30 �߰�) ���� �ڵ������ư ��Ȳ�� �������� ��Ȳ�� �ٸ���,
    if (this->notePadForm->isLineChangeButtonClicked != this->isLineChangeButtonClicked) {

        //1.1. ���� �ڵ������ư�� ������ ������, �ڵ����� ���� ��ǥ�� ���Ѵ�. 
        if (this->notePadForm->isLineChangeButtonClicked == true) {
            this->notePadForm->lineChange->FindCaretAfterLineChange(currentX, currentY, &currentX, &currentY);
        }
        //1.2. ���� �ڵ������ư�� �ȴ����� ������, �ڵ����� ���� ��ǥ�� ���Ѵ�.
        else {
            //1.2.1. �ڵ����� �����ش�.
            this->notePadForm->lineChange->LineChangeButtonClicked(this->windowWidth);
            //1.2.2. �ڵ����� �� ��ǥ ���Ѵ�.
            this->notePadForm->lineChange->FindCaretBeforeLineChange(currentX, currentY, &currentX, &currentY);
            //1.2.3. �ڵ����� ����Ѵ�.
            this->notePadForm->lineChange->LineChangeButtonNotClicked();
        }
    }
    //2. ���� �ڵ������ư ��Ȳ�� �������� ��Ȳ�� ������, �ڵ������� �� ������ �ʺ� �ٸ� ���,
    else if (this->isLineChangeButtonClicked == true && windowWidth != this->windowWidth) {
        //2.1. �ڵ������ ������ ��ģ��.
        this->notePadForm->lineChange->LineChangeButtonNotClicked();

        //2.2. �ڵ����� �����ش�.
        this->notePadForm->lineChange->LineChangeButtonClicked(this->windowWidth);

        //2.3. �ڵ����� �� ��ǥ�� ���Ѵ�.
        this->notePadForm->lineChange->FindCaretBeforeLineChange(currentX, currentY, &currentX, &currentY);

        //2.4. �ڵ������ ������ ��ģ��.
        this->notePadForm->lineChange->LineChangeButtonNotClicked();

        //2.5. ��ģ ������ ���� windowWidth��ŭ �ڵ������Ѵ�.
        this->notePadForm->lineChange->LineChangeButtonClicked(windowWidth);

        //2.6. �ڵ����� �� ��ǥ�� ���Ѵ�.
        this->notePadForm->lineChange->FindCaretAfterLineChange(currentX, currentY, &currentX, &currentY);
    }
#if 0
    else if (this->isLineChangeButtonClicked == true) {
        //�ڵ����� �� ��ǥ�� ���Ѵ�.
        this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
        this->notePadForm->lineChange->FindCaretAfterLineChange(endX, endY, &endX, &endY);
    }
#endif

    //3. ���� ĳ���� ��ġ�� �������ش�.
    this->notePadForm->note->MoveCurrent(currentY);
    Glyph* row = this->notePadForm->note->GetChild(currentY);
    row->MoveCurrent(currentX);
    this->notePadForm->isUndo = true;

#if 0
    //4. Paste�� �����Ѵ�.
    KeyAction* keyAction = NULL;
    FactoryProduct factoryProduct(this->notePadForm);

    keyAction = factoryProduct.Make(0x56, VK_CONTROL, NULL);
    keyAction->KeyDown();
#endif

    //4. character�� length��ŭ �ݺ��Ѵ�.
    while (i < this->character.size()) {

        //4.1.  ĳ���� ũ��� Undo�� 3mm�� �����Ѵ�.
        this->notePadForm->isFirstComposing = false;

        //4.2. 1Byte���� 2Byte���� Ȯ���Ѵ�.
        singleByteCharacter[0] = this->character.at(i);

        //4.3. ���� character�� �ִ� ���ڰ� 1Byte�̸�, OnChar�� ȣ���Ѵ�.
        if (singleByteCharacter[0] >= 0 && singleByteCharacter[0] <= 127) {

            this->notePadForm->selectWords->firstX = 0;
            this->notePadForm->selectWords->firstY = 0;
            this->notePadForm->selectWords->endX = 0;
            this->notePadForm->selectWords->endY = 0;


            //4.3.1. ���๮���̸� ��ǥ�� currentY++, currentX=0 ���� ��ȯ���ش�.
            this->notePadForm->nChar = singleByteCharacter[0];
            this->notePadForm->SendMessage(WM_COMMAND, 40001);
        }
        //4.4. ���� character�� �ִ� ���ڰ� 2Byte�̸�, OnImeComposition ȣ���Ѵ�.
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


    //1. (21.11.04 �߰�) ���� �ڵ������ư ��Ȳ�� �������� ��Ȳ�� �ٸ���,
    if (this->notePadForm->isLineChangeButtonClicked != this->isLineChangeButtonClicked) {

        //1.1. ���� �ڵ������ư�� ������ ������, �ڵ����� ���� ��ǥ�� ���Ѵ�. 
        if (this->notePadForm->isLineChangeButtonClicked == true) {
            this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
            this->notePadForm->lineChange->FindCaretAfterLineChange(endX, endY, &endX, &endY);
        }
        //1.2. ���� �ڵ������ư�� �ȴ����� ������, �ڵ����� ���� ��ǥ�� ���Ѵ�.
        else {
            //1.2.1. �ڵ����� �����ش�.
            this->notePadForm->lineChange->LineChangeButtonClicked(this->windowWidth); //-25
            //1.2.2. �ڵ����� �� ��ǥ ���Ѵ�.
            this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
            this->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);
            //1.2.3. �ڵ����� ����Ѵ�.
            this->notePadForm->lineChange->LineChangeButtonNotClicked();
        }
    }
    //2. ���� �ڵ������ư ��Ȳ�� �������� ��Ȳ�� ������, �ڵ������� �� ������ �ʺ� �ٸ� ���,
    else if (this->isLineChangeButtonClicked == true && windowWidth != this->windowWidth) {
        //2.1. �ڵ������ ������ ��ģ��.
        this->notePadForm->lineChange->LineChangeButtonNotClicked();

        //2.2. �ڵ����� �����ش�.
        this->notePadForm->lineChange->LineChangeButtonClicked(this->windowWidth); //-25

        //2.3. �ڵ����� �� ��ǥ�� ���Ѵ�.
        this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
        this->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);

        //2.4. �ڵ������ ������ ��ģ��.
        this->notePadForm->lineChange->LineChangeButtonNotClicked();

        //2.5. ��ģ ������ ���� windowWidth��ŭ �ڵ������Ѵ�.
        this->notePadForm->lineChange->LineChangeButtonClicked(windowWidth);

        //2.6. �ڵ����� �� ��ǥ�� ���Ѵ�.
        this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
        this->notePadForm->lineChange->FindCaretAfterLineChange(endX, endY, &endX, &endY);
    }

    this->windowWidth = this->notePadForm->scroll->windowWidth;
    this->isLineChangeButtonClicked = this->notePadForm->isLineChangeButtonClicked;

    //5. firstXY, endXY�� �������ش�.
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
    //6. Undo �� �ٲ� ĳ���� ��ġ�� �����Ѵ�.
    this->currentX = this->notePadForm->currentX;
    this->currentY = this->notePadForm->currentY;
#endif


    this->notePadForm->isUndo = false;

}


void OnCutOffCommand::Redo() { //BackSpace�� ���, OnImeChar�� Undo�� ���
    int currentX = this->currentX;
    int currentY = this->currentY;
    int firstX = this->firstX;
    int firstY = this->firstY;
    int endX = this->endX;
    int endY = this->endY;
    int windowWidth = this->notePadForm->scroll->windowWidth; //���� ������ʺ�


    //1. (21.11.04 �߰�) ���� �ڵ������ư ��Ȳ�� �������� ��Ȳ�� �ٸ���,
    if (this->notePadForm->isLineChangeButtonClicked != this->isLineChangeButtonClicked) {

        //1.1. ���� �ڵ������ư�� ������ ������, �ڵ����� ���� ��ǥ�� ���Ѵ�. 
        if (this->notePadForm->isLineChangeButtonClicked == true) {
            this->notePadForm->lineChange->FindCaretAfterLineChange(currentX, currentY, &currentX, &currentY);
            this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
            this->notePadForm->lineChange->FindCaretAfterLineChange(endX, endY, &endX, &endY);
        }
        //1.2. ���� �ڵ������ư�� �ȴ����� ������, �ڵ����� ���� ��ǥ�� ���Ѵ�.
        else {
            //1.2.1. �ڵ����� �����ش�.
            this->notePadForm->lineChange->LineChangeButtonClicked(this->windowWidth);
            //1.2.2. �ڵ����� �� ��ǥ ���Ѵ�.
            this->notePadForm->lineChange->FindCaretBeforeLineChange(currentX, currentY, &currentX, &currentY);
            this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
            this->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);
            //1.2.3. �ڵ����� ����Ѵ�.
            this->notePadForm->lineChange->LineChangeButtonNotClicked();
        }
    }
    //2. ���� �ڵ������ư ��Ȳ�� �������� ��Ȳ�� ������, �ڵ������� �� ������ �ʺ� �ٸ� ���,
    else if (this->isLineChangeButtonClicked == true && windowWidth != this->windowWidth) {
        //2.1. �ڵ������ ������ ��ģ��.
        this->notePadForm->lineChange->LineChangeButtonNotClicked();

        //2.2. �ڵ����� �����ش�.
        this->notePadForm->lineChange->LineChangeButtonClicked(this->windowWidth);

        //2.3. �ڵ����� �� ��ǥ�� ���Ѵ�.
        this->notePadForm->lineChange->FindCaretBeforeLineChange(currentX, currentY, &currentX, &currentY);
        this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
        this->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);

        //2.4. �ڵ������ ������ ��ģ��.
        this->notePadForm->lineChange->LineChangeButtonNotClicked();

        //2.5. ��ģ ������ ���� windowWidth��ŭ �ڵ������Ѵ�.
        this->notePadForm->lineChange->LineChangeButtonClicked(windowWidth);

        //2.6. �ڵ����� �� ��ǥ�� ���Ѵ�.
        this->notePadForm->lineChange->FindCaretAfterLineChange(currentX, currentY, &currentX, &currentY);
        this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
        this->notePadForm->lineChange->FindCaretAfterLineChange(endX, endY, &endX, &endY);
    }

    //1. ���� ĳ���� ��ġ�� �������ش�.
    this->notePadForm->note->MoveCurrent(endY);
    Glyph* row = this->notePadForm->note->GetChild(endY);
    row->MoveCurrent(endX);

    //2. Execute �ϱ� ���� ��ǥ�� �ٽ� �����ش�.
    this->notePadForm->selectWords->firstX = firstX;
    this->notePadForm->selectWords->firstY = firstY;
    this->notePadForm->selectWords->endX = endX;
    this->notePadForm->selectWords->endY = endY;

    this->firstX = firstX;
    this->firstY = firstY;
    this->endX = endX;
    this->endY = endY;


    //ĳ���� ũ��� Undo�� 3mm�� �����Ѵ�.
    this->notePadForm->isFirstComposingForMove = false;
    this->notePadForm->isOnImeCharForMove = false;

    //(21.10.01 �߰�)
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








