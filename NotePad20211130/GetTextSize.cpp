//GetTextSize.cpp
#include"GetTextSize.h"
#include"NotePadForm.h"
#include"Location.h"
#include"Font.h"
#include"Caret.h"
#include"Glyph.h"
#include"Composite.h"
#include"Character.h"
#include"Note.h"
#include"Row.h"
#include"SingleByteCharacter.h"
#include"DoubleByteCharacter.h"
#include <afxwin.h>
#pragma warning(disable:4996)

GetTextSize::GetTextSize(NotePadForm* notePadForm, Long capacity) 
	:asciis(capacity){
	this->capacity = capacity;
	this->length = 0;
	this->notePadForm = notePadForm;

	Long i;
	CClientDC dc(this->notePadForm);
	CDC* pdc = this->notePadForm->GetDC();

	//폰트추가***************************************************************
	CFont cFont;
	int fontSize = this->notePadForm->font->GetFontSize();
	string fontName = this->notePadForm->font->GetFontName();
	cFont.CreatePointFont(fontSize, CString(fontName.c_str()), pdc); //pdc
	dc.SelectObject(&cFont);
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
			strSize = dc.GetTabbedTextExtent("ㄱ", 0, NULL);
			//strSize = dc.GetTabbedTextExtent("ㄱ", 0, NULL);

			this->asciis.Store(i, strSize.cx);	
		}

		// 탭키가 들어오면,
		else if (i == 129) {
			//사이즈를 구한다.
			strSize = dc.GetTabbedTextExtent(" ", 0, NULL);


			this->asciis.Store(i, strSize.cx * 8);
		}

		//공백이 들어오면
		else if (i == 130) {
			//2.1. 사이즈를 구한다.
			strSize = dc.GetTabbedTextExtent(" ", 0, NULL);
			//strSize = dc.GetTabbedTextExtent(" ", 0, NULL);

			this->asciis.Store(i, strSize.cx);
		}
		// ㄱ글자의 세로 높이
		else if (i == 131) {
			//2.1. 사이즈를 구한다.
			strSize = dc.GetTabbedTextExtent("ㄱ", 0, NULL);
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
			strSize = dc.GetTabbedTextExtent(character_, 0, NULL); //GetOutputTabbedTextExtent
			//strSize = dc.GetTabbedTextExtent(character_, 0, NULL);


			this->asciis.Store(i, strSize.cx);
		}
	}
	//새로운 폰트 제거
	cFont.DeleteObject();
	this->notePadForm->ReleaseDC(pdc);
}

GetTextSize::~GetTextSize() {
}

GetTextSize::GetTextSize(const GetTextSize& source) {//복사생성자
	this->asciis = source.asciis;
	this->capacity = source.capacity;
	this->length = source.length;
}

GetTextSize& GetTextSize::operator=(const GetTextSize& source) {//치환연산자
	this->asciis = source.asciis;
	this->capacity = source.capacity;
	this->length = source.length;

	return *this;
}

int GetTextSize::GetX(int asciiCode) {
	int size;

	size = this->asciis.GetAt(asciiCode);
	return size;
}

void GetTextSize::CalculateX(char(*character)) {
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

Long GetTextSize::totalCalculateX(Long endX, Glyph* row) {
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

Long GetTextSize::totalCalculateEndX() { // 현재 줄에서 endX까지의 너비를 구하는 함수.
	Glyph* character;
	string strCharacter;
	char chaCharacter[3];
	int i = 0;

	//1. 현재 row와 해당 row의 endX를 구한다.
	Long endX = this->notePadForm->row->GetLength();

	//2. caretX를 초기화 시켜준다.
	this->notePadForm->location->caret->x = 0;

	//3. currentX번째 만큼 반복한다.
	while (i < endX) {
		//i번째 문자를 얻는다.
		character = this->notePadForm->row->GetChild(i);
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

int GetTextSize::totalCalculateX(string character) {

	char chaCharacter[3];
	int i = 0;

	//1. caretX를 초기화 시켜준다.
	this->notePadForm->location->caret->x = 0;

	//2. currentX번째 만큼 반복한다.
	while (i < character.length()) {

		//i번째 문자를 얻는다.
		chaCharacter[0] = character.at(i);

		if (chaCharacter[0] >= 0 && chaCharacter[0] < 128) {
			chaCharacter[1] = '\0';
		}
		else {
			i++;
			chaCharacter[1] = character.at(i);
			chaCharacter[2] = '\0';
		}

		//CalculateX를 호출한다.
		CalculateX(chaCharacter);

		i++;
	}

	Long caretX = this->notePadForm->location->caret->GetX(); //최근까지의 너비 
	return caretX;
}

Long GetTextSize::GetMouseY(Long y) {

	Long height = 0;
	Long strSizeY = GetX(131);
	int i = 0;
	int currentY;

	//1. note의 끝이 아니고, 글자 높이의 합이 y좌표 이내인 동안 반복한다.
	while (i<this->notePadForm->note->GetLength() && y >= height) {
		height = height + strSizeY;
		i++;
	}
	i--;
	currentY = i;

	return currentY;
}

Long GetTextSize::GetMouseX(Long x, Glyph* row) {
	Glyph* character;
	string strCharacter;
	char chaCharacter[3];
	int i = 0;
	Long currentWidth;
	Long beforeWidth = 0;
	int halfWidth;
	Long currentX;

	//1. caretX를 초기화 시켜준다.
	this->notePadForm->location->caret->x = 0;

	//2. row의 끝이 아니고, 글씨 너비의 합이 x좌표 이내인 동안 반복한다.
	while (i < row->GetLength() && x >= this->notePadForm->location->caret->x) {

		//2.1. beforeCaretX를 구해놓는다.
		beforeWidth = this->notePadForm->location->caret->x;

		//2.1. i번째 문자를 얻는다.
		character = row->GetChild(i);
		strCharacter = character->GetContent();
		strcpy(chaCharacter, strCharacter.c_str());

		if (chaCharacter[0] >= 0 && chaCharacter[0] < 128) {
			chaCharacter[1] = '\0';
		}
		else {
			chaCharacter[2] = '\0';
		}

		//2.2. CalculateX를 호출한다.
		CalculateX(chaCharacter);
		i++;
	}
	i--;
	currentWidth = this->notePadForm->location->caret->x;

	//3. 걸린 글자의 반의 너비를 구한다.
	halfWidth = (currentWidth - beforeWidth) / 2;

	//4. beforeWidth의 너비에 halfWidth를 더한 너비가 x를 초과하면 오른쪽에 커서를 둔다.
	if (beforeWidth + halfWidth < x) {
		i++;
	}

	currentX = i;

	return currentX;
}

Long GetTextSize::GetLeftX(Long currentX, Glyph* row) {
#if 0
	Glyph* character;
	string strCharacter;

	//1. 공백이거나 row의 처음이 아닌동안 반복한다.
	currentX--;
    character = row->GetChild(currentX);
	strCharacter = character->GetContent();

	while (currentX >= 0 && strCharacter == " ") {
		character = row->GetChild(currentX);
		strCharacter = character->GetContent();
		currentX--;
	}

	//2. 공백 전이거나 row의 끝이 아닌동안 반복한다.
	while (currentX >= 0 && strCharacter != " " ) {
		character = row->GetChild(currentX);
		strCharacter = character->GetContent();
		currentX--;
	}
	if (currentX >= 0) {
		currentX = currentX + 2;
	}
	else {
		currentX = 0;
	}

	return currentX;
#endif

	Glyph* character;
	string strCharacter;
	Long length;

	//3. PreviousWord를 호출했을 때
	  //3.1. 현재 줄을 찾는다.
	//3.2. 해당 줄의 current를 찾는다.

	//3.3. current가 0보다 큰 경우,
	if (currentX > 0) {
		//3.3.1. 커서 앞이 공백으로 시작하면, 
		currentX--;
		character = row->GetChild(currentX);
		strCharacter = character->GetContent();

		if (strCharacter == " ") {
			//1. x가 0보다 크고, 공백일 동안 반복한다.
			while (currentX > 0 && strCharacter == " ") {
				currentX--;
				character = row->GetChild(currentX);
				strCharacter = character->GetContent();
			}

			//2. x가 0보다 크면, 
				//2.1. x가 0보다 크고, 공백이 아닌동안 반복한다.
			while (currentX > 0 && strCharacter != " ") {
				currentX--;
				character = row->GetChild(currentX);
				strCharacter = character->GetContent();
}
			if (currentX > 0) {
				currentX++;
			}

		  }
		//3.3.2. 커서 앞이 공백으로 시작하지 않으면, X가 0보다 크고, 공백이 아닌동안 반복한다.
		else {
			while (currentX > 0 && strCharacter != " ") {
				currentX--;
				character = row->GetChild(currentX);
				strCharacter = character->GetContent();
			}
			if (currentX > 0) {
				currentX++;
			}
		}
	}
	return currentX;
}

Long GetTextSize::GetRightX(Long currentX, Glyph* row) {
#if 0
	Glyph* character;
	string strCharacter;

	//1.currentX--번째 글자가 공백인지 확인한다.
	currentX--;
	if (currentX >= 0) {
		character = row->GetChild(currentX);
		strCharacter = character->GetContent();
		currentX++;
	}

	else {
	
		character = row->GetChild(currentX);
		strCharacter = character->GetContent();
	}

	//2. currentX가 0이거나 왼쪽이 공백, 오른쪽이 글자 또는 끝이 아닌 경우,
	if (currentX <= 0 || !(strCharacter == " "  )) {

		//1. 현재 위치에서 공백 전이거나, row의 끝이 아닌동안 반복한다.
		character = row->GetChild(currentX);
		strCharacter = character->GetContent();

		while (currentX < row->GetLength() && strCharacter!=" ") {

		}
	}
	return currentX;
#endif

	Glyph* character;
	string strCharacter;
	string leftCharacter = " ";
	string rightCharacter = " ";
	Long length;

	//4. NextWord를 호출하였을 때
	 //4.1. 현재 줄을 찾는다.
	length = row->GetLength();

	//4.2. currentX를 기준으로 왼쪽이 공백이고 오른쪽이 글자 또는 row의 끝이면 기존 currentX를 리턴한다.
	currentX--;
	if (currentX >= 0) {
		character = row->GetChild(currentX);
		leftCharacter = character->GetContent();	
	}
	currentX++;
	if (currentX < length) {
		character = row->GetChild(currentX);
		rightCharacter = character->GetContent();
	}


	//4.3. current가 row의 length보다 작은 경우, 
	if (currentX < length && !(leftCharacter == " " && rightCharacter !=" ")) {
		//4.3.1. 커서 앞에 문자로 시작하면,
		character = row->GetChild(currentX);
		strCharacter = character->GetContent();
		if (strCharacter != " ") {
			//1. x가 length보다 작고, 문자일 동안 반복한다.
			while (currentX < length - 1 && strCharacter != " ") {
				currentX++;
				character = row->GetChild(currentX);
				strCharacter = character->GetContent();
			}
			if (currentX <= length - 1 && strCharacter != " ") {
				currentX++;
			}

			//2. x가 length보다 작으면, length보다 작고 공백일 동안 반복한다.
			while (currentX < length - 1 && strCharacter == " ") {
				currentX++;
				character = row->GetChild(currentX);
				strCharacter = character->GetContent();
			}
			if (currentX <= length - 1 && strCharacter == " ") {
				currentX++;
			}

		}
		//4.3.2. 커서 앞이 공백으로 시작하면, x가 length보다 작고 공백일 동안 반복한다.
		else {
			while (currentX < length-1 && strCharacter == " ") {
				currentX++;
				character = row->GetChild(currentX);
				strCharacter = character->GetContent();
				//currentX++;
			}
		}
	}

	return currentX;
}

Long GetTextSize::GetLargestWidth() {
	int i;
	int j = 0;
	Long noteLength = this->notePadForm->note->GetLength();
	Long rowLength;
	Glyph* character;
	string strCharacter;
	char chaCharacter[3];
	int currentLargestWidth = 0;
	int caretX_;
	Long pageX = 0;
	Glyph* row;

	while (j < noteLength) {
		row = this->notePadForm->note->GetChild(j);
		rowLength = row->GetLength();

		//3.1.1. 해당 줄의 너비를 구한다.

		this->notePadForm->location->caret->x = 0;
		i = 0;
		while (i < rowLength) {
			character = row->GetChild(i);

			if (character == NULL) {
				i++;
				if (i < rowLength) {
					character = row->GetChild(i);
				}
				else {
					break;
				}
			}
			//#endif

			strCharacter = character->GetContent();
			strcpy(chaCharacter, strCharacter.c_str());

			if (chaCharacter[0] >= 0 && chaCharacter[0] < 128) {
				chaCharacter[1] = '\0';
			}
			else {
				chaCharacter[2] = '\0';
			}
			// Location의 CalculateX를 호출한다.
			this->notePadForm->getTextSize->CalculateX(chaCharacter);
			i++;
		}
		caretX_ = this->notePadForm->location->caret->GetX();

		if (caretX_ > currentLargestWidth) {
			currentLargestWidth = caretX_;
		}

		j++;
	}

	return currentLargestWidth;
}
