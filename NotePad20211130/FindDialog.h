//FindDialog.h
#ifndef _FINDDIALOG_H
#define _FINDDIALOG_H
#include<afxdlgs.h> 
#include<afxwin.h>
#include"resource.h"

class NotePadForm;
class FindDialog : public CFindReplaceDialog{

public: 
	enum{IDD = IDD_FINDINGFORM};

public:
	NotePadForm* notePadForm;

public:
	FindDialog(NotePadForm* notePadForm);
	BOOL Create(LPCTSTR lpszFindWhat, DWORD dwFlags = FR_DOWN, CWnd* pParentWnd = NULL);
	BOOL PreTranslateMessage(MSG* pMsg); //ESC로 종료할 시 해줘야 하는 처리.

protected:
	afx_msg void OnMyCommand(UINT id);
	afx_msg void OnFindTextChange();
	afx_msg void OnFindNextButtonClicked();
	//afx_msg void OnCancelButtonClicked();
	afx_msg void OnCancel();
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()

};

#endif //_FINDDIALOG_H
