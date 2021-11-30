//LineChange.h
#ifndef _LINECHANGE_H
#define _LINECHANGE_H

class Glyph;
class NotePadForm;
class LineChange {
public:
	LineChange(NotePadForm* notePadForm);
	~LineChange();
	int FindSplitLocation(Glyph* row);
	int FindSplitLocationByWriting(Glyph* row);
	void FillRestDummyRow(int currentY);
	void LineChangeButtonClicked();
	void LineChangeButtonNotClicked();
	void FindCaretBeforeLineChange(int* beforeLineChangeCurrentX, int* currentY);
	void FindCaretBeforeLineChangeForFirstXY(int* beforeLineChangeCurrentX, int* currentY); //이벤트성
	void FindCaretBeforeLineChangeForEndXY(int* beforeLineChangeCurrentX, int* currentY); //이벤트성
	void FindCaretBeforeLineChange(int firstX, int firstY, int* beforeLineChangeCurrentX, int* currentY);
	void FindCaretAfterLineChange(int beforeLineChangeCurrentX, int currentY);
	void FindCaretAfterLineChangeForFirst(int beforeLineChangeCurrentX, int currentY); //OnImeComposition용(0번째일때)
	void FindCaretAfterLineChange(int beforeLineChangeCurrentX, int currentY, int *afterCurrentX, int *afterCurrentY);
	void SmallerThanBefore();
	void BiggerThanBefore();

	//(21.09.23 추가) 페이지 설정 전용 함수 오버로딩
	void LineChangeButtonClicked(int width);
	int FindSplitLocation(Glyph* row, int width);
	//(21.10.01 추가) Undo, Redo시 여유 간격(windowWidth) 빼주는 거 적용.
	int FindSplitLocationForMargin(Glyph* row, int width);

	//(21.11.30. 추가) 해상도 고려한 자동개행. ******************************
	void LineChangeButtonClickedForDpi(int width);
	int FindSplitLocationForDpi(Glyph* row, int width);
	//***********************************************************************


private:
	NotePadForm* notePadForm;
};

#endif//_LINECHANGE_H

