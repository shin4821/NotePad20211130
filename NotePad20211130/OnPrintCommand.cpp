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

	//1. �����ȭ����(�μ�)�� �����.
	CPrintDialog dlg_(FALSE, PD_ALLPAGES | PD_USEDEVMODECOPIES | PD_NOPAGENUMS | PD_HIDEPRINTTOFILE | PD_NOSELECTION);


	//2. ����ڰ� ���������� ������ ��������,
	if (dlg_.DoModal() == IDOK) {

		//2.0. hdc�� pDC�� �����.
		CDC* pDC = new CDC;
		pDC->Attach(dlg_.m_pd.hDC);

		//2.1. CPrintDialog�� m_pd ������� hDevMode�� �����´�. ************************************
		// ��Ʈ ���� ���� DevMode�� ������ ����Ѵ�. �ȱ׷��� ��Ʈ �����Ѱ͵� Reset �ȴ�.

		//2.1.1. GlobalLock���� dlg_�� hDevMode�� �����´�.
		pDevMode = (DEVMODE*)GlobalLock(dlg_.m_pd.hDevMode);


		//2.1.2. hDevMode�� �ʵ带 �������ش�. (���μ��κ�ȯ, ���� ������)
		pDevMode->dmFields |= DM_ORIENTATION | DM_PAPERSIZE;
		
		pDevMode->dmPaperSize = paperSize;
		this->notePadForm->pDevMode_ = pDevMode;

		//2.1.4. ������ ���� ���⿡ ���� ������ ������ �ٲ��ش�.
		if (orientation == 2) {
			pDevMode->dmOrientation = DMORIENT_LANDSCAPE; //���� �μ� �����Ѵ�.
		}
		
		//����� �ɼ��� �������� dc�� ������Ʈ �Ѵ�. 
		pDC->ResetDC(pDevMode);
		GlobalUnlock(dlg_.m_pd.hDevMode);
		//*******************************************************************************************

		//dc�� ������Ʈ �� �Ŀ��� ������ �ɼǴ�� ũ�⸦ ���� �� �ִ�.
		this->notePadForm->printWidth = pDC->GetDeviceCaps(HORZRES);
		this->notePadForm->printHeight = pDC->GetDeviceCaps(VERTRES);
		Long printWidth = this->notePadForm->printWidth;
		Long printHeight = this->notePadForm->printHeight;

		this->notePadForm->printDC = pDC;
		this->notePadForm->rateWidth = pDC->GetDeviceCaps(HORZRES) / 25.4; //210
		this->notePadForm->rateHeight = pDC->GetDeviceCaps(VERTRES) / 25.4; //297

		int rate = pDC->GetDeviceCaps(HORZRES) / virtualPaperWidth;

		//2.2. �ش� DC���� ������ �⺻ ���̸� ���Ѵ�.
		//��Ʈ�߰�***************************************************************
		CFont cFont;
		int fontSize = this->notePadForm->font->GetFontSize(); 
		this->notePadForm->printFontSize = fontSize;

		//���� ��Ʈ ����ü ����´�.
		LOGFONT printLogFont = this->notePadForm->m_font;

		//�ػ� ������ ��Ʈ������ ���� (��ũ��)
		printLogFont.lfHeight = -MulDiv(fontSize / 10, 600, 72);
		
		HFONT hFont = CreateFontIndirect(&printLogFont);
		pDC->SelectObject(hFont);
		//***********************************************************************

		strSize = pDC->GetTabbedTextExtent("��", 0, NULL);
		strSizeY = strSize.cy;
		strSizeX = strSize.cx;

		//(21.10.27.�߰�) ���� �ڵ����� �Ǿ�������, �ڵ����� Ǯ���ش�.
		if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
			this->notePadForm->lineChange->LineChangeButtonNotClicked();
		}

		//(21.11.30. �߰�) �ػ� �����Ѵ�. ************************************************************************
       //�ػ� ������ GetTextSizeForDpi �迭�� �����.
		if (this->notePadForm->getTextSizeForDpi == NULL) {
			this->notePadForm->getTextSizeForDpi = new GetTextSizeForDpi(this->notePadForm);
		}

		//�ڵ����� �����ش�.
		printWidth = printWidth - ((left * 23.57) + (right * 23.57));
		this->notePadForm->lineChange->LineChangeButtonClickedForDpi(printWidth); //228�� ��� ���⿡ 4961��

		int length = this->notePadForm->note->GetLength();
		//************************************************************************************************************

		//�ػ� ������ GetTextSizeForDpi �迭�� �����.
		if (this->notePadForm->getTextSizeForDpi != NULL) {
			delete this->notePadForm->getTextSizeForDpi;
			this->notePadForm->getTextSizeForDpi = NULL;
		}

		//2.2. �����ͷ� ��½ÿ� ����ϴ� ���� ����
		DOCINFO docinfo = { sizeof(DOCINFO),"Print1: Test",NULL };

		//2.3. ���� ����, ���� �� ������ �����ߴ��� Ȯ���Ѵ�.
		//�����μ�=DMORIENT_PORTRAIT, �����μ�=DMORIENT_LANDSCAPE


		//2.6 ������ DC�� ������ ������ ��� ������ �˸���.
		pDC->StartDoc(&docinfo);


		//2.7. actualWidth, actualHeight�� �̿��Ͽ� �ڵ����� �� ���� ���Ѵ�.
		

		//(������ �ӽ÷� �صξ��� ����)
		//this->notePadForm->lineChange->LineChangeButtonClicked(actualWidth * 4.65);  

		//(��ʽ����� �ѹ� ó���غ���.. 997�� A4����)
		//�׽�Ʈ
		//this->notePadForm->lineChange->LineChangeButtonClicked(actualWidth * 4.74);
		//length = this->notePadForm->note->GetLength();
		
		int l;

		//2.7.1. �ٴڱ��� ���� ���, �ٴڱ��� ���� ���� ���ش�.
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



		//2.8. ������ DC�� ����� �����͸� ����Ѵ�.
		while (i < length) { //&& actualHeight > compareHeight

			j = 0;

			//2.8.1. ���ο� �������� �˸���.
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

				//1. ù��°�̰� �Ӹ����� ������,
				if (isFirst == true && header != "") {

					//1.1. ��� �����Ͽ� TextOut �Ѵ�.
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
			//(21.10.19 �߰�)
			if (isFirstCount == true) {
				this->notePadForm->rowNumber = i + l;
			}

			//2.8.2. �ٴڱ��� ������ ������ ���� �ٴڱ۷� ����Ѵ�.
			pDC->DrawText(footer, CRect(0, (virtualPaperHeight * 23.57) - (bottom * 23.57 + strSizeY),
				virtualPaperWidth * 23.57, virtualPaperHeight * 23.57), DT_SINGLELINE | DT_CENTER);
#if 0
			//2.8.2.1. compareHeight�� actualHeight �Ѿ����� �� �����ٿ� �ٴڱ� ����Ѵ�.
			if (i < length && footer != "") { //(strSizeY * j) + (top * 23.57) + strSizeY

				pDC->DrawText(footer, CRect(0, (strSizeY * j) + (top * 23.57), virtualPaperWidth * 23.57,
					virtualPaperHeight * 23.57), DT_SINGLELINE | DT_CENTER);
			}
			//2.8.2.2. compareHeight�� actualHeight�� ���� ���Ͽ����� �ٴڱ� ���п� ���� ����Ѵ�.
			else if (i >= length && footer != "") {

				pDC->DrawText(footer, CRect(0, (virtualPaperHeight * 23.57) - (bottom * 23.57 + strSizeY),
					virtualPaperWidth * 23.57, virtualPaperHeight * 23.57), DT_SINGLELINE | DT_CENTER);
			}
#endif

			//2.9. �� �������� ���� �˸���.
			pDC->EndPage();
		}

		//(21.10.27.�߰�) ���� �ڵ����� �Ǿ�������, �ڵ����� Ǯ���ְ� �ٽ� �ڵ����� �Ѵ�.
		if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
			this->notePadForm->lineChange->LineChangeButtonNotClicked();
			this->notePadForm->lineChange->LineChangeButtonClicked();
		}
		//2.9. �ڵ������ ������ �ٽ� Ǯ���ش�.
		else {
			this->notePadForm->lineChange->LineChangeButtonNotClicked();
		}

		//3.0. �����͸� ����� DC�� �������Ѵ�.
		//::RestoreDC(hdc, -1);
		pDC->RestoreDC(-1);

		//3.1. ���� ��� ����
		//::EndDoc(hdc);
		pDC->EndDoc();


		pDC->DeleteDC();
		pDC->Detach();
		delete pDC;


	}
}

