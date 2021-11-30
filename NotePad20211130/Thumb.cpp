//Thumb.cpp
#include"Thumb.h"
#include"NotePadForm.h"
#include"Scroll.h"
#include"GetTextSize.h"
#include"Note.h"
#include"Location.h"

Thumb::Thumb(NotePadForm* notePadForm) {
	this->notePadForm = notePadForm;
}

Thumb::~Thumb() {
}

void Thumb::LineRight() {

	int strSizeX = this->notePadForm->getTextSize->GetX(97); //"��"�� ũ��

	//1. Horizon ��ũ�� ������ Get�Ѵ�.
	this->notePadForm->scroll->siHorizon.fMask = SIF_POS;
	this->notePadForm->GetScrollInfo(SB_HORZ, &this->notePadForm->scroll->siHorizon);
	this->notePadForm->scroll->beforeX = this->notePadForm->scroll->siHorizon.nPos;


	//2.�ش� ��ũ���� nPos�� ���Ѵ�.
	//3. nPos�� 1�� ���Ѵ�.
	this->notePadForm->scroll->siHorizon.nPos = this->notePadForm->scroll->siHorizon.nPos + strSizeX;
	int newNPos = this->notePadForm->scroll->siHorizon.nPos;
	int horizonMax = this->notePadForm->scroll->siHorizon.nMax;
	int windowHeight = this->notePadForm->scroll->windowHeight;

	//4. nPos�� �ִ밪���� ũ�� �ش� �ִ밪���� ġȯ�Ѵ�.(�ִ밪: nMax - windowHeight)
#if 0
	if (newNPos >horizonMax - windowHeight) {
		newNPos = horizonMax - windowHeight;
		this->notePadForm->scroll->siHorizon.nPos = newNPos;
	}
#endif
	//5. ��ũ�� ������ set�Ѵ�.
	this->notePadForm->SetScrollInfo(SB_HORZ, &this->notePadForm->scroll->siHorizon, TRUE);
}

void Thumb::LineLeft() {

	int strSizeX = this->notePadForm->getTextSize->GetX(97);//"��"�� ũ��

	//1. Horizon ��ũ�� ������ Get�Ѵ�.
	this->notePadForm->scroll->siHorizon.fMask = SIF_POS;
	this->notePadForm->GetScrollInfo(SB_HORZ, &this->notePadForm->scroll->siHorizon);
	this->notePadForm->scroll->beforeX = this->notePadForm->scroll->siHorizon.nPos;


	//2.�ش� ��ũ���� nPos�� ���Ѵ�.
	//3. nPos�� 1�� ����.
	//	int newNPos = this->notePadForm->scroll->siHorizon.nPos--;
	this->notePadForm->scroll->siHorizon.nPos = this->notePadForm->scroll->siHorizon.nPos - strSizeX;
	int newNPos = this->notePadForm->scroll->siHorizon.nPos;


	//4. nPos�� 0���� ������ 0���� ġȯ�Ѵ�.
	if (newNPos < 0) {
		this->notePadForm->scroll->siHorizon.nPos = 0;
	}

	//5. ��ũ�� ������ set�Ѵ�.
	this->notePadForm->SetScrollInfo(SB_HORZ, &this->notePadForm->scroll->siHorizon, TRUE);

}

void Thumb::PageLeft() {

	//1. Horizon ��ũ�� ������ Get�Ѵ�.
	this->notePadForm->scroll->siHorizon.fMask = SIF_POS| SIF_PAGE;
	this->notePadForm->GetScrollInfo(SB_HORZ, &this->notePadForm->scroll->siHorizon);
	this->notePadForm->scroll->beforeX = this->notePadForm->scroll->siHorizon.nPos;


	//2.�ش� ��ũ���� nPos�� ���Ѵ�.
	//3. nPos���� nPage�� ����.
	int pageX = this->notePadForm->scroll->siHorizon.nPage;
	this->notePadForm->scroll->siHorizon.nPos = this->notePadForm->scroll->siHorizon.nPos - pageX;
	int newNPos = this->notePadForm->scroll->siHorizon.nPos;


	//4. nPos�� 0���� ������ 0���� ġȯ�Ѵ�.
	if (newNPos < 0) {
		this->notePadForm->scroll->siHorizon.nPos = 0;
	}

	//5. ��ũ�� ������ set�Ѵ�.
	this->notePadForm->SetScrollInfo(SB_HORZ, &this->notePadForm->scroll->siHorizon, TRUE);
}

void Thumb::PageRight() {

	//1. Horizon ��ũ�� ������ Get�Ѵ�.
	this->notePadForm->scroll->siHorizon.fMask = SIF_POS | SIF_PAGE;
	this->notePadForm->GetScrollInfo(SB_HORZ, &this->notePadForm->scroll->siHorizon);
	this->notePadForm->scroll->beforeX = this->notePadForm->scroll->siHorizon.nPos;


	//2.�ش� ��ũ���� nPos�� ���Ѵ�.
	//3. nPos���� nPage�� ���Ѵ�..
	int pageX = this->notePadForm->scroll->siHorizon.nPage;
	this->notePadForm->scroll->siHorizon.nPos = this->notePadForm->scroll->siHorizon.nPos + pageX;
	int newNPos = this->notePadForm->scroll->siHorizon.nPos;

	//5. ��ũ�� ������ set�Ѵ�.
	this->notePadForm->SetScrollInfo(SB_HORZ, &this->notePadForm->scroll->siHorizon, TRUE);
}

void Thumb::LineUp() {

	int strSizeY = this->notePadForm->getTextSize->GetX(131);//"��"�� ����ũ��

	//1. Vertical ��ũ�� ������ Get�Ѵ�.
	this->notePadForm->scroll->siVertical.fMask = SIF_POS;
	this->notePadForm->GetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical);
	this->notePadForm->scroll->beforeY = this->notePadForm->scroll->siVertical.nPos;
	int beforeY = this->notePadForm->scroll->beforeY;


	//2.�ش� ��ũ���� nPos�� ���Ѵ�.
	//3. nPos�� 1�� ����.
	this->notePadForm->scroll->siVertical.nPos = this->notePadForm->scroll->siVertical.nPos - strSizeY;
	int newNPos = this->notePadForm->scroll->siVertical.nPos;


	//4. nPos�� 0���� ������ 0���� ġȯ�Ѵ�.
	if (newNPos < 0) {
		this->notePadForm->scroll->siVertical.nPos = 0;
	}

	//5. ��ũ�� ������ set�Ѵ�.
	this->notePadForm->SetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical, TRUE);


}

void Thumb::LineDown() {

	int strSizeY = this->notePadForm->getTextSize->GetX(131);//"��"�� ����ũ��

	//1. Vertical ��ũ�� ������ Get�Ѵ�.
	this->notePadForm->scroll->siVertical.fMask = SIF_POS;
	this->notePadForm->GetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical);
	this->notePadForm->scroll->beforeY = this->notePadForm->scroll->siVertical.nPos;
	int beforeY = this->notePadForm->scroll->beforeY;


	//2.�ش� ��ũ���� nPos�� ���Ѵ�.
	//3. nPos�� 1�� ���Ѵ�.
	this->notePadForm->scroll->siVertical.nPos = this->notePadForm->scroll->siVertical.nPos + strSizeY;
	//int newNPos = this->notePadForm->scroll->siVertical.nPos;

	//5. ��ũ�� ������ set�Ѵ�.
	this->notePadForm->SetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical, TRUE);
}

void Thumb::PageUp() {

#if 0
	//1. Vertical ��ũ�� ������ Get�Ѵ�.
	this->notePadForm->scroll->siVertical.fMask = SIF_POS | SIF_PAGE;
	this->notePadForm->GetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical);
	this->notePadForm->scroll->beforeY = this->notePadForm->scroll->siVertical.nPos;


	//2.�ش� ��ũ���� nPos�� ���Ѵ�.
	//3. nPos���� nPage�� ����.
	int pageY = this->notePadForm->scroll->siVertical.nPage;
	this->notePadForm->scroll->siVertical.nPos = this->notePadForm->scroll->siVertical.nPos - pageY;
	int newNPos = this->notePadForm->scroll->siVertical.nPos;


	//4. nPos�� 0���� ������ 0���� ġȯ�Ѵ�.
	if (newNPos < 0) {
		this->notePadForm->scroll->siVertical.nPos = 0;
	}

	//5. ��ũ�� ������ set�Ѵ�.
	this->notePadForm->SetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical, TRUE);
#endif
	if (this->notePadForm->scroll->siVertical.nPos > 0) {

		//(21.11.23.�������) Caret ����
        //1. ���� ĳ���� ��ǥ�� ���Ѵ�.
		int currentY = this->notePadForm->note->GetCurrent();
		Glyph* row = this->notePadForm->note->GetChild(currentY);
		int currentX = row->GetCurrent();

		//2. ���� y��ǥ�� firstTextY���� ���̸� ���Ѵ�.
		Long firstTextY = this->notePadForm->firstTextY;
		int differenceText = currentY - firstTextY;

		//(21.11.23.�������) nPos ���� *****************************************************************
		//3. Vertical ��ũ�� ������ Get�Ѵ�.
		this->notePadForm->scroll->siVertical.fMask = SIF_POS | SIF_PAGE;
		this->notePadForm->GetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical);

		//4. 0 ���� firstTextY -difference ������ ���� ���Ѵ�.
		Long endTextY = this->notePadForm->endTextY;
		int strSizeY = this->notePadForm->getTextSize->GetX(131);
		int difference = endTextY - firstTextY;
		Long changedFirstText = firstTextY - difference;
		Long changedNPos = strSizeY * changedFirstText;

		//5. nPos�� �������ش�.
		this->notePadForm->scroll->siVertical.nPos = changedNPos;

		//6. nPos�� 0���� ������ 0���� ġȯ�Ѵ�.
		if (changedNPos < 0) {
			this->notePadForm->scroll->siVertical.nPos = 0;
			changedFirstText = 0;
		}

		//7. ��ũ�� ������ set�Ѵ�.
		this->notePadForm->SetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical, TRUE);
		//*************************************************************************************************

		//8. changedFirstText + 1�� �ش� ���̸� ���� �ذ����� ĳ���� �̵��Ѵ�.
		int changedY = changedFirstText + differenceText;
		this->notePadForm->note->MoveCurrent(changedY);
		row = this->notePadForm->note->GetChild(changedY);

		//10. ���� row�� length�� currentX���� ũ�ų� ������ currentX�� �Ű��ش�.
		int rowLength = row->GetLength();
		if (rowLength >= currentX) {
			row->MoveCurrent(currentX);
		}
		else {
			row->MoveCurrent(rowLength);
		}

	}
}

void Thumb::PageDown() {

#if 0
	//1. Vertical ��ũ�� ������ Get�Ѵ�.
	this->notePadForm->scroll->siVertical.fMask = SIF_POS | SIF_PAGE;
	this->notePadForm->GetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical);
	this->notePadForm->scroll->beforeY = this->notePadForm->scroll->siVertical.nPos;


	//2.�ش� ��ũ���� nPos�� ���Ѵ�.
	//3. nPos���� nPage�� ���Ѵ�..
	int pageY = this->notePadForm->scroll->siVertical.nPage;
	this->notePadForm->scroll->siVertical.nPos = this->notePadForm->scroll->siVertical.nPos + pageY;

	//5. ��ũ�� ������ set�Ѵ�.
	this->notePadForm->SetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical, TRUE);
#endif
	Long firstTextY = this->notePadForm->firstTextY;
	Long endTextY = this->notePadForm->endTextY;
	int strSizeY = this->notePadForm->getTextSize->GetX(131);

	//0. �����뼱 nPos�� ���س��´�. (����÷ο� ����)
	int noteLength = this->notePadForm->note->GetLength();

	//0.1. ���� windowHeight���� ��� ���� �ִ��� Ȯ���Ѵ�.
	int windowHeightY = (endTextY - firstTextY) + 1;
	int maginot = (noteLength - windowHeightY);
	Long totalNPos = (strSizeY * maginot);

	if (this->notePadForm->scroll->siVertical.nPos < totalNPos) {
		//(21.11.23.�������) Caret ����
	    //1. ���� ĳ���� ��ǥ�� ���Ѵ�.
		int currentY = this->notePadForm->note->GetCurrent();
		Glyph* row = this->notePadForm->note->GetChild(currentY);
		int currentX = row->GetCurrent();

		//2. ���� y��ǥ�� firstTextY���� ���̸� ���Ѵ�.
		int difference = currentY - firstTextY;


		//(21.11.23.�������) nPos ���� *****************************************************************

		//3. Vertical ��ũ�� ������ Get�Ѵ�.
		this->notePadForm->scroll->siVertical.fMask = SIF_POS | SIF_PAGE;
		this->notePadForm->GetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical);

		//4. 0��° �ٺ��� textOutY-1��° �ٱ����� ���̸� ���Ѵ�.
		Long changedNPos = strSizeY * endTextY;

		//5.�ش� ��ũ���� nPos�� �����Ѵ�.
		this->notePadForm->scroll->siVertical.nPos = changedNPos;

		//7. ���� endTextY�� �����뼱 nPos���� Ŭ ���, �����뼱 nPos�� �����Ѵ�.
		if (endTextY > maginot) {
			this->notePadForm->scroll->siVertical.nPos = totalNPos;
			endTextY = maginot;
		}

		//8. ��ũ�� ������ set�Ѵ�.
		this->notePadForm->SetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical, TRUE);
		//***********************************************************************************************

		//9. endTextY + 1�� �ش� ���̸� ���� �ذ����� ĳ���� �̵��Ѵ�.
		int changedY = endTextY + difference;
		this->notePadForm->note->MoveCurrent(changedY);
		row = this->notePadForm->note->GetChild(changedY);

		//10. ���� row�� length�� currentX���� ũ�ų� ������ currentX�� �Ű��ش�.
		int rowLength = row->GetLength();
		if (rowLength >= currentX) {
			row->MoveCurrent(currentX);
		}
		else {
			row->MoveCurrent(rowLength);
		}

	}
}

void Thumb::PageUpForMouse() {
	if (this->notePadForm->scroll->siVertical.nPos > 0) {

		//(21.11.23.�������) nPos ���� *****************************************************************
		//3. Vertical ��ũ�� ������ Get�Ѵ�.
		this->notePadForm->scroll->siVertical.fMask = SIF_POS | SIF_PAGE;
		this->notePadForm->GetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical);

		//4. 0 ���� firstTextY -difference ������ ���� ���Ѵ�.
		Long firstTextY = this->notePadForm->firstTextY;
		Long endTextY = this->notePadForm->endTextY;
		int strSizeY = this->notePadForm->getTextSize->GetX(131);
		int difference = endTextY - firstTextY;
		Long changedFirstText = firstTextY - difference;
		Long changedNPos = strSizeY * changedFirstText;

		//5. nPos�� �������ش�.
		this->notePadForm->scroll->siVertical.nPos = changedNPos;

		//6. nPos�� 0���� ������ 0���� ġȯ�Ѵ�.
		if (changedNPos < 0) {
			this->notePadForm->scroll->siVertical.nPos = 0;
			changedFirstText = 0;
		}

		//7. ��ũ�� ������ set�Ѵ�.
		this->notePadForm->SetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical, TRUE);
		//************************************************************************************************
	}
}

void Thumb::PageDownForMouse() {
	Long firstTextY = this->notePadForm->firstTextY;
	Long endTextY = this->notePadForm->endTextY;
	int strSizeY = this->notePadForm->getTextSize->GetX(131);

	//0. �����뼱 nPos�� ���س��´�. (����÷ο� ����)
	int noteLength = this->notePadForm->note->GetLength();

	//0.1. ���� windowHeight���� ��� ���� �ִ��� Ȯ���Ѵ�.
	int windowHeightY = (endTextY - firstTextY) + 1;
	int maginot = (noteLength - windowHeightY);
	Long totalNPos = (strSizeY * maginot);

	if (this->notePadForm->scroll->siVertical.nPos < totalNPos) {

		//(21.11.23.�������) nPos ���� *****************************************************************

		//3. Vertical ��ũ�� ������ Get�Ѵ�.
		this->notePadForm->scroll->siVertical.fMask = SIF_POS | SIF_PAGE;
		this->notePadForm->GetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical);

		//4. 0��° �ٺ��� textOutY-1��° �ٱ����� ���̸� ���Ѵ�.
		Long changedNPos = strSizeY * endTextY;

		//5.�ش� ��ũ���� nPos�� �����Ѵ�.
		this->notePadForm->scroll->siVertical.nPos = changedNPos;

		//7. ���� endTextY�� �����뼱 nPos���� Ŭ ���, �����뼱 nPos�� �����Ѵ�.
		if (endTextY > maginot) {
			this->notePadForm->scroll->siVertical.nPos = totalNPos;
			endTextY = maginot;
		}

		//8. ��ũ�� ������ set�Ѵ�.
		this->notePadForm->SetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical, TRUE);
		//**********************************************************************************************

	}
}



void Thumb::ThumbTrackHorizon() {
    //1. Horizon ��ũ�� ������ Get�Ѵ�.
	this->notePadForm->scroll->siHorizon.fMask = SIF_POS;
	this->notePadForm->GetScrollInfo(SB_HORZ, &this->notePadForm->scroll->siHorizon);
	this->notePadForm->scroll->beforeX = this->notePadForm->scroll->siHorizon.nPos;


	//2.�ش� ��ũ���� nPos�� ���Ѵ�.
	//3. nTrackPos�� nPos�� �ٲ۴�.
	this->notePadForm->scroll->siHorizon.nPos = this->notePadForm->scroll->siHorizon.nTrackPos;
	int newNPos = this->notePadForm->scroll->siHorizon.nPos;


	//4. ��ũ�� ������ set�Ѵ�.
	this->notePadForm->SetScrollInfo(SB_HORZ, &this->notePadForm->scroll->siHorizon, TRUE);

}

void Thumb::ThumbTrackVertical() {
//1. Vertical ��ũ�� ������ Get�Ѵ�.
	this->notePadForm->scroll->siVertical.fMask = SIF_POS;
	this->notePadForm->GetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical);
	this->notePadForm->scroll->beforeY = this->notePadForm->scroll->siVertical.nPos;


	//2.�ش� ��ũ���� nPos�� ���Ѵ�.
	//3. nTrackPos�� nPos�� �ٲ۴�.
	this->notePadForm->scroll->siVertical.nPos = this->notePadForm->scroll->siVertical.nTrackPos;
	int newNPos = this->notePadForm->scroll->siVertical.nPos;


	//4. ��ũ�� ������ set�Ѵ�.
	this->notePadForm->SetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical, TRUE);

}

void Thumb::Top() {
	//1. Vertical ��ũ�� ������ Get�Ѵ�.
	this->notePadForm->scroll->siVertical.fMask = SIF_POS;
	this->notePadForm->GetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical);
	this->notePadForm->scroll->beforeY = this->notePadForm->scroll->siVertical.nPos;


	//2.�ش� ��ũ���� nPos�� ���Ѵ�.
	//3. nTrackPos�� nPos�� �ٲ۴�.
	this->notePadForm->scroll->siVertical.nPos = this->notePadForm->scroll->siVertical.nMin;
	int newNPos = this->notePadForm->scroll->siVertical.nPos;


	//4. ��ũ�� ������ set�Ѵ�.
	this->notePadForm->SetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical, TRUE);

}

#if 0

void Thumb::MoveThumbAppend(Long horizonRangeValue, Long verticalRangeValue, Long x, Long y) {
	//1. horizon�� vertical�� MaxRangeValue, x, y���� �Է¹޴´�.
	//2. horizonRangeValue-x�� ���� horizonSize�� �����Ѵ�.
	this->horizonSize = horizonRangeValue - x;
	//3. verticalRangeValue-y�� ���� verticalSize�� �����Ѵ�.
	this->verticalSize = verticalRangeValue - y;
}

void Thumb::MoveThumbInsert(char(*character), Long size) { //�����̸� size�� ����, ���ڸ� size�� �ʺ�� ���´�.
	//1. MoveThumbInsert�߻��� ��
	  //1.1. �߰��� ���ڰ� ������ �ƴѰ��, �ش� �ʺ�ŭ ���� Thumb ũ�⸦ ���������� ���δ�.
	  if (character[0] != '\n' && character[0] != '\r') {
		  this->horizonSize = this->horizonSize - size;
	  }
	  //1.2. �߰��� ���ڰ� ������ ���, �ش� ���̸�ŭ vertical Thumb ũ�⸦ ������ ���δ�.
	  else if (character[0] == '\n' || character[0] == '\r') {
		  this->verticalSize = this->verticalSize - size;
	  }
}

#endif
