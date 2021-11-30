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

	int strSizeX = this->notePadForm->getTextSize->GetX(97); //"ㄱ"의 크기

	//1. Horizon 스크롤 정보를 Get한다.
	this->notePadForm->scroll->siHorizon.fMask = SIF_POS;
	this->notePadForm->GetScrollInfo(SB_HORZ, &this->notePadForm->scroll->siHorizon);
	this->notePadForm->scroll->beforeX = this->notePadForm->scroll->siHorizon.nPos;


	//2.해당 스크롤의 nPos를 구한다.
	//3. nPos를 1씩 더한다.
	this->notePadForm->scroll->siHorizon.nPos = this->notePadForm->scroll->siHorizon.nPos + strSizeX;
	int newNPos = this->notePadForm->scroll->siHorizon.nPos;
	int horizonMax = this->notePadForm->scroll->siHorizon.nMax;
	int windowHeight = this->notePadForm->scroll->windowHeight;

	//4. nPos의 최대값보다 크면 해당 최대값으로 치환한다.(최대값: nMax - windowHeight)
#if 0
	if (newNPos >horizonMax - windowHeight) {
		newNPos = horizonMax - windowHeight;
		this->notePadForm->scroll->siHorizon.nPos = newNPos;
	}
#endif
	//5. 스크롤 정보를 set한다.
	this->notePadForm->SetScrollInfo(SB_HORZ, &this->notePadForm->scroll->siHorizon, TRUE);
}

void Thumb::LineLeft() {

	int strSizeX = this->notePadForm->getTextSize->GetX(97);//"ㄱ"의 크기

	//1. Horizon 스크롤 정보를 Get한다.
	this->notePadForm->scroll->siHorizon.fMask = SIF_POS;
	this->notePadForm->GetScrollInfo(SB_HORZ, &this->notePadForm->scroll->siHorizon);
	this->notePadForm->scroll->beforeX = this->notePadForm->scroll->siHorizon.nPos;


	//2.해당 스크롤의 nPos를 구한다.
	//3. nPos를 1씩 뺀다.
	//	int newNPos = this->notePadForm->scroll->siHorizon.nPos--;
	this->notePadForm->scroll->siHorizon.nPos = this->notePadForm->scroll->siHorizon.nPos - strSizeX;
	int newNPos = this->notePadForm->scroll->siHorizon.nPos;


	//4. nPos가 0보다 작으면 0으로 치환한다.
	if (newNPos < 0) {
		this->notePadForm->scroll->siHorizon.nPos = 0;
	}

	//5. 스크롤 정보를 set한다.
	this->notePadForm->SetScrollInfo(SB_HORZ, &this->notePadForm->scroll->siHorizon, TRUE);

}

void Thumb::PageLeft() {

	//1. Horizon 스크롤 정보를 Get한다.
	this->notePadForm->scroll->siHorizon.fMask = SIF_POS| SIF_PAGE;
	this->notePadForm->GetScrollInfo(SB_HORZ, &this->notePadForm->scroll->siHorizon);
	this->notePadForm->scroll->beforeX = this->notePadForm->scroll->siHorizon.nPos;


	//2.해당 스크롤의 nPos를 구한다.
	//3. nPos에서 nPage를 뺀다.
	int pageX = this->notePadForm->scroll->siHorizon.nPage;
	this->notePadForm->scroll->siHorizon.nPos = this->notePadForm->scroll->siHorizon.nPos - pageX;
	int newNPos = this->notePadForm->scroll->siHorizon.nPos;


	//4. nPos가 0보다 작으면 0으로 치환한다.
	if (newNPos < 0) {
		this->notePadForm->scroll->siHorizon.nPos = 0;
	}

	//5. 스크롤 정보를 set한다.
	this->notePadForm->SetScrollInfo(SB_HORZ, &this->notePadForm->scroll->siHorizon, TRUE);
}

void Thumb::PageRight() {

	//1. Horizon 스크롤 정보를 Get한다.
	this->notePadForm->scroll->siHorizon.fMask = SIF_POS | SIF_PAGE;
	this->notePadForm->GetScrollInfo(SB_HORZ, &this->notePadForm->scroll->siHorizon);
	this->notePadForm->scroll->beforeX = this->notePadForm->scroll->siHorizon.nPos;


	//2.해당 스크롤의 nPos를 구한다.
	//3. nPos에서 nPage를 더한다..
	int pageX = this->notePadForm->scroll->siHorizon.nPage;
	this->notePadForm->scroll->siHorizon.nPos = this->notePadForm->scroll->siHorizon.nPos + pageX;
	int newNPos = this->notePadForm->scroll->siHorizon.nPos;

	//5. 스크롤 정보를 set한다.
	this->notePadForm->SetScrollInfo(SB_HORZ, &this->notePadForm->scroll->siHorizon, TRUE);
}

void Thumb::LineUp() {

	int strSizeY = this->notePadForm->getTextSize->GetX(131);//"ㄱ"의 세로크기

	//1. Vertical 스크롤 정보를 Get한다.
	this->notePadForm->scroll->siVertical.fMask = SIF_POS;
	this->notePadForm->GetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical);
	this->notePadForm->scroll->beforeY = this->notePadForm->scroll->siVertical.nPos;
	int beforeY = this->notePadForm->scroll->beforeY;


	//2.해당 스크롤의 nPos를 구한다.
	//3. nPos를 1씩 뺀다.
	this->notePadForm->scroll->siVertical.nPos = this->notePadForm->scroll->siVertical.nPos - strSizeY;
	int newNPos = this->notePadForm->scroll->siVertical.nPos;


	//4. nPos가 0보다 작으면 0으로 치환한다.
	if (newNPos < 0) {
		this->notePadForm->scroll->siVertical.nPos = 0;
	}

	//5. 스크롤 정보를 set한다.
	this->notePadForm->SetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical, TRUE);


}

void Thumb::LineDown() {

	int strSizeY = this->notePadForm->getTextSize->GetX(131);//"ㄱ"의 세로크기

	//1. Vertical 스크롤 정보를 Get한다.
	this->notePadForm->scroll->siVertical.fMask = SIF_POS;
	this->notePadForm->GetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical);
	this->notePadForm->scroll->beforeY = this->notePadForm->scroll->siVertical.nPos;
	int beforeY = this->notePadForm->scroll->beforeY;


	//2.해당 스크롤의 nPos를 구한다.
	//3. nPos를 1씩 더한다.
	this->notePadForm->scroll->siVertical.nPos = this->notePadForm->scroll->siVertical.nPos + strSizeY;
	//int newNPos = this->notePadForm->scroll->siVertical.nPos;

	//5. 스크롤 정보를 set한다.
	this->notePadForm->SetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical, TRUE);
}

void Thumb::PageUp() {

#if 0
	//1. Vertical 스크롤 정보를 Get한다.
	this->notePadForm->scroll->siVertical.fMask = SIF_POS | SIF_PAGE;
	this->notePadForm->GetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical);
	this->notePadForm->scroll->beforeY = this->notePadForm->scroll->siVertical.nPos;


	//2.해당 스크롤의 nPos를 구한다.
	//3. nPos에서 nPage를 뺀다.
	int pageY = this->notePadForm->scroll->siVertical.nPage;
	this->notePadForm->scroll->siVertical.nPos = this->notePadForm->scroll->siVertical.nPos - pageY;
	int newNPos = this->notePadForm->scroll->siVertical.nPos;


	//4. nPos가 0보다 작으면 0으로 치환한다.
	if (newNPos < 0) {
		this->notePadForm->scroll->siVertical.nPos = 0;
	}

	//5. 스크롤 정보를 set한다.
	this->notePadForm->SetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical, TRUE);
#endif
	if (this->notePadForm->scroll->siVertical.nPos > 0) {

		//(21.11.23.전면수정) Caret 변경
        //1. 현재 캐럿의 좌표를 구한다.
		int currentY = this->notePadForm->note->GetCurrent();
		Glyph* row = this->notePadForm->note->GetChild(currentY);
		int currentX = row->GetCurrent();

		//2. 현재 y좌표와 firstTextY와의 차이를 구한다.
		Long firstTextY = this->notePadForm->firstTextY;
		int differenceText = currentY - firstTextY;

		//(21.11.23.전면수정) nPos 변경 *****************************************************************
		//3. Vertical 스크롤 정보를 Get한다.
		this->notePadForm->scroll->siVertical.fMask = SIF_POS | SIF_PAGE;
		this->notePadForm->GetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical);

		//4. 0 부터 firstTextY -difference 까지의 높이 구한다.
		Long endTextY = this->notePadForm->endTextY;
		int strSizeY = this->notePadForm->getTextSize->GetX(131);
		int difference = endTextY - firstTextY;
		Long changedFirstText = firstTextY - difference;
		Long changedNPos = strSizeY * changedFirstText;

		//5. nPos를 변경해준다.
		this->notePadForm->scroll->siVertical.nPos = changedNPos;

		//6. nPos가 0보다 작으면 0으로 치환한다.
		if (changedNPos < 0) {
			this->notePadForm->scroll->siVertical.nPos = 0;
			changedFirstText = 0;
		}

		//7. 스크롤 정보를 set한다.
		this->notePadForm->SetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical, TRUE);
		//*************************************************************************************************

		//8. changedFirstText + 1에 해당 차이를 더해 준곳으로 캐럿을 이동한다.
		int changedY = changedFirstText + differenceText;
		this->notePadForm->note->MoveCurrent(changedY);
		row = this->notePadForm->note->GetChild(changedY);

		//10. 현재 row의 length가 currentX보다 크거나 같으면 currentX로 옮겨준다.
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
	//1. Vertical 스크롤 정보를 Get한다.
	this->notePadForm->scroll->siVertical.fMask = SIF_POS | SIF_PAGE;
	this->notePadForm->GetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical);
	this->notePadForm->scroll->beforeY = this->notePadForm->scroll->siVertical.nPos;


	//2.해당 스크롤의 nPos를 구한다.
	//3. nPos에서 nPage를 더한다..
	int pageY = this->notePadForm->scroll->siVertical.nPage;
	this->notePadForm->scroll->siVertical.nPos = this->notePadForm->scroll->siVertical.nPos + pageY;

	//5. 스크롤 정보를 set한다.
	this->notePadForm->SetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical, TRUE);
#endif
	Long firstTextY = this->notePadForm->firstTextY;
	Long endTextY = this->notePadForm->endTextY;
	int strSizeY = this->notePadForm->getTextSize->GetX(131);

	//0. 마지노선 nPos를 구해놓는다. (언더플로우 정리)
	int noteLength = this->notePadForm->note->GetLength();

	//0.1. 현재 windowHeight까지 몇개의 줄이 있는지 확인한다.
	int windowHeightY = (endTextY - firstTextY) + 1;
	int maginot = (noteLength - windowHeightY);
	Long totalNPos = (strSizeY * maginot);

	if (this->notePadForm->scroll->siVertical.nPos < totalNPos) {
		//(21.11.23.전면수정) Caret 변경
	    //1. 현재 캐럿의 좌표를 구한다.
		int currentY = this->notePadForm->note->GetCurrent();
		Glyph* row = this->notePadForm->note->GetChild(currentY);
		int currentX = row->GetCurrent();

		//2. 현재 y좌표와 firstTextY와의 차이를 구한다.
		int difference = currentY - firstTextY;


		//(21.11.23.전면수정) nPos 변경 *****************************************************************

		//3. Vertical 스크롤 정보를 Get한다.
		this->notePadForm->scroll->siVertical.fMask = SIF_POS | SIF_PAGE;
		this->notePadForm->GetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical);

		//4. 0번째 줄부터 textOutY-1번째 줄까지의 높이를 구한다.
		Long changedNPos = strSizeY * endTextY;

		//5.해당 스크롤의 nPos를 변경한다.
		this->notePadForm->scroll->siVertical.nPos = changedNPos;

		//7. 현재 endTextY가 마지노선 nPos보다 클 경우, 마지노선 nPos로 변경한다.
		if (endTextY > maginot) {
			this->notePadForm->scroll->siVertical.nPos = totalNPos;
			endTextY = maginot;
		}

		//8. 스크롤 정보를 set한다.
		this->notePadForm->SetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical, TRUE);
		//***********************************************************************************************

		//9. endTextY + 1에 해당 차이를 더해 준곳으로 캐럿을 이동한다.
		int changedY = endTextY + difference;
		this->notePadForm->note->MoveCurrent(changedY);
		row = this->notePadForm->note->GetChild(changedY);

		//10. 현재 row의 length가 currentX보다 크거나 같으면 currentX로 옮겨준다.
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

		//(21.11.23.전면수정) nPos 변경 *****************************************************************
		//3. Vertical 스크롤 정보를 Get한다.
		this->notePadForm->scroll->siVertical.fMask = SIF_POS | SIF_PAGE;
		this->notePadForm->GetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical);

		//4. 0 부터 firstTextY -difference 까지의 높이 구한다.
		Long firstTextY = this->notePadForm->firstTextY;
		Long endTextY = this->notePadForm->endTextY;
		int strSizeY = this->notePadForm->getTextSize->GetX(131);
		int difference = endTextY - firstTextY;
		Long changedFirstText = firstTextY - difference;
		Long changedNPos = strSizeY * changedFirstText;

		//5. nPos를 변경해준다.
		this->notePadForm->scroll->siVertical.nPos = changedNPos;

		//6. nPos가 0보다 작으면 0으로 치환한다.
		if (changedNPos < 0) {
			this->notePadForm->scroll->siVertical.nPos = 0;
			changedFirstText = 0;
		}

		//7. 스크롤 정보를 set한다.
		this->notePadForm->SetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical, TRUE);
		//************************************************************************************************
	}
}

void Thumb::PageDownForMouse() {
	Long firstTextY = this->notePadForm->firstTextY;
	Long endTextY = this->notePadForm->endTextY;
	int strSizeY = this->notePadForm->getTextSize->GetX(131);

	//0. 마지노선 nPos를 구해놓는다. (언더플로우 정리)
	int noteLength = this->notePadForm->note->GetLength();

	//0.1. 현재 windowHeight까지 몇개의 줄이 있는지 확인한다.
	int windowHeightY = (endTextY - firstTextY) + 1;
	int maginot = (noteLength - windowHeightY);
	Long totalNPos = (strSizeY * maginot);

	if (this->notePadForm->scroll->siVertical.nPos < totalNPos) {

		//(21.11.23.전면수정) nPos 변경 *****************************************************************

		//3. Vertical 스크롤 정보를 Get한다.
		this->notePadForm->scroll->siVertical.fMask = SIF_POS | SIF_PAGE;
		this->notePadForm->GetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical);

		//4. 0번째 줄부터 textOutY-1번째 줄까지의 높이를 구한다.
		Long changedNPos = strSizeY * endTextY;

		//5.해당 스크롤의 nPos를 변경한다.
		this->notePadForm->scroll->siVertical.nPos = changedNPos;

		//7. 현재 endTextY가 마지노선 nPos보다 클 경우, 마지노선 nPos로 변경한다.
		if (endTextY > maginot) {
			this->notePadForm->scroll->siVertical.nPos = totalNPos;
			endTextY = maginot;
		}

		//8. 스크롤 정보를 set한다.
		this->notePadForm->SetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical, TRUE);
		//**********************************************************************************************

	}
}



void Thumb::ThumbTrackHorizon() {
    //1. Horizon 스크롤 정보를 Get한다.
	this->notePadForm->scroll->siHorizon.fMask = SIF_POS;
	this->notePadForm->GetScrollInfo(SB_HORZ, &this->notePadForm->scroll->siHorizon);
	this->notePadForm->scroll->beforeX = this->notePadForm->scroll->siHorizon.nPos;


	//2.해당 스크롤의 nPos를 구한다.
	//3. nTrackPos를 nPos로 바꾼다.
	this->notePadForm->scroll->siHorizon.nPos = this->notePadForm->scroll->siHorizon.nTrackPos;
	int newNPos = this->notePadForm->scroll->siHorizon.nPos;


	//4. 스크롤 정보를 set한다.
	this->notePadForm->SetScrollInfo(SB_HORZ, &this->notePadForm->scroll->siHorizon, TRUE);

}

void Thumb::ThumbTrackVertical() {
//1. Vertical 스크롤 정보를 Get한다.
	this->notePadForm->scroll->siVertical.fMask = SIF_POS;
	this->notePadForm->GetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical);
	this->notePadForm->scroll->beforeY = this->notePadForm->scroll->siVertical.nPos;


	//2.해당 스크롤의 nPos를 구한다.
	//3. nTrackPos를 nPos로 바꾼다.
	this->notePadForm->scroll->siVertical.nPos = this->notePadForm->scroll->siVertical.nTrackPos;
	int newNPos = this->notePadForm->scroll->siVertical.nPos;


	//4. 스크롤 정보를 set한다.
	this->notePadForm->SetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical, TRUE);

}

void Thumb::Top() {
	//1. Vertical 스크롤 정보를 Get한다.
	this->notePadForm->scroll->siVertical.fMask = SIF_POS;
	this->notePadForm->GetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical);
	this->notePadForm->scroll->beforeY = this->notePadForm->scroll->siVertical.nPos;


	//2.해당 스크롤의 nPos를 구한다.
	//3. nTrackPos를 nPos로 바꾼다.
	this->notePadForm->scroll->siVertical.nPos = this->notePadForm->scroll->siVertical.nMin;
	int newNPos = this->notePadForm->scroll->siVertical.nPos;


	//4. 스크롤 정보를 set한다.
	this->notePadForm->SetScrollInfo(SB_VERT, &this->notePadForm->scroll->siVertical, TRUE);

}

#if 0

void Thumb::MoveThumbAppend(Long horizonRangeValue, Long verticalRangeValue, Long x, Long y) {
	//1. horizon과 vertical의 MaxRangeValue, x, y값을 입력받는다.
	//2. horizonRangeValue-x의 값을 horizonSize로 설정한다.
	this->horizonSize = horizonRangeValue - x;
	//3. verticalRangeValue-y의 값을 verticalSize로 설정한다.
	this->verticalSize = verticalRangeValue - y;
}

void Thumb::MoveThumbInsert(char(*character), Long size) { //개행이면 size가 높이, 문자면 size가 너비로 들어온다.
	//1. MoveThumbInsert발생할 때
	  //1.1. 추가한 문자가 개행이 아닌경우, 해당 너비만큼 현재 Thumb 크기를 오른쪽으로 줄인다.
	  if (character[0] != '\n' && character[0] != '\r') {
		  this->horizonSize = this->horizonSize - size;
	  }
	  //1.2. 추가한 문자가 개행인 경우, 해당 높이만큼 vertical Thumb 크기를 밑으로 줄인다.
	  else if (character[0] == '\n' || character[0] == '\r') {
		  this->verticalSize = this->verticalSize - size;
	  }
}

#endif
