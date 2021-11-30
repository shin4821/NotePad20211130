//GetInformation.h
#ifndef _GETINFORMATION_H
#define _GETINFORMATION_H
#include<afxwin.h>
#include"resource.h"

class NotePadForm;
class GetInformation :public CDialog {
public:
	GetInformation(NotePadForm* notePadForm, CWnd* parent = NULL);
	~GetInformation();

public:
	enum{IDD = IDD_INFORMATION};

public:
	NotePadForm* notePadForm;
};

#endif //_GETINFORMATION_H
