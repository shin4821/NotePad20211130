//Preview.h
#ifndef _PREVIEW_H
#define _PREVIEW_H
#include<afxwin.h>
#include<windef.h>
#include<afxext.h>
typedef signed long int Long;

class NotePadForm;
class Preview :public CFrameWnd {
public:
	Preview(NotePadForm* notePadForm); 

public:
	NotePadForm* notePadForm;

public:
	Long windowWidth;
	Long windowHeight;
	CToolBar m_wndToolBar;
	int line;
	int lastPage;
	Long m_nCurrentPage;
	//UINT IDC_EDIT_CURRENTPAGE;



protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnPrintIconClicked();
	//afx_msg void OnPaperInvertIconClicked();
	afx_msg void OnPageSetIconClicked();
	afx_msg void OnFirstIconClicked();
	afx_msg void OnPreviousIconClicked();
	afx_msg void OnNextIconClicked();
	afx_msg void OnLastIconClicked();
	//afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC); //화면 깜빡임(flickering) 방지 메세지
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
};

#endif//_PREVIEW_H
