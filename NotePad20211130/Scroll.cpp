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

	//1. �������� ũ�� ���Ѵ�.
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


	//1. �������� ũ�� ���Ѵ�.
	this->notePadForm->GetClientRect(this->notePadForm->m_rectCurHist);
	//this->GetWindowRect(m_rectCurHist);
	this->windowWidth = this->notePadForm->m_rectCurHist.right - this->notePadForm->m_rectCurHist.left;
	this->windowHeight = this->notePadForm->m_rectCurHist.bottom - this->notePadForm->m_rectCurHist.top;

	//2. ĳ���� ũ�� ���Ѵ�.
	int strSizeY = this->notePadForm->getTextSize->GetX(131);

	//Scroll ������ Location�� ȣ��ǹǷ� caretX, caretY�� ������ ��������Ѵ�.....-----------------------------
	  //1.2. ���� note�� current Ȯ���Ѵ�.
	Long currentY = this->notePadForm->note->GetCurrent();
	noteLength = this->notePadForm->note->GetLength();
	//1.3. ���� note�� row�� ã�´�.
	Glyph* row = this->notePadForm->note->GetChild(currentY);
	//1.4. ���� row�� current Ȯ���Ѵ�.
	Long currentX = row->GetCurrent();

	if (this->notePadForm->isLineChangeButtonClicked == FALSE) {
		this->notePadForm->location->caret->x = 0;
		i = 0;
		while (i < currentX) {
			//2.6.1. i��° ���ڸ� ��´�.
			character = row->GetChild(i);
			strCharacter = character->GetContent();
			strcpy(chaCharacter, strCharacter.c_str());

			if (chaCharacter[0] >= 0 && chaCharacter[0] < 128) {
				chaCharacter[1] = '\0';
			}
			else {
				chaCharacter[2] = '\0';
			}

			//2.6.2. Location�� CalculateX�� ȣ���Ѵ�.
			this->notePadForm->getTextSize->CalculateX(chaCharacter);
			//CalculateX(chaCharacter);
			i++;
		}
	}

	int caretX = this->notePadForm->location->caret->GetX(); //�ֱٱ����� �ʺ� (��������, �������� ����, Ŀ��������)
	int caretY = currentY; //�ֱٱ����� ����
	caretHeight = (caretY + 1) * strSizeY;
	//------------------------------------------------------------------------------------------------------------



	if (this->notePadForm->isMoveActionHorizon == TRUE && this->notePadForm->isMoveActionVertical == FALSE) {
		//------------------------------------------------------------------------------------------------------------
        //5. �ٲ� Horizon ��ǥ�� Get�Ѵ�.
		//this->notePadForm->GetScrollInfo(SB_HORZ, &this->siHorizon, SIF_POS);

		//5.1. ���� Horizon��ǥ�� �ٲ� Horizon��ǥ�� �ٸ��� �����츦 ��ũ���Ѵ�.
		//Horizon ��ũ�� Ȱ��ȭ�� �����Ѵ�.
		if (this->siHorizon.nPos != this->beforeX) {
			this->notePadForm->xTextOut = this->siHorizon.nPos * -1;
			this->notePadForm->ScrollWindow(this->siHorizon.nPos, 0, NULL, NULL);
			this->notePadForm->UpdateWindow();
		}	
	}
	else if(this->notePadForm->isMoveActionVertical == TRUE && this->notePadForm->isMoveActionHorizon == FALSE){
		//6. �ٲ� Vertical ��ǥ�� Get�Ѵ�.
		//this->notePadForm->GetScrollInfo(SB_VERT, &this->siVertical, SIF_POS);

		if (this->siVertical.nPos != this->beforeY) { //siVertical.nPos != beforeY
			this->notePadForm->yTextOut = this->siVertical.nPos * -1;
			this->notePadForm->ScrollWindow(0, this->siVertical.nPos, NULL, NULL);
			this->notePadForm->UpdateWindow();
		}
	}
	//FirstDoc,EndDoc
	else if (this->notePadForm->isMoveActionVertical == TRUE && this->notePadForm->isMoveActionHorizon == TRUE) {
		//5. �ٲ� Horizon ��ǥ�� Get�Ѵ�.
		//this->notePadForm->GetScrollInfo(SB_HORZ, &this->siHorizon, SIF_POS);

		//5.1. ���� Horizon��ǥ�� �ٲ� Horizon��ǥ�� �ٸ��� �����츦 ��ũ���Ѵ�.
		//Horizon ��ũ�� Ȱ��ȭ�� �����Ѵ�.

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
		//4. ���� ��ǥ�� Get�Ѵ�.
		this->siHorizon.cbSize = sizeof(siHorizon);
		this->siHorizon.fMask = SIF_ALL;

		this->siVertical.cbSize = sizeof(siVertical);
		this->siVertical.fMask = SIF_ALL;
		
		int beforeX = this->siHorizon.nPos;
		int beforeY = this->siVertical.nPos;

		//3. �ٲ� ��ǥ�� ���Ѵ�.
		  //3.1. �������� ���� ���, 
		if (this->notePadForm->isAppend == TRUE) {
			this->notePadForm->page->MovePageAppend(this->windowWidth, this->windowHeight, caretX, (caretY + 1) * strSizeY);
			pageX = this->notePadForm->page->GetX();
			pageY = this->notePadForm->page->GetY();

			if (currentX == 0) {
				pageX = 0;
			}
		}
		//3.2. �������� ���� ���,
		else if (this->notePadForm->isInsert == TRUE) {
			//3.2.0. caret�� x�� row�� length�� �ʱ�ȭ�Ѵ�.
			this->notePadForm->location->caret->x = 0;

			//3.2.1. ���� Ŀ���� �ִ� ���� length������ �ʺ� ���Ѵ�.
			length = this->notePadForm->row->GetLength();
			i = 0;
			while (i < length) {
				//1. i��° ���ڸ� ��´�.
				character = this->notePadForm->row->GetChild(i);
				strCharacter = character->GetContent();
				strcpy(chaCharacter, strCharacter.c_str());

				if (chaCharacter[0] >= 0 && chaCharacter[0] < 128) {
					chaCharacter[1] = '\0';
				}
				else {
					chaCharacter[2] = '\0';
				}

				//2. Location�� CalculateX�� ȣ���Ѵ�.
				this->notePadForm->getTextSize->CalculateX(chaCharacter);
				i++;
			}
			totalCaretX = this->notePadForm->location->caret->GetX();



			//3.2.2. �ش� �ʺ� - ������ �ʺ� ���� ���� x������ ũ��,
			if (totalCaretX > this->windowWidth && totalCaretX - this->windowWidth > this->siHorizon.nPos) {
				//1. pageX�� ���Ѵ�.
				pageX = totalCaretX - this->windowWidth;
			}
			else if (totalCaretX <= 0) {
				pageX = 0;
			}

			//3.2.3. ���� note�� length������ ���̸� ���Ѵ�.
			length = this->notePadForm->note->GetLength();

			//3.2.4. �ش� ���̰� ������ ���̺��� ũ��, 
			if (length * strSizeY > windowHeight && (length * strSizeY) - windowHeight > this->siVertical.nPos) {
				//1. pageY�� ���Ѵ�.
				pageY = (length * strSizeY) - this->windowHeight;

#if 0
				//(21.08.16 �߰�) ������ �ؿ� �߰��س��� �ּ� �޾Ƴ���.
				if (this->siVertical.nMax < length * strSizeY) {
					this->siVertical.nPos = pageY;
				}
#endif

			}

		}

		//4. �ٲ� ��ǥ�� Set �Ѵ�. ----------------------------------------------------------------------------------
		this->siHorizon.fMask = SIF_ALL;
		this->siVertical.fMask = SIF_ALL;

		//Long pageX = this->notePadForm->page->GetX();

		actualCaretX = caretX - beforeX;

		if (this->notePadForm->isAppend == TRUE || (this->notePadForm->isInsert == TRUE && pageX == 0)) {
			// ���������� ��쿡�� nPos �����ϱ�. OR ���������ε� ���๮�ڷν� ù���� ���� ���,
			this->siHorizon.nPos = pageX;
		}
		else if (this->notePadForm->isInsert == TRUE && actualCaretX >= this->windowWidth) {
			//���������ε� ĳ���� �ٿ���� �Ѱ��� ���,
			this->siHorizon.nPos = caretX - 300;

			if (caretX+300 > totalCaretX) {
				this->siHorizon.nPos = totalCaretX-windowWidth;
			}
			else {
				this->siHorizon.nPos = caretX - 300;
			}
		}

		
		//21.08.03. ���� XMax���� �� Ŭ ��쿡�� �ٲ��ش�.
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
		if (this->notePadForm->isAppend == TRUE) {// ���������� ��쿡�� nPos �����ϱ�. 
			this->siVertical.nPos = pageY;
		}
		//21.08.03. ���� YMax���� �� Ŭ ��쿡�� �ٲ��ش�.
		if (pageY > this->beforePageY || this->siVertical.nMax < length * strSizeY) {//beforeYMax < this->windowHeight + pageY
			this->siVertical.nMax = caretHeight;  //this->windowHeight + pageY; length * strSizeY;
			this->siVertical.nPos = caretHeight - this->windowHeight; //pageY;
			this->beforePageY = pageY;
		}
		this->notePadForm->SetScrollInfo(SB_VERT, &this->siVertical, TRUE);

		//------------------------------------------------------------------------------------------------------------

		//5. �ٲ� Horizon ��ǥ�� Get�Ѵ�.
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

		//6. �ٲ� Vertical ��ǥ�� Get�Ѵ�.
		//this->notePadForm->GetScrollInfo(SB_VERT, &this->siVertical);

		if (this->siVertical.nPos != beforeY) { //siVertical.nPos != beforeY
			//this->notePadForm->ScrollWindow(siVertical.nPos, 0, NULL, NULL);
			this->notePadForm->yTextOut = this->siVertical.nPos * -1;
			this->notePadForm->ScrollWindow(0, this->siVertical.nPos, NULL, NULL);
			this->notePadForm->UpdateWindow();
		}
	}


	if (this->notePadForm->isDeleteAction == TRUE ) {
	   //5. Horizon�� nMax, nPos�� �����Ѵ�.
	     //5.1. note�� length��ŭ �ݺ��Ѵ�. (���� �� ���� �ʺ� ���Ѵ�.)
		 while (j < noteLength) {
			 row = this->notePadForm->note->GetChild(j);
			 rowLength = row->GetLength();

			 //5.1.1. �ش� ���� �ʺ� ���Ѵ�.

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
				 //2.6.2. Location�� CalculateX�� ȣ���Ѵ�.
				 this->notePadForm->getTextSize->CalculateX(chaCharacter);
				 i++;
			 }
			 caretX_ = this->notePadForm->location->caret->GetX();

			 if (caretX_ > currentLargestWidth) {
				 currentLargestWidth = caretX_;
			 }

			 j++;	
		 } 

	 	 //5.2. ���� �� ���� windowWidth �� �Ѱ��� ���,
		 if (currentLargestWidth > this->windowWidth) {

			 //5.2.1. ���� largestWidth ���� �� Ŭ ���, nMax�� �������ش�.
			 if (currentLargestWidth > this->notePadForm->largestWidth) {
				 this->siHorizon.nMax = currentLargestWidth;	 
				 this->notePadForm->largestWidth = currentLargestWidth;
			 }

			 //5.5.1. ���� Ŀ�������� �ʺ� ���Ѵ�.
			 //5.2.1. ���� nMax���� �� Ŭ ��쿡�� �ٲ��ش�.
			 pageX = caretX - this->windowWidth;

			 if (pageX > this->beforePageX) { 
				 this->beforePageX = pageX;
			 }

			 //5.2.2. �ش� ���� Ŀ���� �ٿ���� �ۿ� ���� ���, ���̸� ��ũ���Ѵ�.
		 }

		 //5.3. ���� �� ���� windowWidth �� �� �Ѱ��� ���,
		 else {
			 //5.3.1. 0���� �۰ų� ���� ���, nMax�� windowWidth�� �����Ѵ�.
			 if (currentLargestWidth <= 0) {
				 this->siHorizon.nMax = 0;
			 }
			 //5.3.2. nPos�� nMin���� �����Ѵ�.
			 this->siHorizon.nPos = this->siHorizon.nMin;
		 }


		 //6. Vertical�� ��ü ���̸� ���ϰ� nMax�� �������ش�.
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
		 //6.1. ���� Ŀ�������� ���̸� ���Ѵ�.
		 caretHeight = (currentY + 1) * strSizeY;

		 //6.2. Vertical�� nPos�� �������ش�.
		 if (caretHeight > this->windowWidth) {
			 this->siVertical.nPos = caretHeight - this->windowHeight;
			 this->notePadForm->ScrollWindow(0, this->siVertical.nPos, NULL, NULL);
			 this->notePadForm->UpdateWindow();
		 }
		 else {
			 this->siVertical.nPos = 0;
		 }

		 //6.3. ���� ������ �μ���� Vertical ������ SET ���ش�.
		 this->notePadForm->SetScrollInfo(SB_VERT, &this->siVertical, TRUE);
//#endif

    }

	if (this->notePadForm->isLineChangeButtonClicked == TRUE && this->notePadForm->isMoveActionVertical == FALSE) {
		//6. Vertical�� ��ü ���̸� ���ϰ� nMax�� �������ش�.
		height = noteLength * strSizeY;
		if (height > this->windowHeight) {
			this->siVertical.nMax = height;
		}
		else {
			this->siVertical.nMax = 0;
		}

		//6.1. ���� Ŀ�������� ���̸� ���Ѵ�.
		caretHeight = (currentY + 1) * strSizeY;

		//6.2. Vertical�� nPos�� �������ְ� �������� �̵��Ѵ�.
		if (caretHeight > this->windowWidth) {
			this->siVertical.nPos = caretHeight - this->windowHeight;
			this->notePadForm->ScrollWindow(0, this->siVertical.nPos, NULL, NULL);
			this->notePadForm->UpdateWindow();
		}
		else {
			this->siVertical.nPos = 0;
		}

		//6.3. ���� ������ �μ���� Vertical ������ SET ���ش�.
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
	//0. �������� ũ�� ���Ѵ�. 
	this->notePadForm->GetClientRect(this->notePadForm->m_rectCurHist);
	//this->GetWindowRect(m_rectCurHist);
	this->windowWidth = this->notePadForm->m_rectCurHist.right - this->notePadForm->m_rectCurHist.left;
	this->windowHeight = this->notePadForm->m_rectCurHist.bottom - this->notePadForm->m_rectCurHist.top;
#endif

	//0. ���� ĳ���� ��ġ�� ���Ѵ�.
	caretY = this->notePadForm->note->GetCurrent();
	this->notePadForm->row = this->notePadForm->note->GetChild(caretY);
	caretX = this->notePadForm->row->GetCurrent();

	Long caretHeight = (caretY + 1) * strSizeY;
	int newNPos;
	Long width;
	Long widthEnd;

	//0.1 ������ �ߴ��� ������ �ӽ÷� �ص�.
	this->beforeX = this->siHorizon.nPos;
	this->beforeY = this->siVertical.nPos;

	//1. siHorizon.nPos �����Ѵ�.
	totalCaretX = this->notePadForm->getTextSize->totalCalculateX(caretX, this->notePadForm->row);

	//(21.11.11.�߰�) ���� ���� Up �Ǵ� Down �� ���, Horizon�� �ǵ帮�� �ʴ´�.
	if (this->notePadForm->isUp == FALSE && this->notePadForm->isDown == FALSE) {
		//1.1. BackSpaceKeyAcion �Ǵ� Previous�� ��, 
		if (this->notePadForm->isJumpOverForPrevious == TRUE) {

			//1.1.1.caretX������ �ʺ� - nPos�� 0���� �۰ų� ���� ���, nPos���� �������� ���ݾ� ���ش�.
			if (totalCaretX - this->siHorizon.nPos <= 0) {
				this->siHorizon.nPos = this->siHorizon.nPos - this->windowWidth / 2; //300
				newNPos = this->siHorizon.nPos;

				if (newNPos < 0) {
					this->siHorizon.nPos = 0;
				}
			}
		}
		//1.2. Next�� �� 
		else if (this->notePadForm->isJumpOverForNext == TRUE) {
			//1.2.1.caretX������ �ʺ� - nPos�� ������ �ʺ񺸴� ũ�ų� ���� ���, nPos���� �������� ���ݾ� �����ش�.
			if (totalCaretX - this->siHorizon.nPos >= this->windowWidth) {

				this->siHorizon.nPos = this->siHorizon.nPos + this->windowWidth/2; //300;
				newNPos = this->siHorizon.nPos;

				//���� row�� endX������ �ʺ� ���Ѵ�.
				caretEndX = this->notePadForm->getTextSize->totalCalculateEndX();

				if (newNPos >= caretEndX) {
					this->siHorizon.nPos = caretEndX + 10;
				}
			}
		}
		//1.3. ĳ�������� �ʺ� ������ʺ� �Ѱ��� ���,
		else if (totalCaretX > this->windowWidth) {

			//1.3.1. (21.11.25.�߰�) ������ʺ� �ȳѾ�� ��ũ���� ��������,,
			if (totalCaretX - this->siHorizon.nPos > this->windowWidth) {
				//�ѱ��̸� ���� ũ�⸦ �ϳ� �����ش�. 
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
		//1.1.BackSpaceKeyAcion �Ǵ� Previous�� ��
		if (this->notePadForm->isJumpOverForPrevious == TRUE) {

			//1.1.1. ���� ĳ�������� �ʺ� ���Ѵ�.
			width = this->notePadForm->getTextSize->totalCalculateX(caretX, row);

			if (width - this->siHorizon.nPos <= 0) { //width <= this->siHorizon.nPos
				this->siHorizon.nPos = this->siHorizon.nPos - 300;
				newNPos = this->siHorizon.nPos;

				if (newNPos < 0) {
					this->siHorizon.nPos = 0;
				}
			}
		}
		//1.2. Next�϶�
		else if (this->notePadForm->isJumpOverForNext == TRUE) {
			//1.2.1. ���� ĳ�������� �ʺ� ���Ѵ�.
			width = this->notePadForm->getTextSize->totalCalculateX(caretX, row);

			//1.2.2. ���� row�� end���� �ʺ� ���Ѵ�.**************************************
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
			//�ѱ��̸� ���� ũ�⸦ �ϳ� �����ش�. 
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

	//2. siVertical.nPos �����Ѵ�.
	//2.1. Up �� ��,
	if (this->notePadForm->isUp == TRUE) {
		if (caretHeight - this->siVertical.nPos <= 0 || caretHeight - this->siVertical.nPos < strSizeY) {
			this->siVertical.nPos = caretHeight - strSizeY;
		}

		//(21.11.24.�߰�)2.2.1. Up�� ĳ�������� ������ �ʺ� �ٲٱ� �� Horizon�� nPos���� ���� ���,
		if (totalCaretX < this->siHorizon.nPos) {
			//2.2.1.1. Down�� ĳ�������� ������ �ʺ񿡼� ������ 3���� 1��ŭ ����.
			this->siHorizon.nPos = totalCaretX - this->windowWidth / 3;
			if (this->siHorizon.nPos < 0) {
				this->siHorizon.nPos = 0;
			}
		}
	}
	//2.2. Down �� ��,
	else if (this->notePadForm->isDown == TRUE) {
		if (caretHeight - this->siVertical.nPos > this->windowHeight) {
			this->siVertical.nPos = this->siVertical.nPos + strSizeY;
		}

		//(21.11.24.�߰�)2.2.1. Down�� ĳ�������� ������ �ʺ� �ٲٱ� �� Horizon�� nPos���� ���� ���,
		if (totalCaretX < this->siHorizon.nPos) {
			//2.2.1.1. Down�� ĳ�������� ������ �ʺ񿡼� ������ 3���� 1��ŭ ����.
			this->siHorizon.nPos = totalCaretX - this->windowWidth / 3;
			if (this->siHorizon.nPos < 0) {
				this->siHorizon.nPos = 0;	
			}
		}

	}
	//2.3. caretHeight - nPos�� ���� ������ ���̺��� ū ���, nPos�� �������ش�.
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


	//3. siHorizon.nMax �����Ѵ�. --------------------------------------------------------------------------------
		//3.1. note�� length��ŭ �ݺ��Ѵ�. (���� �� ���� �ʺ� ���Ѵ�.)
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

		//3.1.1. �ش� ���� �ʺ� ���Ѵ�.

		this->notePadForm->location->caret->x = 0;
		i = 0;
		while (i < rowLength) {
			character = row->GetChild(i);

//#if 0
			//(21.11.02. �߰�)
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
			// Location�� CalculateX�� ȣ���Ѵ�.
			this->notePadForm->getTextSize->CalculateX(chaCharacter);
			i++;
		}
		caretX_ = this->notePadForm->location->caret->GetX();

		if (caretX_ > currentLargestWidth) {
			currentLargestWidth = caretX_;
		}

		j++;
	}


	//(21.10.28.�߰�) �ڵ������� �ƴ� ��쿡�� siHorizon.nMax �������ش�.
	this->siHorizon.nPage = this->windowWidth;
	if (this->notePadForm->isLineChangeButtonClicked == FALSE) {
		//this->siHorizon.nMax = currentLargestWidth;

		this->siHorizon.nPage = this->windowWidth;
		//3.2. ���� �� ���� windowWidth �� �Ѱ��� ���,
		if (currentLargestWidth > this->windowWidth) {

			this->siHorizon.nMax = currentLargestWidth;
			this->notePadForm->largestWidth = currentLargestWidth;

			//5.5.1. ���� Ŀ�������� �ʺ� ���Ѵ�.
			//5.2.1. ���� nMax���� �� Ŭ ��쿡�� �ٲ��ش�.
			pageX = caretX - this->windowWidth;

			if (pageX > this->beforePageX) {
				this->beforePageX = pageX;
			}

			//5.2.2. �ش� ���� Ŀ���� �ٿ���� �ۿ� ���� ���, ���̸� ��ũ���Ѵ�.
		}

		//3.3. ���� �� ���� windowWidth �� �� �Ѱ��� ���,
		else {
			this->siHorizon.nMax = this->windowWidth + 50; ////this->windowWidth - 50(��������), this->windowWidth + 50(�����)
			this->siHorizon.nPos = 0; //this->siHorizon.nMin;
			isEnableForHorizon = true;
		}
	}
	else {
		this->siHorizon.nMax = this->windowWidth + 50;  //this->windowWidth - 50(��������), this->windowWidth + 50(�����)
		this->siHorizon.nPos = 0; //this->siHorizon.nMin
		isEnableForHorizon = true;
	}

	//3.4. ���� ������ �μ���� Horizon ������ SET ���ش�.
	this->notePadForm->SetScrollInfo(SB_HORZ, &this->siHorizon, TRUE);
	//---------------------------------------------------------------------------------------------------------------



	//4. siVertical.nMax �����Ѵ�.
	//4.1. Vertical�� ��ü ���̸� ���ϰ� nMax�� �������ش�.
	int height = noteLength * strSizeY;
	this->siVertical.nPage = this->windowHeight;
	if (height > this->windowHeight) {
		this->siVertical.nMax = height;
	}
	else { //(21.11.20.�߰�)
		this->siVertical.nMax = this->siVertical.nPage + 50;
		this->siVertical.nPos = 0;
		isEnableForVertical = true;
	}

	//6.3. ���� ������ �μ���� Vertical ������ SET ���ش�.
	this->notePadForm->SetScrollInfo(SB_VERT, &this->siVertical, TRUE);

#if 0
	//(21.11.18.�߰�) 5. ��Ȥ���� �ڵ����� �� ������ũ���� ���� ��ǥ�� ����� �� �ִ� ��츦 ���� �߰�.
	//5.1. �ڵ����� �Ǿ��ִ� ���, �ڵ������� ĳ�� ���Ѵ�.
	int currentX;
	int currentY;

	if (this->notePadForm->isLineChangeButtonClicked == true && this->notePadForm->isComposition == false) {
		//5.1.1. �ڵ����� �� ĳ���� ��ġ�� ã�´�.
		this->notePadForm->lineChange->FindCaretBeforeLineChange(&currentX, &currentY);

		//5.1.2. �ڵ����� �� ������ ��ģ��.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

		//5.1.3. ��ģ ������ �ٽ� �ڵ������Ѵ�. (ON)
		this->windowWidth = this->notePadForm->m_rectCurHist.right - this->notePadForm->m_rectCurHist.left;
		this->notePadForm->lineChange->LineChangeButtonClicked();

		//5.1.4. �ڵ����� �� ĳ�� ���Ѵ�.
		this->notePadForm->lineChange->FindCaretAfterLineChange(currentX, currentY);
	
	}

	this->notePadForm->isComposition = true;
#endif

	CWnd* pWnd = AfxGetMainWnd();
	HWND hWnd = pWnd->m_hWnd;
	DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);

//#if 0
	//Horzizon ��ũ���� ��Ȱ��ȭ�� ���Ѵ�.
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

	//Vertical ��ũ���� ��Ȱ��ȭ�� ���Ѵ�.
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

	//(21.11.19.�߰�) ��Ȱ��ȭ & ������ ���� ��ũ���� �����.
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



	//3. siHorizon.nMax �����Ѵ�. --------------------------------------------------------------------------------
	//3.1. note�� length��ŭ �ݺ��Ѵ�. (���� �� ���� �ʺ� ���Ѵ�.)
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

		//3.1.1. �ش� ���� �ʺ� ���Ѵ�.

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
			// Location�� CalculateX�� ȣ���Ѵ�.
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
	//3.2. ���� �� ���� windowWidth �� �Ѱ��� ���,
	if (currentLargestWidth > this->windowWidth) {

		//3.2.1. ���� largestWidth ���� �� Ŭ ���, nMax�� �������ش�.
		if (currentLargestWidth > this->notePadForm->largestWidth) {
			this->siHorizon.nMax = currentLargestWidth;
			this->notePadForm->largestWidth = currentLargestWidth;
		}
	}
	//3.3. ���� �� ���� windowWidth �� �� �Ѱ��� ���,
	else {
		//3.3.1. 0���� �۰ų� ���� ���, nMax�� windowWidth�� �����Ѵ�.
		if (currentLargestWidth <= 0) {
			this->siHorizon.nMax = 0;
		}
	}
#endif

	//5.2. nMax�� �ش� �ʺ�� ġȯ�Ѵ�.
	this->notePadForm->SetScrollRange(SB_HORZ, 0, 1000, FALSE);
	this->notePadForm->largestWidth = currentLargestWidth;
	this->siHorizon.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	//this->siHorizon.fMask = SIF_ALL & ~SIF_POS;
	this->siHorizon.nMin = 0;
	this->siHorizon.nPos = 0;
	this->siHorizon.nPage = this->windowWidth;
	this->notePadForm->SetScrollInfo(SB_HORZ, &this->siHorizon, TRUE);

	//---------------------------------------------------------------------------------------------------------------

	//4. siVertical.nMax �����Ѵ�.
	//4.1. Vertical�� ��ü ���̸� ���ϰ� nMax�� �������ش�.
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

	//6.3. ���� ������ �μ���� Vertical ������ SET ���ش�.
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










