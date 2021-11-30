//OnImeCharCommand.cpp
#include"OnImeCharCommand.h"
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
#include"SelectWords.h"
using namespace std;

OnImeCharCommand::OnImeCharCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {

	this->isChecked = false;
	this->isLineChanged = false;
	this->currentX = 0;
	this->currentY = 0;
	this->beforeCurrentX = 0;
	this->beforeCurrentY = 0;
	//this->character[2] = '\0';
	this->isRedo = false;

	this->isLineChangeButtonClicked = false;
	this->notePadForm->isRedo = this->isRedo;
	this->notePadForm->isOnImeCharForMove = true;	
}

OnImeCharCommand::~OnImeCharCommand() {
}

void OnImeCharCommand::Execute() {

	Glyph* character;
	char letter[3];
	letter[0] = this->notePadForm->letter.at(0);
	letter[1] = this->notePadForm->letter.at(1);
	letter[2] = '\0';
#if 0
	letter[0] = HIBYTE(LOWORD(this->notePadForm->letter));
	letter[1] = LOBYTE(LOWORD(this->notePadForm->letter));
	letter[2] = '\0';
#endif
	Long length;
	Long i = 0;
	string strCharacter;
	char chaCharacter[3];
	char characterMove[3];
	Long caretX;
	Long caretY;
	Long changedX;
	Long changedY;
	this->windowWidth = this->notePadForm->scroll->windowWidth;
	CreateGlyph* creator = new CreateProduct();

	character = creator->Make(letter);

	this->character = letter;

	//21.08.03 �߰�

	//2. ���ڰ� �ϼ� �Ǿ��� ��
	  //2.1. ���� ĳ���� ��ġ�� Ȯ���Ѵ�.
	int currentY = this->notePadForm->note->GetCurrent();
	this->notePadForm->row = this->notePadForm->note->GetChild(currentY);
	int currentX = this->notePadForm->row->GetCurrent();
	length = this->notePadForm->row->GetLength();


	//Command�� ��� �����Ѵ�. (21.09.06)*********************************************************
	this->beforeCurrentX = currentX;
	this->beforeCurrentY = currentY;

	//(21.11.09.�߰�) ���� currentX�� 0�̸�, currentY = currentY-1, currnetX�� ���� row�� length�� ġȯ�Ѵ�.
	if (this->notePadForm->isLineChangeAppend == false && currentY > 0 && currentX <= 0) {

		Glyph* beforeRow = this->notePadForm->note->GetChild(currentY - 1);
		int beforeLength = beforeRow->GetLength();

		if (beforeLength > 0) {
			this->beforeCurrentX = beforeLength;
			this->beforeCurrentY = currentY - 1;
		}
	}

	//(21.11.09.�߰�)
	this->notePadForm->isLineChangeAppend = false;

	//********************************************************************************************



	//2.2 �ڵ������ư�� ������ ���,
	if (this->notePadForm->isLineChangeButtonClicked == true) {

		//2.1. �ڵ����� �� ĳ���� ��ġ�� ã�´�.
		this->notePadForm->lineChange->FindCaretBeforeLineChange(&currentX, &currentY);

		//2.2. �ڵ����� �� ������ ��ģ��.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

		this->notePadForm->row = this->notePadForm->note->GetChild(currentY);
		length = this->notePadForm->row->GetLength();
		this->isLineChangeButtonClicked = true;
		this->windowWidth = this->notePadForm->scroll->windowWidth;
	}

	//2.3. ������ ������ ���ڸ� �����. (Redo�� �ƴ� ���)
	if (this->isRedo == false && this->notePadForm->isUndo == false) {
		this->notePadForm->row->Remove(currentX);
	}

	//2.4. currentX�� row�� length-1���� �۰ų�, currentX�� length-1���� ũ�ų� ����, Ŀ�� ũ�Ⱑ 3�� ���,
	if (currentX < length - 1 || (currentX >= length - 1 && this->notePadForm->caretWidth == 3)) {
		//2.4.1. ���� �ٿ� ���� ���ڸ� �������´�.
		this->notePadForm->row->Add(currentX, character);

	}

	//2.5. currentX�� row�� length���� ũ�ų� ���ų�, currentX�� length-1���� ũ�ų� ����, Ŀ�� ũ�Ⱑ 3���� ū ���,
	else if (currentX >= length || (currentX >= length - 1 && this->notePadForm->caretWidth > 3)) {
		//2.5.1. ���� �ٿ� ���� ���ڸ� �������´�.
		this->notePadForm->row->Add(character);
	}

	//2.6. ��ǥ�� currentX+1�� �̵��Ѵ�.
	changedY = currentY;
	changedX = currentX + 1;

	//2.7. �ڵ����� ��ư�� ������ ���,
	if (this->notePadForm->isLineChangeButtonClicked == true) {

		//2.7.1. ��ģ ������ �ٽ� �ڵ������Ѵ�. (ON)
		this->notePadForm->lineChange->LineChangeButtonClicked();

		//2.7.2. �ڵ����� �� �̵��� ĳ���� ��ġ�� ���Ѵ�.
		this->notePadForm->lineChange->FindCaretAfterLineChange(changedX, changedY);
	}
	else {
		this->notePadForm->row->MoveCurrent(changedX);
	}

	//Command�� ��� �����Ѵ�. *******************************************************************
	//1.1. ���� ĳ���� ��ġ�� Ȯ���Ѵ�.
	currentY = this->notePadForm->note->GetCurrent();
	this->notePadForm->row = this->notePadForm->note->GetChild(currentY);
	currentX = this->notePadForm->row->GetCurrent();

	//1.2. ���� ĳ���� ��ġ�� ����� �����Ѵ�.
	this->currentX = currentX;
	this->currentY = currentY;

	//1.3. ���� BackSpace�� Undo���̶�� �ٲ� ĳ���� ��ġ�� �����Ѵ�.
	this->notePadForm->currentX = currentX;
	this->notePadForm->currentY = currentY;
	//********************************************************************************************


	this->notePadForm->isOnImeCharForMove = true;
	this->notePadForm->isFirstComposingForMove = this->notePadForm->isFirstComposing;

	//Scroll�� ������ �����Ѵ�.
	this->notePadForm->isDoubleByte = true;
	this->notePadForm->isJumpOverForPrevious = false;
	this->notePadForm->isJumpOverForNext = false;
	this->notePadForm->isUp = false;
	this->notePadForm->isDown = false;

	this->notePadForm->Notify();


	this->notePadForm->isFirstComposing = true;
	this->notePadForm->isChanged = true;

	this->notePadForm->Invalidate();


#if 0
	//2.2. ������ ������ ���ڸ� �����. (Redo�� �ƴ� ���)
	if (this->isRedo == false) {
		this->notePadForm->row->Remove(currentX);
	}

	length = this->notePadForm->row->GetLength();


	//(21.08.12. �߰�) �ڵ������ư OFF�� ���,
	if (this->notePadForm->isLineChangeButtonClicked == false) {

		//2.3. currentX�� row�� length-1���� �۰ų�, currentX�� length-1���� ũ�ų� ����, Ŀ�� ũ�Ⱑ 3�� ���,
		if (currentX < length - 1 || (currentX >= length - 1 && this->notePadForm->caretWidth == 3)) {
			//2.3.1. ���� �ٿ� ���� ���ڸ� �������´�.
			this->notePadForm->row->Add(currentX, character);

			this->notePadForm->isInsert = true;
			this->notePadForm->isAppend = false;
			this->notePadForm->isMoveActionVertical = false;
			this->notePadForm->isMoveActionHorizon = false;
			this->notePadForm->isDeleteAction = false;
		}

		//2.4. currentX�� row�� length���� ũ�ų� ���ų�, currentX�� length-1���� ũ�ų� ����, Ŀ�� ũ�Ⱑ 3���� ū ���,
		else if (currentX >= length || (currentX >= length - 1 && this->notePadForm->caretWidth > 3)) {
			//2.4.1. ���� �ٿ� ���� ���ڸ� �������´�.
			this->notePadForm->row->Add(character);

			this->notePadForm->isInsert = false;
			this->notePadForm->isAppend = true;
			this->notePadForm->isMoveActionVertical = false;
			this->notePadForm->isMoveActionHorizon = false;
			this->notePadForm->isDeleteAction = false;
		}

		//2.5. ��ǥ�� currentX+1�� �̵��Ѵ�.
		changedX = currentX + 1;
		this->notePadForm->row->MoveCurrent(currentX + 1);
	}
	//(21.08.12. �߰�) �ڵ������ư ON�� ���,
	else if (this->notePadForm->isLineChangeButtonClicked == true) {

#if 0
		//2.3. currentX�� row�� length-1���� �۰ų�, currentX�� length-1���� ũ�ų� ����, Ŀ�� ũ�Ⱑ 3�� ���,
		if (currentX < length - 1 || (currentX >= length - 1 && this->caretWidth == 3)) {
			//2.3.1. ���� �ٿ� ���� ���ڸ� �������´�.
			this->row->Add(currentX, character);
		}

		//2.4. currentX�� row�� length���� ũ�ų� ���ų�, currentX�� length-1���� ũ�ų� ����, Ŀ�� ũ�Ⱑ 3���� ū ���,
		else if (currentX >= length || (currentX >= length - 1 && this->caretWidth > 3)) {
			//2.4.1. ���� �ٿ� ���� ���ڸ� �������´�.
			this->row->Add(character);
		}
#endif
		this->notePadForm->row->Add(currentX, character);

		//2.5. ��ǥ�� currentX+1�� �̵��Ѵ�.
		changedX = currentX + 1;
		this->notePadForm->row->MoveCurrent(currentX + 1);


		//��ũ�� ����
		this->notePadForm->isInsert = false;
		this->notePadForm->isAppend = false;
		this->notePadForm->isMoveActionVertical = false;
		this->notePadForm->isMoveActionHorizon = false;
		this->notePadForm->isDeleteAction = false;
	}

	//Command�� ��� �����Ѵ�. *******************************************************************

	//1. ���� ĳ���� ��ġ�� ����� �����Ѵ�.
	this->currentX = changedX;
	this->currentY = currentY;
	//********************************************************************************************

	//2.7. currentX(row�� current)���� ���� ���� �ݺ��Ѵ�.
	this->notePadForm->isOnImeCharForMove = true;
	this->notePadForm->isFirstComposingForMove = this->notePadForm->isFirstComposing;
	this->notePadForm->Notify();


	this->notePadForm->isFirstComposing = true;
	this->notePadForm->isChanged = true;

	this->notePadForm->Invalidate();

	//return 0;
#endif


}

void OnImeCharCommand::Undo() {

	int windowWidth = this->notePadForm->scroll->windowWidth;
	KeyAction* keyAction = NULL;
	FactoryProduct factoryProduct(this->notePadForm);

	keyAction = factoryProduct.Make(VK_BACK, NULL, NULL);

	int currentX = this->currentX;
	int currentY = this->currentY;

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


	//���� ĳ���� ��ġ�� �������ش�.
	this->notePadForm->note->MoveCurrent(currentY);
	Glyph* row = this->notePadForm->note->GetChild(currentY);
	row->MoveCurrent(currentX);

	//���õ� ������ �ʱ�ȭ�Ѵ�.
	this->notePadForm->selectWords->firstX = 0;
	this->notePadForm->selectWords->firstY = 0;
	this->notePadForm->selectWords->endX = 0;
	this->notePadForm->selectWords->endY = 0;


	keyAction->KeyDown();
}


void OnImeCharCommand::Redo() {

	int windowWidth = this->notePadForm->scroll->windowWidth;
	int currentX = this->beforeCurrentX;
	int currentY = this->beforeCurrentY;
	Glyph* newRow;

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
			//this->notePadForm->lineChange->FindCaretBeforeLineChangeForDouble(currentX, currentY, &currentX, &currentY);
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

#if 0
	//(21.11.02.�߰�) �� ���� ���� ������ ������ش�.
	CreateProduct createProduct;
	if (this->notePadForm->note->GetLength() <= currentY) {
		Glyph* row_ = new DummyRow;
		this->notePadForm->note->Add(row_);
	}
#endif

	Glyph* row = this->notePadForm->note->GetChild(currentY);
	row->MoveCurrent(currentX);

	this->isRedo = true;
	this->notePadForm->isRedo = this->isRedo;
	this->isLineChangeButtonClicked = this->notePadForm->isLineChangeButtonClicked;
	this->windowWidth = windowWidth;

	Execute();
}

bool OnImeCharCommand::GetChecked() {
	return this->isChecked;

}

Long OnImeCharCommand::GetCurrentX() {
	return this->currentX;
}

Long OnImeCharCommand::GetCurrentY() {
	return this->currentY;
}

void OnImeCharCommand::SetCheck() {
	this->isChecked = true;
}

bool OnImeCharCommand::GetLineChanged() {
	return this->isLineChanged;
}

#if 0
char* OnImeCharCommand::GetCharacter() {
	return this->character;
}


WPARAM OnImeCharCommand::GetCharacterFor2Byte() {
	return this->character;
}
#endif

string OnImeCharCommand::GetCharacterForDoubleByte() {
	return this->character;
}









