//NotePadApp.cpp
#include"NotePadApp.h"
#include"NotePadForm.h"

BOOL NotePadApp::InitInstance() {

	this->m_pMainWnd = new NotePadForm;

	//(21.10.18 추가) 레지스트리 추가 ******************************************************************
	
	//1. 이 두 줄을 추가하면 실행의 레지스트리 편집기에 SetRegistryKey의 문자열로 폴더가 생성됨.
	//(레지스트리 키 설정)
	SetRegistryKey("NotePadForm");
	LoadStdProfileSettings(8);

	//2.이 함수는 첫 번째 문자열 정보로 있으면 읽어오고, 없으면 만듭니다. 
	//두 번째 문자열 정보로 파일이 있으면 값을 읽어오고, 없으면 세 번째 값으로 파일을 만듭니다.
	//GetProfileInt이면 정수 값을 저장하고, String이면 문자열 값을 저장합니다.

	//2.1. 자동 줄바꿈 정보를 읽는다.

	//2.2. 폰트 정보를 가져온다.

	//2.3. 메모장 윈도우의 위치를 가져온다.

	//2.4. 메모장 크기를 가져온다.
	//**************************************************************************************************


	((CFrameWnd*)this->m_pMainWnd)->Create(NULL, "메모장");

//#if 0
	//(21.09.29 추가) CPageDialog 창에서 ok시 바로 해당 함수로 들어가지 않아 추가.***********************
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
















