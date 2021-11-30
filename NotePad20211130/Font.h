//Font.h
#ifndef _FONT_H
#define _FONT_H
#include<string>
using namespace std;

class NotePadForm;
class Font {
public:
	Font(NotePadForm* notePadForm);
	~Font();
	//CFont SetFont(int size, CString fontName, CDC* pDC);
	int GetFontSize();
	string GetFontName();
	int GetFontWeight();
	bool IsItalic();
	bool IsStrikeOut();
	bool IsUnderLine();

public:
	NotePadForm* notePadForm;
	int fontSize;
	string fontName;
    int fontWeight; //�β�
	bool fontItalic; //����
	bool fontStrikeOut; //��Ҽ�
	bool fontUnderLine; //����

};

#endif//_FONT_H

