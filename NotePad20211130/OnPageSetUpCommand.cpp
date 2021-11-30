//OnPageSetUpCommand.cpp
#include"OnPageSetUpCommand.h"
#include"OnPreviewCommand.h"
#include"Command.h"
#include"NotePadForm.h"
#include"LineChange.h"
#include"PageSetUpDialog.h"
#include"PageInfo.h"
#include"Preview.h"
#include<afxdlgs.h>
#include<afxwin.h>

OnPageSetUpCommand::OnPageSetUpCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
#if 0
	this->top = 20;
	this->bottom = 20;
	this->left = 25;
	this->right = 25;
	this->paperWidth = 210; //A4 기본 너비
	this->paperHeight = 297; //A4 기본 높이
	this->actualWidth = 170; //A4 기본 너비 - 4000
 	this->actualHeight = 247; //A4 기본 높이 - 5000
	
	//this->paperSize = 기본 A4 사이즈로 하고싶은데, 대화상자를 만들어야 알 수 있으니 원,,,
#endif
}

OnPageSetUpCommand::~OnPageSetUpCommand() {

}

void OnPageSetUpCommand::Execute() {

	CSize paperSize;
	CString header;
	CString footer;
	HANDLE  hDevMode = NULL;
	int orientation;
	int actualWidth;
	int actualHeight;
	int paperSizeCx;
	int paperSizeCy;
	int beforeOrientation;

	//Modal 이어서 굳이 힙에 할당 안해도 됨.
	PageSetUpDialog pageSetUpDialog(this->notePadForm);

#if 0
	if (this->notePadForm->pageSetUpDialog == NULL) {
		this->notePadForm->pageSetUpDialog = new PageSetUpDialog(this->notePadForm);
	}
#endif

	//#if 0

	//3. 사용자가 열 파일을 정상적으로 선택 하였을 때
	if (IDOK == pageSetUpDialog.DoModal()) {
		//if (IDOK == this->notePadForm->pageSetUpDialog->DoModal()) {

			//3.1. 입력된 여백을 읽는다.
		this->notePadForm->pageInfo->FixTop(pageSetUpDialog.m_psd.rtMargin.top / 100);
		this->notePadForm->pageInfo->FixLeft(pageSetUpDialog.m_psd.rtMargin.left / 100);
		this->notePadForm->pageInfo->FixRight(pageSetUpDialog.m_psd.rtMargin.right / 100);
		this->notePadForm->pageInfo->FixBottom(pageSetUpDialog.m_psd.rtMargin.bottom / 100);

		//3.2. 머리글, 바닥글을 구한다. 
		//DoModal에서 작업해야되는게 아니라, DoModal하면 컨트롤이 사라지므로, 작동되는 중에(PageSetUpDialog) 얻어야한다.

		this->notePadForm->pageInfo->FixHeader(pageSetUpDialog.GetHeader());
		this->notePadForm->pageInfo->FixFooter(pageSetUpDialog.GetFooter());

		//3.3. 선택한 용지를 구한다.
		DEVMODE* portName = pageSetUpDialog.GetDevMode();
		portName->dmFields |= DM_ORIENTATION | DM_PAPERSIZE;

		CString beforeFormName = this->notePadForm->pageInfo->GetFormName();
		this->notePadForm->pageInfo->FixFormName(portName->dmFormName);
		this->notePadForm->pageInfo->FixPaperSize(portName->dmPaperSize);

		//3.4. 선택한 방향을 구한다.
		beforeOrientation = this->notePadForm->pageInfo->GetOrientation();
		this->notePadForm->pageInfo->FixOrientation(portName->dmOrientation);

		//3.5. 해당 용지의 너비, 길이를 구한다.
		paperSize = pageSetUpDialog.GetPaperSize();

		if (beforeFormName != portName->dmFormName) {
			this->notePadForm->pageInfo->FixPaperWidth(paperSize.cx / 100);
			this->notePadForm->pageInfo->FixPaperHeight(paperSize.cy / 100);
			paperSizeCx = paperSize.cx / 100;
			paperSizeCy = paperSize.cy / 100;
		}
		else {
			if (beforeOrientation == portName->dmOrientation) {
				paperSizeCx = this->notePadForm->pageInfo->GetPaperWidth();
				paperSizeCy = this->notePadForm->pageInfo->GetPaperHeight();
			}
			else {
				paperSizeCy = this->notePadForm->pageInfo->GetPaperWidth();
				paperSizeCx = this->notePadForm->pageInfo->GetPaperHeight();
				this->notePadForm->pageInfo->FixPaperWidth(paperSizeCx);
				this->notePadForm->pageInfo->FixPaperHeight(paperSizeCy);
			}
		}


		//3.6. 실제 너비를 구한다.
		actualWidth = paperSizeCx -
			(pageSetUpDialog.m_psd.rtMargin.left / 100 + pageSetUpDialog.m_psd.rtMargin.right / 100);
		actualHeight = paperSizeCy -
			(pageSetUpDialog.m_psd.rtMargin.top / 100 + pageSetUpDialog.m_psd.rtMargin.bottom / 100);


		this->notePadForm->pageInfo->FixActualWidth(actualWidth);
		this->notePadForm->pageInfo->FixActualHeight(actualHeight);
#if 0
		paperSize = pageSetUpDialog.GetPaperSize();
		this->notePadForm->pageInfo->FixPaperWidth(paperSize.cx / 100);
		this->notePadForm->pageInfo->FixPaperHeight(paperSize.cy / 100);

		//3.6. 실제 너비를 구한다.
		actualWidth = paperSize.cx / 100 -
			(pageSetUpDialog.m_psd.rtMargin.left / 100 + pageSetUpDialog.m_psd.rtMargin.right / 100);
		actualHeight = paperSize.cy / 100 -
			(pageSetUpDialog.m_psd.rtMargin.top / 100 + pageSetUpDialog.m_psd.rtMargin.bottom / 100);

		this->notePadForm->pageInfo->FixActualWidth(actualWidth);
		this->notePadForm->pageInfo->FixActualHeight(actualHeight);
	}
#endif 

	    //(21.11.28. 추가) 3.7. 현재 미리보기 창이 켜져있을 경우, 업데이트 시켜준다.
	    if (this->notePadForm->preview != NULL) {
			//---------------------------------------------------------------------------------------
			//원래 Preview의 OnCreate에서 설정해주는 건데, 여기서는 OnCreate를 거치지 않고 바로 업데이트므로 여기서 처리.


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
			//----------------------------------------------------------------------------------------


			this->notePadForm->preview->m_nCurrentPage = 1;

			//2.1. Edit Control에 현재 페이지를 출력한다.
			CString currentPage;
			currentPage.Format("%d", this->notePadForm->preview->m_nCurrentPage);
			this->notePadForm->preview->GetDlgItem(50000)->SetWindowText(currentPage);


			this->notePadForm->preview->Invalidate();
		}


	}
}



