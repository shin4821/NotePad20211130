//NotePadForm.h
#ifndef _NOTEPADFORM_H
#define _NOTEPADFORM_H
#include<afxwin.h>
#include"Subject.h"
#include<string>
using namespace std;
typedef signed long int Long;

class GetTextSizeForDpi;
class Preview;
class Visitor;
class WindowText;
class GetInformation;
class PageInfo;
class PageSetUpDialog;
class CommandStory;
class Command;
class ReplaceDialog;
class FindDialog;
class FindSameText;
class PasteText;
class CopyText;
class SelectWords;
class LineChange;
class FactoryProductScroll;
class Scroll;
class Thumb;
class FactoryProduct;
class Previous;
class Observer;
class Glyph;
class GetTextSize;
class Location;
class Font;
class NotePadForm :public CFrameWnd, public Subject {
public:
	NotePadForm();
	//virtual Long Attach(Observer* observer); //21.07.20 _ FOR Observe Pattern -->Subject�� �ű��
	//virtual Long Detach(Long index); //21.07.20 _ FOR Observe Pattern -->Subject�� �ű��
	//virtual void Notify(); //21.07.20 _ FOR Observe Pattern -->Subject�� �ű��

public:
	Glyph* note;
	Glyph* row;
	Long index;
	BOOL isFirstComposing;
	BOOL isFirstComposingForCommand;
	Long x;
	Long y;
	GetTextSize* getTextSize; //21.07.07. ����߰�
	Location* location; //21.07.07. ����߰�
	Font* font; //21.07.09. ����߰�
	CFont noteFont; //21.07.09. ����߰� (�׽�Ʈ��)
	BOOL isChanged; //21.07.15 ����߰�(Load��)
	Long caretWidth;
	FactoryProduct* factoryProduct;
	BOOL isOnImeCharForMove;
	BOOL isFirstComposingForMove;
	CRect m_rectCurHist; //21.07.28 ���̾˷α� ũ�� ���ϴ� ����
	Thumb* thumb;
	Scroll* scroll;//21.07.29 ��ũ���Լ� �߰�.
	Long xTextOut;
	Long yTextOut;
	BOOL isJumpOverForPrevious; // MovePage ���� (Previous, BackSpace)
	BOOL isJumpOverForNext; // MovePage ���� (Next)
	BOOL isUp;
	BOOL isDown;
	//BOOL isDeleteAction_; //MovePage ����
	BOOL isDoubleByte;
	int largestWidth;
	LineChange* lineChange;
	BOOL isLineChangeButtonClicked;
	//FactoryProductScroll* factoryProductScroll;
	SelectWords* selectWords;
	CopyText* copyText;
	PasteText* pasteText;
	FindSameText* findSameText;
	FindDialog* findDialog;
	ReplaceDialog* replaceDialog;
	Command* command;
	UINT nChar;
	CommandStory* commandStory;
	string letter; //WPARAM letter;
	BOOL isRedo;
	BOOL isUndo;
	UINT nSBCode;
	Long currentX; // Ŀ�ǵ� ���� ��, Undo, Redo �� �ٲ� ĳ���� ��ġ�� ����Ŭ�������� �����ϱ� ����.
 	Long currentY;
	BOOL isThereSelected; // ����� ������� �Ҷ� �뵵�� �������.
	PageSetUpDialog* pageSetUpDialog;
	PageInfo* pageInfo;
	GetInformation* getInformation;
	BOOL isMouseDrag;
	BOOL isMouseLeave;
	DEVMODE* pDevMode_;
	int printWidth; //(21.10.18 �߰�) �������Ҷ� �ʺ�
	int printHeight;//(21.10.18 �߰�) �������Ҷ� ����
	int rateWidth; //(21.10.18 �߰�) �������Ҷ� �ʺ�
	int rateHeight;//(21.10.18 �߰�) �������Ҷ� ����
	int printFontSize; //(21.10.18 �߰�) �������Ҷ� ��Ʈũ��
	CDC* printDC;
	int rowNumber; //(21.10.19 �߰�)
	LOGFONT m_font;
	CWinApp* pApp;
	int strSizeY;
	BOOL isKillFocus;
	BOOL isInsert;
	BOOL isLineChangeAppend;
	WindowText* windowText;
	BOOL isOpen;
	BOOL isOpenChanged;
	BOOL isShiftClicked;
	int ShiftX;
	int ShiftY;
	BOOL isShifting;
	BOOL isComposition;
	Long endTextY;
	Long firstTextY;
	BOOL isKeyBoard;
	Visitor* visitor;
	CDC dcTemp;
	Preview* preview;
	BOOL isEndDoc;
	GetTextSizeForDpi* getTextSizeForDpi;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnClose();
	afx_msg LRESULT OnImeComposition(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnImeStartComposition(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnImeChar(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
#if 0
	afx_msg void OnSaveButtonClicked();
	afx_msg void OnSaveByDifferentNameButtonClicked();
	afx_msg void OnOpenButtonClicked();
	afx_msg void OnFontButtonClicked();
	afx_msg void OnLineChangeButtonClicked();
	afx_msg void OnUnExecuteButtonClicked();
#endif
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFindButtonClicked();
	afx_msg void OnReplaceButtonClicked();
	//(21.09.01) �߰�
	afx_msg void OnMyCommand(UINT id);//int parm_control_id
	//(21.10.06) �߰�
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC); //ȭ�� ������(flickering) ���� �޼���

	DECLARE_MESSAGE_MAP()

#if 0 //-->Subject�� �ű��
private:
	Array<Observer*>observers; //21.07.20 _ FOR Observe Pattern
	Long capacity;
	Long length;
#endif

};



LRESULT CALLBACK SaveMessageBoxProc(int nCode, WPARAM wParam, LPARAM lParam); //�޼����ڽ� ��ŷ��
int SaveMessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT nType); //�޼����ڽ� ��ŷ��
#endif//_NOTEPADFORM_H
