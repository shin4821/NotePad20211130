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

	//1. �ӽ� DC�� �����.
	CClientDC dc(this->notePadForm);
	Long i;
	CDC* dcTemp = this->notePadForm->GetDC();
	//dcTemp->CreateCompatibleDC(&dc);


#if 0
	CClientDC dc(this->notePadForm);
	CDC* pdc = this->notePadForm->GetDC();

	//��Ʈ�߰�***************************************************************
	CFont cFont;
	int fontSize = this->notePadForm->font->GetFontSize();
	string fontName = this->notePadForm->font->GetFontName();
	cFont.CreatePointFont(fontSize, CString(fontName.c_str()), pdc); //pdc
	dc.SelectObject(&cFont);
	//***********************************************************************
#endif

	//Dpi ������ ��Ʈ�߰� ***************************************************
	int fontSize = this->notePadForm->font->GetFontSize();
	string fontName = this->notePadForm->font->GetFontName();


	HFONT hFont;
	HFONT oldFont;
	LOGFONT printLogFont = this->notePadForm->m_font;
	printLogFont.lfHeight = -MulDiv(fontSize / 10, 600, 72);

	//��Ʈ�̸� ����
	strcpy_s(printLogFont.lfFaceName, fontName.c_str());

	hFont = CreateFontIndirect(&printLogFont);
	oldFont = (HFONT)dcTemp->SelectObject(hFont);
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
			strSize = dcTemp->GetTabbedTextExtent("��", 0, NULL);
			//strSize = dc.GetTabbedTextExtent("��", 0, NULL);

			this->asciis.Store(i, strSize.cx);
		}

		// ��Ű�� ������,
		else if (i == 129) {
			//����� ���Ѵ�.
			strSize = dcTemp->GetTabbedTextExtent(" ", 0, NULL);


			this->asciis.Store(i, strSize.cx * 8);
		}

		//������ ������
		else if (i == 130) {
			//2.1. ����� ���Ѵ�.
			strSize = dcTemp->GetTabbedTextExtent(" ", 0, NULL);
			//strSize = dc.GetTabbedTextExtent(" ", 0, NULL);

			this->asciis.Store(i, strSize.cx);
		}
		// �������� ���� ����
		else if (i == 131) {
			//2.1. ����� ���Ѵ�.
			strSize = dcTemp->GetTabbedTextExtent("��", 0, NULL);
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
			strSize = dcTemp->GetTabbedTextExtent(character_, 0, NULL); //GetOutputTabbedTextExtent
			//strSize = dc.GetTabbedTextExtent(character_, 0, NULL);


			this->asciis.Store(i, strSize.cx);
		}
	}
	//���ο� ��Ʈ ����
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

Long GetTextSizeForDpi::totalCalculateX(Long endX, Glyph* row) {
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



















