//OnFontCommand.cpp
#include"OnFontCommand.h"
#include"NotePadForm.h"
#include"Font.h"
#include"GetTextSize.h"
#include"LineChange.h"
#include<afxdlgs.h>

OnFontCommand::OnFontCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
}

OnFontCommand::~OnFontCommand() {
}

void OnFontCommand::Execute() {

	CString font;
	int size;
	LOGFONT m_font; //폰트의 정보를 얻기위한 구조체선언.

	CString fontStyle; //글꼴스타일
	BOOL cancelLine; //취소선
	BOOL bottomLine; //밑줄
	COLORREF colorName; //색상


	//1. 글꼴 버튼을 클릭했을 때
	  //1.1. 글꼴이라는 폰트 공통 대화상자를 만든다.(현재 선택된 글꼴과 사이즈를 대화상자의 기본으로 설정한다.)
	memset(&m_font, 0, sizeof(LOGFONT));

	  //1.2. 폰트 공통 대화상자의 초기값을 설정한다.
	CClientDC dc(this->notePadForm); //현재 화면을 얻을 수 있다.

	 //1.2.1. 폰트 사이즈를 정한다. 
	int fontSize = this->notePadForm->font->fontSize / 10;
	m_font.lfHeight = -MulDiv(fontSize, dc.GetDeviceCaps(LOGPIXELSY), 72);

	 //1.2.2. 폰트명을 정한다.
	string previousFont = this->notePadForm->font->fontName;
	_tcscpy_s(m_font.lfFaceName, LF_FACESIZE, previousFont.c_str());

	 //1.2.3. 폰트두께를 정한다.
	m_font.lfWeight = this->notePadForm->font->fontWeight;

	 //1.2.4. 폰트기울기를 정한다.
	m_font.lfItalic = this->notePadForm->font->fontItalic;

	 //1.2.5. 폰트취소선을 정한다.
	m_font.lfStrikeOut = this->notePadForm->font->fontStrikeOut;

	 //1.2.6. 폰트밑줄을 정한다.
	m_font.lfUnderline = this->notePadForm->font->fontUnderLine;


	CFontDialog dlg(&m_font); //(&m_font)
	

	//1.2. 공통 대화상자를 띄운다.
	if (IDOK == dlg.DoModal()) {
		//1.2.1. 확인을 누를 경우,

		//(21.10.25. 추가)
		dlg.GetCurrentFont(&m_font); //폰트를 선택하고 확인버튼을 누르면 구조체로 폰트정보를 받는다.

		//(21.10.25. 추가)
		this->notePadForm->m_font = m_font;

		 //3. 적용한다.
		this->notePadForm->font->fontSize = dlg.GetSize();  //m_font.lfHeight;
		this->notePadForm->font->fontName = (LPCTSTR)dlg.GetFaceName();
		this->notePadForm->font->fontWeight = dlg.GetWeight();
		this->notePadForm->font->fontItalic = dlg.IsItalic();
		this->notePadForm->font->fontStrikeOut = dlg.IsStrikeOut();
		this->notePadForm->font->fontUnderLine = dlg.IsUnderline();


		//4.(21.10.25.추가) 레지스터에 값을 저장한다.
		//CWinApp* pApp = AfxGetApp();
		this->notePadForm->pApp->WriteProfileString("NotePadSection", "FontName", (LPCTSTR)dlg.GetFaceName());
		this->notePadForm->pApp->WriteProfileInt("NotePadSection", "FontSize", dlg.GetSize()); //m_font.lfHeight
		this->notePadForm->pApp->WriteProfileInt("NotePadSection", "FontWeight", dlg.GetWeight()); //두께
		this->notePadForm->pApp->WriteProfileInt("NotePadSection", "FontItalic", dlg.IsItalic()); //기울기
		this->notePadForm->pApp->WriteProfileInt("NotePadSection", "FontStrikeOut", dlg.IsStrikeOut()); //취소선
		this->notePadForm->pApp->WriteProfileInt("NotePadSection", "FontUnderline", dlg.IsUnderline()); //밑줄

		
#if 0
		if (this->getTextSize != NULL) {
			delete this->getTextSize;
		}
#endif
		this->notePadForm->getTextSize = new GetTextSize(this->notePadForm);


		//(21.10.26.추가)5. 자동개행 버튼이 눌려져 있는 경우,
		if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
			int currentX;
			int currentY;

			//5.1. 자동개행 전 캐럿의 위치를 찾는다.
			this->notePadForm->lineChange->FindCaretBeforeLineChange(&currentX, &currentY);

			//5.2. 자동개행된 문서를 푼다.
			this->notePadForm->lineChange->LineChangeButtonNotClicked();

			//5.3. 바뀐 조건에 따라 다시 자동개행 한다.
			this->notePadForm->lineChange->LineChangeButtonClicked();

			//5.4. 자동개행 후 이동할 캐럿의 위치를 구한다.
			this->notePadForm->lineChange->FindCaretAfterLineChange(currentX, currentY);

			this->notePadForm->Notify();
		}

	}

	this->notePadForm->Notify();
	this->notePadForm->Invalidate();

}

