//OnCharCommand.cpp
#include"OnCharCommand.h"
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
#include"SelectWords.h"
#include"DummyRow.h"


OnCharCommand::OnCharCommand(NotePadForm* notePadForm) // OnChar* onChar
	:Command(notePadForm) {

	this->isChecked = false;
	this->isLineChanged = false;
    this->currentX = 0;
	this->currentY = 0;
	this->beforeCurrentX = 0;
	this->beforeCurrentY = 0;
	this->character[1]= '\0';
	this->isSelected = false;
	this->isLineChangeButtonClicked = false;
}

OnCharCommand::~OnCharCommand() {

}

void OnCharCommand::Execute() {

	char chaCharacter[3];
	string strCharacter;
	Glyph* character = NULL;
	Glyph* row;
	Long rowLength;
	Long noteLength;
	Long i;
	Long j;
	int currentY;
	int currentX;
	int caretX;
	int caretY;
	char characterMove[3];
	int location;
	Glyph* dummyRow;
	Glyph* nextRow;
	Glyph* testRow = NULL;
	int width;
	int exceptWidth;
	BOOL isLineChanged = false;
	int beforeLineChangeCurrentX;
	int changedX;
	int changedY;


	//(21.11.10.�߰�)
	if (this->notePadForm->isOpen == TRUE) {
		this->notePadForm->isOpenChanged = TRUE;
	}



	//21.08.03 �߰�
	char character_[2];

	character_[0] = this->notePadForm->nChar;
	character_[1] = '\0';

	this->character[0] = character_[0];

	CreateGlyph* creator = new CreateProduct();

	//1.1. ���� ĳ���� ��ġ�� Ȯ���Ѵ�.
	currentY = this->notePadForm->note->GetCurrent();
	row = this->notePadForm->note->GetChild(currentY);
	currentX = row->GetCurrent();
	noteLength = this->notePadForm->note->GetLength();
	rowLength = row->GetLength();


	//Command�� ��� �����Ѵ�. (21.09.06)*********************************************************

	//1. ���� ĳ���� ��ġ�� ����� �����Ѵ�.
	this->beforeCurrentX = currentX;
	this->beforeCurrentY = currentY;
	//********************************************************************************************

	//1.2. �ڵ����� ��ư�� ������ ���,
	if (this->notePadForm->isLineChangeButtonClicked == true) {

		//1.2.1. �ڵ����� �� ĳ���� ��ġ�� ã�´�.
		this->notePadForm->lineChange->FindCaretBeforeLineChange(&currentX, &currentY);

		//1.2.2. �ڵ����� �� ������ ��ģ��.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

		row = this->notePadForm->note->GetChild(currentY);
		rowLength = row->GetLength();

		//1.2.3. ���� ������ ���, ���� ������ �ʺ� �����Ѵ�.
		this->isLineChangeButtonClicked = true;
		this->windowWidth = this->notePadForm->scroll->windowWidth;
	}

	//1.3. �Է¹��� ���ڰ� ���๮���� ���,
	if (character_[0] == '\n' || character_[0] == '\r') {

		//(21.11.09.�߰�)
		this->notePadForm->isLineChangeAppend = true;

		//1.3.1. ���� note�� currentY+1��°�� �� ���� �����ִ´�. (�������� + ��������)
		this->notePadForm->row = creator->Make(character_);
		this->notePadForm->note->Add(currentY+1, this->notePadForm->row); //currentY + 1

		//1.3.2. currentX�� row�� length���� ���� ���, (��������)
		if (currentX < rowLength) {
			//1.3.2.1. ���� row�� length��ŭ �ݺ��Ѵ�.
			while (currentX < rowLength) {
				//1. ���ڸ� �д´�.
				character = row->GetChild(currentX);
				//2. currentY+1��°�� �������´�.
				this->notePadForm->row->Add(character);

				//3. currentX��° ���ڸ� �����Ѵ�.
				row->Remove(currentX);

				rowLength--;
			}
		}
		//1.3.3. ĳ���� (currentY+1, 0)���� ��ǥ�����Ѵ�.
		changedY = currentY + 1;
		changedX = 0;


		//(21.09.03 �߰�) ���๮���̸� command�� isChecked ����� true�� �ٲ��ش�.
		this->isChecked = true;
		this->isLineChanged = true;
	}

	//1.4. �Է¹��� ���ڰ� ���๮�ڰ� �ƴ� ���,
	else {
		//(21.11.09.�߰�)
		this->notePadForm->isLineChangeAppend = false;

		//1.4.0. character�� �����.
		character = creator->Make(character_);

		//1.4.1. currentX�� row�� length���� ���� ���(��������), ���� row�� currentX��°�� ���ڸ� �������´�.
		if (currentX < rowLength) {
			row->Add(currentX, character);
		}
		//1.4.2. currentX�� row�� length���� ũ�ų� ���� ���, ���� row�� ���� ���´�.
		else {
			row->Add(character);
		}
		//1.4.3. ĳ���� (currentY, currentX+1)�� ��ǥ�����Ѵ�.
		changedY = currentY;
		changedX = currentX + 1;
		//this->isChecked = false;
	}

	//1.5. �ڵ����� ��ư�� ������ ���,
	if (this->notePadForm->isLineChangeButtonClicked == true) {

		//1.5.1. ��ģ ������ �ٽ� �ڵ������Ѵ�. (ON)
		this->notePadForm->lineChange->LineChangeButtonClicked();

		//1.5.1. �ڵ����� �� �̵��� ĳ���� ��ġ�� ���Ѵ�.
		this->notePadForm->lineChange->FindCaretAfterLineChange(changedX, changedY);
	}
	else {
		//1.6. ĳ���� �ű��.
		this->notePadForm->note->MoveCurrent(changedY);
		this->notePadForm->row = this->notePadForm->note->GetChild(changedY);
		this->notePadForm->row->MoveCurrent(changedX);
	}

	if (creator != NULL) {
		delete creator;
		creator = NULL;
	}

	//2.6. currentX(row�� current)���� ���� ���� �ݺ��Ѵ�.
	this->notePadForm->isOnImeCharForMove = false;
	this->notePadForm->isFirstComposingForMove = false;


	//----------------------------------------------------------------------------
	// Scroll ������Ʈ �� ���� ������ �����Ѵ�.
	this->notePadForm->isJumpOverForPrevious = false;
	this->notePadForm->isJumpOverForNext = false;
	this->notePadForm->isDoubleByte = false;
	this->notePadForm->isUp = false;
	this->notePadForm->isDown = false;


	//Command�� ��� �����Ѵ�. *******************************************************************
	
	//1.1. ���� ĳ���� ��ġ�� Ȯ���Ѵ�.
	currentY = this->notePadForm->note->GetCurrent();
	row = this->notePadForm->note->GetChild(currentY);	
	currentX = row->GetCurrent();

	//1.2. ���� ĳ���� ��ġ�� ����� �����Ѵ�.
	this->currentX = currentX;
	this->currentY = currentY;

#if 0
	//(21.10.31.�߰�) ���๮�� ������ currentY �����Ҷ��� 1���ش�. �׷��� üũ�� �ȴ�.
	if (character_[0] == '\n' || character_[0] == '\r') {
		this->currentY--;
	}
#endif

	//1.3. ���� BackSpace�� Undo���̶�� �ٲ� ĳ���� ��ġ�� �����Ѵ�.
	this->notePadForm->currentX = currentX;
	this->notePadForm->currentY = currentY;
	//********************************************************************************************


	this->notePadForm->Notify(); //Observer Pattern ����


	this->notePadForm->isChanged = true;
	this->notePadForm->Invalidate();
}

void OnCharCommand::Undo() {

	int windowWidth = this->notePadForm->scroll->windowWidth; //���� ������ʺ�
	KeyAction* keyAction = NULL;
	FactoryProduct factoryProduct(this->notePadForm);

	keyAction = factoryProduct.Make(VK_BACK, NULL, NULL);


	int currentX = this->currentX;
	int currentY = this->currentY;

	//1. (21.09.30 �߰�) ���� �ڵ������ư ��Ȳ�� �������� ��Ȳ�� �ٸ���,
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



	//3. ���� ĳ���� ��ġ�� �������ش�.
	this->notePadForm->note->MoveCurrent(currentY);
	Glyph* row = this->notePadForm->note->GetChild(currentY);
	row->MoveCurrent(currentX);


	//(21.11.01.�߰�) ��ü ���� ������ �ʱ�ȭ�Ѵ�.
	this->notePadForm->selectWords->firstX = 0;
	this->notePadForm->selectWords->firstY = 0;
	this->notePadForm->selectWords->endX = 0;
	this->notePadForm->selectWords->endY = 0;



	keyAction->KeyDown();

}

void OnCharCommand::Redo() {

	int windowWidth = this->notePadForm->scroll->windowWidth; //���� ������ʺ�
	int currentX = this->beforeCurrentX;
	int  currentY = this->beforeCurrentY;

	//1. (21.09.30 �߰�) ���� �ڵ������ư ��Ȳ�� �������� ��Ȳ�� �ٸ���,
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

	//���� ĳ���� ��ġ�� �������ش�.
	this->notePadForm->note->MoveCurrent(currentY);
	Glyph* row = this->notePadForm->note->GetChild(currentY);
	row->MoveCurrent(currentX);

	//(21.10.01 �߰�)
	this->isLineChangeButtonClicked = this->notePadForm->isLineChangeButtonClicked;
	this->windowWidth = windowWidth;

	Execute();
}




#if 0
void OnCharCommand::Pile() {
	Command* previousCommand;
	Long previousColumn;
	Long column = this->GetColumn();

	//1. Ŀ�ǵ带 PUSH�ϱ� ��, üũ�� �ʿ��ϸ� üũ���ش�. **************************************
	//1.1. undoList�� ������� ������,
	if (this->notePadForm->commandStory->undoList->IsEmpty() == false) {
		previousCommand = this->notePadForm->commandStory->undoList->Top();
		previousColumn = previousCommand->GetColumn();

		bool isChecked = previousCommand->GetChecked();

		//1.1.1. ���� commmand�� üũ�Ǿ� ���� �ʰ�, �� �÷��� �� �÷��� ���̰� 1�� �ƴ� ���,
		if (this->isChecked == false && (column - previousColumn != 1)) {
			this->isChecked = true;
		}
	}
	//1.2. undoList�� ���������,
	else {
		//1.2.1. command�� isChecked�� true�� �ٲ��ش�.
		this->isChecked = true;
	}
	//********************************************************************************************
	//UndoList�� �߰��Ѵ�.
	this->notePadForm->commandStory->undoList->Push(this);

	//�ش� Ŀ�ǵ尡 �����̵Ǹ� redo�� ����� �ʱ�ȭ�Ǳ� ������ this.redoList�� �ʱ�ȭ��Ų��.
	this->notePadForm->commandStory->redoList->DeleteAll();
}
#endif

bool OnCharCommand::GetChecked() {
	return this->isChecked;

}

Long OnCharCommand::GetCurrentX() {
	return this->currentX;
}

Long OnCharCommand::GetCurrentY() {
	return this->currentY;
}

void OnCharCommand::SetCheck() {
	this->isChecked = true;
}

bool OnCharCommand::GetLineChanged() {
	return this->isLineChanged;
}

char OnCharCommand::GetCharacter() {
	return this->character[0];
}