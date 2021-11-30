//DeleteKeyAction.cpp
#include"DeleteKeyAction.h"
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
#include"LineChange.h"
#include"Caret.h"
#include"Observer.h"
#include"Scroll.h"
#include"SelectWords.h"
#include"LineChange.h"
#include<string>
using namespace std;
#pragma warning(disable:4996)

DeleteKeyAction::DeleteKeyAction(NotePadForm* notePadForm)
	:KeyAction(notePadForm) {
}

DeleteKeyAction::~DeleteKeyAction() {
}

void DeleteKeyAction::KeyDown() {
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

	//1. Delete키 눌렀을 때
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


	  int startX = firstX;
	  int startY = firstY;
	  int lastX = endX;
	  int lastY = endY;

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


		//4. 자동개행버튼이 눌려진 경우,
	  if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		  //4.1. 자동개행 전 firstXY, endXY의 좌표를 구한다.
		  this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
		  this->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);
		  this->notePadForm->lineChange->FindCaretBeforeLineChange(currentX, currentY, &currentX, &currentY);

		  //4.2. 자동개행된 문서를 펼친다.
		  this->notePadForm->lineChange->LineChangeButtonNotClicked();

		  //4.3. 현재 row를 구한다.
		  row = this->notePadForm->note->GetChild(currentY);
	  }

















	  //1.3. 자동개행버튼이 안눌린 경우,
	  if (this->notePadForm->isLineChangeButtonClicked == FALSE) {

		  //선택된 범위가 없을 경우,
		  if (firstX <= 0 && firstY <= 0 && endX <= 0 && endY <= 0) {
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
				  this->notePadForm->note->Remove(currentY + 1);
			  }
			  //1.4. currentX가 row의 length보다 작은 경우, currentX번째의 문자를 지운다.
			  else if (currentX < rowLength) {
				  row->Remove(currentX);
			  }
		  }

		  //선택된 범위가 있을 경우,
		  else {
			  //1.3. firstY, endY가 같은 경우,
			  if (firstY == endY) {
				  //1.3.1. endX와 firstX의 차액을 구한다.
				  difference = endX - firstX;

				  //1.3.2. 차액이 0보다 큰 동안 firstX번째 글자를 지워준다.
				  while (difference > 0) {
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
						  row->Remove(j);
						  difference--;
					  }

					  //1.4.2.4. row의 length가 0보다 작거나 같은 경우,해당 줄을 지운다.
					  if (row->GetLength() <= 0) {

						  //아예 마지막 줄이면 삭제하면 안된다. 그 이내의 줄만 ***************************
						  if (i < this->notePadForm->note->GetLength() - 1) {
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
			  this->notePadForm->note->MoveCurrent(firstY);
			  row = this->notePadForm->note->GetChild(firstY);
			  row->MoveCurrent(firstX);
		  }
	  }

#if 0
	  //1.4. 자동개행버튼이 눌린 경우,
	  else if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
	       
	       //선택된 범위가 없을 경우,
		   if (firstX <= 0 && firstY <= 0 && endX <= 0 && endY <= 0) {
			   //1.3. currentX가 row의 length보다 크거나 같은 경우,
			   if (currentX < row->GetLength()) {
				   //1.3.1. currentX번째 문자를 지운다.
				   row->Remove(currentX);

				   //1.3.2. nextRow가 dummyRow인 동안 반복한다.
				   this->notePadForm->lineChange->FillRestDummyRow(currentY);
			   }

			   //1.4. currnetX가 row의 length보다 크거나 같은 경우,
			   else if (currentX >= row->GetLength() && currentY + 1 < length) {

				   //1.4.1. nextRow의 length보다 적고, 현재 row의 width가 윈도우 너비보다 작은동안 반복한다.
				   width = this->notePadForm->getTextSize->totalCalculateX(row->GetLength(), row);
				   nextRow = this->notePadForm->note->GetChild(currentY + 1);

				   while (0 < nextRow->GetLength() && width <= this->notePadForm->scroll->windowWidth) {
					   character = nextRow->GetChild(0);
					   row->Add(character);
					   nextRow->Remove(0);
					   width = this->notePadForm->getTextSize->totalCalculateX(row->GetLength(), row);
				   }

				   //1.4.2. 총 너비를 넘긴 경우, nextRow에 다시 더한다.
				   if (width > this->notePadForm->scroll->windowWidth) {
					   nextRow->Add(0, character);
					   row->Remove(row->GetLength() - 1);
				   }
				   //#if 0
								 //1.4.3. nextRow에 length가 0보다 큰 경우,
				   if (nextRow->GetLength() > 0) {
					   //1. dummyRow를 만든다.
					   dummyRow = new DummyRow();

					   //2. 나머지 nextRow를 dummyRow에 넘겨준다.
					   i = 0;
					   while (0 < nextRow->GetLength()) {
						   character = nextRow->GetChild(0);
						   dummyRow->Add(i, character);
						   nextRow->Remove(0);
						   i++;
					   }

					   //3. currentY+1의 note에 끼운다.
					   this->notePadForm->note->Add(currentY + 1, dummyRow);

					   //4. 기존 nextRow를 지운다.
					   this->notePadForm->note->Remove(currentY + 2);

					   //5. nextRow가 dummyRow인 동안 반복한다.
					   this->notePadForm->lineChange->FillRestDummyRow(currentY + 1);

				   }
				   //#endif
			   }



		   }

		   //선택된 범위가 있을 경우,
		   else {
			   //1.4. firstY, endY가 같은 경우
			   if (firstY == endY) {
				   //1.4.1. 현재 row가 DummyRow이거나 Row인데 dummyRow가 밑에 있는 경우 true로 구분한다.
				   if (dynamic_cast<DummyRow*>(row)) {
					   isDummyRow = TRUE;
				   }
				   else {
					   if (firstY + 1 < this->notePadForm->note->GetLength()) {
						   nextRow = this->notePadForm->note->GetChild(firstY + 1);
					   }
					   else {
						   nextRow = NULL;
					   }
					   if (nextRow != NULL && dynamic_cast<DummyRow*>(nextRow)) {
						   isDummyRow = TRUE;
					   }
				   }

				   //1.4.2. endX와 firstX의 차액을 구한다.
				   difference = endX - firstX;

				   //1.4.3. 차액이 0보다 큰 동안 firstX번째 글자를 지워준다.
				   while (difference > 0) {
					   row->Remove(firstX);
					   difference--;
				   }

				   //1.4.4. isDummyRow가 true였으면 length가 0인 경우 해당 줄을 지운다.
				   if (isDummyRow == TRUE && row->GetLength() <= 0) {
					   this->notePadForm->note->Remove(firstY);
				   }
			   }

			   //1.5. endY가 firstY보다 더 큰 경우,
			   else if (endY > firstY) {
				   //#if 0
								   //1.4.1. 붙여적어야 하는 조건을 확인한다.
				   endRow = this->notePadForm->note->GetChild(endY);
				   if ((firstX > 0 && endX < endRow->GetLength())) {
					   isAppend = TRUE;
				   }
				   //#endif

								   //1.5.1 firstY가 endY보다 작거나 같은 동안 반복한다.
				   i = firstY;
				   while (i <= endY) {
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
						   row->Remove(j);
						   difference--;
					   }

					   //1.4.2.4. row의 length가 0보다 작거나 같은 경우,해당 줄을 지운다.
					   if (row->GetLength() <= 0) {

						   //아예 마지막 줄이면 삭제하면 안된다. 그 이내의 줄만 ***************************
						   if (i < this->notePadForm->note->GetLength() - 1) {
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
				   //#if 0
					//1.4.3. isAppend가 true이면. 
				   if (isAppend == TRUE) {
					   row = this->notePadForm->note->GetChild(firstY);
					   endRow = this->notePadForm->note->GetChild(firstY + 1);

					   i = 0;
					   width = this->notePadForm->getTextSize->totalCalculateX(row->GetLength(), row);

					   //1.4.3.1. firstY의 row에 붙혀적는다.
					   while (endRow->GetLength() > 0 && width <= this->notePadForm->scroll->windowWidth) {

						   character = endRow->GetChild(i);
						   row->Add(character);
						   endRow->Remove(i);

						   // row의 총 너비를 구한다.
						   width = this->notePadForm->getTextSize->totalCalculateX(row->GetLength(), row);

					   }
					   //1.4.3.2. firstY+1의 row를 지운다.
					   //this->notePadForm->note->Remove(firstY + 1);
					   if (endRow->GetLength() <= 0) {
						   this->notePadForm->note->Remove(firstY + 1);
					   }
					   else {
						   //1.5.2. FillRestDummyRow를 호출하여 남은 부분을 채워준다.
						   this->notePadForm->lineChange->FillRestDummyRow(firstY);
					   }

				   }
				   //#endif
				   //1.5.2. FillRestDummyRow를 호출하여 남은 부분을 채워준다.
				   //this->notePadForm->lineChange->FillRestDummyRow(firstY);
			   }

			   //1.5. 캐럿의 좌표를 옮긴다.
			   this->notePadForm->note->MoveCurrent(firstY);
			   row = this->notePadForm->note->GetChild(firstY);
			   row->MoveCurrent(firstX);
		   }
	  }
#endif

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















