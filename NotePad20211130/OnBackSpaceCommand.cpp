//OnBackSpaceCommand.cpp
#include"OnBackSpaceCommand.h"
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

OnBackSpaceCommand::OnBackSpaceCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
	this->isChecked = false;
	this->notePadForm->isUndo = false;
	//this->character.clear();
	this->notePadForm->isThereSelected = false;
	this->firstX = 0;
	this->firstY = 0;
	this->endX = 0;
	this->endY = 0;
	this->isLineChangeButtonClicked = false;
}

OnBackSpaceCommand::~OnBackSpaceCommand() {
}


void OnBackSpaceCommand::Execute() {

#if 0
	//this->factoryProduct = new FactoryProduct(this);
	keyAction = factoryProduct.Make(nChar, NULL, NULL);
	keyAction->KeyDown();
#endif

	Long i = 0;
	Long j;
	Glyph* character = NULL;
	Glyph* row;
	Glyph* rowUp;
	Glyph* dummyRow;
	Glyph* nextRow;
	Glyph* endRow;
	Long length;
	Long lengthUp;
	char chaCharacter[3];
	char characterMove[3];
	int changedX = 0;
	int changedY = 0;
	string strCharacter;
	Long caretX;
	int width;
	int x = 0;
	BOOL isBigger = TRUE;
	//BOOL isSelected = FALSE;
	BOOL isAppend = FALSE;
	BOOL isDummyRow = FALSE;
	int difference;
	int limitX;
	int startX;
	int startY;
	int lastX;
	int lastY;
	BOOL isLineChanged = FALSE;
	int lineChangeCount = 0; //(21.11.18.추가)


	//0. BackSpace를 눌렀을 때

	 //21.08.05 추가
	this->notePadForm->scroll->beforeCurrentY = this->notePadForm->note->GetCurrent();
	Long beforeCurrentY = this->notePadForm->note->GetCurrent();

	//1. 현재 캐럿의 좌표를 구한다.
	int currentY = this->notePadForm->note->GetCurrent();

	//2. 현재 row를 구한다.
	row = this->notePadForm->note->GetChild(currentY);
	int currentX = row->GetCurrent();

	//(21.08.19 추가) 선택하다 포함 ******************************************************
	//3. firstX,Y, endX,Y를 구한다.
	int firstY = this->notePadForm->selectWords->GetFirstY();
	int firstX = this->notePadForm->selectWords->GetFirstX();
	int endY = this->notePadForm->selectWords->GetEndY();
	int endX = this->notePadForm->selectWords->GetEndX();


	//4. 만약 선택된 단어가 있으면 멤버 좌표를 옮겨준다.
	if (this->notePadForm->isThereSelected == true) {

		firstY = this->firstY;
		firstX = this->firstX;
		endY = this->endY;
		endX = this->endX;

		//4.1. 자동개행이 눌려져 있는 경우, 자동개행 전 firstXY, endXY의 좌표를 구한다.
		if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
			
#if 0
			this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
			this->notePadForm->lineChange->FindCaretAfterLineChange(endX, endY, &endX, &endY);
#endif
			this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &startX, &startY);
			this->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &lastX, &lastY);
		}
	}

#if 0
	int startX = firstX;
	int startY = firstY;
	int lastX = endX;
	int lastY = endY;
#endif
	//3.1. isBigger이 true인지 확인한다. 
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

	//3.2. isBigger이 아니면 좌표를 서로 바꾼다.

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

	//3.(21.11.02.추가) 선택된 단어가 있을 경우, 자동개행이 되었든 안되었든 현재 좌표를 넘겨준다.
	if (firstX > 0 || firstY > 0 || endX > 0 || endY > 0) {
		this->firstX = firstX;
		this->firstY = firstY;
		this->endX = endX;
		this->endY = endY;
	}


	//4. 자동개행버튼이 눌려진 경우,
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

#if 0
		//(21.11.02 수정) 그냥 자동개행 되었을때의 그 좌표를 넘겨보기로 함...
        //해당 선택된 좌표를 멤버에 넘겨준다.
		this->firstX = firstX;
		this->firstY = firstY;
		this->endX = endX;
		this->endY = endY;
#endif

		//4.1. 자동개행 전 firstXY, endXY의 좌표를 구한다.
		this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
		this->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);
		this->notePadForm->lineChange->FindCaretBeforeLineChange(currentX, currentY, &currentX, &currentY);

		//4.2. 자동개행된 문서를 펼친다.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

		//4.3. 현재 row를 구한다.
		row = this->notePadForm->note->GetChild(currentY);
		this->isLineChangeButtonClicked = true;
		this->windowWidth = this->notePadForm->scroll->windowWidth;
	}

	//(21.11.18.추가) 4.총 몇개의 줄을 지워야 하는지 미리 계산해준다.
	lineChangeCount = endY - firstY;


	//5. 선택된 범위가 없을 경우,
	if (firstX <= 0 && firstY <= 0 && endX <= 0 && endY <= 0) {

		//멤버들을 초기화해준다.
		this->firstX = 0;
		this->firstY = 0;
		this->endX = 0;
		this->endY = 0;

		//5.1. currentY가 0보다 크고, currentX가 0인 경우,
		if (currentY > 0 && currentX <= 0) {
			length = row->GetLength();
			rowUp = this->notePadForm->note->GetChild(currentY - 1);
			lengthUp = rowUp->GetLength();

			//5.1.1. 해당 row의 length가 0보다 큰 경우,
			if (length > 0) {
				//5.1.1.1. length만큼 반복한다.
				while (i < length) {
					//1. currentY의 row의 문자를 읽는다.
					character = row->GetChild(i);


					//개행문자를 character 멤버로 준다.****************
					this->character = '\n';
					//*************************************************


					//2. currentY-1의 row에 문자를 add한다.
					rowUp->Add(character);
					i++;
				}
				//5.1.1.2. 캐럿의 Y를 currentY-1,X를 lengthUp으로 옮긴다.
				changedY = currentY - 1;
				changedX = lengthUp;

				//5.1.1.3. currentY의 row를 삭제한다.
				this->notePadForm->note->Remove(currentY);
			}


			//5.1.2. 해당 row의 length가 0과 같거나 작은 경우,
			else {
				//5.2.0. 문자를 지우기 전 character 멤버로 옮겨준다.****************
				this->character = '\n';
				//******************************************************************

				//5.1.2.1. 해당 row를 삭제한다.
				this->notePadForm->note->Remove(currentY);
				//5.1.2.2. 캐럿의 Y를 currentY-1, X를 lengthUp으로 이동한다.
				changedY = currentY - 1;
				changedX = lengthUp;
			}
		}
		//5.2. currentX가 0보다 큰 경우,
		else if (currentX > 0) {
			//5.2.0. 문자를 지우기 전 character 멤버로 옮겨준다.****************
			character = row->GetChild(currentX - 1);
			strCharacter = character->GetContent();
			this->character = strCharacter;
			//******************************************************************


			//5.2.1. currentX-1번째 문자를 지운다.
			row->Remove(currentX - 1);

			//5.2.2. currentX-1번째로 캐럿을 이동한다.
			changedY = currentY;
			changedX = currentX - 1;
		}
	}

	//6. 선택된 범위가 있을 경우,
	else {
#if 0
		//(21.11.02 수정) 그냥 자동개행 되었을때의 그 좌표를 넘겨보기로 함...
		//해당 선택된 좌표를 멤버에 넘겨준다.
		this->firstX = firstX;
		this->firstY = firstY;
		this->endX = endX;
		this->endY = endY;
#endif
		//4.3. 현재 row를 구한다.
		row = this->notePadForm->note->GetChild(firstY);

		//선택 여부를 TRUE로 바꾼다.
		this->notePadForm->isThereSelected = true;

		//character 멤버를 초기화해준다.
		this->character.clear();

		//1.3. firstY, endY가 같은 경우,
		if (firstY == endY) {
			//1.3.1. endX와 firstX의 차액을 구한다.
			difference = endX - firstX;

			//1.3.2. 차액이 0보다 큰 동안 firstX번째 글자를 지워준다.
			while (difference > 0) {
				//1.3.2.1. 문자를 지우기 전 character 멤버로 옮겨준다.****************
				character = row->GetChild(firstX);
				strCharacter = character->GetContent();
				this->character.append(strCharacter);
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
				if (i > firstY && isLineChanged == FALSE) {
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
					strCharacter = character->GetContent();
					this->character.append(strCharacter);
					//************************************************************

					row->Remove(j);
					difference--;

				}

				//1.4.2.4. row의 length가 0보다 작거나 같은 경우,해당 줄을 지운다.
				if (row->GetLength() <= 0 && lineChangeCount>0 ) {

					this->notePadForm->note->Remove(i);

					if (i < endY) {
	                    // 개행문자를 character 멤버로 옮겨준다.****************
						this->character.append("\n");// \n
						//******************************************************
					}

					endY--;
					lineChangeCount--;
					isLineChanged = TRUE;
				}
				else {

					isLineChanged = FALSE;
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

			    // 개행문자를 character 멤버로 옮겨준다.****************
				//this->character.append("\n");
				//******************************************************
				this->notePadForm->note->Remove(firstY + 1);
			}
		}

		//1.5. 캐럿의 옮길 좌표를 정한다.
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

	// Scroll 업데이트 할 때의 조건을 설정한다.
	this->notePadForm->isJumpOverForPrevious = TRUE;
	this->notePadForm->isJumpOverForNext = FALSE;
	this->notePadForm->isDoubleByte = FALSE;
	this->notePadForm->isUp = FALSE;
	this->notePadForm->isDown = FALSE;

	//(21.10.01 추가)
	//this->isLineChangeButtonClicked = this->notePadForm->isLineChangeButtonClicked;

	//(21.11.10.추가)
	if (this->notePadForm->isOpen == TRUE) {
		this->notePadForm->isOpenChanged = TRUE;
	}

	if (beforeCurrentY > changedY) {
		this->notePadForm->isJumpOverForPrevious = FALSE;
	}
	this->notePadForm->Notify();

	this->notePadForm->Invalidate();


}

void OnBackSpaceCommand::Undo() {
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

	this->notePadForm->selectWords->firstX = 0;
	this->notePadForm->selectWords->firstY = 0;
	this->notePadForm->selectWords->endX = 0;
	this->notePadForm->selectWords->endY = 0;


	//1. (21.09.30 추가) 현재 자동개행버튼 상황과 적을때의 상황이 다르면,
	if (this->notePadForm->isLineChangeButtonClicked != this->isLineChangeButtonClicked) {

		//1.1. 현재 자동개행버튼이 눌려져 있으면, 자동개행 후의 좌표를 구한다. 
		if (this->notePadForm->isLineChangeButtonClicked == true) {
			this->notePadForm->lineChange->FindCaretAfterLineChange(currentX, currentY, &currentX, &currentY);
			//this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
			//this->notePadForm->lineChange->FindCaretAfterLineChange(endX, endY, &endX, &endY);
		}
		//1.2. 현재 자동개행버튼이 안눌려져 있으면, 자동개행 전의 좌표를 구한다.
		else {
			//1.2.1. 자동개행 시켜준다.
			this->notePadForm->lineChange->LineChangeButtonClicked(this->windowWidth); //-25

			//1.2.2. 자동개행 전 좌표 구한다.
			this->notePadForm->lineChange->FindCaretBeforeLineChange(currentX, currentY, &currentX, &currentY);
			//this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
			//this->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);
			
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
		this->notePadForm->lineChange->FindCaretBeforeLineChange(currentX, currentY, &currentX, &currentY);
		//this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
		//this->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);

		//2.4. 자동개행된 문서를 펼친다.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

		//2.5. 펼친 문서를 현재 windowWidth만큼 자동개행한다.
		this->notePadForm->lineChange->LineChangeButtonClicked(windowWidth);

		//2.6. 자동개행 후 좌표를 구한다.
		this->notePadForm->lineChange->FindCaretAfterLineChange(currentX, currentY, &currentX, &currentY);
		//this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
		//this->notePadForm->lineChange->FindCaretAfterLineChange(endX, endY, &endX, &endY);
	}
#if 0
	else if(this->isLineChangeButtonClicked == true) {

		//자동개행 후 좌표를 구한다.
		this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
		this->notePadForm->lineChange->FindCaretAfterLineChange(endX, endY, &endX, &endY);
	}
#endif

	//0. 현재 캐럿의 위치를 변경해준다.
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
				
				singleByteCharacter[1] = '\0';

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
				//this->notePadForm->nChar = singleByteCharacter[0];
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
#if 0
		//2.1. firstXY, endXY를 설정해준다.
		this->notePadForm->selectWords->firstX = firstX;
		this->notePadForm->selectWords->firstY = firstY;
		this->notePadForm->selectWords->endX = endX;
		this->notePadForm->selectWords->endY = endY;

		this->firstX = firstX;
		this->firstY = firstY;
		this->endX = endX;
		this->endY = endY;
#endif
	}

#if 0
	//3. Undo 후 바뀐 캐럿의 위치를 적용한다.
	this->currentX = this->notePadForm->currentX;
	this->currentY = this->notePadForm->currentY;

#endif


	//1. (21.09.30 추가) 현재 자동개행버튼 상황과 적을때의 상황이 다르면,
	if (this->notePadForm->isLineChangeButtonClicked != this->isLineChangeButtonClicked) {

		//1.1. 현재 자동개행버튼이 눌려져 있으면, 자동개행 후의 좌표를 구한다. 
		if (this->notePadForm->isLineChangeButtonClicked == true) {
			//this->notePadForm->lineChange->FindCaretAfterLineChange(currentX, currentY, &currentX, &currentY);
			this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
			this->notePadForm->lineChange->FindCaretAfterLineChange(endX, endY, &endX, &endY);
		}
		//1.2. 현재 자동개행버튼이 안눌려져 있으면, 자동개행 전의 좌표를 구한다.
		else {
			//1.2.1. 자동개행 시켜준다.
			this->notePadForm->lineChange->LineChangeButtonClicked(this->windowWidth); //-25
			//1.2.2. 자동개행 전 좌표 구한다.
			//this->notePadForm->lineChange->FindCaretBeforeLineChange(currentX, currentY, &currentX, &currentY);
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
		//this->notePadForm->lineChange->FindCaretBeforeLineChange(currentX, currentY, &currentX, &currentY);
		this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
		this->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);

		//2.4. 자동개행된 문서를 펼친다.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

		//2.5. 펼친 문서를 현재 windowWidth만큼 자동개행한다.
		this->notePadForm->lineChange->LineChangeButtonClicked(windowWidth);

		//2.6. 자동개행 후 좌표를 구한다.
		//this->notePadForm->lineChange->FindCaretAfterLineChange(currentX, currentY, &currentX, &currentY);
		this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
		this->notePadForm->lineChange->FindCaretAfterLineChange(endX, endY, &endX, &endY);
	}

#if 0
	if (this->isLineChangeButtonClicked == true) {

		//자동개행 후 좌표를 구한다.
		this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
		this->notePadForm->lineChange->FindCaretAfterLineChange(endX, endY, &endX, &endY);
	}
#endif


	if (this->notePadForm->isThereSelected == true) {

		//2.1. firstXY, endXY를 설정해준다.
		this->notePadForm->selectWords->firstX = firstX;
		this->notePadForm->selectWords->firstY = firstY;
		this->notePadForm->selectWords->endX = endX;
		this->notePadForm->selectWords->endY = endY;


		this->firstX = firstX;
		this->firstY = firstY;
		this->endX = endX;
		this->endY = endY;
	}


	//3. Undo 후 바뀐 캐럿의 위치를 적용한다.
	this->currentX = this->notePadForm->currentX;
	this->currentY = this->notePadForm->currentY;
	
	this->notePadForm->isUndo = false;
	//this->notePadForm->isThereSelected == false;

	//(21.11.02. 추가) 현재 자동개행버튼 상황을 적용시킨다.
	this->isLineChangeButtonClicked = this->notePadForm->isLineChangeButtonClicked;
	this->windowWidth = windowWidth;
}

void OnBackSpaceCommand::Redo() {

	int currentX = this->currentX;
	int currentY = this->currentY;
	int firstX = this->firstX;
	int firstY = this->firstY;
	int endX = this->endX;
	int endY = this->endY;
	int windowWidth = this->notePadForm->scroll->windowWidth; //현재 윈도우너비
#if 0
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
#endif

#if 0
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
#endif

	//1. (21.11.02. 추가) 현재 자동개행버튼 상황과 적을때의 상황이 다르면,
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
			this->notePadForm->lineChange->LineChangeButtonClicked(this->windowWidth); //-25
			
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
		this->notePadForm->lineChange->LineChangeButtonClicked(this->windowWidth); //-25

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

	//(21.10.29. 추가) 1. 선택된 단어가 있으면 endX, endY로 위치를 변경해준다.
	if (firstX > 0 || firstY > 0 || endX > 0 || endY > 0) {

		//1.1. 현재 캐럿의 위치를 변경해준다.
		this->notePadForm->note->MoveCurrent(endY); 
		Glyph* row = this->notePadForm->note->GetChild(endY);
		row->MoveCurrent(endX);
	}
	//(21.10.29. 추가) 2. 선택된 단어가 없으면 currentX, currentY로 위치를 변경해준다.
	else {

		//2.1. 현재 캐럿의 위치를 변경해준다.
		this->notePadForm->note->MoveCurrent(currentY); 
		Glyph* row = this->notePadForm->note->GetChild(currentY);
		row->MoveCurrent(currentX);
	}



	//2. Execute 하기 전에 좌표를 다시 정해준다.
	this->notePadForm->selectWords->firstX = firstX;
	this->notePadForm->selectWords->firstY = firstY;
	this->notePadForm->selectWords->endX = endX;
	this->notePadForm->selectWords->endY = endY;

	this->firstX = firstX;
	this->firstY = firstY;
	this->endX = endX;
	this->endY = endY;

#if 0
	if (this->notePadForm->isThereSelected == true) {
	
	
	}
#endif


	//캐럿의 크기는 Undo시 3mm로 설정한다.
	this->notePadForm->isFirstComposingForMove = false;
	this->notePadForm->isOnImeCharForMove = false;

	//(21.10.01 추가)
	this->isLineChangeButtonClicked = this->notePadForm->isLineChangeButtonClicked;
	this->windowWidth = windowWidth;

	Execute();
}

Long OnBackSpaceCommand::GetCurrentX() {
	return this->currentX;
}

Long OnBackSpaceCommand::GetCurrentY() {
	return this->currentY;
}

string OnBackSpaceCommand::GetCharacterForAll() {
	return this->character;
}

void OnBackSpaceCommand::SetCheck() {
	this->isChecked = true;
}


bool OnBackSpaceCommand::GetChecked() {
	return this->isChecked;
}

