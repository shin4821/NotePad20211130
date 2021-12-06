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
	ON_WM_ERASEBKGND() //화면 깜빡임(flickering) 방지 메세지
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

	//1. ToolBar를 만든다.
	if (!this->m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | 
		CBRS_FLYBY | CBRS_SIZE_DYNAMIC) || !this->m_wndToolBar.LoadToolBar(IDR_TOOLBAR)) {
		
	
		TRACE0("도구 모음을 만들지 못했습니다.\n");
		return -1;     
	}

	this->m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&this->m_wndToolBar);


	//2. Edit Control를 만든다.
	//2.1. 현재 윈도우의 크기를 구한다.
	CRect rect;
	this->GetClientRect(rect);

	CEdit *cEdit = new CEdit;
	cEdit->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, CRect(260, 6, 300, 38), this, 50000);
	//CRect(330, 6, 370, 38)
	CString currentPage = "1";
	GetDlgItem(50000)->SetWindowText(currentPage);


	//3. Static Text를 만든다.

	//3.1. 슬라이드 static을 만든다.
	CStatic* cStaticSlide = new CStatic;
	cStaticSlide->Create(_T("/"), WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(300, 9, 320, 38), this, 50001);

	//3.2. 전체 쪽수 static을 만든다.
	CStatic* totalPage = new CStatic;
	// CRect(390, 9, 410, 38)
	totalPage->Create(_T(" "), WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(320, 9, 340, 38), this, 50002);


	//(21.10.18 추가) OnPrintCommand의 dc에서 데이터를 갖고 온다.
	Long rateWidth = this->notePadForm->rateWidth;
	Long rateHeight = this->notePadForm->rateHeight;
	Long printWidth = this->notePadForm->printWidth;
	Long printHeight = this->notePadForm->printHeight;


	// 인쇄dc가 없는 경우, 대화상자를 띄우지 않고 객체를 생성한다.
//	if (this->notePadForm->printDC == NULL) {
		CPrintDialog dlg(FALSE, PD_RETURNDEFAULT);
		dlg.DoModal();

		CDC* pDC = new CDC;
		pDC->Attach(dlg.m_pd.hDC);

		//1. GlobalLock으로 dlg_의 hDevMode를 가져온다.
		DEVMODE* pDevMode = (DEVMODE*)GlobalLock(dlg.m_pd.hDevMode);

		//2. hDevMode의 필드를 설정해준다. (가로세로변환, 용지 사이즈)
		int paperSize = this->notePadForm->pageInfo->GetPaperSize();
		pDevMode->dmFields |= DM_ORIENTATION | DM_PAPERSIZE;
		pDevMode->dmPaperSize = paperSize;

		//3. 선택한 용지 방향에 따라 용지의 방향을 바꿔준다.
		int orientation = this->notePadForm->pageInfo->GetOrientation();
		if (orientation == 2) {
			pDevMode->dmOrientation = DMORIENT_LANDSCAPE; //가로 인쇄 설정한다.
		}

		//5. 변경된 옵션을 바탕으로 dc를 업데이트 한다. 
		pDC->ResetDC(pDevMode);
		GlobalUnlock(dlg.m_pd.hDevMode);

		this->notePadForm->printWidth = pDC->GetDeviceCaps(HORZRES);
		this->notePadForm->printHeight = pDC->GetDeviceCaps(VERTRES);

		pDC->DeleteDC();
		pDC->Detach();
		delete pDC;
//	}


#if 0
	//4.만약 OnPrintCommand가 생성되지 않았으면, DoModal하여 dc를 설정한다.
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
	//0. pageInfo의 정보를 가져온다.
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



	//1. 임시 DC를 만든다.
	//CDC* pDC = GetDC();
	CPaintDC dc(this);
	CDC dcTemp;

	//2. 현재 미리보기 칸의 크기를 구한다.
	CRect m_rectCurHist; //다이알로그 크기 구하는 변수
	this->GetClientRect(m_rectCurHist);
	this->windowWidth = this->notePadForm->printWidth;
	this->windowHeight = this->notePadForm->printHeight;

	int previewWidth = m_rectCurHist.right - m_rectCurHist.left;
	int previewHeight = m_rectCurHist.bottom - m_rectCurHist.top;

	//(21.10.12)3. 현재 윈도우 크기에 맞게 비트맵을 만든다.
	//미리보기 클라이언트 화면 크기로 비트맵을 만들고 있었음. 
	//인쇄 dc를 통해 얻은 너비와 높이로 비트맵을 만들어서 화면의 종이 크기로 축소해야함.
	dcTemp.CreateCompatibleDC(&dc);

	//(21.10.18 추가) OnPrintCommand의 dc에서 데이터를 갖고 온다.
	Long rateWidth = this->notePadForm->rateWidth;
	Long rateHeight = this->notePadForm->rateHeight;
	Long printWidth = this->notePadForm->printWidth;
	Long printHeight = this->notePadForm->printHeight;

	dcTemp.SetMapMode(MM_TEXT);

	//PrintDC에서 dc를 가져와서 비트맵을 만든다.
	HBITMAP hbmp = ::CreateCompatibleBitmap(dc, this->windowWidth, this->windowHeight);
	HBITMAP oldBMP = (HBITMAP)dcTemp.SelectObject(hbmp);

	//5. 사각형을 그린다. (사각형을 그리는 행위가 필요하지 않다)
	int realWindowWidth = ((previewHeight - 75) * (virtualPaperWidth)) / (virtualPaperHeight);
	int realWindowHeight = (realWindowWidth * (virtualPaperHeight)) / (virtualPaperWidth);
	int x1 = (previewWidth - realWindowWidth) / 2;
	int x2 = x1 + (realWindowWidth);


	CRect rect(0, 0, printWidth, printHeight);
	CRect background(0, 0, previewWidth, previewHeight);

	//6. 바깥쪽을 회색으로 칠한다.
	dc.FillRect(&background, CBrush::FromHandle((HBRUSH)GetStockObject(GRAY_BRUSH)));

	//7. 안쪽을 흰색으로 칠한다.
	dcTemp.FillRect(&rect, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));


	CFont cFont;
	int fontSize = this->notePadForm->font->GetFontSize();
	//int fontSize = this->notePadForm->printFontSize;
	string fontName = this->notePadForm->font->GetFontName();


	//(21.10.27.추가) 현재 자동개행 되어 있을 경우, 자동개행을 풀어준다.
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
		this->notePadForm->lineChange->LineChangeButtonNotClicked();
	}


	HFONT hFont;
	HFONT oldFont;
	LOGFONT printLogFont = this->notePadForm->m_font;
	printLogFont.lfHeight = -MulDiv(fontSize / 10, 600, 72);
	//printLogFont.lfWidth = -MulDiv(fontSize / 10, 600, 72);

	//폰트이름 적용
	strcpy_s(printLogFont.lfFaceName, fontName.c_str());

	hFont = CreateFontIndirect(&printLogFont);
	oldFont = (HFONT)dcTemp.SelectObject(hFont);

	//ReleaseDC(pDC);

	//글자를 출력한다.
	CSize strSize = dcTemp.GetTabbedTextExtent("각", 0, NULL);
	int strSizeY = strSize.cy;

	//(21.11.30. 추가) 해상도 적용한다. ************************************************************************

	//해상도 적용한 GetTextSizeForDpi 배열을 만든다.
	if (this->notePadForm->getTextSizeForDpi == NULL) {
		this->notePadForm->getTextSizeForDpi = new GetTextSizeForDpi(this->notePadForm);
	}

	//자동개행 시켜준다.
	this->windowWidth = this->windowWidth - ((left * 23.57) + (right * 23.57));
	this->notePadForm->lineChange->LineChangeButtonClickedForDpi(this->windowWidth); //228줄 대신 여기에 4961을

	int length = this->notePadForm->note->GetLength();
	//************************************************************************************************************
#if 0
	int strSizeX = strSize.cx;

	printLogFont.lfWidth = -strSizeX;

	hFont = CreateFontIndirect(&printLogFont);
	oldFont = (HFONT)dcTemp.SelectObject(hFont);

	ReleaseDC(pDC);
#endif

	//3.3. 바닥글이 있을 경우, 바닥글의 높이 역시 빼준다.
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

	//(21.10.14. 추가)*********************************************************************
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


	//2.8.1. 첫번째 페이지를 출력한다.
	j = 0; 
	compareHeight = (strSizeY * j) + top * 23.57; //(strSizeY * j) + (top);

	isFirst = TRUE;
	int k = 1;

#if 0
	//인쇄DC 거치지 않고 바로 미리보기로 올 경우, rowNumber가 정해지지 않는다.
	if (this->notePadForm->rowNumber == 0) {
		while (i < length && actualHeight > compareHeight) { //actualHeight > compareHeight

			row = this->notePadForm->note->GetChild(i);
			content_ = row->GetContent();
			content = CString(content_.c_str());

			//1. 첫번째이고 머리글이 있으면,
			if (isFirst == TRUE && header != "") {

				//1.1. 가운데 정렬하여 TextOut 한다.
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

			//1. 첫번째이고 머리글이 있으면,
			if (isFirst == TRUE && header != "") {

				//1.1. 가운데 정렬하여 TextOut 한다.
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

		//1. 첫번째이고 머리글이 있으면,
		if (isFirst == TRUE && header != "") {

			//1.1. 가운데 정렬하여 TextOut 한다.
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

	//lastPage 구했으면 static에 업데이트 해준다.
	CString currentPage;
	currentPage.Format("%d", this->lastPage);
	GetDlgItem(50002)->SetWindowText(currentPage);

	//2.8.2. 바닥글이 있으면 마지막 줄을 바닥글로 출력한다.

	dcTemp.DrawText(footer, CRect(0, printHeight - (bottom * 23.57 + strSizeY),
		printWidth, printHeight), DT_SINGLELINE | DT_CENTER);

#if 0
	pDC->DrawText(footer, CRect(0, (virtualPaperHeight * 23.57) - (bottom * 23.57 + strSizeY),
		virtualPaperWidth * 23.57, virtualPaperHeight * 23.57), DT_SINGLELINE | DT_CENTER);
#endif

#if 0
	//2.8.2.1. compareHeight이 actualHeight 넘었으면 그 다음줄에 바닥글 출력한다.
	if (i < length && footer != "") {
		dcTemp.DrawText(footer, CRect(0, (strSizeY* j) + (top * 23.57), printWidth,
			printHeight + 400), DT_SINGLELINE | DT_CENTER); 
	}
	//2.8.2.2. compareHeight이 actualHeight을 넘지 못하였으면 바닥글 여분에 따라 출력한다.
	else if (i >= length && footer != "") {
		dcTemp.DrawText(footer, CRect(0, (virtualPaperHeight * 23.57) - (bottom * 23.57 + strSizeY),
			printWidth, printHeight), DT_SINGLELINE | DT_CENTER);
	} 
#endif
	//2.9. 한 페이지의 끝을 알린다.
	//dcTemp.EndPage();


	//4. 해당 비트맵 이미지를 현재 화면 크기에 맞게 축소시킨다. 
	int x = this->windowHeight / virtualPaperHeight;
	dc.SetStretchBltMode(HALFTONE);	
	dc.StretchBlt(x1, 50, realWindowWidth, previewHeight - 75, &dcTemp, 0, 0, printWidth, printHeight, SRCCOPY);


	dcTemp.SelectObject(oldBMP);
	::DeleteObject(hbmp);
	dcTemp.DeleteDC();


	//----------------------------------------------------------------------------------------------------
	//(21.10.27.추가)6. 자동개행 되어 있었으면 다시 자동개행 해준다.
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
		//풀어준다.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();
		//다시 자동개행한다.
		this->notePadForm->lineChange->LineChangeButtonClicked();
	}
	//5. 자동개행된 문서를 다시 풀어준다.
	else {
		this->notePadForm->lineChange->LineChangeButtonNotClicked();
	}

	//해상도 적용한 GetTextSizeForDpi 배열을 지운다.
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

	//2.1. Edit Control에 현재 페이지를 출력한다.
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




