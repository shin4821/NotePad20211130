//Scroll.cpp
#include"Scroll.h"
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
#include"Thumb.h"
#include"LineChange.h"
#pragma warning(disable:4996)

Scroll::Scroll(NotePadForm* notePadForm) {
	this->notePadForm = notePadForm;
	this->beforeX = 0;
	this->beforeY = 0;
	this->beforePageX = 0;
	this->beforePageY = 0;
	this->beforeCurrentY = 0;

	//1. 윈도우의 크기 구한다.
	this->notePadForm->GetClientRect(this->notePadForm->m_rectCurHist);
	this->windowWidth = this->notePadForm->m_rectCurHist.right - this->notePadForm->m_rectCurHist.left;
	this->beforeWindowWidth = this->windowWidth;
	this->windowHeight = this->notePadForm->m_rectCurHist.bottom - this->notePadForm->m_rectCurHist.top;

}
Scroll::~Scroll() {
}

#if 0
void Scroll::MovePageAndThumb() {
	SCROLLINFO siHorizon;
	SCROLLINFO siVertical;
	Long length = 0;
	Long i = 0;
	Long j = 0;
	Long noteLength;
	Long rowLength;
	Glyph* character;
	string strCharacter;
	char chaCharacter[3];
	Long pageX = 0;
	Long pageY = 0;
	int totalCaretX;
	int totalCaretY;
	int actualCaretX;
	int currentLargestWidth = 0;
	int newNPos;
	int caretX_;
	int height;
	int caretHeight;


	//1. 윈도우의 크기 구한다.
	this->notePadForm->GetClientRect(this->notePadForm->m_rectCurHist);
	//this->GetWindowRect(m_rectCurHist);
	this->windowWidth = this->notePadForm->m_rectCurHist.right - this->notePadForm->m_rectCurHist.left;
	this->windowHeight = this->notePadForm->m_rectCurHist.bottom - this->notePadForm->m_rectCurHist.top;

	//2. 캐럿의 크기 구한다.
	int strSizeY = this->notePadForm->getTextSize->GetX(131);

	//Scroll 다음에 Location이 호출되므로 caretX, caretY는 일일이 구해줘야한다.....-----------------------------
	  //1.2. 현재 note의 current 확인한다.
	Long currentY = this->notePadForm->note->GetCurrent();
	noteLength = this->notePadForm->note->GetLength();
	//1.3. 현재 note의 row를 찾는다.
	Glyph* row = this->notePadForm->note->GetChild(currentY);
	//1.4. 현재 row의 current 확인한다.
	Long currentX = row->GetCurrent();

	if (this->notePadForm->isLineChangeButtonClicked == FALSE) {
		this->notePadForm->location->caret->x = 0;
		i = 0;
		while (i < currentX) {
			//2.6.1. i번째 문자를 얻는다.
			character = row->GetChild(i);
			strCharacter = character->GetContent();
			strcpy(chaCharacter, strCharacter.c_str());

			if (chaCharacter[0] >= 0 && chaCharacter[0] < 128) {
				chaCharacter[1] = '\0';
			}
			else {
				chaCharacter[2] = '\0';
			}

			//2.6.2. Location의 CalculateX를 호출한다.
			this->notePadForm->getTextSize->CalculateX(chaCharacter);
			//CalculateX(chaCharacter);
			i++;
		}
	}

	int caretX = this->notePadForm->location->caret->GetX(); //최근까지의 너비 (붙혀적다, 끼워적다 공용, 커서까지의)
	int caretY = currentY; //최근까지의 높이
	caretHeight = (caretY + 1) * strSizeY;
	//------------------------------------------------------------------------------------------------------------



	if (this->notePadForm->isMoveActionHorizon == TRUE && this->notePadForm->isMoveActionVertical == FALSE) {
		//------------------------------------------------------------------------------------------------------------
        //5. 바뀐 Horizon 좌표를 Get한다.
		//this->notePadForm->GetScrollInfo(SB_HORZ, &this->siHorizon, SIF_POS);

		//5.1. 이전 Horizon좌표와 바뀐 Horizon좌표가 다르면 윈도우를 스크롤한다.
		//Horizon 스크롤 활성화를 결정한다.
		if (this->siHorizon.nPos != this->beforeX) {
			this->notePadForm->xTextOut = this->siHorizon.nPos * -1;
			this->notePadForm->ScrollWindow(this->siHorizon.nPos, 0, NULL, NULL);
			this->notePadForm->UpdateWindow();
		}	
	}
	else if(this->notePadForm->isMoveActionVertical == TRUE && this->notePadForm->isMoveActionHorizon == FALSE){
		//6. 바뀐 Vertical 좌표를 Get한다.
		//this->notePadForm->GetScrollInfo(SB_VERT, &this->siVertical, SIF_POS);

		if (this->siVertical.nPos != this->beforeY) { //siVertical.nPos != beforeY
			this->notePadForm->yTextOut = this->siVertical.nPos * -1;
			this->notePadForm->ScrollWindow(0, this->siVertical.nPos, NULL, NULL);
			this->notePadForm->UpdateWindow();
		}
	}
	//FirstDoc,EndDoc
	else if (this->notePadForm->isMoveActionVertical == TRUE && this->notePadForm->isMoveActionHorizon == TRUE) {
		//5. 바뀐 Horizon 좌표를 Get한다.
		//this->notePadForm->GetScrollInfo(SB_HORZ, &this->siHorizon, SIF_POS);

		//5.1. 이전 Horizon좌표와 바뀐 Horizon좌표가 다르면 윈도우를 스크롤한다.
		//Horizon 스크롤 활성화를 결정한다.

		if (this->siHorizon.nPos != this->beforeX) {
			this->notePadForm->xTextOut = this->siHorizon.nPos * -1;

		}

		//this->notePadForm->GetScrollInfo(SB_VERT, &this->siVertical);

		if (this->siVertical.nPos != this->beforeY) { //siVertical.nPos != beforeY
			this->notePadForm->yTextOut = this->siVertical.nPos * -1;
		}

		this->notePadForm->ScrollWindow(this->siHorizon.nPos, this->siVertical.nPos, NULL, NULL);
		this->notePadForm->UpdateWindow();
	}
	else if(this->notePadForm->isAppend == TRUE || this->notePadForm->isInsert == TRUE){
		//4. 이전 좌표를 Get한다.
		this->siHorizon.cbSize = sizeof(siHorizon);
		this->siHorizon.fMask = SIF_ALL;

		this->siVertical.cbSize = sizeof(siVertical);
		this->siVertical.fMask = SIF_ALL;
		
		int beforeX = this->siHorizon.nPos;
		int beforeY = this->siVertical.nPos;

		//3. 바뀐 좌표를 구한다.
		  //3.1. 붙혀적기 했을 경우, 
		if (this->notePadForm->isAppend == TRUE) {
			this->notePadForm->page->MovePageAppend(this->windowWidth, this->windowHeight, caretX, (caretY + 1) * strSizeY);
			pageX = this->notePadForm->page->GetX();
			pageY = this->notePadForm->page->GetY();

			if (currentX == 0) {
				pageX = 0;
			}
		}
		//3.2. 끼워적기 했을 경우,
		else if (this->notePadForm->isInsert == TRUE) {
			//3.2.0. caret의 x를 row의 length로 초기화한다.
			this->notePadForm->location->caret->x = 0;

			//3.2.1. 현재 커서가 있는 줄의 length까지의 너비를 구한다.
			length = this->notePadForm->row->GetLength();
			i = 0;
			while (i < length) {
				//1. i번째 문자를 얻는다.
				character = this->notePadForm->row->GetChild(i);
				strCharacter = character->GetContent();
				strcpy(chaCharacter, strCharacter.c_str());

				if (chaCharacter[0] >= 0 && chaCharacter[0] < 128) {
					chaCharacter[1] = '\0';
				}
				else {
					chaCharacter[2] = '\0';
				}

				//2. Location의 CalculateX를 호출한다.
				this->notePadForm->getTextSize->CalculateX(chaCharacter);
				i++;
			}
			totalCaretX = this->notePadForm->location->caret->GetX();



			//3.2.2. 해당 너비 - 윈도우 너비 값이 현재 x값보다 크면,
			if (totalCaretX > this->windowWidth && totalCaretX - this->windowWidth > this->siHorizon.nPos) {
				//1. pageX를 구한다.
				pageX = totalCaretX - this->windowWidth;
			}
			else if (totalCaretX <= 0) {
				pageX = 0;
			}

			//3.2.3. 현재 note의 length까지의 높이를 구한다.
			length = this->notePadForm->note->GetLength();

			//3.2.4. 해당 높이가 윈도우 높이보다 크면, 
			if (length * strSizeY > windowHeight && (length * strSizeY) - windowHeight > this->siVertical.nPos) {
				//1. pageY를 구한다.
				pageY = (length * strSizeY) - this->windowHeight;

#if 0
				//(21.08.16 추가) 어차피 밑에 추가해놔서 주석 달아놓음.
				if (this->siVertical.nMax < length * strSizeY) {
					this->siVertical.nPos = pageY;
				}
#endif

			}

		}

		//4. 바뀐 좌표를 Set 한다. ----------------------------------------------------------------------------------
		this->siHorizon.fMask = SIF_ALL;
		this->siVertical.fMask = SIF_ALL;

		//Long pageX = this->notePadForm->page->GetX();

		actualCaretX = caretX - beforeX;

		if (this->notePadForm->isAppend == TRUE || (this->notePadForm->isInsert == TRUE && pageX == 0)) {
			// 붙혀적다의 경우에만 nPos 변경하기. OR 끼워쓰기인데 개행문자로써 첫열에 왔을 경우,
			this->siHorizon.nPos = pageX;
		}
		else if (this->notePadForm->isInsert == TRUE && actualCaretX >= this->windowWidth) {
			//끼워쓰기인데 캐럿이 바운더리 넘겼을 경우,
			this->siHorizon.nPos = caretX - 300;

			if (caretX+300 > totalCaretX) {
				this->siHorizon.nPos = totalCaretX-windowWidth;
			}
			else {
				this->siHorizon.nPos = caretX - 300;
			}
		}

		
		//21.08.03. 이전 XMax보다 더 클 경우에만 바꿔준다.
#if 0
		if (pageX>this->beforePageX) { //beforeXMax < this->windowWidth + pageX
			this->siHorizon.nMax = this->windowWidth + pageX;  //this->siHorizon.nMax + pageX;
			this->beforePageX = pageX;
			this->notePadForm->largestWidth = this->windowWidth + pageX;
		}
#endif
		if (this->windowWidth + pageX > this->notePadForm->largestWidth) { 
			this->siHorizon.nMax = this->windowWidth + pageX;  
			this->beforePageX = pageX;
			this->notePadForm->largestWidth = this->windowWidth + pageX;
		}


		this->notePadForm->SetScrollInfo(SB_HORZ, &this->siHorizon, TRUE);


		//Long pageY = this->notePadForm->page->GetY();
		if (this->notePadForm->isAppend == TRUE) {// 붙혀적다의 경우에만 nPos 변경하기. 
			this->siVertical.nPos = pageY;
		}
		//21.08.03. 이전 YMax보다 더 클 경우에만 바꿔준다.
		if (pageY > this->beforePageY || this->siVertical.nMax < length * strSizeY) {//beforeYMax < this->windowHeight + pageY
			this->siVertical.nMax = caretHeight;  //this->windowHeight + pageY; length * strSizeY;
			this->siVertical.nPos = caretHeight - this->windowHeight; //pageY;
			this->beforePageY = pageY;
		}
		this->notePadForm->SetScrollInfo(SB_VERT, &this->siVertical, TRUE);

		//------------------------------------------------------------------------------------------------------------

		//5. 바뀐 Horizon 좌표를 Get한다.
		//this->notePadForm->GetScrollInfo(SB_HORZ, &this->siHorizon);

		if (this->notePadForm->isAppend == TRUE && pageX != beforeX) { //this->siHorizon.nPos != beforeX
			//this->notePadForm->ScrollWindow(siVertical.nPos, 0, NULL, NULL);
			this->notePadForm->xTextOut = this->siHorizon.nPos * -1;
			this->notePadForm->ScrollWindow(this->siHorizon.nPos, 0, NULL, NULL);
			this->notePadForm->UpdateWindow();
		}
		else if (this->notePadForm->isInsert == TRUE && actualCaretX > windowWidth) {
			this->notePadForm->xTextOut = this->siHorizon.nPos * -1;
			this->notePadForm->ScrollWindow(this->siHorizon.nPos, 0, NULL, NULL);
			this->notePadForm->UpdateWindow();
		}

		//6. 바뀐 Vertical 좌표를 Get한다.
		//this->notePadForm->GetScrollInfo(SB_VERT, &this->siVertical);

		if (this->siVertical.nPos != beforeY) { //siVertical.nPos != beforeY
			//this->notePadForm->ScrollWindow(siVertical.nPos, 0, NULL, NULL);
			this->notePadForm->yTextOut = this->siVertical.nPos * -1;
			this->notePadForm->ScrollWindow(0, this->siVertical.nPos, NULL, NULL);
			this->notePadForm->UpdateWindow();
		}
	}


	if (this->notePadForm->isDeleteAction == TRUE ) {
	   //5. Horizon의 nMax, nPos를 설정한다.
	     //5.1. note의 length만큼 반복한다. (제일 긴 줄의 너비를 구한다.)
		 while (j < noteLength) {
			 row = this->notePadForm->note->GetChild(j);
			 rowLength = row->GetLength();

			 //5.1.1. 해당 줄의 너비를 구한다.

			 this->notePadForm->location->caret->x = 0;
			 i = 0;
			 while (i < rowLength) {
				 character = row->GetChild(i);
				 strCharacter = character->GetContent();
				 strcpy(chaCharacter, strCharacter.c_str());

				 if (chaCharacter[0] >= 0 && chaCharacter[0] < 128) {
					 chaCharacter[1] = '\0';
				 }
				 else {
					 chaCharacter[2] = '\0';
				 }
				 //2.6.2. Location의 CalculateX를 호출한다.
				 this->notePadForm->getTextSize->CalculateX(chaCharacter);
				 i++;
			 }
			 caretX_ = this->notePadForm->location->caret->GetX();

			 if (caretX_ > currentLargestWidth) {
				 currentLargestWidth = caretX_;
			 }

			 j++;	
		 } 

	 	 //5.2. 가장 긴 줄이 windowWidth 를 넘겼을 경우,
		 if (currentLargestWidth > this->windowWidth) {

			 //5.2.1. 이전 largestWidth 보다 더 클 경우, nMax를 변경해준다.
			 if (currentLargestWidth > this->notePadForm->largestWidth) {
				 this->siHorizon.nMax = currentLargestWidth;	 
				 this->notePadForm->largestWidth = currentLargestWidth;
			 }

			 //5.5.1. 현재 커서까지의 너비를 구한다.
			 //5.2.1. 이전 nMax보다 더 클 경우에만 바꿔준다.
			 pageX = caretX - this->windowWidth;

			 if (pageX > this->beforePageX) { 
				 this->beforePageX = pageX;
			 }

			 //5.2.2. 해당 줄의 커서가 바운더리 밖에 있을 경우, 종이를 스크롤한다.
		 }

		 //5.3. 가장 긴 줄이 windowWidth 를 못 넘겼을 경우,
		 else {
			 //5.3.1. 0보다 작거나 같은 경우, nMax를 windowWidth로 변경한다.
			 if (currentLargestWidth <= 0) {
				 this->siHorizon.nMax = 0;
			 }
			 //5.3.2. nPos를 nMin으로 변경한다.
			 this->siHorizon.nPos = this->siHorizon.nMin;
		 }


		 //6. Vertical의 전체 높이를 구하고 nMax를 변경해준다.
		 height = noteLength * strSizeY;
		 if (height > this->windowHeight) {
			 this->siVertical.nMax = height;
			 //this->siVertical.nPos = this->windowHeight - height;
		 }
		 else {
			 this->siVertical.nMax = 0;
			 //this->siVertical.nPos = 0;
		 }
//#if 0
		 //6.1. 현재 커서까지의 높이를 구한다.
		 caretHeight = (currentY + 1) * strSizeY;

		 //6.2. Vertical의 nPos를 변경해준다.
		 if (caretHeight > this->windowWidth) {
			 this->siVertical.nPos = caretHeight - this->windowHeight;
			 this->notePadForm->ScrollWindow(0, this->siVertical.nPos, NULL, NULL);
			 this->notePadForm->UpdateWindow();
		 }
		 else {
			 this->siVertical.nPos = 0;
		 }

		 //6.3. 최종 결정된 인수들로 Vertical 정보를 SET 해준다.
		 this->notePadForm->SetScrollInfo(SB_VERT, &this->siVertical, TRUE);
//#endif

    }

	if (this->notePadForm->isLineChangeButtonClicked == TRUE && this->notePadForm->isMoveActionVertical == FALSE) {
		//6. Vertical의 전체 높이를 구하고 nMax를 변경해준다.
		height = noteLength * strSizeY;
		if (height > this->windowHeight) {
			this->siVertical.nMax = height;
		}
		else {
			this->siVertical.nMax = 0;
		}

		//6.1. 현재 커서까지의 높이를 구한다.
		caretHeight = (currentY + 1) * strSizeY;

		//6.2. Vertical의 nPos를 변경해주고 페이지를 이동한다.
		if (caretHeight > this->windowWidth) {
			this->siVertical.nPos = caretHeight - this->windowHeight;
			this->notePadForm->ScrollWindow(0, this->siVertical.nPos, NULL, NULL);
			this->notePadForm->UpdateWindow();
		}
		else {
			this->siVertical.nPos = 0;
		}

		//6.3. 최종 결정된 인수들로 Vertical 정보를 SET 해준다.
		this->notePadForm->SetScrollInfo(SB_VERT, &this->siVertical, TRUE);
	}



}
#endif


void Scroll::MovePage() {
	SCROLLINFO siHorizon;
	SCROLLINFO siVertical;
	Glyph* row;
	Long totalCaretX;
	int strSizeY = this->notePadForm->getTextSize->GetX(131);
	int caretY;
	int caretX;
	int caretEndX;
	int doubleByteSize;
	bool isEnableForHorizon = false;
	bool isEnableForVertical = false;

#if 0
	//0. 윈도우의 크기 구한다. 
	this->notePadForm->GetClientRect(this->notePadForm->m_rectCurHist);
	//this->GetWindowRect(m_rectCurHist);
	this->windowWidth = this->notePadForm->m_rectCurHist.right - this->notePadForm->m_rectCurHist.left;
	this->windowHeight = this->notePadForm->m_rectCurHist.bottom - this->notePadForm->m_rectCurHist.top;
#endif

	//0. 현재 캐럿의 위치를 구한다.
	caretY = this->notePadForm->note->GetCurrent();
	this->notePadForm->row = this->notePadForm->note->GetChild(caretY);
	caretX = this->notePadForm->row->GetCurrent();

	Long caretHeight = (caretY + 1) * strSizeY;
	int newNPos;
	Long width;
	Long widthEnd;

	//0.1 예전에 했던거 때문에 임시로 해둠.
	this->beforeX = this->siHorizon.nPos;
	this->beforeY = this->siVertical.nPos;

	//1. siHorizon.nPos 설정한다.
	totalCaretX = this->notePadForm->getTextSize->totalCalculateX(caretX, this->notePadForm->row);

	//(21.11.11.추가) 만약 현재 Up 또는 Down 인 경우, Horizon은 건드리지 않는다.
	if (this->notePadForm->isUp == FALSE && this->notePadForm->isDown == FALSE) {
		//1.1. BackSpaceKeyAcion 또는 Previous일 때, 
		if (this->notePadForm->isJumpOverForPrevious == TRUE) {

			//1.1.1.caretX까지의 너비 - nPos가 0보다 작거나 같은 경우, nPos에서 윈도우의 절반씩 빼준다.
			if (totalCaretX - this->siHorizon.nPos <= 0) {
				this->siHorizon.nPos = this->siHorizon.nPos - this->windowWidth / 2; //300
				newNPos = this->siHorizon.nPos;

				if (newNPos < 0) {
					this->siHorizon.nPos = 0;
				}
			}
		}
		//1.2. Next일 때 
		else if (this->notePadForm->isJumpOverForNext == TRUE) {
			//1.2.1.caretX까지의 너비 - nPos가 윈도우 너비보다 크거나 같은 경우, nPos에서 윈도우의 절반씩 더해준다.
			if (totalCaretX - this->siHorizon.nPos >= this->windowWidth) {

				this->siHorizon.nPos = this->siHorizon.nPos + this->windowWidth/2; //300;
				newNPos = this->siHorizon.nPos;

				//현재 row의 endX까지의 너비를 구한다.
				caretEndX = this->notePadForm->getTextSize->totalCalculateEndX();

				if (newNPos >= caretEndX) {
					this->siHorizon.nPos = caretEndX + 10;
				}
			}
		}
		//1.3. 캐럿까지의 너비가 윈도우너비를 넘겼을 경우,
		else if (totalCaretX > this->windowWidth) {

			//1.3.1. (21.11.25.추가) 윈도우너비 안넘었어도 스크롤이 움직여서,,
			if (totalCaretX - this->siHorizon.nPos > this->windowWidth) {
				//한글이면 글자 크기를 하나 더해준다. 
				if (this->notePadForm->isDoubleByte == TRUE) {
					doubleByteSize = this->notePadForm->getTextSize->GetX(128);
					this->siHorizon.nPos = (totalCaretX - this->windowWidth) + doubleByteSize + 10;
				}
				else {
					this->siHorizon.nPos = (totalCaretX - this->windowWidth) + 10;
				}
			
			}
		}
		//1.4.
		else if(this->notePadForm->isEndDoc == FALSE){
			this->siHorizon.nPos = 0;
		}
	}


#if 0
	if (totalCaretX > this->windowWidth) {
		//1.1.BackSpaceKeyAcion 또는 Previous일 때
		if (this->notePadForm->isJumpOverForPrevious == TRUE) {

			//1.1.1. 현재 캐럿까지의 너비를 구한다.
			width = this->notePadForm->getTextSize->totalCalculateX(caretX, row);

			if (width - this->siHorizon.nPos <= 0) { //width <= this->siHorizon.nPos
				this->siHorizon.nPos = this->siHorizon.nPos - 300;
				newNPos = this->siHorizon.nPos;

				if (newNPos < 0) {
					this->siHorizon.nPos = 0;
				}
			}
		}
		//1.2. Next일때
		else if (this->notePadForm->isJumpOverForNext == TRUE) {
			//1.2.1. 현재 캐럿까지의 너비를 구한다.
			width = this->notePadForm->getTextSize->totalCalculateX(caretX, row);

			//1.2.2. 현재 row의 end까지 너비를 구한다.**************************************
			caretEndX = this->notePadForm->getTextSize->totalCalculateEndX();


			if (width - this->siHorizon.nPos >= this->windowWidth) {
				this->siHorizon.nPos = this->siHorizon.nPos + 300;
				newNPos = this->siHorizon.nPos;

				if (newNPos >= caretEndX) {
					this->siHorizon.nPos = caretEndX + 10;
				}
			}
		}

		else {
			//한글이면 글자 크기를 하나 더해준다. 
			if (this->notePadForm->isDoubleByte == TRUE) {
				doubleByteSize = this->notePadForm->getTextSize->GetX(128);
				this->siHorizon.nPos = (totalCaretX - this->windowWidth) + doubleByteSize + 10;
			}
			else {
				this->siHorizon.nPos = (totalCaretX - this->windowWidth) + 10;
			}

		}

		//this->notePadForm->SetScrollInfo(SB_HORZ, &this->siHorizon, TRUE);
		//this->notePadForm->ScrollWindow(this->siHorizon.nPos, 0, NULL, NULL);
		//this->notePadForm->UpdateWindow();
	}
	else {
		this->siHorizon.nPos = 0;
		//this->notePadForm->SetScrollInfo(SB_HORZ, &this->siHorizon, TRUE);
		//this->notePadForm->ScrollWindow(this->siHorizon.nPos, 0, NULL, NULL);
		//this->notePadForm->UpdateWindow();
	}
#endif

	//2. siVertical.nPos 설정한다.
	//2.1. Up 일 때,
	if (this->notePadForm->isUp == TRUE) {
		if (caretHeight - this->siVertical.nPos <= 0 || caretHeight - this->siVertical.nPos < strSizeY) {
			this->siVertical.nPos = caretHeight - strSizeY;
		}

		//(21.11.24.추가)2.2.1. Up후 캐럿까지의 물리적 너비가 바꾸기 전 Horizon의 nPos보다 작은 경우,
		if (totalCaretX < this->siHorizon.nPos) {
			//2.2.1.1. Down후 캐럿까지의 물리적 너비에서 윈도우 3분의 1만큼 뺀다.
			this->siHorizon.nPos = totalCaretX - this->windowWidth / 3;
			if (this->siHorizon.nPos < 0) {
				this->siHorizon.nPos = 0;
			}
		}
	}
	//2.2. Down 일 때,
	else if (this->notePadForm->isDown == TRUE) {
		if (caretHeight - this->siVertical.nPos > this->windowHeight) {
			this->siVertical.nPos = this->siVertical.nPos + strSizeY;
		}

		//(21.11.24.추가)2.2.1. Down후 캐럿까지의 물리적 너비가 바꾸기 전 Horizon의 nPos보다 작은 경우,
		if (totalCaretX < this->siHorizon.nPos) {
			//2.2.1.1. Down후 캐럿까지의 물리적 너비에서 윈도우 3분의 1만큼 뺀다.
			this->siHorizon.nPos = totalCaretX - this->windowWidth / 3;
			if (this->siHorizon.nPos < 0) {
				this->siHorizon.nPos = 0;	
			}
		}

	}
	//2.3. caretHeight - nPos가 현재 윈도우 높이보다 큰 경우, nPos를 변경해준다.
	else if (caretHeight - this->siVertical.nPos > this->windowHeight) {
		this->siVertical.nPos = caretHeight - this->windowHeight;
	}
	else if (caretHeight <= this->siVertical.nPos || caretHeight - this->siVertical.nPos < strSizeY) {
		this->siVertical.nPos = this->siVertical.nPos - strSizeY; //caretHeight - strSizeY;

		if (caretY == 0) {
			this->siVertical.nPos = 0;
		}
	}
#if 0
	else {
		this->siVertical.nPos = 0;
	}
#endif


	//3. siHorizon.nMax 설정한다. --------------------------------------------------------------------------------
		//3.1. note의 length만큼 반복한다. (제일 긴 줄의 너비를 구한다.)
	int i;
	int j = 0;
	Long noteLength = this->notePadForm->note->GetLength();
	Long rowLength;
	Glyph* character;
	string strCharacter;
	char chaCharacter[3];
	int currentLargestWidth = 0;
	int caretX_;
	Long pageX = 0;

	while (j < noteLength) {
		row = this->notePadForm->note->GetChild(j);
		rowLength = row->GetLength();

		//3.1.1. 해당 줄의 너비를 구한다.

		this->notePadForm->location->caret->x = 0;
		i = 0;
		while (i < rowLength) {
			character = row->GetChild(i);

//#if 0
			//(21.11.02. 추가)
			if (character == NULL) {
				i++;
				if (i < rowLength) {
					character = row->GetChild(i);
				}
				else {
					break;
				}
			}
//#endif

			strCharacter = character->GetContent();
			strcpy(chaCharacter, strCharacter.c_str());

			if (chaCharacter[0] >= 0 && chaCharacter[0] < 128) {
				chaCharacter[1] = '\0';
			}
			else {
				chaCharacter[2] = '\0';
			}
			// Location의 CalculateX를 호출한다.
			this->notePadForm->getTextSize->CalculateX(chaCharacter);
			i++;
		}
		caretX_ = this->notePadForm->location->caret->GetX();

		if (caretX_ > currentLargestWidth) {
			currentLargestWidth = caretX_;
		}

		j++;
	}


	//(21.10.28.추가) 자동개행이 아닐 경우에만 siHorizon.nMax 설정해준다.
	this->siHorizon.nPage = this->windowWidth;
	if (this->notePadForm->isLineChangeButtonClicked == FALSE) {
		//this->siHorizon.nMax = currentLargestWidth;

		this->siHorizon.nPage = this->windowWidth;
		//3.2. 가장 긴 줄이 windowWidth 를 넘겼을 경우,
		if (currentLargestWidth > this->windowWidth) {

			this->siHorizon.nMax = currentLargestWidth;
			this->notePadForm->largestWidth = currentLargestWidth;

			//5.5.1. 현재 커서까지의 너비를 구한다.
			//5.2.1. 이전 nMax보다 더 클 경우에만 바꿔준다.
			pageX = caretX - this->windowWidth;

			if (pageX > this->beforePageX) {
				this->beforePageX = pageX;
			}

			//5.2.2. 해당 줄의 커서가 바운더리 밖에 있을 경우, 종이를 스크롤한다.
		}

		//3.3. 가장 긴 줄이 windowWidth 를 못 넘겼을 경우,
		else {
			this->siHorizon.nMax = this->windowWidth + 50; ////this->windowWidth - 50(없어진다), this->windowWidth + 50(생긴다)
			this->siHorizon.nPos = 0; //this->siHorizon.nMin;
			isEnableForHorizon = true;
		}
	}
	else {
		this->siHorizon.nMax = this->windowWidth + 50;  //this->windowWidth - 50(없어진다), this->windowWidth + 50(생긴다)
		this->siHorizon.nPos = 0; //this->siHorizon.nMin
		isEnableForHorizon = true;
	}

	//3.4. 최종 결정된 인수들로 Horizon 정보를 SET 해준다.
	this->notePadForm->SetScrollInfo(SB_HORZ, &this->siHorizon, TRUE);
	//---------------------------------------------------------------------------------------------------------------



	//4. siVertical.nMax 설정한다.
	//4.1. Vertical의 전체 높이를 구하고 nMax를 변경해준다.
	int height = noteLength * strSizeY;
	this->siVertical.nPage = this->windowHeight;
	if (height > this->windowHeight) {
		this->siVertical.nMax = height;
	}
	else { //(21.11.20.추가)
		this->siVertical.nMax = this->siVertical.nPage + 50;
		this->siVertical.nPos = 0;
		isEnableForVertical = true;
	}

	//6.3. 최종 결정된 인수들로 Vertical 정보를 SET 해준다.
	this->notePadForm->SetScrollInfo(SB_VERT, &this->siVertical, TRUE);

#if 0
	//(21.11.18.추가) 5. 간혹가다 자동개행 시 수직스크롤이 생겨 좌표가 변경될 수 있는 경우를 위해 추가.
	//5.1. 자동개행 되어있는 경우, 자동개행전 캐럿 구한다.
	int currentX;
	int currentY;

	if (this->notePadForm->isLineChangeButtonClicked == true && this->notePadForm->isComposition == false) {
		//5.1.1. 자동개행 전 캐럿의 위치를 찾는다.
		this->notePadForm->lineChange->FindCaretBeforeLineChange(&currentX, &currentY);

		//5.1.2. 자동개행 된 문서를 펼친다.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

		//5.1.3. 펼친 문서를 다시 자동개행한다. (ON)
		this->windowWidth = this->notePadForm->m_rectCurHist.right - this->notePadForm->m_rectCurHist.left;
		this->notePadForm->lineChange->LineChangeButtonClicked();

		//5.1.4. 자동개행 후 캐럿 구한다.
		this->notePadForm->lineChange->FindCaretAfterLineChange(currentX, currentY);
	
	}

	this->notePadForm->isComposition = true;
#endif

	CWnd* pWnd = AfxGetMainWnd();
	HWND hWnd = pWnd->m_hWnd;
	DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);

//#if 0
	//Horzizon 스크롤의 비활성화를 정한다.
	if (isEnableForHorizon == true) {

		dwStyle = dwStyle | WS_HSCROLL; //dwStyle | WS_HSCROLL;
		SetWindowLong(hWnd, GWL_STYLE, dwStyle);

		this->notePadForm->EnableScrollBar(SB_HORZ, ESB_DISABLE_BOTH);
	}
	else {

		dwStyle = dwStyle | WS_HSCROLL; //| WS_HSCROLL | WS_VSCROLL
		SetWindowLong(hWnd, GWL_STYLE, dwStyle);

		this->notePadForm->EnableScrollBar(SB_HORZ, ESB_ENABLE_BOTH);
	}
//#endif

	//Vertical 스크롤의 비활성화를 정한다.
	if (isEnableForVertical == true) {

		dwStyle = dwStyle | WS_VSCROLL; //| WS_HSCROLL | WS_VSCROLL
		SetWindowLong(hWnd, GWL_STYLE, dwStyle);

		this->notePadForm->EnableScrollBar(SB_VERT, ESB_DISABLE_BOTH);
	}
	else {
		dwStyle = dwStyle | WS_VSCROLL; //| WS_HSCROLL | WS_VSCROLL
		SetWindowLong(hWnd, GWL_STYLE, dwStyle);

		this->notePadForm->EnableScrollBar(SB_VERT, ESB_ENABLE_BOTH);
	}

	this->notePadForm->isEndDoc = FALSE;
	//this->notePadForm->RedrawWindow(NULL, NULL, RDW_UPDATENOW | RDW_ERASE);
	//this->notePadForm->EnableScrollBar(SB_HORZ, ESB_ENABLE_BOTH);
	this->notePadForm->Invalidate();

}


#if 0
void Scroll::MovePage() {

	//(21.11.19.추가) 비활성화 & 고정된 양쪽 스크롤을 만든다.
	CWnd* pWnd = AfxGetMainWnd();
	HWND hWnd = pWnd->m_hWnd;
	DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);
	dwStyle = dwStyle | WS_HSCROLL | WS_VSCROLL;
	SetWindowLong(hWnd, GWL_STYLE, dwStyle);

	this->notePadForm->EnableScrollBar(SB_BOTH, ESB_ENABLE_BOTH);

	this->notePadForm->Invalidate();

}
#endif

void Scroll::Create() {
	Glyph* row;
	Long totalCaretX;
	int strSizeY = this->notePadForm->getTextSize->GetX(131);
	int caretY;
	int caretX;
	int doubleByteSize;



	//3. siHorizon.nMax 설정한다. --------------------------------------------------------------------------------
	//3.1. note의 length만큼 반복한다. (제일 긴 줄의 너비를 구한다.)
	int i;
	int j = 0;
	Long noteLength = this->notePadForm->note->GetLength();
	Long rowLength;
	Glyph* character;
	string strCharacter;
	char chaCharacter[3];
	int currentLargestWidth = 0;
	int caretX_;
	Long pageX = 0;


	while (j < noteLength) {
		row = this->notePadForm->note->GetChild(j);
		rowLength = row->GetLength();

		//3.1.1. 해당 줄의 너비를 구한다.

		this->notePadForm->location->caret->x = 0;
		i = 0;
		while (i < rowLength) {
			character = row->GetChild(i);
			strCharacter = character->GetContent();
			strcpy(chaCharacter, strCharacter.c_str());

			if (chaCharacter[0] >= 0 && chaCharacter[0] < 128) {
				chaCharacter[1] = '\0';
			}
			else {
				chaCharacter[2] = '\0';
			}
			// Location의 CalculateX를 호출한다.
			this->notePadForm->getTextSize->CalculateX(chaCharacter);
			i++;
		}
		caretX_ = this->notePadForm->location->caret->GetX();

		if (caretX_ > currentLargestWidth) {
			currentLargestWidth = caretX_;
		}

		j++;
	}
	this->siHorizon.nMax = currentLargestWidth;


#if 0
	//3.2. 가장 긴 줄이 windowWidth 를 넘겼을 경우,
	if (currentLargestWidth > this->windowWidth) {

		//3.2.1. 이전 largestWidth 보다 더 클 경우, nMax를 변경해준다.
		if (currentLargestWidth > this->notePadForm->largestWidth) {
			this->siHorizon.nMax = currentLargestWidth;
			this->notePadForm->largestWidth = currentLargestWidth;
		}
	}
	//3.3. 가장 긴 줄이 windowWidth 를 못 넘겼을 경우,
	else {
		//3.3.1. 0보다 작거나 같은 경우, nMax를 windowWidth로 변경한다.
		if (currentLargestWidth <= 0) {
			this->siHorizon.nMax = 0;
		}
	}
#endif

	//5.2. nMax를 해당 너비로 치환한다.
	this->notePadForm->SetScrollRange(SB_HORZ, 0, 1000, FALSE);
	this->notePadForm->largestWidth = currentLargestWidth;
	this->siHorizon.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	//this->siHorizon.fMask = SIF_ALL & ~SIF_POS;
	this->siHorizon.nMin = 0;
	this->siHorizon.nPos = 0;
	this->siHorizon.nPage = this->windowWidth;
	this->notePadForm->SetScrollInfo(SB_HORZ, &this->siHorizon, TRUE);

	//---------------------------------------------------------------------------------------------------------------

	//4. siVertical.nMax 설정한다.
	//4.1. Vertical의 전체 높이를 구하고 nMax를 변경해준다.
	int height = noteLength * strSizeY;

	this->siVertical.nMax = height;
#if 0
	if (height > this->windowHeight) {
		this->siVertical.nMax = height;
	}
	else {
		this->siVertical.nMax = 0;
	}
#endif

	//6.3. 최종 결정된 인수들로 Vertical 정보를 SET 해준다.
	this->notePadForm->SetScrollRange(SB_VERT, 0, 1000, FALSE);
	this->siVertical.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	this->siVertical.nMin = 0;
	this->siVertical.nPos = 0;
	this->siVertical.nPage = this->windowHeight;
	this->notePadForm->SetScrollInfo(SB_VERT, &this->siVertical, TRUE);
	//------------------------------------------------------------------------------------------------------
}

void Scroll::Update() {
	MovePage();
}










