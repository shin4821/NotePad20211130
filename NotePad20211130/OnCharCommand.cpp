//OnCharCommand.cpp
#include"OnCharCommand.h"
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
#include"SelectWords.h"
#include"DummyRow.h"


OnCharCommand::OnCharCommand(NotePadForm* notePadForm) // OnChar* onChar
	:Command(notePadForm) {

	this->isChecked = false;
	this->isLineChanged = false;
    this->currentX = 0;
	this->currentY = 0;
	this->beforeCurrentX = 0;
	this->beforeCurrentY = 0;
	this->character[1]= '\0';
	this->isSelected = false;
	this->isLineChangeButtonClicked = false;
}

OnCharCommand::~OnCharCommand() {

}

void OnCharCommand::Execute() {

	char chaCharacter[3];
	string strCharacter;
	Glyph* character = NULL;
	Glyph* row;
	Long rowLength;
	Long noteLength;
	Long i;
	Long j;
	int currentY;
	int currentX;
	int caretX;
	int caretY;
	char characterMove[3];
	int location;
	Glyph* dummyRow;
	Glyph* nextRow;
	Glyph* testRow = NULL;
	int width;
	int exceptWidth;
	BOOL isLineChanged = false;
	int beforeLineChangeCurrentX;
	int changedX;
	int changedY;


	//(21.11.10.추가)
	if (this->notePadForm->isOpen == TRUE) {
		this->notePadForm->isOpenChanged = TRUE;
	}



	//21.08.03 추가
	char character_[2];

	character_[0] = this->notePadForm->nChar;
	character_[1] = '\0';

	this->character[0] = character_[0];

	CreateGlyph* creator = new CreateProduct();

	//1.1. 현재 캐럿의 위치를 확인한다.
	currentY = this->notePadForm->note->GetCurrent();
	row = this->notePadForm->note->GetChild(currentY);
	currentX = row->GetCurrent();
	noteLength = this->notePadForm->note->GetLength();
	rowLength = row->GetLength();


	//Command의 멤버 설정한다. (21.09.06)*********************************************************

	//1. 현재 캐럿의 위치를 멤버에 설정한다.
	this->beforeCurrentX = currentX;
	this->beforeCurrentY = currentY;
	//********************************************************************************************

	//1.2. 자동개행 버튼이 눌려진 경우,
	if (this->notePadForm->isLineChangeButtonClicked == true) {

		//1.2.1. 자동개행 전 캐럿의 위치를 찾는다.
		this->notePadForm->lineChange->FindCaretBeforeLineChange(&currentX, &currentY);

		//1.2.2. 자동개행 된 문서를 펼친다.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

		row = this->notePadForm->note->GetChild(currentY);
		rowLength = row->GetLength();

		//1.2.3. 개행 상태일 경우, 현재 윈도우 너비를 설정한다.
		this->isLineChangeButtonClicked = true;
		this->windowWidth = this->notePadForm->scroll->windowWidth;
	}

	//1.3. 입력받은 문자가 개행문자인 경우,
	if (character_[0] == '\n' || character_[0] == '\r') {

		//(21.11.09.추가)
		this->notePadForm->isLineChangeAppend = true;

		//1.3.1. 현재 note에 currentY+1번째에 새 줄을 끼워넣는다. (붙혀적다 + 끼워적다)
		this->notePadForm->row = creator->Make(character_);
		this->notePadForm->note->Add(currentY+1, this->notePadForm->row); //currentY + 1

		//1.3.2. currentX가 row의 length보다 작은 경우, (끼워적다)
		if (currentX < rowLength) {
			//1.3.2.1. 현재 row의 length만큼 반복한다.
			while (currentX < rowLength) {
				//1. 문자를 읽는다.
				character = row->GetChild(currentX);
				//2. currentY+1번째에 붙혀적는다.
				this->notePadForm->row->Add(character);

				//3. currentX번째 글자를 삭제한다.
				row->Remove(currentX);

				rowLength--;
			}
		}
		//1.3.3. 캐럿을 (currentY+1, 0)으로 좌표설정한다.
		changedY = currentY + 1;
		changedX = 0;


		//(21.09.03 추가) 개행문자이면 command의 isChecked 멤버를 true로 바꿔준다.
		this->isChecked = true;
		this->isLineChanged = true;
	}

	//1.4. 입력받은 문자가 개행문자가 아닌 경우,
	else {
		//(21.11.09.추가)
		this->notePadForm->isLineChangeAppend = false;

		//1.4.0. character를 만든다.
		character = creator->Make(character_);

		//1.4.1. currentX가 row의 length보다 작은 경우(끼워적다), 현재 row의 currentX번째에 글자를 끼워적는다.
		if (currentX < rowLength) {
			row->Add(currentX, character);
		}
		//1.4.2. currentX가 row의 length보다 크거나 같은 경우, 현재 row에 붙혀 적는다.
		else {
			row->Add(character);
		}
		//1.4.3. 캐럿을 (currentY, currentX+1)로 좌표설정한다.
		changedY = currentY;
		changedX = currentX + 1;
		//this->isChecked = false;
	}

	//1.5. 자동개행 버튼이 눌려진 경우,
	if (this->notePadForm->isLineChangeButtonClicked == true) {

		//1.5.1. 펼친 문서를 다시 자동개행한다. (ON)
		this->notePadForm->lineChange->LineChangeButtonClicked();

		//1.5.1. 자동개행 후 이동할 캐럿의 위치를 구한다.
		this->notePadForm->lineChange->FindCaretAfterLineChange(changedX, changedY);
	}
	else {
		//1.6. 캐럿을 옮긴다.
		this->notePadForm->note->MoveCurrent(changedY);
		this->notePadForm->row = this->notePadForm->note->GetChild(changedY);
		this->notePadForm->row->MoveCurrent(changedX);
	}

	if (creator != NULL) {
		delete creator;
		creator = NULL;
	}

	//2.6. currentX(row의 current)보다 작은 동안 반복한다.
	this->notePadForm->isOnImeCharForMove = false;
	this->notePadForm->isFirstComposingForMove = false;


	//----------------------------------------------------------------------------
	// Scroll 업데이트 할 때의 조건을 설정한다.
	this->notePadForm->isJumpOverForPrevious = false;
	this->notePadForm->isJumpOverForNext = false;
	this->notePadForm->isDoubleByte = false;
	this->notePadForm->isUp = false;
	this->notePadForm->isDown = false;


	//Command의 멤버 설정한다. *******************************************************************
	
	//1.1. 현재 캐럿의 위치를 확인한다.
	currentY = this->notePadForm->note->GetCurrent();
	row = this->notePadForm->note->GetChild(currentY);	
	currentX = row->GetCurrent();

	//1.2. 현재 캐럿의 위치를 멤버에 설정한다.
	this->currentX = currentX;
	this->currentY = currentY;

#if 0
	//(21.10.31.추가) 개행문자 였으면 currentY 설정할때는 1빼준다. 그래야 체크가 된다.
	if (character_[0] == '\n' || character_[0] == '\r') {
		this->currentY--;
	}
#endif

	//1.3. 만약 BackSpace의 Undo중이라면 바뀐 캐럿의 위치를 공유한다.
	this->notePadForm->currentX = currentX;
	this->notePadForm->currentY = currentY;
	//********************************************************************************************


	this->notePadForm->Notify(); //Observer Pattern 이후


	this->notePadForm->isChanged = true;
	this->notePadForm->Invalidate();
}

void OnCharCommand::Undo() {

	int windowWidth = this->notePadForm->scroll->windowWidth; //현재 윈도우너비
	KeyAction* keyAction = NULL;
	FactoryProduct factoryProduct(this->notePadForm);

	keyAction = factoryProduct.Make(VK_BACK, NULL, NULL);


	int currentX = this->currentX;
	int currentY = this->currentY;

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



	//3. 현재 캐럿의 위치를 변경해준다.
	this->notePadForm->note->MoveCurrent(currentY);
	Glyph* row = this->notePadForm->note->GetChild(currentY);
	row->MoveCurrent(currentX);


	//(21.11.01.추가) 전체 선택 영역을 초기화한다.
	this->notePadForm->selectWords->firstX = 0;
	this->notePadForm->selectWords->firstY = 0;
	this->notePadForm->selectWords->endX = 0;
	this->notePadForm->selectWords->endY = 0;



	keyAction->KeyDown();

}

void OnCharCommand::Redo() {

	int windowWidth = this->notePadForm->scroll->windowWidth; //현재 윈도우너비
	int currentX = this->beforeCurrentX;
	int  currentY = this->beforeCurrentY;

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

	//현재 캐럿의 위치를 변경해준다.
	this->notePadForm->note->MoveCurrent(currentY);
	Glyph* row = this->notePadForm->note->GetChild(currentY);
	row->MoveCurrent(currentX);

	//(21.10.01 추가)
	this->isLineChangeButtonClicked = this->notePadForm->isLineChangeButtonClicked;
	this->windowWidth = windowWidth;

	Execute();
}




#if 0
void OnCharCommand::Pile() {
	Command* previousCommand;
	Long previousColumn;
	Long column = this->GetColumn();

	//1. 커맨드를 PUSH하기 전, 체크가 필요하면 체크해준다. **************************************
	//1.1. undoList가 비어있지 않으면,
	if (this->notePadForm->commandStory->undoList->IsEmpty() == false) {
		previousCommand = this->notePadForm->commandStory->undoList->Top();
		previousColumn = previousCommand->GetColumn();

		bool isChecked = previousCommand->GetChecked();

		//1.1.1. 현재 commmand가 체크되어 있지 않고, 전 컬럼과 현 컬럼의 차이가 1이 아닌 경우,
		if (this->isChecked == false && (column - previousColumn != 1)) {
			this->isChecked = true;
		}
	}
	//1.2. undoList가 비어있으면,
	else {
		//1.2.1. command의 isChecked를 true로 바꿔준다.
		this->isChecked = true;
	}
	//********************************************************************************************
	//UndoList에 추가한다.
	this->notePadForm->commandStory->undoList->Push(this);

	//해당 커맨드가 수행이되면 redo의 기능이 초기화되기 때문에 this.redoList를 초기화시킨다.
	this->notePadForm->commandStory->redoList->DeleteAll();
}
#endif

bool OnCharCommand::GetChecked() {
	return this->isChecked;

}

Long OnCharCommand::GetCurrentX() {
	return this->currentX;
}

Long OnCharCommand::GetCurrentY() {
	return this->currentY;
}

void OnCharCommand::SetCheck() {
	this->isChecked = true;
}

bool OnCharCommand::GetLineChanged() {
	return this->isLineChanged;
}

char OnCharCommand::GetCharacter() {
	return this->character[0];
}