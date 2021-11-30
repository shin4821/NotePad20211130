//OnImeCompositionCommand.cpp
#include"OnImeCompositionCommand.h"
#include"NotePadForm.h"
#include"Glyph.h"
#include"Composite.h"
#include"Character.h"
#include"Note.h"
#include"Row.h"
#include"SingleByteCharacter.h"
#include"DoubleByteCharacter.h"
#include"CreateGlyph.h"
#include"CreateProduct.h"
#include"LineChange.h"
#include"Scroll.h"
#include"KeyAction.h"
#include"BackSpaceKeyAction.h"
#include"FactoryProduct.h"
#include"CommandStory.h"
#include"UndoList.h"
#include"RedoList.h"
#include"resource.h"
#include"Font.h"
#include"DummyRow.h"
#include"GetTextSize.h"
using namespace std;


OnImeCompositionCommand::OnImeCompositionCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {

	this->isChecked = false;
	this->isLineChanged = false;
	this->currentX = 0;
	this->currentY = 0;
	//this->character[2] = '\0';
	this->notePadForm->isRedo = false;
}

OnImeCompositionCommand::~OnImeCompositionCommand() {
}

void OnImeCompositionCommand::Execute() {

	char letter[3] = { '\0', };
	string fontName = this->notePadForm->font->GetFontName();
	int fontSize = this->notePadForm->font->GetFontSize();
	Long length;
	Long i = 0;
	string strCharacter;
	char chaCharacter[3];
	char characterMove[3];
	Long caretX;
	Long caretY;
	Glyph* dummyRow = NULL;
	BOOL isLineChanged = FALSE;
	int location;
	Glyph* row;
	int exceptWidth;
	BOOL isLastOneDeleted = FALSE;

	//(21.11.10.�߰�)
	if (this->notePadForm->isOpen == TRUE) {
		this->notePadForm->isOpenChanged = TRUE;
	}

#if 0
	letter[0] = HIBYTE(LOWORD(this->notePadForm->letter));
	letter[1] = LOBYTE(LOWORD(this->notePadForm->letter));
	letter[2] = '\0';
#endif

	if (this->notePadForm->letter != "") {
		letter[0] = this->notePadForm->letter.at(0);
		letter[1] = this->notePadForm->letter.at(1);
		letter[2] = '\0';
	}
	else {
		this->notePadForm->isFirstComposing = FALSE; //�ι����϶� ��� ���� �����̴ϱ� FALSE ó��.
		isLastOneDeleted = TRUE;
	}

#if 0
	//(21.09.14 �߰�) Delete�Ҷ� null ������ X.
	if (letter[0] != '\0') {
		this->character = letter;

		CreateGlyph* creator = new CreateProduct();
		Glyph* character = creator->Make(letter);

		//21.08.03 �߰�
		//this->scroll->windowWidth = this->m_rectCurHist.right - this->m_rectCurHist.left;
		//this->scroll->windowHeight = this->m_rectCurHist.bottom - this->m_rectCurHist.top;


		//1. �������� ��
		  //1.1. ���� ĳ���� ��ġ�� Ȯ���Ѵ�.
		int currentY = this->notePadForm->note->GetCurrent();
		this->notePadForm->row = this->notePadForm->note->GetChild(currentY);
		int currentX = this->notePadForm->row->GetCurrent();
		length = this->notePadForm->row->GetLength();


		//Command�� ��� �����Ѵ�. (21.09.06)*********************************************************

		//1. ���� ĳ���� ��ġ�� ����� �����Ѵ�.
		this->currentX = currentX;
		this->currentY = currentY;
		//********************************************************************************************
		this->notePadForm->isFirstComposingForCommand = this->notePadForm->isFirstComposing;

		//2. �ڵ������ư�� ������ ���,
		if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

			//2.1. �ڵ����� �� ĳ���� ��ġ�� ã�´�.
			this->notePadForm->lineChange->FindCaretBeforeLineChange(&currentX, &currentY);

			//2.2. �ڵ����� �� ������ ��ģ��.
			this->notePadForm->lineChange->LineChangeButtonNotClicked();

			this->notePadForm->row = this->notePadForm->note->GetChild(currentY);
			length = this->notePadForm->row->GetLength();
		}

		//3. ���� ������ ���� �ܿ� ���� ���ڸ� �����.
		if (this->notePadForm->isFirstComposing == FALSE) {
			this->notePadForm->row->Remove(currentX); //this->index
		}

		//4. currentX�� row�� length-1���� �۰ų�, currentX�� length-1���� ũ�ų� ����, Ŀ�� ũ�Ⱑ 3�� ���,
		if (currentX < length - 1) {
			//|| (currentX >= length - 1 && this->caretWidth == 3) ��û���. �ٽ� ������.

			//4.1. ���� �ٿ� ���� ���ڸ� �������´�.
			this->notePadForm->row->Add(currentX, character);
		}

		//5. currentX�� row�� length���� ũ�ų� ���ų�, currentX�� length-1���� ũ�ų� ����, Ŀ�� ũ�Ⱑ 3���� ū ���,
		else if (currentX >= length || (currentX >= length - 1 && this->notePadForm->caretWidth > 3)) {
			//5.1. ���� �ٿ� ���� ���ڸ� �������´�.
			this->notePadForm->row->Add(character);
		}

		//6.  �ڵ����� ��ư�� ������ ���,
		if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
			//6.1. ��ģ ������ �ٽ� �ڵ������Ѵ�. (ON)
			this->notePadForm->lineChange->LineChangeButtonClicked();

			//6.2. �ڵ����� �� �̵��� ĳ���� ��ġ�� ���Ѵ�.
			this->notePadForm->lineChange->FindCaretAfterLineChange(currentX, currentY);
		}

		//7. ��ǥ �̵����� �ʰ�, Ŀ�� ũ�⸸ �ٲ��ش�.
		if (this->notePadForm->isFirstComposing == TRUE) {

			this->notePadForm->isOnImeCharForMove = FALSE;
			this->notePadForm->isFirstComposingForMove = this->notePadForm->isFirstComposing;

			// ���� �����ֱ�
			this->notePadForm->isDoubleByte = TRUE;
			this->notePadForm->isJumpOverForPrevious = FALSE;
			this->notePadForm->isJumpOverForNext = FALSE;
			this->notePadForm->isUp = FALSE;
			this->notePadForm->isDown = FALSE;


			this->notePadForm->Notify();
		}


		this->notePadForm->isFirstComposing = FALSE;
		this->notePadForm->isChanged = TRUE;

		this->notePadForm->Invalidate();
	}
#endif

//#if 0
	this->character = letter;

	//CreateGlyph* creator = new CreateProduct();
	CreateProduct creator;
	Glyph* character;

	//21.08.03 �߰�
	//this->scroll->windowWidth = this->m_rectCurHist.right - this->m_rectCurHist.left;
	//this->scroll->windowHeight = this->m_rectCurHist.bottom - this->m_rectCurHist.top;


	//1. �������� ��
	  //1.1. ���� ĳ���� ��ġ�� Ȯ���Ѵ�.
	int currentY = this->notePadForm->note->GetCurrent();
	this->notePadForm->row = this->notePadForm->note->GetChild(currentY);
	int currentX = this->notePadForm->row->GetCurrent();
	length = this->notePadForm->row->GetLength();


	//Command�� ��� �����Ѵ�. (21.09.06)*********************************************************

    //1. ���� ĳ���� ��ġ�� ����� �����Ѵ�.
	this->currentX = currentX;
	this->currentY = currentY;
	//********************************************************************************************
	this->notePadForm->isFirstComposingForCommand = this->notePadForm->isFirstComposing;

	//2. �ڵ������ư�� ������ ���,
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		//2.1. �ڵ����� �� ĳ���� ��ġ�� ã�´�.
		this->notePadForm->lineChange->FindCaretBeforeLineChange(&currentX, &currentY);

		//2.2. �ڵ����� �� ������ ��ģ��.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

		this->notePadForm->row = this->notePadForm->note->GetChild(currentY);
		length = this->notePadForm->row->GetLength();
	}

	//3. ���� ������ ���� �ܿ� ���� ���ڸ� �����.
	if (this->notePadForm->isFirstComposing == FALSE) {
		this->notePadForm->row->Remove(currentX); //this->index
	}

	//4. currentX�� row�� length-1���� �۰ų�, currentX�� length-1���� ũ�ų� ����, Ŀ�� ũ�Ⱑ 3�� ���,
	if (currentX < length && letter[0]!='\0') { //currentX < length - 1
		//|| (currentX >= length - 1 && this->caretWidth == 3) ��û���. �ٽ� ������.
		character = creator.Make(letter);

		//4.1. ���� �ٿ� ���� ���ڸ� �������´�.
		this->notePadForm->row->Add(currentX, character);
	}

	//5. currentX�� row�� length���� ũ�ų� ���ų�, currentX�� length-1���� ũ�ų� ����, Ŀ�� ũ�Ⱑ 3���� ū ���,
	else if (currentX >= length || (currentX >= length - 1 && this->notePadForm->caretWidth > 3) && letter[0] != '\0') {
		character = creator.Make(letter);
		//5.1. ���� �ٿ� ���� ���ڸ� �������´�.
		this->notePadForm->row->Add(character);
	}

	//6.  �ڵ����� ��ư�� ������ ���,
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
		//6.1. ��ģ ������ �ٽ� �ڵ������Ѵ�. (ON)
		this->notePadForm->lineChange->LineChangeButtonClicked();


		//(21.10.28.�߰�)�ƿ� ���ﶧ�� �߰��Ҷ� �����ؼ� ĳ���� ��ġ�� ���ؾ߰ڴ�.
		if (this->notePadForm->letter != "") { //�߰��ϴ� ���,

			//6.2. �ڵ����� �� �̵��� ĳ���� ��ġ�� ���Ѵ�.
			this->notePadForm->lineChange->FindCaretAfterLineChangeForFirst(currentX, currentY);
		}
		else { //���� ���,

			//6.3. �ڵ����� �� �̵��� ĳ���� ��ġ�� ���Ѵ�.
			this->notePadForm->lineChange->FindCaretAfterLineChange(currentX, currentY);
		}


	}

	//7. ��ǥ �̵����� �ʰ�, Ŀ�� ũ�⸸ �ٲ��ش�. 
	if (this->notePadForm->isFirstComposing == TRUE) {
#if 0

		//7.0. (21.10.01 �߰�) �ڵ������� ���, ��ǥ �Ű��ش�.*******************************
		if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
			this->notePadForm->note->MoveCurrent(currentY);
			row = this->notePadForm->note->GetChild(currentY);
			row->MoveCurrent(currentX);
		}
		//***********************************************************************************
#endif
		this->notePadForm->isOnImeCharForMove = FALSE;
		this->notePadForm->isFirstComposingForMove = this->notePadForm->isFirstComposing;

		//7.1. ���� �ϳ� ������ �� ���� ���, isFirstComposingForMove�� FALSE�� �����ش�.
#if 0
		if (this->notePadForm->letter != "") {
			this->notePadForm->isFirstComposingForMove = FALSE;
		}
#endif

		//7.2. Notify ���� �����Ѵ�.
		this->notePadForm->isDoubleByte = TRUE;
		this->notePadForm->isJumpOverForPrevious = FALSE;
		this->notePadForm->isJumpOverForNext = FALSE;
		this->notePadForm->isUp = FALSE;
		this->notePadForm->isDown = FALSE;

		int testCurrentY = this->notePadForm->note->GetCurrent();
		Glyph* testRow = this->notePadForm->note->GetChild(testCurrentY);
		int testCurrentX = testRow->GetCurrent();


		this->notePadForm->isComposition = TRUE;
		this->notePadForm->Notify();
	}
//#if 0
	else if (isLastOneDeleted == TRUE) {
		
		this->notePadForm->isOnImeCharForMove = FALSE;
		this->notePadForm->isFirstComposingForMove = FALSE;
		this->notePadForm->isDoubleByte = FALSE;

		//7.2. Notify ���� �����Ѵ�.
		this->notePadForm->isDoubleByte = TRUE;
		this->notePadForm->isJumpOverForPrevious = FALSE;
		this->notePadForm->isJumpOverForNext = FALSE;
		this->notePadForm->isUp = FALSE;
		this->notePadForm->isDown = FALSE;

		this->notePadForm->Notify();
	}
//#endif

	//(21.10.01 �߰�)
	if (letter[0] == '\0') { //letter�� null�� ���, �����ٶ��̹Ƿ� isFirstComposing�� TRUE�� �ؾ� �ٽ� ���� ��������.
		this->notePadForm->isFirstComposing = TRUE;
		this->notePadForm->isDoubleByte = FALSE;
	}
	else {
		this->notePadForm->isFirstComposing = FALSE;
	}

	this->notePadForm->isChanged = TRUE;

	this->notePadForm->Invalidate();
//#endif


#if 0
	//1.2. ���� ������ ���� �ܿ� ���� ���ڸ� �����.
	if (this->notePadForm->isFirstComposing == FALSE) {
		this->notePadForm->row->Remove(currentX); //this->index
	}

	//1.3. (21.08.12 �߰�) �ڵ������ư OFF�� ���,
	if (this->notePadForm->isLineChangeButtonClicked == FALSE) {
		//1.3. currentX�� row�� length-1���� �۰ų�, currentX�� length-1���� ũ�ų� ����, Ŀ�� ũ�Ⱑ 3�� ���,
		if (currentX < length - 1) {
			//|| (currentX >= length - 1 && this->caretWidth == 3) ��û���. �ٽ� ������.

			//1.3.1. ���� �ٿ� ���� ���ڸ� �������´�.
			this->notePadForm->row->Add(currentX, character);

			this->notePadForm->isInsert = TRUE;
			this->notePadForm->isAppend = FALSE;
			this->notePadForm->isMoveActionVertical = FALSE;
			this->notePadForm->isMoveActionHorizon = FALSE;
			this->notePadForm->isDeleteAction = FALSE;
		}

		//1.4. currentX�� row�� length���� ũ�ų� ���ų�, currentX�� length-1���� ũ�ų� ����, Ŀ�� ũ�Ⱑ 3���� ū ���,
		else if (currentX >= length || (currentX >= length - 1 && this->notePadForm->caretWidth > 3)) {
			//1.4.1. ���� �ٿ� ���� ���ڸ� �������´�.
			this->notePadForm->row->Add(character);

			this->notePadForm->isInsert = FALSE;
			this->notePadForm->isAppend = TRUE;
			this->notePadForm->isMoveActionVertical = FALSE;
			this->notePadForm->isMoveActionHorizon = FALSE;
		}

		//1.5. ��ǥ �̵����� �ʰ�, Ŀ�� ũ�⸸ �ٲ��ش�.
		if (this->notePadForm->isFirstComposing == TRUE) {

			this->notePadForm->isOnImeCharForMove = FALSE;
			this->notePadForm->isFirstComposingForMove = this->notePadForm->isFirstComposing;

			this->notePadForm->Notify();
		}

	}

	//1.4. (21.08.12 �߰�) �ڵ������ư ON�� ���,
	else if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
		//1.4.0. ���������� ���,
		if (currentY + 1 < this->notePadForm->note->GetLength()) {
			dummyRow = this->notePadForm->note->GetChild(currentY + 1);
		}
		else {
			dummyRow = NULL;
		}
		//1.4.1. currentX�� row�� length-1���� �۰ų�, currentX�� length-1���� ũ�ų� ����, Ŀ�� ũ�Ⱑ 3�� ���,(��������)
		if (currentX < length || (dummyRow != NULL && dynamic_cast<DummyRow*>(dummyRow))) { //currentX < length - 1

			row = this->notePadForm->note->GetChild(currentY);

			//1.4.1.1. ���ǿ� ���� ĳ���� ������ �����Ѵ�.
			if (currentX < length) { //length - 1
				isLineChanged = FALSE;
			}
			else if (dummyRow != NULL && dynamic_cast<DummyRow*>(dummyRow)) {
				isLineChanged = TRUE;
			}

			//1.4.1.2. ���� �ٿ� ���� ���ڸ� �������´�.
			row->Add(currentX, character);

			//(21.08.13. ���ܻ�Ȳ �߰�)********************************************
			//1. ���������ε� (dummy�� dummy����)
			//2. �� �� Ŀ�� ��ġ (currentX = row->GetLength())
			//3. �̴�� ���� ������ ������ isLineChanged = TRUE ������,
			//4. ������ ��쵵 �ִ�. ��, ���������µ��� windowWidth�� �ȳѱ�� ���
			//5. �׷��� �������� ��, width �׽�Ʈ ���ִ� ���� �߰�..

			exceptWidth = this->notePadForm->getTextSize->totalCalculateX(row->GetLength(), row);
			if (isLineChanged == TRUE && exceptWidth <= this->notePadForm->scroll->windowWidth) {
				isLineChanged = FALSE;
			}
			//**********************************************************************

			//1.4.1.3. ���� ������ ������ ���,
			if (this->notePadForm->isFirstComposing == TRUE) {

				//�ش� row���� �ڸ� ��ġ�� 0���� ũ�ų� �������� �ݺ��Ѵ�.
				location = this->notePadForm->lineChange->FindSplitLocationByWriting(row);

				i = currentY;
				while (location >= 0) {
					//1. �ش� row�� ���� row�� dummyRow�� �ƴϰų� ���� ���,dummyRow �����.
					if (i + 1 < this->notePadForm->note->GetLength()) {

						dummyRow = this->notePadForm->note->GetChild(i + 1);

						if (!dynamic_cast<DummyRow*>(dummyRow)) {
							dummyRow = new DummyRow();
							this->notePadForm->note->Add(i + 1, dummyRow);
						}
					}
					else {

						dummyRow = new DummyRow();
						this->notePadForm->note->Add(dummyRow);
					}

					//2. SplitForFront�� ȣ���Ѵ�.
					dummyRow = row->SplitForFront(dummyRow, location);

					//3. ��¹��� dummyRow���� �ڸ� ��ġ�� �ִ��� ã�´�.
					location = this->notePadForm->lineChange->FindSplitLocationByWriting(dummyRow);
					row = dummyRow;
					i++;
				}

				//#if 0
								  // ĳ�� ��ǥ�� �����Ѵ�.
				if (isLineChanged == TRUE) { //���� �ٲ�����,
					this->notePadForm->note->MoveCurrent(currentY + 1);
					this->notePadForm->row = this->notePadForm->note->GetChild(currentY + 1);
					this->notePadForm->row->MoveCurrent(0); // �ѱ��̹Ƿ� 0���� ����.

				}
				//#endif
			}

		}

		//1.4. currentX�� row�� length���� ũ�ų� ���ų�, currentX�� length-1���� ũ�ų� ����, Ŀ�� ũ�Ⱑ 3���� ū ���,
		else if (currentX >= length || (currentX >= length - 1 && this->notePadForm->caretWidth > 3)) {
			//1.4.1. ���� �ٿ� ���� ���ڸ� �������´�.
			this->notePadForm->row->Add(character);

			//1.4.2. ���� ������ ������ ���,
			if (this->notePadForm->isFirstComposing == TRUE) {
				//1.4.2.1. �ش� row���� �ڸ� ��ġ�� 0���� ũ�ų� ���� ���,
				location = this->notePadForm->lineChange->FindSplitLocationByWriting(this->notePadForm->row);
				if (location >= 0) {
					//1. dummyRow�� �����.
					dummyRow = new DummyRow();

					//2. dummyRow�� �߰��Ѵ�.
					character = this->notePadForm->row->GetChild(location);
					dummyRow->Add(character);

					//3. currentY+1�� ��Ʈ�� �����.
					this->notePadForm->note->Add(currentY + 1, dummyRow);

					//4. ���� row���� ����
					this->notePadForm->row->Remove(location);

					//5. ĳ���� (currentY+1, 0)���� ��ǥ �����Ѵ�.
					this->notePadForm->note->MoveCurrent(currentY + 1);
					dummyRow->MoveCurrent(0);
				}
			}
		}

		//1.5. ��ǥ �̵����� �ʰ�, Ŀ�� ũ�⸸ �ٲ��ش�.
		if (this->notePadForm->isFirstComposing == TRUE) {
			// ��ũ�� ����
			this->notePadForm->isInsert = FALSE;
			this->notePadForm->isAppend = FALSE;
			this->notePadForm->isMoveActionVertical = FALSE;
			this->notePadForm->isMoveActionHorizon = FALSE;
			this->notePadForm->isDeleteAction = FALSE;

			//ĳ�� ��ǥ ����
			this->notePadForm->isOnImeCharForMove = FALSE;
			this->notePadForm->isFirstComposingForMove = this->notePadForm->isFirstComposing;

			this->notePadForm->Notify();
		}
	}

	this->notePadForm->isFirstComposing = FALSE;
	this->notePadForm->isChanged = TRUE;

	this->notePadForm->Invalidate();
#endif
}

void OnImeCompositionCommand::Undo() {
	KeyAction* keyAction = NULL;
	FactoryProduct factoryProduct(this->notePadForm);

	keyAction = factoryProduct.Make(VK_BACK, NULL, NULL);

	//���� ĳ���� ��ġ�� �������ش�.
	Long currentX = this->currentX;
	Long currentY = this->currentY;

	this->notePadForm->note->MoveCurrent(currentY);
	Glyph* row = this->notePadForm->note->GetChild(currentY);
	row->MoveCurrent(currentX);

	keyAction->KeyDown();
}

void OnImeCompositionCommand::Redo() {
	//���� ĳ���� ��ġ�� �������ش�.
	Long currentX = this->currentX;
	Long currentY = this->currentY;

	this->notePadForm->note->MoveCurrent(currentY);
	Glyph* row = this->notePadForm->note->GetChild(currentY);
	row->MoveCurrent(currentX);

	Execute();
}

bool OnImeCompositionCommand::GetChecked() {
	return this->isChecked;

}

Long OnImeCompositionCommand::GetCurrentX() {
	return this->currentX;
}

Long OnImeCompositionCommand::GetCurrentY() {
	return this->currentY;
}

void OnImeCompositionCommand::SetCheck() {
	this->isChecked = true;
}

bool OnImeCompositionCommand::GetLineChanged() {
	return this->isLineChanged;
}

#if 0
char OnImeCompositionCommand::GetCharacter() {
	return this->notePadForm->letter[0];
}
#endif

string OnImeCompositionCommand::GetCharacterForDoubleByte() {
	return this->character;
}













