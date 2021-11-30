//PageInfo.h
#ifndef _PAGEINFO_H
#define _PAGEINFO_H
#include<afxwin.h>

class NotePadForm;
class PageInfo {
public:
	PageInfo(NotePadForm* notePadForm);
	~PageInfo();
	CString GetHeader();
	CString GetFooter();
	CString GetFormName();
	int GetPaperWidth();
	int GetPaperHeight();
	int GetOrientation();
	int GetTop();
	int GetLeft();
	int GetRight();
	int GetBottom();
	int GetActualWidth();
	int GetActualHeight();
	int GetPaperSize();

	void FixHeader(CString header);
	void FixFooter(CString footer);
	void FixFormName(CString formName);
	void FixPaperWidth(int paperWidth);
	void FixPaperHeight(int paperHeight);
	void FixOrientation(int orientation);
	void FixTop(int top);
	void FixLeft(int left);
	void FixRight(int right);
	void FixBottom(int bottom);
	void FixActualWidth(int actualWidth);
	void FixActualHeight(int actualHeight);
	void FixPaperSize(int paperSize);

private:
	int top;
	int left;
	int right;
	int bottom;
	int paperWidth;
	int paperHeight;
	int actualWidth;
	int actualHeight;
	int orientation;
	int paperSize;
	CString header;
	CString footer;
	CString formName;
	NotePadForm* notePadForm;

};

#endif//_PAGEINFO_H
