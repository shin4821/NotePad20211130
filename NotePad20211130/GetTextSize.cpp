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

	//��Ʈ�߰�***************************************************************
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
		//1. 0~31, 127�� ������ 0�� �迭�� �Ҵ��Ѵ�. (ũ��=0)
		if ((i >= 0 && i < 33) || i == 127) {
			this->asciis.Store(i, 0);
		}
		//2. �ѱ��� ������,
		else if (i == 128) {
			//2.1. ����� ���Ѵ�.
			strSize = dc.GetTabbedTextExtent("��", 0, NULL);
			//strSize = dc.GetTabbedTextExtent("��", 0, NULL);

			this->asciis.Store(i, strSize.cx);	
		}

		// ��Ű�� ������,
		else if (i == 129) {
			//����� ���Ѵ�.
			strSize = dc.GetTabbedTextExtent(" ", 0, NULL);


			this->asciis.Store(i, strSize.cx * 8);
		}

		//������ ������
		else if (i == 130) {
			//2.1. ����� ���Ѵ�.
			strSize = dc.GetTabbedTextExtent(" ", 0, NULL);
			//strSize = dc.GetTabbedTextExtent(" ", 0, NULL);

			this->asciis.Store(i, strSize.cx);
		}
		// �������� ���� ����
		else if (i == 131) {
			//2.1. ����� ���Ѵ�.
			strSize = dc.GetTabbedTextExtent("��", 0, NULL);
			//strSize = dc.GetTabbedTextExtent("��", 0, NULL);

			this->asciis.Store(i, strSize.cy);
		}
		//3. 1byte�� ������,
		else {
			//3.1. i�� CString ���� �ٲ۴�

			cCharacter = (char)i;

			//character = to_string(i);
			character_ = cCharacter;

			//3.2. ����� ���Ѵ�.
			strSize = dc.GetTabbedTextExtent(character_, 0, NULL); //GetOutputTabbedTextExtent
			//strSize = dc.GetTabbedTextExtent(character_, 0, NULL);


			this->asciis.Store(i, strSize.cx);
		}
	}
	//���ο� ��Ʈ ����
	cFont.DeleteObject();
	this->notePadForm->ReleaseDC(pdc);
}

GetTextSize::~GetTextSize() {
}

GetTextSize::GetTextSize(const GetTextSize& source) {//���������
	this->asciis = source.asciis;
	this->capacity = source.capacity;
	this->length = source.length;
}

GetTextSize& GetTextSize::operator=(const GetTextSize& source) {//ġȯ������
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

	//1. Ŀ���� �̵��� ��
	//1.1 ��Ű�� ��
	if (character[0] == '\t') {
		// ������ �⺻ ũ�� Ȯ���Ѵ�.
		strSizeX = GetX(129);

		// Caret�� x,y���� �����´�.
		caretX = this->notePadForm->location->caret->GetX();
		caretY = this->notePadForm->location->caret->GetY(); //this->caret->GetY();

		//1.3.5. Caret�� x�� ���� �����ش�.
		this->notePadForm->location->caret->x = this->notePadForm->location->caret->x + strSizeX;
	}
	//1.3. ���� �ƴϰ� �����̽� �� �ƴϰ� 1byte�� ��
	else if (character[1] == '\0' && character[0] != ' ') {

		//1.3.1. �Է¹��� ������ �ʺ� Ȯ���Ѵ�.
		charInt = character[0];
		strSizeX = GetX(charInt);

		//1.3.3. Caret�� x,y���� �����´�.
		caretX = this->notePadForm->location->caret->GetX();
		caretY = this->notePadForm->location->caret->GetY();

		//1.3.5. Caret�� x�� ���� �����ش�.
		this->notePadForm->location->caret->x = this->notePadForm->location->caret->x + strSizeX;

	}
	// ���� �ƴϰ� �����̽� ���� ��
	else if (character[1] == '\0' && character[0] == ' ') {
		// ������ �⺻ ũ�� Ȯ���Ѵ�.
		strSizeX = GetX(130);

		// Caret�� x,y���� �����´�.
		caretX = this->notePadForm->location->caret->GetX();
		caretY = this->notePadForm->location->caret->GetY();

		//1.3.5. Caret�� x�� ���� �����ش�.
		this->notePadForm->location->caret->x = this->notePadForm->location->caret->x + strSizeX;
	}

	//1.4. 2byte�϶�
	else if (character[2] == '\0') {
		//1.4.1. �ѱ��� �⺻ ũ�� Ȯ���Ѵ�.
		strSizeX = GetX(128);

		// Caret�� x,y���� �����´�.
		caretX = this->notePadForm->location->caret->GetX();
		caretY = this->notePadForm->location->caret->GetY();

		//1.3.5. Caret�� x�� ���� �����ش�.
		this->notePadForm->location->caret->x = this->notePadForm->location->caret->x + strSizeX;
	}

}

Long GetTextSize::totalCalculateX(Long endX, Glyph* row) {
	Glyph* character;
	string strCharacter;
	char chaCharacter[3];
	int i = 0;

	//1. caretX, currentX, row�� �Է¹޴´�. ���� caretX�� 0�̳�, ���� �ٲ� �� ���ؼ� ���ؾ��ϹǷ�,,
	//2. caretX�� �ʱ�ȭ �����ش�.
	
	this->notePadForm->location->caret->x = 0;

	//3. currentX��° ��ŭ �ݺ��Ѵ�.
	while (i < endX) {
		//i��° ���ڸ� ��´�.
		character = row->GetChild(i);

		//(21.11.02. �߰�)
		if (character == NULL) {
			i++;
			if (i < endX) {
				character = row->GetChild(i);
			}
			else {
				Long caretX = this->notePadForm->location->caret->GetX(); //�ֱٱ����� �ʺ� 
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

		//CalculateX�� ȣ���Ѵ�.
		CalculateX(chaCharacter);

		i++;
	}


	Long caretX = this->notePadForm->location->caret->GetX(); //�ֱٱ����� �ʺ� 

	return caretX;
}

Long GetTextSize::totalCalculateEndX() { // ���� �ٿ��� endX������ �ʺ� ���ϴ� �Լ�.
	Glyph* character;
	string strCharacter;
	char chaCharacter[3];
	int i = 0;

	//1. ���� row�� �ش� row�� endX�� ���Ѵ�.
	Long endX = this->notePadForm->row->GetLength();

	//2. caretX�� �ʱ�ȭ �����ش�.
	this->notePadForm->location->caret->x = 0;

	//3. currentX��° ��ŭ �ݺ��Ѵ�.
	while (i < endX) {
		//i��° ���ڸ� ��´�.
		character = this->notePadForm->row->GetChild(i);
		strCharacter = character->GetContent();
		strcpy(chaCharacter, strCharacter.c_str());

		if (chaCharacter[0] >= 0 && chaCharacter[0] < 128) {
			chaCharacter[1] = '\0';
		}
		else {
			chaCharacter[2] = '\0';
		}

		//CalculateX�� ȣ���Ѵ�.
		CalculateX(chaCharacter);

		i++;
	}


	Long caretX = this->notePadForm->location->caret->GetX(); //�ֱٱ����� �ʺ� 

	return caretX;
}

int GetTextSize::totalCalculateX(string character) {

	char chaCharacter[3];
	int i = 0;

	//1. caretX�� �ʱ�ȭ �����ش�.
	this->notePadForm->location->caret->x = 0;

	//2. currentX��° ��ŭ �ݺ��Ѵ�.
	while (i < character.length()) {

		//i��° ���ڸ� ��´�.
		chaCharacter[0] = character.at(i);

		if (chaCharacter[0] >= 0 && chaCharacter[0] < 128) {
			chaCharacter[1] = '\0';
		}
		else {
			i++;
			chaCharacter[1] = character.at(i);
			chaCharacter[2] = '\0';
		}

		//CalculateX�� ȣ���Ѵ�.
		CalculateX(chaCharacter);

		i++;
	}

	Long caretX = this->notePadForm->location->caret->GetX(); //�ֱٱ����� �ʺ� 
	return caretX;
}

Long GetTextSize::GetMouseY(Long y) {

	Long height = 0;
	Long strSizeY = GetX(131);
	int i = 0;
	int currentY;

	//1. note�� ���� �ƴϰ�, ���� ������ ���� y��ǥ �̳��� ���� �ݺ��Ѵ�.
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

	//1. caretX�� �ʱ�ȭ �����ش�.
	this->notePadForm->location->caret->x = 0;

	//2. row�� ���� �ƴϰ�, �۾� �ʺ��� ���� x��ǥ �̳��� ���� �ݺ��Ѵ�.
	while (i < row->GetLength() && x >= this->notePadForm->location->caret->x) {

		//2.1. beforeCaretX�� ���س��´�.
		beforeWidth = this->notePadForm->location->caret->x;

		//2.1. i��° ���ڸ� ��´�.
		character = row->GetChild(i);
		strCharacter = character->GetContent();
		strcpy(chaCharacter, strCharacter.c_str());

		if (chaCharacter[0] >= 0 && chaCharacter[0] < 128) {
			chaCharacter[1] = '\0';
		}
		else {
			chaCharacter[2] = '\0';
		}

		//2.2. CalculateX�� ȣ���Ѵ�.
		CalculateX(chaCharacter);
		i++;
	}
	i--;
	currentWidth = this->notePadForm->location->caret->x;

	//3. �ɸ� ������ ���� �ʺ� ���Ѵ�.
	halfWidth = (currentWidth - beforeWidth) / 2;

	//4. beforeWidth�� �ʺ� halfWidth�� ���� �ʺ� x�� �ʰ��ϸ� �����ʿ� Ŀ���� �д�.
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

	//1. �����̰ų� row�� ó���� �ƴѵ��� �ݺ��Ѵ�.
	currentX--;
    character = row->GetChild(currentX);
	strCharacter = character->GetContent();

	while (currentX >= 0 && strCharacter == " ") {
		character = row->GetChild(currentX);
		strCharacter = character->GetContent();
		currentX--;
	}

	//2. ���� ���̰ų� row�� ���� �ƴѵ��� �ݺ��Ѵ�.
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

	//3. PreviousWord�� ȣ������ ��
	  //3.1. ���� ���� ã�´�.
	//3.2. �ش� ���� current�� ã�´�.

	//3.3. current�� 0���� ū ���,
	if (currentX > 0) {
		//3.3.1. Ŀ�� ���� �������� �����ϸ�, 
		currentX--;
		character = row->GetChild(currentX);
		strCharacter = character->GetContent();

		if (strCharacter == " ") {
			//1. x�� 0���� ũ��, ������ ���� �ݺ��Ѵ�.
			while (currentX > 0 && strCharacter == " ") {
				currentX--;
				character = row->GetChild(currentX);
				strCharacter = character->GetContent();
			}

			//2. x�� 0���� ũ��, 
				//2.1. x�� 0���� ũ��, ������ �ƴѵ��� �ݺ��Ѵ�.
			while (currentX > 0 && strCharacter != " ") {
				currentX--;
				character = row->GetChild(currentX);
				strCharacter = character->GetContent();
}
			if (currentX > 0) {
				currentX++;
			}

		  }
		//3.3.2. Ŀ�� ���� �������� �������� ������, X�� 0���� ũ��, ������ �ƴѵ��� �ݺ��Ѵ�.
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

	//1.currentX--��° ���ڰ� �������� Ȯ���Ѵ�.
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

	//2. currentX�� 0�̰ų� ������ ����, �������� ���� �Ǵ� ���� �ƴ� ���,
	if (currentX <= 0 || !(strCharacter == " "  )) {

		//1. ���� ��ġ���� ���� ���̰ų�, row�� ���� �ƴѵ��� �ݺ��Ѵ�.
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

	//4. NextWord�� ȣ���Ͽ��� ��
	 //4.1. ���� ���� ã�´�.
	length = row->GetLength();

	//4.2. currentX�� �������� ������ �����̰� �������� ���� �Ǵ� row�� ���̸� ���� currentX�� �����Ѵ�.
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


	//4.3. current�� row�� length���� ���� ���, 
	if (currentX < length && !(leftCharacter == " " && rightCharacter !=" ")) {
		//4.3.1. Ŀ�� �տ� ���ڷ� �����ϸ�,
		character = row->GetChild(currentX);
		strCharacter = character->GetContent();
		if (strCharacter != " ") {
			//1. x�� length���� �۰�, ������ ���� �ݺ��Ѵ�.
			while (currentX < length - 1 && strCharacter != " ") {
				currentX++;
				character = row->GetChild(currentX);
				strCharacter = character->GetContent();
			}
			if (currentX <= length - 1 && strCharacter != " ") {
				currentX++;
			}

			//2. x�� length���� ������, length���� �۰� ������ ���� �ݺ��Ѵ�.
			while (currentX < length - 1 && strCharacter == " ") {
				currentX++;
				character = row->GetChild(currentX);
				strCharacter = character->GetContent();
			}
			if (currentX <= length - 1 && strCharacter == " ") {
				currentX++;
			}

		}
		//4.3.2. Ŀ�� ���� �������� �����ϸ�, x�� length���� �۰� ������ ���� �ݺ��Ѵ�.
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

		//3.1.1. �ش� ���� �ʺ� ���Ѵ�.

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
			// Location�� CalculateX�� ȣ���Ѵ�.
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
