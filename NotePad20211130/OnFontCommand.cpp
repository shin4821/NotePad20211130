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
	LOGFONT m_font; //��Ʈ�� ������ ������� ����ü����.

	CString fontStyle; //�۲ý�Ÿ��
	BOOL cancelLine; //��Ҽ�
	BOOL bottomLine; //����
	COLORREF colorName; //����


	//1. �۲� ��ư�� Ŭ������ ��
	  //1.1. �۲��̶�� ��Ʈ ���� ��ȭ���ڸ� �����.(���� ���õ� �۲ð� ����� ��ȭ������ �⺻���� �����Ѵ�.)
	memset(&m_font, 0, sizeof(LOGFONT));

	  //1.2. ��Ʈ ���� ��ȭ������ �ʱⰪ�� �����Ѵ�.
	CClientDC dc(this->notePadForm); //���� ȭ���� ���� �� �ִ�.

	 //1.2.1. ��Ʈ ����� ���Ѵ�. 
	int fontSize = this->notePadForm->font->fontSize / 10;
	m_font.lfHeight = -MulDiv(fontSize, dc.GetDeviceCaps(LOGPIXELSY), 72);

	 //1.2.2. ��Ʈ���� ���Ѵ�.
	string previousFont = this->notePadForm->font->fontName;
	_tcscpy_s(m_font.lfFaceName, LF_FACESIZE, previousFont.c_str());

	 //1.2.3. ��Ʈ�β��� ���Ѵ�.
	m_font.lfWeight = this->notePadForm->font->fontWeight;

	 //1.2.4. ��Ʈ���⸦ ���Ѵ�.
	m_font.lfItalic = this->notePadForm->font->fontItalic;

	 //1.2.5. ��Ʈ��Ҽ��� ���Ѵ�.
	m_font.lfStrikeOut = this->notePadForm->font->fontStrikeOut;

	 //1.2.6. ��Ʈ������ ���Ѵ�.
	m_font.lfUnderline = this->notePadForm->font->fontUnderLine;


	CFontDialog dlg(&m_font); //(&m_font)
	

	//1.2. ���� ��ȭ���ڸ� ����.
	if (IDOK == dlg.DoModal()) {
		//1.2.1. Ȯ���� ���� ���,

		//(21.10.25. �߰�)
		dlg.GetCurrentFont(&m_font); //��Ʈ�� �����ϰ� Ȯ�ι�ư�� ������ ����ü�� ��Ʈ������ �޴´�.

		//(21.10.25. �߰�)
		this->notePadForm->m_font = m_font;

		 //3. �����Ѵ�.
		this->notePadForm->font->fontSize = dlg.GetSize();  //m_font.lfHeight;
		this->notePadForm->font->fontName = (LPCTSTR)dlg.GetFaceName();
		this->notePadForm->font->fontWeight = dlg.GetWeight();
		this->notePadForm->font->fontItalic = dlg.IsItalic();
		this->notePadForm->font->fontStrikeOut = dlg.IsStrikeOut();
		this->notePadForm->font->fontUnderLine = dlg.IsUnderline();


		//4.(21.10.25.�߰�) �������Ϳ� ���� �����Ѵ�.
		//CWinApp* pApp = AfxGetApp();
		this->notePadForm->pApp->WriteProfileString("NotePadSection", "FontName", (LPCTSTR)dlg.GetFaceName());
		this->notePadForm->pApp->WriteProfileInt("NotePadSection", "FontSize", dlg.GetSize()); //m_font.lfHeight
		this->notePadForm->pApp->WriteProfileInt("NotePadSection", "FontWeight", dlg.GetWeight()); //�β�
		this->notePadForm->pApp->WriteProfileInt("NotePadSection", "FontItalic", dlg.IsItalic()); //����
		this->notePadForm->pApp->WriteProfileInt("NotePadSection", "FontStrikeOut", dlg.IsStrikeOut()); //��Ҽ�
		this->notePadForm->pApp->WriteProfileInt("NotePadSection", "FontUnderline", dlg.IsUnderline()); //����

		
#if 0
		if (this->getTextSize != NULL) {
			delete this->getTextSize;
		}
#endif
		this->notePadForm->getTextSize = new GetTextSize(this->notePadForm);


		//(21.10.26.�߰�)5. �ڵ����� ��ư�� ������ �ִ� ���,
		if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
			int currentX;
			int currentY;

			//5.1. �ڵ����� �� ĳ���� ��ġ�� ã�´�.
			this->notePadForm->lineChange->FindCaretBeforeLineChange(&currentX, &currentY);

			//5.2. �ڵ������ ������ Ǭ��.
			this->notePadForm->lineChange->LineChangeButtonNotClicked();

			//5.3. �ٲ� ���ǿ� ���� �ٽ� �ڵ����� �Ѵ�.
			this->notePadForm->lineChange->LineChangeButtonClicked();

			//5.4. �ڵ����� �� �̵��� ĳ���� ��ġ�� ���Ѵ�.
			this->notePadForm->lineChange->FindCaretAfterLineChange(currentX, currentY);

			this->notePadForm->Notify();
		}

	}

	this->notePadForm->Notify();
	this->notePadForm->Invalidate();

}

