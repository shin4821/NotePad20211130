//OnDeleteCommand.cpp 
#include"OnDeleteCommand.h"
#include"NotePadForm.h"
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
#include"SelectWords.h"
#include"LineChange.h"
#include<string>
using namespace std;
#pragma warning(disable:4996)


OnDeleteCommand::OnDeleteCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
	this->firstX = 0;
	this->firstY = 0;
	this->endX = 0;
	this->endY = 0;
	this->notePadForm->isThereSelected = false;
	this->isLineChangeButtonClicked = false;
}

OnDeleteCommand::~OnDeleteCommand() {
}

void OnDeleteCommand::Execute() {
	Long length;
	Long rowLength;
	Glyph* rowDown;
	Long rowDownLength;
	Long i = 0;
	Glyph* character = NULL;
	Glyph* nextRow;
	Glyph* dummyRow;
	Glyph* endRow;
	int width;
	BOOL isBigger = TRUE;
	//BOOL isSelected = FALSE;
	BOOL isAppend = FALSE;
	BOOL isDummyRow = FALSE;
	int difference;
	int limitX;
	int j;
	int changedX;
	int changedY;
	string content;
	int startX;
	int startY;
	int lastX;
	int lastY;
	this->character.clear();

	//1. Delete키 눌렀을 때

	//(21.11.10.추가)
	if (this->notePadForm->isOpen == TRUE) {
		this->notePadForm->isOpenChanged = TRUE;
	}

	  //1.0. 현재 캐럿의 좌표를 구한다.
	int currentY = this->notePadForm->note->GetCurrent();
	Glyph* row = this->notePadForm->note->GetChild(currentY);
	int currentX = row->GetCurrent();

	//1.1. 현재 row를 구한다.
	//1.2. currentY+1이 note의 length보다 작고, currentX가 row의 length인 경우,
	length = this->notePadForm->note->GetLength();
	rowLength = row->GetLength();

	//(21.08.19 추가) 선택하다 포함 ******************************************************
	//1.3. firstX,Y, endX,Y를 구한다.
	int firstY = this->notePadForm->selectWords->GetFirstY();
	int firstX = this->notePadForm->selectWords->GetFirstX();
	int endY = this->notePadForm->selectWords->GetEndY();
	int endX = this->notePadForm->selectWords->GetEndX();

	//1.4. 만약 선택된 단어가 있으면 멤버 좌표를 옮겨준다.
	if (this->notePadForm->isThereSelected == true) {

		firstY = this->firstY;
		firstX = this->firstX;
		endY = this->endY;
		endX = this->endX;

#if 0
		//4.1. 자동개행이 눌려져 있는 경우, 자동개행 전 firstXY, endXY의 좌표를 구한다.
		if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

			this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &startX, &startY);
			this->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &lastX, &lastY);
		}
#endif
	}


#if 0
	int startX = firstX;
	int startY = firstY;
	int lastX = endX;
	int lastY = endY;
#endif

	//1.4. isBigger이 true인지 확인한다. 
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

	//1.5. isBigger이 아니면 좌표를 서로 바꾼다.

	if (isBigger == FALSE) {
		startX = firstX;
		startY = firstY;
		lastX = endX;
		lastY = endY;

		firstX = lastX;
		firstY = lastY;
		endX = startX;
		endY = startY;
	}
	//************************************************************************************

	changedX = currentX;
	changedY = currentY;


	//3.(21.11.11.추가) 선택된 단어가 있을 경우, 자동개행이 되었든 안되었든 현재 좌표를 넘겨준다.
	if (firstX > 0 || firstY > 0 || endX > 0 || endY > 0) {
		this->firstX = firstX;
		this->firstY = firstY;
		this->endX = endX;
		this->endY = endY;

		changedX = firstX;
		changedY = firstY;
	}

	  //4. 자동개행버튼이 눌려진 경우,
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		//4.1. 자동개행 전 firstXY, endXY의 좌표를 구한다.
		this->notePadForm->lineChange->FindCaretBeforeLineChange(currentX, currentY, &currentX, &currentY);
		this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
		this->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);

		//4.2. 자동개행된 문서를 펼친다.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

		//4.3. 현재 row를 구한다.
		row = this->notePadForm->note->GetChild(currentY);

		//4.4. length와 rowLength를 자동개행 전 버전으로 구한다.
		length = this->notePadForm->note->GetLength();
		rowLength = row->GetLength();

		this->isLineChangeButtonClicked = true;
		this->windowWidth = this->notePadForm->scroll->windowWidth;
	}

	//changedX = currentX;
	//changedY = currentY;


	//5. 선택된 범위가 없을 경우,
	if (firstX <= 0 && firstY <= 0 && endX <= 0 && endY <= 0) {
		
		this->notePadForm->isThereSelected = false;

		//멤버들을 초기화해준다.
		this->firstX = 0;
		this->firstY = 0;
		this->endX = 0;
		this->endY = 0;

		if (currentY + 1 < length && currentX == rowLength) {
			//1.3.1. currentY+1의 row를 찾는다.
			rowDown = this->notePadForm->note->GetChild(currentY + 1);
			//1.3.2. rowDown의 length를 구한다.
			rowDownLength = rowDown->GetLength();
			//1.3.3. rowDown의 length만큼 반복한다.
			while (i < rowDownLength) {
				//1. rowDown의 문자를 읽는다.
				character = rowDown->GetChild(i);
				//2. row에 문자를 add한다.
				row->Add(character);
				i++;
			}
			//1.3.4. rowDown을 삭제한다.
			this->character = '\n';
			this->notePadForm->note->Remove(currentY + 1);
		}
		//1.4. currentX가 row의 length보다 작은 경우, currentX번째의 문자를 지운다.
		else if (currentX < rowLength) {
			character = row->GetChild(currentX);
			content = character->GetContent();
			this->character = content;

			row->Remove(currentX);
		}

		changedY = currentY;
		changedX = currentX;
	}

	//6. 선택된 범위가 있을 경우,
	else {
#if 0
		//해당 선택된 좌표를 멤버에 넘겨준다.
		this->firstX = firstX;
		this->firstY = firstY;
		this->endX = endX;
		this->endY = endY;
#endif

		this->notePadForm->isThereSelected = true;

		//1.3. firstY, endY가 같은 경우,
		if (firstY == endY) {
			//1.3.1. endX와 firstX의 차액을 구한다.
			difference = endX - firstX;

			//1.3.2. 차액이 0보다 큰 동안 firstX번째 글자를 지워준다.
			while (difference > 0) {
				//1.3.2.1. 문자를 지우기 전 character 멤버로 옮겨준다.****************
				character = row->GetChild(firstX);
				content = character->GetContent();
				this->character.append(content);
				//******************************************************************
				row->Remove(firstX);
				difference--;
			}
		}

		//1.4. endY가 firstY보다 더 큰 경우,
		else if (endY > firstY) {

			//1.4.1. 붙여적어야 하는 조건을 확인한다.
			endRow = this->notePadForm->note->GetChild(endY);
			if (firstX > 0 && endX < endRow->GetLength()) {
				isAppend = TRUE;
			}

			//1.4.2. firstY가 endY보다 작거나 같은 동안 반복한다.
			i = firstY;
			while (i <= endY) {
				//처음 들어올 때 이후부터 개행문자 넣어준다 **********************
				if (i > firstY) {
					this->character.append("\n");
				}
				//****************************************************************
				row = this->notePadForm->note->GetChild(i);

				//1.4.2.1. j를 정한다.
				if (i <= firstY) {
					j = firstX;
				}
				else {
					j = 0;
				}

				//1.4.2.2. limitX 를 결정한다.
				if (i >= endY) {
					limitX = endX;
				}
				else {
					limitX = row->GetLength();
				}

				difference = limitX - j;

				//1.4.2.3. difference가 0보다 큰 동안 반복한다.
				while (difference > 0) {
					// 문자를 지우기 전 character 멤버로 옮겨준다.****************
					character = row->GetChild(j);
					content = character->GetContent();
					this->character.append(content);
					//************************************************************
					row->Remove(j);
					difference--;
				}

				//1.4.2.4. row의 length가 0보다 작거나 같은 경우,해당 줄을 지운다.
				if (row->GetLength() <= 0) {

					//아예 마지막 줄이면 삭제하면 안된다. 그 이내의 줄만 ***************************
					if (i < this->notePadForm->note->GetLength() - 1) {
						// 개행문자를 character 멤버로 옮겨준다.****************
						this->character.append("\n");
						//******************************************************
						this->notePadForm->note->Remove(i);
					}
					//*******************************************************************************
					//this->notePadForm->note->Remove(i);
					endY--;
				}
				else {
					i++;
				}
			}

			//1.4.3. isAppend가 true이면. 
			if (isAppend == TRUE) {
				row = this->notePadForm->note->GetChild(firstY);
				endRow = this->notePadForm->note->GetChild(firstY + 1);
				i = 0;

				//1.4.3.1. firstY의 row에 붙혀적는다.
				while (i < endRow->GetLength()) {

					character = endRow->GetChild(i);
					row->Add(character);
					i++;
				}
				//1.4.3.2. firstY+1의 row를 지운다.
				this->notePadForm->note->Remove(firstY + 1);
			}
		}

		//1.5. 캐럿의 좌표를 옮긴다.
#if 0
		this->notePadForm->note->MoveCurrent(firstY);
		row = this->notePadForm->note->GetChild(firstY);
		row->MoveCurrent(firstX);
#endif
		changedY = firstY;
		changedX = firstX;
	}

	//7. 자동개행이 눌려진 경우,
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		//7.1. 펼친 문서를 다시 자동개행한다. (ON)
		this->notePadForm->lineChange->LineChangeButtonClicked();

		//7.2. 자동개행 후 이동할 캐럿의 위치를 구한다.
		this->notePadForm->lineChange->FindCaretAfterLineChange(changedX, changedY, &changedX, &changedY);
	}

	//8. 캐럿의 위치를 바꿔준다.
	this->notePadForm->note->MoveCurrent(changedY);
	this->notePadForm->row = this->notePadForm->note->GetChild(changedY);
	this->notePadForm->row->MoveCurrent(changedX);

	//멤버에 해당 캐럿의 위치를 넣어준다. **************************
	this->currentX = changedX;
	this->currentY = changedY;
	//**************************************************************

	//1.6. 캐럿을 이동한다.
	this->notePadForm->isOnImeCharForMove = FALSE;
	this->notePadForm->isFirstComposingForMove = FALSE;


	//2. firstXY, endXY를 초기화한다. ****************************
	this->notePadForm->selectWords->firstX = 0;
	this->notePadForm->selectWords->firstY = 0;
	this->notePadForm->selectWords->endX = 0;
	this->notePadForm->selectWords->endY = 0;
	//************************************************************


	//Scroll 업데이트 할 때의 조건을 설정한다.
	this->notePadForm->isJumpOverForPrevious = FALSE;
	this->notePadForm->isJumpOverForNext = FALSE;
	this->notePadForm->isDoubleByte = FALSE;
	this->notePadForm->isUp = FALSE;
	this->notePadForm->isDown = FALSE;

	this->notePadForm->Notify();

	this->notePadForm->Invalidate();
}

void OnDeleteCommand::Undo() {
	string temp;
	string doubleByteCharacter;
	char singleByteCharacter[2];
	singleByteCharacter[0] = this->character.at(0);
	singleByteCharacter[1] = '\0';
	int id;
	int i = 0;

	//0. 현재 캐럿의 위치를 변경해준다.
	Long currentX = this->currentX;
	Long currentY = this->currentY;

	this->notePadForm->note->MoveCurrent(currentY);
	Glyph* row = this->notePadForm->note->GetChild(currentY);
	row->MoveCurrent(currentX);
	this->notePadForm->isUndo = true;


	//1. 선택된 단어가 없으면, (한글자이면)
	if (this->notePadForm->isThereSelected == false) {

		//1.1. 현재 character에 있는 문자가 1Byte이면, OnChar을 호출한다.
		if (singleByteCharacter[0] >= 0 && singleByteCharacter[0] <= 127) {
			this->notePadForm->nChar = singleByteCharacter[0];
			id = 40001;
		}

		//1.2. 현재 character에 있는 문자가 2Byte이면, OnImeComposition 호출한다.
		else {
			this->notePadForm->letter = this->character;
			id = 40005;
		}

		//1.3.  캐럿의 크기는 Undo시 3mm로 설정한다.
		this->notePadForm->isFirstComposing = false;

		//1.4. OnImeCharCommand 또는 OnChar을 호출한다.
		this->notePadForm->SendMessage(WM_COMMAND, id);

		//1.5. 캐럿의 위치를 변경해준다.
		currentY = this->notePadForm->note->GetCurrent();
		row = this->notePadForm->note->GetChild(currentY);
		currentX = row->GetCurrent();
		currentX--;
		row->MoveCurrent(currentX);

		//1.6. Notify한다.
		// Scroll 업데이트 할 때의 조건을 설정한다.
		this->notePadForm->isJumpOverForPrevious = FALSE;
		this->notePadForm->isJumpOverForNext = FALSE;
		this->notePadForm->isDoubleByte = FALSE;
		this->notePadForm->isUp = FALSE;
		this->notePadForm->isDown = FALSE;

		this->notePadForm->Notify();

		this->notePadForm->Invalidate();

	}

	//2. 선택된 단어가 있으면,
	else {
		//2.1. character의 length만큼 반복한다.
		while (i < this->character.size()) {

			//2.1.0.  캐럿의 크기는 Undo시 3mm로 설정한다.
			this->notePadForm->isFirstComposing = false;

			//2.1.1. 1Byte인지 2Byte인지 확인한다.
			singleByteCharacter[0] = this->character.at(i);

			//2.1.2. 현재 character에 있는 문자가 1Byte이면, OnChar을 호출한다.
			if (singleByteCharacter[0] >= 0 && singleByteCharacter[0] <= 127) {

				//2.1.2.1. 개행문자이면 좌표를 currentY++, currentX=0 으로 변환해준다.
#if 0
				if (singleByteCharacter[0] == '\n') {
					currentY++;
					currentX = 0;

					this->notePadForm->note->MoveCurrent(currentY);
					Glyph* row = this->notePadForm->note->GetChild(currentY);
					row->MoveCurrent(currentX);
				}
#endif
				this->notePadForm->nChar = singleByteCharacter[0];
				this->notePadForm->SendMessage(WM_COMMAND, 40001);
			}
			//2.1.3. 현재 character에 있는 문자가 2Byte이면, OnImeComposition 호출한다.
			else {
				doubleByteCharacter.clear();
				temp = this->character.at(i);
				doubleByteCharacter.append(temp);
				temp = this->character.at(i + 1);
				doubleByteCharacter.append(temp);

				this->notePadForm->letter = doubleByteCharacter;
				this->notePadForm->SendMessage(WM_COMMAND, 40005);
				i++;
			}
			i++;
		}


		//2.1. firstXY, endXY를 설정해준다.
		this->notePadForm->selectWords->firstX = this->firstX;
		this->notePadForm->selectWords->firstY = this->firstY;
		this->notePadForm->selectWords->endX = this->endX;
		this->notePadForm->selectWords->endY = this->endY;

	}

	this->notePadForm->isUndo = false;
	//this->notePadForm->isThereSelected == false;

}

void OnDeleteCommand::Redo() {

	//1. 현재 캐럿의 위치를 변경해준다.
	Long currentX = this->currentX;
	Long currentY = this->currentY;

	this->notePadForm->note->MoveCurrent(currentY);
	Glyph* row = this->notePadForm->note->GetChild(currentY);
	row->MoveCurrent(currentX);

#if 0
	if (this->notePadForm->isThereSelected == true) {


	}
#endif

	//캐럿의 크기는 Undo시 3mm로 설정한다.
	this->notePadForm->isFirstComposingForMove = false;
	this->notePadForm->isOnImeCharForMove = false;

	Execute();
}

Long OnDeleteCommand::GetCurrentX() {
	return this->currentX;
}

Long OnDeleteCommand::GetCurrentY() {
	return this->currentY;
}

void OnDeleteCommand::SetCheck() {
	this->isChecked = true;
}

bool OnDeleteCommand::GetChecked() {
	return this->isChecked;
}




