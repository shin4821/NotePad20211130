//OnTimeAndDateCommand.cpp
#include"OnTimeAndDateCommand.h"
#include"NotePadForm.h"
#include"Note.h"
#include"LineChange.h"
#include"CreateGlyph.h"
#include"CreateProduct.h"
#include"SelectWords.h"
#include"KeyAction.h"
#include"FactoryProduct.h"
#include"Scroll.h"
#include <time.h>
#pragma warning(disable:4996);

OnTimeAndDateCommand::OnTimeAndDateCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
	this->isLineChangeButtonClicked = false;
}

OnTimeAndDateCommand::~OnTimeAndDateCommand() {
}

void OnTimeAndDateCommand::Execute() {
	time_t cur = time(NULL);
	struct tm* ptm = localtime(&cur);
	CString amPm;
	char totalBuf[100] = { 0 };
	char buf[100] = { 0 };
	CString totalDate;
	int i = 0;
	int j = 0;
	int k;
	bool isLineChanged = false;
	bool lastLineChanged = false;
	bool isThereRestOnes = false;
	int changedX;
	int changedY;
	char afterCharacter[2];
	char* character_;
	CreateGlyph* creator = 0;
	Glyph* character = 0;
	Glyph* currentRow;
	int restX;
	int restY;
	int restLength;

	//1. �ð�/��¥ ������ ��
	creator = new CreateProduct();

	//1.0. ���� �ð��� ���´�.
	size_t strftime(char* strDest, size_t maxsize, const char* format, const struct tm* timeptr);

	//1.1. PM�̸� ����, AM�̸� �������� �ٲ۴�.
	strftime(buf, sizeof(buf), "%p", ptm);
	if (buf == "AM") {
		amPm = "����";
	}
	else {
		amPm = "����";
	}

	//1.2. �� �տ� 0�̸� �ڿ� �ѱ��ڷθ� �ٲ۴�.
	strftime(buf, sizeof(buf), "%I", ptm);
	CString timeNow(buf);
	if (timeNow.Left(1) == "0") {
		timeNow = timeNow.Mid(1, 1);
	}

	strftime(totalBuf, sizeof(totalBuf), "%M %Y-%m-%d", ptm); //"%I:%M %Y-%m-%d"
	CString strTotalBuf(totalBuf);
	totalDate.Format("%s %s:%s", amPm, timeNow, strTotalBuf);

	this->totalDate = (LPCTSTR)totalDate;
	//1.3. ���� ĳ���� �ִ� ��ġ�� ���� ��¥�� ���´�.

	//2. ���� ĳ���� ��ġ�� Ȯ���Ѵ�.
	int currentY = this->notePadForm->note->GetCurrent();
	this->notePadForm->row = this->notePadForm->note->GetChild(currentY);
	int currentX = this->notePadForm->row->GetCurrent();
	int rowLength = this->notePadForm->row->GetLength();
	int strLength = totalDate.GetLength();
	character_ = new char[strLength + 1];
	strcpy(character_, totalDate);

	//2.1. ���� ��¥ �����ֱ� ���� ��ǥ�� ���س��´�.
	this->firstX = currentX;
	this->firstY = currentY;

	//3.(21.08.24 �߰�) �ڵ������� ������ �ִ� ���, ****************************************
	if (this->notePadForm->isLineChangeButtonClicked == true) {

		//3.1. �ڵ����� �� ĳ�� ��ǥ�� ã�´�.
		this->notePadForm->lineChange->FindCaretBeforeLineChange(&currentX, &currentY);

		//3.2. �ڵ������ ������ ��ģ��. (�ڵ������ư ������� �� (OFF))
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

		//3.3. �ش� ���� length�� ã�´�.
		this->notePadForm->row = this->notePadForm->note->GetChild(currentY);
		rowLength = this->notePadForm->row->GetLength();
		this->isLineChangeButtonClicked = true;
		this->windowWidth = this->notePadForm->scroll->windowWidth;
	}
	//****************************************************************************************
	//3.1. ���������� ���
	if (currentX >= rowLength) {

		k = currentY;
		this->notePadForm->row = this->notePadForm->note->GetChild(k);

		//3.1.1 str�� length��ŭ �ݺ��Ѵ�.
		while (i < strLength) {//character_[i]!= NULL
			//3.1.1.1. ��Ʈ�� ���� ���� ���Ѵ�.						
			isLineChanged = false;

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
					this->notePadForm->row = creator->Make(afterCharacter);
					this->notePadForm->note->Add(k, this->notePadForm->row);
					//row = this->notePadForm->note->GetChild(k);


					//1.1.2. ������ �ٲ۴�.
					isLineChanged = true;
					lastLineChanged = true;
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
			if (isLineChanged == false) {
				//this->notePadForm->note->GetChild(k);
				this->notePadForm->row->Add(character); //row->Add(character); ************************************
				//row->MoveCurrent(row->GetLength());
				j++;
			}

		}

		//3.1.2. �ٲ� ��ǥ�� �����Ѵ�.
		changedY = k;
		//changedX = currentX + j;
		if (lastLineChanged == true) {
			changedX = j;
		}
		else {
			changedX = currentX + j;
		}
	}

	//3.2. ���������� ���
	else if (currentX < rowLength) {

		//3.2.1 str�� length��ŭ �ݺ��Ѵ�.
		int l = currentX;
		k = currentY;
		i = 0;

		//3.1.1 str�� length��ŭ �ݺ��Ѵ�.
		while (i < strLength) {//character_[i]!= NULL
			//3.1.1.1. ��Ʈ�� ���� ���� ���Ѵ�.
			//row = this->notePadForm->note->GetChild(k);

			isLineChanged = false;

			afterCharacter[0] = character_[i];
			afterCharacter[1] = character_[i + 1];

			//1.1byte�� ���,
			if (afterCharacter[0] >= 0 && afterCharacter[0] < 128) {

				//1.0. ù��° ��ġ�� �ι��ڸ� �־��ش�.
				afterCharacter[1] = '\0';

				//1.1. ���๮���̸�,
				if (afterCharacter[0] == '\n') {

					//������ ������ ���ڰ� �ִ��� Ȯ���Ѵ�.**************************			
					restX = currentX + j;

					if (this->notePadForm->row->GetLength() > restX) {
						isThereRestOnes = true;
						restY = currentY;
					}
					//***************************************************************

					//1.1.1. ���ο� ���ڸ� �����.
					k++;

					//row = new Row();
					this->notePadForm->row = creator->Make(afterCharacter);
					this->notePadForm->note->Add(k, this->notePadForm->row);

					//1.1.3. ������ �ٲ۴�.
					isLineChanged = true;
					lastLineChanged = true;
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
			if (isLineChanged == false) {
				this->notePadForm->row = this->notePadForm->note->GetChild(k);
				this->notePadForm->row->Add(l, character); //row->Add(character);
				//row->MoveCurrent(row->GetLength());
				l++;
				j++;
			}

		}
		//3.1.2. �������� �� ��, ���� �� ������ ���ڸ� ���� �� ���ڿ� �������´�.******************
		if (isThereRestOnes == true) {

			// ������ ���� ���� ���� ����
			this->notePadForm->row = this->notePadForm->note->GetChild(restY);
			rowLength = this->notePadForm->row->GetLength();
			restLength = rowLength - restX;

			// �ֱٱ����� �߰��� ���� ����
			currentRow = this->notePadForm->note->GetChild(k);

			// �ֱٱ����� �ٿ� ������ ���� ���ڸ� �߰��Ѵ�.
			while (restLength > 0) {
				character = this->notePadForm->row->GetChild(restX);
				currentRow->Add(character);
				this->notePadForm->row->Remove(restX);
				j++;
				restLength--;
			}

		}
		//******************************************************************************************

		//3.1.3. �ٲ� ��ǥ�� �����Ѵ�.
		changedY = k;
		//changedX = currentX + j;
		if (lastLineChanged == true) {
			changedX = j;
		}
		else {
			changedX = currentX + j;
		}

	}


	//4.(21.08.24 �߰�) �ڵ������� ������ �ִ� ���, ****************************************
	if (this->notePadForm->isLineChangeButtonClicked == true) {

		//4.1. ��ģ ������ �ٽ� �ڵ������Ѵ�. (�ڵ������ư ��������(ON)
		this->notePadForm->lineChange->LineChangeButtonClicked();

		//4.2. �ڵ����� �� �̵��� ĳ���� ��ġ�� ���Ѵ�.
		this->notePadForm->lineChange->FindCaretAfterLineChange(changedX, changedY, &changedX, &changedY);
	}
	//********************************************************************************************

	//5. ��ǥ�� �̵����ش�.
	this->notePadForm->note->MoveCurrent(changedY);
	this->notePadForm->row = this->notePadForm->note->GetChild(changedY);
	this->notePadForm->row->MoveCurrent(changedX);

	//5. endXY ��ǥ�� �������ش�.
	this->endX = changedX;
	this->endY = changedY;
	this->currentX = changedX;
	this->currentY = changedY;


	this->notePadForm->Notify();
}

void OnTimeAndDateCommand::Undo() {
	Glyph* row;
	int windowWidth = this->notePadForm->scroll->windowWidth; //���� ������ʺ�

	//1. firstX, firstY ���� �����.
	int firstX = this->firstX;
	int firstY = this->firstY;
	int currentX = this->endX;
	int currentY = this->endY;


	//1. (21.09.30 �߰�) ���� �ڵ������ư ��Ȳ�� �������� ��Ȳ�� �ٸ���,
	if (this->notePadForm->isLineChangeButtonClicked != this->isLineChangeButtonClicked) {

		//1.1. ���� �ڵ������ư�� ������ ������, �ڵ����� ���� ��ǥ�� ���Ѵ�. 
		if (this->notePadForm->isLineChangeButtonClicked == true) {
			this->notePadForm->lineChange->FindCaretAfterLineChange(currentX, currentY, &currentX, &currentY);
			this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);

		}
		//1.2. ���� �ڵ������ư�� �ȴ����� ������, �ڵ����� ���� ��ǥ�� ���Ѵ�.
		else {
			//1.2.1. �ڵ����� �����ش�.
			this->notePadForm->lineChange->LineChangeButtonClicked(this->windowWidth);
			//1.2.2. �ڵ����� �� ��ǥ ���Ѵ�.
			this->notePadForm->lineChange->FindCaretBeforeLineChange(currentX, currentY, &currentX, &currentY);
			this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
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
		this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);

		//2.4. �ڵ������ ������ ��ģ��.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

		//2.5. ��ģ ������ ���� windowWidth��ŭ �ڵ������Ѵ�.
		this->notePadForm->lineChange->LineChangeButtonClicked(windowWidth);

		//2.6. �ڵ����� �� ��ǥ�� ���Ѵ�.
		this->notePadForm->lineChange->FindCaretAfterLineChange(currentX, currentY, &currentX, &currentY);
		this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
	}

	//���õ� ��ǥ�� ����ġ�Ѵ�.
	this->notePadForm->selectWords->firstX = firstX;
	this->notePadForm->selectWords->firstY = firstY;
	this->notePadForm->selectWords->endX = currentX;
	this->notePadForm->selectWords->endY = currentY;

	//2. endX, endY�� ���� ��ġ�� �������ش�.
	this->notePadForm->note->MoveCurrent(currentY);
	row = this->notePadForm->note->GetChild(currentY);
	row->MoveCurrent(currentX);


	KeyAction* keyAction = NULL;
	FactoryProduct factoryProduct(this->notePadForm);

	keyAction = factoryProduct.Make(VK_BACK, NULL, NULL);
	keyAction->KeyDown();
}

void OnTimeAndDateCommand::Redo() {
	string temp;
	string doubleByteCharacter;
	char singleByteCharacter[2];
	singleByteCharacter[0] = this->totalDate.at(0);
	int id;
	int i = 0;
	int windowWidth = this->notePadForm->scroll->windowWidth; //���� ������ʺ�
	int currentX = this->firstX;
	int currentY = this->firstY;


	//1. (21.10.01 �߰�) ���� �ڵ������ư ��Ȳ�� �������� ��Ȳ�� �ٸ���,
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

	//1. ���� ĳ���� ��ġ�� �������ش�.
	this->notePadForm->note->MoveCurrent(currentY);
	Glyph* row = this->notePadForm->note->GetChild(currentY);
	row->MoveCurrent(currentX);
	this->notePadForm->isUndo = true;

	//2. character�� length��ŭ �ݺ��Ѵ�.
	while (i < this->totalDate.size()) {

		//2.1.  ĳ���� ũ��� Undo�� 3mm�� �����Ѵ�.
		this->notePadForm->isFirstComposing = false;

		//2.1. 1Byte���� 2Byte���� Ȯ���Ѵ�.
		singleByteCharacter[0] = this->totalDate.at(i);

		//2.2. ���� character�� �ִ� ���ڰ� 1Byte�̸�, OnChar�� ȣ���Ѵ�.
		if (singleByteCharacter[0] >= 0 && singleByteCharacter[0] <= 127) {

			//2.2.1. ���๮���̸� ��ǥ�� currentY++, currentX=0 ���� ��ȯ���ش�.
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
		//2.3. ���� character�� �ִ� ���ڰ� 2Byte�̸�, OnImeComposition ȣ���Ѵ�.
		else {
			doubleByteCharacter.clear();
			temp = this->totalDate.at(i);
			doubleByteCharacter.append(temp);
			temp = this->totalDate.at(i + 1);
			doubleByteCharacter.append(temp);

			this->notePadForm->letter = doubleByteCharacter;
			this->notePadForm->SendMessage(WM_COMMAND, 40005);
			i++;
		}
		i++;
	}
	//3. Redo �� �ٲ� ĳ���� ��ġ�� �����Ѵ�.
	this->currentX = this->notePadForm->currentX;
	this->currentY = this->notePadForm->currentY;

	this->notePadForm->isUndo = false;
	//this->notePadForm->isThereSelected == false;
}

void OnTimeAndDateCommand::SetCheck() {
	this->isChecked = true;
}

bool OnTimeAndDateCommand::GetChecked() {
	return this->isChecked;
}

Long OnTimeAndDateCommand::GetCurrentX() {
	return this->endX;
}
Long OnTimeAndDateCommand::GetCurrentY() {
	return this->endY;
}
char OnTimeAndDateCommand::GetCharacter() {
	return 'A';
}
