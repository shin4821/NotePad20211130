//OnPrintCommand.cpp
#include"OnPrintCommand.h"
#include"NotePadForm.h"
#include"GetTextSize.h"
#include"Note.h"
#include"Font.h"
#include"LineChange.h"
//#include"CreateGlyph.h"
//#include"CreateProduct.h"
#include<afxdlgs.h>
#include"PageSetUpDialog.h"
#include"PageInfo.h"
#include"GetTextSizeForDpi.h"

OnPrintCommand::OnPrintCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
}

OnPrintCommand::~OnPrintCommand() {
}

void OnPrintCommand::Execute() {
	CString content;
	string content_;
	Glyph* row;
	int i = 0;
	int j = 0;
	int length;
	int strSizeY;
	int strSizeX;
	CSize strSize;
	int actualHeight;
	int compareHeight;
	bool isFirst = false;
	int paperWidth;
	CString header = this->notePadForm->pageInfo->GetHeader();
	CString footer = this->notePadForm->pageInfo->GetFooter();
	CString formName = this->notePadForm->pageInfo->GetFormName();
	int orientation = this->notePadForm->pageInfo->GetOrientation();
	int actualWidth = this->notePadForm->pageInfo->GetActualWidth();
	int virtualActualHeight = this->notePadForm->pageInfo->GetActualHeight();
	int top = this->notePadForm->pageInfo->GetTop();
	int left = this->notePadForm->pageInfo->GetLeft();
	int right = this->notePadForm->pageInfo->GetRight();
	int bottom = this->notePadForm->pageInfo->GetBottom();
	int virtualPaperWidth = this->notePadForm->pageInfo->GetPaperWidth();
	int virtualPaperHeight = this->notePadForm->pageInfo->GetPaperHeight();
	int paperSize = this->notePadForm->pageInfo->GetPaperSize();


	DEVMODE* pDevMode;

	//1. 공통대화상자(인쇄)를 만든다.
	CPrintDialog dlg_(FALSE, PD_ALLPAGES | PD_USEDEVMODECOPIES | PD_NOPAGENUMS | PD_HIDEPRINTTOFILE | PD_NOSELECTION);


	//2. 사용자가 정상적으로 파일을 열었으면,
	if (dlg_.DoModal() == IDOK) {

		//2.0. hdc와 pDC를 만든다.
		CDC* pDC = new CDC;
		pDC->Attach(dlg_.m_pd.hDC);

		//2.1. CPrintDialog의 m_pd 멤버에서 hDevMode를 가져온다. ************************************
		// 폰트 설정 전에 DevMode를 설정해 줘야한다. 안그러면 폰트 설정한것도 Reset 된다.

		//2.1.1. GlobalLock으로 dlg_의 hDevMode를 가져온다.
		pDevMode = (DEVMODE*)GlobalLock(dlg_.m_pd.hDevMode);


		//2.1.2. hDevMode의 필드를 설정해준다. (가로세로변환, 용지 사이즈)
		pDevMode->dmFields |= DM_ORIENTATION | DM_PAPERSIZE;
		
		pDevMode->dmPaperSize = paperSize;
		this->notePadForm->pDevMode_ = pDevMode;

		//2.1.4. 선택한 용지 방향에 따라 용지의 방향을 바꿔준다.
		if (orientation == 2) {
			pDevMode->dmOrientation = DMORIENT_LANDSCAPE; //가로 인쇄 설정한다.
		}
		
		//변경된 옵션을 바탕으로 dc를 업데이트 한다. 
		pDC->ResetDC(pDevMode);
		GlobalUnlock(dlg_.m_pd.hDevMode);
		//*******************************************************************************************

		//dc를 업데이트 한 후에야 설정된 옵션대로 크기를 구할 수 있다.
		this->notePadForm->printWidth = pDC->GetDeviceCaps(HORZRES);
		this->notePadForm->printHeight = pDC->GetDeviceCaps(VERTRES);
		Long printWidth = this->notePadForm->printWidth;
		Long printHeight = this->notePadForm->printHeight;

		this->notePadForm->printDC = pDC;
		this->notePadForm->rateWidth = pDC->GetDeviceCaps(HORZRES) / 25.4; //210
		this->notePadForm->rateHeight = pDC->GetDeviceCaps(VERTRES) / 25.4; //297

		int rate = pDC->GetDeviceCaps(HORZRES) / virtualPaperWidth;

		//2.2. 해당 DC에서 글자의 기본 높이를 구한다.
		//폰트추가***************************************************************
		CFont cFont;
		int fontSize = this->notePadForm->font->GetFontSize(); 
		this->notePadForm->printFontSize = fontSize;

		//기존 폰트 구조체 갖고온다.
		LOGFONT printLogFont = this->notePadForm->m_font;

		//해상도 적용한 폰트사이즈 적용 (논리크기)
		printLogFont.lfHeight = -MulDiv(fontSize / 10, 600, 72);
		
		HFONT hFont = CreateFontIndirect(&printLogFont);
		pDC->SelectObject(hFont);
		//***********************************************************************

		strSize = pDC->GetTabbedTextExtent("ㄱ", 0, NULL);
		strSizeY = strSize.cy;
		strSizeX = strSize.cx;

		//(21.10.27.추가) 현재 자동개행 되어있으면, 자동개행 풀어준다.
		if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
			this->notePadForm->lineChange->LineChangeButtonNotClicked();
		}

		//(21.11.30. 추가) 해상도 적용한다. ************************************************************************
       //해상도 적용한 GetTextSizeForDpi 배열을 만든다.
		if (this->notePadForm->getTextSizeForDpi == NULL) {
			this->notePadForm->getTextSizeForDpi = new GetTextSizeForDpi(this->notePadForm);
		}

		//자동개행 시켜준다.
		printWidth = printWidth - ((left * 23.57) + (right * 23.57));
		this->notePadForm->lineChange->LineChangeButtonClickedForDpi(printWidth); //228줄 대신 여기에 4961을

		int length = this->notePadForm->note->GetLength();
		//************************************************************************************************************

		//해상도 적용한 GetTextSizeForDpi 배열을 지운다.
		if (this->notePadForm->getTextSizeForDpi != NULL) {
			delete this->notePadForm->getTextSizeForDpi;
			this->notePadForm->getTextSizeForDpi = NULL;
		}

		//2.2. 프린터로 출력시에 출력하는 정보 설정
		DOCINFO docinfo = { sizeof(DOCINFO),"Print1: Test",NULL };

		//2.3. 현재 가로, 세로 중 무엇을 선택했는지 확인한다.
		//수직인쇄=DMORIENT_PORTRAIT, 수평인쇄=DMORIENT_LANDSCAPE


		//2.6 프린터 DC에 프린터 정보를 출력 시작을 알린다.
		pDC->StartDoc(&docinfo);


		//2.7. actualWidth, actualHeight를 이용하여 자동개행 후 문서 구한다.
		

		//(기존에 임시로 해두었던 간격)
		//this->notePadForm->lineChange->LineChangeButtonClicked(actualWidth * 4.65);  

		//(비례식으로 한번 처리해보기.. 997은 A4기준)
		//테스트
		//this->notePadForm->lineChange->LineChangeButtonClicked(actualWidth * 4.74);
		//length = this->notePadForm->note->GetLength();
		
		int l;

		//2.7.1. 바닥글이 있을 경우, 바닥글의 높이 역시 빼준다.
		if (footer != "") {
			if (top == 0 && bottom == 0) {
				actualHeight = this->notePadForm->printHeight - (strSizeY*2); //virtualActualHeight * 22.5 - strSizeY; 
			}
			else if (bottom > 0) {
				actualHeight = this->notePadForm->printHeight - (strSizeY*2) - (bottom * 23.57);  //virtualActualHeight * 24.5 - strSizeY;
			}
			l = 1;
		}
		else {
			if (top == 0 && bottom == 0) {
				actualHeight = this->notePadForm->printHeight - strSizeY;
			}
			else if(bottom >0 ){
				actualHeight = this->notePadForm->printHeight - strSizeY - (bottom * 23.57); // virtualActualHeight * 24.5;
			}

			l = 0;
		}



		//2.8. 프린터 DC에 출력할 데이터를 기록한다.
		while (i < length) { //&& actualHeight > compareHeight

			j = 0;

			//2.8.1. 새로운 페이지를 알린다.
			pDC->StartPage();
			compareHeight = (strSizeY * j) + (top * 23.57);
			
			isFirst = true;

			bool isFirstCount = false;
			if (i == 0) {
				isFirstCount = true;
			}

			while (i < length && actualHeight > compareHeight) { 

				row = this->notePadForm->note->GetChild(i);
				content_ = row->GetContent();
				content = CString(content_.c_str());

				//1. 첫번째이고 머리글이 있으면,
				if (isFirst == true && header != "") {

					//1.1. 가운데 정렬하여 TextOut 한다.
					pDC->DrawText(header, CRect(0, top * 23.57, virtualPaperWidth * 23.57, virtualPaperHeight * 23.57),
						DT_SINGLELINE | DT_CENTER ); //| DT_VCENTER
				}
				else {
					pDC->TextOut(left * 23.57, (strSizeY * j) + (top * 23.57), content);
					i++;
				}
				j++;
				compareHeight = (strSizeY * j) + (top * 23.57);
				isFirst = false;

			}
			//(21.10.19 추가)
			if (isFirstCount == true) {
				this->notePadForm->rowNumber = i + l;
			}

			//2.8.2. 바닥글이 있으면 마지막 줄을 바닥글로 출력한다.
			pDC->DrawText(footer, CRect(0, (virtualPaperHeight * 23.57) - (bottom * 23.57 + strSizeY),
				virtualPaperWidth * 23.57, virtualPaperHeight * 23.57), DT_SINGLELINE | DT_CENTER);
#if 0
			//2.8.2.1. compareHeight이 actualHeight 넘었으면 그 다음줄에 바닥글 출력한다.
			if (i < length && footer != "") { //(strSizeY * j) + (top * 23.57) + strSizeY

				pDC->DrawText(footer, CRect(0, (strSizeY * j) + (top * 23.57), virtualPaperWidth * 23.57,
					virtualPaperHeight * 23.57), DT_SINGLELINE | DT_CENTER);
			}
			//2.8.2.2. compareHeight이 actualHeight을 넘지 못하였으면 바닥글 여분에 따라 출력한다.
			else if (i >= length && footer != "") {

				pDC->DrawText(footer, CRect(0, (virtualPaperHeight * 23.57) - (bottom * 23.57 + strSizeY),
					virtualPaperWidth * 23.57, virtualPaperHeight * 23.57), DT_SINGLELINE | DT_CENTER);
			}
#endif

			//2.9. 한 페이지의 끝을 알린다.
			pDC->EndPage();
		}

		//(21.10.27.추가) 현재 자동개행 되어있으면, 자동개행 풀어주고 다시 자동개행 한다.
		if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
			this->notePadForm->lineChange->LineChangeButtonNotClicked();
			this->notePadForm->lineChange->LineChangeButtonClicked();
		}
		//2.9. 자동개행된 문서를 다시 풀어준다.
		else {
			this->notePadForm->lineChange->LineChangeButtonNotClicked();
		}

		//3.0. 데이터를 기록한 DC를 제저장한다.
		//::RestoreDC(hdc, -1);
		pDC->RestoreDC(-1);

		//3.1. 정보 출력 종료
		//::EndDoc(hdc);
		pDC->EndDoc();


		pDC->DeleteDC();
		pDC->Detach();
		delete pDC;


	}
}

