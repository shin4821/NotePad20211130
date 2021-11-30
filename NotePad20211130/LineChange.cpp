//LineChange.cpp
#include"LineChange.h"
#include"NotePadForm.h"
#include"Glyph.h"
#include"Composite.h"
#include"Character.h"
#include"Note.h"
#include"Row.h"
#include"SingleByteCharacter.h"
#include"DoubleByteCharacter.h"
#include"Scroll.h"
#include"GetTextSize.h"
#include"SelectWords.h"
#include"DummyRow.h"
#include"GetTextSizeForDpi.h"

LineChange::LineChange(NotePadForm* notePadForm) {
	this->notePadForm = notePadForm;
}
LineChange::~LineChange() {
}
int LineChange::FindSplitLocation(Glyph* row) {
	int index = 1;
	int windowWidth;
	int width = 0;
	int length;
	int location = -1;

	//1. 줄을 입력받는다.
	//2. windowWidth 보다 작거나 같고, length보다 작은 동안 반복한다.
	windowWidth = this->notePadForm->scroll->windowWidth;

	//(21.09.10 추가) windowWidth에서 10을 빼줌으로써 여유있게 자동개행되도록 한다. *************
	windowWidth = windowWidth - 25;
	//********************************************************************************************

	length = row->GetLength();

	while (width <= windowWidth && index <= length) { //width <= windowWidth  현재<에서 =를 추가해줌.
		width = this->notePadForm->getTextSize->totalCalculateX(index, row);
		index++;
	}
	index--;

	if (width > windowWidth) {
		location = index - 1;
	}
#if 0
	//if (index <= length) 
	if (width > windowWidth) { //location을 첨자화하기.(첫번째 빼야하는 부분부터)
		if (index <= length) {
			location = index - 1;   //(21.09.10 수정) location = index - 2로 되있었는데, 윈도우사이즈 줄일때 두글자씩 넘어감. 
		}
		else {
			index--;
			location = index - 1;	
		}
	}
#endif

	return location;
}

int LineChange::FindSplitLocationByWriting(Glyph* row) {
	int index = 1;
	int windowWidth;
	int width = 0;
	int length;
	int location = -1;

	//1. 줄을 입력받는다.
	//2. windowWidth 보다 작거나 같고, length보다 작은 동안 반복한다.
	windowWidth = this->notePadForm->scroll->windowWidth;
	length = row->GetLength();

	while (width <= windowWidth && index <= length) {
		width = this->notePadForm->getTextSize->totalCalculateX(index, row);
		index++;
	}


	if (width>windowWidth) { //location을 첨자화하기.(첫번째 빼야하는 부분부터)
		location = index - 2;
	}

	return location;


}

void LineChange::FillRestDummyRow(int currentY) {
	int i;
	Glyph* row;
	Glyph* nextRow;
	Glyph* character = NULL;
	int width;


	//1.3.2. nextRow가 dummyRow인 동안 반복한다.
	i = currentY;
	row = this->notePadForm->note->GetChild(i);
	if (i + 1 < this->notePadForm->note->GetLength()) {
		nextRow = this->notePadForm->note->GetChild(i + 1);
	}
	else {
		nextRow = NULL;
	}
	while (nextRow != NULL && dynamic_cast<DummyRow*>(nextRow)) {
		//1. dummyRow의 length보다 적고, width가 윈도우 너비보다 작은동안 반복한다.
		width = this->notePadForm->getTextSize->totalCalculateX(row->GetLength(), row);

		while (0 < nextRow->GetLength() && width <= this->notePadForm->scroll->windowWidth) {
			//1.1. nextRow의 글자를 읽는다.
			character = nextRow->GetChild(0);

			//1.2. row의 맨 뒤에 추가한다.
			row->Add(character);

			//1.3. nextRow에서 지운다.
			nextRow->Remove(0);

			//1.4. row의 총 너비를 구한다.
			width = this->notePadForm->getTextSize->totalCalculateX(row->GetLength(), row);
		}
		//2. 총 너비가 windowWidth를 넘긴 경우,
		if (width > this->notePadForm->scroll->windowWidth) {
			nextRow->Add(0, character);
			row->Remove(row->GetLength() - 1);
			row = nextRow;
			i++;
		}
		else if (nextRow->GetLength() <= 0 && width <= this->notePadForm->scroll->windowWidth) {
			this->notePadForm->note->Remove(i + 1);
		}

		//i++;

		if (i + 1 < this->notePadForm->note->GetLength()) {
			nextRow = this->notePadForm->note->GetChild(i + 1);
		}
		else {
			nextRow = NULL;
		}

	}
}

void LineChange::LineChangeButtonClicked() {
	int i = 0;
	int j;
	Glyph* row;
	Glyph* dummyRow;
	Glyph* nextRow;
	int location;
	BOOL ret;

	//2.0. Horizon Scroll을 없앤다.
	//this->notePadForm->ModifyStyle(WS_HSCROLL, 0);

	//2.1. note의 length만큼 반복한다. (자동개행한다)
	while (i < this->notePadForm->note->GetLength()) {

		//2.1.1. 해당 row 또는 dummyRow의 자를 위치를 찾는다.
		row = this->notePadForm->note->GetChild(i);
		location = FindSplitLocation(row);

		//2.1.2. 해당 위치가 0보다 크거나 같은 동안 반복한다.
		ret = FALSE;
		while (location >= 0) {//한 줄에 대해서만 처리
			//1. dummyRow를 생성한다.
			dummyRow = new DummyRow;

			//2. note의 i+1번째에 Insert한다.
			this->notePadForm->note->Add(i + 1, dummyRow);

			//3. Split을 호출한다.
			dummyRow = row->Split(dummyRow, location);

			//4. 출력받은 dummyRow에서 자를 위치가 있는지 찾는다.
			location = FindSplitLocation(dummyRow);
			row = dummyRow;
			i++;
			ret = TRUE;
		}

		//2.1.3. 해당 위치가 0보다 작으면 그 다음 row 또는 dummyRow를 찾는다.
		if (ret == FALSE) { //만약 처음부터 location이 -1이면, 그 다음 row로 넘어가야 하니까
			i++;
		}
	}

}

void LineChange::LineChangeButtonNotClicked() {
	int i = 0;
	int j;
	Glyph* row;
	Glyph* dummyRow;
	Glyph* nextRow;
	int location;
	BOOL ret;

	//0. Horizon Scroll을 다시 만든다.
	this->notePadForm->ModifyStyle(0, WS_HSCROLL); //WS_VSCROLL | 

	row = this->notePadForm->note->GetChild(i);

	//1. note의 length만큼 반복한다.
	while (row != NULL && i < this->notePadForm->note->GetLength() - 1) {
		//1.1. 현재 row의 다음 줄을 구한다.
		nextRow = this->notePadForm->note->GetChild(i + 1);

		//1.2. 다음줄이 dummyRow인 동안 반복한다.
		while (nextRow != NULL && dynamic_cast<DummyRow*>(nextRow)) {

			//1.2.1. 현재 row에서 Combine을 호출한다.
			row->Combine(nextRow);

			//1.2.2. 다 옮긴 nextRow를 note에서 삭제한다.
			this->notePadForm->note->Remove(i + 1);

			//1.2.3. 그 다음 row를 구한다.
			if (i + 1 < this->notePadForm->note->GetLength()) {
				nextRow = this->notePadForm->note->GetChild(i + 1);
			}
			else {
				nextRow = NULL;
			}
		}
		row = nextRow;
		i++;
	}

#if 0
	//(21.11.03.추가)마지막에 "" 그냥 이런 줄이 있을때는 없애주자고.
	if (row != NULL && row->GetContent() == "") {
		this->notePadForm->note->Remove(i + 1);
	}
#endif



}

void LineChange::FindCaretBeforeLineChange(int* beforeLineChangeCurrentX, int* currentY) {
	int i = 0;
	int j;
	Glyph* row = NULL;
	Glyph* nextRow;
	int currentX;
	Glyph* previousRow;
	int previousLength;
	int rowLength;
	//BOOL isDummySame = FALSE;

	//1. 자동개행 후의 현재 캐럿의 위치를 찾는다.(자동개행 안되있을 수도 있음)
	(*currentY) = this->notePadForm->note->GetCurrent();
	row = this->notePadForm->note->GetChild((*currentY));
	currentX = row->GetCurrent();
	(*beforeLineChangeCurrentX) = currentX;

	//2. 현재 캐럿이 DummyRow에 있는지 Row에 있는지 확인한다. (자동개행 전 캐럿의 X좌표)

#if 0
	//(21.11.03.추가) 만약 현재 캐럿이 Row에 있는데, 그 Row가 마지막 Row이고 beforeLineChangeCurrentX가 0인경우 들어간다.
	//int noteLength = this->notePadForm->note->GetLength();
	//&& (*currentY) == noteLength + 1 &&
	int noteLength = this->notePadForm->note->GetLength();
	if (dynamic_cast<Row*>(row) && (*beforeLineChangeCurrentX) == 0 && (*currentY) == noteLength && (*currentY)>0) {//(*currentY) == noteLength - 1 &&

		isDummySame = TRUE;
	}
	//*******************************************************************************************************************
#endif

	while (row != NULL && dynamic_cast<DummyRow*>(row)) { //||isDummySame == TRUE
		//2.1. DummyRow이면 그 전 row의 length를 구한다.
		previousRow = this->notePadForm->note->GetChild((*currentY) - 1);
		previousLength = previousRow->GetLength();

		//2.2. 자동개행 전 X는 기존length에 더한다.
		(*beforeLineChangeCurrentX) = (*beforeLineChangeCurrentX) + previousLength;

		//2.3. 자동개행 전 Y는 1씩 뺀다. 
		(*currentY)--;

		if ((*currentY) > 0) {
			row = this->notePadForm->note->GetChild((*currentY));
		}
		else {
			row = NULL;
		}
	}

	//3. 자동개행 전 캐럿의 Y좌표를 구한다.
	if ((*currentY) - 1 > 0) {

		i = (*currentY) - 1;
		row = this->notePadForm->note->GetChild(i);
		while (i > 0) {

			if (dynamic_cast<DummyRow*>(row)) {
				(*currentY)--;
			}
			i--;
			row = this->notePadForm->note->GetChild(i);
		}
	}


}

void LineChange::FindCaretBeforeLineChangeForFirstXY(int* beforeLineChangeCurrentX, int* currentY) {//이벤트성
	int i = 0;
	int j;
	Glyph* row = NULL;
	Glyph* nextRow;
	int currentX;
	Glyph* previousRow;
	int previousLength;
	int rowLength;
	BOOL isDummySame = FALSE;

	//1. 자동개행 후의 현재 캐럿의 위치를 찾는다.(자동개행 안되있을 수도 있음)
	(*currentY) = this->notePadForm->selectWords->firstY;
	row = this->notePadForm->note->GetChild((*currentY));
	currentX = row->GetCurrent();
	(*beforeLineChangeCurrentX) = this->notePadForm->selectWords->firstX;

	//2. 현재 캐럿이 DummyRow에 있는지 Row에 있는지 확인한다. (자동개행 전 캐럿의 X좌표)
	row = this->notePadForm->note->GetChild((*currentY));

#if 0
		//(21.11.03.추가) 만약 현재 캐럿이 Row에 있는데, 그 Row가 마지막 Row이고 beforeLineChangeCurrentX가 0인경우 들어간다.
		//&& noteLength>1 && (*currentY)>0
	int noteLength = this->notePadForm->note->GetLength();
	if (dynamic_cast<Row*>(row) && (*beforeLineChangeCurrentX) == 0 && (*currentY) == noteLength + 1 && (*currentY) > 0) {//(*currentY) == noteLength - 1 &&

		//아니면 그냥 저 ROW를 없애고 Dummyrow를 만들어서 끼울까
		isDummySame = TRUE;
	}
	//*******************************************************************************************************************
#endif

	while ((row != NULL && dynamic_cast<DummyRow*>(row)) || isDummySame == TRUE) { //||isDummySame == TRUE
		//2.1. DummyRow이면 그 전 row의 length를 구한다.
		previousRow = this->notePadForm->note->GetChild((*currentY) - 1);
		previousLength = previousRow->GetLength();

		//2.2. 자동개행 전 X는 기존length에 더한다.
		(*beforeLineChangeCurrentX) = (*beforeLineChangeCurrentX) + previousLength;

		//2.3. 자동개행 전 Y는 1씩 뺀다. 
		(*currentY)--;

		if ((*currentY) > 0) {
			row = this->notePadForm->note->GetChild((*currentY));
		}
		else {
			row = NULL;
		}

		isDummySame = FALSE;
	}

	//3. 자동개행 전 캐럿의 Y좌표를 구한다.
	if ((*currentY) - 1 > 0) {

		i = (*currentY) - 1;
		row = this->notePadForm->note->GetChild(i);
		while (i > 0) {

			if (dynamic_cast<DummyRow*>(row)) {
				(*currentY)--;
			}
			i--;
			row = this->notePadForm->note->GetChild(i);
		}
	}
}

void LineChange::FindCaretBeforeLineChangeForEndXY(int* beforeLineChangeCurrentX, int* currentY) {//이벤트성
	int i = 0;
	int j;
	Glyph* row = NULL;
	Glyph* nextRow;
	int currentX;
	Glyph* previousRow;
	int previousLength;
	int rowLength;
	BOOL isDummySame = FALSE;

	//1. 자동개행 후의 현재 캐럿의 위치를 찾는다.(자동개행 안되있을 수도 있음)
	(*currentY) = this->notePadForm->selectWords->endY;
	row = this->notePadForm->note->GetChild((*currentY));
	currentX = row->GetCurrent();
	(*beforeLineChangeCurrentX) = this->notePadForm->selectWords->endX;

	//2. 현재 캐럿이 DummyRow에 있는지 Row에 있는지 확인한다. (자동개행 전 캐럿의 X좌표)
	row = this->notePadForm->note->GetChild((*currentY));

#if 0
		//(21.11.03.추가) 만약 현재 캐럿이 Row에 있는데, 그 Row가 마지막 Row이고 beforeLineChangeCurrentX가 0인경우 들어간다.
		//&& noteLength>1 && (*currentY)>0
	int noteLength = this->notePadForm->note->GetLength();
	if (dynamic_cast<Row*>(row) && (*beforeLineChangeCurrentX) == 0 && (*currentY) == noteLength + 1 && (*currentY) > 0) {//(*currentY) == noteLength - 1 &&

		//아니면 그냥 저 ROW를 없애고 Dummyrow를 만들어서 끼울까
		isDummySame = TRUE;
	}
	//*******************************************************************************************************************
#endif

	while ((row != NULL && dynamic_cast<DummyRow*>(row)) || isDummySame == TRUE) { //||isDummySame == TRUE
		//2.1. DummyRow이면 그 전 row의 length를 구한다.
		previousRow = this->notePadForm->note->GetChild((*currentY) - 1);
		previousLength = previousRow->GetLength();

		//2.2. 자동개행 전 X는 기존length에 더한다.
		(*beforeLineChangeCurrentX) = (*beforeLineChangeCurrentX) + previousLength;

		//2.3. 자동개행 전 Y는 1씩 뺀다. 
		(*currentY)--;

		if ((*currentY) > 0) {
			row = this->notePadForm->note->GetChild((*currentY));
		}
		else {
			row = NULL;
		}

		isDummySame = FALSE;
	}

	//3. 자동개행 전 캐럿의 Y좌표를 구한다.
	if ((*currentY) - 1 > 0) {

		i = (*currentY) - 1;
		row = this->notePadForm->note->GetChild(i);
		while (i > 0) {

			if (dynamic_cast<DummyRow*>(row)) {
				(*currentY)--;
			}
			i--;
			row = this->notePadForm->note->GetChild(i);
		}
	}
}

void LineChange::FindCaretBeforeLineChange(int firstX, int firstY, int* beforeLineChangeCurrentX, int* currentY) {
	int i = 0;
	int j;
	Glyph* row = NULL;
	Glyph* nextRow;
	int currentX;
	Glyph* previousRow;
	int previousLength;
	int rowLength;
	BOOL isDummySame = FALSE;
	int noteLength = this->notePadForm->note->GetLength();

	//1. 자동개행 후의 현재 캐럿의 위치를 찾는다.(자동개행 안되있을 수도 있음)
	(*currentY) = firstY;
	row = this->notePadForm->note->GetChild(firstY);
	//currentX = row->GetCurrent();
	(*beforeLineChangeCurrentX) = firstX;

	//2. 현재 캐럿이 DummyRow에 있는지 Row에 있는지 확인한다. (자동개행 전 캐럿의 X좌표)

#if 0
	//(21.11.03.추가) 만약 현재 캐럿이 Row에 있는데, 그 Row가 마지막 Row이고 beforeLineChangeCurrentX가 0인경우 들어간다.
	//&& noteLength>1 && firstY>0
	int noteLength = this->notePadForm->note->GetLength();
	if (dynamic_cast<Row*>(row) && (*beforeLineChangeCurrentX) == 0 && (*currentY) == noteLength && (*currentY) > 0) { 
		//firstY == noteLength - 1 &&

		//아니면 그냥 저 ROW를 없애고 Dummyrow를 만들어서 끼울까
		isDummySame = TRUE;
#if 0
		this->notePadForm->note->Remove((*currentY));
		Glyph* row_ = new DummyRow;
		this->notePadForm->note->Add(row_);
		row = this->notePadForm->note->GetChild((*currentY));
#endif
	}
	//*******************************************************************************************************************
#endif

	while (row != NULL && dynamic_cast<DummyRow*>(row)) {//|| isDummySame == TRUE
    //2.1. DummyRow이면 그 전 row의 length를 구한다.
		previousRow = this->notePadForm->note->GetChild((*currentY) - 1);
		previousLength = previousRow->GetLength();

		//2.2. 자동개행 전 X는 기존length에 더한다.
		(*beforeLineChangeCurrentX) = (*beforeLineChangeCurrentX) + previousLength;

		//2.3. 자동개행 전 Y는 1씩 뺀다. 
		(*currentY)--;

		if ((*currentY) > 0) {
			row = this->notePadForm->note->GetChild((*currentY));
		}
		else {
			row = NULL;
		}

	}



	//3. 자동개행 전 캐럿의 Y좌표를 구한다.
	if ((*currentY) - 1 > 0) {

		i = (*currentY) - 1;
		row = this->notePadForm->note->GetChild(i);
		while (i > 0) {

			if (dynamic_cast<DummyRow*>(row)) {
				(*currentY)--;
			}
			i--;
			row = this->notePadForm->note->GetChild(i);
		}
	}


}

void LineChange::FindCaretAfterLineChange(int beforeLineChangeCurrentX, int currentY) {

	int i = 0;
	int j;
	Glyph* row = NULL;
	Glyph* nextRow;
	int currentX;
	Glyph* previousRow;
	int previousLength;
	int rowLength;
	bool isFirst = true;

//0. 자동개행 후 캐럿의 위치가 변경되었으면,
//1. currentY번째 "Row"를 찾는다.
	i = 0;
	j = 0;
	row = this->notePadForm->note->GetChild(j);
	while (row != NULL && i < currentY + 1) {

		//DummyRow가 아니면, 즉 Row이면
		if (!dynamic_cast<DummyRow*>(row)) {
			i++;
		}
		j++;

		if (j < this->notePadForm->note->GetLength()) {
			row = this->notePadForm->note->GetChild(j);
		}
		else {
			row = NULL;
		}
	}
	j--;

	//2. currentX가 beforeLineChangeCurrentX보다 작을 때까지 반복한다.
	currentY = j;
	row = this->notePadForm->note->GetChild(j);
	rowLength = row->GetLength();
	currentX = 0;

	while (currentX < beforeLineChangeCurrentX) {

		i = 0;
		while (i < rowLength && currentX < beforeLineChangeCurrentX) {
			currentX++;
			i++;
		}
#if 0
		//(21.10.01 추가) 자동개행 후 X좌표가 0인 경우,
		if (isFirst == true && currentX == beforeLineChangeCurrentX && i == rowLength) {
			i = 0;
			currentY++;
		}
#endif

		currentY++;

		if (currentY < this->notePadForm->note->GetLength()) {
			nextRow = this->notePadForm->note->GetChild(currentY);
			rowLength = nextRow->GetLength();
		}
		else {
			nextRow = NULL;
		}

		isFirst = false;
	}

	// 현재 커서의 X좌표가 0인 경우, 위치를 바꿔주면 안된다.
	if ((currentX != 0 && beforeLineChangeCurrentX != 0) && currentX >= beforeLineChangeCurrentX) {
		currentY--;
	}
	else if (currentX == 0 && beforeLineChangeCurrentX == 0) {
		i = 0;
	}


	//2.3. (21.08.23 추가 3) 해당 위치로 이동한다.*****************************************************
	this->notePadForm->note->MoveCurrent(currentY);
	row = this->notePadForm->note->GetChild(currentY);
	row->MoveCurrent(i);
	//*************************************************************************************************
}

void LineChange::FindCaretAfterLineChangeForFirst(int beforeLineChangeCurrentX, int currentY) {
	int i = 0;
	int j;
	Glyph* row = NULL;
	Glyph* nextRow;
	int currentX;
	Glyph* previousRow;
	int previousLength;
	int rowLength;
	bool isFirst = true;

	//0. 자동개행 후 캐럿의 위치가 변경되었으면,
	//1. currentY번째 "Row"를 찾는다.
	i = 0;
	j = 0;
	row = this->notePadForm->note->GetChild(j);
	while (row != NULL && i < currentY + 1) {

		//DummyRow가 아니면, 즉 Row이면
		if (!dynamic_cast<DummyRow*>(row)) {
			i++;
		}
		j++;

		if (j < this->notePadForm->note->GetLength()) {
			row = this->notePadForm->note->GetChild(j);
		}
		else {
			row = NULL;
		}
	}
	j--;

	//2. currentX가 beforeLineChangeCurrentX보다 작을 때까지 반복한다.
	currentY = j;
	row = this->notePadForm->note->GetChild(j);
	rowLength = row->GetLength();
	currentX = 0;

	while (currentX < beforeLineChangeCurrentX) {

		i = 0;
		while (i < rowLength && currentX < beforeLineChangeCurrentX) {
			currentX++;
			i++;
		}

//#if 0
		//(21.10.01 추가) 자동개행 후 X좌표가 0인 경우,
		if (currentX == beforeLineChangeCurrentX && i == rowLength) { //isFirst == true && 
			i = 0;
			currentY++;
		}
//#endif

		currentY++;

		if (currentY < this->notePadForm->note->GetLength()) {
			nextRow = this->notePadForm->note->GetChild(currentY);
			rowLength = nextRow->GetLength();
		}
		else {
			nextRow = NULL;
		}

		isFirst = false;
	}

	// 현재 커서의 X좌표가 0인 경우, 위치를 바꿔주면 안된다.
	if ((currentX != 0 && beforeLineChangeCurrentX != 0) && currentX >= beforeLineChangeCurrentX) {
		currentY--;
	}
	else if (currentX == 0 && beforeLineChangeCurrentX == 0) {
		i = 0;
	}


	//2.3. (21.08.23 추가 3) 해당 위치로 이동한다.*****************************************************
	this->notePadForm->note->MoveCurrent(currentY);
	row = this->notePadForm->note->GetChild(currentY);
	row->MoveCurrent(i);
	//*************************************************************************************************
}

void LineChange::FindCaretAfterLineChange(int beforeLineChangeCurrentX, int currentY, int* afterCurrentX, int* afterCurrentY) {

	int i = 0;
	int j;
	Glyph* row = NULL;
	Glyph* nextRow;
	int currentX;
	Glyph* previousRow;
	int previousLength;
	int rowLength;

	//0. 자동개행 후 캐럿의 위치가 변경되었으면,
	//1. currentY번째 "Row"를 찾는다.
	i = 0;
	j = 0;
	row = this->notePadForm->note->GetChild(j);
	while (row != NULL && i < currentY + 1) {

		//DummyRow가 아니면, 즉 Row이면
		if (!dynamic_cast<DummyRow*>(row)) {
			i++;
		}
		j++;

		if (j < this->notePadForm->note->GetLength()) {
			row = this->notePadForm->note->GetChild(j);
		}
		else {
			row = NULL;
		}
	}
	j--;

	//2. currentX가 beforeLineChangeCurrentX보다 작을 때까지 반복한다.
	currentY = j;
	row = this->notePadForm->note->GetChild(j);
	rowLength = row->GetLength();
	currentX = 0;

	while (currentX < beforeLineChangeCurrentX) {

		i = 0;
		while (i < rowLength && currentX < beforeLineChangeCurrentX) {
			currentX++;
			i++;
		}
		currentY++;

		if (currentY < this->notePadForm->note->GetLength()) {
			nextRow = this->notePadForm->note->GetChild(currentY);
			rowLength = nextRow->GetLength();
		}
		else {
			nextRow = NULL;
		}

	}

	// 현재 커서의 X좌표가 0인 경우, 위치를 바꿔주면 안된다.
	if ((currentX != 0 && beforeLineChangeCurrentX != 0) && currentX >= beforeLineChangeCurrentX) {
		currentY--;
	}
	else if (currentX == 0 && beforeLineChangeCurrentX == 0) {
		i = 0;
	}

	(*afterCurrentY) = currentY;
	(*afterCurrentX) = i;

}

void LineChange::SmallerThanBefore() {
	int i = 0;
	int j;
	Glyph* row = NULL;
	Glyph* dummyRow;
	Glyph* nextRow;
	Glyph* character = NULL;
	int location;
	int width;
	BOOL ret = TRUE;


	//1.1.1. note의 length만큼 반복한다.
	while (i < this->notePadForm->note->GetLength()) {
		//1. 해당 row 또는 dummyRow의 자를 위치를 찾는다.
		row = this->notePadForm->note->GetChild(i);
		location = FindSplitLocation(row);

		//2. 해당 위치가 0보다 크거나 같은 동안 반복한다.
		while (location >= 0) {
			//2.1. i+1번째에 dummyRow가 아니거나, i+1번째가 null이면,
			if (i + 1 < this->notePadForm->note->GetLength()) { //row뒤에 row든 dummyRow든 무언가가 있을경우,
				dummyRow = this->notePadForm->note->GetChild(i + 1);

				if (!dynamic_cast<DummyRow*>(dummyRow)) {
					//2.1.1. dummyRow를 생성한다.
					dummyRow = new DummyRow();

					//2.1.2. note의 i+1번째에 추가한다.
					this->notePadForm->note->Add(i + 1, dummyRow);
				}
			}
			else {
				dummyRow = new DummyRow();
				this->notePadForm->note->Add(dummyRow);
			}

			//2.2. Split을 호출한다. (앞에 추가되도록)
			dummyRow = row->SplitForFront(dummyRow, location);

			//2.3. 출력받은 dummyRow에서 자를 위치가 있는지 찾는다.
			location = this->notePadForm->lineChange->FindSplitLocation(dummyRow);
			row = dummyRow;
			i++;
		}
		//3. 해당 위치가 0보다 작으면, 그 다음 row 또는 dummyRow를 찾는다.
		i++;
	}
	//1.1.2.  beforeWindowWidth를 갱신한다.
	this->notePadForm->scroll->beforeWindowWidth = this->notePadForm->scroll->windowWidth;


}

void LineChange::BiggerThanBefore() {
	int i = 0;
	int j;
	Glyph* row = NULL;
	Glyph* dummyRow;
	Glyph* nextRow;
	Glyph* character = NULL;
	int location;
	int width;
	BOOL ret = TRUE;
	int windowWidth = this->notePadForm->scroll->windowWidth;
	windowWidth = windowWidth - 5;
	//windowWidth = windowWidth - 25;
	BOOL ret_ = FALSE;


	//1.2.1. note의 length만큼 반복한다.
	i = 0;
	while (i < this->notePadForm->note->GetLength()) {
		//1. 해당 row의 nextRow를 구한다.
		if (ret == TRUE) {
			row = this->notePadForm->note->GetChild(i);
		}
		if (i + 1 < this->notePadForm->note->GetLength()) {
			nextRow = this->notePadForm->note->GetChild(i + 1);
		}
		else {
			nextRow = NULL;
		}

		//2. nextRow가 NULL이 아니고 dummyRow인 경우,
		if (nextRow != NULL && dynamic_cast<DummyRow*>(nextRow)) {

			//2.1. dummyRow의 length보다 적고, width가 windowWidth보다 적은동안 반복한다.
			width = this->notePadForm->getTextSize->totalCalculateX(row->GetLength(), row);

			while (0 < nextRow->GetLength() && width <= windowWidth) {
				//2.1.1. nextRow의 글자를 읽는다.
				character = nextRow->GetChild(0); //무조건 첫번째 글자

				//2.1.2. row의 맨 뒤에 추가한다.
				row->Add(character);

				//2.1.3. nextRow에서 지운다.
				nextRow->Remove(0);

				//2.1.4. row의 총 너비를 구한다.
				width = this->notePadForm->getTextSize->totalCalculateX(row->GetLength(), row);

				ret_ = TRUE;
			}

			//2.2. 총 너비가 windowWidth를 넘는 경우,
			if (width > windowWidth) {

				//(21.11.02.추가) while문 통과하지 않았으면 character 구해준다.
				if (ret_ == FALSE) {
					//2.1.1. nextRow의 글자를 읽는다.
					character = nextRow->GetChild(0); //무조건 첫번째 글자
				}

				//2.2.1. nextRow에 다시 더한다. (제일 앞에)
				nextRow->Add(0, character);

				//2.2.2. row에서 지운다. (맨 뒤에)
				row->Remove(row->GetLength() - 1);

				ret = TRUE;
				i++;
			}

			//2.3. nextRow가 0이지만 아직 width가 남는 경우,
			else if (nextRow->GetLength() <= 0 && width <= windowWidth) {
				//2.3.1. nextRow를 지운다.
				this->notePadForm->note->Remove(i + 1);
				ret = FALSE;
			}

		}

		//3. nextRow가 NULL이거나 row인 경우,
		else {
			ret = TRUE;
			i++;
		}
	}
	//1.2.2.  beforeWindowWidth를 갱신한다.
	this->notePadForm->scroll->beforeWindowWidth = this->notePadForm->scroll->windowWidth;
}

void LineChange::LineChangeButtonClicked(int width) {
	int i = 0;
	int j;
	Glyph* row;
	Glyph* dummyRow;
	Glyph* nextRow;
	int location;
	BOOL ret;

	//2.0. Horizon Scroll을 없앤다.
	//this->notePadForm->ModifyStyle(WS_HSCROLL, 0);

	//2.1. note의 length만큼 반복한다. (자동개행한다)
	while (i < this->notePadForm->note->GetLength()) {

		//2.1.1. 해당 row 또는 dummyRow의 자를 위치를 찾는다.
		row = this->notePadForm->note->GetChild(i);
		location = FindSplitLocation(row, width);

		//2.1.2. 해당 위치가 0보다 크거나 같은 동안 반복한다.
		ret = FALSE;
		while (location >= 0) {//한 줄에 대해서만 처리
			//1. dummyRow를 생성한다.
			dummyRow = new DummyRow;

			//2. note의 i+1번째에 Insert한다.
			this->notePadForm->note->Add(i + 1, dummyRow);

			//3. Split을 호출한다.
			dummyRow = row->Split(dummyRow, location);

			//4. 출력받은 dummyRow에서 자를 위치가 있는지 찾는다.
			location = FindSplitLocation(dummyRow, width);
			row = dummyRow;
			i++;
			ret = TRUE;
		}

		//2.1.3. 해당 위치가 0보다 작으면 그 다음 row 또는 dummyRow를 찾는다.
		if (ret == FALSE) { //만약 처음부터 location이 -1이면, 그 다음 row로 넘어가야 하니까
			i++;
		}
	}
}

int LineChange::FindSplitLocation(Glyph* row, int width) {

	int index = 1;
	int windowWidth;
	int width_ = 0;
	int length;
	int location = -1;

	//1. 줄을 입력받는다.
	//2. windowWidth 보다 작거나 같고, length보다 작은 동안 반복한다.
	windowWidth = width;

//#if 0
	//(21.09.10 추가) windowWidth에서 10을 빼줌으로써 여유있게 자동개행되도록 한다. *************
	windowWidth = windowWidth - 25;
	//********************************************************************************************
//#endif

	length = row->GetLength();

	while (width_ <= windowWidth && index <= length) { //width <= windowWidth
		width_ = this->notePadForm->getTextSize->totalCalculateX(index, row);
		index++;
	}

	index--;
	if (width_ > windowWidth) { //location을 첨자화하기.(첫번째 빼야하는 부분부터)
		location = index - 1;   //(21.09.10 수정) location = index - 2로 되있었는데, 윈도우사이즈 줄일때 두글자씩 넘어감. 
	}

#if 0
	if (index <= length) { //location을 첨자화하기.(첫번째 빼야하는 부분부터)
		location = index - 1;   //(21.09.10 수정) location = index - 2로 되있었는데, 윈도우사이즈 줄일때 두글자씩 넘어감. 
	}
#endif

	return location;

}

int LineChange::FindSplitLocationForMargin(Glyph* row, int width) {

	int index = 1;
	int windowWidth;
	int width_ = 0;
	int length;
	int location = -1;

	//1. 줄을 입력받는다.
	//2. windowWidth 보다 작거나 같고, length보다 작은 동안 반복한다.
	windowWidth = width;


	//windowWidth에서 10을 빼줌으로써 여유있게 자동개행되도록 한다. *************
	windowWidth = windowWidth - 25;
	//***************************************************************************


	length = row->GetLength();

	while (width_ <= windowWidth && index <= length) { //width <= windowWidth
		width_ = this->notePadForm->getTextSize->totalCalculateX(index, row);
		index++;
	}

	index--;
	if (width_ > windowWidth) { //location을 첨자화하기.(첫번째 빼야하는 부분부터)
		location = index - 1;   //(21.09.10 수정) location = index - 2로 되있었는데, 윈도우사이즈 줄일때 두글자씩 넘어감. 
	}

#if 0
	if (index <= length) { //location을 첨자화하기.(첫번째 빼야하는 부분부터)
		location = index - 1;   //(21.09.10 수정) location = index - 2로 되있었는데, 윈도우사이즈 줄일때 두글자씩 넘어감. 
	}
#endif

	return location;



}

void LineChange::LineChangeButtonClickedForDpi(int width) {

	int i = 0;
	int j;
	Glyph* row;
	Glyph* dummyRow;
	Glyph* nextRow;
	int location;
	BOOL ret;

	//2.0. Horizon Scroll을 없앤다.
	//this->notePadForm->ModifyStyle(WS_HSCROLL, 0);

	//2.1. note의 length만큼 반복한다. (자동개행한다)
	while (i < this->notePadForm->note->GetLength()) {

		//2.1.1. 해당 row 또는 dummyRow의 자를 위치를 찾는다.
		row = this->notePadForm->note->GetChild(i);
		location = FindSplitLocationForDpi(row, width);

		//2.1.2. 해당 위치가 0보다 크거나 같은 동안 반복한다.
		ret = FALSE;
		while (location >= 0) {//한 줄에 대해서만 처리
			//1. dummyRow를 생성한다.
			dummyRow = new DummyRow;

			//2. note의 i+1번째에 Insert한다.
			this->notePadForm->note->Add(i + 1, dummyRow);

			//3. Split을 호출한다.
			dummyRow = row->Split(dummyRow, location);

			//4. 출력받은 dummyRow에서 자를 위치가 있는지 찾는다.
			location = FindSplitLocationForDpi(dummyRow, width);
			row = dummyRow;
			i++;
			ret = TRUE;
		}

		//2.1.3. 해당 위치가 0보다 작으면 그 다음 row 또는 dummyRow를 찾는다.
		if (ret == FALSE) { //만약 처음부터 location이 -1이면, 그 다음 row로 넘어가야 하니까
			i++;
		}
	}
}

int LineChange::FindSplitLocationForDpi(Glyph* row, int width) {
	int index = 1;
	int windowWidth;
	int width_ = 0;
	int length;
	int location = -1;

	//1. 줄을 입력받는다.
	//2. windowWidth 보다 작거나 같고, length보다 작은 동안 반복한다.
	windowWidth = width;

	//(21.09.10 추가) windowWidth에서 10을 빼줌으로써 여유있게 자동개행되도록 한다. *************
	//windowWidth = windowWidth - 25;
	//********************************************************************************************

	length = row->GetLength();

	while (width_ <= windowWidth && index <= length) { //width <= windowWidth
		width_ = this->notePadForm->getTextSizeForDpi->totalCalculateX(index, row);
		index++;
	}

	index--;
	if (width_ > windowWidth) { //location을 첨자화하기.(첫번째 빼야하는 부분부터)
		location = index - 1;   //(21.09.10 수정) location = index - 2로 되있었는데, 윈도우사이즈 줄일때 두글자씩 넘어감. 
	}


	return location;

}