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
	
	//1. �⺻ ������ �����Ѵ�.
	this->m_psd.rtMargin.top = this->notePadForm->pageInfo->GetTop() * 100;
	this->m_psd.rtMargin.left = this->notePadForm->pageInfo->GetLeft() * 100;
	this->m_psd.rtMargin.right = this->notePadForm->pageInfo->GetRight() * 100;
	this->m_psd.rtMargin.bottom = this->notePadForm->pageInfo->GetBottom() * 100;
	
#if 0
	//1. GlobalLock���� dlg_�� hDevMode�� �����´�.
	DEVMODE* pDevMode = (DEVMODE*)GlobalLock(this->m_psd.hDevMode);

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
	//ResetDC(pDevMode);
	GlobalUnlock(this->m_psd.hDevMode);
#endif

	return CPageSetupDialog::DoModal();
}


BOOL PageSetUpDialog::OnInitDialog() {
	
	CString paperSize = this->notePadForm->pageInfo->GetFormName();

	//1. �Ӹ���, �ٴڱ��� �ʱ�ȭ�Ѵ�.
	this->header = NULL;
	this->footer = NULL;
	GetDlgItem(IDC_EDIT_HEADER)->SetWindowText(this->notePadForm->pageInfo->GetHeader());
	GetDlgItem(IDC_EDIT_FOOTER)->SetWindowText(this->notePadForm->pageInfo->GetFooter());

	//2. ���� ũ�⸦ �ʱ�ȭ�Ѵ�.
	//GetDlgItem(1137)->SetWindowText(paperSize);
	((CComboBox*)GetDlgItem(1137))->SelectString(0, (LPCTSTR)paperSize);

	//3. ���� ������ �ʱ�ȭ�Ѵ�.
	if (this->notePadForm->pageInfo->GetOrientation() == 1) {//�����̸�,
		((CButton*)GetDlgItem(1056))->SetCheck(BST_CHECKED);
		((CButton*)GetDlgItem(1057))->SetCheck(BST_UNCHECKED);
	}
	else { //�����̸�,
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

	//1. header�� ���� �д´�.
	GetDlgItem(IDC_EDIT_HEADER)->GetWindowText(header);

	//2. ���� length��ŭ �Ҵ�����ش�.
	this->header = new char[header.GetLength() + 1];
	
	//3. �ش� ���� ����� �����Ѵ�.
	strcpy(this->header, (LPCTSTR)header);
	
	//4. footer�� ���� �д´�.
	GetDlgItem(IDC_EDIT_FOOTER)->GetWindowText(footer);

	//5. ���� length��ŭ �Ҵ�����ش�.
	this->footer = new char[footer.GetLength() + 1];

	//6. �ش� ���� ����� �����Ѵ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}
