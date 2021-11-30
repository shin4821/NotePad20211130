//SelectWords.h
#ifndef _SELECTWORDS_H
#define _SELECTWORDS_H
#include<afxwin.h>
#include<string>
using namespace std;

typedef signed long int Long;
class Glyph;
class NotePadForm;
class SelectWords {
public:
	SelectWords(NotePadForm* notePadForm);
	~SelectWords();
	Long GetCurrentX(Long x, Glyph* row);
	Long GetCurrentY(Long y);
	int GetFirstX();
	int GetFirstY();
	int GetEndX();
	int GetEndY();
	string GetSelectedWord();

public:
	NotePadForm* notePadForm;
	BOOL isDragging;
	int firstX;
	int firstY;
	int endX;
	int endY;
	int beforeFirstX;
	int beforeFirstY;
	int beforeEndX;
	int beforeEndY;
	int afterEndX;
	int afterEndY;
	int isReplaced;
	BOOL isLineChangeButtonClicked;
	int windowWidth;
};

#endif//_SELECTWORDS_H
