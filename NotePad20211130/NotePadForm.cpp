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
//(21.09.01 추가) Command Pattern 
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

HHOOK hSaveMessageBoxHook; // 메세지박스 후킹용 전역변수 선언.
//HWND hwnd = HtmlHelpA(GetDesktopWindow(), "HelpDocument.chm", HH_DISPLAY_TOPIC, NULL); //도움말 띄우기


BEGIN_MESSAGE_MAP(NotePadForm, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_CHAR()

	//(21.09.01) 추가
	ON_COMMAND_RANGE(40001, 40053, OnMyCommand) //40038
	ON_WM_ERASEBKGND() //화면 깜빡임(flickering) 방지 메세지
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


//(21.09.01) 추가
void NotePadForm::OnMyCommand(UINT id) {
	FactoryProductCommand factoryProductCommand(this);
	Command* command = NULL;
	string character;

	// 해당 command를 생성한다.
	command = factoryProductCommand.Make(id);

	//해당 command를 실행한다.
	command->Execute();

	//(21.10.29.추가) BackSpace시 character에 무엇이 있는 경우에만 history list에 추가한다.
	if (id == 40013) {
		character = command->GetCharacterForAll();
		if (character != "") {

			this->commandStory->Pile(command);
		}	
	}

	//해당 command를 undolist에 쌓는다.
	//실행취소 또는 실행복구가 아니면,
	else if (id != 40002 && id!=40003 && id!=40006 && id!=40007 && id!=40008 && id!=40009 && id!=40010 && id!=40011 && id!=40012
		&& id!=40014 && id!=40015 && id!=40016 && id!=40017 && id!=40018 && id!=40019 && id!=40020 && id!=40021 && id!=40022 &&
		id!=40023 && id!=40024 && id!=40025 && id!=40026 && id!=40027 && id!=40028 && id!=40029 && id!=40030 && id!=40031 &&
		id!=40032 && id!=40033 && id!=40034 && id!=40035 && id!=40039 && id != 40040 && id!=40041 &&
		id!=40043 && id!=40046 && id!=40047 && id!=40048 && id!=40049 && id!=40051 && id!=40052 && id!=40053 &&
		isUndo==FALSE) {
		//실행한 command를 history list에 추가한다.
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



	//2. Subject 멤버 추가해준다.
	
	dynamic_cast<Subject*>(this)->Attach(this->scroll);

	this->location = new Location(this);
	dynamic_cast<Subject*>(this)->Attach(this->location); //Attach의 매개변수는 Observer* observe이지만

	this->windowText = new WindowText(this);
	dynamic_cast<Subject*>(this)->Attach(this->windowText);


	// 폰트 클래스를 만든다. 
	this->font = new Font(this);
	this->selectWords = new SelectWords(this);
	

	//(21.10.25.추가) 기존에 저장해 놨던 폰트 이름, 사이즈를 레지스터에서 가져온다.
//#if 0
	this->pApp = AfxGetApp();
	this->font->fontName = this->pApp->GetProfileString("NotePadSection", "FontName", "맑은 고딕");
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
	this->font->fontName = "맑은 고딕";//segoe script
#endif

	this->getTextSize = new GetTextSize(this);

	//21.11.30. 추가
	//this->getTextSizeForDpi = new GetTextSizeForDpi(this);

	//21.08.08. 추가(자동개행 객체 생성)
	this->lineChange = new LineChange(this);

	//21.08.20. 추가(복사 & 붙혀넣기)
	this->copyText = new CopyText(this);
	this->pasteText = new PasteText(this);
	
	//21.08.25. 추가(찾기 & 바꾸기)
	this->findSameText = new FindSameText(this);

	//21.09.02 추가(Stack의 history)
	this->commandStory = new CommandStory(this);

	//21.09.28 추가 (페이지인포)
	this->pageInfo = new PageInfo(this);

	//21.10.05 추가
	this->getInformation = new GetInformation(this);

	//21.07.28. 추가(스크롤 객체 생성)
	this->thumb = new Thumb(this);

	//21.11.24.추가
	this->visitor = new Visitor(this);

	//21.10.17. 추가 (마우스 오른쪽 버튼 메뉴 비활성화 위해)
	this->m_bAutoMenuEnable = FALSE;

	this->note = new Note();
	this->row = new Row();
	this->note->Add(this->row);


	//21.07.13.)1. 저장 메뉴 추가할 때
	//2.0 제목없음으로 캡션 단다.
	//SetWindowText("제목없음"); //"제목없음 - Windows 메모장"??


	//3.7.6. 해당 파일을 Load한다. 이동하다 위해 키자마자 뜨도록***********************
	this->note->Load("테스트용.txt");
	//3.7.7. 해당 파일의 캡션을 바꾼다.
	SetWindowText("테스트용");

    //current를 맨 처음으로(0,) 이동한다.
	this->note->MoveCurrent(0);
	Glyph* row = note->GetChild(0);
	row->MoveCurrent(0);
	//*********************************************************************************


//#if 0
	//(21.10.25 추가) 윈도우 크기, 위치에 대하여 레지스트리 처리한다. **********
	//기존에 저장해 놨던 윈도우의 크기 및 위치를 가져온다.
	Long windowLeft = this->pApp->GetProfileInt("NotePadSection", "WindowLeft", 513);
	Long windowTop = this->pApp->GetProfileInt("NotePadSection", "WindowTop", 272);
	Long windowWidth_ = this->pApp->GetProfileInt("NotePadSection", "WindowWidth", 873);
	Long windowHeight_ = this->pApp->GetProfileInt("NotePadSection", "WindowHeight", 394);

	//해당 위치와 크기만큼 설정해준다.
	MoveWindow(windowLeft, windowTop, windowWidth_, windowHeight_);
	this->scroll->windowWidth = windowWidth_;
	this->scroll->windowHeight = windowHeight_;

	//***************************************************************************
//#endif


	
#if 0
	//1.8. (21.10.25.추가) 레지스트리에서 자동개행 여부 확인한다.************************************
	int isLineChanged_ = this->pApp->GetProfileInt("NotePadSection", "IsLineChanged", 0);

	//1.9. 레지스트리 값에 따라 자동개행 여부 설정해준다.
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


	//3.7.8. 글자 변화를 초기화한다.
	this->isChanged = FALSE;

	//1.1. 메인 메뉴를 생성한다.
	CMenu menuMain;
	menuMain.CreateMenu();

	//1.2. 팝업메뉴(파일)를 생성한다.
	CMenu menuFile;
	menuFile.CreatePopupMenu();

	// 팝업메뉴에 새로만들기를 추가한다.
	menuFile.AppendMenu(MF_STRING, 40048, "새로 만들기(&N)                      Ctrl+N");

	//1.3. 팝업메뉴에 저장을 추가한다.
	menuFile.AppendMenu(MF_STRING, 40006,	"저장(&S)                                 Ctrl+S");
	
	// 팝업 메뉴에 다른 이름으로 저장 추가한다.
	menuFile.AppendMenu(MF_STRING, 40007 , "다른 이름으로 저장(&A)            Ctrl+Shift+S");

	// 팝업 메뉴에 열기 추가한다.
	menuFile.AppendMenu(MF_STRING, 40008, "열기(&O)                                Ctrl+O");

	menuFile.AppendMenu(MF_SEPARATOR);

	// 팝업 메뉴에 페이지 설정 추가한다.
	menuFile.AppendMenu(MF_STRING, 40046, "페이지 설정(&U)");

	// 팝업 메뉴에 인쇄 추가한다.
	menuFile.AppendMenu(MF_STRING, 40047, "인쇄(&P)                                 Ctrl+P");

	// 팝업 메뉴에 미리보기 추가한다.
	menuFile.AppendMenu(MF_STRING, 40053, "미리보기");

	menuFile.AppendMenu(MF_SEPARATOR);

	// 팝업 메뉴에 끝내기 추가한다.
	menuFile.AppendMenu(MF_STRING, 40009, "끝내기(&X)");

	//1.4. 메인메뉴에 팝업(파일)메뉴를 추가한다.
	menuMain.AppendMenu(MF_POPUP, (UINT_PTR)menuFile.Detach(), "파일(&F)");


	//(21.08.24 추가) 1.1.) 찾다: 팝업메뉴(편집)를 생성한다.
	CMenu meunEdit;
	meunEdit.CreatePopupMenu();

	//1.1. 팝업메뉴(편집)에 실행취소를 추가한다.
	meunEdit.AppendMenu(MF_STRING, 40002, "실행 취소(&U)                  Ctrl+Z");

	meunEdit.AppendMenu(MF_STRING, 40003, "다시 실행(&Y)                  Ctrl+Y");

	meunEdit.AppendMenu(MF_SEPARATOR);

	meunEdit.AppendMenu(MF_STRING, 40037, "잘라내기(&T)                   Ctrl+X");

	meunEdit.AppendMenu(MF_STRING, 40035, "복사(&C)                         Ctrl+C");

	meunEdit.AppendMenu(MF_STRING, 40036, "붙여넣기(&P)                   Ctrl+V");

	//팝업메뉴(편집)에 삭제를 추가한다.
	meunEdit.AppendMenu(MF_STRING, 40013, "삭제(&L)                             Del"); //선택된것이 없으면 클릭못하도록 바꾸기

	meunEdit.AppendMenu(MF_SEPARATOR);

	//1.2. 팝업메뉴(편집)에 찾기를 추가한다.
	meunEdit.AppendMenu(MF_STRING, 40039, "찾기(&F)                          Ctrl+F");

	//1.3. 팝업메뉴(편집)에 다음찾기를 추가한다.
	meunEdit.AppendMenu(MF_STRING, 40040, "다음 찾기(&N)                       F3");

	//1.4. 팝업메뉴(편집)에 이전찾기를 추가한다.
	meunEdit.AppendMenu(MF_STRING, 40041, "이전 찾기(&V)               Shift+F3");

	//1.5. 팝업메뉴(편집)에 바꾸기를 추가한다
	meunEdit.AppendMenu(MF_STRING, 40043, "바꾸기(&R)                      Ctrl+H");

	meunEdit.AppendMenu(MF_SEPARATOR);

	//(21.09.30) 팝업메뉴(편집)에 모두 선택을 추가한다.
	meunEdit.AppendMenu(MF_STRING, 40049, "모두 선택(&A)                  Ctrl+A");

	//(21.09.30) 팝업메뉴(편집)에 모두 선택을 추가한다.
	meunEdit.AppendMenu(MF_STRING, 40050, "시간/날짜(&D)                       F5");

	//1.6.  메인메뉴에 팝업(편집)메뉴를 추가한다.
	menuMain.AppendMenu(MF_POPUP, (UINT_PTR)meunEdit.Detach(), "편집(&E)");


	//(21.07.21.추가) 1.1.)글꼴을 정하다: 팝업메뉴(서식)를 생성한다.
	CMenu menuForm;
	menuForm.CreatePopupMenu();

	//1.2. 팝업메뉴(서식)에 자동개행을 추가한다. 
	menuForm.AppendMenu(MF_STRING, 40014, "자동 줄 바꿈(&W)");

	//1.3. 팝업메뉴(서식)에 글꼴을 추가한다. 
	menuForm.AppendMenu(MF_STRING, 40010, "글꼴(&F)");

	//this->menuForm = menuMain;
	//CMenu* mmenu = GetMenu();
	//this->menuForm = mmenu->GetSubMenu(4);
	
	//1.4. 메인메뉴에 팝업(서식)메뉴를 추가한다.
	menuMain.AppendMenu(MF_POPUP, (UINT_PTR)menuForm.Detach(), "서식(&O)");

	//(21.10.05 추가) 팝업메뉴(도움말)를 생성한다.
	CMenu help;
	help.CreatePopupMenu();

	//1.1. 팝업메뉴(도움말)에 도움말 보기를 추가한다.
	help.AppendMenu(MF_STRING, 40051, "도움말 보기(&H)");

	help.AppendMenu(MF_SEPARATOR);

	//1.2. 팝업메뉴(도움말)에 메모장 정보를 추가한다.
	help.AppendMenu(MF_STRING, 40052, "메모장 정보(&A)");

	//1.3. 메인메뉴에 팝업(도움말)메뉴를 추가한다.
	menuMain.AppendMenu(MF_POPUP, (UINT_PTR)help.Detach(), "도움말(&H)");

	//1.5. 메인메뉴를 윈도우에 추가한다.
	SetMenu(&menuMain);

	//1.6. 메뉴객체와 메뉴로 분리한다.
	//menuMain변수가 범위를 벗어나 제거되도 메뉴는 남아 있게된다. Drtach() 호출 없이 실행하면 에러 난다.
	menuMain.Detach();

	//1.7. 스크롤 막대의 최소 및 최대값을 설정한다.

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
	Glyph* character1 = new DoubleByteCharacter("안");
	row1->Add(character1);
	Glyph* character2 = new DoubleByteCharacter("녕");
	row1->Add(character2);
	Glyph* character3 = new DoubleByteCharacter("하");
	row1->Add(character3);
	Glyph* character4 = new DoubleByteCharacter("세");
	row1->Add(character4);
	Glyph* character5 = new DoubleByteCharacter("요");
	row1->Add(character5);
	Glyph* character6 = new SingleByteCharacter('.');
	row1->Add(character6);
	this->note->Add(row1);

	Glyph* row2 = new Row();
	Glyph* character7 = new DoubleByteCharacter("반");
	row2->Add(character7);
	Glyph* character8 = new DoubleByteCharacter("갑");
	row2->Add(character8);
	Glyph* character9 = new DoubleByteCharacter("습");
	row2->Add(character9);
	Glyph* character10 = new DoubleByteCharacter("니");
	row2->Add(character10);
	Glyph* character11 = new DoubleByteCharacter("다");
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
	//(21.10.25 추가) 윈도우 크기, 위치에 대하여 레지스트리 처리한다. **********
	//기존에 저장해 놨던 윈도우의 크기 및 위치를 가져온다.
	Long windowLeft = this->pApp->GetProfileInt("NotePadSection", "WindowLeft", 513);
	Long windowTop = this->pApp->GetProfileInt("NotePadSection", "WindowTop", 272);
	Long windowWidth_ = this->pApp->GetProfileInt("NotePadSection", "WindowWidth", 873);
	Long windowHeight_ = this->pApp->GetProfileInt("NotePadSection", "WindowHeight", 394);

	//해당 위치와 크기만큼 설정해준다.
	MoveWindow(windowLeft, windowTop, windowWidth_, windowHeight_);
	this->scroll->windowWidth = windowWidth_;
	this->scroll->windowHeight = windowHeight_;
	
	//***************************************************************************
#endif
	//MoveWindow(513, 272, 873, 394);


//#if 0
	//1.8. (21.10.25.추가) 레지스트리에서 자동개행 여부 확인한다.************************************
	int isLineChanged_ = this->pApp->GetProfileInt("NotePadSection", "IsLineChanged", 0);

	//1.9. 레지스트리 값에 따라 자동개행 여부 설정해준다.
	CMenu* pMenu = AfxGetMainWnd()->GetMenu(); //AfxGetMainWnd()->GetMenu();

	if (isLineChanged_ == 1) {
		pMenu->CheckMenuItem(40014, MF_CHECKED);
		this->isLineChangeButtonClicked = TRUE;
		// 자동개행 시킨다.
		//this->lineChange->LineChangeButtonClicked();
	}

	else {
		pMenu->CheckMenuItem(40014, MF_UNCHECKED);
		this->isLineChangeButtonClicked = FALSE;
		// 자동개행 취소한다.
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

	//(21.11.02.추가)
		//3.1. isBigger이 true인지 확인한다. 
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

	//3.2. isBigger이 아니면 좌표를 서로 바꾼다.
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


	//윈도우 사이즈 바뀔때마다 갱신해주고, nPage 값을 설정한다.
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

	//0. (21.09.10 추가) 윈도우 사이즈 바뀔때마다 갱신해준다.
	//this->scroll->beforeWindowWidth = this->scroll->windowWidth;
	this->GetClientRect(this->m_rectCurHist);
	this->scroll->windowWidth = this->m_rectCurHist.right - this->m_rectCurHist.left;
	this->scroll->windowHeight = this->m_rectCurHist.bottom - this->m_rectCurHist.top;

	//0.1. (21.09.14 추가) 바뀐 사이즈를 갱신해준다.
	this->scroll->siHorizon.nPage = this->scroll->windowWidth;
	this->SetScrollInfo(SB_HORZ, &this->scroll->siHorizon, TRUE);

	this->scroll->siHorizon.nPage = this->scroll->windowHeight;
	this->SetScrollInfo(SB_VERT, &this->scroll->siVertical, TRUE);


	row = this->note->GetChild(0);
	row->GetContent();


	//1. OnSize 호출할 때(자동개행 체크 되어있는 경우)
	if (this->isLineChangeButtonClicked == TRUE) {

		//(21.08.23 추가 1) 자동개행 전 캐럿의 위치를 찾는다.

#if 0
		this->lineChange->FindCaretBeforeLineChange(&beforeLineChangeCurrentX, &currentY);
		this->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
		this->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);
#endif

		this->lineChange->FindCaretBeforeLineChange(&beforeLineChangeCurrentX, &currentY);
		this->lineChange->FindCaretBeforeLineChangeForFirstXY(&firstX, &firstY);
		this->lineChange->FindCaretBeforeLineChangeForEndXY(&endX, &endY);



		//1.1. 기존 windowWidth에서 적어진 경우,
		//SmallerThanBefore 연산이 KillFocus일어나면 무한 반복 돈다.
		if (this->isKillFocus == FALSE && this->scroll->beforeWindowWidth > this->scroll->windowWidth) { //cx
			
            this->lineChange->SmallerThanBefore();

			//자동개행된 문서를 펼쳐준다.
			//this->lineChange->LineChangeButtonNotClicked();

			//펼쳐진 문서를 현재 윈도우 너비만큼 자동개행한다.
			//this->lineChange->LineChangeButtonClicked(this->scroll->windowWidth);
		}

		//1.2. 기존 windowWidth에서 커진 경우,
		else if (this->scroll->beforeWindowWidth < this->scroll->windowWidth) { //cx
			this->lineChange->BiggerThanBefore();
		}

		//2.2. (21.08.23 추가 2)자동개행 후 이동할 캐럿의 위치를 구한다.

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


	//1. 현재 FALSE인데 자동 개행 버튼 눌렀을 경우 TRUE로 바꿔준다.
	if (this->isLineChangeButtonClicked == FALSE) {
		this->isLineChangeButtonClicked = TRUE;	
	}
	else if(this->isLineChangeButtonClicked == TRUE){
		this->isLineChangeButtonClicked = FALSE;
	}


	//2. 자동개행 버튼 눌렀을 때(ON)
	if (this->isLineChangeButtonClicked == TRUE) {
		this->lineChange->LineChangeButtonClicked();	
	}

	//3. 자동개행 버튼 취소했을 때(OFF)
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

	 //2. 저장 버튼을 클릭했을 때
	 //1.1.현재 메모장의 이름을 확인한다. 
	 GetWindowText(title);
	 CString title_;
	 title_.Format("%s%s", title, ".txt");

	 CFileFind pFind;
	 BOOL ret_ = pFind.FindFile(title_);


	 //1.2. 해당 이름의 파일을 열어보고, 있으면 갱신만 한다.(공통대화상자x)
	 if (ret_ == TRUE) {
		 this->note->Save((LPCTSTR)title_);
		 this->isChanged = FALSE;
	 }

	//2. 현재 파일의 제목이 없으면,
	else {


		//2.1. 필터를 만든다.
		LPCTSTR fileFilter(_T("모든파일|*.*|텍스트문서(*.txt)|*.txt||"));

		//2.2. 공통대화상자(다른이름으로 저장)를 만든다.
		CFileDialog dlg(FALSE, _T("txt"), _T("*.txt"), OFN_HIDEREADONLY, fileFilter, NULL, 0, NULL);
		//OFN_HIDEREADONLY 한 개의 파일 선택할 경우

		//2.3. 공통대화상자(다른이름으로 저장)를 띄운다.
		//2.4. 사용자가 저장할 파일을 정상적으로 선택하였을때, 
		if (IDOK == dlg.DoModal()) {


			//2.4.1. strPathName에 지정할 파일의 경로 저장한다.
			strPathName = dlg.GetPathName();

			//2.4.2. 선택한 파일의 이름을 갖고 온다.
			fileName = dlg.GetFileName();//선택된 파일의 이름과 확장자
			fileNameWithoutTxt = dlg.GetFileTitle();

			fileName_ = (LPCTSTR)fileName;
			//MessageBox(title);

			//2.4.3. 파일 이름이 중복되면 
			if (PathFileExists(fileName)) {
				str.Format("%s%s%s", fileName, "이(가) 이미 있습니다.\n", "바꾸시겠습니까?");
				ret = MessageBox(str, _T("다른 이름으로 저장 확인"), MB_YESNO);

				if (ret == IDYES) {// Yes 눌렀을 경우, 해당 파일에 덮어쓰기한다.
					//기존에 있던 해당 이름의 파일을 삭제한다.
					//remove(fileName);

					this->note->Save(fileName_);
					//입력한 이름으로 변경한다.
					SetWindowText(fileNameWithoutTxt);
					//rename("NotePad.txt", fileName);
				}
			}
			//2.4.4. 파일 이름이 중복되지 않으면,
			else {
				this->note->Save(fileName_);
				//입력한 이름으로 변경한다.
				SetWindowText(fileNameWithoutTxt);
				//rename("NotePad.txt", fileName); //fileName
			}

			this->isChanged = FALSE;


		}
		//2.5. 사용자가 파일 선택 안했다면 strPathName 초기화하고 저장X.
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

	//3. 열기버튼 클릭했을 때
	 //3.1. 현재 메모장의 타이틀을 구한다.
     GetWindowText(title);
	 CString titleWithTxt;
	 titleWithTxt.Format("%s%s", title, ".txt");


	 //3.2. 해당 파일이 존재하고, 내용 변화가 있으면,
	 CFileFind pFind;
	 Glyph* row = this->note->GetChild(0);


	 ret = pFind.FindFile(titleWithTxt);
	 if (ret == TRUE && this->isChanged == TRUE) {
		 //3.2.1. 메세지를 띄운다.
		 str.Format("%s%s%s", "변경내용을 ", title, "에 저장하시겠습니까?");
		 ret_ = SaveMessageBox(this->GetSafeHwnd(), str, "메모장", MB_YESNOCANCEL);
		 //ret_ = MessageBox(str, "메모장", MB_YESNOCANCEL);

		 //3.2.2. YES를 누를 경우, 저장한다.
		 if (ret_ == IDYES) {
			 this->note->Save((LPCTSTR)titleWithTxt);
		 }
	 }

	 //3.3. 해당 파일이 존재하지 않고 글자가 하나라도 있으면,
	 else if(ret == FALSE && (row->GetLength() > 0 || this->note->GetLength() >1)) {
		 //3.3.1. 메세지를 띄운다.
		 str.Format("%s%s%s", "변경내용을 ", title, "에 저장하시겠습니까?");
		 ret_ = SaveMessageBox(this->GetSafeHwnd(), str, "메모장", MB_YESNOCANCEL);

		 //3.3.2. YES를 누를 경우,
		 if (ret_ == IDYES) {
			 //2.1. 필터를 만든다.
			 LPCTSTR fileFilter(_T("모든파일|*.*|텍스트문서(*.txt)|*.txt||"));

			 //2.2. 공통대화상자(다른이름으로 저장)를 만든다.
			 CFileDialog dlg(FALSE, _T("txt"), _T("*.txt"), OFN_HIDEREADONLY, fileFilter, NULL, 0, NULL);
			 //OFN_HIDEREADONLY 한 개의 파일 선택할 경우

			 //2.3. 공통대화상자(다른이름으로 저장)를 띄운다.
			 //2.4. 사용자가 저장할 파일을 정상적으로 선택하였을때, 
			 if (IDOK == dlg.DoModal()) {
				 //2.4.1. strPathName에 지정할 파일의 경로 저장한다.
				 strPathName = dlg.GetPathName();

				 //2.4.2. 선택한 파일의 이름을 갖고 온다.
				 fileName = dlg.GetFileName();//선택된 파일의 이름과 확장자
				 fileNameWithouttxt = dlg.GetFileTitle();

				 //MessageBox(title);

				 //2.4.3. 파일 이름이 중복되면 
				 if (PathFileExists(fileName)) {
					 str.Format("%s%s%s", fileName, "이(가) 이미 있습니다.\n", "바꾸시겠습니까?");
					 ret = MessageBox(str, _T("다른 이름으로 저장 확인"), MB_YESNO);

					 if (ret == IDYES) {// Yes 눌렀을 경우, 해당 파일에 덮어쓰기한다.
						 //기존에 있던 해당 이름의 파일을 삭제한다.
						 //remove(fileName);

						 this->note->Save((LPCTSTR)fileName);

						 //입력한 이름으로 변경한다
						 SetWindowText(fileNameWithouttxt);
					 }
				 }
				 //2.4.4. 파일 이름이 중복되지 않으면,
				 else {
					 this->note->Save((LPCTSTR)fileName);
					 //입력한 이름으로 변경한다.
					 SetWindowText(fileNameWithouttxt);
				 }
			 }
			 //2.5. 사용자가 파일 선택 안했다면 strPathName 초기화하고 저장X.
			 else {
				 strPathName = "";
			 }
		 }
	 }


	 //3.4. 필터를 만든다.
	 LPCTSTR fileFilter_(_T("모든파일|*.*|텍스트문서(*.txt)|*.txt||"));

	 //3.5. 공통대화상자(열기) 만든다.
	 CFileDialog dlg_(TRUE, _T("txt"), _T("*.txt"), OFN_HIDEREADONLY, fileFilter_, NULL, 0, NULL);

	 //3.6. 공통대화상자 띄운다.
	 //3.7. 사용자가 열 파일을 정상적으로 선택 하였을 때
	 if (IDOK == dlg_.DoModal()) {
		 //3.7.1. 선택한 파일의 이름을 갖고 온다.
		 fileName = dlg_.GetFileName();
		 fileNameWithouttxt = dlg_.GetFileTitle();

		 //3.7.2. 현재 note를 삭제한다.
		 if (this->note != NULL) {
			 delete this->note;
		 }
		 //3.7.3. 새로운 노트를 만든다.
		 this->note = new Note();
		 //3.7.4. 새로운 줄을 만든다.
		 this->row = new Row();
		 //3.7.5. 노트에 줄을 Add한다.
		 this->note->Add(this->row);
		 //3.7.6. 해당 파일을 Load한다.
		 this->note->Load((LPCTSTR)fileName);
		 //3.7.7. 해당 파일의 캡션을 바꾼다.
		 SetWindowText(fileNameWithouttxt);


		 //3.7.8. 글자 변화를 초기화한다.
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
	LOGFONT m_font; //폰트의 정보를 얻기위한 구조체선언.

	//1. 글꼴 버튼을 클릭했을 때
	  //1.1. 글꼴이라는 폰트 공통 대화상자를 만든다.(현재 선택된 글꼴과 사이즈를 대화상자의 기본으로 설정한다.)
	memset(&m_font, 0, sizeof(LOGFONT));
	CClientDC dc(this);
	int fontSize = this->font->fontSize / 10;
	m_font.lfHeight = -MulDiv(fontSize, dc.GetDeviceCaps(LOGPIXELSY), 72);
	string previousFont = this->font->fontName;
	_tcscpy_s(m_font.lfFaceName, LF_FACESIZE, previousFont.c_str());


	CFontDialog dlg(&m_font); //(&m_font)

	//1.2. 공통 대화상자를 띄운다.
	if (IDOK == dlg.DoModal()) {
		//1.2.1. 확인을 누를 경우,

		//1. 선택한 글꼴을 확인한다.
		font = dlg.GetFaceName();
		//2. 선택한 글꼴의 크기를 확인한다.
		size = dlg.GetSize();
		// dlg.GetCurrentFont(&m_font); //폰트를 선택하고 확인버튼을 누르면 구조체로 폰트정보를 받는다.
		// m_font.lfFaceName();

		 //3. 적용한다.
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
	//1. 실행취소버튼 클릭했을 때
	//1.1. OnMyCommand 호출한다.

	//OnMyCommand(2002);
	this->SendMessage(WM_COMMAND, 2002);
}
#endif

void NotePadForm::OnPaint() {

	//2. 윈도우가 갱신될 때
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

	//0. CPaintDC를 얻고 CFont를 만들어준다.
	CPaintDC dc(this);
	CFont cFont;

	//그냥 출력하게 되면 흔들거려서 안되고, 비트맵화 해서 더블버퍼링 처리 해줘야 안 흔들리므로 절차 추가.
	//(21.10.15 비트맵 추가) 1. 현재 윈도우이 크기를 구한다.
	CRect rect;
	this->GetClientRect(&rect);

	//(21.10.15 비트맵 추가) 2. 임시 dc를 만들어준다.
	this->dcTemp.CreateCompatibleDC(&dc);

	//(21.10.15 비트맵 추가) 3. 현재 윈도우 크기에 맞게 비트맵을 만든다.
	HBITMAP hbmp = ::CreateCompatibleBitmap(dc, rect.right, rect.bottom);
	HBITMAP oldBMP = (HBITMAP)this->dcTemp.SelectObject(hbmp);

	//(21.10.15 비트맵 추가) 4. 비트맵의 색깔을 흰색으로 초기화한다.
	this->dcTemp.FillRect(&rect, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));


	//폰트추가*********************************************************
	LOGFONT printLogFont = this->m_font; //OnFontCommand에서 변경된 LOGFONT 구조체임.
	HFONT hFont = CreateFontIndirect(&printLogFont);
	HFONT oldFont = (HFONT)this->dcTemp.SelectObject(hFont);

	CSize strSize = this->dcTemp.GetTabbedTextExtent("각", 0, NULL);
	int strSizeY = strSize.cy;
	int strSizeX = strSize.cx;
	this->strSizeY = strSizeY;
	//*****************************************************************
	

	//(21.10.15. 추가) 현재 this->scroll->siVertical.nPos에 맞는 i(첨자)를 찾는다.
	int k = 0;
	while (strSizeY * k < this->scroll->siVertical.nPos) {
		k++;
	}
	i = k;

	//(21.10.15. 추가) 현재 this->scroll->siVertical.nPos + windowHeight 에 맞는 length를 찾는다.
	Long caretHeight = length * strSizeY;

	//만약 전체 글자 높이가 nPos + windowHeight 보다 큰 경우, (아닌 경우 length는 기존 length로 갑니다.)
	if (caretHeight > this->scroll->siVertical.nPos + this->scroll->windowHeight) {
		int k = 1;
		int l = 0;
		while (l < length && strSizeY * k <= this->scroll->siVertical.nPos + this->scroll->windowHeight) {
			k++;
			l++;
		}
		length = l;
	}

	//(21.11.23.추가) 
	this->firstTextY = i;
	this->endTextY = length - 1;

	//(21.11.24. Visitor 패턴 추가) ++++++++++++++++++++++++++++++++++
	this->note->Accept(this->visitor);
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	//(21.08.18) 선택하다 추가*****************************************
	Long firstY = this->selectWords->GetFirstY();
	Long firstX = this->selectWords->GetFirstX();
	Long endY = this->selectWords->GetEndY();
	Long endX = this->selectWords->GetEndX();
	//******************************************************************

	//좌표가 isBigger인지 확인한다.
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

	// isBigger이 아니면 좌표를 서로 바꾼다.

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

	//선택된 단어가 있으면,
	if (endX > 0 || endY > 0) {

		i = firstY;
		//(21.11.25.추가) 선택하다 visitor 패턴 적용.
		this->visitor->SetFirstY(firstY);
		this->visitor->SetFirstX(firstX);
		this->visitor->SetEndY(endY);
		this->visitor->SetEndX(endX);

		// 배경색과 글자색을 바꿔준다.
		this->dcTemp.SetTextColor(RGB(255, 255, 255));
		this->dcTemp.SetBkColor(RGB(000, 102, 204));

		//(21.11.24. Visitor 패턴 추가) ++++++++++++++++++++++++++++++++++
		this->visitor->VisitNoteForSelect(this->note);
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	}

	//(21.10.15 비트맵 추가) 5. 비트맵을 출력한다.
	dc.BitBlt(0, 0, rect.right, rect.bottom, &this->dcTemp, 0, 0, SRCCOPY);

	//(21.10.15 비트맵 추가) 6. 만든 비트맵을 삭제한다.
	this->dcTemp.SelectObject(oldBMP);
	::DeleteObject(hbmp);
	this->dcTemp.DeleteDC();


	//새로운 폰트 제거
	cFont.DeleteObject();

#if 0
	//2. 윈도우가 갱신될 때
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

	//0. CPaintDC를 얻고 CFont를 만들어준다.
	CPaintDC dc(this);
	CFont cFont;

	//그냥 출력하게 되면 흔들거려서 안되고, 비트맵화 해서 더블버퍼링 처리 해줘야 안 흔들리므로 절차 추가.
	//(21.10.15 비트맵 추가) 1. 현재 윈도우이 크기를 구한다.
	CRect rect;
	this->GetClientRect(&rect);

	//(21.10.15 비트맵 추가) 2. 임시 dc를 만들어준다.
	CDC dcTemp;
	dcTemp.CreateCompatibleDC(&dc);

	//(21.10.15 비트맵 추가) 3. 현재 윈도우 크기에 맞게 비트맵을 만든다.
	HBITMAP hbmp = ::CreateCompatibleBitmap(dc, rect.right, rect.bottom);
	HBITMAP oldBMP = (HBITMAP)dcTemp.SelectObject(hbmp);

	//(21.10.15 비트맵 추가) 4. 비트맵의 색깔을 흰색으로 초기화한다.
	dcTemp.FillRect(&rect, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));


	//폰트추가*********************************************************
	LOGFONT printLogFont = this->m_font; //OnFontCommand에서 변경된 LOGFONT 구조체임.
	HFONT hFont = CreateFontIndirect(&printLogFont);
	HFONT oldFont = (HFONT)dcTemp.SelectObject(hFont);
	
	CSize strSize = dcTemp.GetTabbedTextExtent("각", 0, NULL);
	int strSizeY = strSize.cy;
	this->strSizeY = strSizeY;
	//*****************************************************************

	//(21.10.15. 추가) 현재 this->scroll->siVertical.nPos에 맞는 i(첨자)를 찾는다.
	int k = 0;
	while (strSizeY * k < this->scroll->siVertical.nPos) {
		k++;	
	}
	i = k;

	//(21.10.15. 추가) 현재 this->scroll->siVertical.nPos + windowHeight 에 맞는 length를 찾는다.
	Long caretHeight = length * strSizeY;

	//만약 전체 글자 높이가 nPos + windowHeight 보다 큰 경우, (아닌 경우 length는 기존 length로 갑니다.)
	if (caretHeight > this->scroll->siVertical.nPos + this->scroll->windowHeight) {
		int k = 1;
		int l = 0;
		while (l < length && strSizeY * k <= this->scroll->siVertical.nPos + this->scroll->windowHeight) {
			k++;
			l++;
		}
		length = l;
	}

	//(21.11.23.추가) 
	this->firstTextY = i;

	// 전체 문자 출력한다.
	while (i < length) {
		row = this->note->GetChild(i);
		content_ = row->GetContent();

		content = CString(content_.c_str());
		
		dcTemp.TextOut(-this->scroll->siHorizon.nPos, (strSizeY * i) - this->scroll->siVertical.nPos, content);

		i++;
	}

	//(21.11.22.추가) 
	this->endTextY = i-1;


	//(21.08.18) 선택하다 추가*****************************************
	Long firstY = this->selectWords->GetFirstY();
	Long firstX = this->selectWords->GetFirstX();
	Long endY = this->selectWords->GetEndY();
	Long endX = this->selectWords->GetEndX();
	//******************************************************************

	//좌표가 isBigger인지 확인한다.
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

	// isBigger이 아니면 좌표를 서로 바꾼다.

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

		//1. j를 결정한다.
		if (i <= firstY) {
			j = firstX;
		}
		else {
			j = 0;
		}

		//2. limitX 를 결정한다.
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

			//(21.11.10.추가)
			if (content == "\t") {
				content = "        ";			
			}


			// 배경색과 글자색을 바꿔준다.
			dcTemp.SetTextColor(RGB(255, 255, 255));
			dcTemp.SetBkColor(RGB(000, 102, 204));

			// X 커서까지의 물리적 거리를 계산한다.
			distanceX = this->getTextSize->totalCalculateX(j, row);

			// 바뀐 부분만 재출력한다.
			dcTemp.TextOut(-this->scroll->siHorizon.nPos + distanceX, (strSizeY* i) - this->scroll->siVertical.nPos, content);
			j++;
		}

		i++;
	}


	//(21.10.15 비트맵 추가) 5. 비트맵을 출력한다.
	dc.BitBlt(0, 0, rect.right, rect.bottom, &dcTemp, 0, 0, SRCCOPY);

	//(21.10.15 비트맵 추가) 6. 만든 비트맵을 삭제한다.
	dcTemp.SelectObject(oldBMP);
	::DeleteObject(hbmp);
	dcTemp.DeleteDC();


	//새로운 폰트 제거
	cFont.DeleteObject();
#endif

}

void NotePadForm::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) { //1Byte 추가될 때

	//OnCommand();

	//char character_[2];
	//this->character[0] = nChar;
	//this->character[1] = '\0';

	//this->nChar = nChar;
	//OnMyCommand(IDC_COMMAND_ONCHAR);


	if (((GetKeyState(VK_CONTROL) >= 0) && (nChar != 'Z' || nChar != 'Y')) && nChar != VK_BACK && nChar != VK_ESCAPE) { //OR !(GetKeyState(VK_CONTROL) & 0x8000) 
		this->nChar = nChar;
		//Notepad 내에서 자신의 연산을 호출하는건 지양해야 하기 때문에 SendMessage를 활용
		//OnMyCommand(IDC_COMMAND_ONCHAR);

		//(21.09.14 추가) 1. 선택된 단어가 있으면, BackSpace부터 호출한다.
		if (this->selectWords->firstX > 0 || this->selectWords->firstY > 0 || this->selectWords->endX > 0 ||
			this->selectWords->endY > 0) {
			this->SendMessage(WM_COMMAND, 40013);
		}

		//2. OnChar 호출한다.
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

	//21.08.03 추가
	this->scroll->windowWidth = this->m_rectCurHist.right - this->m_rectCurHist.left;
	this->scroll->windowHeight = this->m_rectCurHist.bottom - this->m_rectCurHist.top;

	//&& (nChar == 0x56 && (GetKeyState(VK_CONTROL) < 0))
	if (nChar != VK_BACK ) { //BackSpace가 아니면,

		char character_[2];
		character_[0] = nChar;
		character_[1] = '\0';

		CreateGlyph* creator = new CreateProduct();

		//1.1. 현재 캐럿의 위치를 확인한다.
		currentY = this->note->GetCurrent();
		row = this->note->GetChild(currentY);
		currentX = row->GetCurrent();
		noteLength = this->note->GetLength();
		rowLength = row->GetLength();

		//1.2. 자동개행 버튼이 눌려진 경우,
		if (this->isLineChangeButtonClicked == TRUE) {
			
			//1.2.1. 자동개행 전 캐럿의 위치를 찾는다.
			this->lineChange->FindCaretBeforeLineChange(&currentX, &currentY);

			//1.2.2. 자동개행 된 문서를 펼친다.
			this->lineChange->LineChangeButtonNotClicked();

			row = this->note->GetChild(currentY);
			rowLength = row->GetLength();
		}

		//1.3. 입력받은 문자가 개행문자인 경우,
		if (character_[0] == '\n' || character_[0] == '\r') {

			//1.3.1. 현재 note에 currentY+1번째에 새 줄을 끼워넣는다. (붙혀적다 + 끼워적다)
			this->row = creator->Make(character_);
			this->note->Add(currentY + 1, this->row);

			//1.3.2. currentX가 row의 length보다 작은 경우, (끼워적다)
			if (currentX < rowLength) {
				//1.3.2.1. 현재 row의 length만큼 반복한다.
				while (currentX < rowLength) {
					//1. 문자를 읽는다.
					character = row->GetChild(currentX);
					//2. currentY+1번째에 붙혀적는다.
					this->row->Add(character);

					//3. currentX번째 글자를 삭제한다.
					row->Remove(currentX);

					rowLength--;
				}
			}
			//1.3.3. 캐럿을 (currentY+1, 0)으로 좌표설정한다.
			changedY = currentY + 1;
			changedX = 0;	
		}

		//1.4. 입력받은 문자가 개행문자가 아닌 경우,
		else {

			//1.4.0. character를 만든다.
			character = creator->Make(character_);

			//1.4.1. currentX가 row의 length보다 작은 경우(끼워적다), 현재 row의 currentX번째에 글자를 끼워적는다.
			if (currentX < rowLength) {
				row->Add(currentX, character);
			}
			//1.4.2. currentX가 row의 length보다 크거나 같은 경우, 현재 row에 붙혀 적는다.
			else {
				row->Add(character);
			}
			//1.4.3. 캐럿을 (currentY, currentX+1)로 좌표설정한다.
			changedY = currentY;
			changedX = currentX + 1;
		}

		//1.5. 자동개행 버튼이 눌려진 경우,
		if (this->isLineChangeButtonClicked == TRUE) {

			//1.5.1. 펼친 문서를 다시 자동개행한다. (ON)
			this->lineChange->LineChangeButtonClicked();

			//1.5.1. 자동개행 후 이동할 캐럿의 위치를 구한다.
			this->lineChange->FindCaretAfterLineChange(changedX, changedY);
		}
		else {
			//1.6. 캐럿을 옮긴다.
			this->note->MoveCurrent(changedY);
			this->row = this->note->GetChild(changedY);
			this->row->MoveCurrent(changedX);
		}

		if (creator != NULL) {
			delete creator;
			creator = NULL;	
		}


		//2.6. currentX(row의 current)보다 작은 동안 반복한다.
		this->isOnImeCharForMove = FALSE;
		this->isFirstComposingForMove = FALSE;


		//----------------------------------------------------------------------------
		this->scroll->MovePage();


		// Scroll 업데이트 할 때의 조건을 설정한다.
		this->isMoveActionHorizon = FALSE;
		this->isMoveActionVertical = FALSE;
		this->isAppend = FALSE;
		this->isInsert = FALSE;
		this->isDeleteAction = FALSE;

		//----------------------------------------------------------------------------

		Notify(); //Observer Pattern 이후


		this->isChanged = TRUE;
		Invalidate();
	}
#endif
}

LRESULT NotePadForm::OnImeComposition(WPARAM wParam, LPARAM lParam) { //UINT wParam, Long LPARAM 에서 변경 ,,WPARAM LPARAM

	//this->letter = wParam;

	char letter[3] = { '\0', };
	letter[0] = HIBYTE(LOWORD(wParam));
	letter[1] = LOBYTE(LOWORD(wParam));
	letter[2] = '\0';

	this->letter = "\0";
	this->letter = letter;

//	if (this->letter != "\0") {
		//(21.09.14 추가) 1. 선택된 단어가 있으면, BackSpace부터 호출한다.
		if (this->selectWords->firstX > 0 || this->selectWords->firstY > 0 || this->selectWords->endX > 0 ||
			this->selectWords->endY > 0) {
			this->SendMessage(WM_COMMAND, 40013);
		}

		//2. OnImeComposition을 호출한다.
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

	//21.08.03 추가
	this->scroll->windowWidth = this->m_rectCurHist.right - this->m_rectCurHist.left;
	this->scroll->windowHeight = this->m_rectCurHist.bottom - this->m_rectCurHist.top;


	//1. 조합중일 때
	  //1.1. 현재 캐럿의 위치를 확인한다.
	  Long currentY = this->note->GetCurrent();
	  this->row = this->note->GetChild(currentY);
	  Long currentX = this->row->GetCurrent();
	  length = this->row->GetLength();

	  //1.2. 새로 시작한 글자 외에 기존 글자를 지운다.
	  if (this->isFirstComposing == FALSE) {
		  this->row->Remove(currentX); //this->index
	  }

	  //1.3. (21.08.12 추가) 자동개행버튼 OFF일 경우,
	  if (this->isLineChangeButtonClicked == FALSE) {
		  //1.3. currentX가 row의 length-1보다 작거나, currentX가 length-1보다 크거나 같고, 커서 크기가 3인 경우,
		  if (currentX < length - 1) {
			  //|| (currentX >= length - 1 && this->caretWidth == 3) 잠시뺐음. 다시 껴야함.

			  //1.3.1. 현재 줄에 새로 글자를 끼워적는다.
			  this->row->Add(currentX, character);

			  this->isInsert = TRUE;
			  this->isAppend = FALSE;
			  this->isMoveActionVertical = FALSE;
			  this->isMoveActionHorizon = FALSE;
			  this->isDeleteAction = FALSE;
		  }

		  //1.4. currentX가 row의 length보다 크거나 같거나, currentX가 length-1보다 크거나 같고, 커서 크기가 3보다 큰 경우,
		  else if (currentX >= length || (currentX >= length - 1 && this->caretWidth > 3)) {
			  //1.4.1. 현재 줄에 새로 글자를 붙혀적는다.
			  this->row->Add(character);

			  this->isInsert = FALSE;
			  this->isAppend = TRUE;
			  this->isMoveActionVertical = FALSE;
			  this->isMoveActionHorizon = FALSE;
		  }

		  //1.5. 좌표 이동하지 않고, 커서 크기만 바꿔준다.
		  if (this->isFirstComposing == TRUE) {

			  this->isOnImeCharForMove = FALSE;
			  this->isFirstComposingForMove = this->isFirstComposing;

			  Notify();
		  }
	  
	  }

	  //1.4. (21.08.12 추가) 자동개행버튼 ON일 경우,
	  else if (this->isLineChangeButtonClicked == TRUE) {
		  //1.4.0. 끼워적다의 경우,
		  if (currentY + 1 < this->note->GetLength()) {
			  dummyRow = this->note->GetChild(currentY + 1); 
		  }
		  else {
			  dummyRow = NULL;
		  }
		  //1.4.1. currentX가 row의 length-1보다 작거나, currentX가 length-1보다 크거나 같고, 커서 크기가 3인 경우,(끼워적다)
		  if (currentX < length || (dummyRow!=NULL && dynamic_cast<DummyRow*>(dummyRow))) { //currentX < length - 1

			  row = this->note->GetChild(currentY);

			  //1.4.1.1. 조건에 따라 캐럿의 조건을 설정한다.
			  if (currentX < length) { //length - 1
				  isLineChanged = FALSE;
			  }
			  else if (dummyRow != NULL && dynamic_cast<DummyRow*>(dummyRow)) {
				  isLineChanged = TRUE;
			  }

			  //1.4.1.2. 현재 줄에 새로 글자를 끼워적는다.
			  row->Add(currentX, character);

			  //(21.08.13. 예외상황 추가)********************************************
              //1. 끼워적기인데 (dummy와 dummy사이)
              //2. 맨 끝 커서 위치 (currentX = row->GetLength())
              //3. 이대로 가면 원래는 무조건 isLineChanged = TRUE 이지만,
              //4. 예외인 경우도 있다. 즉, 끼워적었는데도 windowWidth를 안넘기는 경우
              //5. 그래서 끼워적은 후, width 테스트 해주는 로직 추가..

			  exceptWidth = this->getTextSize->totalCalculateX(row->GetLength(), row);
			  if (isLineChanged == TRUE && exceptWidth <= this->scroll->windowWidth) {
				  isLineChanged = FALSE;
			  }
			  //**********************************************************************

			  //1.4.1.3. 새로 시작한 글자일 경우,
			  if (this->isFirstComposing == TRUE) {

				  //해당 row에서 자를 위치가 0보다 크거나 같은동안 반복한다.
				  location = this->lineChange->FindSplitLocationByWriting(row);

				  i = currentY;
				  while (location >= 0) {
					  //1. 해당 row의 다음 row가 dummyRow가 아니거나 없을 경우,dummyRow 만든다.
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

					  //2. SplitForFront를 호출한다.
					  dummyRow = row->SplitForFront(dummyRow, location);

					  //3. 출력받은 dummyRow에서 자를 위치가 있는지 찾는다.
					  location = this->lineChange->FindSplitLocationByWriting(dummyRow);
					  row = dummyRow;
					  i++;
				  }
				  
//#if 0
				  // 캐럿 좌표를 설정한다.
				  if (isLineChanged == TRUE) { //줄이 바꼈으면,
					  this->note->MoveCurrent(currentY + 1);
					  this->row = this->note->GetChild(currentY + 1);
					  this->row->MoveCurrent(0); // 한글이므로 0으로 설정.

				  }
//#endif
			  }

		  }

		  //1.4. currentX가 row의 length보다 크거나 같거나, currentX가 length-1보다 크거나 같고, 커서 크기가 3보다 큰 경우,
		  else if (currentX >= length || (currentX >= length - 1 && this->caretWidth > 3)) {
			  //1.4.1. 현재 줄에 새로 글자를 붙혀적는다.
			  this->row->Add(character);

			  //1.4.2. 새로 시작한 글자의 경우,
			  if (this->isFirstComposing == TRUE) {
				  //1.4.2.1. 해당 row에서 자를 위치가 0보다 크거나 같은 경우,
				  location = this->lineChange->FindSplitLocationByWriting(this->row);
				  if (location >= 0) {
					  //1. dummyRow를 만든다.
					  dummyRow = new DummyRow();

					  //2. dummyRow에 추가한다.
					  character = this->row->GetChild(location);
					  dummyRow->Add(character);

					  //3. currentY+1의 노트에 끼운다.
					  this->note->Add(currentY + 1, dummyRow);

					  //4. 기존 row에서 삭제
					  this->row->Remove(location);

					  //5. 캐럿을 (currentY+1, 0)으로 좌표 설정한다.
					  this->note->MoveCurrent(currentY + 1);
					  dummyRow->MoveCurrent(0);
				  }
			  }
		  }

		  //1.5. 좌표 이동하지 않고, 커서 크기만 바꿔준다.
		  if (this->isFirstComposing == TRUE) {
			  // 스크롤 설정
			  this->isInsert = FALSE;
			  this->isAppend = FALSE;
			  this->isMoveActionVertical = FALSE;
			  this->isMoveActionHorizon = FALSE;
			  this->isDeleteAction = FALSE;

			  //캐럿 좌표 설정
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

	//21.08.03 추가
	this->scroll->windowWidth = this->m_rectCurHist.right - this->m_rectCurHist.left;
	this->scroll->windowHeight = this->m_rectCurHist.bottom - this->m_rectCurHist.top;



	//2. 글자가 완성 되었을 때
	  //2.1. 현재 캐럿의 위치를 확인한다.
	  Long currentY = this->note->GetCurrent();
	  this->row = this->note->GetChild(currentY);
	  Long currentX = this->row->GetCurrent();

	  //2.2. 기존에 쓰여진 글자를 지운다.
	  this->row->Remove(currentX);
	  length = this->row->GetLength();


	  //(21.08.12. 추가) 자동개행버튼 OFF일 경우,
	  if (this->isLineChangeButtonClicked == FALSE) {

		  //2.3. currentX가 row의 length-1보다 작거나, currentX가 length-1보다 크거나 같고, 커서 크기가 3인 경우,
		  if (currentX < length - 1 || (currentX >= length - 1 && this->caretWidth == 3)) {
			  //2.3.1. 현재 줄에 새로 글자를 끼워적는다.
			  this->row->Add(currentX, character);

			  this->isInsert = TRUE;
			  this->isAppend = FALSE;
			  this->isMoveActionVertical = FALSE;
			  this->isMoveActionHorizon = FALSE;
			  this->isDeleteAction = FALSE;
		  }

		  //2.4. currentX가 row의 length보다 크거나 같거나, currentX가 length-1보다 크거나 같고, 커서 크기가 3보다 큰 경우,
		  else if (currentX >= length || (currentX >= length - 1 && this->caretWidth > 3)) {
			  //2.4.1. 현재 줄에 새로 글자를 붙혀적는다.
			  this->row->Add(character);

			  this->isInsert = FALSE;
			  this->isAppend = TRUE;
			  this->isMoveActionVertical = FALSE;
			  this->isMoveActionHorizon = FALSE;
			  this->isDeleteAction = FALSE;
		  }

		  //2.5. 좌표를 currentX+1로 이동한다.
		  this->row->MoveCurrent(currentX + 1);
	  }
	  //(21.08.12. 추가) 자동개행버튼 ON일 경우,
	  else if (this->isLineChangeButtonClicked == TRUE) {

#if 0
		  //2.3. currentX가 row의 length-1보다 작거나, currentX가 length-1보다 크거나 같고, 커서 크기가 3인 경우,
		  if (currentX < length - 1 || (currentX >= length - 1 && this->caretWidth == 3)) {
			  //2.3.1. 현재 줄에 새로 글자를 끼워적는다.
			  this->row->Add(currentX, character);
		  }

		  //2.4. currentX가 row의 length보다 크거나 같거나, currentX가 length-1보다 크거나 같고, 커서 크기가 3보다 큰 경우,
		  else if (currentX >= length || (currentX >= length - 1 && this->caretWidth > 3)) {
			  //2.4.1. 현재 줄에 새로 글자를 붙혀적는다.
			  this->row->Add(character);
		  }
#endif
		  this->row->Add(currentX, character);

		  //2.5. 좌표를 currentX+1로 이동한다.
		  this->row->MoveCurrent(currentX + 1);

		  //스크롤 설정
		  this->isInsert = FALSE;
		  this->isAppend = FALSE;
		  this->isMoveActionVertical = FALSE;
		  this->isMoveActionHorizon = FALSE;
		  this->isDeleteAction = FALSE;
	  }

	  //2.7. currentX(row의 current)보다 작은 동안 반복한다.
	  this->isOnImeCharForMove = TRUE;
	  this->isFirstComposingForMove = this->isFirstComposing;
	  Notify();


	  this->isFirstComposing = TRUE;
	  this->isChanged = TRUE;

	  Invalidate();

	  return 0;
#endif
}

void NotePadForm::OnSetFocus(CWnd* pOldWnd) { //Attach 해주기
	CFrameWnd::OnSetFocus(pOldWnd);

	this->isKillFocus = FALSE;
	int strSize = this->getTextSize->GetX(128);

#if 0
	CreateSolidCaret(3, strSize); //캐럿생성  (3, strSize.cy);
	SetCaretPos(CPoint(0,0)); //위치설정

	ShowCaret(); //화면에 보이도록 한다.
#endif

#if 0
	//(21.11.11.추가) 멤버 currentX, currentY의 값이 있을 경우, 설정해준다.
	this->note->MoveCurrent(this->currentY);
	Glyph* row = this->note->GetChild(this->currentY);
	row->MoveCurrent(this->currentX);
#endif

	//1. Subject 멤버에 Location을 추가해준다.

	if (this->location == NULL) {
		this->location = new Location(this);	
		dynamic_cast<Subject*>(this)->Attach(this->location); //Attach의 매개변수는 Observer* observe이지만
	}


	//어차피 location의 부모클래스가 Observer여서 굳이 할당해줄 필요x.

#if 0
	// Scroll 업데이트 할 때의 조건을 설정한다.
	this->isJumpOverForPrevious = false;
	this->isJumpOverForNext = false;
	this->isDoubleByte = false;
	this->isUp = false;
	this->isDown = false;

	Notify();
	Invalidate();

#endif
}

void NotePadForm::OnKillFocus(CWnd* pNewWnd) { //Detach 해주기
	CFrameWnd::OnKillFocus(pNewWnd);

	this->isKillFocus = TRUE;

#if 0

	//(21.11.11.추가) 1. 현재 캐럿의 위치를 구한다.
	this->currentY = this->note->GetCurrent();
	Glyph* row = this->note->GetChild(this->currentY);
	this->currentX = row->GetCurrent();


	//2. 캐럿을 없앤다.
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
	char characterMove[3]; //Observe Pattern 때문에.
	KeyAction* keyAction = NULL;
	FactoryProduct factoryProduct(this);
	Command* command;

	//***************OnKeyDown에 팩토리만들때 할당을 계속 하고있다. 의존화 관계이면 내용으로 만들고
	//***************Make 하면 된다.FactoryProduct factoryProduct; factoryProduct.Make(~~);

	this->nChar = nChar;

	//0. 이동 키가 눌렸으면 선택된 좌표를 초기화한다.*****************************************************
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

		//1. Shift 키에서 손을 뗐으면, 드래그 중을 false로 설정한다.
		this->selectWords->isDragging = FALSE;
		this->isShiftClicked = FALSE;
		this->isShifting = FALSE;

		this->ShiftX = 0;
		this->ShiftY = 0;
	}
	//******************************************************************************************************

	//37. (21.11.17.추가) Shift + 마우스 왼쪽 버튼 눌렀을 때
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


	//1. 왼쪽 키 버튼 눌렀을 때
	if (nChar == VK_LEFT && (GetKeyState(VK_CONTROL) >= 0) && (GetKeyState(VK_SHIFT) >= 0)) {
		this->SendMessage(WM_COMMAND, 40025);

#if 0
		//this->factoryProduct = new FactoryProduct(this);
		keyAction = factoryProduct.Make(nChar, NULL, NULL);
		keyAction->KeyDown();
#endif
	
	}
	//2. 오른쪽 키 버튼 눌렀을 때
	else if (nChar == VK_RIGHT && (GetKeyState(VK_CONTROL) >= 0) && (GetKeyState(VK_SHIFT) >= 0)) {
		this->SendMessage(WM_COMMAND, 40026);
#if 0
		//this->factoryProduct = new FactoryProduct(this);
		keyAction = factoryProduct.Make(nChar, NULL, NULL);
		keyAction->KeyDown();
#endif
	}
	//3. Ctrl + 왼쪽 키 눌렀을 때
	else if (nChar == VK_LEFT && (GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_SHIFT) >= 0)) {
		this->SendMessage(WM_COMMAND, 40027);
#if 0
		//this->factoryProduct = new FactoryProduct(this);
		keyAction = factoryProduct.Make(nChar, VK_CONTROL, NULL);
		keyAction->KeyDown();
#endif
	}
	//4. Ctrl + 오른쪽 키 눌렀을 때
	else if (nChar == VK_RIGHT && (GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_SHIFT) >= 0)) {
		this->SendMessage(WM_COMMAND, 40028);
#if 0
		//this->factoryProduct = new FactoryProduct(this);
		keyAction = factoryProduct.Make(nChar, VK_CONTROL, NULL);
		keyAction->KeyDown();
#endif
	}
	//5. home 키 눌렸을 때
	else if (nChar == VK_HOME && (GetKeyState(VK_CONTROL) >= 0) && (GetKeyState(VK_SHIFT) >= 0)) {
		this->SendMessage(WM_COMMAND, 40029);
#if 0
		//this->factoryProduct = new FactoryProduct(this);
		keyAction = factoryProduct.Make(nChar, NULL, NULL);
		keyAction->KeyDown();
#endif
	}
	//6. end 키 눌렸을 때
	else if (nChar == VK_END && (GetKeyState(VK_CONTROL) >= 0) && (GetKeyState(VK_SHIFT) >= 0)) {
		this->SendMessage(WM_COMMAND, 40030);
#if 0
		//this->factoryProduct = new FactoryProduct(this);
		keyAction = factoryProduct.Make(nChar, NULL, NULL);
		keyAction->KeyDown();
#endif
	}
	//7. Ctrl + home 키 눌렀을 때
	else if (nChar == VK_HOME && (GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_SHIFT) >= 0)) { 
		this->SendMessage(WM_COMMAND, 40031);
#if 0
		//this->factoryProduct = new FactoryProduct(this);
		keyAction = factoryProduct.Make(nChar, VK_CONTROL, NULL);
		keyAction->KeyDown();
#endif
	}
	//8. Ctrl + end 키 눌렀을 때
	else if (nChar == VK_END && (GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_SHIFT) >= 0)) { 
		this->SendMessage(WM_COMMAND, 40032);
#if 0
		//this->factoryProduct = new FactoryProduct(this);
		keyAction = factoryProduct.Make(nChar, VK_CONTROL, NULL);
		keyAction->KeyDown();
#endif
	}
	//9. 윗쪽 키 눌렀을 때
	else if (nChar == VK_UP && (GetKeyState(VK_CONTROL) >= 0) && (GetKeyState(VK_SHIFT) >= 0)) {
		this->SendMessage(WM_COMMAND, 40033);
#if 0
		//this->factoryProduct = new FactoryProduct(this);
		keyAction = factoryProduct.Make(nChar, NULL, NULL);
		keyAction->KeyDown();
#endif
	}
	//10. 아랫쪽 키 눌렀을 때
	else if (nChar == VK_DOWN && (GetKeyState(VK_CONTROL) >= 0) && (GetKeyState(VK_SHIFT) >= 0)) {
		this->SendMessage(WM_COMMAND, 40034);
#if 0
		//this->factoryProduct = new FactoryProduct(this);
		keyAction = factoryProduct.Make(nChar, NULL, NULL);
		keyAction->KeyDown();
#endif
	}
	//11.(21.08.17 추가) Shift + 왼쪽 키 버튼 눌렀을 때
	else if (nChar == VK_LEFT && (GetKeyState(VK_CONTROL) >= 0) && (GetKeyState(VK_SHIFT) < 0)) {
		this->SendMessage(WM_COMMAND, 40015);
#if 0
		//this->factoryProduct = new FactoryProduct(this);
		keyAction = factoryProduct.Make(nChar, VK_SHIFT, NULL);
		keyAction->KeyDown();
#endif
	}

	//12. (21.08.17 추가) Shift + 오른쪽 키 버튼 눌렀을 때
	else if (nChar == VK_RIGHT && (GetKeyState(VK_CONTROL) >= 0) && (GetKeyState(VK_SHIFT) < 0)) {
		this->SendMessage(WM_COMMAND, 40016);
#if 0
		//this->factoryProduct = new FactoryProduct(this);
		keyAction = factoryProduct.Make(nChar, VK_SHIFT, NULL);
		keyAction->KeyDown();
#endif
	}

	//13. (21.08.18 추가) Shift + Ctrl + 왼쪽 키 버튼 눌렀을 때
	else if (nChar == VK_LEFT && (GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_SHIFT) < 0)) {
		this->SendMessage(WM_COMMAND, 40018);
#if 0
		keyAction = factoryProduct.Make(nChar, VK_CONTROL, VK_SHIFT);
		keyAction->KeyDown();
#endif
	}

	//14. (21.08.18 추가) Shift + Ctrl + 오른쪽 키 버튼 눌렀을 때
	else if (nChar == VK_RIGHT && (GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_SHIFT) < 0)) {
		this->SendMessage(WM_COMMAND, 40017);
#if 0
		keyAction = factoryProduct.Make(nChar, VK_CONTROL, VK_SHIFT);
		keyAction->KeyDown();
#endif
	}

	//15. (21.08.18 추가) Shift + home 키 버튼 눌렀을 때
	else if (nChar == VK_HOME && (GetKeyState(VK_SHIFT) < 0) && (GetKeyState(VK_CONTROL) >= 0)) {
		this->SendMessage(WM_COMMAND, 40019);
#if 0
		keyAction = factoryProduct.Make(nChar, VK_SHIFT, NULL);
		keyAction->KeyDown();
#endif
	}

	//16. (21.08.18 추가) Shift + end 키 버튼 눌렀을 때
	else if (nChar == VK_END && (GetKeyState(VK_SHIFT) < 0) && (GetKeyState(VK_CONTROL) >= 0)) {
		this->SendMessage(WM_COMMAND, 40020);
#if 0
		keyAction = factoryProduct.Make(nChar, VK_SHIFT, NULL);
		keyAction->KeyDown();
#endif
	}

	//17.  Ctrl + Shift + home 키 눌렀을 때
	else if (nChar == VK_HOME && (GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_SHIFT) < 0)) { //(GetKeyState(VK_CONTROL) < 0)
	    this->SendMessage(WM_COMMAND, 40021);
#if 0
        //this->factoryProduct = new FactoryProduct(this);
		keyAction = factoryProduct.Make(nChar, VK_CONTROL, VK_SHIFT);
		keyAction->KeyDown();
#endif
	}

	//18.  Ctrl + Shift + end 키 눌렀을 때
	else if (nChar == VK_END && (GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_SHIFT) < 0)) { //(GetKeyState(VK_CONTROL) < 0)
	    this->SendMessage(WM_COMMAND, 40022);
#if 0
		//this->factoryProduct = new FactoryProduct(this);
		keyAction = factoryProduct.Make(nChar, VK_CONTROL, VK_SHIFT);
		keyAction->KeyDown();
#endif
	}

	//19. (21.08.17 추가) Shift + 윗쪽 키 버튼 눌렀을 때
	else if (nChar == VK_UP && (GetKeyState(VK_CONTROL) >= 0) && (GetKeyState(VK_SHIFT) < 0)) {
	    this->SendMessage(WM_COMMAND, 40023);
#if 0
		//this->factoryProduct = new FactoryProduct(this);
		keyAction = factoryProduct.Make(nChar, VK_SHIFT, NULL);
		keyAction->KeyDown();
#endif
	}

	//20. (21.08.17 추가) Shift + 아래쪽 키 버튼 눌렀을 때
	else if (nChar == VK_DOWN && (GetKeyState(VK_CONTROL) >= 0) && (GetKeyState(VK_SHIFT) < 0)) {
	    this->SendMessage(WM_COMMAND, 40024);
#if 0
		//this->factoryProduct = new FactoryProduct(this);
		keyAction = factoryProduct.Make(nChar, VK_SHIFT, NULL);
		keyAction->KeyDown();
#endif
	}
	//21. BACKSPACE 눌렀을 때
	else if (nChar == VK_BACK && (GetKeyState(VK_CONTROL) >= 0)) {  	    
#if 0
		//this->factoryProduct = new FactoryProduct(this);
		keyAction = factoryProduct.Make(nChar, NULL, NULL);
		keyAction->KeyDown();
#endif
		this->SendMessage(WM_COMMAND, 40013);
	}
	//22. Delete 눌렀을 때
	else if (nChar == VK_DELETE && (GetKeyState(VK_CONTROL) >= 0)) {
#if 0
		//this->factoryProduct = new FactoryProduct(this);
		keyAction = factoryProduct.Make(nChar, NULL, NULL);
		keyAction->KeyDown();
#endif
		this->SendMessage(WM_COMMAND, 40038);
	}
	//23. Ctrl + C (복사하기) 눌렀을 때
	else if (nChar == 0x43 && (GetKeyState(VK_CONTROL) < 0)) {
	    this->SendMessage(WM_COMMAND, 40035);
#if 0
	    keyAction = factoryProduct.Make(nChar, VK_CONTROL, NULL);
	    keyAction->KeyDown();
#endif
    }
	//24. Ctrl + V (붙혀넣기) 눌렀을 때
	else if (nChar == 0x56 && (GetKeyState(VK_CONTROL) < 0)) {
	     this->SendMessage(WM_COMMAND, 40036);
#if 0
	    keyAction = factoryProduct.Make(nChar, VK_CONTROL, NULL);
	    keyAction->KeyDown();
#endif
	}
	
	//25. (21.08.24. 추가) Ctrl + X (잘라내다) 눌렀을 때
	else if (nChar == 0x58 && (GetKeyState(VK_CONTROL) < 0)) {
	     this->SendMessage(WM_COMMAND, 40037);
#if 0
	    keyAction = factoryProduct.Make(nChar, VK_CONTROL, NULL);
	    keyAction->KeyDown();
#endif
    } 
	//26. (21.08.26. 추가) Ctrl + F (찾다) 눌렀을 때
	else if (nChar == 0x46 && (GetKeyState(VK_CONTROL) < 0)) {
	     this->SendMessage(WM_COMMAND, 40039);
#if 0
	   if (this->findDialog == NULL) {
		   this->findDialog = new FindDialog(this);
	   }
	   this->findDialog->Create(NULL, FR_DOWN, this);
#endif
    }
	
	// (21.09.15 추가) Ctrl + H (바꾸다) 눌렀을 때
	else if (nChar == 0x48 && (GetKeyState(VK_CONTROL) < 0)) {
	     this->SendMessage(WM_COMMAND, 40043);
    }
	//27. (21.09.02 추가) Ctrl + Z (실행취소) 눌렀을 때
	else if (nChar == 0x5A && (GetKeyState(VK_CONTROL) < 0)) {
		//OnMyCommand(2002);
		this->SendMessage(WM_COMMAND, 40002);
    }
	//28. (21.09.05 추가) Ctrl + Y (실행복구) 눌렀을 때
	else if (nChar == 0x59 && (GetKeyState(VK_CONTROL) < 0)) {
	    this->SendMessage(WM_COMMAND, 40003);
    }
	//29. (21.09.07 추가) Ctrl + S (저장) 눌렀을 때
	else if (nChar == 0x53 && (GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_SHIFT) >= 0)) {
	    this->SendMessage(WM_COMMAND, 40006);
	}
	//30. (21.09.07 추가) Ctrl + Shift + S (다른 이름으로 저장) 눌렀을 때
	else if (nChar == 0x53 && (GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_SHIFT) < 0)) {
	    this->SendMessage(WM_COMMAND, 40007);
	}
	//31. (21.09.07 추가) Ctrl + O (열기) 눌렀을 때
	else if (nChar == 0x4F && (GetKeyState(VK_CONTROL) < 0)) {
	    this->SendMessage(WM_COMMAND, 40008);
    }
	//32. (21.09.14 추가) F3 눌렀을 때 (다음 찾기)
	else if (nChar == VK_F3 && (GetKeyState(VK_CONTROL) >= 0) && (GetKeyState(VK_SHIFT) >= 0)) {

		//1. 찾는 내용 없거나 FindDialog가 NULL이면 FindDialog 생성
		if (this->findSameText->findStr == "") { //
			this->SendMessage(WM_COMMAND, 40039);
		}
		//2. 찾는 내용 있으면 바로 FindNext 실행.
		else if (this->findSameText->findStr != "") {
			this->SendMessage(WM_COMMAND, 40040);
		}
    }
	//33. (21.09.16.추가) F3 + SHIFT 눌렀을 때
	else if (nChar == VK_F3 && (GetKeyState(VK_CONTROL) >= 0) && (GetKeyState(VK_SHIFT) < 0)) {
		//1. 찾는 내용 없거나 FindDialog가 NULL이면 FindDialog 생성
		if (this->findSameText->findStr == "") { //
			this->SendMessage(WM_COMMAND, 40039);
		}
		//2. 찾는 내용 있으면 바로 FindNext 실행.
		else if (this->findSameText->findStr != "") {
			this->SendMessage(WM_COMMAND, 40041);
		}
    }
	//34. (21.09.30 추가) Ctrl + N (새로 만들기) 눌렀을 때
	else if (nChar == 0x4E && (GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_SHIFT) >= 0)) {
	    this->SendMessage(WM_COMMAND, 40048);
	}
	//35. (21.09.30 추가) Ctrl + A (전체 선택) 눌렀을 때
	else if (nChar == 0x41 && (GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_SHIFT) >= 0)) {
	    this->SendMessage(WM_COMMAND, 40049);
    }
	//36. (21.09.30 추가) F5 눌렀을 때
	else if (nChar == VK_F5 && (GetKeyState(VK_CONTROL) >= 0) && (GetKeyState(VK_SHIFT) >= 0)) {
	    this->SendMessage(WM_COMMAND, 40050);
    }
	//37. (21.11.21.추가) PageUp 눌렀을 때
	else if (nChar == VK_PRIOR) {
	    this->nSBCode = SB_PAGEUP;
		this->isKeyBoard = TRUE;
	    this->SendMessage(WM_COMMAND, 40012);
    }
	//38. (21.11.21.추가) PageDown 눌렀을 때
	else if (nChar == VK_NEXT) {
	    this->nSBCode = SB_PAGEDOWN;
	    this->isKeyBoard = TRUE;
	    this->SendMessage(WM_COMMAND, 40012);
	}


#if 0
	//37. (21.11.17.추가) Shift + 마우스 왼쪽 버튼 눌렀을 때
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
		//1. Shift 키에서 손을 뗐으면, 드래그 중을 false로 설정한다.
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
	

	//1. SB_LINERIGHT를 클릭했을 때
	if (nSBCode == SB_LINELEFT) {
		MoveAction = factoryProductScroll.MakeHorizon(nSBCode);
		//MoveAction = this->factoryProductScroll->MakeHorizon(nSBCode);
		MoveAction->KeyDown();
	}
	//2. SB_LINELEFT를 클릭했을 때
	else if (nSBCode == SB_LINERIGHT) {
		MoveAction = factoryProductScroll.MakeHorizon(nSBCode);
		//MoveAction = this->factoryProductScroll->MakeHorizon(nSBCode);
		MoveAction->KeyDown();
	}
	//3. SB_PAGELEFT를 클릭했을 때
	else if (nSBCode == SB_PAGELEFT) {
		MoveAction = factoryProductScroll.MakeHorizon(nSBCode);
		//MoveAction = this->factoryProductScroll->MakeHorizon(nSBCode);
		MoveAction->KeyDown();
	}
	//3. SB_PAGERIGHT를 클릭했을 때
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

	//1. SB_LINEUP를 클릭했을 때
	if (nSBCode == SB_LINEUP) {
		MoveAction = factoryProductScroll.MakeVertical(nSBCode);
		//MoveAction = this->factoryProductScroll->MakeVertical(nSBCode);
		MoveAction->KeyDown();
	}
	//2. SB_LINEDOWN을 클릭했을 때
	else if (nSBCode == SB_LINEDOWN) {
		MoveAction = factoryProductScroll.MakeVertical(nSBCode);
		//MoveAction = this->factoryProductScroll->MakeVertical(nSBCode);
		MoveAction->KeyDown();
	}
	//3. SB_PAGEUP을 클릭했을 때
	else if (nSBCode == SB_PAGEUP) {
		MoveAction = factoryProductScroll.MakeVertical(nSBCode);
		MoveAction->KeyDown();
	}
	//4. SB_PAGEDOWN을 클릭했을 때
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
	//2. SB_LINELEFT를 클릭했을 때
	else if (nSBCode == SB_LINEDOWN) {
		MoveAction = factoryProductScroll.Make(nSBCode);
		MoveAction->KeyDown();
	}
	//3. SB_PAGELEFT를 클릭했을 때
	else if (nSBCode == SB_PAGEUP) {
		MoveAction = factoryProductScroll.Make(nSBCode);
		MoveAction->KeyDown();
	}
	//3. SB_PAGERIGHT를 클릭했을 때
	else if (nSBCode == SB_PAGEDOWN) {
		MoveAction = factoryProductScroll.Make(nSBCode);
		MoveAction->KeyDown();
	}
#endif

#endif
}

#if 0
void NotePadForm::OnFindButtonClicked() {

	//1. 찾기 버튼을 클릭했을 때
	  //1.1. 찾기 Dialog를 부른다.
	if (this->findDialog == NULL) {
		this->findDialog = new FindDialog(this);
	}
	this->findDialog->Create(NULL, FR_DOWN, this);

#if 0
	  m_pFRDlg = new CFindReplaceDialog(); // Must be created on the heap
	  m_pFRDlg->Create(TRUE, _T(""), _T(""), FR_DOWN | FR_FINDNEXT| FR_MATCHCASE, this);
	  findNext = m_pFRDlg->FindNext();

	  //1.2. "다음 찾기"버튼을 눌렀을 경우,
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

			  //1.2.1. 기재한 찾을 내용을 확인한다.
			  findStr = m_pFRDlg->GetFindString();
			  this->findSameText->findStr = findStr;

			  //1.2.2. 사용자가 검색 문자열의 대/소문자를 정확히 일치시키는지 확인한다. (구분:TRUE, 안구분:FALSE)
			  matchCase = m_pFRDlg->MatchCase();
			  this->findSameText->matchCase = matchCase;

			  //1.2.3. 사용자가 검색을 아래쪽 방향으로 진행하는지 확인한다 (아랫쪽:TRUE, 위쪽:FALSE)
			  searchDown = m_pFRDlg->SearchDown();
			  this->findSameText->searchDown = searchDown;

			  //1.2.4. 사용자가 전체 단어만 일치시킬지 여부를 확인한다.
			  matchWholeWord = m_pFRDlg->MatchWholeWord();
			  this->findSameText->matchWholeWord = matchWholeWord;

			  //1.2.5. FindSameText클래스의 SearchText를 호출한다.
			  this->findSameText->SearchText();

			  //1.2.6. 같은 단어가 없으면 메세지박스를 출력한다. (""을(를) 찾을 수 없습니다.)
	  }

	// Notify(); // 만약 스크롤 범위 밖에 단어가 있을 경우
	Invalidate();
#endif
}



void NotePadForm::OnReplaceButtonClicked() {

	//2. 바꾸기 버튼을 클릭했을 때
	  //2.1. 바꾸기 Dialog를 부른다.
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

	//(21.11.17.추가) 변경하기 전 좌표를 구해놓는다.
	Long beforeCurrentY = this->note->GetCurrent();
	row = this->note->GetChild(beforeCurrentY);
	Long beforeCurrentX = row->GetCurrent();


	//0. 선택된 단어를 초기화한다.
	this->selectWords->firstX = 0;
	this->selectWords->firstY = 0;
	this->selectWords->endX = 0;
	this->selectWords->endY = 0;

	this->selectWords->isDragging = FALSE;
	this->isShiftClicked = FALSE;
	this->isShifting = FALSE;

	this->ShiftX = 0;
	this->ShiftY = 0;

	//0. 마우스가 영역 밖으로 이동했을 때 WM_MOUSEMOVE를 받고 싶으면 SetCapture, ReleaseCapture를 사용
	this->SetCapture();

	//1. 현재 눌려진 좌표를 구한다.
	//2. y축 구한다.

	currentY = this->getTextSize->GetMouseY(y);
	this->row = this->note->GetChild(currentY);
	currentX = this->getTextSize->GetMouseX(x, this->row);

	if (currentX < 0) {
		currentX = 0;
	}

	//4. 해당 좌표를 현재 캐럿의 위치로 정한다.
	this->note->MoveCurrent(currentY);
	this->row->MoveCurrent(currentX);

	//5. 플래그 설정한다.
	this->isMouseDrag = TRUE;

#if 0
	//6. 선택된 단어를 정한다.
	this->selectWords->firstX = currentX;
	this->selectWords->firstY = currentY;
	this->selectWords->endX = currentX;
	this->selectWords->endY = currentY;
#endif

	//(21.11.17.추가) 만약 Shift키 눌려있으면 해당 좌표까지 선택해준다.
	if (this->isShiftClicked == TRUE) {

		this->selectWords->firstX = this->ShiftX;
		this->selectWords->firstY = this->ShiftY;
		this->selectWords->endX = currentX;
		this->selectWords->endY = currentY;


	}
	else {
		//6. 선택된 단어를 정한다.
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

	//1. 현재 좌표를 구한다.
	currentY = this->getTextSize->GetMouseY(y);
	this->row = this->note->GetChild(currentY);
	currentX = this->getTextSize->GetMouseX(x, this->row);

	//2. leftX를 구한다.
	leftX = this->getTextSize->GetLeftX(currentX, this->row);

	//3. rightX를 구한다.
	rightX = this->getTextSize->GetRightX(currentX, this->row);
	
	//4. rightX를 현재 캐럿의 위치로 이동한다.
	this->row->MoveCurrent(rightX);

	//5. leftX, rightX를 선택 처리한다.
	this->selectWords->firstX = leftX;
	this->selectWords->firstY = currentY;
	this->selectWords->endX = rightX;
	this->selectWords->endY = currentY;

	//6. Notify 조건을 설정한다.
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

	//1. 마우스가 드래그 중이면,
	if (this->isMouseDrag == TRUE) {

		//1.1. 현재 좌표를 구한다.
#if 0
		currentY = this->getTextSize->GetMouseY(y);
		this->row = this->note->GetChild(currentY);
		currentX = this->getTextSize->GetMouseX(x, this->row);
#endif
		//2. y축 구한다.
		if (y >= 0) {
			currentY = this->getTextSize->GetMouseY(y);
		}
		else {
			currentY = 0;
		}
		this->row = this->note->GetChild(currentY);

		//3. x축 구한다.
		if (x > 0) { //x >= 0
			currentX = this->getTextSize->GetMouseX(x, this->row);
		}
		else {
			currentX = this->row->GetLength(); 
		}


		//1.2. 해당 좌표로 캐럿을 이동한다.
		this->note->MoveCurrent(currentY);
		this->row = this->note->GetChild(currentY);
		this->row->MoveCurrent(currentX);

		//1.3. 선택된 좌표를 정한다.
		this->selectWords->endY = currentY;
		this->selectWords->endX = currentX;

		//2. Notify 조건을 설정한다.
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

	//1. 플래그가 FALSE이면,
	if (this->isMouseDrag == FALSE 
		|| (this->selectWords->firstX == this->selectWords->endX && this->selectWords->firstY == this->selectWords->endY)) {
		//1.1. 선택된 단어를 초기화한다.
		this->selectWords->firstX = 0;
		this->selectWords->firstY = 0;
		this->selectWords->endX = 0;
		this->selectWords->endY = 0;

		//1.2. Shift 키에서 손을 뗐으면, 드래그 중을 false로 설정한다.
		this->selectWords->isDragging = FALSE;
		this->isShiftClicked = FALSE;
		this->isShifting = FALSE;

		this->ShiftX = 0;
		this->ShiftY = 0;

		//2.1. 플래그를 변경한다.
		this->isMouseDrag = FALSE;

		//2.2. 캡쳐를 풀어준다.
		ReleaseCapture();

	}
	
	//2. 플래그가 TRUE이면,
	else {
		//2.1. 플래그를 변경한다.
		this->isMouseDrag = FALSE;
		this->selectWords->isDragging = TRUE;

		//2.2. 캡쳐를 풀어준다.
		ReleaseCapture();
	}

}

BOOL NotePadForm::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {

	int currentX;
	int currentY;
	int sizeCurrentX;
	int sizeCurrentY;

	//1. 최대 caretHeight를 구한다.
	int strSizeY = this->getTextSize->GetX(131);
	int length = this->note->GetLength();
	Long caretHeight = length * strSizeY;
	Long biggestNPos;

	//2. 최대 caretHeight이 현재 윈도우보다 큰 경우,
	if (caretHeight > this->scroll->windowHeight) {

		//2.0. biggestNPos를 구한다.
		biggestNPos = caretHeight - this->scroll->windowHeight;

		//2.1. 현재 vertical의 nPos에 입력받은 마우스휠의 이동거리를 더한다.
		this->scroll->siVertical.nPos = this->scroll->siVertical.nPos - zDelta;

		//2.2. nPos가 0보다 작으면 0을 넣는다.
		if (this->scroll->siVertical.nPos < 0) {
			this->scroll->siVertical.nPos = 0;
		}
		//2.3. nPos가 최대 caretHeight보다 큰 경우 최대 caretHeight을 넣는다.
		else if (this->scroll->siVertical.nPos > biggestNPos) { //caretHeight
				this->scroll->siVertical.nPos = biggestNPos;
		}

		//3. nPos를 set한다.
		this->SetScrollInfo(SB_VERT, &this->scroll->siVertical, TRUE);

		this->ScrollWindow(0, this->scroll->siVertical.nPos, NULL, NULL);
		this->UpdateWindow();
		this->location->MoveCaret();
		Invalidate();
	}

	return FALSE;
}

void NotePadForm::OnRButtonDown(UINT nFlags, CPoint point) {

	//1. 마우스 우클릭 했을 시
	//1.1. 컨텍스트 메뉴를 생성합니다.
	CMenu menu;
	menu.CreatePopupMenu();

	//1.2. 메뉴를 추가합니다.
	menu.AppendMenu(MF_STRING, 40002, "실행 취소(&U)"); //MF_GRAYED , 40002
	//HMENU unexecute = menu.GetSafeHmenu();
	//menu.Attach(unexecute);

	menu.AppendMenu(MF_SEPARATOR);
	menu.AppendMenu(MF_STRING, 40037, "잘라내기(&T)");
	menu.AppendMenu(MF_STRING, 40035, "복사(&C)");
	menu.AppendMenu(MF_STRING, 40036, "붙여넣기(&P)");
	menu.AppendMenu(MF_STRING, 40038, "삭제(&D)");
	menu.AppendMenu(MF_SEPARATOR);
	menu.AppendMenu(MF_STRING, 40049, "모두 선택(&A)");

	//1.3. 비활성화 시킬 메뉴는 비활성화 시킨다.
	//1.3.1. 실행취소 메뉴관련, Undo 리스트에 한개라도 있으면 활성화, 없으면 비활성화 시킨다.
	if (this->commandStory->undoList->IsEmpty() == TRUE) {
		menu.EnableMenuItem(40002, MF_BYCOMMAND | MF_DISABLED | MF_ENABLED);
	}
	//1.3.2. 잘라내기, 복사, 삭제 메뉴관련, 선택된 단어가 있으면 활성화, 없으면 비활성화 시킨다.
	if (this->selectWords->firstX <= 0 && this->selectWords->firstY <= 0 && this->selectWords->endX <= 0 
		&& this->selectWords->endY <= 0) {
		menu.EnableMenuItem(40037, MF_BYCOMMAND | MF_DISABLED | MF_ENABLED);
		menu.EnableMenuItem(40035, MF_BYCOMMAND | MF_DISABLED | MF_ENABLED);
		menu.EnableMenuItem(40038, MF_BYCOMMAND | MF_DISABLED | MF_ENABLED);
	}
	//1.3.3. 붙여넣기 메뉴관련, 클립보드가 안비어있으면 활성, 비어있으면 비활성화 시킨다.
      //1.3.3.1. 클립보드 핸들을 연다.
	  if (OpenClipboard()) {
		  //1.3.3.1.1. 클립보드에 텍스트 데이터가 없으면
		  if (!IsClipboardFormatAvailable(CF_TEXT)) {
			  menu.EnableMenuItem(40036, MF_BYCOMMAND | MF_DISABLED | MF_ENABLED);
		  }
	  }
   //1.3.4. 모두선택 메뉴관련, 모두 선택되어 있으면 비활성, 선택 안되어있으면 활성화 시킨다.
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
	
	pSubMenu->EnableMenuItem(40002, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);    //비활성화
#endif

	//1.4. 컨텍스트 메뉴를 x,y 좌표에 출력합니다.
	POINT pt = { 0 };
	::GetCursorPos(&pt); // 마우스 커서의 좌표를 얻은 후

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

	//3. 닫기 버튼 클릭했을 때
	 //3.1. 현재 메모장의 타이틀을 구한다.
	GetWindowText(title);

	// title에 *가 포함되어있으면 제거한다.
	if (title.Find("*") == 0) {
		title.Replace("*", "");
	}

	CString titleWithTxt;
	titleWithTxt.Format("%s%s", title, ".txt");


	//3.2. 해당 파일이 존재하고, 내용 변화가 있으면,
	CFileFind pFind;
	if (this->note != NULL) {
		row = this->note->GetChild(0);
	}

	ret = pFind.FindFile(titleWithTxt);
	if (ret == TRUE && this->isChanged == TRUE) {
		//3.2.1. 메세지를 띄운다.
		str.Format("%s%s%s", "변경내용을 ", title, "에 저장하시겠습니까?");
		ret_ = SaveMessageBox(this->GetSafeHwnd(), str, "메모장", MB_YESNOCANCEL);
		//ret_ = MessageBox(str, "메모장", MB_YESNOCANCEL);


		//3.2.2. YES를 누를 경우, 저장한다.
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

	//3.3. 해당 파일이 존재하지 않고 글자가 하나라도 있으면,
	else if (ret == FALSE && (row->GetLength() > 0 || this->note->GetLength() > 1)) {

		//3.3.1. 메세지를 띄운다.
		str.Format("%s%s%s", "변경내용을 ", title, "에 저장하시겠습니까?");
		//ret_ = MessageBox(str, "메모장", MB_YESNOCANCEL);//MessageBox(str, "메모장", MB_YESNOCANCEL);
		ret_ = SaveMessageBox(this->GetSafeHwnd(), str, "메모장", MB_YESNOCANCEL); //MessageBox(str, "메모장", MB_YESNOCANCEL);

		//3.3.2. YES를 누를 경우,
		if (ret_ == IDYES) {
			//2.1. 필터를 만든다.
			LPCTSTR fileFilter(_T("모든파일|*.*|텍스트문서(*.txt)|*.txt||"));

			//2.2. 공통대화상자(다른이름으로 저장)를 만든다.
			CFileDialog dlg(FALSE, _T("txt"), _T("*.txt"), OFN_HIDEREADONLY, fileFilter, NULL, 0, NULL);
			//OFN_HIDEREADONLY 한 개의 파일 선택할 경우

			//2.3. 공통대화상자(다른이름으로 저장)를 띄운다.
			//2.4. 사용자가 저장할 파일을 정상적으로 선택하였을때, 
			if (IDOK == dlg.DoModal()) {
				//2.4.1. strPathName에 지정할 파일의 경로 저장한다.
				strPathName = dlg.GetPathName();

				//2.4.2. 선택한 파일의 이름을 갖고 온다.
				fileName = dlg.GetFileName();//선택된 파일의 이름과 확장자
				fileNameWithouttxt = dlg.GetFileTitle();

				//MessageBox(title);

				//2.4.3. 파일 이름이 중복되면 
				if (PathFileExists(fileName)) {
					str.Format("%s%s%s", fileName, "이(가) 이미 있습니다.\n", "바꾸시겠습니까?");
					ret = MessageBox(str, _T("다른 이름으로 저장 확인"), MB_YESNO);

					if (ret == IDYES) {// Yes 눌렀을 경우, 해당 파일에 덮어쓰기한다.
						//기존에 있던 해당 이름의 파일을 삭제한다.
						//remove(fileName);

						this->note->Save((LPCTSTR)fileName);

						//입력한 이름으로 변경한다
						SetWindowText(fileNameWithouttxt);
					}
				}
				//2.4.4. 파일 이름이 중복되지 않으면,
				else {
					this->note->Save((LPCTSTR)fileName);
					//입력한 이름으로 변경한다.
					SetWindowText(fileNameWithouttxt);
				}
			}
			//2.5. 사용자가 파일 선택 안했다면 strPathName 초기화하고 저장X.
			else {
				strPathName = "";
			}
			//isClose = TRUE;
		}
		else if (ret_ == IDCANCEL) {
			isClose = FALSE;
		}

	}

	//(21.10.25.추가) 레지스트리에 닫기 전 윈도우의 마지막 위치를 저장시킨다.
	//현재 위치를 구한다.
	CRect rect;
	GetWindowRect(rect);

	//현재 위치를 레지스트리에 저장한다.
	this->pApp->WriteProfileInt("NotePadSection", "WindowLeft", rect.left);
	this->pApp->WriteProfileInt("NotePadSection", "WindowTop", rect.top);

	//(21.10.25.추가) 레지스트리에 닫기 전 윈도우의 마지막 크기를 저장시킨다.
	//현재 크기를 구한다.
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

	//3. 열기버튼 클릭했을 때
	 //3.1. 현재 메모장의 타이틀을 구한다.
	GetWindowText(title);
	CString titleWithTxt;
	titleWithTxt.Format("%s%s", title, ".txt");


	//3.2. 해당 파일이 존재하고, 내용 변화가 있으면,
	CFileFind pFind;
	Glyph* row = this->note->GetChild(0);


	ret = pFind.FindFile(titleWithTxt);
	if (ret == TRUE && this->isChanged == TRUE) {
		//3.2.1. 메세지를 띄운다.
		str.Format("%s%s%s", "변경내용을 ", title, "에 저장하시겠습니까?");
		ret_ = SaveMessageBox(this->GetSafeHwnd(), str, "메모장", MB_YESNOCANCEL);
		//ret_ = MessageBox(str, "메모장", MB_YESNOCANCEL);


		//3.2.2. YES를 누를 경우, 저장한다.
		if (ret_ == IDYES) {
			this->note->Save((LPCTSTR)titleWithTxt);
		}
	}

	//3.3. 해당 파일이 존재하지 않고 글자가 하나라도 있으면,
	else if (ret == FALSE && (row->GetLength() > 0 || this->note->GetLength() > 1)) {
		//3.3.1. 메세지를 띄운다.
		str.Format("%s%s%s", "변경내용을 ", title, "에 저장하시겠습니까?");
		//ret_ = MessageBox(str, "메모장", MB_YESNOCANCEL);//MessageBox(str, "메모장", MB_YESNOCANCEL);
		ret_ = SaveMessageBox(this->GetSafeHwnd(), str, "메모장", MB_YESNOCANCEL); //MessageBox(str, "메모장", MB_YESNOCANCEL);

		//3.3.2. YES를 누를 경우,
		if (ret_ == IDYES) {
			//2.1. 필터를 만든다.
			LPCTSTR fileFilter(_T("모든파일|*.*|텍스트문서(*.txt)|*.txt||"));

			//2.2. 공통대화상자(다른이름으로 저장)를 만든다.
			CFileDialog dlg(FALSE, _T("txt"), _T("*.txt"), OFN_HIDEREADONLY, fileFilter, NULL, 0, NULL);
			//OFN_HIDEREADONLY 한 개의 파일 선택할 경우

			//2.3. 공통대화상자(다른이름으로 저장)를 띄운다.
			//2.4. 사용자가 저장할 파일을 정상적으로 선택하였을때, 
			if (IDOK == dlg.DoModal()) {
				//2.4.1. strPathName에 지정할 파일의 경로 저장한다.
				strPathName = dlg.GetPathName();

				//2.4.2. 선택한 파일의 이름을 갖고 온다.
				fileName = dlg.GetFileName();//선택된 파일의 이름과 확장자
				fileNameWithouttxt = dlg.GetFileTitle();

				//MessageBox(title);

				//2.4.3. 파일 이름이 중복되면 
				if (PathFileExists(fileName)) {
					str.Format("%s%s%s", fileName, "이(가) 이미 있습니다.\n", "바꾸시겠습니까?");
					ret = MessageBox(str, _T("다른 이름으로 저장 확인"), MB_YESNO);

					if (ret == IDYES) {// Yes 눌렀을 경우, 해당 파일에 덮어쓰기한다.
						//기존에 있던 해당 이름의 파일을 삭제한다.
						//remove(fileName);

						this->note->Save((LPCTSTR)fileName);

						//입력한 이름으로 변경한다
						SetWindowText(fileNameWithouttxt);
					}
				}
				//2.4.4. 파일 이름이 중복되지 않으면,
				else {
					this->note->Save((LPCTSTR)fileName);
					//입력한 이름으로 변경한다.
					SetWindowText(fileNameWithouttxt);
				}
			}
			//2.5. 사용자가 파일 선택 안했다면 strPathName 초기화하고 저장X.
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
			msg = "저장(&S)";
			SetWindowText(GetDlgItem(hChildWnd, IDYES), msg);
		}

		if (GetDlgItem(hChildWnd, IDNO) != NULL) {
			msg = "저장 안 함(&N)";
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
















