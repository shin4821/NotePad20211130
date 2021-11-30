//PageSetUpDialog.cpp
#include"PageSetUpDialog.h"
#include"NotePadForm.h"
#include"PageInfo.h"
#include<afxdlgs.h>
#include<afxwin.h>
#pragma warning(disable:4996)
#include <iostream>
using namespace std;

BEGIN_MESSAGE_MAP(PageSetUpDialog, CPageSetupDialog) //, CPageSetupDialog
	//ON_WM_CLOSE()
	ON_BN_CLICKED(IDOK, &PageSetUpDialog::OnOK)
END_MESSAGE_MAP()


//
PageSetUpDialog::PageSetUpDialog(NotePadForm* notePadForm) 
:CPageSetupDialog(PSD_MARGINS | PSD_INHUNDREDTHSOFMILLIMETERS | PSD_MINMARGINS, notePadForm) {

	this->notePadForm = notePadForm;
	// this->m_psd.Flags = this->m_psd.Flags | PSD_ENABLEPAGESETUPTEMPLATE;

	this->m_psd.Flags |= PSD_ENABLEPAGESETUPTEMPLATE;
	this->m_psd.hInstance = AfxGetResourceHandle();
	this->m_psd.lpPageSetupTemplateName = MAKEINTRESOURCE(PageSetUpDialog::IDD); //lpPageSetupTemplateName
}




INT_PTR PageSetUpDialog::DoModal() {
	
	//1. 기본 여백을 설정한다.
	this->m_psd.rtMargin.top = this->notePadForm->pageInfo->GetTop() * 100;
	this->m_psd.rtMargin.left = this->notePadForm->pageInfo->GetLeft() * 100;
	this->m_psd.rtMargin.right = this->notePadForm->pageInfo->GetRight() * 100;
	this->m_psd.rtMargin.bottom = this->notePadForm->pageInfo->GetBottom() * 100;
	
#if 0
	//1. GlobalLock으로 dlg_의 hDevMode를 가져온다.
	DEVMODE* pDevMode = (DEVMODE*)GlobalLock(this->m_psd.hDevMode);

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
	//ResetDC(pDevMode);
	GlobalUnlock(this->m_psd.hDevMode);
#endif

	return CPageSetupDialog::DoModal();
}


BOOL PageSetUpDialog::OnInitDialog() {
	
	CString paperSize = this->notePadForm->pageInfo->GetFormName();

	//1. 머리글, 바닥글을 초기화한다.
	this->header = NULL;
	this->footer = NULL;
	GetDlgItem(IDC_EDIT_HEADER)->SetWindowText(this->notePadForm->pageInfo->GetHeader());
	GetDlgItem(IDC_EDIT_FOOTER)->SetWindowText(this->notePadForm->pageInfo->GetFooter());

	//2. 용지 크기를 초기화한다.
	//GetDlgItem(1137)->SetWindowText(paperSize);
	((CComboBox*)GetDlgItem(1137))->SelectString(0, (LPCTSTR)paperSize);

	//3. 용지 방향을 초기화한다.
	if (this->notePadForm->pageInfo->GetOrientation() == 1) {//세로이면,
		((CButton*)GetDlgItem(1056))->SetCheck(BST_CHECKED);
		((CButton*)GetDlgItem(1057))->SetCheck(BST_UNCHECKED);
	}
	else { //가로이면,
		((CButton*)GetDlgItem(1056))->SetCheck(BST_UNCHECKED);
		((CButton*)GetDlgItem(1057))->SetCheck(BST_CHECKED);
	}


	CPageSetupDialog::OnInitDialog();
	return FALSE;

}

PageSetUpDialog::~PageSetUpDialog() {

	if (this->header != NULL) {
		delete[] this->header;
		this->header = NULL;
	}
	if (this->footer != NULL) {
		delete[] this->footer;
		this->footer = NULL;
	}

}


void PageSetUpDialog::OnOK() {
	CString header;
	CString footer;

	//1. header의 값을 읽는다.
	GetDlgItem(IDC_EDIT_HEADER)->GetWindowText(header);

	//2. 값의 length만큼 할당시켜준다.
	this->header = new char[header.GetLength() + 1];
	
	//3. 해당 값을 멤버로 복사한다.
	strcpy(this->header, (LPCTSTR)header);
	
	//4. footer의 값을 읽는다.
	GetDlgItem(IDC_EDIT_FOOTER)->GetWindowText(footer);

	//5. 값의 length만큼 할당시켜준다.
	this->footer = new char[footer.GetLength() + 1];

	//6. 해당 값을 멤버로 복사한다.
	strcpy(this->footer, (LPCTSTR)footer);

	CPageSetupDialog::OnOK();
	//CDialog::OnOK();
}

CString PageSetUpDialog::GetHeader() {

	CString header;
	header.Format("%s", this->header);

	return header;
}

CString PageSetUpDialog::GetFooter() {

	CString footer;
	footer.Format("%s", this->footer);

	return footer;
}

#if 0
void PageSetUpDialog::OnClose() {

	if (this->header != NULL) {
		delete[] this->header;
		this->header = NULL;
	}
	if (this->footer != NULL) {
		delete[] this->footer;
		this->footer = NULL;
	}
	EndDialog(0);
}

#endif

void PageSetUpDialog::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
