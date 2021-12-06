//Preview.cpp
#include"Preview.h"
#include"NotePadForm.h"
#include"PageInfo.h"
#include"Font.h"
#include"LineChange.h"
#include"Note.h"
#include"resource.h"
#include"OnPrintCommand.h"
#include"OnPageSetUpCommand.h"
#include"GetTextSizeForDpi.h"


BEGIN_MESSAGE_MAP(Preview, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND() //ȭ�� ������(flickering) ���� �޼���
	ON_COMMAND(ID_BUTTON_PRINT, &Preview::OnPrintIconClicked)
	//ON_COMMAND(ID_BUTTON_INVERT, &Preview::OnPaperInvertIconClicked)
	ON_COMMAND(ID_BUTTON_PAGESET, &Preview::OnPageSetIconClicked)
	ON_COMMAND(ID_BUTTON_FIRST, &Preview::OnFirstIconClicked)
	ON_COMMAND(ID_BUTTON_PREVIOUS, &Preview::OnPreviousIconClicked)
	ON_COMMAND(ID_BUTTON_NEXT, &Preview::OnNextIconClicked)
	ON_COMMAND(ID_BUTTON_LAST, &Preview::OnLastIconClicked)
	ON_WM_SIZE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

BOOL Preview::OnEraseBkgnd(CDC* pDC) {
	return TRUE;
}


Preview::Preview(NotePadForm* notePadForm) { 
	this->notePadForm = notePadForm;
	this->line = 0;
	this->m_nCurrentPage = 1;

}

int Preview::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CFrameWnd::OnCreate(lpCreateStruct);

	//1. ToolBar�� �����.
	if (!this->m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | 
		CBRS_FLYBY | CBRS_SIZE_DYNAMIC) || !this->m_wndToolBar.LoadToolBar(IDR_TOOLBAR)) {
		
	
		TRACE0("���� ������ ������ ���߽��ϴ�.\n");
		return -1;     
	}

	this->m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&this->m_wndToolBar);


	//2. Edit Control�� �����.
	//2.1. ���� �������� ũ�⸦ ���Ѵ�.
	CRect rect;
	this->GetClientRect(rect);

	CEdit *cEdit = new CEdit;
	cEdit->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, CRect(260, 6, 300, 38), this, 50000);
	//CRect(330, 6, 370, 38)
	CString currentPage = "1";
	GetDlgItem(50000)->SetWindowText(currentPage);


	//3. Static Text�� �����.

	//3.1. �����̵� static�� �����.
	CStatic* cStaticSlide = new CStatic;
	cStaticSlide->Create(_T("/"), WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(300, 9, 320, 38), this, 50001);

	//3.2. ��ü �ʼ� static�� �����.
	CStatic* totalPage = new CStatic;
	// CRect(390, 9, 410, 38)
	totalPage->Create(_T(" "), WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(320, 9, 340, 38), this, 50002);


	//(21.10.18 �߰�) OnPrintCommand�� dc���� �����͸� ���� �´�.
	Long rateWidth = this->notePadForm->rateWidth;
	Long rateHeight = this->notePadForm->rateHeight;
	Long printWidth = this->notePadForm->printWidth;
	Long printHeight = this->notePadForm->printHeight;


	// �μ�dc�� ���� ���, ��ȭ���ڸ� ����� �ʰ� ��ü�� �����Ѵ�.
//	if (this->notePadForm->printDC == NULL) {
		CPrintDialog dlg(FALSE, PD_RETURNDEFAULT);
		dlg.DoModal();

		CDC* pDC = new CDC;
		pDC->Attach(dlg.m_pd.hDC);

		//1. GlobalLock���� dlg_�� hDevMode�� �����´�.
		DEVMODE* pDevMode = (DEVMODE*)GlobalLock(dlg.m_pd.hDevMode);

		//2. hDevMode�� �ʵ带 �������ش�. (���μ��κ�ȯ, ���� ������)
		int paperSize = this->notePadForm->pageInfo->GetPaperSize();
		pDevMode->dmFields |= DM_ORIENTATION | DM_PAPERSIZE;
		pDevMode->dmPaperSize = paperSize;

		//3. ������ ���� ���⿡ ���� ������ ������ �ٲ��ش�.
		int orientation = this->notePadForm->pageInfo->GetOrientation();
		if (orientation == 2) {
			pDevMode->dmOrientation = DMORIENT_LANDSCAPE; //���� �μ� �����Ѵ�.
		}

		//5. ����� �ɼ��� �������� dc�� ������Ʈ �Ѵ�. 
		pDC->ResetDC(pDevMode);
		GlobalUnlock(dlg.m_pd.hDevMode);

		this->notePadForm->printWidth = pDC->GetDeviceCaps(HORZRES);
		this->notePadForm->printHeight = pDC->GetDeviceCaps(VERTRES);

		pDC->DeleteDC();
		pDC->Detach();
		delete pDC;
//	}


#if 0
	//4.���� OnPrintCommand�� �������� �ʾ�����, DoModal�Ͽ� dc�� �����Ѵ�.
	if (rateWidth == 0 && rateHeight == 0 && printWidth == 0 && printHeight == 0) {

		CPrintDialog dlg(FALSE, PD_RETURNDEFAULT);
		dlg.DoModal();
	}
#endif

	Invalidate();
	return 0;
}

#if 0
void Preview::OnSize(UINT nType, int cx, int cy) {

}
#endif

void Preview::OnPaint() {
	//0. pageInfo�� ������ �����´�.
	CString header = this->notePadForm->pageInfo->GetHeader();
	CString footer = this->notePadForm->pageInfo->GetFooter();
	CString formName = this->notePadForm->pageInfo->GetFormName();
	int orientation = this->notePadForm->pageInfo->GetOrientation();
	int actualWidth = this->notePadForm->pageInfo->GetActualWidth();
	int virtualActualHeigth = this->notePadForm->pageInfo->GetActualHeight();
	int top = this->notePadForm->pageInfo->GetTop();
	int left = this->notePadForm->pageInfo->GetLeft();
	int right = this->notePadForm->pageInfo->GetRight();
	int bottom = this->notePadForm->pageInfo->GetBottom();
	int virtualPaperWidth = this->notePadForm->pageInfo->GetPaperWidth();
	int virtualPaperHeight = this->notePadForm->pageInfo->GetPaperHeight();
	int paperSize = this->notePadForm->pageInfo->GetPaperSize();
	int actualHeight;
	CString content;
	string content_;
	Glyph* row;
	int j;
	Long compareHeight;
	BOOL isFirst = FALSE;



	//1. �ӽ� DC�� �����.
	//CDC* pDC = GetDC();
	CPaintDC dc(this);
	CDC dcTemp;

	//2. ���� �̸����� ĭ�� ũ�⸦ ���Ѵ�.
	CRect m_rectCurHist; //���̾˷α� ũ�� ���ϴ� ����
	this->GetClientRect(m_rectCurHist);
	this->windowWidth = this->notePadForm->printWidth;
	this->windowHeight = this->notePadForm->printHeight;

	int previewWidth = m_rectCurHist.right - m_rectCurHist.left;
	int previewHeight = m_rectCurHist.bottom - m_rectCurHist.top;

	//(21.10.12)3. ���� ������ ũ�⿡ �°� ��Ʈ���� �����.
	//�̸����� Ŭ���̾�Ʈ ȭ�� ũ��� ��Ʈ���� ����� �־���. 
	//�μ� dc�� ���� ���� �ʺ�� ���̷� ��Ʈ���� ���� ȭ���� ���� ũ��� ����ؾ���.
	dcTemp.CreateCompatibleDC(&dc);

	//(21.10.18 �߰�) OnPrintCommand�� dc���� �����͸� ���� �´�.
	Long rateWidth = this->notePadForm->rateWidth;
	Long rateHeight = this->notePadForm->rateHeight;
	Long printWidth = this->notePadForm->printWidth;
	Long printHeight = this->notePadForm->printHeight;

	dcTemp.SetMapMode(MM_TEXT);

	//PrintDC���� dc�� �����ͼ� ��Ʈ���� �����.
	HBITMAP hbmp = ::CreateCompatibleBitmap(dc, this->windowWidth, this->windowHeight);
	HBITMAP oldBMP = (HBITMAP)dcTemp.SelectObject(hbmp);

	//5. �簢���� �׸���. (�簢���� �׸��� ������ �ʿ����� �ʴ�)
	int realWindowWidth = ((previewHeight - 75) * (virtualPaperWidth)) / (virtualPaperHeight);
	int realWindowHeight = (realWindowWidth * (virtualPaperHeight)) / (virtualPaperWidth);
	int x1 = (previewWidth - realWindowWidth) / 2;
	int x2 = x1 + (realWindowWidth);


	CRect rect(0, 0, printWidth, printHeight);
	CRect background(0, 0, previewWidth, previewHeight);

	//6. �ٱ����� ȸ������ ĥ�Ѵ�.
	dc.FillRect(&background, CBrush::FromHandle((HBRUSH)GetStockObject(GRAY_BRUSH)));

	//7. ������ ������� ĥ�Ѵ�.
	dcTemp.FillRect(&rect, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));


	CFont cFont;
	int fontSize = this->notePadForm->font->GetFontSize();
	//int fontSize = this->notePadForm->printFontSize;
	string fontName = this->notePadForm->font->GetFontName();


	//(21.10.27.�߰�) ���� �ڵ����� �Ǿ� ���� ���, �ڵ������� Ǯ���ش�.
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
		this->notePadForm->lineChange->LineChangeButtonNotClicked();
	}


	HFONT hFont;
	HFONT oldFont;
	LOGFONT printLogFont = this->notePadForm->m_font;
	printLogFont.lfHeight = -MulDiv(fontSize / 10, 600, 72);
	//printLogFont.lfWidth = -MulDiv(fontSize / 10, 600, 72);

	//��Ʈ�̸� ����
	strcpy_s(printLogFont.lfFaceName, fontName.c_str());

	hFont = CreateFontIndirect(&printLogFont);
	oldFont = (HFONT)dcTemp.SelectObject(hFont);

	//ReleaseDC(pDC);

	//���ڸ� ����Ѵ�.
	CSize strSize = dcTemp.GetTabbedTextExtent("��", 0, NULL);
	int strSizeY = strSize.cy;

	//(21.11.30. �߰�) �ػ� �����Ѵ�. ************************************************************************

	//�ػ� ������ GetTextSizeForDpi �迭�� �����.
	if (this->notePadForm->getTextSizeForDpi == NULL) {
		this->notePadForm->getTextSizeForDpi = new GetTextSizeForDpi(this->notePadForm);
	}

	//�ڵ����� �����ش�.
	this->windowWidth = this->windowWidth - ((left * 23.57) + (right * 23.57));
	this->notePadForm->lineChange->LineChangeButtonClickedForDpi(this->windowWidth); //228�� ��� ���⿡ 4961��

	int length = this->notePadForm->note->GetLength();
	//************************************************************************************************************
#if 0
	int strSizeX = strSize.cx;

	printLogFont.lfWidth = -strSizeX;

	hFont = CreateFontIndirect(&printLogFont);
	oldFont = (HFONT)dcTemp.SelectObject(hFont);

	ReleaseDC(pDC);
#endif

	//3.3. �ٴڱ��� ���� ���, �ٴڱ��� ���� ���� ���ش�.
	if (footer != "") {
		if (top == 0 && bottom == 0) {
			actualHeight = this->notePadForm->printHeight - (strSizeY * 2); //- (strSizeY * 2)
		}
		else {
			actualHeight = this->notePadForm->printHeight - (strSizeY * 2) - (bottom * 23.57); 
		}
	}
	else {
		if (top == 0 && bottom == 0) {
			actualHeight = this->notePadForm->printHeight - strSizeY; // - strSizeY
		}
		else {
			actualHeight = this->notePadForm->printHeight - (bottom * 23.57); // - strSizeY
		}
	}

	//(21.10.14. �߰�)*********************************************************************
#if 0
	int documentHeight = strSizeY * this->notePadForm->note->GetLength();
	this->lastPage = documentHeight / actualHeight;
	int pageLineCount = this->notePadForm->note->GetLength() / this->lastPage;
#endif


	//int i = (this->m_nCurrentPage - 1) * pageLineCount;
	int i = (this->m_nCurrentPage - 1) * this->line;
	BOOL isFirstLine = FALSE;

	if (i == 0) {
		isFirstLine = TRUE;
	}
	//*************************************************************************************


	//2.8.1. ù��° �������� ����Ѵ�.
	j = 0; 
	compareHeight = (strSizeY * j) + top * 23.57; //(strSizeY * j) + (top);

	isFirst = TRUE;
	int k = 1;

#if 0
	//�μ�DC ��ġ�� �ʰ� �ٷ� �̸������ �� ���, rowNumber�� �������� �ʴ´�.
	if (this->notePadForm->rowNumber == 0) {
		while (i < length && actualHeight > compareHeight) { //actualHeight > compareHeight

			row = this->notePadForm->note->GetChild(i);
			content_ = row->GetContent();
			content = CString(content_.c_str());

			//1. ù��°�̰� �Ӹ����� ������,
			if (isFirst == TRUE && header != "") {

				//1.1. ��� �����Ͽ� TextOut �Ѵ�.
				dcTemp.DrawText(header, CRect(0, top * 23.57, printWidth, printHeight), 
					DT_SINGLELINE | DT_CENTER); 
			}
			else { 
				dcTemp.TextOut(left * 23.57, (strSizeY * j) + (top * 23.57), content);
				i++;
			}

			j++;
			compareHeight = (strSizeY * j) + (top * 23.57);
			isFirst = FALSE;
		}

	}
	else {
		while (i < length && k <= this->notePadForm->rowNumber) {

			row = this->notePadForm->note->GetChild(i);
			content_ = row->GetContent();
			content = CString(content_.c_str());

			//1. ù��°�̰� �Ӹ����� ������,
			if (isFirst == TRUE && header != "") {

				//1.1. ��� �����Ͽ� TextOut �Ѵ�.
				dcTemp.DrawText(header, CRect(0, top * 23.57, printWidth, printHeight), 
					DT_SINGLELINE | DT_CENTER); 
			}
			else { 
				dcTemp.TextOut(left * 23.57, (strSizeY * j) + (top * 23.57), content); 
				i++;
			}
			isFirst = FALSE;
			j++;
			k++;
		}

	}
#endif


	while (i < length && actualHeight > compareHeight) { //actualHeight > compareHeight

		row = this->notePadForm->note->GetChild(i);
		content_ = row->GetContent();
		content = CString(content_.c_str());

		//1. ù��°�̰� �Ӹ����� ������,
		if (isFirst == TRUE && header != "") {

			//1.1. ��� �����Ͽ� TextOut �Ѵ�.
			dcTemp.DrawText(header, CRect(0, top * 23.57, printWidth, printHeight),
				DT_SINGLELINE | DT_CENTER);
		}
		else {
			dcTemp.TextOut(left * 23.57, (strSizeY * j) + (top * 23.57), content);
			i++;
		}

		j++;
		compareHeight = (strSizeY * j) + (top * 23.57);
		isFirst = FALSE;
	}

	if (isFirstLine == TRUE) {
		this->line = i;
		//this->lastPage = (length / (i + 1))+1; 
		int lastPage = length / i;
		float remainder = length % i;
		if (remainder > 0) {
			this->lastPage = lastPage + 1;
		}
		else {
			this->lastPage = lastPage;
		}
	}

	//lastPage �������� static�� ������Ʈ ���ش�.
	CString currentPage;
	currentPage.Format("%d", this->lastPage);
	GetDlgItem(50002)->SetWindowText(currentPage);

	//2.8.2. �ٴڱ��� ������ ������ ���� �ٴڱ۷� ����Ѵ�.

	dcTemp.DrawText(footer, CRect(0, printHeight - (bottom * 23.57 + strSizeY),
		printWidth, printHeight), DT_SINGLELINE | DT_CENTER);

#if 0
	pDC->DrawText(footer, CRect(0, (virtualPaperHeight * 23.57) - (bottom * 23.57 + strSizeY),
		virtualPaperWidth * 23.57, virtualPaperHeight * 23.57), DT_SINGLELINE | DT_CENTER);
#endif

#if 0
	//2.8.2.1. compareHeight�� actualHeight �Ѿ����� �� �����ٿ� �ٴڱ� ����Ѵ�.
	if (i < length && footer != "") {
		dcTemp.DrawText(footer, CRect(0, (strSizeY* j) + (top * 23.57), printWidth,
			printHeight + 400), DT_SINGLELINE | DT_CENTER); 
	}
	//2.8.2.2. compareHeight�� actualHeight�� ���� ���Ͽ����� �ٴڱ� ���п� ���� ����Ѵ�.
	else if (i >= length && footer != "") {
		dcTemp.DrawText(footer, CRect(0, (virtualPaperHeight * 23.57) - (bottom * 23.57 + strSizeY),
			printWidth, printHeight), DT_SINGLELINE | DT_CENTER);
	} 
#endif
	//2.9. �� �������� ���� �˸���.
	//dcTemp.EndPage();


	//4. �ش� ��Ʈ�� �̹����� ���� ȭ�� ũ�⿡ �°� ��ҽ�Ų��. 
	int x = this->windowHeight / virtualPaperHeight;
	dc.SetStretchBltMode(HALFTONE);	
	dc.StretchBlt(x1, 50, realWindowWidth, previewHeight - 75, &dcTemp, 0, 0, printWidth, printHeight, SRCCOPY);


	dcTemp.SelectObject(oldBMP);
	::DeleteObject(hbmp);
	dcTemp.DeleteDC();


	//----------------------------------------------------------------------------------------------------
	//(21.10.27.�߰�)6. �ڵ����� �Ǿ� �־����� �ٽ� �ڵ����� ���ش�.
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
		//Ǯ���ش�.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();
		//�ٽ� �ڵ������Ѵ�.
		this->notePadForm->lineChange->LineChangeButtonClicked();
	}
	//5. �ڵ������ ������ �ٽ� Ǯ���ش�.
	else {
		this->notePadForm->lineChange->LineChangeButtonNotClicked();
	}

	//�ػ� ������ GetTextSizeForDpi �迭�� �����.
	if (this->notePadForm->getTextSizeForDpi != NULL) {
		delete this->notePadForm->getTextSizeForDpi;
		this->notePadForm->getTextSizeForDpi = NULL;
	}
}

void Preview::OnPrintIconClicked() {
		
	Command* command = new OnPrintCommand(this->notePadForm);
	command->Execute();
}

#if 0
void Preview::OnPaperInvertIconClicked() {
	int orientation = this->notePadForm->pageInfo->GetOrientation();
	int paperSize = this->notePadForm->pageInfo->GetPaperSize();

	this->notePadForm->pDevMode_->dmFields |= DM_ORIENTATION | DM_PAPERSIZE;
	this->notePadForm->pDevMode_->dmPaperSize = paperSize;
	                  
}
#endif

void Preview::OnPageSetIconClicked() {
	Command* command = new OnPageSetUpCommand(this->notePadForm);
	command->Execute();

}

void Preview::OnFirstIconClicked() {

	this->m_nCurrentPage = 1;

	//2.1. Edit Control�� ���� �������� ����Ѵ�.
	CString currentPage;
	currentPage.Format("%d", this->m_nCurrentPage);
	GetDlgItem(50000)->SetWindowText(currentPage);

	Invalidate();
}

void Preview::OnPreviousIconClicked() {

	this->m_nCurrentPage--;
	if (this->m_nCurrentPage < 1) {
		this->m_nCurrentPage = 1;
	}

	CString currentPage;
	currentPage.Format("%d", this->m_nCurrentPage);
	GetDlgItem(50000)->SetWindowText(currentPage);

	Invalidate();
}

void Preview::OnNextIconClicked() {

	this->m_nCurrentPage++;
	if (this->m_nCurrentPage > this->lastPage) {
		this->m_nCurrentPage = this->lastPage;
	}

	CString currentPage;
	currentPage.Format("%d", this->m_nCurrentPage);
	GetDlgItem(50000)->SetWindowText(currentPage);


	Invalidate();
}

void Preview::OnLastIconClicked() {

	this->m_nCurrentPage = this->lastPage;

	CString currentPage;
	currentPage.Format("%d", this->m_nCurrentPage);
	GetDlgItem(50000)->SetWindowText(currentPage);

	Invalidate();
}

void Preview::OnClose() {

	this->notePadForm->preview = NULL;

	CFrameWnd::OnClose();
}




