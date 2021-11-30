//PasteText.cpp
#include"PasteText.h"
#include"NotePadForm.h"
#include"SelectWords.h"
#include"Glyph.h"
#include"Composite.h"
#include"Character.h"
#include"Note.h"
#include"Row.h"
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
#include"LineChange.h"
#include<string>
using namespace std;
#pragma warning(disable:4996)

PasteText::PasteText(NotePadForm* notePadForm)
	:KeyAction(notePadForm) {
}

PasteText::~PasteText() {
}

void PasteText::KeyDown() {
	HANDLE hClipboard;
	int currentY;
	int currentX;
	Glyph* row;
	Long rowLength;
	Long strLength;
	int i = 0;
	int j = 0;
	int k;
	char* character_;
	char afterCharacter[2];
	Glyph* character = NULL;
	int changedX;
	int changedY;
	BOOL isLineChanged = FALSE;
	BOOL lastLineChanged = FALSE;
	BOOL isThereRestOnes = FALSE;
	int restX;
	int restY;
	int restLength;
	Glyph* currentRow;
	int beforeLineChangeCurrentX;
	int currentY_;
	CreateGlyph* creator = NULL;

//1. Ctrl + V(붙혀적기)를 눌렀을 경우,
	creator = new CreateProduct();

	//1.1. 클립보드 핸들을 연다.  
	if (OpenClipboard(this->notePadForm->GetSafeHwnd())) {
		//1.1.1. 클립보드에 텍스트 데이터가 있으면,
		if (IsClipboardFormatAvailable(CF_TEXT)) {
			
			//클립보드에서 데이터를 얻는다.
			hClipboard = GetClipboardData(CF_TEXT);

			//해당 데이터가 NULL이 아니면,

			if (hClipboard != NULL) {

				//1.메모리를 잠가놓는다.
				LPTSTR lptstr = (LPTSTR)GlobalLock(hClipboard);
				CString str(lptstr);

				//2. 해당 CString 메모리를 char*로 변환한다.
				strLength = str.GetLength();
				character_ = new char[strLength + 1];
				strcpy(character_, str);


				//Note에 붙혀넣기

				//2. 현재 캐럿의 위치를 확인한다.
				currentY = this->notePadForm->note->GetCurrent();
				this->notePadForm->row = this->notePadForm->note->GetChild(currentY);
				currentX = this->notePadForm->row->GetCurrent();
				//rowLength = this->notePadForm->note->GetLength();
				rowLength = this->notePadForm->row->GetLength();

				//3.(21.08.24 추가) 자동개행이 눌려져 있는 경우, ****************************************
				if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

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
						isLineChanged = FALSE;

						afterCharacter[0] = character_[i];
						afterCharacter[1] = character_[i + 1];

						//1.1byte인 경우,
						if (afterCharacter[0] >= 0 && afterCharacter[0] < 128) {

							//1.0. 첫번째 위치에 널문자를 넣어준다.
							afterCharacter[1] = '\0';

							//1.1. 개행문자이면, 
							if (afterCharacter[0] == '\n') { //afterCharacter[0] == '\r' || 
								//1.1.1. 새로운 문자를 만든다.
								k++;
								
								//row = new Row();
								this->notePadForm->row = creator->Make(afterCharacter);
								this->notePadForm->note->Add(k, this->notePadForm->row);
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
							this->notePadForm->row->Add(character); //row->Add(character); ************************************
							//row->MoveCurrent(row->GetLength());
							j++;
						}

					}

					//3.1.2. 바뀐 좌표를 설정한다.
					changedY = k;
					//changedX = currentX + j;
					if (lastLineChanged == TRUE) {
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
								restX = currentX + j;

								if (this->notePadForm->row->GetLength() > restX) {
									isThereRestOnes = TRUE;
									restY = currentY;
								}
								//***************************************************************

								//1.1.1. 새로운 문자를 만든다.
								k++;

								//row = new Row();
								this->notePadForm->row = creator->Make(afterCharacter);
								this->notePadForm->note->Add(k, this->notePadForm->row);

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
							this->notePadForm->row = this->notePadForm->note->GetChild(k);
							this->notePadForm->row->Add(l, character); //row->Add(character);
							//row->MoveCurrent(row->GetLength());
							l++;
							j++;
						}

					}
					//3.1.2. 끼워적고 난 후, 개행 전 나머지 문자를 개행 후 문자에 붙혀적는다.******************
					if (isThereRestOnes == TRUE) {

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
					if (lastLineChanged == TRUE) {
						changedX = j;
					}
					else {
						changedX = currentX + j;
					}

				}


				//4.(21.08.24 추가) 자동개행이 눌려져 있는 경우, ****************************************
				if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

					//4.1. 펼친 문서를 다시 자동개행한다. (자동개행버튼 눌렀을때(ON)
					this->notePadForm->lineChange->LineChangeButtonClicked();

					//4.2. 자동개행 후 이동할 캐럿의 위치를 구한다.
					this->notePadForm->lineChange->FindCaretAfterLineChange(changedX, changedY);
				}
				//********************************************************************************************
				
				else {
					//5. 좌표를 이동해준다.
					this->notePadForm->note->MoveCurrent(changedY);
					this->notePadForm->row = this->notePadForm->note->GetChild(changedY);
					this->notePadForm->row->MoveCurrent(changedX);
				}



				//잡아두었던 메모리를 풀어준다.
				GlobalUnlock(hClipboard);
			}

		}

		//Caret과 Scroll을 업데이트 해준다. *************************************

		// Scroll 업데이트 할 때의 조건을 설정한다.
		this->notePadForm->isJumpOverForPrevious = FALSE;
		this->notePadForm->isJumpOverForNext = FALSE;
		this->notePadForm->isUp = FALSE;
		this->notePadForm->isDown = FALSE;
		this->notePadForm->isDoubleByte = FALSE;


		this->notePadForm->Notify();
		//***********************************************************************

		//클립보드를 닫는다.
		CloseClipboard();
	}
	if (creator != NULL) {
		delete creator;
		creator = NULL;
	}

	this->notePadForm->Invalidate();
}

















