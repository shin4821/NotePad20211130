//OnGetHelpCommand.cpp
#include"OnGetHelpCommand.h"
#include"NotePadForm.h"

OnGetHelpCommand::OnGetHelpCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
}

OnGetHelpCommand::~OnGetHelpCommand() {
}

void OnGetHelpCommand::Execute() {

#if 0
	//해당 링크를 띄운다.
	ShellExecute(this->notePadForm->m_hWnd, TEXT("open"), TEXT("MicrosoftEdge.exe"), TEXT("https://www.bing.com/search?q=windows+10%EC%9D%98+%EB%A9%94%EB%AA%A8%EC%9E%A5%EC%97%90+%EB%8C%80%ED%95%9C+%EB%8F%84%EC%9B%80%EB%A7%90+%EB%B3%B4%EA%B8%B0&filters=guid:%224466414-ko-dia%22%20lang:%22ko%22&form=T00032&ocid=HelpPane-BingIA"), 
		NULL, SW_SHOW);
#endif

	HWND hwnd = HtmlHelpA(GetDesktopWindow(), "helpDoc.chm", HH_DISPLAY_TOPIC, NULL); //도움말 띄우기

}

