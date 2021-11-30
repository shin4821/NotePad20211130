//FindSameText.cpp
#include"FindSameText.h"
#include"NotePadForm.h"
#include"Note.h"
#include"LineChange.h"
#include"SelectWords.h"
#include"FindAndReplace.h"
#include"Scroll.h"

FindSameText::FindSameText(NotePadForm* notePadForm) {
	this->notePadForm = notePadForm;
	this->beforeFindStr = "";
	this->replaceStr = "";
	this->findStr = "";
	this->matchCase = FALSE;
	this->searchDown = TRUE;
	this->searchAround = FALSE;
	this->exception = FALSE;
	this->findAndReplace = new FindAndReplace(this);
}

FindSameText::~FindSameText() {

//#if 0
	if (this->findAndReplace != NULL) {
		delete this->findAndReplace;
		this->findAndReplace = NULL;
	}
//#endif
}

void FindSameText::SearchText() {
	int currentY;
	int currentX;
	Glyph* row;
	BOOL exception = FALSE;

	//1. 찾기 버튼을 클릭했을 때
	//1.1. 현재 캐럿의 위치를 찾는다.
	currentY = this->notePadForm->note->GetCurrent();
	row = this->notePadForm->note->GetChild(currentY);
	currentX = row->GetCurrent();


	//예외) 검색방향이 위쪽이면 이전 찾았던 단어와 현재 찾고자 하는 단어와 같으면 firstXY를 currentXY로 넘긴다.******
    // ** 그렇지 않으면 캐럿이 endXY여서 계속 같은 단어에 머무른다.
	if (this->searchDown == FALSE && this->notePadForm->findSameText->beforeFindStr != "" &&
		this->notePadForm->findSameText->beforeFindStr == this->notePadForm->findSameText->findStr &&
		(this->notePadForm->selectWords->firstY > 0 || this->notePadForm->selectWords->firstX > 0 ||
			this->notePadForm->selectWords->endY > 0 || this->notePadForm->selectWords->endX > 0)) {

		currentY = this->notePadForm->selectWords->firstY;
		currentX = this->notePadForm->selectWords->firstX;

		exception = TRUE;
	}
	//****************************************************************************************************************



	//1.2. 자동개행이 눌려져 있는 경우,
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		//1.2.0. 예외인 상황에서, 자동개행 전 캐럿의 위치를 찾는다.
		if (exception == TRUE) {
			this->notePadForm->lineChange->FindCaretBeforeLineChange(this->notePadForm->selectWords->firstX ,
				this->notePadForm->selectWords->firstY , &currentX, &currentY);
		}
		//1.2.1. 예외가 아닌 상황에서, 자동개행 전 캐럿의 위치를 찾는다.
		else {
			this->notePadForm->lineChange->FindCaretBeforeLineChange(&currentX, &currentY);
		}


		//1.2.2. 자동개행 된 문서를 펼친다.(OFF)
		this->notePadForm->lineChange->LineChangeButtonNotClicked();
	}

	//1.3. 검색을 아랫쪽 방향으로 진행하면, 해당 방향으로 같은 단어가 있는지 확인한다.
	if (this->searchDown == TRUE) {

		//(21.11.04.추가) 
		this->notePadForm->isUp = FALSE; //주위에 배치일때 맨밑에서 맨위로 갈때 반대로 설정
		this->notePadForm->isDown = FALSE;


		this->notePadForm->note->FindSameTextWithDown((LPCTSTR)this->findStr, this->matchCase, currentY, currentX,
			&this->notePadForm->selectWords->firstY, &this->notePadForm->selectWords->firstX,
			&this->notePadForm->selectWords->endY, &this->notePadForm->selectWords->endX);

		//1.3.1. 같은 단어가 없고, 주위에 배치여부가 TRUE이면 위,아래에 따라 조건을 달리하여 같은단어가 있는지 확인한다.
		if (this->searchAround == TRUE && (this->notePadForm->selectWords->firstY <= 0 &&
			this->notePadForm->selectWords->firstX <= 0 && this->notePadForm->selectWords->endY <= 0 &&
			this->notePadForm->selectWords->endX <= 0)) {
			 
			//(21.11.04.추가) 
			this->notePadForm->isUp = TRUE; //주위에 배치일때 맨밑에서 맨위로 갈때 반대로 설정
			this->notePadForm->isDown = FALSE;


			//(0,0)부터 다시 찾다.
			this->notePadForm->note->FindSameTextWithDown((LPCTSTR)this->findStr, this->matchCase, 0, 0,
				&this->notePadForm->selectWords->firstY, &this->notePadForm->selectWords->firstX,
				&this->notePadForm->selectWords->endY, &this->notePadForm->selectWords->endX);
		}
	}

	//1.4. 검색을 위쪽 방향으로 진행하면, 해당 방향으로 같은 단어가 있는지 확인한다.
	if (this->searchDown == FALSE) {

		//(21.11.04.추가) 
		this->notePadForm->isUp = TRUE; //주위에 배치일때 맨밑에서 맨위로 갈때 반대로 설정
		this->notePadForm->isDown = FALSE;


		this->notePadForm->note->FindSameTextWithUp((LPCTSTR)this->findStr, this->matchCase, currentY, currentX,
			&this->notePadForm->selectWords->firstY, &this->notePadForm->selectWords->firstX,
			&this->notePadForm->selectWords->endY, &this->notePadForm->selectWords->endX);

		//1.4.1. 같은 단어가 없고, 주위에 배치여부가 TRUE이면 위,아래에 따라 조건을 달리하여 같은단어가 있는지 확인한다.
		if (this->searchAround == TRUE && (this->notePadForm->selectWords->firstY <= 0 &&
			this->notePadForm->selectWords->firstX <= 0 && this->notePadForm->selectWords->endY <= 0 &&
			this->notePadForm->selectWords->endX <= 0)) {

			//(21.11.04.추가) 
			this->notePadForm->isUp = FALSE; //주위에 배치일때 맨밑에서 맨위로 갈때 반대로 설정
			this->notePadForm->isDown = FALSE;

			int noteLength = this->notePadForm->note->GetLength();
			row = this->notePadForm->note->GetChild(noteLength - 1);
			int rowLength = row->GetLength();

			//맨끝줄, 끝좌표에서 다시 찾다.
			this->notePadForm->note->FindSameTextWithUp((LPCTSTR)this->findStr, this->matchCase, noteLength-1, rowLength,
				&this->notePadForm->selectWords->firstY, &this->notePadForm->selectWords->firstX,
				&this->notePadForm->selectWords->endY, &this->notePadForm->selectWords->endX);
		}
	}
	
	//1.5. 자동개행이 눌려져있는 경우,
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		//1.5.1. 펼친 문서를 다시 자동개행한다. (ON)
		this->notePadForm->lineChange->LineChangeButtonClicked();

		//1.5.2. first좌표, end좌표의 자동개행 후의 위치를 구한다.
		this->notePadForm->lineChange->FindCaretAfterLineChange(this->notePadForm->selectWords->firstX,
			this->notePadForm->selectWords->firstY, &this->notePadForm->selectWords->firstX,
			&this->notePadForm->selectWords->firstY);

		this->notePadForm->lineChange->FindCaretAfterLineChange(this->notePadForm->selectWords->endX,
			this->notePadForm->selectWords->endY, &this->notePadForm->selectWords->endX,
			&this->notePadForm->selectWords->endY);
	}

	//1.6. 같은 단어가 있는 경우, end좌표로 캐럿을 이동한다.
	if (this->notePadForm->selectWords->firstX > 0 || this->notePadForm->selectWords->firstY > 0 ||
		this->notePadForm->selectWords->endX > 0 || this->notePadForm->selectWords->endY > 0) {

		this->notePadForm->note->MoveCurrent(this->notePadForm->selectWords->endY);
		row = this->notePadForm->note->GetChild(this->notePadForm->selectWords->endY);
		row->MoveCurrent(this->notePadForm->selectWords->endX);
	}

}

void FindSameText::ReplaceAndSearchText() {
	int currentY;
	int currentX;
	Glyph* row;
	Glyph* selectedRow;
	Glyph* character;
	string strCharacter;
	string selectedStr;
	int i;
	int j;
	int exception;
	int firstX = this->notePadForm->selectWords->GetFirstX();
	int firstY = this->notePadForm->selectWords->GetFirstY();
	int endX = this->notePadForm->selectWords->GetEndX();
	int endY = this->notePadForm->selectWords->GetEndY();
	BOOL isBigger = FALSE;
	BOOL isReplaced = FALSE;
	int startX;
	int startY;
	int lastX;
	int lastY;
	FindAndReplace* findAndReplace = NULL;


	//(21.11.04.추가) 
	this->notePadForm->isUp = FALSE; //주위에 배치일때 맨밑에서 맨위로 갈때 반대로 설정
	this->notePadForm->isDown = FALSE;

	//_______________________________________________
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
	//________________________________________________


	//(21.09.16 추가)
	this->notePadForm->selectWords->beforeFirstX = firstX;
	this->notePadForm->selectWords->beforeFirstY = firstY;
	this->notePadForm->selectWords->beforeEndX = endX;
	this->notePadForm->selectWords->beforeEndY = endY;
	this->notePadForm->selectWords->afterEndY = endY;


	//1. 바꾸기 버튼을 클릭했을 때
	//1.1. 현재 캐럿의 위치를 찾는다.
	currentY = this->notePadForm->note->GetCurrent();
	row = this->notePadForm->note->GetChild(currentY);
	currentX = row->GetCurrent();

	//1.2. 자동개행이 눌려져 있는 경우,
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		//1.2.1. 예외가 아닌 상황에서, 자동개행 전 캐럿의 위치를 찾는다.
		this->notePadForm->lineChange->FindCaretBeforeLineChange(&currentX, &currentY);

		//1.3.1. 자동개행인 경우, 자동 개행 전 firstXY, endXY를 구한다.
		this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
		this->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);
		
		//1.2.2. 자동개행 된 문서를 펼친다.(OFF)
		this->notePadForm->lineChange->LineChangeButtonNotClicked();
		this-> notePadForm->selectWords->isLineChangeButtonClicked = TRUE;
		this->notePadForm->selectWords->windowWidth = this->notePadForm->scroll->windowWidth;
	}

	//1.3.2. 선택된 단어가 있는 줄을 찾는다.
	selectedRow = this->notePadForm->note->GetChild(firstY);



	//1.3.3. 해당 단어를 찾는다.
	i = firstX;
	j = firstY;
	int selectedRowLength;
	
	if (firstY == endY) {
		while (i < endX) {
			character = selectedRow->GetChild(i);
			strCharacter = character->GetContent();
			selectedStr.append(strCharacter);
			i++;
		}
	}
	else {
		while (j <= endY) {

			selectedRow = this->notePadForm->note->GetChild(j);
			selectedRowLength = selectedRow->GetLength();

			if (j == endY) {
				selectedRowLength = endX;
			}
			while (i < selectedRowLength) {
				character = selectedRow->GetChild(i);
				strCharacter = character->GetContent();
				selectedStr.append(strCharacter);
				i++;	
			}

			j++;

			if (j <= endY) {
				selectedStr.append("\n");
				i = 0;
			}
		}
	}


	//--------------------------------------------------------------------------------------------------------
#if 0
	//주위에 배치인 경우,
	if (this->searchAround == TRUE && this->exception == TRUE) {


		this->notePadForm->note->ReplaceSameText((LPCTSTR)this->findStr, (LPCTSTR)this->replaceStr, selectedStr, 
			this->matchCase, this->searchAround,
			currentY, currentX, &this->notePadForm->selectWords->firstY, &this->notePadForm->selectWords->firstX,
			&this->notePadForm->selectWords->endY, &this->notePadForm->selectWords->endX, &this->exception);


		this->searchAround = FALSE;
		this->exception = FALSE;
	}

	else {
		//1.4. 단어를 바꾼 후, 다음 단어를 찾는다.
		this->notePadForm->note->ReplaceSameText((LPCTSTR)this->findStr, (LPCTSTR)this->replaceStr, selectedStr, 
			this->matchCase, this->searchAround,
			currentY, currentX, &this->notePadForm->selectWords->firstY, &this->notePadForm->selectWords->firstX,
			&this->notePadForm->selectWords->endY, &this->notePadForm->selectWords->endX, &this->exception);
	}
#endif
	//-------------------------------------------------------------------------------------
	//1.4.(21.09.17 수정) 단어를 바꾼 후, 다음 단어를 찾는다. 
#if 0
	this->notePadForm->note->ReplaceSameText((LPCTSTR)this->findStr, (LPCTSTR)this->replaceStr, selectedStr,
		this->matchCase, FALSE,
		currentY, currentX, &this->notePadForm->selectWords->firstY, &this->notePadForm->selectWords->firstX,
		&this->notePadForm->selectWords->endY, &this->notePadForm->selectWords->endX, &this->exception);
#endif

	findAndReplace = new FindAndReplace(this);
	
	// 처음에 '바꾸다' 눌렀을 때는 주위에 배치를 FALSE로 설정해준다.
	findAndReplace->ReplaceSameText((LPCTSTR)this->findStr, (LPCTSTR)this->replaceStr, selectedStr,
		this->matchCase, FALSE,
		currentY, currentX, &this->notePadForm->selectWords->firstY, &this->notePadForm->selectWords->firstX,
		&this->notePadForm->selectWords->endY, &this->notePadForm->selectWords->endX, &this->exception,
		&this->notePadForm->selectWords->isReplaced);

	isReplaced = this->notePadForm->selectWords->isReplaced;

	//주위에 배치인 경우,
	if ((this->searchAround == TRUE && this->exception == TRUE) ||
		(this->notePadForm->selectWords->firstY<=0 && this->notePadForm->selectWords->firstX <=0 &&
			this->notePadForm->selectWords->endY<=0 && this->notePadForm->selectWords->endX<=0)) {


		//(21.11.04.추가) 
		this->notePadForm->isUp = TRUE; //주위에 배치일때 맨밑에서 맨위로 갈때 반대로 설정
		this->notePadForm->isDown = FALSE;


		//1.2. 선택된 단어가 있는 줄을 찾는다.
		selectedRow = this->notePadForm->note->GetChild(this->notePadForm->selectWords->firstY);

		//1.3. 해당 단어를 찾는다.
		selectedStr.clear();

#if 0
		i = this->notePadForm->selectWords->firstX;
		while (i < this->notePadForm->selectWords->endX) {
			character = selectedRow->GetChild(i);
			strCharacter = character->GetContent();
			selectedStr.append(strCharacter);
			i++;
		}
#endif

#if 0
		//1.3.3. 해당 단어를 찾는다.
		i = this->notePadForm->selectWords->firstX;
		j = i = this->notePadForm->selectWords->firstY;

		if (firstY == endY) {
			while (i < endX) {
				character = selectedRow->GetChild(i);
				strCharacter = character->GetContent();
				selectedStr.append(strCharacter);
				i++;
			}
		}
		else {
			while (j <= endY) {

				selectedRow = this->notePadForm->note->GetChild(j);
				selectedRowLength = selectedRow->GetLength();

				if (j == endY) {
					selectedRowLength = endX;
				}
				while (i < selectedRowLength) {
					character = selectedRow->GetChild(i);
					strCharacter = character->GetContent();
					selectedStr.append(strCharacter);
					i++;
				}

				j++;

				if (j <= endY) {
					selectedStr.append("\n");
					i = 0;
				}
			}
		}
#endif

#if 0
		this->notePadForm->note->ReplaceSameText((LPCTSTR)this->findStr, (LPCTSTR)this->replaceStr, selectedStr,
			this->matchCase, this->searchAround,
			currentY, currentX, &this->notePadForm->selectWords->firstY, &this->notePadForm->selectWords->firstX,
			&this->notePadForm->selectWords->endY, &this->notePadForm->selectWords->endX, &this->exception);
#endif

		findAndReplace->ReplaceSameText((LPCTSTR)this->findStr, (LPCTSTR)this->replaceStr, selectedStr,
			this->matchCase, this->searchAround,
			currentY, currentX, &this->notePadForm->selectWords->firstY, &this->notePadForm->selectWords->firstX,
			&this->notePadForm->selectWords->endY, &this->notePadForm->selectWords->endX, &this->exception,
			&this->notePadForm->selectWords->isReplaced);


		this->notePadForm->selectWords->isReplaced = isReplaced;
		this->searchAround = FALSE;
		this->exception = FALSE;
	}
	//--------------------------------------------------------------------------------------
#if 0
	//1.4.1. 같은 단어가 없고, 주위에 배치여부가 TRUE이면 위,아래에 따라 조건을 달리하여 같은단어가 있는지 확인한다.
	if (this->searchAround == TRUE && this->exception == TRUE) {

		//(0,0)부터 다시 찾다.
#if 0
		this->notePadForm->note->ReplaceSameText((LPCTSTR)this->findStr, (LPCTSTR)this->replaceStr, selectedStr, 
			this->matchCase, 0, 0, &this->notePadForm->selectWords->firstY, 
			&this->notePadForm->selectWords->firstX,
			&this->notePadForm->selectWords->endY, &this->notePadForm->selectWords->endX, &this->exception);
#endif
		ReplaceAndSearchText(); //ReplaceSameText 들어갈때 
	}
#endif

	//1.5. 자동개행이 눌려져있는 경우,
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		//1.5.1. 펼친 문서를 다시 자동개행한다. (ON)
		this->notePadForm->lineChange->LineChangeButtonClicked();

		//1.5.2. first좌표, end좌표의 자동개행 후의 위치를 구한다.
		this->notePadForm->lineChange->FindCaretAfterLineChange(this->notePadForm->selectWords->firstX,
			this->notePadForm->selectWords->firstY, &this->notePadForm->selectWords->firstX,
			&this->notePadForm->selectWords->firstY);

		this->notePadForm->lineChange->FindCaretAfterLineChange(this->notePadForm->selectWords->endX,
			this->notePadForm->selectWords->endY, &this->notePadForm->selectWords->endX,
			&this->notePadForm->selectWords->endY);

#if 0
		//1.5.3. (21.09.19 추가) afterEndX, afterEndY 좌표의 자동개행 후의 위치를 구한다.
		this->notePadForm->lineChange->FindCaretAfterLineChange(this->notePadForm->selectWords->afterEndX,
			this->notePadForm->selectWords->afterEndY, &this->notePadForm->selectWords->afterEndX,
			&this->notePadForm->selectWords->afterEndY);
#endif
	}




	//1.6. 같은 단어가 있는 경우, end좌표로 캐럿을 이동한다.
	if (this->notePadForm->selectWords->firstX > 0 || this->notePadForm->selectWords->firstY > 0 ||
		this->notePadForm->selectWords->endX > 0 || this->notePadForm->selectWords->endY > 0) {

		this->notePadForm->note->MoveCurrent(this->notePadForm->selectWords->endY);
		row = this->notePadForm->note->GetChild(this->notePadForm->selectWords->endY);
		row->MoveCurrent(this->notePadForm->selectWords->endX);
	}


	if (findAndReplace != NULL) {
		delete findAndReplace;
	}

}

void FindSameText::ReplaceAllText() {

	int currentY;
	int currentX;
	Glyph* row;
	Glyph* selectedRow;
	Glyph* character;
	string strCharacter;
	string selectedStr;
	int i;
	int j;
	int exception;
	int firstX = this->notePadForm->selectWords->GetFirstX();
	int firstY = this->notePadForm->selectWords->GetFirstY();
	int endX = this->notePadForm->selectWords->GetEndX();
	int endY = this->notePadForm->selectWords->GetEndY();
	FindAndReplace* findAndReplace = NULL;


	//1. 바꾸기 버튼을 클릭했을 때
	//1.1. 현재 캐럿의 위치를 찾는다.
	currentY = this->notePadForm->note->GetCurrent();
	row = this->notePadForm->note->GetChild(currentY);
	currentX = row->GetCurrent();

	//1.2. 자동개행이 눌려져 있는 경우,
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		//1.2.1. 예외가 아닌 상황에서, 자동개행 전 캐럿의 위치를 찾는다.
		this->notePadForm->lineChange->FindCaretBeforeLineChange(&currentX, &currentY);
		this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
		this->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);

		//1.2.2. 자동개행 된 문서를 펼친다.(OFF)
		this->notePadForm->lineChange->LineChangeButtonNotClicked();
	}


	//1.3.2. 선택된 단어가 있는 줄을 찾는다.
	selectedRow = this->notePadForm->note->GetChild(firstY);

	//1.3.3. 해당 단어를 찾는다.
	i = firstX;
	j = firstY;
	int selectedRowLength;

	if (firstY == endY) {
		while (i < endX) {
			character = selectedRow->GetChild(i);
			strCharacter = character->GetContent();
			selectedStr.append(strCharacter);
			i++;
		}
	}
	else {
		while (j <= endY) {

			selectedRow = this->notePadForm->note->GetChild(j);
			selectedRowLength = selectedRow->GetLength();

			if (j == endY) {
				selectedRowLength = endX;
			}
			while (i < selectedRowLength) {
				character = selectedRow->GetChild(i);
				strCharacter = character->GetContent();
				selectedStr.append(strCharacter);
				i++;
			}

			j++;

			if (j <= endY) {
				selectedStr.append("\n");
				i = 0;
			}
		}
	}


	//--------------------------------------------------------------------------------------------------------



	//1.4. 단어를 바꾼 후, 다음 단어를 찾는다.
	findAndReplace = new FindAndReplace(this);

	findAndReplace->ReplaceSameText((LPCTSTR)this->findStr, (LPCTSTR)this->replaceStr, selectedStr, this->matchCase,
		FALSE,
		currentY, currentX, &this->notePadForm->selectWords->firstY, &this->notePadForm->selectWords->firstX,
		&this->notePadForm->selectWords->endY, &this->notePadForm->selectWords->endX, &this->exception,
		&this->notePadForm->selectWords->isReplaced);

#if 0
	//1.3. 검색을 아랫쪽 방향으로 진행하면, 해당 방향으로 같은 단어가 있는지 확인한다.
	if (this->searchDown == TRUE) {
		this->notePadForm->note->FindSameTextWithDown((LPCTSTR)this->findStr, this->matchCase, currentY, currentX,
			&this->notePadForm->selectWords->firstY, &this->notePadForm->selectWords->firstX,
			&this->notePadForm->selectWords->endY, &this->notePadForm->selectWords->endX);

		//1.3.1. 같은 단어가 없고, 주위에 배치여부가 TRUE이면 위,아래에 따라 조건을 달리하여 같은단어가 있는지 확인한다.
		if (this->searchAround == TRUE && (this->notePadForm->selectWords->firstY <= 0 &&
			this->notePadForm->selectWords->firstX <= 0 && this->notePadForm->selectWords->endY <= 0 &&
			this->notePadForm->selectWords->endX <= 0)) {

			//(0,0)부터 다시 찾다.
			this->notePadForm->note->FindSameTextWithDown((LPCTSTR)this->findStr, this->matchCase, 0, 0,
				&this->notePadForm->selectWords->firstY, &this->notePadForm->selectWords->firstX,
				&this->notePadForm->selectWords->endY, &this->notePadForm->selectWords->endX);
		}
	}
#endif


	//1.5. 자동개행이 눌려져있는 경우,
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		//1.5.1. 펼친 문서를 다시 자동개행한다. (ON)
		this->notePadForm->lineChange->LineChangeButtonClicked();

		//1.5.2. first좌표, end좌표의 자동개행 후의 위치를 구한다.
		this->notePadForm->lineChange->FindCaretAfterLineChange(this->notePadForm->selectWords->firstX,
			this->notePadForm->selectWords->firstY, &this->notePadForm->selectWords->firstX,
			&this->notePadForm->selectWords->firstY);

		this->notePadForm->lineChange->FindCaretAfterLineChange(this->notePadForm->selectWords->endX,
			this->notePadForm->selectWords->endY, &this->notePadForm->selectWords->endX,
			&this->notePadForm->selectWords->endY);
	}

	//1.6. 같은 단어가 있는 경우, end좌표로 캐럿을 이동한다.
	if (this->notePadForm->selectWords->firstX > 0 || this->notePadForm->selectWords->firstY > 0 ||
		this->notePadForm->selectWords->endX > 0 || this->notePadForm->selectWords->endY > 0) {

		this->notePadForm->note->MoveCurrent(this->notePadForm->selectWords->endY);
		row = this->notePadForm->note->GetChild(this->notePadForm->selectWords->endY);
		row->MoveCurrent(this->notePadForm->selectWords->endX);
	}

}













