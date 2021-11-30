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
	void FindCaretBeforeLineChangeForFirstXY(int* beforeLineChangeCurrentX, int* currentY); //�̺�Ʈ��
	void FindCaretBeforeLineChangeForEndXY(int* beforeLineChangeCurrentX, int* currentY); //�̺�Ʈ��
	void FindCaretBeforeLineChange(int firstX, int firstY, int* beforeLineChangeCurrentX, int* currentY);
	void FindCaretAfterLineChange(int beforeLineChangeCurrentX, int currentY);
	void FindCaretAfterLineChangeForFirst(int beforeLineChangeCurrentX, int currentY); //OnImeComposition��(0��°�϶�)
	void FindCaretAfterLineChange(int beforeLineChangeCurrentX, int currentY, int *afterCurrentX, int *afterCurrentY);
	void SmallerThanBefore();
	void BiggerThanBefore();

	//(21.09.23 �߰�) ������ ���� ���� �Լ� �����ε�
	void LineChangeButtonClicked(int width);
	int FindSplitLocation(Glyph* row, int width);
	//(21.10.01 �߰�) Undo, Redo�� ���� ����(windowWidth) ���ִ� �� ����.
	int FindSplitLocationForMargin(Glyph* row, int width);

	//(21.11.30. �߰�) �ػ� ����� �ڵ�����. ******************************
	void LineChangeButtonClickedForDpi(int width);
	int FindSplitLocationForDpi(Glyph* row, int width);
	//***********************************************************************


private:
	NotePadForm* notePadForm;
};

#endif//_LINECHANGE_H

