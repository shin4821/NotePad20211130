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

	//1. ���� �Է¹޴´�.
	//2. windowWidth ���� �۰ų� ����, length���� ���� ���� �ݺ��Ѵ�.
	windowWidth = this->notePadForm->scroll->windowWidth;

	//(21.09.10 �߰�) windowWidth���� 10�� �������ν� �����ְ� �ڵ�����ǵ��� �Ѵ�. *************
	windowWidth = windowWidth - 25;
	//********************************************************************************************

	length = row->GetLength();

	while (width <= windowWidth && index <= length) { //width <= windowWidth  ����<���� =�� �߰�����.
		width = this->notePadForm->getTextSize->totalCalculateX(index, row);
		index++;
	}
	index--;

	if (width > windowWidth) {
		location = index - 1;
	}
#if 0
	//if (index <= length) 
	if (width > windowWidth) { //location�� ÷��ȭ�ϱ�.(ù��° �����ϴ� �κк���)
		if (index <= length) {
			location = index - 1;   //(21.09.10 ����) location = index - 2�� ���־��µ�, ����������� ���϶� �α��ھ� �Ѿ. 
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

	//1. ���� �Է¹޴´�.
	//2. windowWidth ���� �۰ų� ����, length���� ���� ���� �ݺ��Ѵ�.
	windowWidth = this->notePadForm->scroll->windowWidth;
	length = row->GetLength();

	while (width <= windowWidth && index <= length) {
		width = this->notePadForm->getTextSize->totalCalculateX(index, row);
		index++;
	}


	if (width>windowWidth) { //location�� ÷��ȭ�ϱ�.(ù��° �����ϴ� �κк���)
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


	//1.3.2. nextRow�� dummyRow�� ���� �ݺ��Ѵ�.
	i = currentY;
	row = this->notePadForm->note->GetChild(i);
	if (i + 1 < this->notePadForm->note->GetLength()) {
		nextRow = this->notePadForm->note->GetChild(i + 1);
	}
	else {
		nextRow = NULL;
	}
	while (nextRow != NULL && dynamic_cast<DummyRow*>(nextRow)) {
		//1. dummyRow�� length���� ����, width�� ������ �ʺ񺸴� �������� �ݺ��Ѵ�.
		width = this->notePadForm->getTextSize->totalCalculateX(row->GetLength(), row);

		while (0 < nextRow->GetLength() && width <= this->notePadForm->scroll->windowWidth) {
			//1.1. nextRow�� ���ڸ� �д´�.
			character = nextRow->GetChild(0);

			//1.2. row�� �� �ڿ� �߰��Ѵ�.
			row->Add(character);

			//1.3. nextRow���� �����.
			nextRow->Remove(0);

			//1.4. row�� �� �ʺ� ���Ѵ�.
			width = this->notePadForm->getTextSize->totalCalculateX(row->GetLength(), row);
		}
		//2. �� �ʺ� windowWidth�� �ѱ� ���,
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

	//2.0. Horizon Scroll�� ���ش�.
	//this->notePadForm->ModifyStyle(WS_HSCROLL, 0);

	//2.1. note�� length��ŭ �ݺ��Ѵ�. (�ڵ������Ѵ�)
	while (i < this->notePadForm->note->GetLength()) {

		//2.1.1. �ش� row �Ǵ� dummyRow�� �ڸ� ��ġ�� ã�´�.
		row = this->notePadForm->note->GetChild(i);
		location = FindSplitLocation(row);

		//2.1.2. �ش� ��ġ�� 0���� ũ�ų� ���� ���� �ݺ��Ѵ�.
		ret = FALSE;
		while (location >= 0) {//�� �ٿ� ���ؼ��� ó��
			//1. dummyRow�� �����Ѵ�.
			dummyRow = new DummyRow;

			//2. note�� i+1��°�� Insert�Ѵ�.
			this->notePadForm->note->Add(i + 1, dummyRow);

			//3. Split�� ȣ���Ѵ�.
			dummyRow = row->Split(dummyRow, location);

			//4. ��¹��� dummyRow���� �ڸ� ��ġ�� �ִ��� ã�´�.
			location = FindSplitLocation(dummyRow);
			row = dummyRow;
			i++;
			ret = TRUE;
		}

		//2.1.3. �ش� ��ġ�� 0���� ������ �� ���� row �Ǵ� dummyRow�� ã�´�.
		if (ret == FALSE) { //���� ó������ location�� -1�̸�, �� ���� row�� �Ѿ�� �ϴϱ�
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

	//0. Horizon Scroll�� �ٽ� �����.
	this->notePadForm->ModifyStyle(0, WS_HSCROLL); //WS_VSCROLL | 

	row = this->notePadForm->note->GetChild(i);

	//1. note�� length��ŭ �ݺ��Ѵ�.
	while (row != NULL && i < this->notePadForm->note->GetLength() - 1) {
		//1.1. ���� row�� ���� ���� ���Ѵ�.
		nextRow = this->notePadForm->note->GetChild(i + 1);

		//1.2. �������� dummyRow�� ���� �ݺ��Ѵ�.
		while (nextRow != NULL && dynamic_cast<DummyRow*>(nextRow)) {

			//1.2.1. ���� row���� Combine�� ȣ���Ѵ�.
			row->Combine(nextRow);

			//1.2.2. �� �ű� nextRow�� note���� �����Ѵ�.
			this->notePadForm->note->Remove(i + 1);

			//1.2.3. �� ���� row�� ���Ѵ�.
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
	//(21.11.03.�߰�)�������� "" �׳� �̷� ���� �������� �������ڰ�.
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

	//1. �ڵ����� ���� ���� ĳ���� ��ġ�� ã�´�.(�ڵ����� �ȵ����� ���� ����)
	(*currentY) = this->notePadForm->note->GetCurrent();
	row = this->notePadForm->note->GetChild((*currentY));
	currentX = row->GetCurrent();
	(*beforeLineChangeCurrentX) = currentX;

	//2. ���� ĳ���� DummyRow�� �ִ��� Row�� �ִ��� Ȯ���Ѵ�. (�ڵ����� �� ĳ���� X��ǥ)

#if 0
	//(21.11.03.�߰�) ���� ���� ĳ���� Row�� �ִµ�, �� Row�� ������ Row�̰� beforeLineChangeCurrentX�� 0�ΰ�� ����.
	//int noteLength = this->notePadForm->note->GetLength();
	//&& (*currentY) == noteLength + 1 &&
	int noteLength = this->notePadForm->note->GetLength();
	if (dynamic_cast<Row*>(row) && (*beforeLineChangeCurrentX) == 0 && (*currentY) == noteLength && (*currentY)>0) {//(*currentY) == noteLength - 1 &&

		isDummySame = TRUE;
	}
	//*******************************************************************************************************************
#endif

	while (row != NULL && dynamic_cast<DummyRow*>(row)) { //||isDummySame == TRUE
		//2.1. DummyRow�̸� �� �� row�� length�� ���Ѵ�.
		previousRow = this->notePadForm->note->GetChild((*currentY) - 1);
		previousLength = previousRow->GetLength();

		//2.2. �ڵ����� �� X�� ����length�� ���Ѵ�.
		(*beforeLineChangeCurrentX) = (*beforeLineChangeCurrentX) + previousLength;

		//2.3. �ڵ����� �� Y�� 1�� ����. 
		(*currentY)--;

		if ((*currentY) > 0) {
			row = this->notePadForm->note->GetChild((*currentY));
		}
		else {
			row = NULL;
		}
	}

	//3. �ڵ����� �� ĳ���� Y��ǥ�� ���Ѵ�.
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

void LineChange::FindCaretBeforeLineChangeForFirstXY(int* beforeLineChangeCurrentX, int* currentY) {//�̺�Ʈ��
	int i = 0;
	int j;
	Glyph* row = NULL;
	Glyph* nextRow;
	int currentX;
	Glyph* previousRow;
	int previousLength;
	int rowLength;
	BOOL isDummySame = FALSE;

	//1. �ڵ����� ���� ���� ĳ���� ��ġ�� ã�´�.(�ڵ����� �ȵ����� ���� ����)
	(*currentY) = this->notePadForm->selectWords->firstY;
	row = this->notePadForm->note->GetChild((*currentY));
	currentX = row->GetCurrent();
	(*beforeLineChangeCurrentX) = this->notePadForm->selectWords->firstX;

	//2. ���� ĳ���� DummyRow�� �ִ��� Row�� �ִ��� Ȯ���Ѵ�. (�ڵ����� �� ĳ���� X��ǥ)
	row = this->notePadForm->note->GetChild((*currentY));

#if 0
		//(21.11.03.�߰�) ���� ���� ĳ���� Row�� �ִµ�, �� Row�� ������ Row�̰� beforeLineChangeCurrentX�� 0�ΰ�� ����.
		//&& noteLength>1 && (*currentY)>0
	int noteLength = this->notePadForm->note->GetLength();
	if (dynamic_cast<Row*>(row) && (*beforeLineChangeCurrentX) == 0 && (*currentY) == noteLength + 1 && (*currentY) > 0) {//(*currentY) == noteLength - 1 &&

		//�ƴϸ� �׳� �� ROW�� ���ְ� Dummyrow�� ���� �����
		isDummySame = TRUE;
	}
	//*******************************************************************************************************************
#endif

	while ((row != NULL && dynamic_cast<DummyRow*>(row)) || isDummySame == TRUE) { //||isDummySame == TRUE
		//2.1. DummyRow�̸� �� �� row�� length�� ���Ѵ�.
		previousRow = this->notePadForm->note->GetChild((*currentY) - 1);
		previousLength = previousRow->GetLength();

		//2.2. �ڵ����� �� X�� ����length�� ���Ѵ�.
		(*beforeLineChangeCurrentX) = (*beforeLineChangeCurrentX) + previousLength;

		//2.3. �ڵ����� �� Y�� 1�� ����. 
		(*currentY)--;

		if ((*currentY) > 0) {
			row = this->notePadForm->note->GetChild((*currentY));
		}
		else {
			row = NULL;
		}

		isDummySame = FALSE;
	}

	//3. �ڵ����� �� ĳ���� Y��ǥ�� ���Ѵ�.
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

void LineChange::FindCaretBeforeLineChangeForEndXY(int* beforeLineChangeCurrentX, int* currentY) {//�̺�Ʈ��
	int i = 0;
	int j;
	Glyph* row = NULL;
	Glyph* nextRow;
	int currentX;
	Glyph* previousRow;
	int previousLength;
	int rowLength;
	BOOL isDummySame = FALSE;

	//1. �ڵ����� ���� ���� ĳ���� ��ġ�� ã�´�.(�ڵ����� �ȵ����� ���� ����)
	(*currentY) = this->notePadForm->selectWords->endY;
	row = this->notePadForm->note->GetChild((*currentY));
	currentX = row->GetCurrent();
	(*beforeLineChangeCurrentX) = this->notePadForm->selectWords->endX;

	//2. ���� ĳ���� DummyRow�� �ִ��� Row�� �ִ��� Ȯ���Ѵ�. (�ڵ����� �� ĳ���� X��ǥ)
	row = this->notePadForm->note->GetChild((*currentY));

#if 0
		//(21.11.03.�߰�) ���� ���� ĳ���� Row�� �ִµ�, �� Row�� ������ Row�̰� beforeLineChangeCurrentX�� 0�ΰ�� ����.
		//&& noteLength>1 && (*currentY)>0
	int noteLength = this->notePadForm->note->GetLength();
	if (dynamic_cast<Row*>(row) && (*beforeLineChangeCurrentX) == 0 && (*currentY) == noteLength + 1 && (*currentY) > 0) {//(*currentY) == noteLength - 1 &&

		//�ƴϸ� �׳� �� ROW�� ���ְ� Dummyrow�� ���� �����
		isDummySame = TRUE;
	}
	//*******************************************************************************************************************
#endif

	while ((row != NULL && dynamic_cast<DummyRow*>(row)) || isDummySame == TRUE) { //||isDummySame == TRUE
		//2.1. DummyRow�̸� �� �� row�� length�� ���Ѵ�.
		previousRow = this->notePadForm->note->GetChild((*currentY) - 1);
		previousLength = previousRow->GetLength();

		//2.2. �ڵ����� �� X�� ����length�� ���Ѵ�.
		(*beforeLineChangeCurrentX) = (*beforeLineChangeCurrentX) + previousLength;

		//2.3. �ڵ����� �� Y�� 1�� ����. 
		(*currentY)--;

		if ((*currentY) > 0) {
			row = this->notePadForm->note->GetChild((*currentY));
		}
		else {
			row = NULL;
		}

		isDummySame = FALSE;
	}

	//3. �ڵ����� �� ĳ���� Y��ǥ�� ���Ѵ�.
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

	//1. �ڵ����� ���� ���� ĳ���� ��ġ�� ã�´�.(�ڵ����� �ȵ����� ���� ����)
	(*currentY) = firstY;
	row = this->notePadForm->note->GetChild(firstY);
	//currentX = row->GetCurrent();
	(*beforeLineChangeCurrentX) = firstX;

	//2. ���� ĳ���� DummyRow�� �ִ��� Row�� �ִ��� Ȯ���Ѵ�. (�ڵ����� �� ĳ���� X��ǥ)

#if 0
	//(21.11.03.�߰�) ���� ���� ĳ���� Row�� �ִµ�, �� Row�� ������ Row�̰� beforeLineChangeCurrentX�� 0�ΰ�� ����.
	//&& noteLength>1 && firstY>0
	int noteLength = this->notePadForm->note->GetLength();
	if (dynamic_cast<Row*>(row) && (*beforeLineChangeCurrentX) == 0 && (*currentY) == noteLength && (*currentY) > 0) { 
		//firstY == noteLength - 1 &&

		//�ƴϸ� �׳� �� ROW�� ���ְ� Dummyrow�� ���� �����
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
    //2.1. DummyRow�̸� �� �� row�� length�� ���Ѵ�.
		previousRow = this->notePadForm->note->GetChild((*currentY) - 1);
		previousLength = previousRow->GetLength();

		//2.2. �ڵ����� �� X�� ����length�� ���Ѵ�.
		(*beforeLineChangeCurrentX) = (*beforeLineChangeCurrentX) + previousLength;

		//2.3. �ڵ����� �� Y�� 1�� ����. 
		(*currentY)--;

		if ((*currentY) > 0) {
			row = this->notePadForm->note->GetChild((*currentY));
		}
		else {
			row = NULL;
		}

	}



	//3. �ڵ����� �� ĳ���� Y��ǥ�� ���Ѵ�.
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

//0. �ڵ����� �� ĳ���� ��ġ�� ����Ǿ�����,
//1. currentY��° "Row"�� ã�´�.
	i = 0;
	j = 0;
	row = this->notePadForm->note->GetChild(j);
	while (row != NULL && i < currentY + 1) {

		//DummyRow�� �ƴϸ�, �� Row�̸�
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

	//2. currentX�� beforeLineChangeCurrentX���� ���� ������ �ݺ��Ѵ�.
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
		//(21.10.01 �߰�) �ڵ����� �� X��ǥ�� 0�� ���,
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

	// ���� Ŀ���� X��ǥ�� 0�� ���, ��ġ�� �ٲ��ָ� �ȵȴ�.
	if ((currentX != 0 && beforeLineChangeCurrentX != 0) && currentX >= beforeLineChangeCurrentX) {
		currentY--;
	}
	else if (currentX == 0 && beforeLineChangeCurrentX == 0) {
		i = 0;
	}


	//2.3. (21.08.23 �߰� 3) �ش� ��ġ�� �̵��Ѵ�.*****************************************************
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

	//0. �ڵ����� �� ĳ���� ��ġ�� ����Ǿ�����,
	//1. currentY��° "Row"�� ã�´�.
	i = 0;
	j = 0;
	row = this->notePadForm->note->GetChild(j);
	while (row != NULL && i < currentY + 1) {

		//DummyRow�� �ƴϸ�, �� Row�̸�
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

	//2. currentX�� beforeLineChangeCurrentX���� ���� ������ �ݺ��Ѵ�.
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
		//(21.10.01 �߰�) �ڵ����� �� X��ǥ�� 0�� ���,
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

	// ���� Ŀ���� X��ǥ�� 0�� ���, ��ġ�� �ٲ��ָ� �ȵȴ�.
	if ((currentX != 0 && beforeLineChangeCurrentX != 0) && currentX >= beforeLineChangeCurrentX) {
		currentY--;
	}
	else if (currentX == 0 && beforeLineChangeCurrentX == 0) {
		i = 0;
	}


	//2.3. (21.08.23 �߰� 3) �ش� ��ġ�� �̵��Ѵ�.*****************************************************
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

	//0. �ڵ����� �� ĳ���� ��ġ�� ����Ǿ�����,
	//1. currentY��° "Row"�� ã�´�.
	i = 0;
	j = 0;
	row = this->notePadForm->note->GetChild(j);
	while (row != NULL && i < currentY + 1) {

		//DummyRow�� �ƴϸ�, �� Row�̸�
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

	//2. currentX�� beforeLineChangeCurrentX���� ���� ������ �ݺ��Ѵ�.
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

	// ���� Ŀ���� X��ǥ�� 0�� ���, ��ġ�� �ٲ��ָ� �ȵȴ�.
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


	//1.1.1. note�� length��ŭ �ݺ��Ѵ�.
	while (i < this->notePadForm->note->GetLength()) {
		//1. �ش� row �Ǵ� dummyRow�� �ڸ� ��ġ�� ã�´�.
		row = this->notePadForm->note->GetChild(i);
		location = FindSplitLocation(row);

		//2. �ش� ��ġ�� 0���� ũ�ų� ���� ���� �ݺ��Ѵ�.
		while (location >= 0) {
			//2.1. i+1��°�� dummyRow�� �ƴϰų�, i+1��°�� null�̸�,
			if (i + 1 < this->notePadForm->note->GetLength()) { //row�ڿ� row�� dummyRow�� ���𰡰� �������,
				dummyRow = this->notePadForm->note->GetChild(i + 1);

				if (!dynamic_cast<DummyRow*>(dummyRow)) {
					//2.1.1. dummyRow�� �����Ѵ�.
					dummyRow = new DummyRow();

					//2.1.2. note�� i+1��°�� �߰��Ѵ�.
					this->notePadForm->note->Add(i + 1, dummyRow);
				}
			}
			else {
				dummyRow = new DummyRow();
				this->notePadForm->note->Add(dummyRow);
			}

			//2.2. Split�� ȣ���Ѵ�. (�տ� �߰��ǵ���)
			dummyRow = row->SplitForFront(dummyRow, location);

			//2.3. ��¹��� dummyRow���� �ڸ� ��ġ�� �ִ��� ã�´�.
			location = this->notePadForm->lineChange->FindSplitLocation(dummyRow);
			row = dummyRow;
			i++;
		}
		//3. �ش� ��ġ�� 0���� ������, �� ���� row �Ǵ� dummyRow�� ã�´�.
		i++;
	}
	//1.1.2.  beforeWindowWidth�� �����Ѵ�.
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


	//1.2.1. note�� length��ŭ �ݺ��Ѵ�.
	i = 0;
	while (i < this->notePadForm->note->GetLength()) {
		//1. �ش� row�� nextRow�� ���Ѵ�.
		if (ret == TRUE) {
			row = this->notePadForm->note->GetChild(i);
		}
		if (i + 1 < this->notePadForm->note->GetLength()) {
			nextRow = this->notePadForm->note->GetChild(i + 1);
		}
		else {
			nextRow = NULL;
		}

		//2. nextRow�� NULL�� �ƴϰ� dummyRow�� ���,
		if (nextRow != NULL && dynamic_cast<DummyRow*>(nextRow)) {

			//2.1. dummyRow�� length���� ����, width�� windowWidth���� �������� �ݺ��Ѵ�.
			width = this->notePadForm->getTextSize->totalCalculateX(row->GetLength(), row);

			while (0 < nextRow->GetLength() && width <= windowWidth) {
				//2.1.1. nextRow�� ���ڸ� �д´�.
				character = nextRow->GetChild(0); //������ ù��° ����

				//2.1.2. row�� �� �ڿ� �߰��Ѵ�.
				row->Add(character);

				//2.1.3. nextRow���� �����.
				nextRow->Remove(0);

				//2.1.4. row�� �� �ʺ� ���Ѵ�.
				width = this->notePadForm->getTextSize->totalCalculateX(row->GetLength(), row);

				ret_ = TRUE;
			}

			//2.2. �� �ʺ� windowWidth�� �Ѵ� ���,
			if (width > windowWidth) {

				//(21.11.02.�߰�) while�� ������� �ʾ����� character �����ش�.
				if (ret_ == FALSE) {
					//2.1.1. nextRow�� ���ڸ� �д´�.
					character = nextRow->GetChild(0); //������ ù��° ����
				}

				//2.2.1. nextRow�� �ٽ� ���Ѵ�. (���� �տ�)
				nextRow->Add(0, character);

				//2.2.2. row���� �����. (�� �ڿ�)
				row->Remove(row->GetLength() - 1);

				ret = TRUE;
				i++;
			}

			//2.3. nextRow�� 0������ ���� width�� ���� ���,
			else if (nextRow->GetLength() <= 0 && width <= windowWidth) {
				//2.3.1. nextRow�� �����.
				this->notePadForm->note->Remove(i + 1);
				ret = FALSE;
			}

		}

		//3. nextRow�� NULL�̰ų� row�� ���,
		else {
			ret = TRUE;
			i++;
		}
	}
	//1.2.2.  beforeWindowWidth�� �����Ѵ�.
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

	//2.0. Horizon Scroll�� ���ش�.
	//this->notePadForm->ModifyStyle(WS_HSCROLL, 0);

	//2.1. note�� length��ŭ �ݺ��Ѵ�. (�ڵ������Ѵ�)
	while (i < this->notePadForm->note->GetLength()) {

		//2.1.1. �ش� row �Ǵ� dummyRow�� �ڸ� ��ġ�� ã�´�.
		row = this->notePadForm->note->GetChild(i);
		location = FindSplitLocation(row, width);

		//2.1.2. �ش� ��ġ�� 0���� ũ�ų� ���� ���� �ݺ��Ѵ�.
		ret = FALSE;
		while (location >= 0) {//�� �ٿ� ���ؼ��� ó��
			//1. dummyRow�� �����Ѵ�.
			dummyRow = new DummyRow;

			//2. note�� i+1��°�� Insert�Ѵ�.
			this->notePadForm->note->Add(i + 1, dummyRow);

			//3. Split�� ȣ���Ѵ�.
			dummyRow = row->Split(dummyRow, location);

			//4. ��¹��� dummyRow���� �ڸ� ��ġ�� �ִ��� ã�´�.
			location = FindSplitLocation(dummyRow, width);
			row = dummyRow;
			i++;
			ret = TRUE;
		}

		//2.1.3. �ش� ��ġ�� 0���� ������ �� ���� row �Ǵ� dummyRow�� ã�´�.
		if (ret == FALSE) { //���� ó������ location�� -1�̸�, �� ���� row�� �Ѿ�� �ϴϱ�
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

	//1. ���� �Է¹޴´�.
	//2. windowWidth ���� �۰ų� ����, length���� ���� ���� �ݺ��Ѵ�.
	windowWidth = width;

//#if 0
	//(21.09.10 �߰�) windowWidth���� 10�� �������ν� �����ְ� �ڵ�����ǵ��� �Ѵ�. *************
	windowWidth = windowWidth - 25;
	//********************************************************************************************
//#endif

	length = row->GetLength();

	while (width_ <= windowWidth && index <= length) { //width <= windowWidth
		width_ = this->notePadForm->getTextSize->totalCalculateX(index, row);
		index++;
	}

	index--;
	if (width_ > windowWidth) { //location�� ÷��ȭ�ϱ�.(ù��° �����ϴ� �κк���)
		location = index - 1;   //(21.09.10 ����) location = index - 2�� ���־��µ�, ����������� ���϶� �α��ھ� �Ѿ. 
	}

#if 0
	if (index <= length) { //location�� ÷��ȭ�ϱ�.(ù��° �����ϴ� �κк���)
		location = index - 1;   //(21.09.10 ����) location = index - 2�� ���־��µ�, ����������� ���϶� �α��ھ� �Ѿ. 
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

	//1. ���� �Է¹޴´�.
	//2. windowWidth ���� �۰ų� ����, length���� ���� ���� �ݺ��Ѵ�.
	windowWidth = width;


	//windowWidth���� 10�� �������ν� �����ְ� �ڵ�����ǵ��� �Ѵ�. *************
	windowWidth = windowWidth - 25;
	//***************************************************************************


	length = row->GetLength();

	while (width_ <= windowWidth && index <= length) { //width <= windowWidth
		width_ = this->notePadForm->getTextSize->totalCalculateX(index, row);
		index++;
	}

	index--;
	if (width_ > windowWidth) { //location�� ÷��ȭ�ϱ�.(ù��° �����ϴ� �κк���)
		location = index - 1;   //(21.09.10 ����) location = index - 2�� ���־��µ�, ����������� ���϶� �α��ھ� �Ѿ. 
	}

#if 0
	if (index <= length) { //location�� ÷��ȭ�ϱ�.(ù��° �����ϴ� �κк���)
		location = index - 1;   //(21.09.10 ����) location = index - 2�� ���־��µ�, ����������� ���϶� �α��ھ� �Ѿ. 
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

	//2.0. Horizon Scroll�� ���ش�.
	//this->notePadForm->ModifyStyle(WS_HSCROLL, 0);

	//2.1. note�� length��ŭ �ݺ��Ѵ�. (�ڵ������Ѵ�)
	while (i < this->notePadForm->note->GetLength()) {

		//2.1.1. �ش� row �Ǵ� dummyRow�� �ڸ� ��ġ�� ã�´�.
		row = this->notePadForm->note->GetChild(i);
		location = FindSplitLocationForDpi(row, width);

		//2.1.2. �ش� ��ġ�� 0���� ũ�ų� ���� ���� �ݺ��Ѵ�.
		ret = FALSE;
		while (location >= 0) {//�� �ٿ� ���ؼ��� ó��
			//1. dummyRow�� �����Ѵ�.
			dummyRow = new DummyRow;

			//2. note�� i+1��°�� Insert�Ѵ�.
			this->notePadForm->note->Add(i + 1, dummyRow);

			//3. Split�� ȣ���Ѵ�.
			dummyRow = row->Split(dummyRow, location);

			//4. ��¹��� dummyRow���� �ڸ� ��ġ�� �ִ��� ã�´�.
			location = FindSplitLocationForDpi(dummyRow, width);
			row = dummyRow;
			i++;
			ret = TRUE;
		}

		//2.1.3. �ش� ��ġ�� 0���� ������ �� ���� row �Ǵ� dummyRow�� ã�´�.
		if (ret == FALSE) { //���� ó������ location�� -1�̸�, �� ���� row�� �Ѿ�� �ϴϱ�
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

	//1. ���� �Է¹޴´�.
	//2. windowWidth ���� �۰ų� ����, length���� ���� ���� �ݺ��Ѵ�.
	windowWidth = width;

	//(21.09.10 �߰�) windowWidth���� 10�� �������ν� �����ְ� �ڵ�����ǵ��� �Ѵ�. *************
	//windowWidth = windowWidth - 25;
	//********************************************************************************************

	length = row->GetLength();

	while (width_ <= windowWidth && index <= length) { //width <= windowWidth
		width_ = this->notePadForm->getTextSizeForDpi->totalCalculateX(index, row);
		index++;
	}

	index--;
	if (width_ > windowWidth) { //location�� ÷��ȭ�ϱ�.(ù��° �����ϴ� �κк���)
		location = index - 1;   //(21.09.10 ����) location = index - 2�� ���־��µ�, ����������� ���϶� �α��ھ� �Ѿ. 
	}


	return location;

}