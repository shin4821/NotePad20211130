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

//1. Ctrl + V(��������)�� ������ ���,
	creator = new CreateProduct();

	//1.1. Ŭ������ �ڵ��� ����.  
	if (OpenClipboard(this->notePadForm->GetSafeHwnd())) {
		//1.1.1. Ŭ�����忡 �ؽ�Ʈ �����Ͱ� ������,
		if (IsClipboardFormatAvailable(CF_TEXT)) {
			
			//Ŭ�����忡�� �����͸� ��´�.
			hClipboard = GetClipboardData(CF_TEXT);

			//�ش� �����Ͱ� NULL�� �ƴϸ�,

			if (hClipboard != NULL) {

				//1.�޸𸮸� �ᰡ���´�.
				LPTSTR lptstr = (LPTSTR)GlobalLock(hClipboard);
				CString str(lptstr);

				//2. �ش� CString �޸𸮸� char*�� ��ȯ�Ѵ�.
				strLength = str.GetLength();
				character_ = new char[strLength + 1];
				strcpy(character_, str);


				//Note�� �����ֱ�

				//2. ���� ĳ���� ��ġ�� Ȯ���Ѵ�.
				currentY = this->notePadForm->note->GetCurrent();
				this->notePadForm->row = this->notePadForm->note->GetChild(currentY);
				currentX = this->notePadForm->row->GetCurrent();
				//rowLength = this->notePadForm->note->GetLength();
				rowLength = this->notePadForm->row->GetLength();

				//3.(21.08.24 �߰�) �ڵ������� ������ �ִ� ���, ****************************************
				if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

					//3.1. �ڵ����� �� ĳ�� ��ǥ�� ã�´�.
					this->notePadForm->lineChange->FindCaretBeforeLineChange(&currentX, &currentY);

					//3.2. �ڵ������ ������ ��ģ��. (�ڵ������ư ������� �� (OFF))
					this->notePadForm->lineChange->LineChangeButtonNotClicked();

					//3.3. �ش� ���� length�� ã�´�.
					this->notePadForm->row = this->notePadForm->note->GetChild(currentY);
					rowLength = this->notePadForm->row->GetLength();
				}
				//****************************************************************************************
				//3.1. ���������� ���
				if (currentX >= rowLength) {

					k = currentY;
					this->notePadForm->row = this->notePadForm->note->GetChild(k);

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
							if (afterCharacter[0] == '\n') { //afterCharacter[0] == '\r' || 
								//1.1.1. ���ο� ���ڸ� �����.
								k++;
								
								//row = new Row();
								this->notePadForm->row = creator->Make(afterCharacter);
								this->notePadForm->note->Add(k, this->notePadForm->row);
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
							this->notePadForm->row->Add(character); //row->Add(character); ************************************
							//row->MoveCurrent(row->GetLength());
							j++;
						}

					}

					//3.1.2. �ٲ� ��ǥ�� �����Ѵ�.
					changedY = k;
					//changedX = currentX + j;
					if (lastLineChanged == TRUE) {
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
								restX = currentX + j;

								if (this->notePadForm->row->GetLength() > restX) {
									isThereRestOnes = TRUE;
									restY = currentY;
								}
								//***************************************************************

								//1.1.1. ���ο� ���ڸ� �����.
								k++;

								//row = new Row();
								this->notePadForm->row = creator->Make(afterCharacter);
								this->notePadForm->note->Add(k, this->notePadForm->row);

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
							this->notePadForm->row = this->notePadForm->note->GetChild(k);
							this->notePadForm->row->Add(l, character); //row->Add(character);
							//row->MoveCurrent(row->GetLength());
							l++;
							j++;
						}

					}
					//3.1.2. �������� �� ��, ���� �� ������ ���ڸ� ���� �� ���ڿ� �������´�.******************
					if (isThereRestOnes == TRUE) {

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
					if (lastLineChanged == TRUE) {
						changedX = j;
					}
					else {
						changedX = currentX + j;
					}

				}


				//4.(21.08.24 �߰�) �ڵ������� ������ �ִ� ���, ****************************************
				if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

					//4.1. ��ģ ������ �ٽ� �ڵ������Ѵ�. (�ڵ������ư ��������(ON)
					this->notePadForm->lineChange->LineChangeButtonClicked();

					//4.2. �ڵ����� �� �̵��� ĳ���� ��ġ�� ���Ѵ�.
					this->notePadForm->lineChange->FindCaretAfterLineChange(changedX, changedY);
				}
				//********************************************************************************************
				
				else {
					//5. ��ǥ�� �̵����ش�.
					this->notePadForm->note->MoveCurrent(changedY);
					this->notePadForm->row = this->notePadForm->note->GetChild(changedY);
					this->notePadForm->row->MoveCurrent(changedX);
				}



				//��Ƶξ��� �޸𸮸� Ǯ���ش�.
				GlobalUnlock(hClipboard);
			}

		}

		//Caret�� Scroll�� ������Ʈ ���ش�. *************************************

		// Scroll ������Ʈ �� ���� ������ �����Ѵ�.
		this->notePadForm->isJumpOverForPrevious = FALSE;
		this->notePadForm->isJumpOverForNext = FALSE;
		this->notePadForm->isUp = FALSE;
		this->notePadForm->isDown = FALSE;
		this->notePadForm->isDoubleByte = FALSE;


		this->notePadForm->Notify();
		//***********************************************************************

		//Ŭ�����带 �ݴ´�.
		CloseClipboard();
	}
	if (creator != NULL) {
		delete creator;
		creator = NULL;
	}

	this->notePadForm->Invalidate();
}

















