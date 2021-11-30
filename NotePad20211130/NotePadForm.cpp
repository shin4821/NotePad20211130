//NotePadForm.cpp
#include"NotePadForm.h"
#include"Glyph.h"
#include"Composite.h"
#include"Character.h"
#include"Note.h"
#include"Row.h"
#include"SingleByteCharacter.h"
#include"DoubleByteCharacter.h"
#include"CreateGlyph.h"
#include"CreateProduct.h"
#include"GetTextSize.h"
#include"Location.h"
#include"Font.h"
#include"Caret.h"
#include"Observer.h"
#include"Previous.h"
#include"FactoryProduct.h"
#include"KeyAction.h"
#include"Thumb.h"
#include"Scroll.h"
#include"MoveAction.h"
#include"FactoryProductScroll.h"
#include"LineChange.h"
#include"DummyRow.h"
#include"SelectWords.h"
#include"CopyText.h"
#include"PasteText.h"
#include"FindSameText.h"
#include"FindDialog.h"
#include"ReplaceDialog.h"
//(21.09.01 �߰�) Command Pattern 
#include"FactoryProductCommand.h"
#include"Command.h"
#include"OnCharCommand.h"
#include"CommandStory.h"
#include"PageSetUpDialog.h"
#include"PageInfo.h"
#include"GetInformation.h"
#include"UndoList.h"
#include"WindowText.h"
#include"Visitor.h"
#include"GetTextSizeForDpi.h"

#include <HTMLHelp.h> // HTML Help API header
#include <htmlhelp.h>
#pragma comment(lib, "htmlhelp")

#include<windef.h>
#include<afx.h> //GetFileName(); CFile
#include<stdlib.h> //_splitpath
#include<wchar.h>//_splitpath
#include<afxext.h>
#include<atltypes.h>
#include<string.h>
#pragma warning(disable:4996)

HHOOK hSaveMessageBoxHook; // �޼����ڽ� ��ŷ�� �������� ����.
//HWND hwnd = HtmlHelpA(GetDesktopWindow(), "HelpDocument.chm", HH_DISPLAY_TOPIC, NULL); //���� ����


BEGIN_MESSAGE_MAP(NotePadForm, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_CHAR()

	//(21.09.01) �߰�
	ON_COMMAND_RANGE(40001, 40053, OnMyCommand) //40038
	ON_WM_ERASEBKGND() //ȭ�� ������(flickering) ���� �޼���
	ON_MESSAGE (WM_IME_COMPOSITION, OnImeComposition)
	ON_MESSAGE (WM_IME_STARTCOMPOSITION, OnImeStartComposition)
	ON_MESSAGE (WM_IME_CHAR, OnImeChar)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
#if 0
	ON_COMMAND(2006, &NotePadForm::OnSaveButtonClicked)
	ON_COMMAND(40007, &NotePadForm::OnSaveByDifferentNameButtonClicked)
	ON_COMMAND(40008, &NotePadForm::OnOpenButtonClicked)
	ON_COMMAND(40010, &NotePadForm::OnFontButtonClicked)
	ON_COMMAND(40014, &NotePadForm::OnLineChangeButtonClicked)
	ON_COMMAND(2002, &NotePadForm::OnUnExecuteButtonClicked)
	ON_COMMAND(40039, &NotePadForm::OnFindButtonClicked)
	ON_COMMAND(40042, &NotePadForm::OnReplaceButtonClicked)
#endif
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_CLOSE()


END_MESSAGE_MAP()

BOOL NotePadForm::OnEraseBkgnd(CDC* pDC) {
	return TRUE;
}


//(21.09.01) �߰�
void NotePadForm::OnMyCommand(UINT id) {
	FactoryProductCommand factoryProductCommand(this);
	Command* command = NULL;
	string character;

	// �ش� command�� �����Ѵ�.
	command = factoryProductCommand.Make(id);

	//�ش� command�� �����Ѵ�.
	command->Execute();

	//(21.10.29.�߰�) BackSpace�� character�� ������ �ִ� ��쿡�� history list�� �߰��Ѵ�.
	if (id == 40013) {
		character = command->GetCharacterForAll();
		if (character != "") {

			this->commandStory->Pile(command);
		}	
	}

	//�ش� command�� undolist�� �״´�.
	//������� �Ǵ� ���ູ���� �ƴϸ�,
	else if (id != 40002 && id!=40003 && id!=40006 && id!=40007 && id!=40008 && id!=40009 && id!=40010 && id!=40011 && id!=40012
		&& id!=40014 && id!=40015 && id!=40016 && id!=40017 && id!=40018 && id!=40019 && id!=40020 && id!=40021 && id!=40022 &&
		id!=40023 && id!=40024 && id!=40025 && id!=40026 && id!=40027 && id!=40028 && id!=40029 && id!=40030 && id!=40031 &&
		id!=40032 && id!=40033 && id!=40034 && id!=40035 && id!=40039 && id != 40040 && id!=40041 &&
		id!=40043 && id!=40046 && id!=40047 && id!=40048 && id!=40049 && id!=40051 && id!=40052 && id!=40053 &&
		isUndo==FALSE) {
		//������ command�� history list�� �߰��Ѵ�.
		this->commandStory->Pile(command);
	}


#if 0
	if (command != NULL) {
		delete command;
		command = NULL;
	}
#endif

	Invalidate();

}

NotePadForm::NotePadForm() {
	this->index = 0;
	this->isFirstComposing=TRUE;
	this->x=0;
	this->y=0;
	this->isChanged = FALSE;
	this->xTextOut = 0;
	this->yTextOut = 0;
	int largestWidth = 0;
	this->isLineChangeButtonClicked;
	this->isRedo = FALSE;
	this->isUndo = FALSE;
	this->isJumpOverForPrevious = FALSE;
	this->isJumpOverForNext = FALSE;
	this->isDoubleByte = FALSE;
	this->isThereSelected = FALSE;
	this->isUp = FALSE;
	this->isDown = FALSE;
	this->pageSetUpDialog = NULL;
	this->isMouseDrag = FALSE;
	this->isMouseLeave = FALSE;
	this->isKillFocus = FALSE;
	this->isInsert = FALSE;
	this->isLineChangeAppend = FALSE;
	this->isOpen = FALSE;
	this->isOpenChanged = FALSE;
	this->currentX = 0;
	this->currentY = 0;
	this->isShiftClicked = FALSE;
	this->isShifting = FALSE;
	this->isComposition = FALSE;
	this->isKeyBoard = FALSE;
	this->isEndDoc = FALSE;
}

int NotePadForm::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CFrameWnd::OnCreate(lpCreateStruct);

	this->scroll = new Scroll(this);

	//GetClientRect(this->m_rectCurHist);
	//this->scroll->windowWidth = this->m_rectCurHist.right - this->m_rectCurHist.left;


	this->scroll->siHorizon.nMin = 0;
	this->scroll->siHorizon.nPos = 0;
	this->scroll->siVertical.nPage = this->scroll->windowWidth;
	//this->scroll->siHorizon.nMax = this->scroll->windowWidth + 1;
	this->scroll->siHorizon.nMax = 0;
	this->SetScrollInfo(SB_HORZ, &this->scroll->siHorizon, TRUE);

	this->scroll->siVertical.nMin = 0;
	this->scroll->siVertical.nPos = 0;
	this->scroll->siVertical.nPage = this->scroll->windowHeight;
	//this->scroll->siVertical.nMax = this->scroll->windowWidth + 1;
	this->scroll->siVertical.nMax = 0;
	this->SetScrollInfo(SB_VERT, &this->scroll->siVertical, TRUE);



	//2. Subject ��� �߰����ش�.
	
	dynamic_cast<Subject*>(this)->Attach(this->scroll);

	this->location = new Location(this);
	dynamic_cast<Subject*>(this)->Attach(this->location); //Attach�� �Ű������� Observer* observe������

	this->windowText = new WindowText(this);
	dynamic_cast<Subject*>(this)->Attach(this->windowText);


	// ��Ʈ Ŭ������ �����. 
	this->font = new Font(this);
	this->selectWords = new SelectWords(this);
	

	//(21.10.25.�߰�) ������ ������ ���� ��Ʈ �̸�, ����� �������Ϳ��� �����´�.
//#if 0
	this->pApp = AfxGetApp();
	this->font->fontName = this->pApp->GetProfileString("NotePadSection", "FontName", "���� ���");
	this->font->fontSize = this->pApp->GetProfileInt("NotePadSection", "FontSize", 140);
	this->font->fontWeight = this->pApp->GetProfileInt("NotePadSection", "FontWeight", 400);
	this->font->fontItalic = this->pApp->GetProfileInt("NotePadSection", "FontItalic", 0);
	this->font->fontStrikeOut = this->pApp->GetProfileInt("NotePadSection", "FontStrikeOut", 0);
	this->font->fontUnderLine = this->pApp->GetProfileInt("NotePadSection", "FontUnderline", 0);
//#endif

	CFont font;
	LOGFONT logFont;

	font.CreatePointFont(this->font->fontSize, this->font->fontName.c_str());
	font.GetLogFont(&logFont);
	this->m_font.lfHeight = logFont.lfHeight;
	//this->m_font.lfHeight = this->font->fontSize;


	strcpy_s(this->m_font.lfFaceName, this->font->fontName.c_str());
	this->m_font.lfWeight = this->font->fontWeight;
	this->m_font.lfItalic = this->font->fontItalic;
	this->m_font.lfStrikeOut = this->font->fontStrikeOut;
	this->m_font.lfUnderline = this->font->fontUnderLine;




#if 0
    this->font->fontSize = 140;
	this->font->fontName = "���� ���";//segoe script
#endif

	this->getTextSize = new GetTextSize(this);

	//21.11.30. �߰�
	//this->getTextSizeForDpi = new GetTextSizeForDpi(this);

	//21.08.08. �߰�(�ڵ����� ��ü ����)
	this->lineChange = new LineChange(this);

	//21.08.20. �߰�(���� & �����ֱ�)
	this->copyText = new CopyText(this);
	this->pasteText = new PasteText(this);
	
	//21.08.25. �߰�(ã�� & �ٲٱ�)
	this->findSameText = new FindSameText(this);

	//21.09.02 �߰�(Stack�� history)
	this->commandStory = new CommandStory(this);

	//21.09.28 �߰� (����������)
	this->pageInfo = new PageInfo(this);

	//21.10.05 �߰�
	this->getInformation = new GetInformation(this);

	//21.07.28. �߰�(��ũ�� ��ü ����)
	this->thumb = new Thumb(this);

	//21.11.24.�߰�
	this->visitor = new Visitor(this);

	//21.10.17. �߰� (���콺 ������ ��ư �޴� ��Ȱ��ȭ ����)
	this->m_bAutoMenuEnable = FALSE;

	this->note = new Note();
	this->row = new Row();
	this->note->Add(this->row);


	//21.07.13.)1. ���� �޴� �߰��� ��
	//2.0 ����������� ĸ�� �ܴ�.
	//SetWindowText("�������"); //"������� - Windows �޸���"??


	//3.7.6. �ش� ������ Load�Ѵ�. �̵��ϴ� ���� Ű�ڸ��� �ߵ���***********************
	this->note->Load("�׽�Ʈ��.txt");
	//3.7.7. �ش� ������ ĸ���� �ٲ۴�.
	SetWindowText("�׽�Ʈ��");

    //current�� �� ó������(0,) �̵��Ѵ�.
	this->note->MoveCurrent(0);
	Glyph* row = note->GetChild(0);
	row->MoveCurrent(0);
	//*********************************************************************************


//#if 0
	//(21.10.25 �߰�) ������ ũ��, ��ġ�� ���Ͽ� ������Ʈ�� ó���Ѵ�. **********
	//������ ������ ���� �������� ũ�� �� ��ġ�� �����´�.
	Long windowLeft = this->pApp->GetProfileInt("NotePadSection", "WindowLeft", 513);
	Long windowTop = this->pApp->GetProfileInt("NotePadSection", "WindowTop", 272);
	Long windowWidth_ = this->pApp->GetProfileInt("NotePadSection", "WindowWidth", 873);
	Long windowHeight_ = this->pApp->GetProfileInt("NotePadSection", "WindowHeight", 394);

	//�ش� ��ġ�� ũ�⸸ŭ �������ش�.
	MoveWindow(windowLeft, windowTop, windowWidth_, windowHeight_);
	this->scroll->windowWidth = windowWidth_;
	this->scroll->windowHeight = windowHeight_;

	//***************************************************************************
//#endif


	
#if 0
	//1.8. (21.10.25.�߰�) ������Ʈ������ �ڵ����� ���� Ȯ���Ѵ�.************************************
	int isLineChanged_ = this->pApp->GetProfileInt("NotePadSection", "IsLineChanged", 0);

	//1.9. ������Ʈ�� ���� ���� �ڵ����� ���� �������ش�.
	CMenu* pMenu = AfxGetMainWnd()->GetMenu(); //AfxGetMainWnd()->GetMenu();

	if (isLineChanged_ == 1) {
		pMenu->CheckMenuItem(40014, MF_CHECKED);
		this->isLineChangeButtonClicked = TRUE;
	}

	else {
		pMenu->CheckMenuItem(40014, MF_UNCHECKED);
		this->isLineChangeButtonClicked = FALSE;
	}
	//*********************************************************************************************
#endif


	//3.7.8. ���� ��ȭ�� �ʱ�ȭ�Ѵ�.
	this->isChanged = FALSE;

	//1.1. ���� �޴��� �����Ѵ�.
	CMenu menuMain;
	menuMain.CreateMenu();

	//1.2. �˾��޴�(����)�� �����Ѵ�.
	CMenu menuFile;
	menuFile.CreatePopupMenu();

	// �˾��޴��� ���θ���⸦ �߰��Ѵ�.
	menuFile.AppendMenu(MF_STRING, 40048, "���� �����(&N)                      Ctrl+N");

	//1.3. �˾��޴��� ������ �߰��Ѵ�.
	menuFile.AppendMenu(MF_STRING, 40006,	"����(&S)                                 Ctrl+S");
	
	// �˾� �޴��� �ٸ� �̸����� ���� �߰��Ѵ�.
	menuFile.AppendMenu(MF_STRING, 40007 , "�ٸ� �̸����� ����(&A)            Ctrl+Shift+S");

	// �˾� �޴��� ���� �߰��Ѵ�.
	menuFile.AppendMenu(MF_STRING, 40008, "����(&O)                                Ctrl+O");

	menuFile.AppendMenu(MF_SEPARATOR);

	// �˾� �޴��� ������ ���� �߰��Ѵ�.
	menuFile.AppendMenu(MF_STRING, 40046, "������ ����(&U)");

	// �˾� �޴��� �μ� �߰��Ѵ�.
	menuFile.AppendMenu(MF_STRING, 40047, "�μ�(&P)                                 Ctrl+P");

	// �˾� �޴��� �̸����� �߰��Ѵ�.
	menuFile.AppendMenu(MF_STRING, 40053, "�̸�����");

	menuFile.AppendMenu(MF_SEPARATOR);

	// �˾� �޴��� ������ �߰��Ѵ�.
	menuFile.AppendMenu(MF_STRING, 40009, "������(&X)");

	//1.4. ���θ޴��� �˾�(����)�޴��� �߰��Ѵ�.
	menuMain.AppendMenu(MF_POPUP, (UINT_PTR)menuFile.Detach(), "����(&F)");


	//(21.08.24 �߰�) 1.1.) ã��: �˾��޴�(����)�� �����Ѵ�.
	CMenu meunEdit;
	meunEdit.CreatePopupMenu();

	//1.1. �˾��޴�(����)�� ������Ҹ� �߰��Ѵ�.
	meunEdit.AppendMenu(MF_STRING, 40002, "���� ���(&U)                  Ctrl+Z");

	meunEdit.AppendMenu(MF_STRING, 40003, "�ٽ� ����(&Y)                  Ctrl+Y");

	meunEdit.AppendMenu(MF_SEPARATOR);

	meunEdit.AppendMenu(MF_STRING, 40037, "�߶󳻱�(&T)                   Ctrl+X");

	meunEdit.AppendMenu(MF_STRING, 40035, "����(&C)                         Ctrl+C");

	meunEdit.AppendMenu(MF_STRING, 40036, "�ٿ��ֱ�(&P)                   Ctrl+V");

	//�˾��޴�(����)�� ������ �߰��Ѵ�.
	meunEdit.AppendMenu(MF_STRING, 40013, "����(&L)                             Del"); //���õȰ��� ������ Ŭ�����ϵ��� �ٲٱ�

	meunEdit.AppendMenu(MF_SEPARATOR);

	//1.2. �˾��޴�(����)�� ã�⸦ �߰��Ѵ�.
	meunEdit.AppendMenu(MF_STRING, 40039, "ã��(&F)                          Ctrl+F");

	//1.3. �˾��޴�(����)�� ����ã�⸦ �߰��Ѵ�.
	meunEdit.AppendMenu(MF_STRING, 40040, "���� ã��(&N)                       F3");

	//1.4. �˾��޴�(����)�� ����ã�⸦ �߰��Ѵ�.
	meunEdit.AppendMenu(MF_STRING, 40041, "���� ã��(&V)               Shift+F3");

	//1.5. �˾��޴�(����)�� �ٲٱ⸦ �߰��Ѵ�
	meunEdit.AppendMenu(MF_STRING, 40043, "�ٲٱ�(&R)                      Ctrl+H");

	meunEdit.AppendMenu(MF_SEPARATOR);

	//(21.09.30) �˾��޴�(����)�� ��� ������ �߰��Ѵ�.
	meunEdit.AppendMenu(MF_STRING, 40049, "��� ����(&A)                  Ctrl+A");

	//(21.09.30) �˾��޴�(����)�� ��� ������ �߰��Ѵ�.
	meunEdit.AppendMenu(MF_STRING, 40050, "�ð�/��¥(&D)                       F5");

	//1.6.  ���θ޴��� �˾�(����)�޴��� �߰��Ѵ�.
	menuMain.AppendMenu(MF_POPUP, (UINT_PTR)meunEdit.Detach(), "����(&E)");


	//(21.07.21.�߰�) 1.1.)�۲��� ���ϴ�: �˾��޴�(����)�� �����Ѵ�.
	CMenu menuForm;
	menuForm.CreatePopupMenu();

	//1.2. �˾��޴�(����)�� �ڵ������� �߰��Ѵ�. 
	menuForm.AppendMenu(MF_STRING, 40014, "�ڵ� �� �ٲ�(&W)");

	//1.3. �˾��޴�(����)�� �۲��� �߰��Ѵ�. 
	menuForm.AppendMenu(MF_STRING, 40010, "�۲�(&F)");

	//this->menuForm = menuMain;
	//CMenu* mmenu = GetMenu();
	//this->menuForm = mmenu->GetSubMenu(4);
	
	//1.4. ���θ޴��� �˾�(����)�޴��� �߰��Ѵ�.
	menuMain.AppendMenu(MF_POPUP, (UINT_PTR)menuForm.Detach(), "����(&O)");

	//(21.10.05 �߰�) �˾��޴�(����)�� �����Ѵ�.
	CMenu help;
	help.CreatePopupMenu();

	//1.1. �˾��޴�(����)�� ���� ���⸦ �߰��Ѵ�.
	help.AppendMenu(MF_STRING, 40051, "���� ����(&H)");

	help.AppendMenu(MF_SEPARATOR);

	//1.2. �˾��޴�(����)�� �޸��� ������ �߰��Ѵ�.
	help.AppendMenu(MF_STRING, 40052, "�޸��� ����(&A)");

	//1.3. ���θ޴��� �˾�(����)�޴��� �߰��Ѵ�.
	menuMain.AppendMenu(MF_POPUP, (UINT_PTR)help.Detach(), "����(&H)");

	//1.5. ���θ޴��� �����쿡 �߰��Ѵ�.
	SetMenu(&menuMain);

	//1.6. �޴���ü�� �޴��� �и��Ѵ�.
	//menuMain������ ������ ��� ���ŵǵ� �޴��� ���� �ְԵȴ�. Drtach() ȣ�� ���� �����ϸ� ���� ����.
	menuMain.Detach();

	//1.7. ��ũ�� ������ �ּ� �� �ִ밪�� �����Ѵ�.

#if 0
	SetScrollRange(SB_HORZ, 0, 1000, FALSE);
	SetScrollRange(SB_VERT, 0, 1000, FALSE);

	// Set the vertical scrolling range and page size
	this->siVertical.cbSize = sizeof(siVertical);
	this->siVertical.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	this->siVertical.nMin = 0;
	this->siVertical.nMax = 1000;
	this->siVertical.nPos = 0;
	this->siVertical.nPage = 0;
	SetScrollInfo(SB_VERT, &this->siVertical, FALSE);

	// Set the horizontal scrolling range and page size. 
	this->siHorizon.cbSize = sizeof(siHorizon);
	this->siHorizon.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	this->siHorizon.nMin = 0;
	this->siHorizon.nMax = 1000;
	this->siHorizon.nPos = 0;
	this->siHorizon.nPage = 0;
	SetScrollInfo(SB_HORZ, &this->siHorizon, FALSE);


	SetScrollPos(SB_HORZ, this->siVertical.nPos, TRUE);
	SetScrollPos(SB_VERT, this->siHorizon.nPos, TRUE);

	EnableScrollBar(SB_BOTH, ESB_DISABLE_BOTH);
#endif

#if 0
	this->note = new Note();

	Glyph* row1 = new Row();
	Glyph* character1 = new DoubleByteCharacter("��");
	row1->Add(character1);
	Glyph* character2 = new DoubleByteCharacter("��");
	row1->Add(character2);
	Glyph* character3 = new DoubleByteCharacter("��");
	row1->Add(character3);
	Glyph* character4 = new DoubleByteCharacter("��");
	row1->Add(character4);
	Glyph* character5 = new DoubleByteCharacter("��");
	row1->Add(character5);
	Glyph* character6 = new SingleByteCharacter('.');
	row1->Add(character6);
	this->note->Add(row1);

	Glyph* row2 = new Row();
	Glyph* character7 = new DoubleByteCharacter("��");
	row2->Add(character7);
	Glyph* character8 = new DoubleByteCharacter("��");
	row2->Add(character8);
	Glyph* character9 = new DoubleByteCharacter("��");
	row2->Add(character9);
	Glyph* character10 = new DoubleByteCharacter("��");
	row2->Add(character10);
	Glyph* character11 = new DoubleByteCharacter("��");
	row2->Add(character11);
	Glyph* character12 = new SingleByteCharacter('.');
	row2->Add(character12);
	this->note->Add(row2);

	Glyph* row3 = new Row();
	Glyph* character13 = new SingleByteCharacter('H');
	row3->Add(character13);
	Glyph* character14 = new SingleByteCharacter('e');
	row3->Add(character14);
	Glyph* character15 = new SingleByteCharacter('l');
	row3->Add(character15);
	Glyph* character16 = new SingleByteCharacter('l');
	row3->Add(character16);
	Glyph* character17 = new SingleByteCharacter('o');
	row3->Add(character17);
	Glyph* character18 = new SingleByteCharacter(' ');
	row3->Add(character18);
	Glyph* character19 = new SingleByteCharacter('W');
	row3->Add(character19);
	Glyph* character20 = new SingleByteCharacter('o');
	row3->Add(character20);
	Glyph* character21 = new SingleByteCharacter('r');
	row3->Add(character21);
	Glyph* character22 = new SingleByteCharacter('l');
	row3->Add(character22);
	Glyph* character23 = new SingleByteCharacter('d');
	row3->Add(character23);
	Glyph* character24 = new SingleByteCharacter('!');
	row3->Add(character24);
	this->note->Add(row3);
#endif

#if 0
	//(21.10.25 �߰�) ������ ũ��, ��ġ�� ���Ͽ� ������Ʈ�� ó���Ѵ�. **********
	//������ ������ ���� �������� ũ�� �� ��ġ�� �����´�.
	Long windowLeft = this->pApp->GetProfileInt("NotePadSection", "WindowLeft", 513);
	Long windowTop = this->pApp->GetProfileInt("NotePadSection", "WindowTop", 272);
	Long windowWidth_ = this->pApp->GetProfileInt("NotePadSection", "WindowWidth", 873);
	Long windowHeight_ = this->pApp->GetProfileInt("NotePadSection", "WindowHeight", 394);

	//�ش� ��ġ�� ũ�⸸ŭ �������ش�.
	MoveWindow(windowLeft, windowTop, windowWidth_, windowHeight_);
	this->scroll->windowWidth = windowWidth_;
	this->scroll->windowHeight = windowHeight_;
	
	//***************************************************************************
#endif
	//MoveWindow(513, 272, 873, 394);


//#if 0
	//1.8. (21.10.25.�߰�) ������Ʈ������ �ڵ����� ���� Ȯ���Ѵ�.************************************
	int isLineChanged_ = this->pApp->GetProfileInt("NotePadSection", "IsLineChanged", 0);

	//1.9. ������Ʈ�� ���� ���� �ڵ����� ���� �������ش�.
	CMenu* pMenu = AfxGetMainWnd()->GetMenu(); //AfxGetMainWnd()->GetMenu();

	if (isLineChanged_ == 1) {
		pMenu->CheckMenuItem(40014, MF_CHECKED);
		this->isLineChangeButtonClicked = TRUE;
		// �ڵ����� ��Ų��.
		//this->lineChange->LineChangeButtonClicked();
	}

	else {
		pMenu->CheckMenuItem(40014, MF_UNCHECKED);
		this->isLineChangeButtonClicked = FALSE;
		// �ڵ����� ����Ѵ�.
		//this->lineChange->LineChangeButtonNotClicked();
	}
	//*********************************************************************************************
//#endif


	Invalidate();

	return 0;
}

void NotePadForm::OnSize(UINT nType, int cx, int cy) {
	CFrameWnd::OnSize(nType, cx, cy);

	int i = 0;
	int j;
	Glyph* row = NULL;
	Glyph* dummyRow;
	Glyph* nextRow;
	Glyph* character = NULL;
	int location;
	int width;
	BOOL ret = TRUE;
	int currentX;
	int currentY;
	Glyph* previousRow;
	int previousLength;
	int beforeLineChangeCurrentX;
	int rowLength;
	int firstX = this->selectWords->firstX;
	int firstY = this->selectWords->firstY;
	int endX = this->selectWords->endX;
	int endY = this->selectWords->endY;
	int changedX;
	int changedY;
	int changedFirstX = 0;
	int changedFirstY = 0;
	int changedEndX = 0;
	int changedEndY = 0;

	BOOL isBigger = TRUE;
	int startX ;
	int startY;
	int lastX;
	int lastY;

	//(21.11.02.�߰�)
		//3.1. isBigger�� true���� Ȯ���Ѵ�. 
	if (firstY < endY) {
		isBigger = TRUE;
	}
	else if (firstY > endY) {
		isBigger = FALSE;
	}
	else if (firstY == endY) {
		if (firstX < endX) {
			isBigger = TRUE;
		}
		else if (firstX > endX) {
			isBigger = FALSE;
		}
	}

	//3.2. isBigger�� �ƴϸ� ��ǥ�� ���� �ٲ۴�.
	if (isBigger == FALSE) {
		startX = firstX;
		startY = firstY;
		lastX = endX;
		lastY = endY;

		firstX = lastX;
		firstY = lastY;
		endX = startX;
		endY = startY;
	}

	this->selectWords->firstX = firstX;
	this->selectWords->firstY = firstY;
	this->selectWords->endX = endX;
	this->selectWords->endY = endY;


	//������ ������ �ٲ𶧸��� �������ְ�, nPage ���� �����Ѵ�.
#if 0
	this->scroll->siHorizon.fMask = SIF_PAGE;
	this->scroll->siHorizon.nPage = cx;
	this->scroll->windowWidth = cx;
	this->SetScrollInfo(SB_HORZ, &this->scroll->siHorizon, TRUE);

	this->scroll->siVertical.fMask = SIF_PAGE;
	this->scroll->siVertical.nPage = cy;
	this->scroll->windowHeight = cy;
	this->SetScrollInfo(SB_VERT, &this->scroll->siVertical, TRUE);
#endif

	//0. (21.09.10 �߰�) ������ ������ �ٲ𶧸��� �������ش�.
	//this->scroll->beforeWindowWidth = this->scroll->windowWidth;
	this->GetClientRect(this->m_rectCurHist);
	this->scroll->windowWidth = this->m_rectCurHist.right - this->m_rectCurHist.left;
	this->scroll->windowHeight = this->m_rectCurHist.bottom - this->m_rectCurHist.top;

	//0.1. (21.09.14 �߰�) �ٲ� ����� �������ش�.
	this->scroll->siHorizon.nPage = this->scroll->windowWidth;
	this->SetScrollInfo(SB_HORZ, &this->scroll->siHorizon, TRUE);

	this->scroll->siHorizon.nPage = this->scroll->windowHeight;
	this->SetScrollInfo(SB_VERT, &this->scroll->siVertical, TRUE);


	row = this->note->GetChild(0);
	row->GetContent();


	//1. OnSize ȣ���� ��(�ڵ����� üũ �Ǿ��ִ� ���)
	if (this->isLineChangeButtonClicked == TRUE) {

		//(21.08.23 �߰� 1) �ڵ����� �� ĳ���� ��ġ�� ã�´�.

#if 0
		this->lineChange->FindCaretBeforeLineChange(&beforeLineChangeCurrentX, &currentY);
		this->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
		this->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);
#endif

		this->lineChange->FindCaretBeforeLineChange(&beforeLineChangeCurrentX, &currentY);
		this->lineChange->FindCaretBeforeLineChangeForFirstXY(&firstX, &firstY);
		this->lineChange->FindCaretBeforeLineChangeForEndXY(&endX, &endY);



		//1.1. ���� windowWidth���� ������ ���,
		//SmallerThanBefore ������ KillFocus�Ͼ�� ���� �ݺ� ����.
		if (this->isKillFocus == FALSE && this->scroll->beforeWindowWidth > this->scroll->windowWidth) { //cx
			
            this->lineChange->SmallerThanBefore();

			//�ڵ������ ������ �����ش�.
			//this->lineChange->LineChangeButtonNotClicked();

			//������ ������ ���� ������ �ʺ�ŭ �ڵ������Ѵ�.
			//this->lineChange->LineChangeButtonClicked(this->scroll->windowWidth);
		}

		//1.2. ���� windowWidth���� Ŀ�� ���,
		else if (this->scroll->beforeWindowWidth < this->scroll->windowWidth) { //cx
			this->lineChange->BiggerThanBefore();
		}

		//2.2. (21.08.23 �߰� 2)�ڵ����� �� �̵��� ĳ���� ��ġ�� ���Ѵ�.

		this->lineChange->FindCaretAfterLineChange(beforeLineChangeCurrentX, currentY);
		//this->lineChange->FindCaretAfterLineChange(beforeLineChangeCurrentX, currentY, &changedX, &changedY);
		this->lineChange->FindCaretAfterLineChange(firstX, firstY, &changedFirstX, &changedFirstY);
		this->lineChange->FindCaretAfterLineChange(endX, endY, &changedEndX, &changedEndY);

	}

	currentY = this->note->GetCurrent();
	row = this->note->GetChild(currentY);
	currentX = row->GetCurrent();


	this->selectWords->firstX = changedFirstX;
	this->selectWords->firstY = changedFirstY;
	this->selectWords->endX = changedEndX;
	this->selectWords->endY = changedEndY;

	this->isJumpOverForPrevious = FALSE;
	this->isJumpOverForNext = FALSE;
	this->isUp = FALSE;
	this->isDown = FALSE;
	this->isDoubleByte = FALSE;

	this->Notify();

	//Invalidate();
}

#if 0
void NotePadForm::OnLineChangeButtonClicked() {

	this->SendMessage(WM_COMMAND, 40014);

#if 0
	int i = 0;
	int j;
	Glyph* row;
	Glyph* dummyRow;
	Glyph* nextRow;
	int location;
	BOOL ret;


	//1. ���� FALSE�ε� �ڵ� ���� ��ư ������ ��� TRUE�� �ٲ��ش�.
	if (this->isLineChangeButtonClicked == FALSE) {
		this->isLineChangeButtonClicked = TRUE;	
	}
	else if(this->isLineChangeButtonClicked == TRUE){
		this->isLineChangeButtonClicked = FALSE;
	}


	//2. �ڵ����� ��ư ������ ��(ON)
	if (this->isLineChangeButtonClicked == TRUE) {
		this->lineChange->LineChangeButtonClicked();	
	}

	//3. �ڵ����� ��ư ������� ��(OFF)
	else if (this->isLineChangeButtonClicked == FALSE) {
		this->lineChange->LineChangeButtonNotClicked();
	}

	Invalidate();
#endif
}

void NotePadForm::OnSaveButtonClicked() {

	this->SendMessage(WM_COMMAND, 2006);

#if 0
	FILE* file = 0;
	CString strPathName;
	CString fileName;
	string fileName_;
	CString str;
	int ret;
	CString title;
	CString fileNameWithoutTxt;

	 //2. ���� ��ư�� Ŭ������ ��
	 //1.1.���� �޸����� �̸��� Ȯ���Ѵ�. 
	 GetWindowText(title);
	 CString title_;
	 title_.Format("%s%s", title, ".txt");

	 CFileFind pFind;
	 BOOL ret_ = pFind.FindFile(title_);


	 //1.2. �ش� �̸��� ������ �����, ������ ���Ÿ� �Ѵ�.(�����ȭ����x)
	 if (ret_ == TRUE) {
		 this->note->Save((LPCTSTR)title_);
		 this->isChanged = FALSE;
	 }

	//2. ���� ������ ������ ������,
	else {


		//2.1. ���͸� �����.
		LPCTSTR fileFilter(_T("�������|*.*|�ؽ�Ʈ����(*.txt)|*.txt||"));

		//2.2. �����ȭ����(�ٸ��̸����� ����)�� �����.
		CFileDialog dlg(FALSE, _T("txt"), _T("*.txt"), OFN_HIDEREADONLY, fileFilter, NULL, 0, NULL);
		//OFN_HIDEREADONLY �� ���� ���� ������ ���

		//2.3. �����ȭ����(�ٸ��̸����� ����)�� ����.
		//2.4. ����ڰ� ������ ������ ���������� �����Ͽ�����, 
		if (IDOK == dlg.DoModal()) {


			//2.4.1. strPathName�� ������ ������ ��� �����Ѵ�.
			strPathName = dlg.GetPathName();

			//2.4.2. ������ ������ �̸��� ���� �´�.
			fileName = dlg.GetFileName();//���õ� ������ �̸��� Ȯ����
			fileNameWithoutTxt = dlg.GetFileTitle();

			fileName_ = (LPCTSTR)fileName;
			//MessageBox(title);

			//2.4.3. ���� �̸��� �ߺ��Ǹ� 
			if (PathFileExists(fileName)) {
				str.Format("%s%s%s", fileName, "��(��) �̹� �ֽ��ϴ�.\n", "�ٲٽðڽ��ϱ�?");
				ret = MessageBox(str, _T("�ٸ� �̸����� ���� Ȯ��"), MB_YESNO);

				if (ret == IDYES) {// Yes ������ ���, �ش� ���Ͽ� ������Ѵ�.
					//������ �ִ� �ش� �̸��� ������ �����Ѵ�.
					//remove(fileName);

					this->note->Save(fileName_);
					//�Է��� �̸����� �����Ѵ�.
					SetWindowText(fileNameWithoutTxt);
					//rename("NotePad.txt", fileName);
				}
			}
			//2.4.4. ���� �̸��� �ߺ����� ������,
			else {
				this->note->Save(fileName_);
				//�Է��� �̸����� �����Ѵ�.
				SetWindowText(fileNameWithoutTxt);
				//rename("NotePad.txt", fileName); //fileName
			}

			this->isChanged = FALSE;


		}
		//2.5. ����ڰ� ���� ���� ���ߴٸ� strPathName �ʱ�ȭ�ϰ� ����X.
		else {
			strPathName = "";
		}
	}

	Invalidate();
#endif
}

void NotePadForm::OnSaveByDifferentNameButtonClicked() {

	this->SendMessage(WM_COMMAND, 40007);
}

void NotePadForm::OnOpenButtonClicked() {

	this->SendMessage(WM_COMMAND, 40008);

#if 0
	CString str;
	BOOL ret;
	BOOL ret_;
	CString title;
	CString title_;
	CString strPathName;
	CString fileName;
	CString fileNameWithouttxt;

	//3. �����ư Ŭ������ ��
	 //3.1. ���� �޸����� Ÿ��Ʋ�� ���Ѵ�.
     GetWindowText(title);
	 CString titleWithTxt;
	 titleWithTxt.Format("%s%s", title, ".txt");


	 //3.2. �ش� ������ �����ϰ�, ���� ��ȭ�� ������,
	 CFileFind pFind;
	 Glyph* row = this->note->GetChild(0);


	 ret = pFind.FindFile(titleWithTxt);
	 if (ret == TRUE && this->isChanged == TRUE) {
		 //3.2.1. �޼����� ����.
		 str.Format("%s%s%s", "���泻���� ", title, "�� �����Ͻðڽ��ϱ�?");
		 ret_ = SaveMessageBox(this->GetSafeHwnd(), str, "�޸���", MB_YESNOCANCEL);
		 //ret_ = MessageBox(str, "�޸���", MB_YESNOCANCEL);

		 //3.2.2. YES�� ���� ���, �����Ѵ�.
		 if (ret_ == IDYES) {
			 this->note->Save((LPCTSTR)titleWithTxt);
		 }
	 }

	 //3.3. �ش� ������ �������� �ʰ� ���ڰ� �ϳ��� ������,
	 else if(ret == FALSE && (row->GetLength() > 0 || this->note->GetLength() >1)) {
		 //3.3.1. �޼����� ����.
		 str.Format("%s%s%s", "���泻���� ", title, "�� �����Ͻðڽ��ϱ�?");
		 ret_ = SaveMessageBox(this->GetSafeHwnd(), str, "�޸���", MB_YESNOCANCEL);

		 //3.3.2. YES�� ���� ���,
		 if (ret_ == IDYES) {
			 //2.1. ���͸� �����.
			 LPCTSTR fileFilter(_T("�������|*.*|�ؽ�Ʈ����(*.txt)|*.txt||"));

			 //2.2. �����ȭ����(�ٸ��̸����� ����)�� �����.
			 CFileDialog dlg(FALSE, _T("txt"), _T("*.txt"), OFN_HIDEREADONLY, fileFilter, NULL, 0, NULL);
			 //OFN_HIDEREADONLY �� ���� ���� ������ ���

			 //2.3. �����ȭ����(�ٸ��̸����� ����)�� ����.
			 //2.4. ����ڰ� ������ ������ ���������� �����Ͽ�����, 
			 if (IDOK == dlg.DoModal()) {
				 //2.4.1. strPathName�� ������ ������ ��� �����Ѵ�.
				 strPathName = dlg.GetPathName();

				 //2.4.2. ������ ������ �̸��� ���� �´�.
				 fileName = dlg.GetFileName();//���õ� ������ �̸��� Ȯ����
				 fileNameWithouttxt = dlg.GetFileTitle();

				 //MessageBox(title);

				 //2.4.3. ���� �̸��� �ߺ��Ǹ� 
				 if (PathFileExists(fileName)) {
					 str.Format("%s%s%s", fileName, "��(��) �̹� �ֽ��ϴ�.\n", "�ٲٽðڽ��ϱ�?");
					 ret = MessageBox(str, _T("�ٸ� �̸����� ���� Ȯ��"), MB_YESNO);

					 if (ret == IDYES) {// Yes ������ ���, �ش� ���Ͽ� ������Ѵ�.
						 //������ �ִ� �ش� �̸��� ������ �����Ѵ�.
						 //remove(fileName);

						 this->note->Save((LPCTSTR)fileName);

						 //�Է��� �̸����� �����Ѵ�
						 SetWindowText(fileNameWithouttxt);
					 }
				 }
				 //2.4.4. ���� �̸��� �ߺ����� ������,
				 else {
					 this->note->Save((LPCTSTR)fileName);
					 //�Է��� �̸����� �����Ѵ�.
					 SetWindowText(fileNameWithouttxt);
				 }
			 }
			 //2.5. ����ڰ� ���� ���� ���ߴٸ� strPathName �ʱ�ȭ�ϰ� ����X.
			 else {
				 strPathName = "";
			 }
		 }
	 }


	 //3.4. ���͸� �����.
	 LPCTSTR fileFilter_(_T("�������|*.*|�ؽ�Ʈ����(*.txt)|*.txt||"));

	 //3.5. �����ȭ����(����) �����.
	 CFileDialog dlg_(TRUE, _T("txt"), _T("*.txt"), OFN_HIDEREADONLY, fileFilter_, NULL, 0, NULL);

	 //3.6. �����ȭ���� ����.
	 //3.7. ����ڰ� �� ������ ���������� ���� �Ͽ��� ��
	 if (IDOK == dlg_.DoModal()) {
		 //3.7.1. ������ ������ �̸��� ���� �´�.
		 fileName = dlg_.GetFileName();
		 fileNameWithouttxt = dlg_.GetFileTitle();

		 //3.7.2. ���� note�� �����Ѵ�.
		 if (this->note != NULL) {
			 delete this->note;
		 }
		 //3.7.3. ���ο� ��Ʈ�� �����.
		 this->note = new Note();
		 //3.7.4. ���ο� ���� �����.
		 this->row = new Row();
		 //3.7.5. ��Ʈ�� ���� Add�Ѵ�.
		 this->note->Add(this->row);
		 //3.7.6. �ش� ������ Load�Ѵ�.
		 this->note->Load((LPCTSTR)fileName);
		 //3.7.7. �ش� ������ ĸ���� �ٲ۴�.
		 SetWindowText(fileNameWithouttxt);


		 //3.7.8. ���� ��ȭ�� �ʱ�ȭ�Ѵ�.
		 this->isChanged = FALSE;
	 }
	Invalidate();
#endif
}

void NotePadForm::OnFontButtonClicked() {

	this->SendMessage(WM_COMMAND, 40010);

#if 0
	CString font;
	int size;
	LOGFONT m_font; //��Ʈ�� ������ ������� ����ü����.

	//1. �۲� ��ư�� Ŭ������ ��
	  //1.1. �۲��̶�� ��Ʈ ���� ��ȭ���ڸ� �����.(���� ���õ� �۲ð� ����� ��ȭ������ �⺻���� �����Ѵ�.)
	memset(&m_font, 0, sizeof(LOGFONT));
	CClientDC dc(this);
	int fontSize = this->font->fontSize / 10;
	m_font.lfHeight = -MulDiv(fontSize, dc.GetDeviceCaps(LOGPIXELSY), 72);
	string previousFont = this->font->fontName;
	_tcscpy_s(m_font.lfFaceName, LF_FACESIZE, previousFont.c_str());


	CFontDialog dlg(&m_font); //(&m_font)

	//1.2. ���� ��ȭ���ڸ� ����.
	if (IDOK == dlg.DoModal()) {
		//1.2.1. Ȯ���� ���� ���,

		//1. ������ �۲��� Ȯ���Ѵ�.
		font = dlg.GetFaceName();
		//2. ������ �۲��� ũ�⸦ Ȯ���Ѵ�.
		size = dlg.GetSize();
		// dlg.GetCurrentFont(&m_font); //��Ʈ�� �����ϰ� Ȯ�ι�ư�� ������ ����ü�� ��Ʈ������ �޴´�.
		// m_font.lfFaceName();

		 //3. �����Ѵ�.
		this->font->fontSize = size;
		this->font->fontName = (LPCTSTR)font;

#if 0
		if (this->getTextSize != NULL) {
			delete this->getTextSize;
		}
#endif
		this->getTextSize = new GetTextSize(this);

	}
	Invalidate();
#endif
}

void NotePadForm::OnUnExecuteButtonClicked() {
	//1. ������ҹ�ư Ŭ������ ��
	//1.1. OnMyCommand ȣ���Ѵ�.

	//OnMyCommand(2002);
	this->SendMessage(WM_COMMAND, 2002);
}
#endif

void NotePadForm::OnPaint() {

	//2. �����찡 ���ŵ� ��
	Long length = this->note->GetLength();
	Long i = 0;
	Long j;
	Long rowLength;
	Long distanceX;
	Long totalRowDistance;
	Glyph* row;
	Glyph* character;
	string content_;
	CString content;
	BOOL isBigger = TRUE;
	int limitX;

	//0. CPaintDC�� ��� CFont�� ������ش�.
	CPaintDC dc(this);
	CFont cFont;

	//�׳� ����ϰ� �Ǹ� ���ŷ��� �ȵǰ�, ��Ʈ��ȭ �ؼ� ������۸� ó�� ����� �� ��鸮�Ƿ� ���� �߰�.
	//(21.10.15 ��Ʈ�� �߰�) 1. ���� �������� ũ�⸦ ���Ѵ�.
	CRect rect;
	this->GetClientRect(&rect);

	//(21.10.15 ��Ʈ�� �߰�) 2. �ӽ� dc�� ������ش�.
	this->dcTemp.CreateCompatibleDC(&dc);

	//(21.10.15 ��Ʈ�� �߰�) 3. ���� ������ ũ�⿡ �°� ��Ʈ���� �����.
	HBITMAP hbmp = ::CreateCompatibleBitmap(dc, rect.right, rect.bottom);
	HBITMAP oldBMP = (HBITMAP)this->dcTemp.SelectObject(hbmp);

	//(21.10.15 ��Ʈ�� �߰�) 4. ��Ʈ���� ������ ������� �ʱ�ȭ�Ѵ�.
	this->dcTemp.FillRect(&rect, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));


	//��Ʈ�߰�*********************************************************
	LOGFONT printLogFont = this->m_font; //OnFontCommand���� ����� LOGFONT ����ü��.
	HFONT hFont = CreateFontIndirect(&printLogFont);
	HFONT oldFont = (HFONT)this->dcTemp.SelectObject(hFont);

	CSize strSize = this->dcTemp.GetTabbedTextExtent("��", 0, NULL);
	int strSizeY = strSize.cy;
	int strSizeX = strSize.cx;
	this->strSizeY = strSizeY;
	//*****************************************************************
	

	//(21.10.15. �߰�) ���� this->scroll->siVertical.nPos�� �´� i(÷��)�� ã�´�.
	int k = 0;
	while (strSizeY * k < this->scroll->siVertical.nPos) {
		k++;
	}
	i = k;

	//(21.10.15. �߰�) ���� this->scroll->siVertical.nPos + windowHeight �� �´� length�� ã�´�.
	Long caretHeight = length * strSizeY;

	//���� ��ü ���� ���̰� nPos + windowHeight ���� ū ���, (�ƴ� ��� length�� ���� length�� ���ϴ�.)
	if (caretHeight > this->scroll->siVertical.nPos + this->scroll->windowHeight) {
		int k = 1;
		int l = 0;
		while (l < length && strSizeY * k <= this->scroll->siVertical.nPos + this->scroll->windowHeight) {
			k++;
			l++;
		}
		length = l;
	}

	//(21.11.23.�߰�) 
	this->firstTextY = i;
	this->endTextY = length - 1;

	//(21.11.24. Visitor ���� �߰�) ++++++++++++++++++++++++++++++++++
	this->note->Accept(this->visitor);
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	//(21.08.18) �����ϴ� �߰�*****************************************
	Long firstY = this->selectWords->GetFirstY();
	Long firstX = this->selectWords->GetFirstX();
	Long endY = this->selectWords->GetEndY();
	Long endX = this->selectWords->GetEndX();
	//******************************************************************

	//��ǥ�� isBigger���� Ȯ���Ѵ�.
	if (firstY < endY) {
		isBigger = TRUE;
	}
	else if (firstY > endY) {
		isBigger = FALSE;
	}
	else if (firstY == endY) {
		if (firstX < endX) {
			isBigger = TRUE;
		}
		else if (firstX > endX) {
			isBigger = FALSE;
		}
	}

	// isBigger�� �ƴϸ� ��ǥ�� ���� �ٲ۴�.

	if (isBigger == FALSE) {
		int startX = firstX;
		int startY = firstY;
		int lastX = endX;
		int lastY = endY;

		firstX = lastX;
		firstY = lastY;
		endX = startX;
		endY = startY;
	}

	//���õ� �ܾ ������,
	if (endX > 0 || endY > 0) {

		i = firstY;
		//(21.11.25.�߰�) �����ϴ� visitor ���� ����.
		this->visitor->SetFirstY(firstY);
		this->visitor->SetFirstX(firstX);
		this->visitor->SetEndY(endY);
		this->visitor->SetEndX(endX);

		// ������ ���ڻ��� �ٲ��ش�.
		this->dcTemp.SetTextColor(RGB(255, 255, 255));
		this->dcTemp.SetBkColor(RGB(000, 102, 204));

		//(21.11.24. Visitor ���� �߰�) ++++++++++++++++++++++++++++++++++
		this->visitor->VisitNoteForSelect(this->note);
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	}

	//(21.10.15 ��Ʈ�� �߰�) 5. ��Ʈ���� ����Ѵ�.
	dc.BitBlt(0, 0, rect.right, rect.bottom, &this->dcTemp, 0, 0, SRCCOPY);

	//(21.10.15 ��Ʈ�� �߰�) 6. ���� ��Ʈ���� �����Ѵ�.
	this->dcTemp.SelectObject(oldBMP);
	::DeleteObject(hbmp);
	this->dcTemp.DeleteDC();


	//���ο� ��Ʈ ����
	cFont.DeleteObject();

#if 0
	//2. �����찡 ���ŵ� ��
	Long length = this->note->GetLength();
	Long i = 0;
	Long j;
	Long rowLength;
	Long distanceX;
	Long totalRowDistance;
	Glyph* row;
	Glyph* character;
	string content_;
	CString content;
	BOOL isBigger = TRUE;
	int limitX;

	//0. CPaintDC�� ��� CFont�� ������ش�.
	CPaintDC dc(this);
	CFont cFont;

	//�׳� ����ϰ� �Ǹ� ���ŷ��� �ȵǰ�, ��Ʈ��ȭ �ؼ� ������۸� ó�� ����� �� ��鸮�Ƿ� ���� �߰�.
	//(21.10.15 ��Ʈ�� �߰�) 1. ���� �������� ũ�⸦ ���Ѵ�.
	CRect rect;
	this->GetClientRect(&rect);

	//(21.10.15 ��Ʈ�� �߰�) 2. �ӽ� dc�� ������ش�.
	CDC dcTemp;
	dcTemp.CreateCompatibleDC(&dc);

	//(21.10.15 ��Ʈ�� �߰�) 3. ���� ������ ũ�⿡ �°� ��Ʈ���� �����.
	HBITMAP hbmp = ::CreateCompatibleBitmap(dc, rect.right, rect.bottom);
	HBITMAP oldBMP = (HBITMAP)dcTemp.SelectObject(hbmp);

	//(21.10.15 ��Ʈ�� �߰�) 4. ��Ʈ���� ������ ������� �ʱ�ȭ�Ѵ�.
	dcTemp.FillRect(&rect, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));


	//��Ʈ�߰�*********************************************************
	LOGFONT printLogFont = this->m_font; //OnFontCommand���� ����� LOGFONT ����ü��.
	HFONT hFont = CreateFontIndirect(&printLogFont);
	HFONT oldFont = (HFONT)dcTemp.SelectObject(hFont);
	
	CSize strSize = dcTemp.GetTabbedTextExtent("��", 0, NULL);
	int strSizeY = strSize.cy;
	this->strSizeY = strSizeY;
	//*****************************************************************

	//(21.10.15. �߰�) ���� this->scroll->siVertical.nPos�� �´� i(÷��)�� ã�´�.
	int k = 0;
	while (strSizeY * k < this->scroll->siVertical.nPos) {
		k++;	
	}
	i = k;

	//(21.10.15. �߰�) ���� this->scroll->siVertical.nPos + windowHeight �� �´� length�� ã�´�.
	Long caretHeight = length * strSizeY;

	//���� ��ü ���� ���̰� nPos + windowHeight ���� ū ���, (�ƴ� ��� length�� ���� length�� ���ϴ�.)
	if (caretHeight > this->scroll->siVertical.nPos + this->scroll->windowHeight) {
		int k = 1;
		int l = 0;
		while (l < length && strSizeY * k <= this->scroll->siVertical.nPos + this->scroll->windowHeight) {
			k++;
			l++;
		}
		length = l;
	}

	//(21.11.23.�߰�) 
	this->firstTextY = i;

	// ��ü ���� ����Ѵ�.
	while (i < length) {
		row = this->note->GetChild(i);
		content_ = row->GetContent();

		content = CString(content_.c_str());
		
		dcTemp.TextOut(-this->scroll->siHorizon.nPos, (strSizeY * i) - this->scroll->siVertical.nPos, content);

		i++;
	}

	//(21.11.22.�߰�) 
	this->endTextY = i-1;


	//(21.08.18) �����ϴ� �߰�*****************************************
	Long firstY = this->selectWords->GetFirstY();
	Long firstX = this->selectWords->GetFirstX();
	Long endY = this->selectWords->GetEndY();
	Long endX = this->selectWords->GetEndX();
	//******************************************************************

	//��ǥ�� isBigger���� Ȯ���Ѵ�.
	if (firstY < endY) {
		isBigger = TRUE;	
	}
	else if (firstY > endY) {
		isBigger = FALSE;
	}
	else if (firstY == endY) {
		if (firstX < endX) {
			isBigger = TRUE;
		}
		else if (firstX > endX) {
			isBigger = FALSE;	
		}
	}

	// isBigger�� �ƴϸ� ��ǥ�� ���� �ٲ۴�.

	if (isBigger == FALSE) {
		int startX = firstX;
		int startY = firstY;
		int lastX = endX;
		int lastY = endY;

		firstX = lastX;
		firstY = lastY;
		endX = startX;
		endY = startY;	
	}

	i = firstY;

	while (i <= endY) {
		row = this->note->GetChild(i);
		//rowLength = row->GetLength();

		//1. j�� �����Ѵ�.
		if (i <= firstY) {
			j = firstX;
		}
		else {
			j = 0;
		}

		//2. limitX �� �����Ѵ�.
		if (i >= endY) {    
			limitX = endX;
		}
		else {
			limitX = row->GetLength();
		}

		while (j < limitX) {
			character = row->GetChild(j);
			content_ = character->GetContent();
			content = CString(content_.c_str());

			//(21.11.10.�߰�)
			if (content == "\t") {
				content = "        ";			
			}


			// ������ ���ڻ��� �ٲ��ش�.
			dcTemp.SetTextColor(RGB(255, 255, 255));
			dcTemp.SetBkColor(RGB(000, 102, 204));

			// X Ŀ�������� ������ �Ÿ��� ����Ѵ�.
			distanceX = this->getTextSize->totalCalculateX(j, row);

			// �ٲ� �κи� ������Ѵ�.
			dcTemp.TextOut(-this->scroll->siHorizon.nPos + distanceX, (strSizeY* i) - this->scroll->siVertical.nPos, content);
			j++;
		}

		i++;
	}


	//(21.10.15 ��Ʈ�� �߰�) 5. ��Ʈ���� ����Ѵ�.
	dc.BitBlt(0, 0, rect.right, rect.bottom, &dcTemp, 0, 0, SRCCOPY);

	//(21.10.15 ��Ʈ�� �߰�) 6. ���� ��Ʈ���� �����Ѵ�.
	dcTemp.SelectObject(oldBMP);
	::DeleteObject(hbmp);
	dcTemp.DeleteDC();


	//���ο� ��Ʈ ����
	cFont.DeleteObject();
#endif

}

void NotePadForm::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) { //1Byte �߰��� ��

	//OnCommand();

	//char character_[2];
	//this->character[0] = nChar;
	//this->character[1] = '\0';

	//this->nChar = nChar;
	//OnMyCommand(IDC_COMMAND_ONCHAR);


	if (((GetKeyState(VK_CONTROL) >= 0) && (nChar != 'Z' || nChar != 'Y')) && nChar != VK_BACK && nChar != VK_ESCAPE) { //OR !(GetKeyState(VK_CONTROL) & 0x8000) 
		this->nChar = nChar;
		//Notepad ������ �ڽ��� ������ ȣ���ϴ°� �����ؾ� �ϱ� ������ SendMessage�� Ȱ��
		//OnMyCommand(IDC_COMMAND_ONCHAR);

		//(21.09.14 �߰�) 1. ���õ� �ܾ ������, BackSpace���� ȣ���Ѵ�.
		if (this->selectWords->firstX > 0 || this->selectWords->firstY > 0 || this->selectWords->endX > 0 ||
			this->selectWords->endY > 0) {
			this->SendMessage(WM_COMMAND, 40013);
		}

		//2. OnChar ȣ���Ѵ�.
		this->SendMessage(WM_COMMAND, 40001, 0);
	}

#if 0
	char chaCharacter[3];
	string strCharacter;
	Glyph* character = NULL;
	Glyph* row;
	Long rowLength;
	Long noteLength;
	Long i;
	Long j;
	int currentY;
	int currentX;
	int caretX;
	int caretY;
	char characterMove[3];
	int location;
	Glyph* dummyRow;
	Glyph* nextRow;
	Glyph* testRow = NULL;
	int width;
	int exceptWidth;
	BOOL isLineChanged = FALSE;
	int beforeLineChangeCurrentX;
	int changedX;
	int changedY;

	//21.08.03 �߰�
	this->scroll->windowWidth = this->m_rectCurHist.right - this->m_rectCurHist.left;
	this->scroll->windowHeight = this->m_rectCurHist.bottom - this->m_rectCurHist.top;

	//&& (nChar == 0x56 && (GetKeyState(VK_CONTROL) < 0))
	if (nChar != VK_BACK ) { //BackSpace�� �ƴϸ�,

		char character_[2];
		character_[0] = nChar;
		character_[1] = '\0';

		CreateGlyph* creator = new CreateProduct();

		//1.1. ���� ĳ���� ��ġ�� Ȯ���Ѵ�.
		currentY = this->note->GetCurrent();
		row = this->note->GetChild(currentY);
		currentX = row->GetCurrent();
		noteLength = this->note->GetLength();
		rowLength = row->GetLength();

		//1.2. �ڵ����� ��ư�� ������ ���,
		if (this->isLineChangeButtonClicked == TRUE) {
			
			//1.2.1. �ڵ����� �� ĳ���� ��ġ�� ã�´�.
			this->lineChange->FindCaretBeforeLineChange(&currentX, &currentY);

			//1.2.2. �ڵ����� �� ������ ��ģ��.
			this->lineChange->LineChangeButtonNotClicked();

			row = this->note->GetChild(currentY);
			rowLength = row->GetLength();
		}

		//1.3. �Է¹��� ���ڰ� ���๮���� ���,
		if (character_[0] == '\n' || character_[0] == '\r') {

			//1.3.1. ���� note�� currentY+1��°�� �� ���� �����ִ´�. (�������� + ��������)
			this->row = creator->Make(character_);
			this->note->Add(currentY + 1, this->row);

			//1.3.2. currentX�� row�� length���� ���� ���, (��������)
			if (currentX < rowLength) {
				//1.3.2.1. ���� row�� length��ŭ �ݺ��Ѵ�.
				while (currentX < rowLength) {
					//1. ���ڸ� �д´�.
					character = row->GetChild(currentX);
					//2. currentY+1��°�� �������´�.
					this->row->Add(character);

					//3. currentX��° ���ڸ� �����Ѵ�.
					row->Remove(currentX);

					rowLength--;
				}
			}
			//1.3.3. ĳ���� (currentY+1, 0)���� ��ǥ�����Ѵ�.
			changedY = currentY + 1;
			changedX = 0;	
		}

		//1.4. �Է¹��� ���ڰ� ���๮�ڰ� �ƴ� ���,
		else {

			//1.4.0. character�� �����.
			character = creator->Make(character_);

			//1.4.1. currentX�� row�� length���� ���� ���(��������), ���� row�� currentX��°�� ���ڸ� �������´�.
			if (currentX < rowLength) {
				row->Add(currentX, character);
			}
			//1.4.2. currentX�� row�� length���� ũ�ų� ���� ���, ���� row�� ���� ���´�.
			else {
				row->Add(character);
			}
			//1.4.3. ĳ���� (currentY, currentX+1)�� ��ǥ�����Ѵ�.
			changedY = currentY;
			changedX = currentX + 1;
		}

		//1.5. �ڵ����� ��ư�� ������ ���,
		if (this->isLineChangeButtonClicked == TRUE) {

			//1.5.1. ��ģ ������ �ٽ� �ڵ������Ѵ�. (ON)
			this->lineChange->LineChangeButtonClicked();

			//1.5.1. �ڵ����� �� �̵��� ĳ���� ��ġ�� ���Ѵ�.
			this->lineChange->FindCaretAfterLineChange(changedX, changedY);
		}
		else {
			//1.6. ĳ���� �ű��.
			this->note->MoveCurrent(changedY);
			this->row = this->note->GetChild(changedY);
			this->row->MoveCurrent(changedX);
		}

		if (creator != NULL) {
			delete creator;
			creator = NULL;	
		}


		//2.6. currentX(row�� current)���� ���� ���� �ݺ��Ѵ�.
		this->isOnImeCharForMove = FALSE;
		this->isFirstComposingForMove = FALSE;


		//----------------------------------------------------------------------------
		this->scroll->MovePage();


		// Scroll ������Ʈ �� ���� ������ �����Ѵ�.
		this->isMoveActionHorizon = FALSE;
		this->isMoveActionVertical = FALSE;
		this->isAppend = FALSE;
		this->isInsert = FALSE;
		this->isDeleteAction = FALSE;

		//----------------------------------------------------------------------------

		Notify(); //Observer Pattern ����


		this->isChanged = TRUE;
		Invalidate();
	}
#endif
}

LRESULT NotePadForm::OnImeComposition(WPARAM wParam, LPARAM lParam) { //UINT wParam, Long LPARAM ���� ���� ,,WPARAM LPARAM

	//this->letter = wParam;

	char letter[3] = { '\0', };
	letter[0] = HIBYTE(LOWORD(wParam));
	letter[1] = LOBYTE(LOWORD(wParam));
	letter[2] = '\0';

	this->letter = "\0";
	this->letter = letter;

//	if (this->letter != "\0") {
		//(21.09.14 �߰�) 1. ���õ� �ܾ ������, BackSpace���� ȣ���Ѵ�.
		if (this->selectWords->firstX > 0 || this->selectWords->firstY > 0 || this->selectWords->endX > 0 ||
			this->selectWords->endY > 0) {
			this->SendMessage(WM_COMMAND, 40013);
		}

		//2. OnImeComposition�� ȣ���Ѵ�.
		this->SendMessage(WM_COMMAND, 40004);
//	}

	return DefWindowProc(WM_IME_COMPOSITION, wParam, lParam);

#if 0
	char letter[3];
	string fontName = this->font->GetFontName();
	int fontSize = this->font->GetFontSize();
	Long length;
	Long i = 0;
	string strCharacter;
	char chaCharacter[3];
	char characterMove[3];
	Long caretX;
	Long caretY;
	Glyph* dummyRow = NULL;
	BOOL isLineChanged = FALSE;
	int location;
	Glyph* row;
	int exceptWidth;


	letter[0] = HIBYTE(LOWORD(wParam));
	letter[1] = LOBYTE(LOWORD(wParam));
	letter[2] = '\0';

	CreateGlyph* creator = new CreateProduct();
	Glyph* character = creator->Make(letter);

	//21.08.03 �߰�
	this->scroll->windowWidth = this->m_rectCurHist.right - this->m_rectCurHist.left;
	this->scroll->windowHeight = this->m_rectCurHist.bottom - this->m_rectCurHist.top;


	//1. �������� ��
	  //1.1. ���� ĳ���� ��ġ�� Ȯ���Ѵ�.
	  Long currentY = this->note->GetCurrent();
	  this->row = this->note->GetChild(currentY);
	  Long currentX = this->row->GetCurrent();
	  length = this->row->GetLength();

	  //1.2. ���� ������ ���� �ܿ� ���� ���ڸ� �����.
	  if (this->isFirstComposing == FALSE) {
		  this->row->Remove(currentX); //this->index
	  }

	  //1.3. (21.08.12 �߰�) �ڵ������ư OFF�� ���,
	  if (this->isLineChangeButtonClicked == FALSE) {
		  //1.3. currentX�� row�� length-1���� �۰ų�, currentX�� length-1���� ũ�ų� ����, Ŀ�� ũ�Ⱑ 3�� ���,
		  if (currentX < length - 1) {
			  //|| (currentX >= length - 1 && this->caretWidth == 3) ��û���. �ٽ� ������.

			  //1.3.1. ���� �ٿ� ���� ���ڸ� �������´�.
			  this->row->Add(currentX, character);

			  this->isInsert = TRUE;
			  this->isAppend = FALSE;
			  this->isMoveActionVertical = FALSE;
			  this->isMoveActionHorizon = FALSE;
			  this->isDeleteAction = FALSE;
		  }

		  //1.4. currentX�� row�� length���� ũ�ų� ���ų�, currentX�� length-1���� ũ�ų� ����, Ŀ�� ũ�Ⱑ 3���� ū ���,
		  else if (currentX >= length || (currentX >= length - 1 && this->caretWidth > 3)) {
			  //1.4.1. ���� �ٿ� ���� ���ڸ� �������´�.
			  this->row->Add(character);

			  this->isInsert = FALSE;
			  this->isAppend = TRUE;
			  this->isMoveActionVertical = FALSE;
			  this->isMoveActionHorizon = FALSE;
		  }

		  //1.5. ��ǥ �̵����� �ʰ�, Ŀ�� ũ�⸸ �ٲ��ش�.
		  if (this->isFirstComposing == TRUE) {

			  this->isOnImeCharForMove = FALSE;
			  this->isFirstComposingForMove = this->isFirstComposing;

			  Notify();
		  }
	  
	  }

	  //1.4. (21.08.12 �߰�) �ڵ������ư ON�� ���,
	  else if (this->isLineChangeButtonClicked == TRUE) {
		  //1.4.0. ���������� ���,
		  if (currentY + 1 < this->note->GetLength()) {
			  dummyRow = this->note->GetChild(currentY + 1); 
		  }
		  else {
			  dummyRow = NULL;
		  }
		  //1.4.1. currentX�� row�� length-1���� �۰ų�, currentX�� length-1���� ũ�ų� ����, Ŀ�� ũ�Ⱑ 3�� ���,(��������)
		  if (currentX < length || (dummyRow!=NULL && dynamic_cast<DummyRow*>(dummyRow))) { //currentX < length - 1

			  row = this->note->GetChild(currentY);

			  //1.4.1.1. ���ǿ� ���� ĳ���� ������ �����Ѵ�.
			  if (currentX < length) { //length - 1
				  isLineChanged = FALSE;
			  }
			  else if (dummyRow != NULL && dynamic_cast<DummyRow*>(dummyRow)) {
				  isLineChanged = TRUE;
			  }

			  //1.4.1.2. ���� �ٿ� ���� ���ڸ� �������´�.
			  row->Add(currentX, character);

			  //(21.08.13. ���ܻ�Ȳ �߰�)********************************************
              //1. ���������ε� (dummy�� dummy����)
              //2. �� �� Ŀ�� ��ġ (currentX = row->GetLength())
              //3. �̴�� ���� ������ ������ isLineChanged = TRUE ������,
              //4. ������ ��쵵 �ִ�. ��, ���������µ��� windowWidth�� �ȳѱ�� ���
              //5. �׷��� �������� ��, width �׽�Ʈ ���ִ� ���� �߰�..

			  exceptWidth = this->getTextSize->totalCalculateX(row->GetLength(), row);
			  if (isLineChanged == TRUE && exceptWidth <= this->scroll->windowWidth) {
				  isLineChanged = FALSE;
			  }
			  //**********************************************************************

			  //1.4.1.3. ���� ������ ������ ���,
			  if (this->isFirstComposing == TRUE) {

				  //�ش� row���� �ڸ� ��ġ�� 0���� ũ�ų� �������� �ݺ��Ѵ�.
				  location = this->lineChange->FindSplitLocationByWriting(row);

				  i = currentY;
				  while (location >= 0) {
					  //1. �ش� row�� ���� row�� dummyRow�� �ƴϰų� ���� ���,dummyRow �����.
					  if (i + 1 < this->note->GetLength()) {

						  dummyRow = this->note->GetChild(i + 1);

						  if (!dynamic_cast<DummyRow*>(dummyRow)) {
							  dummyRow = new DummyRow();
							  this->note->Add(i + 1, dummyRow); 
						  }					  
					  }
					  else {

						  dummyRow = new DummyRow();
						  this->note->Add(dummyRow); 
					  }

					  //2. SplitForFront�� ȣ���Ѵ�.
					  dummyRow = row->SplitForFront(dummyRow, location);

					  //3. ��¹��� dummyRow���� �ڸ� ��ġ�� �ִ��� ã�´�.
					  location = this->lineChange->FindSplitLocationByWriting(dummyRow);
					  row = dummyRow;
					  i++;
				  }
				  
//#if 0
				  // ĳ�� ��ǥ�� �����Ѵ�.
				  if (isLineChanged == TRUE) { //���� �ٲ�����,
					  this->note->MoveCurrent(currentY + 1);
					  this->row = this->note->GetChild(currentY + 1);
					  this->row->MoveCurrent(0); // �ѱ��̹Ƿ� 0���� ����.

				  }
//#endif
			  }

		  }

		  //1.4. currentX�� row�� length���� ũ�ų� ���ų�, currentX�� length-1���� ũ�ų� ����, Ŀ�� ũ�Ⱑ 3���� ū ���,
		  else if (currentX >= length || (currentX >= length - 1 && this->caretWidth > 3)) {
			  //1.4.1. ���� �ٿ� ���� ���ڸ� �������´�.
			  this->row->Add(character);

			  //1.4.2. ���� ������ ������ ���,
			  if (this->isFirstComposing == TRUE) {
				  //1.4.2.1. �ش� row���� �ڸ� ��ġ�� 0���� ũ�ų� ���� ���,
				  location = this->lineChange->FindSplitLocationByWriting(this->row);
				  if (location >= 0) {
					  //1. dummyRow�� �����.
					  dummyRow = new DummyRow();

					  //2. dummyRow�� �߰��Ѵ�.
					  character = this->row->GetChild(location);
					  dummyRow->Add(character);

					  //3. currentY+1�� ��Ʈ�� �����.
					  this->note->Add(currentY + 1, dummyRow);

					  //4. ���� row���� ����
					  this->row->Remove(location);

					  //5. ĳ���� (currentY+1, 0)���� ��ǥ �����Ѵ�.
					  this->note->MoveCurrent(currentY + 1);
					  dummyRow->MoveCurrent(0);
				  }
			  }
		  }

		  //1.5. ��ǥ �̵����� �ʰ�, Ŀ�� ũ�⸸ �ٲ��ش�.
		  if (this->isFirstComposing == TRUE) {
			  // ��ũ�� ����
			  this->isInsert = FALSE;
			  this->isAppend = FALSE;
			  this->isMoveActionVertical = FALSE;
			  this->isMoveActionHorizon = FALSE;
			  this->isDeleteAction = FALSE;

			  //ĳ�� ��ǥ ����
			  this->isOnImeCharForMove = FALSE;
			  this->isFirstComposingForMove = this->isFirstComposing;

			  Notify();
		  }
	  }

	  this->isFirstComposing = FALSE;
	  this->isChanged = TRUE;

	  Invalidate();

	  return DefWindowProc(WM_IME_COMPOSITION, wParam, lParam);
#endif
}

LRESULT NotePadForm::OnImeStartComposition(WPARAM wParam, LPARAM lParam) {
	return 0;	
}
LRESULT NotePadForm::OnImeChar(WPARAM wParam, LPARAM lParam) {

	char letter[3];
	letter[0] = HIBYTE(LOWORD(wParam));
	letter[1] = LOBYTE(LOWORD(wParam));
	letter[2] = '\0';

	this->letter = letter;
	
	
	//this->letter = wParam;
	this->SendMessage(WM_COMMAND, IDC_COMMAND_ONIMECHAR);
	return 0;

#if 0
	Glyph* character;
	char letter[3];
	letter[0] = HIBYTE(LOWORD(wParam));
	letter[1] = LOBYTE(LOWORD(wParam));
	letter[2] = '\0';
	Long length;
	Long i = 0;
	string strCharacter;
	char chaCharacter[3];
	char characterMove[3];
	Long caretX;
	Long caretY;

	CreateGlyph* creator = new CreateProduct();
	character = creator->Make(letter);

	//21.08.03 �߰�
	this->scroll->windowWidth = this->m_rectCurHist.right - this->m_rectCurHist.left;
	this->scroll->windowHeight = this->m_rectCurHist.bottom - this->m_rectCurHist.top;



	//2. ���ڰ� �ϼ� �Ǿ��� ��
	  //2.1. ���� ĳ���� ��ġ�� Ȯ���Ѵ�.
	  Long currentY = this->note->GetCurrent();
	  this->row = this->note->GetChild(currentY);
	  Long currentX = this->row->GetCurrent();

	  //2.2. ������ ������ ���ڸ� �����.
	  this->row->Remove(currentX);
	  length = this->row->GetLength();


	  //(21.08.12. �߰�) �ڵ������ư OFF�� ���,
	  if (this->isLineChangeButtonClicked == FALSE) {

		  //2.3. currentX�� row�� length-1���� �۰ų�, currentX�� length-1���� ũ�ų� ����, Ŀ�� ũ�Ⱑ 3�� ���,
		  if (currentX < length - 1 || (currentX >= length - 1 && this->caretWidth == 3)) {
			  //2.3.1. ���� �ٿ� ���� ���ڸ� �������´�.
			  this->row->Add(currentX, character);

			  this->isInsert = TRUE;
			  this->isAppend = FALSE;
			  this->isMoveActionVertical = FALSE;
			  this->isMoveActionHorizon = FALSE;
			  this->isDeleteAction = FALSE;
		  }

		  //2.4. currentX�� row�� length���� ũ�ų� ���ų�, currentX�� length-1���� ũ�ų� ����, Ŀ�� ũ�Ⱑ 3���� ū ���,
		  else if (currentX >= length || (currentX >= length - 1 && this->caretWidth > 3)) {
			  //2.4.1. ���� �ٿ� ���� ���ڸ� �������´�.
			  this->row->Add(character);

			  this->isInsert = FALSE;
			  this->isAppend = TRUE;
			  this->isMoveActionVertical = FALSE;
			  this->isMoveActionHorizon = FALSE;
			  this->isDeleteAction = FALSE;
		  }

		  //2.5. ��ǥ�� currentX+1�� �̵��Ѵ�.
		  this->row->MoveCurrent(currentX + 1);
	  }
	  //(21.08.12. �߰�) �ڵ������ư ON�� ���,
	  else if (this->isLineChangeButtonClicked == TRUE) {

#if 0
		  //2.3. currentX�� row�� length-1���� �۰ų�, currentX�� length-1���� ũ�ų� ����, Ŀ�� ũ�Ⱑ 3�� ���,
		  if (currentX < length - 1 || (currentX >= length - 1 && this->caretWidth == 3)) {
			  //2.3.1. ���� �ٿ� ���� ���ڸ� �������´�.
			  this->row->Add(currentX, character);
		  }

		  //2.4. currentX�� row�� length���� ũ�ų� ���ų�, currentX�� length-1���� ũ�ų� ����, Ŀ�� ũ�Ⱑ 3���� ū ���,
		  else if (currentX >= length || (currentX >= length - 1 && this->caretWidth > 3)) {
			  //2.4.1. ���� �ٿ� ���� ���ڸ� �������´�.
			  this->row->Add(character);
		  }
#endif
		  this->row->Add(currentX, character);

		  //2.5. ��ǥ�� currentX+1�� �̵��Ѵ�.
		  this->row->MoveCurrent(currentX + 1);

		  //��ũ�� ����
		  this->isInsert = FALSE;
		  this->isAppend = FALSE;
		  this->isMoveActionVertical = FALSE;
		  this->isMoveActionHorizon = FALSE;
		  this->isDeleteAction = FALSE;
	  }

	  //2.7. currentX(row�� current)���� ���� ���� �ݺ��Ѵ�.
	  this->isOnImeCharForMove = TRUE;
	  this->isFirstComposingForMove = this->isFirstComposing;
	  Notify();


	  this->isFirstComposing = TRUE;
	  this->isChanged = TRUE;

	  Invalidate();

	  return 0;
#endif
}

void NotePadForm::OnSetFocus(CWnd* pOldWnd) { //Attach ���ֱ�
	CFrameWnd::OnSetFocus(pOldWnd);

	this->isKillFocus = FALSE;
	int strSize = this->getTextSize->GetX(128);

#if 0
	CreateSolidCaret(3, strSize); //ĳ������  (3, strSize.cy);
	SetCaretPos(CPoint(0,0)); //��ġ����

	ShowCaret(); //ȭ�鿡 ���̵��� �Ѵ�.
#endif

#if 0
	//(21.11.11.�߰�) ��� currentX, currentY�� ���� ���� ���, �������ش�.
	this->note->MoveCurrent(this->currentY);
	Glyph* row = this->note->GetChild(this->currentY);
	row->MoveCurrent(this->currentX);
#endif

	//1. Subject ����� Location�� �߰����ش�.

	if (this->location == NULL) {
		this->location = new Location(this);	
		dynamic_cast<Subject*>(this)->Attach(this->location); //Attach�� �Ű������� Observer* observe������
	}


	//������ location�� �θ�Ŭ������ Observer���� ���� �Ҵ����� �ʿ�x.

#if 0
	// Scroll ������Ʈ �� ���� ������ �����Ѵ�.
	this->isJumpOverForPrevious = false;
	this->isJumpOverForNext = false;
	this->isDoubleByte = false;
	this->isUp = false;
	this->isDown = false;

	Notify();
	Invalidate();

#endif
}

void NotePadForm::OnKillFocus(CWnd* pNewWnd) { //Detach ���ֱ�
	CFrameWnd::OnKillFocus(pNewWnd);

	this->isKillFocus = TRUE;

#if 0

	//(21.11.11.�߰�) 1. ���� ĳ���� ��ġ�� ���Ѵ�.
	this->currentY = this->note->GetCurrent();
	Glyph* row = this->note->GetChild(this->currentY);
	this->currentX = row->GetCurrent();


	//2. ĳ���� ���ش�.
	DestroyCaret();
#endif
}

void NotePadForm::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long currentY;
	Long currentX;
	Glyph* character;
	Glyph* row;
	Long i = 0; 
	int strSizeY;
	string strCharacter;
	char chaCharacter[3];
	Long caretX;
	Long caretY;
	Long DoubleByteCaretWidth;
	Long length;
	char characterMove[3]; //Observe Pattern ������.
	KeyAction* keyAction = NULL;
	FactoryProduct factoryProduct(this);
	Command* command;

	//***************OnKeyDown�� ���丮���鶧 �Ҵ��� ��� �ϰ��ִ�. ����ȭ �����̸� �������� �����
	//***************Make �ϸ� �ȴ�.FactoryProduct factoryProduct; factoryProduct.Make(~~);

	this->nChar = nChar;

	//0. �̵� Ű�� �������� ���õ� ��ǥ�� �ʱ�ȭ�Ѵ�.*****************************************************
	//(nChar == VK_LEFT && (GetKeyState(VK_CONTROL) >= 0) && (GetKeyState(VK_SHIFT) >= 0)) ||
	//(nChar == VK_RIGHT && (GetKeyState(VK_CONTROL) >= 0) && (GetKeyState(VK_SHIFT) >= 0)) ||

	if ((nChar == VK_LEFT && (GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_SHIFT) >= 0))||
		(nChar == VK_RIGHT && (GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_SHIFT) >= 0))||
		(nChar == VK_HOME && (GetKeyState(VK_CONTROL) >= 0) && (GetKeyState(VK_SHIFT) >= 0))||
		(nChar == VK_END && (GetKeyState(VK_CONTROL) >= 0) && (GetKeyState(VK_SHIFT) >= 0))||
		(nChar == VK_HOME && (GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_SHIFT) >= 0))||
		(nChar == VK_END && (GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_SHIFT) >= 0)) ||
		(nChar == VK_UP && (GetKeyState(VK_CONTROL) >= 0) && (GetKeyState(VK_SHIFT) >= 0)) ||
		(nChar == VK_DOWN && (GetKeyState(VK_CONTROL) >= 0) && (GetKeyState(VK_SHIFT) >= 0)))  {

		this->selectWords->firstX = 0;
		this->selectWords->firstY = 0;
		this->selectWords->endX = 0;
		this->selectWords->endY = 0;

		//1. Shift Ű���� ���� ������, �巡�� ���� false�� �����Ѵ�.
		this->selectWords->isDragging = FALSE;
		this->isShiftClicked = FALSE;
		this->isShifting = FALSE;

		this->ShiftX = 0;
		this->ShiftY = 0;
	}
	//******************************************************************************************************

	//37. (21.11.17.�߰�) Shift + ���콺 ���� ��ư ������ ��
	if (GetKeyState(VK_SHIFT) < 0) {

		if (this->isShifting == TRUE) {

			this->ShiftY = this->selectWords->firstY;
			this->ShiftX = this->selectWords->firstX;
		
		}
		else {
			this->ShiftY = this->note->GetCurrent();
			Glyph* testRow = this->note->GetChild(this->ShiftY);
			this->ShiftX = testRow->GetCurrent();
		}

		//this->selectWords->firstX = this->ShiftX;
		//this->selectWords->firstY = this->ShiftY;
		this->isShifting = TRUE;
		this->isShiftClicked = TRUE;

	}
	else {
		this->isShiftClicked = FALSE;
	}


	//1. ���� Ű ��ư ������ ��
	if (nChar == VK_LEFT && (GetKeyState(VK_CONTROL) >= 0) && (GetKeyState(VK_SHIFT) >= 0)) {
		this->SendMessage(WM_COMMAND, 40025);

#if 0
		//this->factoryProduct = new FactoryProduct(this);
		keyAction = factoryProduct.Make(nChar, NULL, NULL);
		keyAction->KeyDown();
#endif
	
	}
	//2. ������ Ű ��ư ������ ��
	else if (nChar == VK_RIGHT && (GetKeyState(VK_CONTROL) >= 0) && (GetKeyState(VK_SHIFT) >= 0)) {
		this->SendMessage(WM_COMMAND, 40026);
#if 0
		//this->factoryProduct = new FactoryProduct(this);
		keyAction = factoryProduct.Make(nChar, NULL, NULL);
		keyAction->KeyDown();
#endif
	}
	//3. Ctrl + ���� Ű ������ ��
	else if (nChar == VK_LEFT && (GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_SHIFT) >= 0)) {
		this->SendMessage(WM_COMMAND, 40027);
#if 0
		//this->factoryProduct = new FactoryProduct(this);
		keyAction = factoryProduct.Make(nChar, VK_CONTROL, NULL);
		keyAction->KeyDown();
#endif
	}
	//4. Ctrl + ������ Ű ������ ��
	else if (nChar == VK_RIGHT && (GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_SHIFT) >= 0)) {
		this->SendMessage(WM_COMMAND, 40028);
#if 0
		//this->factoryProduct = new FactoryProduct(this);
		keyAction = factoryProduct.Make(nChar, VK_CONTROL, NULL);
		keyAction->KeyDown();
#endif
	}
	//5. home Ű ������ ��
	else if (nChar == VK_HOME && (GetKeyState(VK_CONTROL) >= 0) && (GetKeyState(VK_SHIFT) >= 0)) {
		this->SendMessage(WM_COMMAND, 40029);
#if 0
		//this->factoryProduct = new FactoryProduct(this);
		keyAction = factoryProduct.Make(nChar, NULL, NULL);
		keyAction->KeyDown();
#endif
	}
	//6. end Ű ������ ��
	else if (nChar == VK_END && (GetKeyState(VK_CONTROL) >= 0) && (GetKeyState(VK_SHIFT) >= 0)) {
		this->SendMessage(WM_COMMAND, 40030);
#if 0
		//this->factoryProduct = new FactoryProduct(this);
		keyAction = factoryProduct.Make(nChar, NULL, NULL);
		keyAction->KeyDown();
#endif
	}
	//7. Ctrl + home Ű ������ ��
	else if (nChar == VK_HOME && (GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_SHIFT) >= 0)) { 
		this->SendMessage(WM_COMMAND, 40031);
#if 0
		//this->factoryProduct = new FactoryProduct(this);
		keyAction = factoryProduct.Make(nChar, VK_CONTROL, NULL);
		keyAction->KeyDown();
#endif
	}
	//8. Ctrl + end Ű ������ ��
	else if (nChar == VK_END && (GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_SHIFT) >= 0)) { 
		this->SendMessage(WM_COMMAND, 40032);
#if 0
		//this->factoryProduct = new FactoryProduct(this);
		keyAction = factoryProduct.Make(nChar, VK_CONTROL, NULL);
		keyAction->KeyDown();
#endif
	}
	//9. ���� Ű ������ ��
	else if (nChar == VK_UP && (GetKeyState(VK_CONTROL) >= 0) && (GetKeyState(VK_SHIFT) >= 0)) {
		this->SendMessage(WM_COMMAND, 40033);
#if 0
		//this->factoryProduct = new FactoryProduct(this);
		keyAction = factoryProduct.Make(nChar, NULL, NULL);
		keyAction->KeyDown();
#endif
	}
	//10. �Ʒ��� Ű ������ ��
	else if (nChar == VK_DOWN && (GetKeyState(VK_CONTROL) >= 0) && (GetKeyState(VK_SHIFT) >= 0)) {
		this->SendMessage(WM_COMMAND, 40034);
#if 0
		//this->factoryProduct = new FactoryProduct(this);
		keyAction = factoryProduct.Make(nChar, NULL, NULL);
		keyAction->KeyDown();
#endif
	}
	//11.(21.08.17 �߰�) Shift + ���� Ű ��ư ������ ��
	else if (nChar == VK_LEFT && (GetKeyState(VK_CONTROL) >= 0) && (GetKeyState(VK_SHIFT) < 0)) {
		this->SendMessage(WM_COMMAND, 40015);
#if 0
		//this->factoryProduct = new FactoryProduct(this);
		keyAction = factoryProduct.Make(nChar, VK_SHIFT, NULL);
		keyAction->KeyDown();
#endif
	}

	//12. (21.08.17 �߰�) Shift + ������ Ű ��ư ������ ��
	else if (nChar == VK_RIGHT && (GetKeyState(VK_CONTROL) >= 0) && (GetKeyState(VK_SHIFT) < 0)) {
		this->SendMessage(WM_COMMAND, 40016);
#if 0
		//this->factoryProduct = new FactoryProduct(this);
		keyAction = factoryProduct.Make(nChar, VK_SHIFT, NULL);
		keyAction->KeyDown();
#endif
	}

	//13. (21.08.18 �߰�) Shift + Ctrl + ���� Ű ��ư ������ ��
	else if (nChar == VK_LEFT && (GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_SHIFT) < 0)) {
		this->SendMessage(WM_COMMAND, 40018);
#if 0
		keyAction = factoryProduct.Make(nChar, VK_CONTROL, VK_SHIFT);
		keyAction->KeyDown();
#endif
	}

	//14. (21.08.18 �߰�) Shift + Ctrl + ������ Ű ��ư ������ ��
	else if (nChar == VK_RIGHT && (GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_SHIFT) < 0)) {
		this->SendMessage(WM_COMMAND, 40017);
#if 0
		keyAction = factoryProduct.Make(nChar, VK_CONTROL, VK_SHIFT);
		keyAction->KeyDown();
#endif
	}

	//15. (21.08.18 �߰�) Shift + home Ű ��ư ������ ��
	else if (nChar == VK_HOME && (GetKeyState(VK_SHIFT) < 0) && (GetKeyState(VK_CONTROL) >= 0)) {
		this->SendMessage(WM_COMMAND, 40019);
#if 0
		keyAction = factoryProduct.Make(nChar, VK_SHIFT, NULL);
		keyAction->KeyDown();
#endif
	}

	//16. (21.08.18 �߰�) Shift + end Ű ��ư ������ ��
	else if (nChar == VK_END && (GetKeyState(VK_SHIFT) < 0) && (GetKeyState(VK_CONTROL) >= 0)) {
		this->SendMessage(WM_COMMAND, 40020);
#if 0
		keyAction = factoryProduct.Make(nChar, VK_SHIFT, NULL);
		keyAction->KeyDown();
#endif
	}

	//17.  Ctrl + Shift + home Ű ������ ��
	else if (nChar == VK_HOME && (GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_SHIFT) < 0)) { //(GetKeyState(VK_CONTROL) < 0)
	    this->SendMessage(WM_COMMAND, 40021);
#if 0
        //this->factoryProduct = new FactoryProduct(this);
		keyAction = factoryProduct.Make(nChar, VK_CONTROL, VK_SHIFT);
		keyAction->KeyDown();
#endif
	}

	//18.  Ctrl + Shift + end Ű ������ ��
	else if (nChar == VK_END && (GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_SHIFT) < 0)) { //(GetKeyState(VK_CONTROL) < 0)
	    this->SendMessage(WM_COMMAND, 40022);
#if 0
		//this->factoryProduct = new FactoryProduct(this);
		keyAction = factoryProduct.Make(nChar, VK_CONTROL, VK_SHIFT);
		keyAction->KeyDown();
#endif
	}

	//19. (21.08.17 �߰�) Shift + ���� Ű ��ư ������ ��
	else if (nChar == VK_UP && (GetKeyState(VK_CONTROL) >= 0) && (GetKeyState(VK_SHIFT) < 0)) {
	    this->SendMessage(WM_COMMAND, 40023);
#if 0
		//this->factoryProduct = new FactoryProduct(this);
		keyAction = factoryProduct.Make(nChar, VK_SHIFT, NULL);
		keyAction->KeyDown();
#endif
	}

	//20. (21.08.17 �߰�) Shift + �Ʒ��� Ű ��ư ������ ��
	else if (nChar == VK_DOWN && (GetKeyState(VK_CONTROL) >= 0) && (GetKeyState(VK_SHIFT) < 0)) {
	    this->SendMessage(WM_COMMAND, 40024);
#if 0
		//this->factoryProduct = new FactoryProduct(this);
		keyAction = factoryProduct.Make(nChar, VK_SHIFT, NULL);
		keyAction->KeyDown();
#endif
	}
	//21. BACKSPACE ������ ��
	else if (nChar == VK_BACK && (GetKeyState(VK_CONTROL) >= 0)) {  	    
#if 0
		//this->factoryProduct = new FactoryProduct(this);
		keyAction = factoryProduct.Make(nChar, NULL, NULL);
		keyAction->KeyDown();
#endif
		this->SendMessage(WM_COMMAND, 40013);
	}
	//22. Delete ������ ��
	else if (nChar == VK_DELETE && (GetKeyState(VK_CONTROL) >= 0)) {
#if 0
		//this->factoryProduct = new FactoryProduct(this);
		keyAction = factoryProduct.Make(nChar, NULL, NULL);
		keyAction->KeyDown();
#endif
		this->SendMessage(WM_COMMAND, 40038);
	}
	//23. Ctrl + C (�����ϱ�) ������ ��
	else if (nChar == 0x43 && (GetKeyState(VK_CONTROL) < 0)) {
	    this->SendMessage(WM_COMMAND, 40035);
#if 0
	    keyAction = factoryProduct.Make(nChar, VK_CONTROL, NULL);
	    keyAction->KeyDown();
#endif
    }
	//24. Ctrl + V (�����ֱ�) ������ ��
	else if (nChar == 0x56 && (GetKeyState(VK_CONTROL) < 0)) {
	     this->SendMessage(WM_COMMAND, 40036);
#if 0
	    keyAction = factoryProduct.Make(nChar, VK_CONTROL, NULL);
	    keyAction->KeyDown();
#endif
	}
	
	//25. (21.08.24. �߰�) Ctrl + X (�߶󳻴�) ������ ��
	else if (nChar == 0x58 && (GetKeyState(VK_CONTROL) < 0)) {
	     this->SendMessage(WM_COMMAND, 40037);
#if 0
	    keyAction = factoryProduct.Make(nChar, VK_CONTROL, NULL);
	    keyAction->KeyDown();
#endif
    } 
	//26. (21.08.26. �߰�) Ctrl + F (ã��) ������ ��
	else if (nChar == 0x46 && (GetKeyState(VK_CONTROL) < 0)) {
	     this->SendMessage(WM_COMMAND, 40039);
#if 0
	   if (this->findDialog == NULL) {
		   this->findDialog = new FindDialog(this);
	   }
	   this->findDialog->Create(NULL, FR_DOWN, this);
#endif
    }
	
	// (21.09.15 �߰�) Ctrl + H (�ٲٴ�) ������ ��
	else if (nChar == 0x48 && (GetKeyState(VK_CONTROL) < 0)) {
	     this->SendMessage(WM_COMMAND, 40043);
    }
	//27. (21.09.02 �߰�) Ctrl + Z (�������) ������ ��
	else if (nChar == 0x5A && (GetKeyState(VK_CONTROL) < 0)) {
		//OnMyCommand(2002);
		this->SendMessage(WM_COMMAND, 40002);
    }
	//28. (21.09.05 �߰�) Ctrl + Y (���ູ��) ������ ��
	else if (nChar == 0x59 && (GetKeyState(VK_CONTROL) < 0)) {
	    this->SendMessage(WM_COMMAND, 40003);
    }
	//29. (21.09.07 �߰�) Ctrl + S (����) ������ ��
	else if (nChar == 0x53 && (GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_SHIFT) >= 0)) {
	    this->SendMessage(WM_COMMAND, 40006);
	}
	//30. (21.09.07 �߰�) Ctrl + Shift + S (�ٸ� �̸����� ����) ������ ��
	else if (nChar == 0x53 && (GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_SHIFT) < 0)) {
	    this->SendMessage(WM_COMMAND, 40007);
	}
	//31. (21.09.07 �߰�) Ctrl + O (����) ������ ��
	else if (nChar == 0x4F && (GetKeyState(VK_CONTROL) < 0)) {
	    this->SendMessage(WM_COMMAND, 40008);
    }
	//32. (21.09.14 �߰�) F3 ������ �� (���� ã��)
	else if (nChar == VK_F3 && (GetKeyState(VK_CONTROL) >= 0) && (GetKeyState(VK_SHIFT) >= 0)) {

		//1. ã�� ���� ���ų� FindDialog�� NULL�̸� FindDialog ����
		if (this->findSameText->findStr == "") { //
			this->SendMessage(WM_COMMAND, 40039);
		}
		//2. ã�� ���� ������ �ٷ� FindNext ����.
		else if (this->findSameText->findStr != "") {
			this->SendMessage(WM_COMMAND, 40040);
		}
    }
	//33. (21.09.16.�߰�) F3 + SHIFT ������ ��
	else if (nChar == VK_F3 && (GetKeyState(VK_CONTROL) >= 0) && (GetKeyState(VK_SHIFT) < 0)) {
		//1. ã�� ���� ���ų� FindDialog�� NULL�̸� FindDialog ����
		if (this->findSameText->findStr == "") { //
			this->SendMessage(WM_COMMAND, 40039);
		}
		//2. ã�� ���� ������ �ٷ� FindNext ����.
		else if (this->findSameText->findStr != "") {
			this->SendMessage(WM_COMMAND, 40041);
		}
    }
	//34. (21.09.30 �߰�) Ctrl + N (���� �����) ������ ��
	else if (nChar == 0x4E && (GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_SHIFT) >= 0)) {
	    this->SendMessage(WM_COMMAND, 40048);
	}
	//35. (21.09.30 �߰�) Ctrl + A (��ü ����) ������ ��
	else if (nChar == 0x41 && (GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_SHIFT) >= 0)) {
	    this->SendMessage(WM_COMMAND, 40049);
    }
	//36. (21.09.30 �߰�) F5 ������ ��
	else if (nChar == VK_F5 && (GetKeyState(VK_CONTROL) >= 0) && (GetKeyState(VK_SHIFT) >= 0)) {
	    this->SendMessage(WM_COMMAND, 40050);
    }
	//37. (21.11.21.�߰�) PageUp ������ ��
	else if (nChar == VK_PRIOR) {
	    this->nSBCode = SB_PAGEUP;
		this->isKeyBoard = TRUE;
	    this->SendMessage(WM_COMMAND, 40012);
    }
	//38. (21.11.21.�߰�) PageDown ������ ��
	else if (nChar == VK_NEXT) {
	    this->nSBCode = SB_PAGEDOWN;
	    this->isKeyBoard = TRUE;
	    this->SendMessage(WM_COMMAND, 40012);
	}


#if 0
	//37. (21.11.17.�߰�) Shift + ���콺 ���� ��ư ������ ��
	if (GetKeyState(VK_SHIFT) < 0) {
	    this->isShiftClicked = TRUE;
		this->ShiftY = this->note->GetCurrent();
		Glyph* testRow = this->note->GetChild(ShiftY);
		this->ShiftX = testRow->GetCurrent();
	}
	else {
		this->isShiftClicked = FALSE;
	}
#endif


	if (keyAction != NULL) {
		delete keyAction;
		keyAction = NULL;
	}


}

void NotePadForm::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {

#if 0
	if (nChar == VK_SHIFT) {
		//1. Shift Ű���� ���� ������, �巡�� ���� false�� �����Ѵ�.
		this->selectWords->isDragging = FALSE;
		this->isShiftClicked = FALSE;
		this->isShifting = FALSE;

		this->ShiftX = 0;
		this->ShiftY = 0;

	}
#endif
}

void NotePadForm::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	
	this->nSBCode = nSBCode;
	this->SendMessage(WM_COMMAND, 40011);
	
#if 0
	MoveAction* MoveAction = NULL;

	FactoryProductScroll factoryProductScroll(this);
	

	//1. SB_LINERIGHT�� Ŭ������ ��
	if (nSBCode == SB_LINELEFT) {
		MoveAction = factoryProductScroll.MakeHorizon(nSBCode);
		//MoveAction = this->factoryProductScroll->MakeHorizon(nSBCode);
		MoveAction->KeyDown();
	}
	//2. SB_LINELEFT�� Ŭ������ ��
	else if (nSBCode == SB_LINERIGHT) {
		MoveAction = factoryProductScroll.MakeHorizon(nSBCode);
		//MoveAction = this->factoryProductScroll->MakeHorizon(nSBCode);
		MoveAction->KeyDown();
	}
	//3. SB_PAGELEFT�� Ŭ������ ��
	else if (nSBCode == SB_PAGELEFT) {
		MoveAction = factoryProductScroll.MakeHorizon(nSBCode);
		//MoveAction = this->factoryProductScroll->MakeHorizon(nSBCode);
		MoveAction->KeyDown();
	}
	//3. SB_PAGERIGHT�� Ŭ������ ��
	else if (nSBCode == SB_PAGERIGHT) {
		MoveAction = factoryProductScroll.MakeHorizon(nSBCode);
		//MoveAction = this->factoryProductScroll->MakeHorizon(nSBCode);
		MoveAction->KeyDown();
	}
	else if (nSBCode == SB_THUMBTRACK) {
		MoveAction = factoryProductScroll.MakeHorizon(nSBCode);
		MoveAction->KeyDown();
	}
#endif

}

void NotePadForm::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {

	this->nSBCode = nSBCode;
	this->isKeyBoard = FALSE;
	this->SendMessage(WM_COMMAND, 40012);

#if 0
	MoveAction* MoveAction = NULL;
	FactoryProductScroll factoryProductScroll(this);

	//1. SB_LINEUP�� Ŭ������ ��
	if (nSBCode == SB_LINEUP) {
		MoveAction = factoryProductScroll.MakeVertical(nSBCode);
		//MoveAction = this->factoryProductScroll->MakeVertical(nSBCode);
		MoveAction->KeyDown();
	}
	//2. SB_LINEDOWN�� Ŭ������ ��
	else if (nSBCode == SB_LINEDOWN) {
		MoveAction = factoryProductScroll.MakeVertical(nSBCode);
		//MoveAction = this->factoryProductScroll->MakeVertical(nSBCode);
		MoveAction->KeyDown();
	}
	//3. SB_PAGEUP�� Ŭ������ ��
	else if (nSBCode == SB_PAGEUP) {
		MoveAction = factoryProductScroll.MakeVertical(nSBCode);
		MoveAction->KeyDown();
	}
	//4. SB_PAGEDOWN�� Ŭ������ ��
	else if (nSBCode == SB_PAGEDOWN) {
		MoveAction = factoryProductScroll.MakeVertical(nSBCode);
		MoveAction->KeyDown();
	}
	else if (nSBCode == SB_THUMBTRACK) {
		MoveAction = factoryProductScroll.MakeVertical(nSBCode);
		MoveAction->KeyDown();
	}
	else if (nSBCode == SB_TOP) {
		MoveAction = factoryProductScroll.MakeVertical(nSBCode);
		MoveAction->KeyDown();
	}


#if 0
	//2. SB_LINELEFT�� Ŭ������ ��
	else if (nSBCode == SB_LINEDOWN) {
		MoveAction = factoryProductScroll.Make(nSBCode);
		MoveAction->KeyDown();
	}
	//3. SB_PAGELEFT�� Ŭ������ ��
	else if (nSBCode == SB_PAGEUP) {
		MoveAction = factoryProductScroll.Make(nSBCode);
		MoveAction->KeyDown();
	}
	//3. SB_PAGERIGHT�� Ŭ������ ��
	else if (nSBCode == SB_PAGEDOWN) {
		MoveAction = factoryProductScroll.Make(nSBCode);
		MoveAction->KeyDown();
	}
#endif

#endif
}

#if 0
void NotePadForm::OnFindButtonClicked() {

	//1. ã�� ��ư�� Ŭ������ ��
	  //1.1. ã�� Dialog�� �θ���.
	if (this->findDialog == NULL) {
		this->findDialog = new FindDialog(this);
	}
	this->findDialog->Create(NULL, FR_DOWN, this);

#if 0
	  m_pFRDlg = new CFindReplaceDialog(); // Must be created on the heap
	  m_pFRDlg->Create(TRUE, _T(""), _T(""), FR_DOWN | FR_FINDNEXT| FR_MATCHCASE, this);
	  findNext = m_pFRDlg->FindNext();

	  //1.2. "���� ã��"��ư�� ������ ���,
	  if (m_pFRDlg->FindNext() == TRUE) { // m_pFRDlg->FindNext() == TRUE
		  
#if 0
		  m_pFRDlg->m_fr.lStructSize = sizeof(FINDREPLACE);
		  m_pFRDlg->m_fr.hwndOwner = this->m_hWnd;
		  m_pFRDlg->m_fr.Flags = FR_FINDNEXT;


		  m_pFRDlg->m_fr.lpstrFindWhat = m_pFRDlg->GetFindWhatStr();
		  //m_pFRDlg->m_fr.lpstrReplaceWith = m_pFRDlg->GetReplaceWithStr();
		  m_pFRDlg->m_fr.wFindWhatLen = m_pFRDlg->GetFindWhatStrLen();
		  //m_pFRDlg->m_fr.wReplaceWithLen = m_pFRDlg->GetReplaceWithStrLen();
#endif

			  //1.2.1. ������ ã�� ������ Ȯ���Ѵ�.
			  findStr = m_pFRDlg->GetFindString();
			  this->findSameText->findStr = findStr;

			  //1.2.2. ����ڰ� �˻� ���ڿ��� ��/�ҹ��ڸ� ��Ȯ�� ��ġ��Ű���� Ȯ���Ѵ�. (����:TRUE, �ȱ���:FALSE)
			  matchCase = m_pFRDlg->MatchCase();
			  this->findSameText->matchCase = matchCase;

			  //1.2.3. ����ڰ� �˻��� �Ʒ��� �������� �����ϴ��� Ȯ���Ѵ� (�Ʒ���:TRUE, ����:FALSE)
			  searchDown = m_pFRDlg->SearchDown();
			  this->findSameText->searchDown = searchDown;

			  //1.2.4. ����ڰ� ��ü �ܾ ��ġ��ų�� ���θ� Ȯ���Ѵ�.
			  matchWholeWord = m_pFRDlg->MatchWholeWord();
			  this->findSameText->matchWholeWord = matchWholeWord;

			  //1.2.5. FindSameTextŬ������ SearchText�� ȣ���Ѵ�.
			  this->findSameText->SearchText();

			  //1.2.6. ���� �ܾ ������ �޼����ڽ��� ����Ѵ�. (""��(��) ã�� �� �����ϴ�.)
	  }

	// Notify(); // ���� ��ũ�� ���� �ۿ� �ܾ ���� ���
	Invalidate();
#endif
}



void NotePadForm::OnReplaceButtonClicked() {

	//2. �ٲٱ� ��ư�� Ŭ������ ��
	  //2.1. �ٲٱ� Dialog�� �θ���.
	if (this->replaceDialog == NULL) {
		this->replaceDialog = new ReplaceDialog(this);
	}
	this->replaceDialog->Create(NULL, FR_DOWN, this);


}

#endif

void NotePadForm::OnLButtonDown(UINT nFlags, CPoint point) {
	Long x = point.x + this->scroll->siHorizon.nPos;
	Long y = point.y + this->scroll->siVertical.nPos;
	Long currentY;
	Long currentX;
	Glyph* row;
	BOOL isBigger = TRUE;

	//(21.11.17.�߰�) �����ϱ� �� ��ǥ�� ���س��´�.
	Long beforeCurrentY = this->note->GetCurrent();
	row = this->note->GetChild(beforeCurrentY);
	Long beforeCurrentX = row->GetCurrent();


	//0. ���õ� �ܾ �ʱ�ȭ�Ѵ�.
	this->selectWords->firstX = 0;
	this->selectWords->firstY = 0;
	this->selectWords->endX = 0;
	this->selectWords->endY = 0;

	this->selectWords->isDragging = FALSE;
	this->isShiftClicked = FALSE;
	this->isShifting = FALSE;

	this->ShiftX = 0;
	this->ShiftY = 0;

	//0. ���콺�� ���� ������ �̵����� �� WM_MOUSEMOVE�� �ް� ������ SetCapture, ReleaseCapture�� ���
	this->SetCapture();

	//1. ���� ������ ��ǥ�� ���Ѵ�.
	//2. y�� ���Ѵ�.

	currentY = this->getTextSize->GetMouseY(y);
	this->row = this->note->GetChild(currentY);
	currentX = this->getTextSize->GetMouseX(x, this->row);

	if (currentX < 0) {
		currentX = 0;
	}

	//4. �ش� ��ǥ�� ���� ĳ���� ��ġ�� ���Ѵ�.
	this->note->MoveCurrent(currentY);
	this->row->MoveCurrent(currentX);

	//5. �÷��� �����Ѵ�.
	this->isMouseDrag = TRUE;

#if 0
	//6. ���õ� �ܾ ���Ѵ�.
	this->selectWords->firstX = currentX;
	this->selectWords->firstY = currentY;
	this->selectWords->endX = currentX;
	this->selectWords->endY = currentY;
#endif

	//(21.11.17.�߰�) ���� ShiftŰ ���������� �ش� ��ǥ���� �������ش�.
	if (this->isShiftClicked == TRUE) {

		this->selectWords->firstX = this->ShiftX;
		this->selectWords->firstY = this->ShiftY;
		this->selectWords->endX = currentX;
		this->selectWords->endY = currentY;


	}
	else {
		//6. ���õ� �ܾ ���Ѵ�.
		this->selectWords->firstX = currentX;
		this->selectWords->firstY = currentY;
		this->selectWords->endX = currentX;
		this->selectWords->endY = currentY;
	}

	this->isJumpOverForPrevious = FALSE;
	this->isJumpOverForNext = TRUE;
	this->isUp = FALSE;
	this->isDown = FALSE;
	this->isDoubleByte = FALSE;
	

	this->Notify();	
	this->Invalidate();
}

void NotePadForm::OnLButtonDblClk(UINT nFlags, CPoint point) {
	Long x = point.x;
	Long y = point.y;
	Long currentY;
	Long currentX;
	Long leftX;
	Long rightX;
	Glyph* row;

	//1. ���� ��ǥ�� ���Ѵ�.
	currentY = this->getTextSize->GetMouseY(y);
	this->row = this->note->GetChild(currentY);
	currentX = this->getTextSize->GetMouseX(x, this->row);

	//2. leftX�� ���Ѵ�.
	leftX = this->getTextSize->GetLeftX(currentX, this->row);

	//3. rightX�� ���Ѵ�.
	rightX = this->getTextSize->GetRightX(currentX, this->row);
	
	//4. rightX�� ���� ĳ���� ��ġ�� �̵��Ѵ�.
	this->row->MoveCurrent(rightX);

	//5. leftX, rightX�� ���� ó���Ѵ�.
	this->selectWords->firstX = leftX;
	this->selectWords->firstY = currentY;
	this->selectWords->endX = rightX;
	this->selectWords->endY = currentY;

	//6. Notify ������ �����Ѵ�.
	this->isJumpOverForPrevious = FALSE;
	this->isJumpOverForNext = FALSE;
	this->isUp = FALSE;
	this->isDown = FALSE;
	this->isDoubleByte = FALSE;

	this->Notify();
	this->Invalidate();
}

void NotePadForm::OnMouseMove(UINT nFlags, CPoint point) {
	Long x = point.x + this->scroll->siHorizon.nPos;
	Long y = point.y + this->scroll->siVertical.nPos;
	Long currentY;
	Long currentX;
	int previousX = this->selectWords->endX;

	//1. ���콺�� �巡�� ���̸�,
	if (this->isMouseDrag == TRUE) {

		//1.1. ���� ��ǥ�� ���Ѵ�.
#if 0
		currentY = this->getTextSize->GetMouseY(y);
		this->row = this->note->GetChild(currentY);
		currentX = this->getTextSize->GetMouseX(x, this->row);
#endif
		//2. y�� ���Ѵ�.
		if (y >= 0) {
			currentY = this->getTextSize->GetMouseY(y);
		}
		else {
			currentY = 0;
		}
		this->row = this->note->GetChild(currentY);

		//3. x�� ���Ѵ�.
		if (x > 0) { //x >= 0
			currentX = this->getTextSize->GetMouseX(x, this->row);
		}
		else {
			currentX = this->row->GetLength(); 
		}


		//1.2. �ش� ��ǥ�� ĳ���� �̵��Ѵ�.
		this->note->MoveCurrent(currentY);
		this->row = this->note->GetChild(currentY);
		this->row->MoveCurrent(currentX);

		//1.3. ���õ� ��ǥ�� ���Ѵ�.
		this->selectWords->endY = currentY;
		this->selectWords->endX = currentX;

		//2. Notify ������ �����Ѵ�.
		if (previousX < currentX) {
			this->isJumpOverForNext = TRUE;
			this->isJumpOverForPrevious = FALSE;
		}
		else {
			this->isJumpOverForNext = FALSE;
			this->isJumpOverForPrevious = TRUE;
		}
		this->isUp = FALSE;
		this->isDown = FALSE;
		this->isDoubleByte = FALSE;

		this->Notify();
		RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	}
}

void NotePadForm::OnLButtonUp(UINT nFlags, CPoint point) {

	//1. �÷��װ� FALSE�̸�,
	if (this->isMouseDrag == FALSE 
		|| (this->selectWords->firstX == this->selectWords->endX && this->selectWords->firstY == this->selectWords->endY)) {
		//1.1. ���õ� �ܾ �ʱ�ȭ�Ѵ�.
		this->selectWords->firstX = 0;
		this->selectWords->firstY = 0;
		this->selectWords->endX = 0;
		this->selectWords->endY = 0;

		//1.2. Shift Ű���� ���� ������, �巡�� ���� false�� �����Ѵ�.
		this->selectWords->isDragging = FALSE;
		this->isShiftClicked = FALSE;
		this->isShifting = FALSE;

		this->ShiftX = 0;
		this->ShiftY = 0;

		//2.1. �÷��׸� �����Ѵ�.
		this->isMouseDrag = FALSE;

		//2.2. ĸ�ĸ� Ǯ���ش�.
		ReleaseCapture();

	}
	
	//2. �÷��װ� TRUE�̸�,
	else {
		//2.1. �÷��׸� �����Ѵ�.
		this->isMouseDrag = FALSE;
		this->selectWords->isDragging = TRUE;

		//2.2. ĸ�ĸ� Ǯ���ش�.
		ReleaseCapture();
	}

}

BOOL NotePadForm::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {

	int currentX;
	int currentY;
	int sizeCurrentX;
	int sizeCurrentY;

	//1. �ִ� caretHeight�� ���Ѵ�.
	int strSizeY = this->getTextSize->GetX(131);
	int length = this->note->GetLength();
	Long caretHeight = length * strSizeY;
	Long biggestNPos;

	//2. �ִ� caretHeight�� ���� �����캸�� ū ���,
	if (caretHeight > this->scroll->windowHeight) {

		//2.0. biggestNPos�� ���Ѵ�.
		biggestNPos = caretHeight - this->scroll->windowHeight;

		//2.1. ���� vertical�� nPos�� �Է¹��� ���콺���� �̵��Ÿ��� ���Ѵ�.
		this->scroll->siVertical.nPos = this->scroll->siVertical.nPos - zDelta;

		//2.2. nPos�� 0���� ������ 0�� �ִ´�.
		if (this->scroll->siVertical.nPos < 0) {
			this->scroll->siVertical.nPos = 0;
		}
		//2.3. nPos�� �ִ� caretHeight���� ū ��� �ִ� caretHeight�� �ִ´�.
		else if (this->scroll->siVertical.nPos > biggestNPos) { //caretHeight
				this->scroll->siVertical.nPos = biggestNPos;
		}

		//3. nPos�� set�Ѵ�.
		this->SetScrollInfo(SB_VERT, &this->scroll->siVertical, TRUE);

		this->ScrollWindow(0, this->scroll->siVertical.nPos, NULL, NULL);
		this->UpdateWindow();
		this->location->MoveCaret();
		Invalidate();
	}

	return FALSE;
}

void NotePadForm::OnRButtonDown(UINT nFlags, CPoint point) {

	//1. ���콺 ��Ŭ�� ���� ��
	//1.1. ���ؽ�Ʈ �޴��� �����մϴ�.
	CMenu menu;
	menu.CreatePopupMenu();

	//1.2. �޴��� �߰��մϴ�.
	menu.AppendMenu(MF_STRING, 40002, "���� ���(&U)"); //MF_GRAYED , 40002
	//HMENU unexecute = menu.GetSafeHmenu();
	//menu.Attach(unexecute);

	menu.AppendMenu(MF_SEPARATOR);
	menu.AppendMenu(MF_STRING, 40037, "�߶󳻱�(&T)");
	menu.AppendMenu(MF_STRING, 40035, "����(&C)");
	menu.AppendMenu(MF_STRING, 40036, "�ٿ��ֱ�(&P)");
	menu.AppendMenu(MF_STRING, 40038, "����(&D)");
	menu.AppendMenu(MF_SEPARATOR);
	menu.AppendMenu(MF_STRING, 40049, "��� ����(&A)");

	//1.3. ��Ȱ��ȭ ��ų �޴��� ��Ȱ��ȭ ��Ų��.
	//1.3.1. ������� �޴�����, Undo ����Ʈ�� �Ѱ��� ������ Ȱ��ȭ, ������ ��Ȱ��ȭ ��Ų��.
	if (this->commandStory->undoList->IsEmpty() == TRUE) {
		menu.EnableMenuItem(40002, MF_BYCOMMAND | MF_DISABLED | MF_ENABLED);
	}
	//1.3.2. �߶󳻱�, ����, ���� �޴�����, ���õ� �ܾ ������ Ȱ��ȭ, ������ ��Ȱ��ȭ ��Ų��.
	if (this->selectWords->firstX <= 0 && this->selectWords->firstY <= 0 && this->selectWords->endX <= 0 
		&& this->selectWords->endY <= 0) {
		menu.EnableMenuItem(40037, MF_BYCOMMAND | MF_DISABLED | MF_ENABLED);
		menu.EnableMenuItem(40035, MF_BYCOMMAND | MF_DISABLED | MF_ENABLED);
		menu.EnableMenuItem(40038, MF_BYCOMMAND | MF_DISABLED | MF_ENABLED);
	}
	//1.3.3. �ٿ��ֱ� �޴�����, Ŭ�����尡 �Ⱥ�������� Ȱ��, ��������� ��Ȱ��ȭ ��Ų��.
      //1.3.3.1. Ŭ������ �ڵ��� ����.
	  if (OpenClipboard()) {
		  //1.3.3.1.1. Ŭ�����忡 �ؽ�Ʈ �����Ͱ� ������
		  if (!IsClipboardFormatAvailable(CF_TEXT)) {
			  menu.EnableMenuItem(40036, MF_BYCOMMAND | MF_DISABLED | MF_ENABLED);
		  }
	  }
   //1.3.4. ��μ��� �޴�����, ��� ���õǾ� ������ ��Ȱ��, ���� �ȵǾ������� Ȱ��ȭ ��Ų��.
	  int endY = this->note->GetLength();
	  endY--;
	  Glyph* row = this->note->GetChild(endY);
	  int endX = row->GetLength();


	  if (this->selectWords->firstX == 0 && this->selectWords->firstY == 0 && this->selectWords->endX == endX &&
		  this->selectWords->endY == endY) {
		  menu.EnableMenuItem(40049, MF_BYCOMMAND | MF_DISABLED | MF_ENABLED);
	  }



#if 0
	CMenu* pMenu = GetMenu();
	CMenu* pSubMenu;

	pSubMenu = pMenu->GetSubMenu(0);
	//pSubMenu = menu.GetSubMenu(0);
	
	pSubMenu->EnableMenuItem(40002, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);    //��Ȱ��ȭ
#endif

	//1.4. ���ؽ�Ʈ �޴��� x,y ��ǥ�� ����մϴ�.
	POINT pt = { 0 };
	::GetCursorPos(&pt); // ���콺 Ŀ���� ��ǥ�� ���� ��

	//menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_BOTTOMALIGN, point.x, point.y, AfxGetMainWnd());
	menu.TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, AfxGetMainWnd());

	Invalidate();
}


void NotePadForm::OnClose() {

	CString str;
	BOOL ret;
	int ret_;
	CString title;
	CString title_;
	CString strPathName;
	CString fileName;
	CString fileNameWithouttxt;
	Glyph* row = NULL;
	BOOL isClose = TRUE;

	//3. �ݱ� ��ư Ŭ������ ��
	 //3.1. ���� �޸����� Ÿ��Ʋ�� ���Ѵ�.
	GetWindowText(title);

	// title�� *�� ���ԵǾ������� �����Ѵ�.
	if (title.Find("*") == 0) {
		title.Replace("*", "");
	}

	CString titleWithTxt;
	titleWithTxt.Format("%s%s", title, ".txt");


	//3.2. �ش� ������ �����ϰ�, ���� ��ȭ�� ������,
	CFileFind pFind;
	if (this->note != NULL) {
		row = this->note->GetChild(0);
	}

	ret = pFind.FindFile(titleWithTxt);
	if (ret == TRUE && this->isChanged == TRUE) {
		//3.2.1. �޼����� ����.
		str.Format("%s%s%s", "���泻���� ", title, "�� �����Ͻðڽ��ϱ�?");
		ret_ = SaveMessageBox(this->GetSafeHwnd(), str, "�޸���", MB_YESNOCANCEL);
		//ret_ = MessageBox(str, "�޸���", MB_YESNOCANCEL);


		//3.2.2. YES�� ���� ���, �����Ѵ�.
		if (ret_ == IDYES) {
			this->note->Save((LPCTSTR)titleWithTxt);
			//isClose = TRUE;
		}
		else if (ret_ == IDNO) {
			//isClose = TRUE;
		}
		else {
			isClose = FALSE;
		}
	}

	//3.3. �ش� ������ �������� �ʰ� ���ڰ� �ϳ��� ������,
	else if (ret == FALSE && (row->GetLength() > 0 || this->note->GetLength() > 1)) {

		//3.3.1. �޼����� ����.
		str.Format("%s%s%s", "���泻���� ", title, "�� �����Ͻðڽ��ϱ�?");
		//ret_ = MessageBox(str, "�޸���", MB_YESNOCANCEL);//MessageBox(str, "�޸���", MB_YESNOCANCEL);
		ret_ = SaveMessageBox(this->GetSafeHwnd(), str, "�޸���", MB_YESNOCANCEL); //MessageBox(str, "�޸���", MB_YESNOCANCEL);

		//3.3.2. YES�� ���� ���,
		if (ret_ == IDYES) {
			//2.1. ���͸� �����.
			LPCTSTR fileFilter(_T("�������|*.*|�ؽ�Ʈ����(*.txt)|*.txt||"));

			//2.2. �����ȭ����(�ٸ��̸����� ����)�� �����.
			CFileDialog dlg(FALSE, _T("txt"), _T("*.txt"), OFN_HIDEREADONLY, fileFilter, NULL, 0, NULL);
			//OFN_HIDEREADONLY �� ���� ���� ������ ���

			//2.3. �����ȭ����(�ٸ��̸����� ����)�� ����.
			//2.4. ����ڰ� ������ ������ ���������� �����Ͽ�����, 
			if (IDOK == dlg.DoModal()) {
				//2.4.1. strPathName�� ������ ������ ��� �����Ѵ�.
				strPathName = dlg.GetPathName();

				//2.4.2. ������ ������ �̸��� ���� �´�.
				fileName = dlg.GetFileName();//���õ� ������ �̸��� Ȯ����
				fileNameWithouttxt = dlg.GetFileTitle();

				//MessageBox(title);

				//2.4.3. ���� �̸��� �ߺ��Ǹ� 
				if (PathFileExists(fileName)) {
					str.Format("%s%s%s", fileName, "��(��) �̹� �ֽ��ϴ�.\n", "�ٲٽðڽ��ϱ�?");
					ret = MessageBox(str, _T("�ٸ� �̸����� ���� Ȯ��"), MB_YESNO);

					if (ret == IDYES) {// Yes ������ ���, �ش� ���Ͽ� ������Ѵ�.
						//������ �ִ� �ش� �̸��� ������ �����Ѵ�.
						//remove(fileName);

						this->note->Save((LPCTSTR)fileName);

						//�Է��� �̸����� �����Ѵ�
						SetWindowText(fileNameWithouttxt);
					}
				}
				//2.4.4. ���� �̸��� �ߺ����� ������,
				else {
					this->note->Save((LPCTSTR)fileName);
					//�Է��� �̸����� �����Ѵ�.
					SetWindowText(fileNameWithouttxt);
				}
			}
			//2.5. ����ڰ� ���� ���� ���ߴٸ� strPathName �ʱ�ȭ�ϰ� ����X.
			else {
				strPathName = "";
			}
			//isClose = TRUE;
		}
		else if (ret_ == IDCANCEL) {
			isClose = FALSE;
		}

	}

	//(21.10.25.�߰�) ������Ʈ���� �ݱ� �� �������� ������ ��ġ�� �����Ų��.
	//���� ��ġ�� ���Ѵ�.
	CRect rect;
	GetWindowRect(rect);

	//���� ��ġ�� ������Ʈ���� �����Ѵ�.
	this->pApp->WriteProfileInt("NotePadSection", "WindowLeft", rect.left);
	this->pApp->WriteProfileInt("NotePadSection", "WindowTop", rect.top);

	//(21.10.25.�߰�) ������Ʈ���� �ݱ� �� �������� ������ ũ�⸦ �����Ų��.
	//���� ũ�⸦ ���Ѵ�.
	GetClientRect(rect);

	this->pApp->WriteProfileInt("NotePadSection", "WindowWidth", (rect.right - rect.left) + 43);
	this->pApp->WriteProfileInt("NotePadSection", "WindowHeight", (rect.bottom - rect.top) + 97);



	if (isClose == TRUE) {
		if (this->note != NULL) {
			delete this->note;
			this->note = NULL;
		}
		if (this->getTextSize != NULL) {
			delete this->getTextSize;
			this->getTextSize = NULL;

		}
		if (this->getTextSizeForDpi != NULL) {
			delete this->getTextSizeForDpi;
			this->getTextSizeForDpi = NULL;

		}
		if (this->font != NULL) {
			delete this->font;
			this->font = NULL;

		}
		if (this->thumb != NULL) {
			delete this->thumb;
			this->thumb = NULL;
		}
		if (this->scroll != NULL) {
			dynamic_cast<Subject*>(this)->Detach(this->scroll);
			delete this->scroll;
			this->scroll = NULL;
		}

		if (this->findDialog != NULL) {
			delete this->findDialog;
			this->findDialog = NULL;
		}
#if 0
		if (this->factoryProductScroll != NULL) {
			delete this->factoryProductScroll;
			this->factoryProductScroll = NULL;

	}
#endif
		if (this->commandStory != NULL) {
			delete this->commandStory;
			this->commandStory = NULL;
		}

		if (this->pageInfo != NULL) {
			delete this->pageInfo;
			this->pageInfo = NULL;
		}
		if (this->getInformation != NULL) {
			delete this->getInformation;
			this->getInformation = NULL;
		}
		if (this->windowText != NULL) {
			dynamic_cast<Subject*>(this)->Detach(this->windowText);
			delete this->windowText;
			this->windowText = NULL;
		}
		if (this->visitor != NULL) {
			delete this->visitor;
			this->visitor = NULL;
		}


		CFrameWnd::OnClose();
	}

#if 0
	CString str;
	BOOL ret;
	int ret_;
	CString title;
	CString title_;
	CString strPathName;
	CString fileName;
	CString fileNameWithouttxt;

	//3. �����ư Ŭ������ ��
	 //3.1. ���� �޸����� Ÿ��Ʋ�� ���Ѵ�.
	GetWindowText(title);
	CString titleWithTxt;
	titleWithTxt.Format("%s%s", title, ".txt");


	//3.2. �ش� ������ �����ϰ�, ���� ��ȭ�� ������,
	CFileFind pFind;
	Glyph* row = this->note->GetChild(0);


	ret = pFind.FindFile(titleWithTxt);
	if (ret == TRUE && this->isChanged == TRUE) {
		//3.2.1. �޼����� ����.
		str.Format("%s%s%s", "���泻���� ", title, "�� �����Ͻðڽ��ϱ�?");
		ret_ = SaveMessageBox(this->GetSafeHwnd(), str, "�޸���", MB_YESNOCANCEL);
		//ret_ = MessageBox(str, "�޸���", MB_YESNOCANCEL);


		//3.2.2. YES�� ���� ���, �����Ѵ�.
		if (ret_ == IDYES) {
			this->note->Save((LPCTSTR)titleWithTxt);
		}
	}

	//3.3. �ش� ������ �������� �ʰ� ���ڰ� �ϳ��� ������,
	else if (ret == FALSE && (row->GetLength() > 0 || this->note->GetLength() > 1)) {
		//3.3.1. �޼����� ����.
		str.Format("%s%s%s", "���泻���� ", title, "�� �����Ͻðڽ��ϱ�?");
		//ret_ = MessageBox(str, "�޸���", MB_YESNOCANCEL);//MessageBox(str, "�޸���", MB_YESNOCANCEL);
		ret_ = SaveMessageBox(this->GetSafeHwnd(), str, "�޸���", MB_YESNOCANCEL); //MessageBox(str, "�޸���", MB_YESNOCANCEL);

		//3.3.2. YES�� ���� ���,
		if (ret_ == IDYES) {
			//2.1. ���͸� �����.
			LPCTSTR fileFilter(_T("�������|*.*|�ؽ�Ʈ����(*.txt)|*.txt||"));

			//2.2. �����ȭ����(�ٸ��̸����� ����)�� �����.
			CFileDialog dlg(FALSE, _T("txt"), _T("*.txt"), OFN_HIDEREADONLY, fileFilter, NULL, 0, NULL);
			//OFN_HIDEREADONLY �� ���� ���� ������ ���

			//2.3. �����ȭ����(�ٸ��̸����� ����)�� ����.
			//2.4. ����ڰ� ������ ������ ���������� �����Ͽ�����, 
			if (IDOK == dlg.DoModal()) {
				//2.4.1. strPathName�� ������ ������ ��� �����Ѵ�.
				strPathName = dlg.GetPathName();

				//2.4.2. ������ ������ �̸��� ���� �´�.
				fileName = dlg.GetFileName();//���õ� ������ �̸��� Ȯ����
				fileNameWithouttxt = dlg.GetFileTitle();

				//MessageBox(title);

				//2.4.3. ���� �̸��� �ߺ��Ǹ� 
				if (PathFileExists(fileName)) {
					str.Format("%s%s%s", fileName, "��(��) �̹� �ֽ��ϴ�.\n", "�ٲٽðڽ��ϱ�?");
					ret = MessageBox(str, _T("�ٸ� �̸����� ���� Ȯ��"), MB_YESNO);

					if (ret == IDYES) {// Yes ������ ���, �ش� ���Ͽ� ������Ѵ�.
						//������ �ִ� �ش� �̸��� ������ �����Ѵ�.
						//remove(fileName);

						this->note->Save((LPCTSTR)fileName);

						//�Է��� �̸����� �����Ѵ�
						SetWindowText(fileNameWithouttxt);
					}
				}
				//2.4.4. ���� �̸��� �ߺ����� ������,
				else {
					this->note->Save((LPCTSTR)fileName);
					//�Է��� �̸����� �����Ѵ�.
					SetWindowText(fileNameWithouttxt);
				}
			}
			//2.5. ����ڰ� ���� ���� ���ߴٸ� strPathName �ʱ�ȭ�ϰ� ����X.
			else {
				strPathName = "";
			}
		}
	}


	if (this->note != NULL) {
		delete this->note;
		this->note = NULL;
	}
	if (this->getTextSize != NULL) {
		delete this->getTextSize;
		this->getTextSize = NULL;

	}
	if (this->font != NULL) {
		delete this->font;
		this->font = NULL;

	}
	if (this->page != NULL) {
		delete this->page;
		this->page = NULL;
	}
	if (this->thumb != NULL) {
		delete this->thumb;
		this->thumb = NULL;
	}
	if (this->scroll != NULL) {
		dynamic_cast<Subject*>(this)->Detach(this->scroll);
		delete this->scroll;
		this->scroll = NULL;
	}

	if (this->findDialog != NULL) {
		delete this->findDialog;
		this->findDialog = NULL;
	}
#if 0
	if (this->factoryProductScroll != NULL) {
		delete this->factoryProductScroll;
		this->factoryProductScroll = NULL;

	}
#endif
	if (this->commandStory != NULL) {
		delete this->commandStory;
		this->commandStory = NULL;
}

	CFrameWnd::OnClose();
#endif
}

LRESULT CALLBACK SaveMessageBoxProc(int nCode, WPARAM wParam, LPARAM lParam) {
	HWND hChildWnd;

	CString msg;

	if (nCode == HCBT_ACTIVATE) {
		hChildWnd = (HWND)wParam;
		if (GetDlgItem(hChildWnd, IDYES) != NULL) {
			msg = "����(&S)";
			SetWindowText(GetDlgItem(hChildWnd, IDYES), msg);
		}

		if (GetDlgItem(hChildWnd, IDNO) != NULL) {
			msg = "���� �� ��(&N)";
			SetWindowText(GetDlgItem(hChildWnd, IDNO), msg);
		}
		UnhookWindowsHookEx(hSaveMessageBoxHook);
	}

	return 0;
}

int SaveMessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT nType) {
	hSaveMessageBoxHook = SetWindowsHookEx(WH_CBT, &SaveMessageBoxProc, 0, GetCurrentThreadId());

	return MessageBox(hWnd, lpText, lpCaption, nType);
}
















