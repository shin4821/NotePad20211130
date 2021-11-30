//OnImeCompositionCommand.cpp
#include"OnImeCompositionCommand.h"
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
#include"LineChange.h"
#include"Scroll.h"
#include"KeyAction.h"
#include"BackSpaceKeyAction.h"
#include"FactoryProduct.h"
#include"CommandStory.h"
#include"UndoList.h"
#include"RedoList.h"
#include"resource.h"
#include"Font.h"
#include"DummyRow.h"
#include"GetTextSize.h"
using namespace std;


OnImeCompositionCommand::OnImeCompositionCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {

	this->isChecked = false;
	this->isLineChanged = false;
	this->currentX = 0;
	this->currentY = 0;
	//this->character[2] = '\0';
	this->notePadForm->isRedo = false;
}

OnImeCompositionCommand::~OnImeCompositionCommand() {
}

void OnImeCompositionCommand::Execute() {

	char letter[3] = { '\0', };
	string fontName = this->notePadForm->font->GetFontName();
	int fontSize = this->notePadForm->font->GetFontSize();
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
	BOOL isLastOneDeleted = FALSE;

	//(21.11.10.추가)
	if (this->notePadForm->isOpen == TRUE) {
		this->notePadForm->isOpenChanged = TRUE;
	}

#if 0
	letter[0] = HIBYTE(LOWORD(this->notePadForm->letter));
	letter[1] = LOBYTE(LOWORD(this->notePadForm->letter));
	letter[2] = '\0';
#endif

	if (this->notePadForm->letter != "") {
		letter[0] = this->notePadForm->letter.at(0);
		letter[1] = this->notePadForm->letter.at(1);
		letter[2] = '\0';
	}
	else {
		this->notePadForm->isFirstComposing = FALSE; //널문자일때 모두 지운 상태이니까 FALSE 처리.
		isLastOneDeleted = TRUE;
	}

#if 0
	//(21.09.14 추가) Delete할때 null 들어오면 X.
	if (letter[0] != '\0') {
		this->character = letter;

		CreateGlyph* creator = new CreateProduct();
		Glyph* character = creator->Make(letter);

		//21.08.03 추가
		//this->scroll->windowWidth = this->m_rectCurHist.right - this->m_rectCurHist.left;
		//this->scroll->windowHeight = this->m_rectCurHist.bottom - this->m_rectCurHist.top;


		//1. 조합중일 때
		  //1.1. 현재 캐럿의 위치를 확인한다.
		int currentY = this->notePadForm->note->GetCurrent();
		this->notePadForm->row = this->notePadForm->note->GetChild(currentY);
		int currentX = this->notePadForm->row->GetCurrent();
		length = this->notePadForm->row->GetLength();


		//Command의 멤버 설정한다. (21.09.06)*********************************************************

		//1. 현재 캐럿의 위치를 멤버에 설정한다.
		this->currentX = currentX;
		this->currentY = currentY;
		//********************************************************************************************
		this->notePadForm->isFirstComposingForCommand = this->notePadForm->isFirstComposing;

		//2. 자동개행버튼이 눌려진 경우,
		if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

			//2.1. 자동개행 전 캐럿의 위치를 찾는다.
			this->notePadForm->lineChange->FindCaretBeforeLineChange(&currentX, &currentY);

			//2.2. 자동개행 된 문서를 펼친다.
			this->notePadForm->lineChange->LineChangeButtonNotClicked();

			this->notePadForm->row = this->notePadForm->note->GetChild(currentY);
			length = this->notePadForm->row->GetLength();
		}

		//3. 새로 시작한 글자 외에 기존 글자를 지운다.
		if (this->notePadForm->isFirstComposing == FALSE) {
			this->notePadForm->row->Remove(currentX); //this->index
		}

		//4. currentX가 row의 length-1보다 작거나, currentX가 length-1보다 크거나 같고, 커서 크기가 3인 경우,
		if (currentX < length - 1) {
			//|| (currentX >= length - 1 && this->caretWidth == 3) 잠시뺐음. 다시 껴야함.

			//4.1. 현재 줄에 새로 글자를 끼워적는다.
			this->notePadForm->row->Add(currentX, character);
		}

		//5. currentX가 row의 length보다 크거나 같거나, currentX가 length-1보다 크거나 같고, 커서 크기가 3보다 큰 경우,
		else if (currentX >= length || (currentX >= length - 1 && this->notePadForm->caretWidth > 3)) {
			//5.1. 현재 줄에 새로 글자를 붙혀적는다.
			this->notePadForm->row->Add(character);
		}

		//6.  자동개행 버튼이 눌려진 경우,
		if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
			//6.1. 펼친 문서를 다시 자동개행한다. (ON)
			this->notePadForm->lineChange->LineChangeButtonClicked();

			//6.2. 자동개행 후 이동할 캐럿의 위치를 구한다.
			this->notePadForm->lineChange->FindCaretAfterLineChange(currentX, currentY);
		}

		//7. 좌표 이동하지 않고, 커서 크기만 바꿔준다.
		if (this->notePadForm->isFirstComposing == TRUE) {

			this->notePadForm->isOnImeCharForMove = FALSE;
			this->notePadForm->isFirstComposingForMove = this->notePadForm->isFirstComposing;

			// 추후 지워주기
			this->notePadForm->isDoubleByte = TRUE;
			this->notePadForm->isJumpOverForPrevious = FALSE;
			this->notePadForm->isJumpOverForNext = FALSE;
			this->notePadForm->isUp = FALSE;
			this->notePadForm->isDown = FALSE;


			this->notePadForm->Notify();
		}


		this->notePadForm->isFirstComposing = FALSE;
		this->notePadForm->isChanged = TRUE;

		this->notePadForm->Invalidate();
	}
#endif

//#if 0
	this->character = letter;

	//CreateGlyph* creator = new CreateProduct();
	CreateProduct creator;
	Glyph* character;

	//21.08.03 추가
	//this->scroll->windowWidth = this->m_rectCurHist.right - this->m_rectCurHist.left;
	//this->scroll->windowHeight = this->m_rectCurHist.bottom - this->m_rectCurHist.top;


	//1. 조합중일 때
	  //1.1. 현재 캐럿의 위치를 확인한다.
	int currentY = this->notePadForm->note->GetCurrent();
	this->notePadForm->row = this->notePadForm->note->GetChild(currentY);
	int currentX = this->notePadForm->row->GetCurrent();
	length = this->notePadForm->row->GetLength();


	//Command의 멤버 설정한다. (21.09.06)*********************************************************

    //1. 현재 캐럿의 위치를 멤버에 설정한다.
	this->currentX = currentX;
	this->currentY = currentY;
	//********************************************************************************************
	this->notePadForm->isFirstComposingForCommand = this->notePadForm->isFirstComposing;

	//2. 자동개행버튼이 눌려진 경우,
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		//2.1. 자동개행 전 캐럿의 위치를 찾는다.
		this->notePadForm->lineChange->FindCaretBeforeLineChange(&currentX, &currentY);

		//2.2. 자동개행 된 문서를 펼친다.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

		this->notePadForm->row = this->notePadForm->note->GetChild(currentY);
		length = this->notePadForm->row->GetLength();
	}

	//3. 새로 시작한 글자 외에 기존 글자를 지운다.
	if (this->notePadForm->isFirstComposing == FALSE) {
		this->notePadForm->row->Remove(currentX); //this->index
	}

	//4. currentX가 row의 length-1보다 작거나, currentX가 length-1보다 크거나 같고, 커서 크기가 3인 경우,
	if (currentX < length && letter[0]!='\0') { //currentX < length - 1
		//|| (currentX >= length - 1 && this->caretWidth == 3) 잠시뺐음. 다시 껴야함.
		character = creator.Make(letter);

		//4.1. 현재 줄에 새로 글자를 끼워적는다.
		this->notePadForm->row->Add(currentX, character);
	}

	//5. currentX가 row의 length보다 크거나 같거나, currentX가 length-1보다 크거나 같고, 커서 크기가 3보다 큰 경우,
	else if (currentX >= length || (currentX >= length - 1 && this->notePadForm->caretWidth > 3) && letter[0] != '\0') {
		character = creator.Make(letter);
		//5.1. 현재 줄에 새로 글자를 붙혀적는다.
		this->notePadForm->row->Add(character);
	}

	//6.  자동개행 버튼이 눌려진 경우,
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
		//6.1. 펼친 문서를 다시 자동개행한다. (ON)
		this->notePadForm->lineChange->LineChangeButtonClicked();


		//(21.10.28.추가)아예 지울때랑 추가할때 구분해서 캐럿의 위치를 구해야겠다.
		if (this->notePadForm->letter != "") { //추가하는 경우,

			//6.2. 자동개행 후 이동할 캐럿의 위치를 구한다.
			this->notePadForm->lineChange->FindCaretAfterLineChangeForFirst(currentX, currentY);
		}
		else { //빼는 경우,

			//6.3. 자동개행 후 이동할 캐럿의 위치를 구한다.
			this->notePadForm->lineChange->FindCaretAfterLineChange(currentX, currentY);
		}


	}

	//7. 좌표 이동하지 않고, 커서 크기만 바꿔준다. 
	if (this->notePadForm->isFirstComposing == TRUE) {
#if 0

		//7.0. (21.10.01 추가) 자동개행일 경우, 좌표 옮겨준다.*******************************
		if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
			this->notePadForm->note->MoveCurrent(currentY);
			row = this->notePadForm->note->GetChild(currentY);
			row->MoveCurrent(currentX);
		}
		//***********************************************************************************
#endif
		this->notePadForm->isOnImeCharForMove = FALSE;
		this->notePadForm->isFirstComposingForMove = this->notePadForm->isFirstComposing;

		//7.1. 자음 하나 남았을 때 지운 경우, isFirstComposingForMove를 FALSE로 돌려준다.
#if 0
		if (this->notePadForm->letter != "") {
			this->notePadForm->isFirstComposingForMove = FALSE;
		}
#endif

		//7.2. Notify 조건 설정한다.
		this->notePadForm->isDoubleByte = TRUE;
		this->notePadForm->isJumpOverForPrevious = FALSE;
		this->notePadForm->isJumpOverForNext = FALSE;
		this->notePadForm->isUp = FALSE;
		this->notePadForm->isDown = FALSE;

		int testCurrentY = this->notePadForm->note->GetCurrent();
		Glyph* testRow = this->notePadForm->note->GetChild(testCurrentY);
		int testCurrentX = testRow->GetCurrent();


		this->notePadForm->isComposition = TRUE;
		this->notePadForm->Notify();
	}
//#if 0
	else if (isLastOneDeleted == TRUE) {
		
		this->notePadForm->isOnImeCharForMove = FALSE;
		this->notePadForm->isFirstComposingForMove = FALSE;
		this->notePadForm->isDoubleByte = FALSE;

		//7.2. Notify 조건 설정한다.
		this->notePadForm->isDoubleByte = TRUE;
		this->notePadForm->isJumpOverForPrevious = FALSE;
		this->notePadForm->isJumpOverForNext = FALSE;
		this->notePadForm->isUp = FALSE;
		this->notePadForm->isDown = FALSE;

		this->notePadForm->Notify();
	}
//#endif

	//(21.10.01 추가)
	if (letter[0] == '\0') { //letter이 null인 경우, 지워줄때이므로 isFirstComposing을 TRUE로 해야 다시 쓸때 안지워짐.
		this->notePadForm->isFirstComposing = TRUE;
		this->notePadForm->isDoubleByte = FALSE;
	}
	else {
		this->notePadForm->isFirstComposing = FALSE;
	}

	this->notePadForm->isChanged = TRUE;

	this->notePadForm->Invalidate();
//#endif


#if 0
	//1.2. 새로 시작한 글자 외에 기존 글자를 지운다.
	if (this->notePadForm->isFirstComposing == FALSE) {
		this->notePadForm->row->Remove(currentX); //this->index
	}

	//1.3. (21.08.12 추가) 자동개행버튼 OFF일 경우,
	if (this->notePadForm->isLineChangeButtonClicked == FALSE) {
		//1.3. currentX가 row의 length-1보다 작거나, currentX가 length-1보다 크거나 같고, 커서 크기가 3인 경우,
		if (currentX < length - 1) {
			//|| (currentX >= length - 1 && this->caretWidth == 3) 잠시뺐음. 다시 껴야함.

			//1.3.1. 현재 줄에 새로 글자를 끼워적는다.
			this->notePadForm->row->Add(currentX, character);

			this->notePadForm->isInsert = TRUE;
			this->notePadForm->isAppend = FALSE;
			this->notePadForm->isMoveActionVertical = FALSE;
			this->notePadForm->isMoveActionHorizon = FALSE;
			this->notePadForm->isDeleteAction = FALSE;
		}

		//1.4. currentX가 row의 length보다 크거나 같거나, currentX가 length-1보다 크거나 같고, 커서 크기가 3보다 큰 경우,
		else if (currentX >= length || (currentX >= length - 1 && this->notePadForm->caretWidth > 3)) {
			//1.4.1. 현재 줄에 새로 글자를 붙혀적는다.
			this->notePadForm->row->Add(character);

			this->notePadForm->isInsert = FALSE;
			this->notePadForm->isAppend = TRUE;
			this->notePadForm->isMoveActionVertical = FALSE;
			this->notePadForm->isMoveActionHorizon = FALSE;
		}

		//1.5. 좌표 이동하지 않고, 커서 크기만 바꿔준다.
		if (this->notePadForm->isFirstComposing == TRUE) {

			this->notePadForm->isOnImeCharForMove = FALSE;
			this->notePadForm->isFirstComposingForMove = this->notePadForm->isFirstComposing;

			this->notePadForm->Notify();
		}

	}

	//1.4. (21.08.12 추가) 자동개행버튼 ON일 경우,
	else if (this->notePadForm->isLineChangeButtonClicked == TRUE) {
		//1.4.0. 끼워적다의 경우,
		if (currentY + 1 < this->notePadForm->note->GetLength()) {
			dummyRow = this->notePadForm->note->GetChild(currentY + 1);
		}
		else {
			dummyRow = NULL;
		}
		//1.4.1. currentX가 row의 length-1보다 작거나, currentX가 length-1보다 크거나 같고, 커서 크기가 3인 경우,(끼워적다)
		if (currentX < length || (dummyRow != NULL && dynamic_cast<DummyRow*>(dummyRow))) { //currentX < length - 1

			row = this->notePadForm->note->GetChild(currentY);

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

			exceptWidth = this->notePadForm->getTextSize->totalCalculateX(row->GetLength(), row);
			if (isLineChanged == TRUE && exceptWidth <= this->notePadForm->scroll->windowWidth) {
				isLineChanged = FALSE;
			}
			//**********************************************************************

			//1.4.1.3. 새로 시작한 글자일 경우,
			if (this->notePadForm->isFirstComposing == TRUE) {

				//해당 row에서 자를 위치가 0보다 크거나 같은동안 반복한다.
				location = this->notePadForm->lineChange->FindSplitLocationByWriting(row);

				i = currentY;
				while (location >= 0) {
					//1. 해당 row의 다음 row가 dummyRow가 아니거나 없을 경우,dummyRow 만든다.
					if (i + 1 < this->notePadForm->note->GetLength()) {

						dummyRow = this->notePadForm->note->GetChild(i + 1);

						if (!dynamic_cast<DummyRow*>(dummyRow)) {
							dummyRow = new DummyRow();
							this->notePadForm->note->Add(i + 1, dummyRow);
						}
					}
					else {

						dummyRow = new DummyRow();
						this->notePadForm->note->Add(dummyRow);
					}

					//2. SplitForFront를 호출한다.
					dummyRow = row->SplitForFront(dummyRow, location);

					//3. 출력받은 dummyRow에서 자를 위치가 있는지 찾는다.
					location = this->notePadForm->lineChange->FindSplitLocationByWriting(dummyRow);
					row = dummyRow;
					i++;
				}

				//#if 0
								  // 캐럿 좌표를 설정한다.
				if (isLineChanged == TRUE) { //줄이 바꼈으면,
					this->notePadForm->note->MoveCurrent(currentY + 1);
					this->notePadForm->row = this->notePadForm->note->GetChild(currentY + 1);
					this->notePadForm->row->MoveCurrent(0); // 한글이므로 0으로 설정.

				}
				//#endif
			}

		}

		//1.4. currentX가 row의 length보다 크거나 같거나, currentX가 length-1보다 크거나 같고, 커서 크기가 3보다 큰 경우,
		else if (currentX >= length || (currentX >= length - 1 && this->notePadForm->caretWidth > 3)) {
			//1.4.1. 현재 줄에 새로 글자를 붙혀적는다.
			this->notePadForm->row->Add(character);

			//1.4.2. 새로 시작한 글자의 경우,
			if (this->notePadForm->isFirstComposing == TRUE) {
				//1.4.2.1. 해당 row에서 자를 위치가 0보다 크거나 같은 경우,
				location = this->notePadForm->lineChange->FindSplitLocationByWriting(this->notePadForm->row);
				if (location >= 0) {
					//1. dummyRow를 만든다.
					dummyRow = new DummyRow();

					//2. dummyRow에 추가한다.
					character = this->notePadForm->row->GetChild(location);
					dummyRow->Add(character);

					//3. currentY+1의 노트에 끼운다.
					this->notePadForm->note->Add(currentY + 1, dummyRow);

					//4. 기존 row에서 삭제
					this->notePadForm->row->Remove(location);

					//5. 캐럿을 (currentY+1, 0)으로 좌표 설정한다.
					this->notePadForm->note->MoveCurrent(currentY + 1);
					dummyRow->MoveCurrent(0);
				}
			}
		}

		//1.5. 좌표 이동하지 않고, 커서 크기만 바꿔준다.
		if (this->notePadForm->isFirstComposing == TRUE) {
			// 스크롤 설정
			this->notePadForm->isInsert = FALSE;
			this->notePadForm->isAppend = FALSE;
			this->notePadForm->isMoveActionVertical = FALSE;
			this->notePadForm->isMoveActionHorizon = FALSE;
			this->notePadForm->isDeleteAction = FALSE;

			//캐럿 좌표 설정
			this->notePadForm->isOnImeCharForMove = FALSE;
			this->notePadForm->isFirstComposingForMove = this->notePadForm->isFirstComposing;

			this->notePadForm->Notify();
		}
	}

	this->notePadForm->isFirstComposing = FALSE;
	this->notePadForm->isChanged = TRUE;

	this->notePadForm->Invalidate();
#endif
}

void OnImeCompositionCommand::Undo() {
	KeyAction* keyAction = NULL;
	FactoryProduct factoryProduct(this->notePadForm);

	keyAction = factoryProduct.Make(VK_BACK, NULL, NULL);

	//현재 캐럿의 위치를 변경해준다.
	Long currentX = this->currentX;
	Long currentY = this->currentY;

	this->notePadForm->note->MoveCurrent(currentY);
	Glyph* row = this->notePadForm->note->GetChild(currentY);
	row->MoveCurrent(currentX);

	keyAction->KeyDown();
}

void OnImeCompositionCommand::Redo() {
	//현재 캐럿의 위치를 변경해준다.
	Long currentX = this->currentX;
	Long currentY = this->currentY;

	this->notePadForm->note->MoveCurrent(currentY);
	Glyph* row = this->notePadForm->note->GetChild(currentY);
	row->MoveCurrent(currentX);

	Execute();
}

bool OnImeCompositionCommand::GetChecked() {
	return this->isChecked;

}

Long OnImeCompositionCommand::GetCurrentX() {
	return this->currentX;
}

Long OnImeCompositionCommand::GetCurrentY() {
	return this->currentY;
}

void OnImeCompositionCommand::SetCheck() {
	this->isChecked = true;
}

bool OnImeCompositionCommand::GetLineChanged() {
	return this->isLineChanged;
}

#if 0
char OnImeCompositionCommand::GetCharacter() {
	return this->notePadForm->letter[0];
}
#endif

string OnImeCompositionCommand::GetCharacterForDoubleByte() {
	return this->character;
}













