//Location.cpp
#include"Location.h"
#include"NotePadForm.h"
#include"Caret.h"
#include"Glyph.h"
#include"Composite.h"
#include"GetTextSize.h"
#include"Font.h"
#include"Scroll.h"
#pragma warning(disable:4996)

Location::Location(NotePadForm* notePadForm) {
	this->caret = new Caret(this);
	this->notePadForm = notePadForm;
}
Location::~Location() {
	if (this->caret != NULL) {
		delete this->caret;
		this->caret = NULL;
	}


}

Long Location::GetX() {
	Long index;
	index = this->caret->GetX();
	return index;
}

Long Location::GetY() {
	Long index;
	index = this->caret->GetY();
	return index;
}

void Location::MoveCaret() {
	Glyph* character;
	string strCharacter;
	char chaCharacter[3];
	Long strSizeX;
	Long caretX;
	Long caretY;
	int charInt;
	Long i = 0;

	//1. Ŀ���� �̵��� ��
	 //1.1. �⺻ ���� ���� ���Ѵ�.
	int fontSize = this->notePadForm->font->GetFontSize();
	string fontName = this->notePadForm->font->GetFontName();
	
	//(21.10.26 ����)
	int strSizeY = this->notePadForm->getTextSize->GetX(131);
	//int strSizeY = this->notePadForm->strSizeY;

	//1.2. ���� currentX, currentY  ���Ѵ�.
	Long currentY = this->notePadForm->note->GetCurrent();
	this->notePadForm->row = this->notePadForm->note->GetChild(currentY);
	Long currentX = this->notePadForm->row->GetCurrent();

	//1.3. caret�� x�� row�� length�� �ʱ�ȭ�Ѵ�.
	this->caret->x = 0;

	//1.4. caret�� y�� note�� current�� �ʱ�ȭ�Ѵ�.
	this->caret->y = currentY;

	//21.07.20. Observe Pattern  �̵��ϴ� ���� ******************************************

	while (i < currentX) {
		//2.6.1. i��° ���ڸ� ��´�.
		character = this->notePadForm->row->GetChild(i);
		strCharacter = character->GetContent();
		strcpy(chaCharacter, strCharacter.c_str());

		if (chaCharacter[0] >= 0 && chaCharacter[0] < 128) {
			chaCharacter[1] = '\0';
		}
		else {
			chaCharacter[2] = '\0';
		}

		//2.6.2. Location�� CalculateX�� ȣ���Ѵ�.
		this->notePadForm->getTextSize->CalculateX(chaCharacter);
		//CalculateX(chaCharacter);
		i++;
	}

	//2.7. caret�� x,y�� �� �����´�.
	caretX = this->caret->GetX();
	caretY = this->caret->GetY();


		if (this->notePadForm->isFirstComposingForMove == FALSE) {
			// Ŀ���� �����.
			this->notePadForm->CreateSolidCaret(3, strSizeY);
			this->notePadForm->caretWidth = 3;  //21.07.19 �߰�, �ѱ� �� �������� �̵�����, Ŀ��ũ�⸸ �ٲٱ�.
		}
		else if ((this->notePadForm->isFirstComposingForMove == TRUE || this->notePadForm->isOnImeCharForMove == TRUE) &&
			this->notePadForm->isRedo == FALSE) {
			//1.4.1. �ѱ��� �⺻ ũ�� Ȯ���Ѵ�.
			strSizeX = this->notePadForm->getTextSize->GetX(128);

			//1.4.2. Ŀ�� �����.
			this->notePadForm->CreateSolidCaret(strSizeX, strSizeY); //strSizeX, strSizeY.cy

			this->notePadForm->caretWidth = strSizeX;  //21.07.19 �߰�, �ѱ� �� �������� �̵�����, Ŀ��ũ�⸸ �ٲٱ�.
		}



		//1 ĳ���� ��ġ�� �̵��Ѵ�.
#if 0
		if (caretX > this->notePadForm->scroll->windowWidth) {
			//1. Horizon ��ũ�� ������ Get�Ѵ�.
			this->notePadForm->scroll->siHorizon.fMask = SIF_POS;
			this->notePadForm->GetScrollInfo(SB_HORZ, &this->notePadForm->scroll->siHorizon);
			caretX= caretX - this->notePadForm->scroll->siHorizon.nPos;
		}
		int lastCurrentY;

		if (strSizeY * currentY > this->notePadForm->scroll->windowHeight) {
			//1. Vertical ��ũ�� ������ Get�Ѵ�.
			this->notePadForm->scroll->siVertical.fMask = SIF_POS;
			this->notePadForm->GetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical);
			//lastCurrentY = (strSizeY * currentY) - this->notePadForm->scroll->siHorizon.nPos;
			lastCurrentY = (strSizeY * currentY) - this->notePadForm->scroll->siVertical.nPos;
		}
		else {
			lastCurrentY = strSizeY * currentY;
		}
#endif
		//1. Horizon ��ũ�� ������ Get�Ѵ�.
		this->notePadForm->scroll->siHorizon.fMask = SIF_POS;
		this->notePadForm->GetScrollInfo(SB_HORZ, &this->notePadForm->scroll->siHorizon);
		caretX = caretX - this->notePadForm->scroll->siHorizon.nPos;

		//1. Vertical ��ũ�� ������ Get�Ѵ�.
		this->notePadForm->scroll->siVertical.fMask = SIF_POS;
		this->notePadForm->GetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical);
		//lastCurrentY = (strSizeY * currentY) - this->notePadForm->scroll->siHorizon.nPos;
		int lastCurrentY = (strSizeY * currentY) - this->notePadForm->scroll->siVertical.nPos;

		this->caret->Move(caretX, lastCurrentY);

		this->notePadForm->Invalidate();
	//***********************************************************************************
}

void Location::Update() {

	MoveCaret();
}

#if 0
void Location::CalculateX(char(*character)) {
	Long strSizeX;
	Long charStrSizeX;
	Long caretX;
	Long caretY;
	int charInt;

	//1. Ŀ���� �̵��� ��
	//1.1 ��Ű�� ��
	if (character[0] == '\t') {
		// ������ �⺻ ũ�� Ȯ���Ѵ�.
		strSizeX = this->notePadForm->getTextSize->GetX(130);

		// Caret�� x,y���� �����´�.
		caretX = this->caret->GetX();
		caretY = this->caret->GetY();

		//1.3.5. Caret�� x�� ���� �����ش�.
		this->caret->x = caretX + (strSizeX * 8);
		//Caret(this, this->caret->x, caretY); //21.07.26. �Ű����� ���� ������ �߰�.
	}
	//1.3. ���� �ƴϰ� �����̽� �� �ƴϰ� 1byte�� ��
	else if (character[1] == '\0' && character[0] != ' ') {

		//1.3.1. �Է¹��� ������ �ʺ� Ȯ���Ѵ�.
		charInt = character[0];
		strSizeX = this->notePadForm->getTextSize->GetX(charInt);

		//1.3.3. Caret�� x,y���� �����´�.
		caretX = this->caret->GetX();
		caretY = this->caret->GetY();

		//1.3.5. Caret�� x�� ���� �����ش�.
		this->caret->x = caretX + strSizeX;
		//Caret(this, this->caret->x, caretY); //21.07.26. �Ű����� ���� ������ �߰�.
	}
	// ���� �ƴϰ� �����̽� ���� ��
	else if (character[1] == '\0' && character[0] == ' ') {
		// ������ �⺻ ũ�� Ȯ���Ѵ�.
		strSizeX = this->notePadForm->getTextSize->GetX(130);

		// Caret�� x,y���� �����´�.
		caretX = this->caret->GetX();
		caretY = this->caret->GetY();

		//1.3.5. Caret�� x�� ���� �����ش�.
		this->caret->x = caretX + strSizeX;
	}

	//1.4. 2byte�϶�
	else if (character[2] == '\0') { 
		//1.4.1. �ѱ��� �⺻ ũ�� Ȯ���Ѵ�.
		strSizeX = this->notePadForm->getTextSize->GetX(128);

		// Caret�� x,y���� �����´�.
		caretX = this->caret->GetX();
		caretY = this->caret->GetY();

		//1.3.5. Caret�� x�� ���� �����ش�.
		this->caret->x = caretX + strSizeX;
	}
}
#endif













