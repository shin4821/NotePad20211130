//NotePadApp.cpp
#include"NotePadApp.h"
#include"NotePadForm.h"

BOOL NotePadApp::InitInstance() {

	this->m_pMainWnd = new NotePadForm;

	//(21.10.18 �߰�) ������Ʈ�� �߰� ******************************************************************
	
	//1. �� �� ���� �߰��ϸ� ������ ������Ʈ�� �����⿡ SetRegistryKey�� ���ڿ��� ������ ������.
	//(������Ʈ�� Ű ����)
	SetRegistryKey("NotePadForm");
	LoadStdProfileSettings(8);

	//2.�� �Լ��� ù ��° ���ڿ� ������ ������ �о����, ������ ����ϴ�. 
	//�� ��° ���ڿ� ������ ������ ������ ���� �о����, ������ �� ��° ������ ������ ����ϴ�.
	//GetProfileInt�̸� ���� ���� �����ϰ�, String�̸� ���ڿ� ���� �����մϴ�.

	//2.1. �ڵ� �ٹٲ� ������ �д´�.

	//2.2. ��Ʈ ������ �����´�.

	//2.3. �޸��� �������� ��ġ�� �����´�.

	//2.4. �޸��� ũ�⸦ �����´�.
	//**************************************************************************************************


	((CFrameWnd*)this->m_pMainWnd)->Create(NULL, "�޸���");

//#if 0
	//(21.09.29 �߰�) CPageDialog â���� ok�� �ٷ� �ش� �Լ��� ���� �ʾ� �߰�.***********************
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CoInitialize(NULL);
	AfxEnableControlContainer();
	//****************************************************************************************************
//#endif

	this->m_pMainWnd->ShowWindow(m_nCmdShow); //SW_SHOW, m_nCmdShow
	this->m_pMainWnd->UpdateWindow();

	return TRUE;
}

NotePadApp notePadApp;
















