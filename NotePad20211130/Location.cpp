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

	//1. 커서를 이동할 때
	 //1.1. 기본 세로 높이 구한다.
	int fontSize = this->notePadForm->font->GetFontSize();
	string fontName = this->notePadForm->font->GetFontName();
	
	//(21.10.26 수정)
	int strSizeY = this->notePadForm->getTextSize->GetX(131);
	//int strSizeY = this->notePadForm->strSizeY;

	//1.2. 현재 currentX, currentY  구한다.
	Long currentY = this->notePadForm->note->GetCurrent();
	this->notePadForm->row = this->notePadForm->note->GetChild(currentY);
	Long currentX = this->notePadForm->row->GetCurrent();

	//1.3. caret의 x를 row의 length로 초기화한다.
	this->caret->x = 0;

	//1.4. caret의 y를 note의 current로 초기화한다.
	this->caret->y = currentY;

	//21.07.20. Observe Pattern  이동하다 전용 ******************************************

	while (i < currentX) {
		//2.6.1. i번째 문자를 얻는다.
		character = this->notePadForm->row->GetChild(i);
		strCharacter = character->GetContent();
		strcpy(chaCharacter, strCharacter.c_str());

		if (chaCharacter[0] >= 0 && chaCharacter[0] < 128) {
			chaCharacter[1] = '\0';
		}
		else {
			chaCharacter[2] = '\0';
		}

		//2.6.2. Location의 CalculateX를 호출한다.
		this->notePadForm->getTextSize->CalculateX(chaCharacter);
		//CalculateX(chaCharacter);
		i++;
	}

	//2.7. caret의 x,y의 값 가져온다.
	caretX = this->caret->GetX();
	caretY = this->caret->GetY();


		if (this->notePadForm->isFirstComposingForMove == FALSE) {
			// 커서를 만든다.
			this->notePadForm->CreateSolidCaret(3, strSizeY);
			this->notePadForm->caretWidth = 3;  //21.07.19 추가, 한글 막 적혔을때 이동방지, 커서크기만 바꾸기.
		}
		else if ((this->notePadForm->isFirstComposingForMove == TRUE || this->notePadForm->isOnImeCharForMove == TRUE) &&
			this->notePadForm->isRedo == FALSE) {
			//1.4.1. 한글의 기본 크기 확인한다.
			strSizeX = this->notePadForm->getTextSize->GetX(128);

			//1.4.2. 커서 만든다.
			this->notePadForm->CreateSolidCaret(strSizeX, strSizeY); //strSizeX, strSizeY.cy

			this->notePadForm->caretWidth = strSizeX;  //21.07.19 추가, 한글 막 적혔을때 이동방지, 커서크기만 바꾸기.
		}



		//1 캐럿의 위치를 이동한다.
#if 0
		if (caretX > this->notePadForm->scroll->windowWidth) {
			//1. Horizon 스크롤 정보를 Get한다.
			this->notePadForm->scroll->siHorizon.fMask = SIF_POS;
			this->notePadForm->GetScrollInfo(SB_HORZ, &this->notePadForm->scroll->siHorizon);
			caretX= caretX - this->notePadForm->scroll->siHorizon.nPos;
		}
		int lastCurrentY;

		if (strSizeY * currentY > this->notePadForm->scroll->windowHeight) {
			//1. Vertical 스크롤 정보를 Get한다.
			this->notePadForm->scroll->siVertical.fMask = SIF_POS;
			this->notePadForm->GetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical);
			//lastCurrentY = (strSizeY * currentY) - this->notePadForm->scroll->siHorizon.nPos;
			lastCurrentY = (strSizeY * currentY) - this->notePadForm->scroll->siVertical.nPos;
		}
		else {
			lastCurrentY = strSizeY * currentY;
		}
#endif
		//1. Horizon 스크롤 정보를 Get한다.
		this->notePadForm->scroll->siHorizon.fMask = SIF_POS;
		this->notePadForm->GetScrollInfo(SB_HORZ, &this->notePadForm->scroll->siHorizon);
		caretX = caretX - this->notePadForm->scroll->siHorizon.nPos;

		//1. Vertical 스크롤 정보를 Get한다.
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

	//1. 커서를 이동할 때
	//1.1 탭키일 때
	if (character[0] == '\t') {
		// 공백의 기본 크기 확인한다.
		strSizeX = this->notePadForm->getTextSize->GetX(130);

		// Caret의 x,y값을 가져온다.
		caretX = this->caret->GetX();
		caretY = this->caret->GetY();

		//1.3.5. Caret의 x의 값을 더해준다.
		this->caret->x = caretX + (strSizeX * 8);
		//Caret(this, this->caret->x, caretY); //21.07.26. 매개변수 갖는 생성자 추가.
	}
	//1.3. 개행 아니고 스페이스 바 아니고 1byte일 때
	else if (character[1] == '\0' && character[0] != ' ') {

		//1.3.1. 입력받은 문자의 너비 확인한다.
		charInt = character[0];
		strSizeX = this->notePadForm->getTextSize->GetX(charInt);

		//1.3.3. Caret의 x,y값을 가져온다.
		caretX = this->caret->GetX();
		caretY = this->caret->GetY();

		//1.3.5. Caret의 x의 값을 더해준다.
		this->caret->x = caretX + strSizeX;
		//Caret(this, this->caret->x, caretY); //21.07.26. 매개변수 갖는 생성자 추가.
	}
	// 개행 아니고 스페이스 바일 때
	else if (character[1] == '\0' && character[0] == ' ') {
		// 공백의 기본 크기 확인한다.
		strSizeX = this->notePadForm->getTextSize->GetX(130);

		// Caret의 x,y값을 가져온다.
		caretX = this->caret->GetX();
		caretY = this->caret->GetY();

		//1.3.5. Caret의 x의 값을 더해준다.
		this->caret->x = caretX + strSizeX;
	}

	//1.4. 2byte일때
	else if (character[2] == '\0') { 
		//1.4.1. 한글의 기본 크기 확인한다.
		strSizeX = this->notePadForm->getTextSize->GetX(128);

		// Caret의 x,y값을 가져온다.
		caretX = this->caret->GetX();
		caretY = this->caret->GetY();

		//1.3.5. Caret의 x의 값을 더해준다.
		this->caret->x = caretX + strSizeX;
	}
}
#endif













