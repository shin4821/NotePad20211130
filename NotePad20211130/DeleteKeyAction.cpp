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

	//1. DeleteŰ ������ ��
	  //1.0. ���� ĳ���� ��ǥ�� ���Ѵ�.
	  int currentY = this->notePadForm->note->GetCurrent();
	  Glyph* row = this->notePadForm->note->GetChild(currentY);
	  int currentX = row->GetCurrent();

	  //1.1. ���� row�� ���Ѵ�.
	  //1.2. currentY+1�� note�� length���� �۰�, currentX�� row�� length�� ���,
	  length = this->notePadForm->note->GetLength();
	  rowLength = row->GetLength();

	  //(21.08.19 �߰�) �����ϴ� ���� ******************************************************
      //1.3. firstX,Y, endX,Y�� ���Ѵ�.
	  int firstY = this->notePadForm->selectWords->GetFirstY();
	  int firstX = this->notePadForm->selectWords->GetFirstX();
	  int endY = this->notePadForm->selectWords->GetEndY();
	  int endX = this->notePadForm->selectWords->GetEndX();


	  int startX = firstX;
	  int startY = firstY;
	  int lastX = endX;
	  int lastY = endY;

	  //1.4. isBigger�� true���� Ȯ���Ѵ�. 
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

	  //1.5. isBigger�� �ƴϸ� ��ǥ�� ���� �ٲ۴�.

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


		//4. �ڵ������ư�� ������ ���,
	  if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		  //4.1. �ڵ����� �� firstXY, endXY�� ��ǥ�� ���Ѵ�.
		  this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
		  this->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);
		  this->notePadForm->lineChange->FindCaretBeforeLineChange(currentX, currentY, &currentX, &currentY);

		  //4.2. �ڵ������ ������ ��ģ��.
		  this->notePadForm->lineChange->LineChangeButtonNotClicked();

		  //4.3. ���� row�� ���Ѵ�.
		  row = this->notePadForm->note->GetChild(currentY);
	  }

















	  //1.3. �ڵ������ư�� �ȴ��� ���,
	  if (this->notePadForm->isLineChangeButtonClicked == FALSE) {

		  //���õ� ������ ���� ���,
		  if (firstX <= 0 && firstY <= 0 && endX <= 0 && endY <= 0) {
			  if (currentY + 1 < length && currentX == rowLength) {
				  //1.3.1. currentY+1�� row�� ã�´�.
				  rowDown = this->notePadForm->note->GetChild(currentY + 1);
				  //1.3.2. rowDown�� length�� ���Ѵ�.
				  rowDownLength = rowDown->GetLength();
				  //1.3.3. rowDown�� length��ŭ �ݺ��Ѵ�.
				  while (i < rowDownLength) {
					  //1. rowDown�� ���ڸ� �д´�.
					  character = rowDown->GetChild(i);
					  //2. row�� ���ڸ� add�Ѵ�.
					  row->Add(character);
					  i++;
				  }
				  //1.3.4. rowDown�� �����Ѵ�.
				  this->notePadForm->note->Remove(currentY + 1);
			  }
			  //1.4. currentX�� row�� length���� ���� ���, currentX��°�� ���ڸ� �����.
			  else if (currentX < rowLength) {
				  row->Remove(currentX);
			  }
		  }

		  //���õ� ������ ���� ���,
		  else {
			  //1.3. firstY, endY�� ���� ���,
			  if (firstY == endY) {
				  //1.3.1. endX�� firstX�� ������ ���Ѵ�.
				  difference = endX - firstX;

				  //1.3.2. ������ 0���� ū ���� firstX��° ���ڸ� �����ش�.
				  while (difference > 0) {
					  row->Remove(firstX);
					  difference--;
				  }
			  }

			  //1.4. endY�� firstY���� �� ū ���,
			  else if (endY > firstY) {

				  //1.4.1. �ٿ������ �ϴ� ������ Ȯ���Ѵ�.
				  endRow = this->notePadForm->note->GetChild(endY);
				  if (firstX > 0 && endX < endRow->GetLength()) {
					  isAppend = TRUE;
				  }

				  //1.4.2. firstY�� endY���� �۰ų� ���� ���� �ݺ��Ѵ�.
				  i = firstY;
				  while (i <= endY) {
					  row = this->notePadForm->note->GetChild(i);

					  //1.4.2.1. j�� ���Ѵ�.
					  if (i <= firstY) {
						  j = firstX;
					  }
					  else {
						  j = 0;
					  }

					  //1.4.2.2. limitX �� �����Ѵ�.
					  if (i >= endY) {
						  limitX = endX;
					  }
					  else {
						  limitX = row->GetLength();
					  }

					  difference = limitX - j;

					  //1.4.2.3. difference�� 0���� ū ���� �ݺ��Ѵ�.
					  while (difference > 0) {
						  row->Remove(j);
						  difference--;
					  }

					  //1.4.2.4. row�� length�� 0���� �۰ų� ���� ���,�ش� ���� �����.
					  if (row->GetLength() <= 0) {

						  //�ƿ� ������ ���̸� �����ϸ� �ȵȴ�. �� �̳��� �ٸ� ***************************
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

				  //1.4.3. isAppend�� true�̸�. 
				  if (isAppend == TRUE) {
					  row = this->notePadForm->note->GetChild(firstY);
					  endRow = this->notePadForm->note->GetChild(firstY + 1);
					  i = 0;

					  //1.4.3.1. firstY�� row�� �������´�.
					  while (i < endRow->GetLength()) {
						  character = endRow->GetChild(i);
						  row->Add(character);
						  i++;
					  }
					  //1.4.3.2. firstY+1�� row�� �����.
					  this->notePadForm->note->Remove(firstY + 1);
				  }
			  }

			  //1.5. ĳ���� ��ǥ�� �ű��.
			  this->notePadForm->note->MoveCurrent(firstY);
			  row = this->notePadForm->note->GetChild(firstY);
			  row->MoveCurrent(firstX);
		  }
	  }

#if 0
	  //1.4. �ڵ������ư�� ���� ���,
	  else if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
	       
	       //���õ� ������ ���� ���,
		   if (firstX <= 0 && firstY <= 0 && endX <= 0 && endY <= 0) {
			   //1.3. currentX�� row�� length���� ũ�ų� ���� ���,
			   if (currentX < row->GetLength()) {
				   //1.3.1. currentX��° ���ڸ� �����.
				   row->Remove(currentX);

				   //1.3.2. nextRow�� dummyRow�� ���� �ݺ��Ѵ�.
				   this->notePadForm->lineChange->FillRestDummyRow(currentY);
			   }

			   //1.4. currnetX�� row�� length���� ũ�ų� ���� ���,
			   else if (currentX >= row->GetLength() && currentY + 1 < length) {

				   //1.4.1. nextRow�� length���� ����, ���� row�� width�� ������ �ʺ񺸴� �������� �ݺ��Ѵ�.
				   width = this->notePadForm->getTextSize->totalCalculateX(row->GetLength(), row);
				   nextRow = this->notePadForm->note->GetChild(currentY + 1);

				   while (0 < nextRow->GetLength() && width <= this->notePadForm->scroll->windowWidth) {
					   character = nextRow->GetChild(0);
					   row->Add(character);
					   nextRow->Remove(0);
					   width = this->notePadForm->getTextSize->totalCalculateX(row->GetLength(), row);
				   }

				   //1.4.2. �� �ʺ� �ѱ� ���, nextRow�� �ٽ� ���Ѵ�.
				   if (width > this->notePadForm->scroll->windowWidth) {
					   nextRow->Add(0, character);
					   row->Remove(row->GetLength() - 1);
				   }
				   //#if 0
								 //1.4.3. nextRow�� length�� 0���� ū ���,
				   if (nextRow->GetLength() > 0) {
					   //1. dummyRow�� �����.
					   dummyRow = new DummyRow();

					   //2. ������ nextRow�� dummyRow�� �Ѱ��ش�.
					   i = 0;
					   while (0 < nextRow->GetLength()) {
						   character = nextRow->GetChild(0);
						   dummyRow->Add(i, character);
						   nextRow->Remove(0);
						   i++;
					   }

					   //3. currentY+1�� note�� �����.
					   this->notePadForm->note->Add(currentY + 1, dummyRow);

					   //4. ���� nextRow�� �����.
					   this->notePadForm->note->Remove(currentY + 2);

					   //5. nextRow�� dummyRow�� ���� �ݺ��Ѵ�.
					   this->notePadForm->lineChange->FillRestDummyRow(currentY + 1);

				   }
				   //#endif
			   }



		   }

		   //���õ� ������ ���� ���,
		   else {
			   //1.4. firstY, endY�� ���� ���
			   if (firstY == endY) {
				   //1.4.1. ���� row�� DummyRow�̰ų� Row�ε� dummyRow�� �ؿ� �ִ� ��� true�� �����Ѵ�.
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

				   //1.4.2. endX�� firstX�� ������ ���Ѵ�.
				   difference = endX - firstX;

				   //1.4.3. ������ 0���� ū ���� firstX��° ���ڸ� �����ش�.
				   while (difference > 0) {
					   row->Remove(firstX);
					   difference--;
				   }

				   //1.4.4. isDummyRow�� true������ length�� 0�� ��� �ش� ���� �����.
				   if (isDummyRow == TRUE && row->GetLength() <= 0) {
					   this->notePadForm->note->Remove(firstY);
				   }
			   }

			   //1.5. endY�� firstY���� �� ū ���,
			   else if (endY > firstY) {
				   //#if 0
								   //1.4.1. �ٿ������ �ϴ� ������ Ȯ���Ѵ�.
				   endRow = this->notePadForm->note->GetChild(endY);
				   if ((firstX > 0 && endX < endRow->GetLength())) {
					   isAppend = TRUE;
				   }
				   //#endif

								   //1.5.1 firstY�� endY���� �۰ų� ���� ���� �ݺ��Ѵ�.
				   i = firstY;
				   while (i <= endY) {
					   row = this->notePadForm->note->GetChild(i);

					   //1.4.2.1. j�� ���Ѵ�.
					   if (i <= firstY) {
						   j = firstX;
					   }
					   else {
						   j = 0;
					   }

					   //1.4.2.2. limitX �� �����Ѵ�.
					   if (i >= endY) {
						   limitX = endX;
					   }
					   else {
						   limitX = row->GetLength();
					   }

					   difference = limitX - j;

					   //1.4.2.3. difference�� 0���� ū ���� �ݺ��Ѵ�.
					   while (difference > 0) {
						   row->Remove(j);
						   difference--;
					   }

					   //1.4.2.4. row�� length�� 0���� �۰ų� ���� ���,�ش� ���� �����.
					   if (row->GetLength() <= 0) {

						   //�ƿ� ������ ���̸� �����ϸ� �ȵȴ�. �� �̳��� �ٸ� ***************************
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
					//1.4.3. isAppend�� true�̸�. 
				   if (isAppend == TRUE) {
					   row = this->notePadForm->note->GetChild(firstY);
					   endRow = this->notePadForm->note->GetChild(firstY + 1);

					   i = 0;
					   width = this->notePadForm->getTextSize->totalCalculateX(row->GetLength(), row);

					   //1.4.3.1. firstY�� row�� �������´�.
					   while (endRow->GetLength() > 0 && width <= this->notePadForm->scroll->windowWidth) {

						   character = endRow->GetChild(i);
						   row->Add(character);
						   endRow->Remove(i);

						   // row�� �� �ʺ� ���Ѵ�.
						   width = this->notePadForm->getTextSize->totalCalculateX(row->GetLength(), row);

					   }
					   //1.4.3.2. firstY+1�� row�� �����.
					   //this->notePadForm->note->Remove(firstY + 1);
					   if (endRow->GetLength() <= 0) {
						   this->notePadForm->note->Remove(firstY + 1);
					   }
					   else {
						   //1.5.2. FillRestDummyRow�� ȣ���Ͽ� ���� �κ��� ä���ش�.
						   this->notePadForm->lineChange->FillRestDummyRow(firstY);
					   }

				   }
				   //#endif
				   //1.5.2. FillRestDummyRow�� ȣ���Ͽ� ���� �κ��� ä���ش�.
				   //this->notePadForm->lineChange->FillRestDummyRow(firstY);
			   }

			   //1.5. ĳ���� ��ǥ�� �ű��.
			   this->notePadForm->note->MoveCurrent(firstY);
			   row = this->notePadForm->note->GetChild(firstY);
			   row->MoveCurrent(firstX);
		   }
	  }
#endif

	  //2. firstXY, endXY�� �ʱ�ȭ�Ѵ�. ****************************
	  this->notePadForm->selectWords->firstX = 0;
	  this->notePadForm->selectWords->firstY = 0;
	  this->notePadForm->selectWords->endX = 0;
	  this->notePadForm->selectWords->endY = 0;
	  //************************************************************


	  //Scroll ������Ʈ �� ���� ������ �����Ѵ�.
	  this->notePadForm->isJumpOverForPrevious = FALSE;
	  this->notePadForm->isJumpOverForNext = FALSE;
	  this->notePadForm->isDoubleByte = FALSE;
	  this->notePadForm->isUp = FALSE;
	  this->notePadForm->isDown = FALSE;

	  this->notePadForm->Notify();

	  this->notePadForm->Invalidate();
}















