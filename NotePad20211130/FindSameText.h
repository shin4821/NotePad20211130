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
	void SearchText(); //ã��
	void ReplaceAndSearchText(); //�ٲٴ�
	void ReplaceAllText();  //��� �ٲٴ�

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

