//PageSetUpDialog.h
#ifndef _PAGESETUPDIALOG_H
#define _PAGESETUPDIALOG_H
#include<afxdlgs.h> 
#include<afxwin.h>
#include"resource.h"

class NotePadForm;
class PageSetUpDialog :public CPageSetupDialog { 
public:
	enum{IDD=IDD_PAGESETUP};
	
public:
	NotePadForm* notePadForm;

public:
	PageSetUpDialog(NotePadForm* notePadForm);
	virtual ~PageSetUpDialog();
	virtual BOOL OnInitDialog();   
	virtual INT_PTR DoModal();
	CString GetHeader();
	CString GetFooter();
	virtual void OnOK();

private:
	char(*header);
	char(*footer);

protected:
	
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};


#endif //_PAGESETUPDIALOG_H