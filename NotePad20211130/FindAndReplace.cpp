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
	//(21.09.18 추가)
	FactoryProduct factoryProduct(this->findSameText->notePadForm);
	bool isThereLineChange = false;

	//(21.09.18 추가) findStr에 개행문자가 포함되어 있는지 확인한다.
	size_t nPos = findStr.find("\n");
	if (nPos != string::npos) { //개행문자가 있는 경우,
		isThereLineChange = true;
	}


	//예외초기화
	(*exception) = 0;
	(*isReplaced) = 0;

	//0. 찾을 내용, 바꿀 내용의 총 개수를 구한다.
	const unsigned char* constStr = reinterpret_cast<const unsigned char*> (findStr.c_str());
	int findLength = _mbslen(constStr);

	//constStr = reinterpret_cast<const unsigned char*> (const_cast<const char*>(replaceStr.c_str()));
	constStr = reinterpret_cast<const unsigned char*> (replaceStr.c_str());
	int replaceLength = _mbslen(constStr);


	//1. 찾을 내용, 바꿀 내용, 현재 선택된 내용, 대소문자 구분, 자동개행 전 캐럿의 좌표를 입력받는다.
	//2. 현재 선택된 단어가 찾고자 하는 단어와 같은지 확인한다.
	  //2.1. 대소문자 구분하면,
	if (matchCase == true && strcmp(cFindStr, cSelectedStr) == 0) {
		isSameText = true;
		this->isReplaced = true;
		(*isReplaced) = 1;

	}

	//2.2. 대소문자 구분 안하면,
	else if (matchCase == false && stricmp(cFindStr, cSelectedStr) == 0) {
		isSameText = true;
		this->isReplaced = true;
		(*isReplaced) = 1;
	}

	//2.3. 선택된 단어와 찾고자 하는 단어가 같으면, (바꾸려는 단어가 아닌)
	if (isSameText == true) {
		//2.3.0. string형을 char형으로 변환한다.
		//chaCharacter = new char(replaceStr.length());
		chaCharacter = new char[replaceStr.length() + 1];
		strcpy(chaCharacter, cReplaceStr);

#if 0
		keyAction = factoryProduct.Make(VK_BACK, NULL, NULL);
		keyAction->KeyDown();
#endif
		RemoveSelectedWord();

		//(21.09.18 추가) 2.3.1. 지우고 난 후 캐럿의 위치를 확인한다.
		currentY = this->findSameText->notePadForm->note->GetCurrent();
		row = this->findSameText->notePadForm->note->GetChild(currentY);
		currentX = row->GetCurrent();

#if 0
		//2.3.1.1. 자동개행이 눌려져 있는 경우, 자동개행전 위치를 구해준다.
		if (this->findSameText->notePadForm->isLineChangeButtonClicked == true) {
			this->findSameText->notePadForm->lineChange->FindCaretBeforeLineChange(currentX, currentY, &currentX, &currentY);
			row = this->findSameText->notePadForm->note->GetChild(currentY);
		}
#endif

		firstCurrentY = currentY;

#if 0
		//2.3.1. 선택된 단어를 지운다.
		selectedFirstX = beforeLineChangeCurrentX - findLength;
		row = this->findSameText->notePadForm->note->GetChild(firstCurrentY);
		//row = this->glyphs.GetAt(firstCurrentY);


		while (i <= findLength) {
			row->Remove(selectedFirstX);
			i++;
		}
#endif


		//2.3.2. 바꿀 내용을 적는다.
		creator = new CreateProduct();
		i = 0;
		j = currentX;  //selectedFirstX
		while (i < replaceStr.length()) {
			afterCharacter[0] = chaCharacter[i];
			afterCharacter[1] = chaCharacter[i + 1];

			//1. 1byte인 경우,
			if (afterCharacter[0] >= 0 && afterCharacter[0] < 128) {
				afterCharacter[1] = '\0';
				character = creator->Make(afterCharacter);
				i++;
			}
			//2. 2byte인 경우,
			else {
				character = creator->Make(afterCharacter);
				i = i + 2;
			}
			//3. 내용을 적는다.
			row->Add(j, character);
			j++;
		}

		//2.3.3. 캐럿의 좌표를 새로 적은 내용의 맨 뒤로 정한다.
		beforeLineChangeCurrentX = currentX + replaceLength;

		//(21.09.19 추가) 
		this->findSameText->notePadForm->selectWords->afterEndX = beforeLineChangeCurrentX;
		this->findSameText->notePadForm->selectWords->afterEndY = currentY;


		//2.3.1.1. 자동개행이 눌려져 있는 경우, 자동개행후 위치를 구해준다.
		if (this->findSameText->notePadForm->isLineChangeButtonClicked == true) {

			//1.5.1. 펼친 문서를 다시 자동개행한다. (ON)
			this->findSameText->notePadForm->lineChange->LineChangeButtonClicked();

			//1.5.2. 자동개행 후 위치를 구한다.
			this->findSameText->notePadForm->lineChange
				->FindCaretAfterLineChange(this->findSameText->notePadForm->selectWords->afterEndX, 
					this->findSameText->notePadForm->selectWords->afterEndY, 
					&this->findSameText->notePadForm->selectWords->afterEndX, 
					&this->findSameText->notePadForm->selectWords->afterEndY);

			//1.5.3. 다시 문서를 펼쳐준다. (OFF)
			this->findSameText->notePadForm->lineChange->LineChangeButtonNotClicked();
		}
	}
	//FindSameTextWithDown과 같음 ----------------------------------------------------------------------------------
	//3. 문서의 끝이 아닌동안 반복한다.

	//(21.08.30 추가) 주위에 배치가 눌려져있는 경우,
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

	//(21.09.18 추가 수정) i를 초기화한다.
	if (searchAround == true) {
		i = 0;
	}
	else {
		i = beforeLineChangeCurrentX;
	}

	//3. 문서의 끝이 아닌동안 반복한다.
	while (isThereLineChange == false && currentY < noteLength && isThereSameText == false) {

		//3.1. 현재 캐럿이 있는 줄을 찾는다.
		row = this->findSameText->notePadForm->note->GetChild(currentY);
		//row = this->glyphs.GetAt(currentY);
		rowLength = row->GetLength();

		//3.2. i를 초기화하고 restLength를 정한다.
		if (currentY == firstCurrentY) {
			restLength = rowLength - i;
		}
		else {
			i = 0;
			restLength = rowLength;
		}

		//3.3. restLength가 findLength보다 크거나 같고, 같은 단어가 없는 동안 반복한다.
		while (restLength >= findLength && isThereSameText == false) {

			//3.3.1. 해당 줄에서 찾을 문자의 총 개수만큼 문자를 만든다.
			temp.clear(); //문자열 지우기
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

			//3.3.2. 대소문자 구분하면,
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
			//3.3.3. 대소문자 구분하지 않으면,
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

	//4. 개행 문자가 포함되어 있는 경우, 문서의 끝이 아닌동안 반복한다.
	while (isThereLineChange == true && currentY < noteLength && isThereSameText == false) {

		//4.1. temp 초기화한다.
		temp.clear();
		tempCurrentY = currentY;
		j = i;
		k = 0;

		//4.2. findLength만큼 비교할 문자를 만든다.
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

		//4.3. 대소문자 구분하면,
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

		//4.4. 대소문자 구분하지 않으면,
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

	//5. 그 다음 같은 단어는 없지만, 처음 들어왔을 때 replace한 경우,
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

		//4.1. 그 다음 같은 단어도 없고, 처음 들어왔을 때 replace하지 않은 경우, firstXY, endXY에 0을 입력한다.
		else if (isSameText == false && searchAround == false) {
			(*firstY) = 0;
			(*firstX) = 0;
			(*endY) = 0;
			(*endX) = 0;
		}
	}
	//------------------------------------------------------------------------------------------------------

	//할당해제 해준다.
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


	//0. 자동개행버튼이 눌려진 경우,
	if (this->findSameText->notePadForm->isLineChangeButtonClicked == TRUE) {

		//0.0  펼친 문서를 다시 자동개행한다. (ON)
		this->findSameText->notePadForm->lineChange->LineChangeButtonClicked();

		//0.1. 현재 currentXY, firstXY, endXY를 구한다.
		currentY = this->findSameText->notePadForm->note->GetCurrent();
		row = this->findSameText->notePadForm->note->GetChild(currentY);
		currentX = row->GetCurrent();

		firstY = this->findSameText->notePadForm->selectWords->GetFirstY();
		firstX = this->findSameText->notePadForm->selectWords->GetFirstX();
		endY = this->findSameText->notePadForm->selectWords->GetEndY();
		endX = this->findSameText->notePadForm->selectWords->GetEndX();

		//0.2. 자동개행 전 firstXY, endXY의 좌표를 구한다.
		this->findSameText->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
		this->findSameText->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);
		this->findSameText->notePadForm->lineChange->FindCaretBeforeLineChange(currentX, currentY, &currentX, &currentY);

		//0.3. 자동개행된 문서를 펼친다.
		this->findSameText->notePadForm->lineChange->LineChangeButtonNotClicked();

		//0.4. 현재 row를 구한다.
		row = this->findSameText->notePadForm->note->GetChild(firstY); //currentY
	}

	//1. 자동개행버튼 안 눌린 경우,
	else {
		//1. 현재 캐럿의 좌표를 구한다.
		currentY = this->findSameText->notePadForm->note->GetCurrent();

		//2. 현재 row를 구한다.
		row = this->findSameText->notePadForm->note->GetChild(firstY); //currentY
		currentX = row->GetCurrent();
	}


	//(21.08.19 추가) 선택하다 포함 ******************************************************
	//3. firstX,Y, endX,Y를 구한다.
	int startX = firstX;
	int startY = firstY;
	int lastX = endX;
	int lastY = endY;

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

	//6. 선택된 범위가 있는 경우만 계산한다.
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
		endRow = this->findSameText->notePadForm->note->GetChild(endY);
		if (firstX > 0 && endX < endRow->GetLength()) {
			isAppend = TRUE;
		}

		//1.4.2. firstY가 endY보다 작거나 같은 동안 반복한다.
		i = firstY;
		while (i <= endY) {
			row = this->findSameText->notePadForm->note->GetChild(i);

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

		//1.4.3. isAppend가 true이면. 
		if (isAppend == TRUE) {
			row = this->findSameText->notePadForm->note->GetChild(firstY);
			endRow = this->findSameText->notePadForm->note->GetChild(firstY + 1);
			i = 0;

			//1.4.3.1. firstY의 row에 붙혀적는다.
			while (i < endRow->GetLength()) {
				character = endRow->GetChild(i);
				row->Add(character);
				i++;
			}
			//1.4.3.2. firstY+1의 row를 지운다.
			this->findSameText->notePadForm->note->Remove(firstY + 1);
		}
	}

	//1.5. 캐럿의 옮길 좌표를 정한다.
	changedY = firstY;
	changedX = firstX;

	this->findSameText->notePadForm->note->MoveCurrent(changedY);
	this->findSameText->notePadForm->row = this->findSameText->notePadForm->note->GetChild(changedY);
	this->findSameText->notePadForm->row->MoveCurrent(changedX);

#if 0
	//7. 자동개행이 눌려진 경우,
	if (this->findSameText->notePadForm->isLineChangeButtonClicked == TRUE) {

		//7.1. 펼친 문서를 다시 자동개행한다. (ON)
		this->findSameText->notePadForm->lineChange->LineChangeButtonClicked();

		//7.2. 자동개행 후 이동할 캐럿의 위치를 구한다.
		this->findSameText->notePadForm->lineChange->FindCaretAfterLineChange(changedX, changedY);
	}
	else {
		this->findSameText->notePadForm->note->MoveCurrent(changedY);
		this->findSameText->notePadForm->row = this->findSameText->notePadForm->note->GetChild(changedY);
		this->findSameText->notePadForm->row->MoveCurrent(changedX);
	}

	//1.6. 캐럿을 이동한다.
	this->findSameText->notePadForm->isOnImeCharForMove = FALSE;
	this->findSameText->notePadForm->isFirstComposingForMove = FALSE;
#endif

	//2. firstXY, endXY를 초기화한다. ****************************
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

	//3.1. 붙혀적다의 경우
	if (firstX >= rowLength) {

		k = firstY;
		this->findSameText->notePadForm->row = this->findSameText->notePadForm->note->GetChild(k);

		//3.1.1 str의 length만큼 반복한다.
		while (i < strLength) {//character_[i]!= NULL
			//3.1.1.1. 노트의 현재 줄을 구한다.						
			isLineChanged = FALSE;

			afterCharacter[0] = character_[i];
			afterCharacter[1] = character_[i + 1];

			//1.1byte인 경우,
			if (afterCharacter[0] >= 0 && afterCharacter[0] < 128) {

				//1.0. 첫번째 위치에 널문자를 넣어준다.
				afterCharacter[1] = '\0';

				//1.1. 개행문자이면, 
				if (afterCharacter[0] == '\n') {
					//1.1.1. 새로운 문자를 만든다.
					k++;

					//row = new Row();
					this->findSameText->notePadForm->row = creator->Make(afterCharacter);
					this->findSameText->notePadForm->note->Add(k, this->findSameText->notePadForm->row);
					//row = this->notePadForm->note->GetChild(k);


					//1.1.2. 조건을 바꾼다.
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

			//2. 2byte인 경우
			else {
				//character = new DoubleByteCharacter(afterCharacter);
				character = creator->Make(afterCharacter);
				i = i + 2;
			}

			//3. 개행문자가 아니면 row에 Add한다.
			if (isLineChanged == FALSE) {
				//this->notePadForm->note->GetChild(k);
				this->findSameText->notePadForm->row->Add(character); //row->Add(character); ************************************
				//row->MoveCurrent(row->GetLength());
				j++;
			}

		}

		//3.1.2. 바뀐 좌표를 설정한다.
		*changedY = k;
		//changedX = currentX + j;
		if (lastLineChanged == TRUE) {
			*changedX = j;
		}
		else {
			*changedX = firstX + j;
		}
	}

	//3.2. 끼워적다의 경우
	else if (firstX < rowLength) {

		//3.2.1 str의 length만큼 반복한다.
		int l = firstX;
		k = firstY;
		i = 0;

		//3.1.1 str의 length만큼 반복한다.
		while (i < strLength) {//character_[i]!= NULL
			//3.1.1.1. 노트의 현재 줄을 구한다.
			//row = this->notePadForm->note->GetChild(k);

			isLineChanged = FALSE;

			afterCharacter[0] = character_[i];
			afterCharacter[1] = character_[i + 1];

			//1.1byte인 경우,
			if (afterCharacter[0] >= 0 && afterCharacter[0] < 128) {

				//1.0. 첫번째 위치에 널문자를 넣어준다.
				afterCharacter[1] = '\0';

				//1.1. 개행문자이면,
				if (afterCharacter[0] == '\n') {

					//나머지 여분의 글자가 있는지 확인한다.**************************			
					restX = firstX + j;

					if (this->findSameText->notePadForm->row->GetLength() > restX) {
						isThereRestOnes = TRUE;
						restY = firstY;
					}
					//***************************************************************

					//1.1.1. 새로운 문자를 만든다.
					k++;

					//row = new Row();
					this->findSameText->notePadForm->row = creator->Make(afterCharacter);
					this->findSameText->notePadForm->note->Add(k, this->findSameText->notePadForm->row);

					//1.1.3. 조건을 바꾼다.
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

			//2. 2byte인 경우
			else {
				//character = new DoubleByteCharacter(afterCharacter);
				character = creator->Make(afterCharacter);
				i = i + 2;
			}

			//3. 개행문자가 아니면 row에 Add한다.
			if (isLineChanged == FALSE) {
				this->findSameText->notePadForm->row = this->findSameText->notePadForm->note->GetChild(k);
				this->findSameText->notePadForm->row->Add(l, character); //row->Add(character);
				//row->MoveCurrent(row->GetLength());
				l++;
				j++;
			}

		}
		//3.1.2. 끼워적고 난 후, 개행 전 나머지 문자를 개행 후 문자에 붙혀적는다.******************
		if (isThereRestOnes == TRUE) {

			// 나머지 문자 남은 줄의 정보
			this->findSameText->notePadForm->row = this->findSameText->notePadForm->note->GetChild(restY);
			rowLength = this->findSameText->notePadForm->row->GetLength();
			restLength = rowLength - restX;

			// 최근까지의 추가한 줄의 정보
			currentRow = this->findSameText->notePadForm->note->GetChild(k);

			// 최근까지의 줄에 나머지 줄의 글자를 추가한다.
			while (restLength > 0) {
				character = this->findSameText->notePadForm->row->GetChild(restX);
				currentRow->Add(character);
				this->findSameText->notePadForm->row->Remove(restX);
				j++;
				restLength--;
			}

		}
		//******************************************************************************************

		//3.1.3. 바뀐 좌표를 설정한다.
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
	//캐럿의 현재 위치를 설정한다.
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

	//1.3. firstY, endY가 같은 경우,
	if (firstY == beforeEndY) {
		//1.3.1. endX와 firstX의 차액을 구한다.
		difference = beforeEndX - firstX;

		//1.3.2. 차액이 0보다 큰 동안 firstX번째 글자를 지워준다.
		while (difference > 0) {
			row->Remove(firstX);
			difference--;
		}
	}

	//1.4. endY가 firstY보다 더 큰 경우,
	else if (tempEndY > firstY) {

		//1.4.1. 붙여적어야 하는 조건을 확인한다.
		endRow = this->findSameText->notePadForm->note->GetChild(tempEndY);
		if (firstX > 0 && tempEndY < endRow->GetLength()) {
			isAppend = TRUE;
		}

		//1.4.2. firstY가 endY보다 작거나 같은 동안 반복한다.
		i = firstY;

		while (i <= tempEndY) {
			row = this->findSameText->notePadForm->note->GetChild(i);

			//1.4.2.1. j를 정한다.
			if (i <= firstY) {
				j = firstX;
			}
			else {
				j = 0;
			}

			//1.4.2.2. limitX 를 결정한다.
			if (i >= tempEndY) {
				limitX = beforeEndX;
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

		//1.4.3. isAppend가 true이면. 
		if (isAppend == TRUE) {
			row = this->findSameText->notePadForm->note->GetChild(firstY);
			endRow = this->findSameText->notePadForm->note->GetChild(firstY + 1);
			i = 0;

			//1.4.3.1. firstY의 row에 붙혀적는다.
			while (i < endRow->GetLength()) {
				character = endRow->GetChild(i);
				row->Add(character);
				i++;
			}
			//1.4.3.2. firstY+1의 row를 지운다.
			this->findSameText->notePadForm->note->Remove(firstY + 1);
		}
	}

#if 0
	//1.5. 캐럿의 옮길 좌표를 정한다.
	changedY = firstY;
	changedX = firstX;

	this->findSameText->notePadForm->note->MoveCurrent(changedY);
	this->findSameText->notePadForm->row = this->findSameText->notePadForm->note->GetChild(changedY);
	this->findSameText->notePadForm->row->MoveCurrent(changedX);
#endif
}



