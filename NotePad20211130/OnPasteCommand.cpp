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

	//1.1. Ŭ������ �ڵ��� ����.  
	if (OpenClipboard(this->notePadForm->GetSafeHwnd())) {

		//1.1.1. Ŭ�����忡 �ؽ�Ʈ �����Ͱ� ������,
		if (IsClipboardFormatAvailable(CF_TEXT)) {

			//Ŭ�����忡�� �����͸� ��´�.
			hClipboard = GetClipboardData(CF_TEXT);

			//1.�޸𸮸� �ᰡ���´�.
			CString str((PSTR)GlobalLock(hClipboard));


			//��Ƶξ��� �޸𸮸� Ǯ���ش�.
			GlobalUnlock(hClipboard);


			//Ŭ�����带 �ݴ´�.
			CloseClipboard();


			//�ش� �����Ͱ� NULL�� �ƴϸ�,

			if (hClipboard != NULL) {


				//(21.11.10.�߰�)
				if (this->notePadForm->isOpen == TRUE) {
					this->notePadForm->isOpenChanged = TRUE;
				}

				//(21.11.11.�߰�) ���õ� �ܾ ������, BackSpace���� ȣ���Ѵ�.
				if (firstX > 0 || firstY > 0 || endX > 0 || endY > 0) {
					this->notePadForm->SendMessage(WM_COMMAND, 40013);
				}

				//1.�޸𸮸� �ᰡ���´�.
				//LPTSTR lptstr = (LPTSTR)GlobalLock(hClipboard);
				//CString str(lptstr);

				//2. �ش� CString �޸𸮸� char*�� ��ȯ�Ѵ�.
				
				//(21.11.08.�߰�)
				if (this->isRedo == true) {
					str = this->character.c_str();
				}

				strLength = str.GetLength();
				character_ = new char[strLength + 1];
				strcpy(character_, str);

				this->character = str;

				//(21.11.04.�߰�) ���� ������ ũ�⸦ �Ѱ��ش�.
				this->windowWidth = this->notePadForm->scroll->windowWidth;


				//Note�� �����ֱ�

				//2. ���� ĳ���� ��ġ�� Ȯ���Ѵ�.
				currentY = this->notePadForm->note->GetCurrent();
				this->notePadForm->row = this->notePadForm->note->GetChild(currentY);
				currentX = this->notePadForm->row->GetCurrent();
				//rowLength = this->notePadForm->note->GetLength();
				rowLength = this->notePadForm->row->GetLength();


				// Paste�ϱ� �� ��ǥ�� ���س��´�.
				this->firstX = currentX;
				this->firstY = currentY;


				//3.(21.08.24 �߰�) �ڵ������� ������ �ִ� ���, ****************************************
				if (this->notePadForm->isLineChangeButtonClicked == true) {

					this->isLineChangeButtonClicked = true;

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
						isLineChanged = false;

						afterCharacter[0] = character_[i];
						afterCharacter[1] = character_[i + 1];

						//1.1byte�� ���,
						if (afterCharacter[0] >= 0 && afterCharacter[0] < 128) {

							//1.0. ù��° ��ġ�� �ι��ڸ� �־��ش�.
							//afterCharacter[1] = '\0';

							//1.1. ���๮���̸�, 

							if (afterCharacter[0] == '\r' && afterCharacter[1] == '\n') {
								afterCharacter[1] = '\0';

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
								i = i + 2;

							}
							else if (afterCharacter[0] == '\n' || afterCharacter[0] == '\r') {

								afterCharacter[1] = '\0';

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

							//1.1. ���๮���̸�,
							if (afterCharacter[0] == '\r' && afterCharacter[1] == '\n') {
								//������ ������ ���ڰ� �ִ��� Ȯ���Ѵ�.**************************			
								restX = currentX + j;

								if (this->notePadForm->row->GetLength() > restX) {
									actualRestX = restX;
									isThereRestOnes = true;
									restY = currentY;
								}
								//***************************************************************

								//1.1.1. ���ο� ���ڸ� �����.
								k++;

								//character = creator->Make(afterCharacter);
								this->notePadForm->row = creator->Make(afterCharacter);
								this->notePadForm->note->Add(k, this->notePadForm->row);

								//1.1.3. ������ �ٲ۴�.
								isLineChanged = true;
								lastLineChanged = true;
								j = 0;
								l = 0;
								i = i + 2;
							}
							else if (afterCharacter[0] == '\n' || afterCharacter[0] == '\r') {
								//1.0. ù��° ��ġ�� �ι��ڸ� �־��ش�.
								afterCharacter[1] = '\0';
								//������ ������ ���ڰ� �ִ��� Ȯ���Ѵ�.**************************			
								restX = currentX + j;

								if (this->notePadForm->row->GetLength() > restX) {
									actualRestX = restX;
									isThereRestOnes = true;
									restY = currentY;
								}
								//***************************************************************

								//1.1.1. ���ο� ���ڸ� �����.
								k++;

								//character = creator->Make(afterCharacter);
								this->notePadForm->row = creator->Make(afterCharacter);
								this->notePadForm->note->Add(k, this->notePadForm->row);

								//1.1.3. ������ �ٲ۴�.
								isLineChanged = true;
								lastLineChanged = true;
								j = 0;
								l = 0;
								i++;
							}

							else {
								//1.0. ù��° ��ġ�� �ι��ڸ� �־��ش�.
								afterCharacter[1] = '\0';
								//character = new SingleByteCharacter(afterCharacter[0]);
								character = creator->Make(afterCharacter);
								i++;
							}


						}

						//2. 2byte�� ���
						else {
							//character = new DoubleByteCharacter(afterCharacter);
							character = creator->Make(afterCharacter);
							i = i + 2;
						}

						//3. ���๮�ڰ� �ƴϸ� row�� Add�Ѵ�.
						if (isLineChanged == false) {
							this->notePadForm->note->GetChild(k);
							this->notePadForm->row->Add(l, character); //row->Add(character);
							//row->MoveCurrent(row->GetLength());
							l++;
							j++;
						}

					}

					//#if 0
					//3.1.2. �������� �� ��, ���� �� ������ ���ڸ� ���� �� ���ڿ� �������´�.******************
					if (isThereRestOnes == true) {

						// ������ ���� ���� ���� ����
						this->notePadForm->row = this->notePadForm->note->GetChild(restY);
						rowLength = this->notePadForm->row->GetLength();
						restLength = rowLength - actualRestX;

						// �ֱٱ����� �߰��� ���� ����
						currentRow = this->notePadForm->note->GetChild(k);

						// �ֱٱ����� �ٿ� ������ ���� ���ڸ� �߰��Ѵ�.
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

				this->currentX = changedX;
				this->currentY = changedY;
				this->endX = changedX;
				this->endY = changedY;


				//��Ƶξ��� �޸𸮸� Ǯ���ش�.
				//GlobalUnlock(hClipboard);
			}

		}

		//Caret�� Scroll�� ������Ʈ ���ش�. *************************************

		// Scroll ������Ʈ �� ���� ������ �����Ѵ�.
		this->notePadForm->isJumpOverForPrevious = false;
		this->notePadForm->isJumpOverForNext = false;
		this->notePadForm->isUp = false;
		this->notePadForm->isDown = false;
		this->notePadForm->isDoubleByte = false;


		this->notePadForm->Notify();
		//***********************************************************************

		//Ŭ�����带 �ݴ´�.
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

	//(21.11.04.�߰�) ���� �ڵ������ư ��Ȳ�� �������� ��Ȳ�� �ٸ���,
	if (this->notePadForm->isLineChangeButtonClicked != this->isLineChangeButtonClicked) {

		//1.1. ���� �ڵ������ư�� ������ ������, �ڵ����� ���� ��ǥ�� ���Ѵ�. 
		if (this->notePadForm->isLineChangeButtonClicked == true) {
			this->notePadForm->lineChange->FindCaretAfterLineChange(currentX, currentY, &currentX, &currentY);
			this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
			this->notePadForm->lineChange->FindCaretAfterLineChange(endX, endY, &endX, &endY);
		}
		//1.2. ���� �ڵ������ư�� �ȴ����� ������, �ڵ����� ���� ��ǥ�� ���Ѵ�.
		else {
			//1.2.1. �ڵ����� �����ش�.
			this->notePadForm->lineChange->LineChangeButtonClicked(this->windowWidth);

			//1.2.2. �ڵ����� �� ��ǥ ���Ѵ�.
			this->notePadForm->lineChange->FindCaretBeforeLineChange(currentX, currentY, &currentX, &currentY);
			this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
			this->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);

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
		this->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);

		//2.4. �ڵ������ ������ ��ģ��.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

		//2.5. ��ģ ������ ���� windowWidth��ŭ �ڵ������Ѵ�.
		this->notePadForm->lineChange->LineChangeButtonClicked(windowWidth);

		//2.6. �ڵ����� �� ��ǥ�� ���Ѵ�.
		this->notePadForm->lineChange->FindCaretAfterLineChange(currentX, currentY, &currentX, &currentY);
		this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
		this->notePadForm->lineChange->FindCaretAfterLineChange(endX, endY, &endX, &endY);
	}

	this->isLineChangeButtonClicked = this->notePadForm->isLineChangeButtonClicked;

	//1. firstX, firstY ���� �����.(�ڵ����� �Ǿ������� �ڵ������ ��ǥ)
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


	//2. endX, endY�� ���� ��ġ�� �������ش�.
	this->notePadForm->note->MoveCurrent(this->endY);
	row = this->notePadForm->note->GetChild(this->endY);

	//(21.11.04.�߰�, ��Ȥ �߻��ϴ� ����) ���� this->endY��°�� row�� ������ length 0��¥�� row���� add���ش�.
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

	//(21.11.04.�߰�)���� �ڵ������ư ��Ȳ�� �������� ��Ȳ�� �ٸ���,
	if (this->notePadForm->isLineChangeButtonClicked != this->isLineChangeButtonClicked) {

		//1.1. ���� �ڵ������ư�� ������ ������, �ڵ����� ���� ��ǥ�� ���Ѵ�. 
		if (this->notePadForm->isLineChangeButtonClicked == true) {
			this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
		}
		//1.2. ���� �ڵ������ư�� �ȴ����� ������, �ڵ����� ���� ��ǥ�� ���Ѵ�.
		else {
			//1.2.1. �ڵ����� �����ش�.
			this->notePadForm->lineChange->LineChangeButtonClicked(windowWidth); //this->windowWidth

			//1.2.2. �ڵ����� �� ��ǥ ���Ѵ�.
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
		this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);

		//2.4. �ڵ������ ������ ��ģ��.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

		//2.5. ��ģ ������ ���� windowWidth��ŭ �ڵ������Ѵ�.
		this->notePadForm->lineChange->LineChangeButtonClicked(windowWidth);

		//2.6. �ڵ����� �� ��ǥ�� ���Ѵ�.
		this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
	}

	this->isLineChangeButtonClicked = this->notePadForm->isLineChangeButtonClicked;


	//1. ���� ��ġ�� firstX,Y�� �������ش�.
	this->firstY = firstY;
	this->firstX = firstX;

	this->notePadForm->note->MoveCurrent(this->firstY);
	row = this->notePadForm->note->GetChild(this->firstY);
	row->MoveCurrent(this->firstX);

	//(21.11.08.�߰�) 
	this->isRedo = true;


	//2. �ٽ� Paste�Ѵ�.
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







