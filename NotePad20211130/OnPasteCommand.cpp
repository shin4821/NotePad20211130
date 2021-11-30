//OnPasteCommand.cpp
#include"OnPasteCommand.h"
#include"NotePadForm.h"
#include"KeyAction.h"
#include"FactoryProduct.h"
#include"CreateGlyph.h"
#include"CreateProduct.h"
#include"LineChange.h"
#include"SelectWords.h"
#include"KeyAction.h"
#include"FactoryProduct.h"
#include"Row.h"
#include"Scroll.h"
#include<string>
using namespace std;
#pragma warning(disable:4996)

OnPasteCommand::OnPasteCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
	this->isChecked = false;
	//this->notePadForm->isUndo = false;
	//this->notePadForm->isThereSelected = false;
	this->firstX = 0;
	this->firstY = 0;
	this->endX=0;
	this->endY=0;
	this->isLineChangeButtonClicked = false;
	this->character = "";
	this->isRedo = false;
}

OnPasteCommand::~OnPasteCommand() {
}

void OnPasteCommand::Execute() {
	HANDLE hClipboard;
	int currentY;
	int currentX;
	Glyph* row;
	Long rowLength;
	Long strLength;
	int i = 0;
	int j = 0;
	int k;
	char* character_ = NULL;
	char afterCharacter[2];
	Glyph* character = NULL;
	int changedX;
	int changedY;
	BOOL isLineChanged = false;
	BOOL lastLineChanged = false;
	BOOL isThereRestOnes = false;
	int restX;
	int restY;
	int actualRestX;
	int actualRestY;
	int restLength;
	Glyph* currentRow;
	int beforeLineChangeCurrentX;
	int currentY_;
	CreateGlyph* creator = NULL;
	int firstX = this->notePadForm->selectWords->firstX;
	int firstY = this->notePadForm->selectWords->firstY;
	int endX = this->notePadForm->selectWords->endX;
	int endY = this->notePadForm->selectWords->endY;

	creator = new CreateProduct();

	//1.1. 클립보드 핸들을 연다.  
	if (OpenClipboard(this->notePadForm->GetSafeHwnd())) {

		//1.1.1. 클립보드에 텍스트 데이터가 있으면,
		if (IsClipboardFormatAvailable(CF_TEXT)) {

			//클립보드에서 데이터를 얻는다.
			hClipboard = GetClipboardData(CF_TEXT);

			//1.메모리를 잠가놓는다.
			CString str((PSTR)GlobalLock(hClipboard));


			//잡아두었던 메모리를 풀어준다.
			GlobalUnlock(hClipboard);


			//클립보드를 닫는다.
			CloseClipboard();


			//해당 데이터가 NULL이 아니면,

			if (hClipboard != NULL) {


				//(21.11.10.추가)
				if (this->notePadForm->isOpen == TRUE) {
					this->notePadForm->isOpenChanged = TRUE;
				}

				//(21.11.11.추가) 선택된 단어가 있으면, BackSpace부터 호출한다.
				if (firstX > 0 || firstY > 0 || endX > 0 || endY > 0) {
					this->notePadForm->SendMessage(WM_COMMAND, 40013);
				}

				//1.메모리를 잠가놓는다.
				//LPTSTR lptstr = (LPTSTR)GlobalLock(hClipboard);
				//CString str(lptstr);

				//2. 해당 CString 메모리를 char*로 변환한다.
				
				//(21.11.08.추가)
				if (this->isRedo == true) {
					str = this->character.c_str();
				}

				strLength = str.GetLength();
				character_ = new char[strLength + 1];
				strcpy(character_, str);

				this->character = str;

				//(21.11.04.추가) 현재 윈도우 크기를 넘겨준다.
				this->windowWidth = this->notePadForm->scroll->windowWidth;


				//Note에 붙혀넣기

				//2. 현재 캐럿의 위치를 확인한다.
				currentY = this->notePadForm->note->GetCurrent();
				this->notePadForm->row = this->notePadForm->note->GetChild(currentY);
				currentX = this->notePadForm->row->GetCurrent();
				//rowLength = this->notePadForm->note->GetLength();
				rowLength = this->notePadForm->row->GetLength();


				// Paste하기 전 좌표를 구해놓는다.
				this->firstX = currentX;
				this->firstY = currentY;


				//3.(21.08.24 추가) 자동개행이 눌려져 있는 경우, ****************************************
				if (this->notePadForm->isLineChangeButtonClicked == true) {

					this->isLineChangeButtonClicked = true;

					//3.1. 자동개행 전 캐럿 좌표를 찾는다.
					this->notePadForm->lineChange->FindCaretBeforeLineChange(&currentX, &currentY);

					//3.2. 자동개행된 문서를 펼친다. (자동개행버튼 취소했을 때 (OFF))
					this->notePadForm->lineChange->LineChangeButtonNotClicked();

					//3.3. 해당 줄의 length를 찾는다.
					this->notePadForm->row = this->notePadForm->note->GetChild(currentY);
					rowLength = this->notePadForm->row->GetLength();
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
							//afterCharacter[1] = '\0';

							//1.1. 개행문자이면, 

							if (afterCharacter[0] == '\r' && afterCharacter[1] == '\n') {
								afterCharacter[1] = '\0';

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
								i = i + 2;

							}
							else if (afterCharacter[0] == '\n' || afterCharacter[0] == '\r') {

								afterCharacter[1] = '\0';

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
								i++;
							}
							else {
								afterCharacter[1] = '\0';
								//character = new SingleByteCharacter(afterCharacter[0]);
								character = creator->Make(afterCharacter);
								i++;
							}

							//i++;
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

							//1.1. 개행문자이면,
							if (afterCharacter[0] == '\r' && afterCharacter[1] == '\n') {
								//나머지 여분의 글자가 있는지 확인한다.**************************			
								restX = currentX + j;

								if (this->notePadForm->row->GetLength() > restX) {
									actualRestX = restX;
									isThereRestOnes = true;
									restY = currentY;
								}
								//***************************************************************

								//1.1.1. 새로운 문자를 만든다.
								k++;

								//character = creator->Make(afterCharacter);
								this->notePadForm->row = creator->Make(afterCharacter);
								this->notePadForm->note->Add(k, this->notePadForm->row);

								//1.1.3. 조건을 바꾼다.
								isLineChanged = true;
								lastLineChanged = true;
								j = 0;
								l = 0;
								i = i + 2;
							}
							else if (afterCharacter[0] == '\n' || afterCharacter[0] == '\r') {
								//1.0. 첫번째 위치에 널문자를 넣어준다.
								afterCharacter[1] = '\0';
								//나머지 여분의 글자가 있는지 확인한다.**************************			
								restX = currentX + j;

								if (this->notePadForm->row->GetLength() > restX) {
									actualRestX = restX;
									isThereRestOnes = true;
									restY = currentY;
								}
								//***************************************************************

								//1.1.1. 새로운 문자를 만든다.
								k++;

								//character = creator->Make(afterCharacter);
								this->notePadForm->row = creator->Make(afterCharacter);
								this->notePadForm->note->Add(k, this->notePadForm->row);

								//1.1.3. 조건을 바꾼다.
								isLineChanged = true;
								lastLineChanged = true;
								j = 0;
								l = 0;
								i++;
							}

							else {
								//1.0. 첫번째 위치에 널문자를 넣어준다.
								afterCharacter[1] = '\0';
								//character = new SingleByteCharacter(afterCharacter[0]);
								character = creator->Make(afterCharacter);
								i++;
							}


						}

						//2. 2byte인 경우
						else {
							//character = new DoubleByteCharacter(afterCharacter);
							character = creator->Make(afterCharacter);
							i = i + 2;
						}

						//3. 개행문자가 아니면 row에 Add한다.
						if (isLineChanged == false) {
							this->notePadForm->note->GetChild(k);
							this->notePadForm->row->Add(l, character); //row->Add(character);
							//row->MoveCurrent(row->GetLength());
							l++;
							j++;
						}

					}

					//#if 0
					//3.1.2. 끼워적고 난 후, 개행 전 나머지 문자를 개행 후 문자에 붙혀적는다.******************
					if (isThereRestOnes == true) {

						// 나머지 문자 남은 줄의 정보
						this->notePadForm->row = this->notePadForm->note->GetChild(restY);
						rowLength = this->notePadForm->row->GetLength();
						restLength = rowLength - actualRestX;

						// 최근까지의 추가한 줄의 정보
						currentRow = this->notePadForm->note->GetChild(k);

						// 최근까지의 줄에 나머지 줄의 글자를 추가한다.
						while (restLength > 0) {
							character = this->notePadForm->row->GetChild(actualRestX);
							currentRow->Add(character);
							this->notePadForm->row->Remove(actualRestX);
							//j++;
							restLength--;
						}

					}
					//#endif
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

				this->currentX = changedX;
				this->currentY = changedY;
				this->endX = changedX;
				this->endY = changedY;


				//잡아두었던 메모리를 풀어준다.
				//GlobalUnlock(hClipboard);
			}

		}

		//Caret과 Scroll을 업데이트 해준다. *************************************

		// Scroll 업데이트 할 때의 조건을 설정한다.
		this->notePadForm->isJumpOverForPrevious = false;
		this->notePadForm->isJumpOverForNext = false;
		this->notePadForm->isUp = false;
		this->notePadForm->isDown = false;
		this->notePadForm->isDoubleByte = false;


		this->notePadForm->Notify();
		//***********************************************************************

		//클립보드를 닫는다.
		//CloseClipboard();
	}
	if (creator != NULL) {
		delete creator;
		creator = NULL;
	}

	if (character_ != NULL) {
		delete[] character_;
	}


	//this->notePadForm->Invalidate();
}

void OnPasteCommand::Undo() {
	Glyph* row;
	Glyph* row_;
	int windowWidth = this->notePadForm->scroll->windowWidth;
	int currentX = this->currentX;
	int currentY = this->currentY;
	int firstX = this->firstX;
	int firstY = this->firstY;
	int endY = this->endY;
	int endX = this->endX;

	//(21.11.04.추가) 현재 자동개행버튼 상황과 적을때의 상황이 다르면,
	if (this->notePadForm->isLineChangeButtonClicked != this->isLineChangeButtonClicked) {

		//1.1. 현재 자동개행버튼이 눌려져 있으면, 자동개행 후의 좌표를 구한다. 
		if (this->notePadForm->isLineChangeButtonClicked == true) {
			this->notePadForm->lineChange->FindCaretAfterLineChange(currentX, currentY, &currentX, &currentY);
			this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
			this->notePadForm->lineChange->FindCaretAfterLineChange(endX, endY, &endX, &endY);
		}
		//1.2. 현재 자동개행버튼이 안눌려져 있으면, 자동개행 전의 좌표를 구한다.
		else {
			//1.2.1. 자동개행 시켜준다.
			this->notePadForm->lineChange->LineChangeButtonClicked(this->windowWidth);

			//1.2.2. 자동개행 전 좌표 구한다.
			this->notePadForm->lineChange->FindCaretBeforeLineChange(currentX, currentY, &currentX, &currentY);
			this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
			this->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);

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
		this->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);

		//2.4. 자동개행된 문서를 펼친다.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

		//2.5. 펼친 문서를 현재 windowWidth만큼 자동개행한다.
		this->notePadForm->lineChange->LineChangeButtonClicked(windowWidth);

		//2.6. 자동개행 후 좌표를 구한다.
		this->notePadForm->lineChange->FindCaretAfterLineChange(currentX, currentY, &currentX, &currentY);
		this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
		this->notePadForm->lineChange->FindCaretAfterLineChange(endX, endY, &endX, &endY);
	}

	this->isLineChangeButtonClicked = this->notePadForm->isLineChangeButtonClicked;

	//1. firstX, firstY 까지 지운다.(자동개행 되어있으면 자동개행된 좌표)
	this->notePadForm->selectWords->firstX = firstX;
	this->notePadForm->selectWords->firstY = firstY;
	this->notePadForm->selectWords->endX = endX;
	this->notePadForm->selectWords->endY = endY;

	this->currentX = currentX;
	this->currentY = currentY;
	this->firstX = firstX;
	this->firstY = firstY;
	this->endX = endX;
	this->endY = endY;


	//2. endX, endY를 현재 위치로 설정해준다.
	this->notePadForm->note->MoveCurrent(this->endY);
	row = this->notePadForm->note->GetChild(this->endY);

	//(21.11.04.추가, 간혹 발생하는 에러) 만약 this->endY번째의 row가 없으면 length 0개짜리 row만들어서 add해준다.
	if (this->notePadForm->note->GetLength()<= this->endY) { //row == NULL
		row_ = new Row;
		this->notePadForm->note->Add(this->endY, row_);
		row = this->notePadForm->note->GetChild(this->endY);
	}

	row->MoveCurrent(this->endX);


	KeyAction* keyAction = NULL;
	FactoryProduct factoryProduct(this->notePadForm);

	keyAction = factoryProduct.Make(VK_BACK, NULL, NULL);
	keyAction->KeyDown();
}



void OnPasteCommand::Redo() {
	Glyph* row;
	int windowWidth = this->notePadForm->scroll->windowWidth;
	int currentX = this->currentX;
	int currentY = this->currentY;
	int firstX = this->firstX;
	int firstY = this->firstY;
	int endY = this->endY;
	int endX = this->endX;

	//(21.11.04.추가)현재 자동개행버튼 상황과 적을때의 상황이 다르면,
	if (this->notePadForm->isLineChangeButtonClicked != this->isLineChangeButtonClicked) {

		//1.1. 현재 자동개행버튼이 눌려져 있으면, 자동개행 후의 좌표를 구한다. 
		if (this->notePadForm->isLineChangeButtonClicked == true) {
			this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
		}
		//1.2. 현재 자동개행버튼이 안눌려져 있으면, 자동개행 전의 좌표를 구한다.
		else {
			//1.2.1. 자동개행 시켜준다.
			this->notePadForm->lineChange->LineChangeButtonClicked(windowWidth); //this->windowWidth

			//1.2.2. 자동개행 전 좌표 구한다.
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
		this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);

		//2.4. 자동개행된 문서를 펼친다.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

		//2.5. 펼친 문서를 현재 windowWidth만큼 자동개행한다.
		this->notePadForm->lineChange->LineChangeButtonClicked(windowWidth);

		//2.6. 자동개행 후 좌표를 구한다.
		this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
	}

	this->isLineChangeButtonClicked = this->notePadForm->isLineChangeButtonClicked;


	//1. 현재 위치를 firstX,Y로 설정해준다.
	this->firstY = firstY;
	this->firstX = firstX;

	this->notePadForm->note->MoveCurrent(this->firstY);
	row = this->notePadForm->note->GetChild(this->firstY);
	row->MoveCurrent(this->firstX);

	//(21.11.08.추가) 
	this->isRedo = true;


	//2. 다시 Paste한다.
	Execute();

}

Long OnPasteCommand::GetCurrentX() {
	return this->currentX;
}

Long OnPasteCommand::GetCurrentY() {
	return this->currentY;
}

void OnPasteCommand::SetCheck() {
	this->isChecked = true;
}

bool OnPasteCommand::GetChecked() {
	return this->isChecked;
}

Long OnPasteCommand::GetEndX() {
	return this->endX;
}

void OnPasteCommand::UnCheck() {
	this->isChecked = false;
}







