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

	//1. DeleteŰ ������ ��

	//(21.11.10.�߰�)
	if (this->notePadForm->isOpen == TRUE) {
		this->notePadForm->isOpenChanged = TRUE;
	}

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

	//1.4. ���� ���õ� �ܾ ������ ��� ��ǥ�� �Ű��ش�.
	if (this->notePadForm->isThereSelected == true) {

		firstY = this->firstY;
		firstX = this->firstX;
		endY = this->endY;
		endX = this->endX;

#if 0
		//4.1. �ڵ������� ������ �ִ� ���, �ڵ����� �� firstXY, endXY�� ��ǥ�� ���Ѵ�.
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

	changedX = currentX;
	changedY = currentY;


	//3.(21.11.11.�߰�) ���õ� �ܾ ���� ���, �ڵ������� �Ǿ��� �ȵǾ��� ���� ��ǥ�� �Ѱ��ش�.
	if (firstX > 0 || firstY > 0 || endX > 0 || endY > 0) {
		this->firstX = firstX;
		this->firstY = firstY;
		this->endX = endX;
		this->endY = endY;

		changedX = firstX;
		changedY = firstY;
	}

	  //4. �ڵ������ư�� ������ ���,
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		//4.1. �ڵ����� �� firstXY, endXY�� ��ǥ�� ���Ѵ�.
		this->notePadForm->lineChange->FindCaretBeforeLineChange(currentX, currentY, &currentX, &currentY);
		this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
		this->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);

		//4.2. �ڵ������ ������ ��ģ��.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

		//4.3. ���� row�� ���Ѵ�.
		row = this->notePadForm->note->GetChild(currentY);

		//4.4. length�� rowLength�� �ڵ����� �� �������� ���Ѵ�.
		length = this->notePadForm->note->GetLength();
		rowLength = row->GetLength();

		this->isLineChangeButtonClicked = true;
		this->windowWidth = this->notePadForm->scroll->windowWidth;
	}

	//changedX = currentX;
	//changedY = currentY;


	//5. ���õ� ������ ���� ���,
	if (firstX <= 0 && firstY <= 0 && endX <= 0 && endY <= 0) {
		
		this->notePadForm->isThereSelected = false;

		//������� �ʱ�ȭ���ش�.
		this->firstX = 0;
		this->firstY = 0;
		this->endX = 0;
		this->endY = 0;

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
			this->character = '\n';
			this->notePadForm->note->Remove(currentY + 1);
		}
		//1.4. currentX�� row�� length���� ���� ���, currentX��°�� ���ڸ� �����.
		else if (currentX < rowLength) {
			character = row->GetChild(currentX);
			content = character->GetContent();
			this->character = content;

			row->Remove(currentX);
		}

		changedY = currentY;
		changedX = currentX;
	}

	//6. ���õ� ������ ���� ���,
	else {
#if 0
		//�ش� ���õ� ��ǥ�� ����� �Ѱ��ش�.
		this->firstX = firstX;
		this->firstY = firstY;
		this->endX = endX;
		this->endY = endY;
#endif

		this->notePadForm->isThereSelected = true;

		//1.3. firstY, endY�� ���� ���,
		if (firstY == endY) {
			//1.3.1. endX�� firstX�� ������ ���Ѵ�.
			difference = endX - firstX;

			//1.3.2. ������ 0���� ū ���� firstX��° ���ڸ� �����ش�.
			while (difference > 0) {
				//1.3.2.1. ���ڸ� ����� �� character ����� �Ű��ش�.****************
				character = row->GetChild(firstX);
				content = character->GetContent();
				this->character.append(content);
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
				if (i > firstY) {
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
					content = character->GetContent();
					this->character.append(content);
					//************************************************************
					row->Remove(j);
					difference--;
				}

				//1.4.2.4. row�� length�� 0���� �۰ų� ���� ���,�ش� ���� �����.
				if (row->GetLength() <= 0) {

					//�ƿ� ������ ���̸� �����ϸ� �ȵȴ�. �� �̳��� �ٸ� ***************************
					if (i < this->notePadForm->note->GetLength() - 1) {
						// ���๮�ڸ� character ����� �Ű��ش�.****************
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
#if 0
		this->notePadForm->note->MoveCurrent(firstY);
		row = this->notePadForm->note->GetChild(firstY);
		row->MoveCurrent(firstX);
#endif
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


	//Scroll ������Ʈ �� ���� ������ �����Ѵ�.
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

	//0. ���� ĳ���� ��ġ�� �������ش�.
	Long currentX = this->currentX;
	Long currentY = this->currentY;

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

		//1.5. ĳ���� ��ġ�� �������ش�.
		currentY = this->notePadForm->note->GetCurrent();
		row = this->notePadForm->note->GetChild(currentY);
		currentX = row->GetCurrent();
		currentX--;
		row->MoveCurrent(currentX);

		//1.6. Notify�Ѵ�.
		// Scroll ������Ʈ �� ���� ������ �����Ѵ�.
		this->notePadForm->isJumpOverForPrevious = FALSE;
		this->notePadForm->isJumpOverForNext = FALSE;
		this->notePadForm->isDoubleByte = FALSE;
		this->notePadForm->isUp = FALSE;
		this->notePadForm->isDown = FALSE;

		this->notePadForm->Notify();

		this->notePadForm->Invalidate();

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


		//2.1. firstXY, endXY�� �������ش�.
		this->notePadForm->selectWords->firstX = this->firstX;
		this->notePadForm->selectWords->firstY = this->firstY;
		this->notePadForm->selectWords->endX = this->endX;
		this->notePadForm->selectWords->endY = this->endY;

	}

	this->notePadForm->isUndo = false;
	//this->notePadForm->isThereSelected == false;

}

void OnDeleteCommand::Redo() {

	//1. ���� ĳ���� ��ġ�� �������ش�.
	Long currentX = this->currentX;
	Long currentY = this->currentY;

	this->notePadForm->note->MoveCurrent(currentY);
	Glyph* row = this->notePadForm->note->GetChild(currentY);
	row->MoveCurrent(currentX);

#if 0
	if (this->notePadForm->isThereSelected == true) {


	}
#endif

	//ĳ���� ũ��� Undo�� 3mm�� �����Ѵ�.
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




