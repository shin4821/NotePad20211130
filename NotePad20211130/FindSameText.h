//FindSameText.h
#ifndef _FINDSAMETEXT_H
#define _FINDSAMETEXT_H
#include<afxwin.h>

class FindAndReplace;
class NotePadForm;
class FindSameText {
public:
	FindSameText(NotePadForm* notePadForm);
	~FindSameText();
	void SearchText(); //찾다
	void ReplaceAndSearchText(); //바꾸다
	void ReplaceAllText();  //모두 바꾸다

public:
	NotePadForm* notePadForm;
	CString findStr;
	CString replaceStr;
	CString beforeFindStr;
	BOOL matchCase;
	BOOL searchDown;
	BOOL searchAround;
	BOOL exception;
	FindAndReplace* findAndReplace;
};

#endif// _FINDSAMETEXT_H

