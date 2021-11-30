//GetTextSizeForDpi.cpp
#include"GetTextSizeForDpi.h"
#include"NotePadForm.h"
#include"Font.h"
#include"Location.h"
#include"Caret.h"
#include"Row.h"
#include"Character.h"
#include <afxwin.h>
#pragma warning(disable:4996)

GetTextSizeForDpi::GetTextSizeForDpi(NotePadForm* notePadForm, Long capacity)
	:asciis(capacity) {
	this->capacity = capacity;
	this->length = 0;
	this->notePadForm = notePadForm;

	//1. 임시 DC를 만든다.
	CClientDC dc(this->notePadForm);
	Long i;
	CDC* dcTemp = this->notePadForm->GetDC();
	//dcTemp->CreateCompatibleDC(&dc);


#if 0
	CClientDC dc(this->notePadForm);
	CDC* pdc = this->notePadForm->GetDC();

	//폰트추가***************************************************************
	CFont cFont;
	int fontSize = this->notePadForm->font->GetFontSize();
	string fontName = this->notePadForm->font->GetFontName();
	cFont.CreatePointFont(fontSize, CString(fontName.c_str()), pdc); //pdc
	dc.SelectObject(&cFont);
	//***********************************************************************
#endif

	//Dpi 적용한 폰트추가 ***************************************************
	int fontSize = this->notePadForm->font->GetFontSize();
	string fontName = this->notePadForm->font->GetFontName();


	HFONT hFont;
	HFONT oldFont;
	LOGFONT printLogFont = this->notePadForm->m_font;
	printLogFont.lfHeight = -MulDiv(fontSize / 10, 600, 72);

	//폰트이름 적용
	strcpy_s(printLogFont.lfFaceName, fontName.c_str());

	hFont = CreateFontIndirect(&printLogFont);
	oldFont = (HFONT)dcTemp->SelectObject(hFont);
	//***********************************************************************

	CSize strSize;
	CString character_;
	char cCharacter;

	for (i = 0; i < 132; i++) {
		//1. 0~31, 127번 들어오면 0을 배열에 할당한다. (크기=0)
		if ((i >= 0 && i < 33) || i == 127) {
			this->asciis.Store(i, 0);
		}
		//2. 한글이 들어오면,
		else if (i == 128) {
			//2.1. 사이즈를 구한다.
			strSize = dcTemp->GetTabbedTextExtent("ㄱ", 0, NULL);
			//strSize = dc.GetTabbedTextExtent("ㄱ", 0, NULL);

			this->asciis.Store(i, strSize.cx);
		}

		// 탭키가 들어오면,
		else if (i == 129) {
			//사이즈를 구한다.
			strSize = dcTemp->GetTabbedTextExtent(" ", 0, NULL);


			this->asciis.Store(i, strSize.cx * 8);
		}

		//공백이 들어오면
		else if (i == 130) {
			//2.1. 사이즈를 구한다.
			strSize = dcTemp->GetTabbedTextExtent(" ", 0, NULL);
			//strSize = dc.GetTabbedTextExtent(" ", 0, NULL);

			this->asciis.Store(i, strSize.cx);
		}
		// ㄱ글자의 세로 높이
		else if (i == 131) {
			//2.1. 사이즈를 구한다.
			strSize = dcTemp->GetTabbedTextExtent("ㄱ", 0, NULL);
			//strSize = dc.GetTabbedTextExtent("ㄱ", 0, NULL);

			this->asciis.Store(i, strSize.cy);
		}
		//3. 1byte가 들어오면,
		else {
			//3.1. i를 CString 으로 바꾼다

			cCharacter = (char)i;

			//character = to_string(i);
			character_ = cCharacter;

			//3.2. 사이즈를 구한다.
			strSize = dcTemp->GetTabbedTextExtent(character_, 0, NULL); //GetOutputTabbedTextExtent
			//strSize = dc.GetTabbedTextExtent(character_, 0, NULL);


			this->asciis.Store(i, strSize.cx);
		}
	}
	//새로운 폰트 제거
	dcTemp->DeleteDC();
	this->notePadForm->ReleaseDC(dcTemp);
}

GetTextSizeForDpi::~GetTextSizeForDpi() {
}

int GetTextSizeForDpi::GetX(int asciiCode) {
	int size;

	size = this->asciis.GetAt(asciiCode);
	return size;
}

void GetTextSizeForDpi::CalculateX(char(*character)) {
	Long strSizeX;
	Long charStrSizeX;
	Long caretX;
	Long caretY;
	int charInt;

	//1. 커서를 이동할 때
	//1.1 탭키일 때
	if (character[0] == '\t') {
		// 공백의 기본 크기 확인한다.
		strSizeX = GetX(129);

		// Caret의 x,y값을 가져온다.
		caretX = this->notePadForm->location->caret->GetX();
		caretY = this->notePadForm->location->caret->GetY(); //this->caret->GetY();

		//1.3.5. Caret의 x의 값을 더해준다.
		this->notePadForm->location->caret->x = this->notePadForm->location->caret->x + strSizeX;
	}
	//1.3. 개행 아니고 스페이스 바 아니고 1byte일 때
	else if (character[1] == '\0' && character[0] != ' ') {

		//1.3.1. 입력받은 문자의 너비 확인한다.
		charInt = character[0];
		strSizeX = GetX(charInt);

		//1.3.3. Caret의 x,y값을 가져온다.
		caretX = this->notePadForm->location->caret->GetX();
		caretY = this->notePadForm->location->caret->GetY();

		//1.3.5. Caret의 x의 값을 더해준다.
		this->notePadForm->location->caret->x = this->notePadForm->location->caret->x + strSizeX;

	}
	// 개행 아니고 스페이스 바일 때
	else if (character[1] == '\0' && character[0] == ' ') {
		// 공백의 기본 크기 확인한다.
		strSizeX = GetX(130);

		// Caret의 x,y값을 가져온다.
		caretX = this->notePadForm->location->caret->GetX();
		caretY = this->notePadForm->location->caret->GetY();

		//1.3.5. Caret의 x의 값을 더해준다.
		this->notePadForm->location->caret->x = this->notePadForm->location->caret->x + strSizeX;
	}

	//1.4. 2byte일때
	else if (character[2] == '\0') {
		//1.4.1. 한글의 기본 크기 확인한다.
		strSizeX = GetX(128);

		// Caret의 x,y값을 가져온다.
		caretX = this->notePadForm->location->caret->GetX();
		caretY = this->notePadForm->location->caret->GetY();

		//1.3.5. Caret의 x의 값을 더해준다.
		this->notePadForm->location->caret->x = this->notePadForm->location->caret->x + strSizeX;
	}

}

Long GetTextSizeForDpi::totalCalculateX(Long endX, Glyph* row) {
	Glyph* character;
	string strCharacter;
	char chaCharacter[3];
	int i = 0;

	//1. caretX, currentX, row를 입력받는다. 보통 caretX는 0이나, 줄이 바뀔 때 더해서 합해야하므로,,
	//2. caretX를 초기화 시켜준다.

	this->notePadForm->location->caret->x = 0;

	//3. currentX번째 만큼 반복한다.
	while (i < endX) {
		//i번째 문자를 얻는다.
		character = row->GetChild(i);

		//(21.11.02. 추가)
		if (character == NULL) {
			i++;
			if (i < endX) {
				character = row->GetChild(i);
			}
			else {
				Long caretX = this->notePadForm->location->caret->GetX(); //최근까지의 너비 
				return caretX;
			}
		}

		strCharacter = character->GetContent();
		strcpy(chaCharacter, strCharacter.c_str());

		if (chaCharacter[0] >= 0 && chaCharacter[0] < 128) {
			chaCharacter[1] = '\0';
		}
		else {
			chaCharacter[2] = '\0';
		}

		//CalculateX를 호출한다.
		CalculateX(chaCharacter);

		i++;
	}


	Long caretX = this->notePadForm->location->caret->GetX(); //최근까지의 너비 

	return caretX;
}



















