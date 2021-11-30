//OnImeCharCommand.cpp
#include"OnImeCharCommand.h"
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
#include"LineChange.h"
#include"Scroll.h"
#include"KeyAction.h"
#include"BackSpaceKeyAction.h"
#include"FactoryProduct.h"
#include"CommandStory.h"
#include"UndoList.h"
#include"RedoList.h"
#include"resource.h"
#include"Font.h"
#include"DummyRow.h"
#include"GetTextSize.h"
#include"SelectWords.h"
using namespace std;

OnImeCharCommand::OnImeCharCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {

	this->isChecked = false;
	this->isLineChanged = false;
	this->currentX = 0;
	this->currentY = 0;
	this->beforeCurrentX = 0;
	this->beforeCurrentY = 0;
	//this->character[2] = '\0';
	this->isRedo = false;

	this->isLineChangeButtonClicked = false;
	this->notePadForm->isRedo = this->isRedo;
	this->notePadForm->isOnImeCharForMove = true;	
}

OnImeCharCommand::~OnImeCharCommand() {
}

void OnImeCharCommand::Execute() {

	Glyph* character;
	char letter[3];
	letter[0] = this->notePadForm->letter.at(0);
	letter[1] = this->notePadForm->letter.at(1);
	letter[2] = '\0';
#if 0
	letter[0] = HIBYTE(LOWORD(this->notePadForm->letter));
	letter[1] = LOBYTE(LOWORD(this->notePadForm->letter));
	letter[2] = '\0';
#endif
	Long length;
	Long i = 0;
	string strCharacter;
	char chaCharacter[3];
	char characterMove[3];
	Long caretX;
	Long caretY;
	Long changedX;
	Long changedY;
	this->windowWidth = this->notePadForm->scroll->windowWidth;
	CreateGlyph* creator = new CreateProduct();

	character = creator->Make(letter);

	this->character = letter;

	//21.08.03 추가

	//2. 글자가 완성 되었을 때
	  //2.1. 현재 캐럿의 위치를 확인한다.
	int currentY = this->notePadForm->note->GetCurrent();
	this->notePadForm->row = this->notePadForm->note->GetChild(currentY);
	int currentX = this->notePadForm->row->GetCurrent();
	length = this->notePadForm->row->GetLength();


	//Command의 멤버 설정한다. (21.09.06)*********************************************************
	this->beforeCurrentX = currentX;
	this->beforeCurrentY = currentY;

	//(21.11.09.추가) 만약 currentX가 0이면, currentY = currentY-1, currnetX는 이전 row의 length로 치환한다.
	if (this->notePadForm->isLineChangeAppend == false && currentY > 0 && currentX <= 0) {

		Glyph* beforeRow = this->notePadForm->note->GetChild(currentY - 1);
		int beforeLength = beforeRow->GetLength();

		if (beforeLength > 0) {
			this->beforeCurrentX = beforeLength;
			this->beforeCurrentY = currentY - 1;
		}
	}

	//(21.11.09.추가)
	this->notePadForm->isLineChangeAppend = false;

	//********************************************************************************************



	//2.2 자동개행버튼이 눌려진 경우,
	if (this->notePadForm->isLineChangeButtonClicked == true) {

		//2.1. 자동개행 전 캐럿의 위치를 찾는다.
		this->notePadForm->lineChange->FindCaretBeforeLineChange(&currentX, &currentY);

		//2.2. 자동개행 된 문서를 펼친다.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

		this->notePadForm->row = this->notePadForm->note->GetChild(currentY);
		length = this->notePadForm->row->GetLength();
		this->isLineChangeButtonClicked = true;
		this->windowWidth = this->notePadForm->scroll->windowWidth;
	}

	//2.3. 기존에 쓰여진 글자를 지운다. (Redo가 아닐 경우)
	if (this->isRedo == false && this->notePadForm->isUndo == false) {
		this->notePadForm->row->Remove(currentX);
	}

	//2.4. currentX가 row의 length-1보다 작거나, currentX가 length-1보다 크거나 같고, 커서 크기가 3인 경우,
	if (currentX < length - 1 || (currentX >= length - 1 && this->notePadForm->caretWidth == 3)) {
		//2.4.1. 현재 줄에 새로 글자를 끼워적는다.
		this->notePadForm->row->Add(currentX, character);

	}

	//2.5. currentX가 row의 length보다 크거나 같거나, currentX가 length-1보다 크거나 같고, 커서 크기가 3보다 큰 경우,
	else if (currentX >= length || (currentX >= length - 1 && this->notePadForm->caretWidth > 3)) {
		//2.5.1. 현재 줄에 새로 글자를 붙혀적는다.
		this->notePadForm->row->Add(character);
	}

	//2.6. 좌표를 currentX+1로 이동한다.
	changedY = currentY;
	changedX = currentX + 1;

	//2.7. 자동개행 버튼이 눌려진 경우,
	if (this->notePadForm->isLineChangeButtonClicked == true) {

		//2.7.1. 펼친 문서를 다시 자동개행한다. (ON)
		this->notePadForm->lineChange->LineChangeButtonClicked();

		//2.7.2. 자동개행 후 이동할 캐럿의 위치를 구한다.
		this->notePadForm->lineChange->FindCaretAfterLineChange(changedX, changedY);
	}
	else {
		this->notePadForm->row->MoveCurrent(changedX);
	}

	//Command의 멤버 설정한다. *******************************************************************
	//1.1. 현재 캐럿의 위치를 확인한다.
	currentY = this->notePadForm->note->GetCurrent();
	this->notePadForm->row = this->notePadForm->note->GetChild(currentY);
	currentX = this->notePadForm->row->GetCurrent();

	//1.2. 현재 캐럿의 위치를 멤버에 설정한다.
	this->currentX = currentX;
	this->currentY = currentY;

	//1.3. 만약 BackSpace의 Undo중이라면 바뀐 캐럿의 위치를 공유한다.
	this->notePadForm->currentX = currentX;
	this->notePadForm->currentY = currentY;
	//********************************************************************************************


	this->notePadForm->isOnImeCharForMove = true;
	this->notePadForm->isFirstComposingForMove = this->notePadForm->isFirstComposing;

	//Scroll의 조건을 설정한다.
	this->notePadForm->isDoubleByte = true;
	this->notePadForm->isJumpOverForPrevious = false;
	this->notePadForm->isJumpOverForNext = false;
	this->notePadForm->isUp = false;
	this->notePadForm->isDown = false;

	this->notePadForm->Notify();


	this->notePadForm->isFirstComposing = true;
	this->notePadForm->isChanged = true;

	this->notePadForm->Invalidate();


#if 0
	//2.2. 기존에 쓰여진 글자를 지운다. (Redo가 아닐 경우)
	if (this->isRedo == false) {
		this->notePadForm->row->Remove(currentX);
	}

	length = this->notePadForm->row->GetLength();


	//(21.08.12. 추가) 자동개행버튼 OFF일 경우,
	if (this->notePadForm->isLineChangeButtonClicked == false) {

		//2.3. currentX가 row의 length-1보다 작거나, currentX가 length-1보다 크거나 같고, 커서 크기가 3인 경우,
		if (currentX < length - 1 || (currentX >= length - 1 && this->notePadForm->caretWidth == 3)) {
			//2.3.1. 현재 줄에 새로 글자를 끼워적는다.
			this->notePadForm->row->Add(currentX, character);

			this->notePadForm->isInsert = true;
			this->notePadForm->isAppend = false;
			this->notePadForm->isMoveActionVertical = false;
			this->notePadForm->isMoveActionHorizon = false;
			this->notePadForm->isDeleteAction = false;
		}

		//2.4. currentX가 row의 length보다 크거나 같거나, currentX가 length-1보다 크거나 같고, 커서 크기가 3보다 큰 경우,
		else if (currentX >= length || (currentX >= length - 1 && this->notePadForm->caretWidth > 3)) {
			//2.4.1. 현재 줄에 새로 글자를 붙혀적는다.
			this->notePadForm->row->Add(character);

			this->notePadForm->isInsert = false;
			this->notePadForm->isAppend = true;
			this->notePadForm->isMoveActionVertical = false;
			this->notePadForm->isMoveActionHorizon = false;
			this->notePadForm->isDeleteAction = false;
		}

		//2.5. 좌표를 currentX+1로 이동한다.
		changedX = currentX + 1;
		this->notePadForm->row->MoveCurrent(currentX + 1);
	}
	//(21.08.12. 추가) 자동개행버튼 ON일 경우,
	else if (this->notePadForm->isLineChangeButtonClicked == true) {

#if 0
		//2.3. currentX가 row의 length-1보다 작거나, currentX가 length-1보다 크거나 같고, 커서 크기가 3인 경우,
		if (currentX < length - 1 || (currentX >= length - 1 && this->caretWidth == 3)) {
			//2.3.1. 현재 줄에 새로 글자를 끼워적는다.
			this->row->Add(currentX, character);
		}

		//2.4. currentX가 row의 length보다 크거나 같거나, currentX가 length-1보다 크거나 같고, 커서 크기가 3보다 큰 경우,
		else if (currentX >= length || (currentX >= length - 1 && this->caretWidth > 3)) {
			//2.4.1. 현재 줄에 새로 글자를 붙혀적는다.
			this->row->Add(character);
		}
#endif
		this->notePadForm->row->Add(currentX, character);

		//2.5. 좌표를 currentX+1로 이동한다.
		changedX = currentX + 1;
		this->notePadForm->row->MoveCurrent(currentX + 1);


		//스크롤 설정
		this->notePadForm->isInsert = false;
		this->notePadForm->isAppend = false;
		this->notePadForm->isMoveActionVertical = false;
		this->notePadForm->isMoveActionHorizon = false;
		this->notePadForm->isDeleteAction = false;
	}

	//Command의 멤버 설정한다. *******************************************************************

	//1. 현재 캐럿의 위치를 멤버에 설정한다.
	this->currentX = changedX;
	this->currentY = currentY;
	//********************************************************************************************

	//2.7. currentX(row의 current)보다 작은 동안 반복한다.
	this->notePadForm->isOnImeCharForMove = true;
	this->notePadForm->isFirstComposingForMove = this->notePadForm->isFirstComposing;
	this->notePadForm->Notify();


	this->notePadForm->isFirstComposing = true;
	this->notePadForm->isChanged = true;

	this->notePadForm->Invalidate();

	//return 0;
#endif


}

void OnImeCharCommand::Undo() {

	int windowWidth = this->notePadForm->scroll->windowWidth;
	KeyAction* keyAction = NULL;
	FactoryProduct factoryProduct(this->notePadForm);

	keyAction = factoryProduct.Make(VK_BACK, NULL, NULL);

	int currentX = this->currentX;
	int currentY = this->currentY;

	//1. (21.10.01 추가) 현재 자동개행버튼 상황과 적을때의 상황이 다르면,
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


	//현재 캐럿의 위치를 변경해준다.
	this->notePadForm->note->MoveCurrent(currentY);
	Glyph* row = this->notePadForm->note->GetChild(currentY);
	row->MoveCurrent(currentX);

	//선택된 영역을 초기화한다.
	this->notePadForm->selectWords->firstX = 0;
	this->notePadForm->selectWords->firstY = 0;
	this->notePadForm->selectWords->endX = 0;
	this->notePadForm->selectWords->endY = 0;


	keyAction->KeyDown();
}


void OnImeCharCommand::Redo() {

	int windowWidth = this->notePadForm->scroll->windowWidth;
	int currentX = this->beforeCurrentX;
	int currentY = this->beforeCurrentY;
	Glyph* newRow;

	//1. (21.10.01 추가) 현재 자동개행버튼 상황과 적을때의 상황이 다르면,
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
			//this->notePadForm->lineChange->FindCaretBeforeLineChangeForDouble(currentX, currentY, &currentX, &currentY);
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

	//현재 캐럿의 위치를 변경해준다.
	this->notePadForm->note->MoveCurrent(currentY);

#if 0
	//(21.11.02.추가) 그 다음 줄이 없으면 만들어준다.
	CreateProduct createProduct;
	if (this->notePadForm->note->GetLength() <= currentY) {
		Glyph* row_ = new DummyRow;
		this->notePadForm->note->Add(row_);
	}
#endif

	Glyph* row = this->notePadForm->note->GetChild(currentY);
	row->MoveCurrent(currentX);

	this->isRedo = true;
	this->notePadForm->isRedo = this->isRedo;
	this->isLineChangeButtonClicked = this->notePadForm->isLineChangeButtonClicked;
	this->windowWidth = windowWidth;

	Execute();
}

bool OnImeCharCommand::GetChecked() {
	return this->isChecked;

}

Long OnImeCharCommand::GetCurrentX() {
	return this->currentX;
}

Long OnImeCharCommand::GetCurrentY() {
	return this->currentY;
}

void OnImeCharCommand::SetCheck() {
	this->isChecked = true;
}

bool OnImeCharCommand::GetLineChanged() {
	return this->isLineChanged;
}

#if 0
char* OnImeCharCommand::GetCharacter() {
	return this->character;
}


WPARAM OnImeCharCommand::GetCharacterFor2Byte() {
	return this->character;
}
#endif

string OnImeCharCommand::GetCharacterForDoubleByte() {
	return this->character;
}









