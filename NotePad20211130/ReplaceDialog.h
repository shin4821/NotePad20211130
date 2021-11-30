//ReplaceDialog.h
#ifndef _REPLACEDIALOG_H
#define _REPLACEDIALOG_H
#include<afxdlgs.h> 
#include"resource.h"

class NotePadForm;
class ReplaceDialog : public CFindReplaceDialog {

public:
	enum{IDD = IDD_REPLACINGFORM};

public:
	NotePadForm* notePadForm;

public:
	ReplaceDialog(NotePadForm* notePadForm);
	BOOL Create(LPCTSTR lpszFindWhat, DWORD dwFlags = FR_DOWN, CWnd* pParentWnd = NULL);
	BOOL PreTranslateMessage(MSG* pMsg); //ESC로 종료할 시 해줘야 하는 처리.

protected:
	afx_msg void OnMyCommand(UINT id);
	afx_msg void OnFindTextChange();
	afx_msg void OnFineNextButtonClicked();
	afx_msg void OnReplaceButtonClicked();
	afx_msg void OnReplaceAllButtonClicked();
	afx_msg void OnCancel();
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()

};

#endif //_REPLACEDIALOG_H