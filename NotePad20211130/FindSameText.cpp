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

	//1. ã�� ��ư�� Ŭ������ ��
	//1.1. ���� ĳ���� ��ġ�� ã�´�.
	currentY = this->notePadForm->note->GetCurrent();
	row = this->notePadForm->note->GetChild(currentY);
	currentX = row->GetCurrent();


	//����) �˻������� �����̸� ���� ã�Ҵ� �ܾ�� ���� ã���� �ϴ� �ܾ�� ������ firstXY�� currentXY�� �ѱ��.******
    // ** �׷��� ������ ĳ���� endXY���� ��� ���� �ܾ �ӹ�����.
	if (this->searchDown == FALSE && this->notePadForm->findSameText->beforeFindStr != "" &&
		this->notePadForm->findSameText->beforeFindStr == this->notePadForm->findSameText->findStr &&
		(this->notePadForm->selectWords->firstY > 0 || this->notePadForm->selectWords->firstX > 0 ||
			this->notePadForm->selectWords->endY > 0 || this->notePadForm->selectWords->endX > 0)) {

		currentY = this->notePadForm->selectWords->firstY;
		currentX = this->notePadForm->selectWords->firstX;

		exception = TRUE;
	}
	//****************************************************************************************************************



	//1.2. �ڵ������� ������ �ִ� ���,
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		//1.2.0. ������ ��Ȳ����, �ڵ����� �� ĳ���� ��ġ�� ã�´�.
		if (exception == TRUE) {
			this->notePadForm->lineChange->FindCaretBeforeLineChange(this->notePadForm->selectWords->firstX ,
				this->notePadForm->selectWords->firstY , &currentX, &currentY);
		}
		//1.2.1. ���ܰ� �ƴ� ��Ȳ����, �ڵ����� �� ĳ���� ��ġ�� ã�´�.
		else {
			this->notePadForm->lineChange->FindCaretBeforeLineChange(&currentX, &currentY);
		}


		//1.2.2. �ڵ����� �� ������ ��ģ��.(OFF)
		this->notePadForm->lineChange->LineChangeButtonNotClicked();
	}

	//1.3. �˻��� �Ʒ��� �������� �����ϸ�, �ش� �������� ���� �ܾ �ִ��� Ȯ���Ѵ�.
	if (this->searchDown == TRUE) {

		//(21.11.04.�߰�) 
		this->notePadForm->isUp = FALSE; //������ ��ġ�϶� �ǹؿ��� ������ ���� �ݴ�� ����
		this->notePadForm->isDown = FALSE;


		this->notePadForm->note->FindSameTextWithDown((LPCTSTR)this->findStr, this->matchCase, currentY, currentX,
			&this->notePadForm->selectWords->firstY, &this->notePadForm->selectWords->firstX,
			&this->notePadForm->selectWords->endY, &this->notePadForm->selectWords->endX);

		//1.3.1. ���� �ܾ ����, ������ ��ġ���ΰ� TRUE�̸� ��,�Ʒ��� ���� ������ �޸��Ͽ� �����ܾ �ִ��� Ȯ���Ѵ�.
		if (this->searchAround == TRUE && (this->notePadForm->selectWords->firstY <= 0 &&
			this->notePadForm->selectWords->firstX <= 0 && this->notePadForm->selectWords->endY <= 0 &&
			this->notePadForm->selectWords->endX <= 0)) {
			 
			//(21.11.04.�߰�) 
			this->notePadForm->isUp = TRUE; //������ ��ġ�϶� �ǹؿ��� ������ ���� �ݴ�� ����
			this->notePadForm->isDown = FALSE;


			//(0,0)���� �ٽ� ã��.
			this->notePadForm->note->FindSameTextWithDown((LPCTSTR)this->findStr, this->matchCase, 0, 0,
				&this->notePadForm->selectWords->firstY, &this->notePadForm->selectWords->firstX,
				&this->notePadForm->selectWords->endY, &this->notePadForm->selectWords->endX);
		}
	}

	//1.4. �˻��� ���� �������� �����ϸ�, �ش� �������� ���� �ܾ �ִ��� Ȯ���Ѵ�.
	if (this->searchDown == FALSE) {

		//(21.11.04.�߰�) 
		this->notePadForm->isUp = TRUE; //������ ��ġ�϶� �ǹؿ��� ������ ���� �ݴ�� ����
		this->notePadForm->isDown = FALSE;


		this->notePadForm->note->FindSameTextWithUp((LPCTSTR)this->findStr, this->matchCase, currentY, currentX,
			&this->notePadForm->selectWords->firstY, &this->notePadForm->selectWords->firstX,
			&this->notePadForm->selectWords->endY, &this->notePadForm->selectWords->endX);

		//1.4.1. ���� �ܾ ����, ������ ��ġ���ΰ� TRUE�̸� ��,�Ʒ��� ���� ������ �޸��Ͽ� �����ܾ �ִ��� Ȯ���Ѵ�.
		if (this->searchAround == TRUE && (this->notePadForm->selectWords->firstY <= 0 &&
			this->notePadForm->selectWords->firstX <= 0 && this->notePadForm->selectWords->endY <= 0 &&
			this->notePadForm->selectWords->endX <= 0)) {

			//(21.11.04.�߰�) 
			this->notePadForm->isUp = FALSE; //������ ��ġ�϶� �ǹؿ��� ������ ���� �ݴ�� ����
			this->notePadForm->isDown = FALSE;

			int noteLength = this->notePadForm->note->GetLength();
			row = this->notePadForm->note->GetChild(noteLength - 1);
			int rowLength = row->GetLength();

			//�ǳ���, ����ǥ���� �ٽ� ã��.
			this->notePadForm->note->FindSameTextWithUp((LPCTSTR)this->findStr, this->matchCase, noteLength-1, rowLength,
				&this->notePadForm->selectWords->firstY, &this->notePadForm->selectWords->firstX,
				&this->notePadForm->selectWords->endY, &this->notePadForm->selectWords->endX);
		}
	}
	
	//1.5. �ڵ������� �������ִ� ���,
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		//1.5.1. ��ģ ������ �ٽ� �ڵ������Ѵ�. (ON)
		this->notePadForm->lineChange->LineChangeButtonClicked();

		//1.5.2. first��ǥ, end��ǥ�� �ڵ����� ���� ��ġ�� ���Ѵ�.
		this->notePadForm->lineChange->FindCaretAfterLineChange(this->notePadForm->selectWords->firstX,
			this->notePadForm->selectWords->firstY, &this->notePadForm->selectWords->firstX,
			&this->notePadForm->selectWords->firstY);

		this->notePadForm->lineChange->FindCaretAfterLineChange(this->notePadForm->selectWords->endX,
			this->notePadForm->selectWords->endY, &this->notePadForm->selectWords->endX,
			&this->notePadForm->selectWords->endY);
	}

	//1.6. ���� �ܾ �ִ� ���, end��ǥ�� ĳ���� �̵��Ѵ�.
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


	//(21.11.04.�߰�) 
	this->notePadForm->isUp = FALSE; //������ ��ġ�϶� �ǹؿ��� ������ ���� �ݴ�� ����
	this->notePadForm->isDown = FALSE;

	//_______________________________________________
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
	//________________________________________________


	//(21.09.16 �߰�)
	this->notePadForm->selectWords->beforeFirstX = firstX;
	this->notePadForm->selectWords->beforeFirstY = firstY;
	this->notePadForm->selectWords->beforeEndX = endX;
	this->notePadForm->selectWords->beforeEndY = endY;
	this->notePadForm->selectWords->afterEndY = endY;


	//1. �ٲٱ� ��ư�� Ŭ������ ��
	//1.1. ���� ĳ���� ��ġ�� ã�´�.
	currentY = this->notePadForm->note->GetCurrent();
	row = this->notePadForm->note->GetChild(currentY);
	currentX = row->GetCurrent();

	//1.2. �ڵ������� ������ �ִ� ���,
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		//1.2.1. ���ܰ� �ƴ� ��Ȳ����, �ڵ����� �� ĳ���� ��ġ�� ã�´�.
		this->notePadForm->lineChange->FindCaretBeforeLineChange(&currentX, &currentY);

		//1.3.1. �ڵ������� ���, �ڵ� ���� �� firstXY, endXY�� ���Ѵ�.
		this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
		this->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);
		
		//1.2.2. �ڵ����� �� ������ ��ģ��.(OFF)
		this->notePadForm->lineChange->LineChangeButtonNotClicked();
		this-> notePadForm->selectWords->isLineChangeButtonClicked = TRUE;
		this->notePadForm->selectWords->windowWidth = this->notePadForm->scroll->windowWidth;
	}

	//1.3.2. ���õ� �ܾ �ִ� ���� ã�´�.
	selectedRow = this->notePadForm->note->GetChild(firstY);



	//1.3.3. �ش� �ܾ ã�´�.
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
	//������ ��ġ�� ���,
	if (this->searchAround == TRUE && this->exception == TRUE) {


		this->notePadForm->note->ReplaceSameText((LPCTSTR)this->findStr, (LPCTSTR)this->replaceStr, selectedStr, 
			this->matchCase, this->searchAround,
			currentY, currentX, &this->notePadForm->selectWords->firstY, &this->notePadForm->selectWords->firstX,
			&this->notePadForm->selectWords->endY, &this->notePadForm->selectWords->endX, &this->exception);


		this->searchAround = FALSE;
		this->exception = FALSE;
	}

	else {
		//1.4. �ܾ �ٲ� ��, ���� �ܾ ã�´�.
		this->notePadForm->note->ReplaceSameText((LPCTSTR)this->findStr, (LPCTSTR)this->replaceStr, selectedStr, 
			this->matchCase, this->searchAround,
			currentY, currentX, &this->notePadForm->selectWords->firstY, &this->notePadForm->selectWords->firstX,
			&this->notePadForm->selectWords->endY, &this->notePadForm->selectWords->endX, &this->exception);
	}
#endif
	//-------------------------------------------------------------------------------------
	//1.4.(21.09.17 ����) �ܾ �ٲ� ��, ���� �ܾ ã�´�. 
#if 0
	this->notePadForm->note->ReplaceSameText((LPCTSTR)this->findStr, (LPCTSTR)this->replaceStr, selectedStr,
		this->matchCase, FALSE,
		currentY, currentX, &this->notePadForm->selectWords->firstY, &this->notePadForm->selectWords->firstX,
		&this->notePadForm->selectWords->endY, &this->notePadForm->selectWords->endX, &this->exception);
#endif

	findAndReplace = new FindAndReplace(this);
	
	// ó���� '�ٲٴ�' ������ ���� ������ ��ġ�� FALSE�� �������ش�.
	findAndReplace->ReplaceSameText((LPCTSTR)this->findStr, (LPCTSTR)this->replaceStr, selectedStr,
		this->matchCase, FALSE,
		currentY, currentX, &this->notePadForm->selectWords->firstY, &this->notePadForm->selectWords->firstX,
		&this->notePadForm->selectWords->endY, &this->notePadForm->selectWords->endX, &this->exception,
		&this->notePadForm->selectWords->isReplaced);

	isReplaced = this->notePadForm->selectWords->isReplaced;

	//������ ��ġ�� ���,
	if ((this->searchAround == TRUE && this->exception == TRUE) ||
		(this->notePadForm->selectWords->firstY<=0 && this->notePadForm->selectWords->firstX <=0 &&
			this->notePadForm->selectWords->endY<=0 && this->notePadForm->selectWords->endX<=0)) {


		//(21.11.04.�߰�) 
		this->notePadForm->isUp = TRUE; //������ ��ġ�϶� �ǹؿ��� ������ ���� �ݴ�� ����
		this->notePadForm->isDown = FALSE;


		//1.2. ���õ� �ܾ �ִ� ���� ã�´�.
		selectedRow = this->notePadForm->note->GetChild(this->notePadForm->selectWords->firstY);

		//1.3. �ش� �ܾ ã�´�.
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
		//1.3.3. �ش� �ܾ ã�´�.
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
	//1.4.1. ���� �ܾ ����, ������ ��ġ���ΰ� TRUE�̸� ��,�Ʒ��� ���� ������ �޸��Ͽ� �����ܾ �ִ��� Ȯ���Ѵ�.
	if (this->searchAround == TRUE && this->exception == TRUE) {

		//(0,0)���� �ٽ� ã��.
#if 0
		this->notePadForm->note->ReplaceSameText((LPCTSTR)this->findStr, (LPCTSTR)this->replaceStr, selectedStr, 
			this->matchCase, 0, 0, &this->notePadForm->selectWords->firstY, 
			&this->notePadForm->selectWords->firstX,
			&this->notePadForm->selectWords->endY, &this->notePadForm->selectWords->endX, &this->exception);
#endif
		ReplaceAndSearchText(); //ReplaceSameText ���� 
	}
#endif

	//1.5. �ڵ������� �������ִ� ���,
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		//1.5.1. ��ģ ������ �ٽ� �ڵ������Ѵ�. (ON)
		this->notePadForm->lineChange->LineChangeButtonClicked();

		//1.5.2. first��ǥ, end��ǥ�� �ڵ����� ���� ��ġ�� ���Ѵ�.
		this->notePadForm->lineChange->FindCaretAfterLineChange(this->notePadForm->selectWords->firstX,
			this->notePadForm->selectWords->firstY, &this->notePadForm->selectWords->firstX,
			&this->notePadForm->selectWords->firstY);

		this->notePadForm->lineChange->FindCaretAfterLineChange(this->notePadForm->selectWords->endX,
			this->notePadForm->selectWords->endY, &this->notePadForm->selectWords->endX,
			&this->notePadForm->selectWords->endY);

#if 0
		//1.5.3. (21.09.19 �߰�) afterEndX, afterEndY ��ǥ�� �ڵ����� ���� ��ġ�� ���Ѵ�.
		this->notePadForm->lineChange->FindCaretAfterLineChange(this->notePadForm->selectWords->afterEndX,
			this->notePadForm->selectWords->afterEndY, &this->notePadForm->selectWords->afterEndX,
			&this->notePadForm->selectWords->afterEndY);
#endif
	}




	//1.6. ���� �ܾ �ִ� ���, end��ǥ�� ĳ���� �̵��Ѵ�.
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


	//1. �ٲٱ� ��ư�� Ŭ������ ��
	//1.1. ���� ĳ���� ��ġ�� ã�´�.
	currentY = this->notePadForm->note->GetCurrent();
	row = this->notePadForm->note->GetChild(currentY);
	currentX = row->GetCurrent();

	//1.2. �ڵ������� ������ �ִ� ���,
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		//1.2.1. ���ܰ� �ƴ� ��Ȳ����, �ڵ����� �� ĳ���� ��ġ�� ã�´�.
		this->notePadForm->lineChange->FindCaretBeforeLineChange(&currentX, &currentY);
		this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
		this->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);

		//1.2.2. �ڵ����� �� ������ ��ģ��.(OFF)
		this->notePadForm->lineChange->LineChangeButtonNotClicked();
	}


	//1.3.2. ���õ� �ܾ �ִ� ���� ã�´�.
	selectedRow = this->notePadForm->note->GetChild(firstY);

	//1.3.3. �ش� �ܾ ã�´�.
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



	//1.4. �ܾ �ٲ� ��, ���� �ܾ ã�´�.
	findAndReplace = new FindAndReplace(this);

	findAndReplace->ReplaceSameText((LPCTSTR)this->findStr, (LPCTSTR)this->replaceStr, selectedStr, this->matchCase,
		FALSE,
		currentY, currentX, &this->notePadForm->selectWords->firstY, &this->notePadForm->selectWords->firstX,
		&this->notePadForm->selectWords->endY, &this->notePadForm->selectWords->endX, &this->exception,
		&this->notePadForm->selectWords->isReplaced);

#if 0
	//1.3. �˻��� �Ʒ��� �������� �����ϸ�, �ش� �������� ���� �ܾ �ִ��� Ȯ���Ѵ�.
	if (this->searchDown == TRUE) {
		this->notePadForm->note->FindSameTextWithDown((LPCTSTR)this->findStr, this->matchCase, currentY, currentX,
			&this->notePadForm->selectWords->firstY, &this->notePadForm->selectWords->firstX,
			&this->notePadForm->selectWords->endY, &this->notePadForm->selectWords->endX);

		//1.3.1. ���� �ܾ ����, ������ ��ġ���ΰ� TRUE�̸� ��,�Ʒ��� ���� ������ �޸��Ͽ� �����ܾ �ִ��� Ȯ���Ѵ�.
		if (this->searchAround == TRUE && (this->notePadForm->selectWords->firstY <= 0 &&
			this->notePadForm->selectWords->firstX <= 0 && this->notePadForm->selectWords->endY <= 0 &&
			this->notePadForm->selectWords->endX <= 0)) {

			//(0,0)���� �ٽ� ã��.
			this->notePadForm->note->FindSameTextWithDown((LPCTSTR)this->findStr, this->matchCase, 0, 0,
				&this->notePadForm->selectWords->firstY, &this->notePadForm->selectWords->firstX,
				&this->notePadForm->selectWords->endY, &this->notePadForm->selectWords->endX);
		}
	}
#endif


	//1.5. �ڵ������� �������ִ� ���,
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		//1.5.1. ��ģ ������ �ٽ� �ڵ������Ѵ�. (ON)
		this->notePadForm->lineChange->LineChangeButtonClicked();

		//1.5.2. first��ǥ, end��ǥ�� �ڵ����� ���� ��ġ�� ���Ѵ�.
		this->notePadForm->lineChange->FindCaretAfterLineChange(this->notePadForm->selectWords->firstX,
			this->notePadForm->selectWords->firstY, &this->notePadForm->selectWords->firstX,
			&this->notePadForm->selectWords->firstY);

		this->notePadForm->lineChange->FindCaretAfterLineChange(this->notePadForm->selectWords->endX,
			this->notePadForm->selectWords->endY, &this->notePadForm->selectWords->endX,
			&this->notePadForm->selectWords->endY);
	}

	//1.6. ���� �ܾ �ִ� ���, end��ǥ�� ĳ���� �̵��Ѵ�.
	if (this->notePadForm->selectWords->firstX > 0 || this->notePadForm->selectWords->firstY > 0 ||
		this->notePadForm->selectWords->endX > 0 || this->notePadForm->selectWords->endY > 0) {

		this->notePadForm->note->MoveCurrent(this->notePadForm->selectWords->endY);
		row = this->notePadForm->note->GetChild(this->notePadForm->selectWords->endY);
		row->MoveCurrent(this->notePadForm->selectWords->endX);
	}

}













