//Visitor.cpp
#include"Visitor.h"
#include"NotePadForm.h"
#include"Glyph.h"
#include"Scroll.h"
#include"GetTextSize.h"


Visitor::Visitor(NotePadForm* notePadForm) {
	this->notePadForm = notePadForm;
	this->distanceX = 0;
	this->rowCount = 0;
	this->firstY = 0;
	this->firstX = 0;
	this->endY = 0;
	this->endX = 0;
}

Visitor::~Visitor() {

}

void Visitor::VisitNote(Glyph* note) {

	Glyph* row;
	int i = this->notePadForm->firstTextY;

	while (i < this->notePadForm->endTextY+1) {
		row = note->GetChild(i);

		//��� ����
		this->rowCount = i;
		this->distanceX = 0;

		row->Accept(this);
		i++;
	}

}


void Visitor::VisitNoteForSelect(Glyph* note) {

	Glyph* row;
	int i = this->firstY;

	while (i <= this->endY) {
		row = note->GetChild(i);

		//��� ���� 
		this->rowCount = i;
		this->distanceX = 0;

		//1. startX�� �����Ѵ�. *******************************
		if (i <= this->firstY) {
			this->startX = this->firstX;
		}
		else {
			this->startX = 0;
		}

		//2. limitX �� �����Ѵ�.
		if (i >= this->endY) {
			this->limitX = this->endX;
		}
		else {
			this->limitX = row->GetLength();
		}
		//*****************************************************

		row->AcceptForSelect(this);
		i++;
	}
}


void Visitor::VisitRowForSelect(Glyph* row) {

	Glyph* character;
	int i = this->startX;

	while (i < this->limitX) {

		character = row->GetChild(i);

		//��� ����) X Ŀ�������� ������ �Ÿ��� ����Ѵ�.
		this->distanceX = this->notePadForm->getTextSize->totalCalculateX(i, row);

		character->Accept(this);
		i++;
	}
}

void Visitor::VisitRow(Glyph* row) {

	Glyph* character;
	Long i = 0;
	int length = row->GetLength();

	while (i < length) {
		character = row->GetChild(i);

		//��� ����) X Ŀ�������� ������ �Ÿ��� ����Ѵ�.
		this->distanceX = this->notePadForm->getTextSize->totalCalculateX(i, row);

		character->Accept(this);
		i++;
	}


}


void Visitor::VisitCharacter(Glyph* character) {

	string content_;
	CString content;


	//������ ������ش�.(�ѱ��ھ�)
	content_ = character->GetContent();
	content = CString(content_.c_str());

	//(21.11.10.�߰�)
	if (content == "\t") {
		content = "        ";
	}
	
	this->notePadForm->dcTemp.TextOut(-this->notePadForm->scroll->siHorizon.nPos + this->distanceX,
		(this->notePadForm->strSizeY * this->rowCount) - this->notePadForm->scroll->siVertical.nPos, content);
}

void Visitor::SetFirstY(int firstY) {
	this->firstY = firstY;
}
void Visitor::SetFirstX(int firstX) {
	this->firstX = firstX;
}
void Visitor::SetEndY(int endY) {
	this->endY = endY;
}
void Visitor::SetEndX(int endX) {
	this->endX = endX;
}










