//FindAndReplace.cpp
#include"FindAndReplace.h"
#include"FindSameText.h"
#include"NotePadForm.h"
#include"Note.h"
#include"CreateGlyph.h"
#include"CreateProduct.h"
#include"KeyAction.h"
#include"FactoryProduct.h"
#include"SelectWords.h"
#include"LineChange.h"

FindAndReplace::FindAndReplace(FindSameText* findSameText) {
	this->findSameText = findSameText;
	this->isReplaced = false;
}

FindAndReplace::~FindAndReplace() {
}

void FindAndReplace::ReplaceSameText(string findStr, string replaceStr, string selectedStr, bool matchCase,
	bool searchAround, int firstCurrentY, int beforeLineChangeCurrentX,
	int* firstY, int* firstX, int* endY, int* endX, int* exception, int* isReplaced) {

	char* chaCharacter = 0;
	char afterCharacter[2];
	int currentX;
	int currentY = 0;
	int rowLength;
	int restLength;
	Glyph* row = NULL;
	Glyph* character;
	bool isSameText = false;
	bool isThereSameText = false;
	string strCharacter;
	string temp;
	int i = 1;
	int j;
	int k;
	int noteLength;
	int tempCurrentY;
	int selectedFirstX = 0;
	CreateGlyph* creator = 0;
	KeyAction* keyAction = NULL;
	const char* cFindStr = findStr.c_str();
	const char* cSelectedStr = selectedStr.c_str();
	const char* cReplaceStr = replaceStr.c_str();
	const char* cTemp;
	//(21.09.18 �߰�)
	FactoryProduct factoryProduct(this->findSameText->notePadForm);
	bool isThereLineChange = false;

	//(21.09.18 �߰�) findStr�� ���๮�ڰ� ���ԵǾ� �ִ��� Ȯ���Ѵ�.
	size_t nPos = findStr.find("\n");
	if (nPos != string::npos) { //���๮�ڰ� �ִ� ���,
		isThereLineChange = true;
	}


	//�����ʱ�ȭ
	(*exception) = 0;
	(*isReplaced) = 0;

	//0. ã�� ����, �ٲ� ������ �� ������ ���Ѵ�.
	const unsigned char* constStr = reinterpret_cast<const unsigned char*> (findStr.c_str());
	int findLength = _mbslen(constStr);

	//constStr = reinterpret_cast<const unsigned char*> (const_cast<const char*>(replaceStr.c_str()));
	constStr = reinterpret_cast<const unsigned char*> (replaceStr.c_str());
	int replaceLength = _mbslen(constStr);


	//1. ã�� ����, �ٲ� ����, ���� ���õ� ����, ��ҹ��� ����, �ڵ����� �� ĳ���� ��ǥ�� �Է¹޴´�.
	//2. ���� ���õ� �ܾ ã���� �ϴ� �ܾ�� ������ Ȯ���Ѵ�.
	  //2.1. ��ҹ��� �����ϸ�,
	if (matchCase == true && strcmp(cFindStr, cSelectedStr) == 0) {
		isSameText = true;
		this->isReplaced = true;
		(*isReplaced) = 1;

	}

	//2.2. ��ҹ��� ���� ���ϸ�,
	else if (matchCase == false && stricmp(cFindStr, cSelectedStr) == 0) {
		isSameText = true;
		this->isReplaced = true;
		(*isReplaced) = 1;
	}

	//2.3. ���õ� �ܾ�� ã���� �ϴ� �ܾ ������, (�ٲٷ��� �ܾ �ƴ�)
	if (isSameText == true) {
		//2.3.0. string���� char������ ��ȯ�Ѵ�.
		//chaCharacter = new char(replaceStr.length());
		chaCharacter = new char[replaceStr.length() + 1];
		strcpy(chaCharacter, cReplaceStr);

#if 0
		keyAction = factoryProduct.Make(VK_BACK, NULL, NULL);
		keyAction->KeyDown();
#endif
		RemoveSelectedWord();

		//(21.09.18 �߰�) 2.3.1. ����� �� �� ĳ���� ��ġ�� Ȯ���Ѵ�.
		currentY = this->findSameText->notePadForm->note->GetCurrent();
		row = this->findSameText->notePadForm->note->GetChild(currentY);
		currentX = row->GetCurrent();

#if 0
		//2.3.1.1. �ڵ������� ������ �ִ� ���, �ڵ������� ��ġ�� �����ش�.
		if (this->findSameText->notePadForm->isLineChangeButtonClicked == true) {
			this->findSameText->notePadForm->lineChange->FindCaretBeforeLineChange(currentX, currentY, &currentX, &currentY);
			row = this->findSameText->notePadForm->note->GetChild(currentY);
		}
#endif

		firstCurrentY = currentY;

#if 0
		//2.3.1. ���õ� �ܾ �����.
		selectedFirstX = beforeLineChangeCurrentX - findLength;
		row = this->findSameText->notePadForm->note->GetChild(firstCurrentY);
		//row = this->glyphs.GetAt(firstCurrentY);


		while (i <= findLength) {
			row->Remove(selectedFirstX);
			i++;
		}
#endif


		//2.3.2. �ٲ� ������ ���´�.
		creator = new CreateProduct();
		i = 0;
		j = currentX;  //selectedFirstX
		while (i < replaceStr.length()) {
			afterCharacter[0] = chaCharacter[i];
			afterCharacter[1] = chaCharacter[i + 1];

			//1. 1byte�� ���,
			if (afterCharacter[0] >= 0 && afterCharacter[0] < 128) {
				afterCharacter[1] = '\0';
				character = creator->Make(afterCharacter);
				i++;
			}
			//2. 2byte�� ���,
			else {
				character = creator->Make(afterCharacter);
				i = i + 2;
			}
			//3. ������ ���´�.
			row->Add(j, character);
			j++;
		}

		//2.3.3. ĳ���� ��ǥ�� ���� ���� ������ �� �ڷ� ���Ѵ�.
		beforeLineChangeCurrentX = currentX + replaceLength;

		//(21.09.19 �߰�) 
		this->findSameText->notePadForm->selectWords->afterEndX = beforeLineChangeCurrentX;
		this->findSameText->notePadForm->selectWords->afterEndY = currentY;


		//2.3.1.1. �ڵ������� ������ �ִ� ���, �ڵ������� ��ġ�� �����ش�.
		if (this->findSameText->notePadForm->isLineChangeButtonClicked == true) {

			//1.5.1. ��ģ ������ �ٽ� �ڵ������Ѵ�. (ON)
			this->findSameText->notePadForm->lineChange->LineChangeButtonClicked();

			//1.5.2. �ڵ����� �� ��ġ�� ���Ѵ�.
			this->findSameText->notePadForm->lineChange
				->FindCaretAfterLineChange(this->findSameText->notePadForm->selectWords->afterEndX, 
					this->findSameText->notePadForm->selectWords->afterEndY, 
					&this->findSameText->notePadForm->selectWords->afterEndX, 
					&this->findSameText->notePadForm->selectWords->afterEndY);

			//1.5.3. �ٽ� ������ �����ش�. (OFF)
			this->findSameText->notePadForm->lineChange->LineChangeButtonNotClicked();
		}
	}
	//FindSameTextWithDown�� ���� ----------------------------------------------------------------------------------
	//3. ������ ���� �ƴѵ��� �ݺ��Ѵ�.

	//(21.08.30 �߰�) ������ ��ġ�� �������ִ� ���,
	if (searchAround == true) {
		currentY = 0;
	}
//#if 0
	else {
		currentY = firstCurrentY;
		//currentY = this->findSameText->notePadForm->note->GetCurrent();
	}
//#endif
	noteLength = this->findSameText->notePadForm->note->GetLength();
	//noteLength = this->length;

	//(21.09.18 �߰� ����) i�� �ʱ�ȭ�Ѵ�.
	if (searchAround == true) {
		i = 0;
	}
	else {
		i = beforeLineChangeCurrentX;
	}

	//3. ������ ���� �ƴѵ��� �ݺ��Ѵ�.
	while (isThereLineChange == false && currentY < noteLength && isThereSameText == false) {

		//3.1. ���� ĳ���� �ִ� ���� ã�´�.
		row = this->findSameText->notePadForm->note->GetChild(currentY);
		//row = this->glyphs.GetAt(currentY);
		rowLength = row->GetLength();

		//3.2. i�� �ʱ�ȭ�ϰ� restLength�� ���Ѵ�.
		if (currentY == firstCurrentY) {
			restLength = rowLength - i;
		}
		else {
			i = 0;
			restLength = rowLength;
		}

		//3.3. restLength�� findLength���� ũ�ų� ����, ���� �ܾ ���� ���� �ݺ��Ѵ�.
		while (restLength >= findLength && isThereSameText == false) {

			//3.3.1. �ش� �ٿ��� ã�� ������ �� ������ŭ ���ڸ� �����.
			temp.clear(); //���ڿ� �����
			j = i;
			k = 0;
			while (k < findLength) {//temp.length()
				character = row->GetChild(j);
				strCharacter = character->GetContent();
				temp.append(strCharacter);
				j++;
				k++;
			}
			cTemp = temp.c_str();

			//3.3.2. ��ҹ��� �����ϸ�,
			if (matchCase == true) {
				if (strcmp(cTemp, cFindStr) == 0) {
					isThereSameText = true;
					(*firstY) = currentY;
					(*firstX) = i;
					(*endY) = currentY;
					(*endX) = j;
				}
				else {
					isThereSameText = false;
				}
			}
			//3.3.3. ��ҹ��� �������� ������,
			else if (matchCase == false) {
				if (stricmp(cTemp, cFindStr) == 0) {
					isThereSameText = true;
					(*firstY) = currentY;
					(*firstX) = i;
					(*endY) = currentY;
					(*endX) = j;
				}
				else {
					isThereSameText = false;
				}
			}
			restLength--;
			i++;
		}
		currentY++;
	}

	//4. ���� ���ڰ� ���ԵǾ� �ִ� ���, ������ ���� �ƴѵ��� �ݺ��Ѵ�.
	while (isThereLineChange == true && currentY < noteLength && isThereSameText == false) {

		//4.1. temp �ʱ�ȭ�Ѵ�.
		temp.clear();
		tempCurrentY = currentY;
		j = i;
		k = 0;

		//4.2. findLength��ŭ ���� ���ڸ� �����.
		while (k < findLength && tempCurrentY < noteLength && isThereSameText == false) {

			row = this->findSameText->notePadForm->note->GetChild(tempCurrentY);

			while (k < findLength && j < row->GetLength()) {

				character = row->GetChild(j);
				strCharacter = character->GetContent();
				temp.append(strCharacter);
				j++;
				k++;
			}
			if (k < findLength) {
				tempCurrentY++;
				temp.append("\n");
				j = 0;
				k++;
			}
		}

		cTemp = temp.c_str();

		//4.3. ��ҹ��� �����ϸ�,
		if (matchCase == true) {
			if (strcmp(cTemp, cFindStr) == 0) {
				isThereSameText = true;
				(*firstY) = currentY;
				(*firstX) = i;
				(*endY) = tempCurrentY;
				(*endX) = j;
			}
			else {
				isThereSameText = false;
			}
		}

		//4.4. ��ҹ��� �������� ������,
		else if (matchCase == false) {
			if (stricmp(cTemp, cFindStr) == 0) {
				isThereSameText = true;
				(*firstY) = currentY;
				(*firstX) = i;
				(*endY) = tempCurrentY;
				(*endX) = j;
			}
			else {
				isThereSameText = false;
			}
		}

		i++;
		row = this->findSameText->notePadForm->note->GetChild(currentY);

		if (row->GetLength() <= i) {
			currentY++;
			i = 0;
		}
	}

	//5. �� ���� ���� �ܾ�� ������, ó�� ������ �� replace�� ���,
	if (isThereSameText == false) {

		//#if 0
		if (isSameText == true) {
			(*firstY) = firstCurrentY; //firstCurrentY
			(*firstX) = currentX;  //selectedFirstX
			(*endY) = firstCurrentY; //firstCurrentY
			(*endX) = beforeLineChangeCurrentX;
			(*exception) = 1;
		}
		//#endif

		//4.1. �� ���� ���� �ܾ ����, ó�� ������ �� replace���� ���� ���, firstXY, endXY�� 0�� �Է��Ѵ�.
		else if (isSameText == false && searchAround == false) {
			(*firstY) = 0;
			(*firstX) = 0;
			(*endY) = 0;
			(*endX) = 0;
		}
	}
	//------------------------------------------------------------------------------------------------------

	//�Ҵ����� ���ش�.
	if (creator != 0) {
		delete creator;
		creator = 0;
	}
	if (chaCharacter != 0) {
		delete[] chaCharacter;
		chaCharacter = 0;
	}
	this->findSameText->notePadForm->Invalidate();
}


void FindAndReplace::RemoveSelectedWord() {

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
	Long changedX = 0;
	Long changedY = 0;
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
	int currentY;
	int currentX;
	int firstY = this->findSameText->notePadForm->selectWords->GetFirstY();
	int firstX = this->findSameText->notePadForm->selectWords->GetFirstX();
	int endY = this->findSameText->notePadForm->selectWords->GetEndY();
	int endX = this->findSameText->notePadForm->selectWords->GetEndX();


	//0. �ڵ������ư�� ������ ���,
	if (this->findSameText->notePadForm->isLineChangeButtonClicked == TRUE) {

		//0.0  ��ģ ������ �ٽ� �ڵ������Ѵ�. (ON)
		this->findSameText->notePadForm->lineChange->LineChangeButtonClicked();

		//0.1. ���� currentXY, firstXY, endXY�� ���Ѵ�.
		currentY = this->findSameText->notePadForm->note->GetCurrent();
		row = this->findSameText->notePadForm->note->GetChild(currentY);
		currentX = row->GetCurrent();

		firstY = this->findSameText->notePadForm->selectWords->GetFirstY();
		firstX = this->findSameText->notePadForm->selectWords->GetFirstX();
		endY = this->findSameText->notePadForm->selectWords->GetEndY();
		endX = this->findSameText->notePadForm->selectWords->GetEndX();

		//0.2. �ڵ����� �� firstXY, endXY�� ��ǥ�� ���Ѵ�.
		this->findSameText->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
		this->findSameText->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);
		this->findSameText->notePadForm->lineChange->FindCaretBeforeLineChange(currentX, currentY, &currentX, &currentY);

		//0.3. �ڵ������ ������ ��ģ��.
		this->findSameText->notePadForm->lineChange->LineChangeButtonNotClicked();

		//0.4. ���� row�� ���Ѵ�.
		row = this->findSameText->notePadForm->note->GetChild(firstY); //currentY
	}

	//1. �ڵ������ư �� ���� ���,
	else {
		//1. ���� ĳ���� ��ǥ�� ���Ѵ�.
		currentY = this->findSameText->notePadForm->note->GetCurrent();

		//2. ���� row�� ���Ѵ�.
		row = this->findSameText->notePadForm->note->GetChild(firstY); //currentY
		currentX = row->GetCurrent();
	}


	//(21.08.19 �߰�) �����ϴ� ���� ******************************************************
	//3. firstX,Y, endX,Y�� ���Ѵ�.
	int startX = firstX;
	int startY = firstY;
	int lastX = endX;
	int lastY = endY;

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

	//6. ���õ� ������ �ִ� ��츸 ����Ѵ�.
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
		endRow = this->findSameText->notePadForm->note->GetChild(endY);
		if (firstX > 0 && endX < endRow->GetLength()) {
			isAppend = TRUE;
		}

		//1.4.2. firstY�� endY���� �۰ų� ���� ���� �ݺ��Ѵ�.
		i = firstY;
		while (i <= endY) {
			row = this->findSameText->notePadForm->note->GetChild(i);

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
				if (i < this->findSameText->notePadForm->note->GetLength() - 1) {
					this->findSameText->notePadForm->note->Remove(i);
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
			row = this->findSameText->notePadForm->note->GetChild(firstY);
			endRow = this->findSameText->notePadForm->note->GetChild(firstY + 1);
			i = 0;

			//1.4.3.1. firstY�� row�� �������´�.
			while (i < endRow->GetLength()) {
				character = endRow->GetChild(i);
				row->Add(character);
				i++;
			}
			//1.4.3.2. firstY+1�� row�� �����.
			this->findSameText->notePadForm->note->Remove(firstY + 1);
		}
	}

	//1.5. ĳ���� �ű� ��ǥ�� ���Ѵ�.
	changedY = firstY;
	changedX = firstX;

	this->findSameText->notePadForm->note->MoveCurrent(changedY);
	this->findSameText->notePadForm->row = this->findSameText->notePadForm->note->GetChild(changedY);
	this->findSameText->notePadForm->row->MoveCurrent(changedX);

#if 0
	//7. �ڵ������� ������ ���,
	if (this->findSameText->notePadForm->isLineChangeButtonClicked == TRUE) {

		//7.1. ��ģ ������ �ٽ� �ڵ������Ѵ�. (ON)
		this->findSameText->notePadForm->lineChange->LineChangeButtonClicked();

		//7.2. �ڵ����� �� �̵��� ĳ���� ��ġ�� ���Ѵ�.
		this->findSameText->notePadForm->lineChange->FindCaretAfterLineChange(changedX, changedY);
	}
	else {
		this->findSameText->notePadForm->note->MoveCurrent(changedY);
		this->findSameText->notePadForm->row = this->findSameText->notePadForm->note->GetChild(changedY);
		this->findSameText->notePadForm->row->MoveCurrent(changedX);
	}

	//1.6. ĳ���� �̵��Ѵ�.
	this->findSameText->notePadForm->isOnImeCharForMove = FALSE;
	this->findSameText->notePadForm->isFirstComposingForMove = FALSE;
#endif

	//2. firstXY, endXY�� �ʱ�ȭ�Ѵ�. ****************************
	this->findSameText->notePadForm->selectWords->firstX = 0;
	this->findSameText->notePadForm->selectWords->firstY = 0;
	this->findSameText->notePadForm->selectWords->endX = 0;
	this->findSameText->notePadForm->selectWords->endY = 0;
	//************************************************************



}

void FindAndReplace::AddWordForUndo(int firstX, int firstY, string findStr, int* changedX, int* changedY) {

	Glyph* row = this->findSameText->notePadForm->note->GetChild(firstY);
	int rowLength = row->GetLength();
	int strLength = findStr.size();
	BOOL isLineChanged = FALSE;
	char afterCharacter[2];
	char* character_;
	CreateGlyph* creator = NULL;
	BOOL lastLineChanged = FALSE;
	Glyph* character = NULL;
	BOOL isThereRestOnes = FALSE;
	Glyph* currentRow;
	CString findStr_ = (CString)findStr.c_str();
	int restLength;
	int currentY;
	int currentX;
	int restX;
	int restY;
	int i = 0;
	int j = 0;
	int k;

	creator = new CreateProduct();
	character_ = new char[strLength + 1];
	strcpy(character_, findStr_);

	//3.1. ���������� ���
	if (firstX >= rowLength) {

		k = firstY;
		this->findSameText->notePadForm->row = this->findSameText->notePadForm->note->GetChild(k);

		//3.1.1 str�� length��ŭ �ݺ��Ѵ�.
		while (i < strLength) {//character_[i]!= NULL
			//3.1.1.1. ��Ʈ�� ���� ���� ���Ѵ�.						
			isLineChanged = FALSE;

			afterCharacter[0] = character_[i];
			afterCharacter[1] = character_[i + 1];

			//1.1byte�� ���,
			if (afterCharacter[0] >= 0 && afterCharacter[0] < 128) {

				//1.0. ù��° ��ġ�� �ι��ڸ� �־��ش�.
				afterCharacter[1] = '\0';

				//1.1. ���๮���̸�, 
				if (afterCharacter[0] == '\n') {
					//1.1.1. ���ο� ���ڸ� �����.
					k++;

					//row = new Row();
					this->findSameText->notePadForm->row = creator->Make(afterCharacter);
					this->findSameText->notePadForm->note->Add(k, this->findSameText->notePadForm->row);
					//row = this->notePadForm->note->GetChild(k);


					//1.1.2. ������ �ٲ۴�.
					isLineChanged = TRUE;
					lastLineChanged = TRUE;
					j = 0;
				}

				else {
					//character = new SingleByteCharacter(afterCharacter[0]);
					character = creator->Make(afterCharacter);
				}

				i++;
			}

			//2. 2byte�� ���
			else {
				//character = new DoubleByteCharacter(afterCharacter);
				character = creator->Make(afterCharacter);
				i = i + 2;
			}

			//3. ���๮�ڰ� �ƴϸ� row�� Add�Ѵ�.
			if (isLineChanged == FALSE) {
				//this->notePadForm->note->GetChild(k);
				this->findSameText->notePadForm->row->Add(character); //row->Add(character); ************************************
				//row->MoveCurrent(row->GetLength());
				j++;
			}

		}

		//3.1.2. �ٲ� ��ǥ�� �����Ѵ�.
		*changedY = k;
		//changedX = currentX + j;
		if (lastLineChanged == TRUE) {
			*changedX = j;
		}
		else {
			*changedX = firstX + j;
		}
	}

	//3.2. ���������� ���
	else if (firstX < rowLength) {

		//3.2.1 str�� length��ŭ �ݺ��Ѵ�.
		int l = firstX;
		k = firstY;
		i = 0;

		//3.1.1 str�� length��ŭ �ݺ��Ѵ�.
		while (i < strLength) {//character_[i]!= NULL
			//3.1.1.1. ��Ʈ�� ���� ���� ���Ѵ�.
			//row = this->notePadForm->note->GetChild(k);

			isLineChanged = FALSE;

			afterCharacter[0] = character_[i];
			afterCharacter[1] = character_[i + 1];

			//1.1byte�� ���,
			if (afterCharacter[0] >= 0 && afterCharacter[0] < 128) {

				//1.0. ù��° ��ġ�� �ι��ڸ� �־��ش�.
				afterCharacter[1] = '\0';

				//1.1. ���๮���̸�,
				if (afterCharacter[0] == '\n') {

					//������ ������ ���ڰ� �ִ��� Ȯ���Ѵ�.**************************			
					restX = firstX + j;

					if (this->findSameText->notePadForm->row->GetLength() > restX) {
						isThereRestOnes = TRUE;
						restY = firstY;
					}
					//***************************************************************

					//1.1.1. ���ο� ���ڸ� �����.
					k++;

					//row = new Row();
					this->findSameText->notePadForm->row = creator->Make(afterCharacter);
					this->findSameText->notePadForm->note->Add(k, this->findSameText->notePadForm->row);

					//1.1.3. ������ �ٲ۴�.
					isLineChanged = TRUE;
					lastLineChanged = TRUE;
					j = 0;
					l = 0;
				}

				else {
					//character = new SingleByteCharacter(afterCharacter[0]);
					character = creator->Make(afterCharacter);
				}

				i++;
			}

			//2. 2byte�� ���
			else {
				//character = new DoubleByteCharacter(afterCharacter);
				character = creator->Make(afterCharacter);
				i = i + 2;
			}

			//3. ���๮�ڰ� �ƴϸ� row�� Add�Ѵ�.
			if (isLineChanged == FALSE) {
				this->findSameText->notePadForm->row = this->findSameText->notePadForm->note->GetChild(k);
				this->findSameText->notePadForm->row->Add(l, character); //row->Add(character);
				//row->MoveCurrent(row->GetLength());
				l++;
				j++;
			}

		}
		//3.1.2. �������� �� ��, ���� �� ������ ���ڸ� ���� �� ���ڿ� �������´�.******************
		if (isThereRestOnes == TRUE) {

			// ������ ���� ���� ���� ����
			this->findSameText->notePadForm->row = this->findSameText->notePadForm->note->GetChild(restY);
			rowLength = this->findSameText->notePadForm->row->GetLength();
			restLength = rowLength - restX;

			// �ֱٱ����� �߰��� ���� ����
			currentRow = this->findSameText->notePadForm->note->GetChild(k);

			// �ֱٱ����� �ٿ� ������ ���� ���ڸ� �߰��Ѵ�.
			while (restLength > 0) {
				character = this->findSameText->notePadForm->row->GetChild(restX);
				currentRow->Add(character);
				this->findSameText->notePadForm->row->Remove(restX);
				j++;
				restLength--;
			}

		}
		//******************************************************************************************

		//3.1.3. �ٲ� ��ǥ�� �����Ѵ�.
		*changedY = k;
		//changedX = currentX + j;
		if (lastLineChanged == TRUE) {
			*changedX = j;
		}
		else {
			*changedX = firstX + j;
		}
	}

#if 0
	//ĳ���� ���� ��ġ�� �����Ѵ�.
	this->findSameText->notePadForm->note->MoveCurrent(changedY);
	row = this->findSameText->notePadForm->note->GetChild(changedY);
	row->MoveCurrent(changedX);
#endif

	this->findSameText->notePadForm->selectWords->afterEndY = *changedY;


	if (creator != NULL) {
		delete creator;
		creator = NULL;
	}


}

void FindAndReplace::RemoveWordForRedo(int firstX, int firstY, int beforeEndX, int beforeEndY) {

	int difference;
	Glyph* row;
	Glyph* endRow;
	Glyph* character;
	BOOL isAppend;
	int tempEndY = beforeEndY;
	int changedX;
	int changedY;
	int limitX;
	int i;
	int j;

	row = this->findSameText->notePadForm->note->GetChild(firstY);

	//1.3. firstY, endY�� ���� ���,
	if (firstY == beforeEndY) {
		//1.3.1. endX�� firstX�� ������ ���Ѵ�.
		difference = beforeEndX - firstX;

		//1.3.2. ������ 0���� ū ���� firstX��° ���ڸ� �����ش�.
		while (difference > 0) {
			row->Remove(firstX);
			difference--;
		}
	}

	//1.4. endY�� firstY���� �� ū ���,
	else if (tempEndY > firstY) {

		//1.4.1. �ٿ������ �ϴ� ������ Ȯ���Ѵ�.
		endRow = this->findSameText->notePadForm->note->GetChild(tempEndY);
		if (firstX > 0 && tempEndY < endRow->GetLength()) {
			isAppend = TRUE;
		}

		//1.4.2. firstY�� endY���� �۰ų� ���� ���� �ݺ��Ѵ�.
		i = firstY;

		while (i <= tempEndY) {
			row = this->findSameText->notePadForm->note->GetChild(i);

			//1.4.2.1. j�� ���Ѵ�.
			if (i <= firstY) {
				j = firstX;
			}
			else {
				j = 0;
			}

			//1.4.2.2. limitX �� �����Ѵ�.
			if (i >= tempEndY) {
				limitX = beforeEndX;
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
				if (i < this->findSameText->notePadForm->note->GetLength() - 1) {
					this->findSameText->notePadForm->note->Remove(i);
				}
				//*******************************************************************************
				//this->notePadForm->note->Remove(i);
				tempEndY--;
			}
			else {
				i++;
			}
		}

		//1.4.3. isAppend�� true�̸�. 
		if (isAppend == TRUE) {
			row = this->findSameText->notePadForm->note->GetChild(firstY);
			endRow = this->findSameText->notePadForm->note->GetChild(firstY + 1);
			i = 0;

			//1.4.3.1. firstY�� row�� �������´�.
			while (i < endRow->GetLength()) {
				character = endRow->GetChild(i);
				row->Add(character);
				i++;
			}
			//1.4.3.2. firstY+1�� row�� �����.
			this->findSameText->notePadForm->note->Remove(firstY + 1);
		}
	}

#if 0
	//1.5. ĳ���� �ű� ��ǥ�� ���Ѵ�.
	changedY = firstY;
	changedX = firstX;

	this->findSameText->notePadForm->note->MoveCurrent(changedY);
	this->findSameText->notePadForm->row = this->findSameText->notePadForm->note->GetChild(changedY);
	this->findSameText->notePadForm->row->MoveCurrent(changedX);
#endif
}



