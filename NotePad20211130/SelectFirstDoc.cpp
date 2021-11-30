//SelectFirstDoc.cpp
#include"SelectFirstDoc.h"
#include"NotePadForm.h"
#include"FirstDoc.h"
#include"SelectWords.h"
#include"Glyph.h"
#include"Composite.h"
#include"Character.h"
#include"Note.h"
#include"Row.h"
#include"DummyRow.h"
#include"SingleByteCharacter.h"
#include"DoubleByteCharacter.h"
#include"CreateGlyph.h"
#include"CreateProduct.h"
#include"GetTextSize.h"
#include"Location.h"
#include"Font.h"
#include"Caret.h"
#include"Observer.h"
#include"Scroll.h"


SelectFirstDoc::SelectFirstDoc(NotePadForm* notePadForm)
	:KeyAction(notePadForm) {
}

SelectFirstDoc:: ~SelectFirstDoc() {
}

void SelectFirstDoc::KeyDown() {
	Long beforeCurrentX;
	Long beforeCurrentY;
	Long currentX;
	Long currentY;
	Glyph* row;


	//1. Up�ϱ� ��, ���� ��ǥ�� ��ġ�� ���Ѵ�.
	beforeCurrentY = this->notePadForm->note->GetCurrent();
	row = this->notePadForm->note->GetChild(beforeCurrentY);
	beforeCurrentX = row->GetCurrent();

	//2. ���� Down Ŭ������ keyDown �Լ��� �����ϰ� ������ش�.
	KeyAction* keyAction = new FirstDoc(this->notePadForm);
	keyAction->KeyDown();

	//3. ó�� �巡�� ������ ���̸�,
	if (this->notePadForm->selectWords->isDragging == FALSE) {

		//3.1. SelectWords�� firstX,Y�� next�ϱ� �� ��ǥ�� ���´�.
		this->notePadForm->selectWords->firstX = beforeCurrentX;
		this->notePadForm->selectWords->firstY = beforeCurrentY;
	}

	//4.SelectFirstDoc ���� ��ǥ�� SelectWords�� endX,Y�� ���´�.
	currentY = this->notePadForm->note->GetCurrent();
	row = this->notePadForm->note->GetChild(currentY);
	currentX = row->GetCurrent();

	this->notePadForm->selectWords->endX = currentX;
	this->notePadForm->selectWords->endY = currentY;

	//5. �巡�� ���� true�� �ٲ۴�.
	this->notePadForm->selectWords->isDragging = TRUE;

}