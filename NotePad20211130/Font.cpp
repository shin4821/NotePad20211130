//Font.cpp
#include"Font.h"
#include"NotePadForm.h"

Font::Font(NotePadForm* notePadForm) {
	this->notePadForm = notePadForm;
	this->fontSize = 10; //�⺻ ��Ʈũ��
	this->fontName = "����ü"; //�⺻ ��Ʈ����
}

Font::~Font() {
}

int Font::GetFontSize() {
	return this->fontSize;
}

string Font::GetFontName() {
	return this->fontName;
}

int Font::GetFontWeight() {
	return this->fontWeight;
}

bool Font::IsItalic() {
	return this->fontItalic;
}

bool Font::IsStrikeOut() {
	return this->fontStrikeOut;
}

bool Font::IsUnderLine() {
	return this->fontUnderLine;
}














