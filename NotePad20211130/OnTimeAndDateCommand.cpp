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

	//1. 시간/날짜 눌렀을 때
	creator = new CreateProduct();

	//1.0. 현재 시간을 얻어온다.
	size_t strftime(char* strDest, size_t maxsize, const char* format, const struct tm* timeptr);

	//1.1. PM이면 오후, AM이면 오전으로 바꾼다.
	strftime(buf, sizeof(buf), "%p", ptm);
	if (buf == "AM") {
		amPm = "오전";
	}
	else {
		amPm = "오후";
	}

	//1.2. 시 앞에 0이면 뒤에 한글자로만 바꾼다.
	strftime(buf, sizeof(buf), "%I", ptm);
	CString timeNow(buf);
	if (timeNow.Left(1) == "0") {
		timeNow = timeNow.Mid(1, 1);
	}

	strftime(totalBuf, sizeof(totalBuf), "%M %Y-%m-%d", ptm); //"%I:%M %Y-%m-%d"
	CString strTotalBuf(totalBuf);
	totalDate.Format("%s %s:%s", amPm, timeNow, strTotalBuf);

	this->totalDate = (LPCTSTR)totalDate;
	//1.3. 현재 캐럿이 있는 위치에 오늘 날짜를 적는다.

	//2. 현재 캐럿의 위치를 확인한다.
	int currentY = this->notePadForm->note->GetCurrent();
	this->notePadForm->row = this->notePadForm->note->GetChild(currentY);
	int currentX = this->notePadForm->row->GetCurrent();
	int rowLength = this->notePadForm->row->GetLength();
	int strLength = totalDate.GetLength();
	character_ = new char[strLength + 1];
	strcpy(character_, totalDate);

	//2.1. 오늘 날짜 적어주기 전에 좌표를 구해놓는다.
	this->firstX = currentX;
	this->firstY = currentY;

	//3.(21.08.24 추가) 자동개행이 눌려져 있는 경우, ****************************************
	if (this->notePadForm->isLineChangeButtonClicked == true) {

		//3.1. 자동개행 전 캐럿 좌표를 찾는다.
		this->notePadForm->lineChange->FindCaretBeforeLineChange(&currentX, &currentY);

		//3.2. 자동개행된 문서를 펼친다. (자동개행버튼 취소했을 때 (OFF))
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

		//3.3. 해당 줄의 length를 찾는다.
		this->notePadForm->row = this->notePadForm->note->GetChild(currentY);
		rowLength = this->notePadForm->row->GetLength();
		this->isLineChangeButtonClicked = true;
		this->windowWidth = this->notePadForm->scroll->windowWidth;
	}
	//****************************************************************************************
	//3.1. 붙혀적다의 경우
	if (currentX >= rowLength) {

		k = currentY;
		this->notePadForm->row = this->notePadForm->note->GetChild(k);

		//3.1.1 str의 length만큼 반복한다.
		while (i < strLength) {//character_[i]!= NULL
			//3.1.1.1. 노트의 현재 줄을 구한다.						
			isLineChanged = false;

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
					this->notePadForm->row = creator->Make(afterCharacter);
					this->notePadForm->note->Add(k, this->notePadForm->row);
					//row = this->notePadForm->note->GetChild(k);


					//1.1.2. 조건을 바꾼다.
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

			//2. 2byte인 경우
			else {
				//character = new DoubleByteCharacter(afterCharacter);
				character = creator->Make(afterCharacter);
				i = i + 2;
			}

			//3. 개행문자가 아니면 row에 Add한다.
			if (isLineChanged == false) {
				//this->notePadForm->note->GetChild(k);
				this->notePadForm->row->Add(character); //row->Add(character); ************************************
				//row->MoveCurrent(row->GetLength());
				j++;
			}

		}

		//3.1.2. 바뀐 좌표를 설정한다.
		changedY = k;
		//changedX = currentX + j;
		if (lastLineChanged == true) {
			changedX = j;
		}
		else {
			changedX = currentX + j;
		}
	}

	//3.2. 끼워적다의 경우
	else if (currentX < rowLength) {

		//3.2.1 str의 length만큼 반복한다.
		int l = currentX;
		k = currentY;
		i = 0;

		//3.1.1 str의 length만큼 반복한다.
		while (i < strLength) {//character_[i]!= NULL
			//3.1.1.1. 노트의 현재 줄을 구한다.
			//row = this->notePadForm->note->GetChild(k);

			isLineChanged = false;

			afterCharacter[0] = character_[i];
			afterCharacter[1] = character_[i + 1];

			//1.1byte인 경우,
			if (afterCharacter[0] >= 0 && afterCharacter[0] < 128) {

				//1.0. 첫번째 위치에 널문자를 넣어준다.
				afterCharacter[1] = '\0';

				//1.1. 개행문자이면,
				if (afterCharacter[0] == '\n') {

					//나머지 여분의 글자가 있는지 확인한다.**************************			
					restX = currentX + j;

					if (this->notePadForm->row->GetLength() > restX) {
						isThereRestOnes = true;
						restY = currentY;
					}
					//***************************************************************

					//1.1.1. 새로운 문자를 만든다.
					k++;

					//row = new Row();
					this->notePadForm->row = creator->Make(afterCharacter);
					this->notePadForm->note->Add(k, this->notePadForm->row);

					//1.1.3. 조건을 바꾼다.
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

			//2. 2byte인 경우
			else {
				//character = new DoubleByteCharacter(afterCharacter);
				character = creator->Make(afterCharacter);
				i = i + 2;
			}

			//3. 개행문자가 아니면 row에 Add한다.
			if (isLineChanged == false) {
				this->notePadForm->row = this->notePadForm->note->GetChild(k);
				this->notePadForm->row->Add(l, character); //row->Add(character);
				//row->MoveCurrent(row->GetLength());
				l++;
				j++;
			}

		}
		//3.1.2. 끼워적고 난 후, 개행 전 나머지 문자를 개행 후 문자에 붙혀적는다.******************
		if (isThereRestOnes == true) {

			// 나머지 문자 남은 줄의 정보
			this->notePadForm->row = this->notePadForm->note->GetChild(restY);
			rowLength = this->notePadForm->row->GetLength();
			restLength = rowLength - restX;

			// 최근까지의 추가한 줄의 정보
			currentRow = this->notePadForm->note->GetChild(k);

			// 최근까지의 줄에 나머지 줄의 글자를 추가한다.
			while (restLength > 0) {
				character = this->notePadForm->row->GetChild(restX);
				currentRow->Add(character);
				this->notePadForm->row->Remove(restX);
				j++;
				restLength--;
			}

		}
		//******************************************************************************************

		//3.1.3. 바뀐 좌표를 설정한다.
		changedY = k;
		//changedX = currentX + j;
		if (lastLineChanged == true) {
			changedX = j;
		}
		else {
			changedX = currentX + j;
		}

	}


	//4.(21.08.24 추가) 자동개행이 눌려져 있는 경우, ****************************************
	if (this->notePadForm->isLineChangeButtonClicked == true) {

		//4.1. 펼친 문서를 다시 자동개행한다. (자동개행버튼 눌렀을때(ON)
		this->notePadForm->lineChange->LineChangeButtonClicked();

		//4.2. 자동개행 후 이동할 캐럿의 위치를 구한다.
		this->notePadForm->lineChange->FindCaretAfterLineChange(changedX, changedY, &changedX, &changedY);
	}
	//********************************************************************************************

	//5. 좌표를 이동해준다.
	this->notePadForm->note->MoveCurrent(changedY);
	this->notePadForm->row = this->notePadForm->note->GetChild(changedY);
	this->notePadForm->row->MoveCurrent(changedX);

	//5. endXY 좌표를 설정해준다.
	this->endX = changedX;
	this->endY = changedY;
	this->currentX = changedX;
	this->currentY = changedY;


	this->notePadForm->Notify();
}

void OnTimeAndDateCommand::Undo() {
	Glyph* row;
	int windowWidth = this->notePadForm->scroll->windowWidth; //현재 윈도우너비

	//1. firstX, firstY 까지 지운다.
	int firstX = this->firstX;
	int firstY = this->firstY;
	int currentX = this->endX;
	int currentY = this->endY;


	//1. (21.09.30 추가) 현재 자동개행버튼 상황과 적을때의 상황이 다르면,
	if (this->notePadForm->isLineChangeButtonClicked != this->isLineChangeButtonClicked) {

		//1.1. 현재 자동개행버튼이 눌려져 있으면, 자동개행 후의 좌표를 구한다. 
		if (this->notePadForm->isLineChangeButtonClicked == true) {
			this->notePadForm->lineChange->FindCaretAfterLineChange(currentX, currentY, &currentX, &currentY);
			this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);

		}
		//1.2. 현재 자동개행버튼이 안눌려져 있으면, 자동개행 전의 좌표를 구한다.
		else {
			//1.2.1. 자동개행 시켜준다.
			this->notePadForm->lineChange->LineChangeButtonClicked(this->windowWidth);
			//1.2.2. 자동개행 전 좌표 구한다.
			this->notePadForm->lineChange->FindCaretBeforeLineChange(currentX, currentY, &currentX, &currentY);
			this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
			//1.2.3. 자동개행 취소한다.
			this->notePadForm->lineChange->LineChangeButtonNotClicked();
		}
	}
	//2. 현재 자동개행버튼 상황과 적을때의 상황이 같지만, 자동개행일 때 윈도우 너비가 다른 경우,
	else if (this->isLineChangeButtonClicked == true && windowWidth != this->windowWidth) {
		//2.1. 자동개행된 문서를 펼친다.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

		//2.2. 자동개행 시켜준다.
		this->notePadForm->lineChange->LineChangeButtonClicked(this->windowWidth);

		//2.3. 자동개행 전 좌표를 구한다.
		this->notePadForm->lineChange->FindCaretBeforeLineChange(currentX, currentY, &currentX, &currentY);
		this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);

		//2.4. 자동개행된 문서를 펼친다.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

		//2.5. 펼친 문서를 현재 windowWidth만큼 자동개행한다.
		this->notePadForm->lineChange->LineChangeButtonClicked(windowWidth);

		//2.6. 자동개행 후 좌표를 구한다.
		this->notePadForm->lineChange->FindCaretAfterLineChange(currentX, currentY, &currentX, &currentY);
		this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
	}

	//선택된 좌표를 원위치한다.
	this->notePadForm->selectWords->firstX = firstX;
	this->notePadForm->selectWords->firstY = firstY;
	this->notePadForm->selectWords->endX = currentX;
	this->notePadForm->selectWords->endY = currentY;

	//2. endX, endY를 현재 위치로 설정해준다.
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
	int windowWidth = this->notePadForm->scroll->windowWidth; //현재 윈도우너비
	int currentX = this->firstX;
	int currentY = this->firstY;


	//1. (21.10.01 추가) 현재 자동개행버튼 상황과 적을때의 상황이 다르면,
	if (this->notePadForm->isLineChangeButtonClicked != this->isLineChangeButtonClicked) {

		//1.1. 현재 자동개행버튼이 눌려져 있으면, 자동개행 후의 좌표를 구한다. 
		if (this->notePadForm->isLineChangeButtonClicked == true) {
			this->notePadForm->lineChange->FindCaretAfterLineChange(currentX, currentY, &currentX, &currentY);
		}
		//1.2. 현재 자동개행버튼이 안눌려져 있으면, 자동개행 전의 좌표를 구한다.
		else {
			//1.2.1. 자동개행 시켜준다.
			this->notePadForm->lineChange->LineChangeButtonClicked(this->windowWidth);
			//1.2.2. 자동개행 전 좌표 구한다.
			this->notePadForm->lineChange->FindCaretBeforeLineChange(currentX, currentY, &currentX, &currentY);
			//1.2.3. 자동개행 취소한다.
			this->notePadForm->lineChange->LineChangeButtonNotClicked();
		}
	}
	//2. 현재 자동개행버튼 상황과 적을때의 상황이 같지만, 자동개행일 때 윈도우 너비가 다른 경우,
	else if (this->isLineChangeButtonClicked == true && windowWidth != this->windowWidth) {
		//2.1. 자동개행된 문서를 펼친다.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

		//2.2. 자동개행 시켜준다.
		this->notePadForm->lineChange->LineChangeButtonClicked(this->windowWidth);

		//2.3. 자동개행 전 좌표를 구한다.
		this->notePadForm->lineChange->FindCaretBeforeLineChange(currentX, currentY, &currentX, &currentY);

		//2.4. 자동개행된 문서를 펼친다.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

		//2.5. 펼친 문서를 현재 windowWidth만큼 자동개행한다.
		this->notePadForm->lineChange->LineChangeButtonClicked(windowWidth);

		//2.6. 자동개행 후 좌표를 구한다.
		this->notePadForm->lineChange->FindCaretAfterLineChange(currentX, currentY, &currentX, &currentY);
	}

	//1. 현재 캐럿의 위치를 변경해준다.
	this->notePadForm->note->MoveCurrent(currentY);
	Glyph* row = this->notePadForm->note->GetChild(currentY);
	row->MoveCurrent(currentX);
	this->notePadForm->isUndo = true;

	//2. character의 length만큼 반복한다.
	while (i < this->totalDate.size()) {

		//2.1.  캐럿의 크기는 Undo시 3mm로 설정한다.
		this->notePadForm->isFirstComposing = false;

		//2.1. 1Byte인지 2Byte인지 확인한다.
		singleByteCharacter[0] = this->totalDate.at(i);

		//2.2. 현재 character에 있는 문자가 1Byte이면, OnChar을 호출한다.
		if (singleByteCharacter[0] >= 0 && singleByteCharacter[0] <= 127) {

			//2.2.1. 개행문자이면 좌표를 currentY++, currentX=0 으로 변환해준다.
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
		//2.3. 현재 character에 있는 문자가 2Byte이면, OnImeComposition 호출한다.
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
	//3. Redo 후 바뀐 캐럿의 위치를 적용한다.
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
