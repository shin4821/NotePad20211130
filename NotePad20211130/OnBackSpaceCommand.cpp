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
	int lineChangeCount = 0; //(21.11.18.�߰�)


	//0. BackSpace�� ������ ��

	 //21.08.05 �߰�
	this->notePadForm->scroll->beforeCurrentY = this->notePadForm->note->GetCurrent();
	Long beforeCurrentY = this->notePadForm->note->GetCurrent();

	//1. ���� ĳ���� ��ǥ�� ���Ѵ�.
	int currentY = this->notePadForm->note->GetCurrent();

	//2. ���� row�� ���Ѵ�.
	row = this->notePadForm->note->GetChild(currentY);
	int currentX = row->GetCurrent();

	//(21.08.19 �߰�) �����ϴ� ���� ******************************************************
	//3. firstX,Y, endX,Y�� ���Ѵ�.
	int firstY = this->notePadForm->selectWords->GetFirstY();
	int firstX = this->notePadForm->selectWords->GetFirstX();
	int endY = this->notePadForm->selectWords->GetEndY();
	int endX = this->notePadForm->selectWords->GetEndX();


	//4. ���� ���õ� �ܾ ������ ��� ��ǥ�� �Ű��ش�.
	if (this->notePadForm->isThereSelected == true) {

		firstY = this->firstY;
		firstX = this->firstX;
		endY = this->endY;
		endX = this->endX;

		//4.1. �ڵ������� ������ �ִ� ���, �ڵ����� �� firstXY, endXY�� ��ǥ�� ���Ѵ�.
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
	//3.1. isBigger�� true���� Ȯ���Ѵ�. 
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

	//3.2. isBigger�� �ƴϸ� ��ǥ�� ���� �ٲ۴�.

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

	//3.(21.11.02.�߰�) ���õ� �ܾ ���� ���, �ڵ������� �Ǿ��� �ȵǾ��� ���� ��ǥ�� �Ѱ��ش�.
	if (firstX > 0 || firstY > 0 || endX > 0 || endY > 0) {
		this->firstX = firstX;
		this->firstY = firstY;
		this->endX = endX;
		this->endY = endY;
	}


	//4. �ڵ������ư�� ������ ���,
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

#if 0
		//(21.11.02 ����) �׳� �ڵ����� �Ǿ������� �� ��ǥ�� �Ѱܺ���� ��...
        //�ش� ���õ� ��ǥ�� ����� �Ѱ��ش�.
		this->firstX = firstX;
		this->firstY = firstY;
		this->endX = endX;
		this->endY = endY;
#endif

		//4.1. �ڵ����� �� firstXY, endXY�� ��ǥ�� ���Ѵ�.
		this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
		this->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);
		this->notePadForm->lineChange->FindCaretBeforeLineChange(currentX, currentY, &currentX, &currentY);

		//4.2. �ڵ������ ������ ��ģ��.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

		//4.3. ���� row�� ���Ѵ�.
		row = this->notePadForm->note->GetChild(currentY);
		this->isLineChangeButtonClicked = true;
		this->windowWidth = this->notePadForm->scroll->windowWidth;
	}

	//(21.11.18.�߰�) 4.�� ��� ���� ������ �ϴ��� �̸� ������ش�.
	lineChangeCount = endY - firstY;


	//5. ���õ� ������ ���� ���,
	if (firstX <= 0 && firstY <= 0 && endX <= 0 && endY <= 0) {

		//������� �ʱ�ȭ���ش�.
		this->firstX = 0;
		this->firstY = 0;
		this->endX = 0;
		this->endY = 0;

		//5.1. currentY�� 0���� ũ��, currentX�� 0�� ���,
		if (currentY > 0 && currentX <= 0) {
			length = row->GetLength();
			rowUp = this->notePadForm->note->GetChild(currentY - 1);
			lengthUp = rowUp->GetLength();

			//5.1.1. �ش� row�� length�� 0���� ū ���,
			if (length > 0) {
				//5.1.1.1. length��ŭ �ݺ��Ѵ�.
				while (i < length) {
					//1. currentY�� row�� ���ڸ� �д´�.
					character = row->GetChild(i);


					//���๮�ڸ� character ����� �ش�.****************
					this->character = '\n';
					//*************************************************


					//2. currentY-1�� row�� ���ڸ� add�Ѵ�.
					rowUp->Add(character);
					i++;
				}
				//5.1.1.2. ĳ���� Y�� currentY-1,X�� lengthUp���� �ű��.
				changedY = currentY - 1;
				changedX = lengthUp;

				//5.1.1.3. currentY�� row�� �����Ѵ�.
				this->notePadForm->note->Remove(currentY);
			}


			//5.1.2. �ش� row�� length�� 0�� ���ų� ���� ���,
			else {
				//5.2.0. ���ڸ� ����� �� character ����� �Ű��ش�.****************
				this->character = '\n';
				//******************************************************************

				//5.1.2.1. �ش� row�� �����Ѵ�.
				this->notePadForm->note->Remove(currentY);
				//5.1.2.2. ĳ���� Y�� currentY-1, X�� lengthUp���� �̵��Ѵ�.
				changedY = currentY - 1;
				changedX = lengthUp;
			}
		}
		//5.2. currentX�� 0���� ū ���,
		else if (currentX > 0) {
			//5.2.0. ���ڸ� ����� �� character ����� �Ű��ش�.****************
			character = row->GetChild(currentX - 1);
			strCharacter = character->GetContent();
			this->character = strCharacter;
			//******************************************************************


			//5.2.1. currentX-1��° ���ڸ� �����.
			row->Remove(currentX - 1);

			//5.2.2. currentX-1��°�� ĳ���� �̵��Ѵ�.
			changedY = currentY;
			changedX = currentX - 1;
		}
	}

	//6. ���õ� ������ ���� ���,
	else {
#if 0
		//(21.11.02 ����) �׳� �ڵ����� �Ǿ������� �� ��ǥ�� �Ѱܺ���� ��...
		//�ش� ���õ� ��ǥ�� ����� �Ѱ��ش�.
		this->firstX = firstX;
		this->firstY = firstY;
		this->endX = endX;
		this->endY = endY;
#endif
		//4.3. ���� row�� ���Ѵ�.
		row = this->notePadForm->note->GetChild(firstY);

		//���� ���θ� TRUE�� �ٲ۴�.
		this->notePadForm->isThereSelected = true;

		//character ����� �ʱ�ȭ���ش�.
		this->character.clear();

		//1.3. firstY, endY�� ���� ���,
		if (firstY == endY) {
			//1.3.1. endX�� firstX�� ������ ���Ѵ�.
			difference = endX - firstX;

			//1.3.2. ������ 0���� ū ���� firstX��° ���ڸ� �����ش�.
			while (difference > 0) {
				//1.3.2.1. ���ڸ� ����� �� character ����� �Ű��ش�.****************
				character = row->GetChild(firstX);
				strCharacter = character->GetContent();
				this->character.append(strCharacter);
				//******************************************************************


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

				//ó�� ���� �� ���ĺ��� ���๮�� �־��ش� **********************
				if (i > firstY && isLineChanged == FALSE) {
					this->character.append("\n");
				}
				//****************************************************************

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
					// ���ڸ� ����� �� character ����� �Ű��ش�.****************
					character = row->GetChild(j);
					strCharacter = character->GetContent();
					this->character.append(strCharacter);
					//************************************************************

					row->Remove(j);
					difference--;

				}

				//1.4.2.4. row�� length�� 0���� �۰ų� ���� ���,�ش� ���� �����.
				if (row->GetLength() <= 0 && lineChangeCount>0 ) {

					this->notePadForm->note->Remove(i);

					if (i < endY) {
	                    // ���๮�ڸ� character ����� �Ű��ش�.****************
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

			    // ���๮�ڸ� character ����� �Ű��ش�.****************
				//this->character.append("\n");
				//******************************************************
				this->notePadForm->note->Remove(firstY + 1);
			}
		}

		//1.5. ĳ���� �ű� ��ǥ�� ���Ѵ�.
		changedY = firstY;
		changedX = firstX;

	}

	//7. �ڵ������� ������ ���,
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		//7.1. ��ģ ������ �ٽ� �ڵ������Ѵ�. (ON)
		this->notePadForm->lineChange->LineChangeButtonClicked();

		//7.2. �ڵ����� �� �̵��� ĳ���� ��ġ�� ���Ѵ�.
		this->notePadForm->lineChange->FindCaretAfterLineChange(changedX, changedY, &changedX, &changedY);
	}

	//8. ĳ���� ��ġ�� �ٲ��ش�.
	this->notePadForm->note->MoveCurrent(changedY);
	this->notePadForm->row = this->notePadForm->note->GetChild(changedY);
	this->notePadForm->row->MoveCurrent(changedX);
	
	//����� �ش� ĳ���� ��ġ�� �־��ش�. **************************
	this->currentX = changedX;
	this->currentY = changedY;
	//**************************************************************

	//1.6. ĳ���� �̵��Ѵ�.
	this->notePadForm->isOnImeCharForMove = FALSE;
	this->notePadForm->isFirstComposingForMove = FALSE;


	//2. firstXY, endXY�� �ʱ�ȭ�Ѵ�. ****************************
	this->notePadForm->selectWords->firstX = 0;
	this->notePadForm->selectWords->firstY = 0;
	this->notePadForm->selectWords->endX = 0;
	this->notePadForm->selectWords->endY = 0;
	//************************************************************

	// Scroll ������Ʈ �� ���� ������ �����Ѵ�.
	this->notePadForm->isJumpOverForPrevious = TRUE;
	this->notePadForm->isJumpOverForNext = FALSE;
	this->notePadForm->isDoubleByte = FALSE;
	this->notePadForm->isUp = FALSE;
	this->notePadForm->isDown = FALSE;

	//(21.10.01 �߰�)
	//this->isLineChangeButtonClicked = this->notePadForm->isLineChangeButtonClicked;

	//(21.11.10.�߰�)
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
	int windowWidth = this->notePadForm->scroll->windowWidth; //���� ������ʺ�

	this->notePadForm->selectWords->firstX = 0;
	this->notePadForm->selectWords->firstY = 0;
	this->notePadForm->selectWords->endX = 0;
	this->notePadForm->selectWords->endY = 0;


	//1. (21.09.30 �߰�) ���� �ڵ������ư ��Ȳ�� �������� ��Ȳ�� �ٸ���,
	if (this->notePadForm->isLineChangeButtonClicked != this->isLineChangeButtonClicked) {

		//1.1. ���� �ڵ������ư�� ������ ������, �ڵ����� ���� ��ǥ�� ���Ѵ�. 
		if (this->notePadForm->isLineChangeButtonClicked == true) {
			this->notePadForm->lineChange->FindCaretAfterLineChange(currentX, currentY, &currentX, &currentY);
			//this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
			//this->notePadForm->lineChange->FindCaretAfterLineChange(endX, endY, &endX, &endY);
		}
		//1.2. ���� �ڵ������ư�� �ȴ����� ������, �ڵ����� ���� ��ǥ�� ���Ѵ�.
		else {
			//1.2.1. �ڵ����� �����ش�.
			this->notePadForm->lineChange->LineChangeButtonClicked(this->windowWidth); //-25

			//1.2.2. �ڵ����� �� ��ǥ ���Ѵ�.
			this->notePadForm->lineChange->FindCaretBeforeLineChange(currentX, currentY, &currentX, &currentY);
			//this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
			//this->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);
			
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
		this->notePadForm->lineChange->FindCaretBeforeLineChange(currentX, currentY, &currentX, &currentY);
		//this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
		//this->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);

		//2.4. �ڵ������ ������ ��ģ��.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

		//2.5. ��ģ ������ ���� windowWidth��ŭ �ڵ������Ѵ�.
		this->notePadForm->lineChange->LineChangeButtonClicked(windowWidth);

		//2.6. �ڵ����� �� ��ǥ�� ���Ѵ�.
		this->notePadForm->lineChange->FindCaretAfterLineChange(currentX, currentY, &currentX, &currentY);
		//this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
		//this->notePadForm->lineChange->FindCaretAfterLineChange(endX, endY, &endX, &endY);
	}
#if 0
	else if(this->isLineChangeButtonClicked == true) {

		//�ڵ����� �� ��ǥ�� ���Ѵ�.
		this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
		this->notePadForm->lineChange->FindCaretAfterLineChange(endX, endY, &endX, &endY);
	}
#endif

	//0. ���� ĳ���� ��ġ�� �������ش�.
	this->notePadForm->note->MoveCurrent(currentY);
	Glyph* row = this->notePadForm->note->GetChild(currentY);
	row->MoveCurrent(currentX);
	this->notePadForm->isUndo = true;

	//1. ���õ� �ܾ ������, (�ѱ����̸�)
	if (this->notePadForm->isThereSelected == false) {

		//1.1. ���� character�� �ִ� ���ڰ� 1Byte�̸�, OnChar�� ȣ���Ѵ�.
		if (singleByteCharacter[0] >= 0 && singleByteCharacter[0] <= 127) {
			this->notePadForm->nChar = singleByteCharacter[0];
			id = 40001;
		}

		//1.2. ���� character�� �ִ� ���ڰ� 2Byte�̸�, OnImeComposition ȣ���Ѵ�.
		else {
			this->notePadForm->letter = this->character;
			id = 40005;
		}

		//1.3.  ĳ���� ũ��� Undo�� 3mm�� �����Ѵ�.
		this->notePadForm->isFirstComposing = false;

		//1.4. OnImeCharCommand �Ǵ� OnChar�� ȣ���Ѵ�.
		this->notePadForm->SendMessage(WM_COMMAND, id);
	}

	//2. ���õ� �ܾ ������,
	else {
		//2.1. character�� length��ŭ �ݺ��Ѵ�.
		while (i < this->character.size()) {

			//2.1.0.  ĳ���� ũ��� Undo�� 3mm�� �����Ѵ�.
			this->notePadForm->isFirstComposing = false;

			//2.1.1. 1Byte���� 2Byte���� Ȯ���Ѵ�.
			singleByteCharacter[0] = this->character.at(i);

			//2.1.2. ���� character�� �ִ� ���ڰ� 1Byte�̸�, OnChar�� ȣ���Ѵ�.
			if (singleByteCharacter[0] >= 0 && singleByteCharacter[0] <= 127) {
				
				singleByteCharacter[1] = '\0';

				//2.1.2.1. ���๮���̸� ��ǥ�� currentY++, currentX=0 ���� ��ȯ���ش�.
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
			//2.1.3. ���� character�� �ִ� ���ڰ� 2Byte�̸�, OnImeComposition ȣ���Ѵ�.
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
		//2.1. firstXY, endXY�� �������ش�.
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
	//3. Undo �� �ٲ� ĳ���� ��ġ�� �����Ѵ�.
	this->currentX = this->notePadForm->currentX;
	this->currentY = this->notePadForm->currentY;

#endif


	//1. (21.09.30 �߰�) ���� �ڵ������ư ��Ȳ�� �������� ��Ȳ�� �ٸ���,
	if (this->notePadForm->isLineChangeButtonClicked != this->isLineChangeButtonClicked) {

		//1.1. ���� �ڵ������ư�� ������ ������, �ڵ����� ���� ��ǥ�� ���Ѵ�. 
		if (this->notePadForm->isLineChangeButtonClicked == true) {
			//this->notePadForm->lineChange->FindCaretAfterLineChange(currentX, currentY, &currentX, &currentY);
			this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
			this->notePadForm->lineChange->FindCaretAfterLineChange(endX, endY, &endX, &endY);
		}
		//1.2. ���� �ڵ������ư�� �ȴ����� ������, �ڵ����� ���� ��ǥ�� ���Ѵ�.
		else {
			//1.2.1. �ڵ����� �����ش�.
			this->notePadForm->lineChange->LineChangeButtonClicked(this->windowWidth); //-25
			//1.2.2. �ڵ����� �� ��ǥ ���Ѵ�.
			//this->notePadForm->lineChange->FindCaretBeforeLineChange(currentX, currentY, &currentX, &currentY);
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
		//this->notePadForm->lineChange->FindCaretBeforeLineChange(currentX, currentY, &currentX, &currentY);
		this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
		this->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);

		//2.4. �ڵ������ ������ ��ģ��.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

		//2.5. ��ģ ������ ���� windowWidth��ŭ �ڵ������Ѵ�.
		this->notePadForm->lineChange->LineChangeButtonClicked(windowWidth);

		//2.6. �ڵ����� �� ��ǥ�� ���Ѵ�.
		//this->notePadForm->lineChange->FindCaretAfterLineChange(currentX, currentY, &currentX, &currentY);
		this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
		this->notePadForm->lineChange->FindCaretAfterLineChange(endX, endY, &endX, &endY);
	}

#if 0
	if (this->isLineChangeButtonClicked == true) {

		//�ڵ����� �� ��ǥ�� ���Ѵ�.
		this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
		this->notePadForm->lineChange->FindCaretAfterLineChange(endX, endY, &endX, &endY);
	}
#endif


	if (this->notePadForm->isThereSelected == true) {

		//2.1. firstXY, endXY�� �������ش�.
		this->notePadForm->selectWords->firstX = firstX;
		this->notePadForm->selectWords->firstY = firstY;
		this->notePadForm->selectWords->endX = endX;
		this->notePadForm->selectWords->endY = endY;


		this->firstX = firstX;
		this->firstY = firstY;
		this->endX = endX;
		this->endY = endY;
	}


	//3. Undo �� �ٲ� ĳ���� ��ġ�� �����Ѵ�.
	this->currentX = this->notePadForm->currentX;
	this->currentY = this->notePadForm->currentY;
	
	this->notePadForm->isUndo = false;
	//this->notePadForm->isThereSelected == false;

	//(21.11.02. �߰�) ���� �ڵ������ư ��Ȳ�� �����Ų��.
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
	int windowWidth = this->notePadForm->scroll->windowWidth; //���� ������ʺ�
#if 0
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
#endif

#if 0
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
#endif

	//1. (21.11.02. �߰�) ���� �ڵ������ư ��Ȳ�� �������� ��Ȳ�� �ٸ���,
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
			this->notePadForm->lineChange->LineChangeButtonClicked(this->windowWidth); //-25
			
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
		this->notePadForm->lineChange->LineChangeButtonClicked(this->windowWidth); //-25

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

	//(21.10.29. �߰�) 1. ���õ� �ܾ ������ endX, endY�� ��ġ�� �������ش�.
	if (firstX > 0 || firstY > 0 || endX > 0 || endY > 0) {

		//1.1. ���� ĳ���� ��ġ�� �������ش�.
		this->notePadForm->note->MoveCurrent(endY); 
		Glyph* row = this->notePadForm->note->GetChild(endY);
		row->MoveCurrent(endX);
	}
	//(21.10.29. �߰�) 2. ���õ� �ܾ ������ currentX, currentY�� ��ġ�� �������ش�.
	else {

		//2.1. ���� ĳ���� ��ġ�� �������ش�.
		this->notePadForm->note->MoveCurrent(currentY); 
		Glyph* row = this->notePadForm->note->GetChild(currentY);
		row->MoveCurrent(currentX);
	}



	//2. Execute �ϱ� ���� ��ǥ�� �ٽ� �����ش�.
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


	//ĳ���� ũ��� Undo�� 3mm�� �����Ѵ�.
	this->notePadForm->isFirstComposingForMove = false;
	this->notePadForm->isOnImeCharForMove = false;

	//(21.10.01 �߰�)
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

