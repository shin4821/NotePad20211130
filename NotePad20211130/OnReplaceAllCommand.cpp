//OnReplaceAllCommand.cpp
#include"OnReplaceAllCommand.h"
#include"NotePadForm.h"
#include"ReplaceDialog.h"
#include"FindSameText.h"
#include"SelectWords.h"
#include"Note.h"
#include"OnReplaceCommand.h"
#include"LineChange.h"
#include"Scroll.h"
#include"KeyAction.h"
#include"FactoryProduct.h"
#include"FindSameText.h"
#include"FindAndReplace.h"
#include"CommandStory.h"
#include"UndoList.h"

OnReplaceAllCommand::OnReplaceAllCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {

	this->isChecked = false;
	this->notePadForm->isUndo = false;
	this->notePadForm->isThereSelected = false;
	this->isReplaced = false;
	this->firstX = 0;
	this->firstY = 0;
	this->beforeEndX = 0;
	this->beforeEndY = 0;
	this->afterEndX = 0;
	this->afterEndY = 0;
	this->isLineChangeButtonClicked = false;
	this->isFirst = true;
	this->isReplacedForMessage = false;
}

OnReplaceAllCommand::~OnReplaceAllCommand() {
}

void OnReplaceAllCommand::Execute() {
	BOOL isFirst = true;
	int isReplaced = 0;

	//1. ���� Ŀ���� ��ġ�� (0,0)���� �������ش�.
	this->notePadForm->note->MoveCurrent(0);
	Glyph* row = this->notePadForm->note->GetChild(0);
	row->MoveCurrent(0);


	//2. ���� ������ �ɼǵ��� Ȯ���Ѵ�. *******************************************************************
	this->notePadForm->findSameText->exception = false;

	//1.0. �ٲ� ������ Ȯ���Ѵ�.
	this->notePadForm->replaceDialog->GetDlgItem(IDC_EDIT_REPLACETEXT)->GetWindowText(this->notePadForm->findSameText->replaceStr);

	//1.1. ã�� ������ Ȯ���Ѵ�.
	this->notePadForm->replaceDialog->GetDlgItem(IDC_EDIT_FINDTEXT)->GetWindowText(this->notePadForm->findSameText->findStr);

	//1.2. ����ڰ� �˻� ���ڿ��� ��/�ҹ��ڸ� ��Ȯ�� ��ġ��Ű���� Ȯ���Ѵ�. (����:true, �ȱ���:false)
	this->notePadForm->findSameText->matchCase =
		((CButton*)this->notePadForm->replaceDialog->GetDlgItem(IDC_BUTTON_MATCHCASE))->GetCheck();

	//1.3. ����ڰ� �˻��� �Ʒ��� �������� �����ϴ��� Ȯ���Ѵ� (�Ʒ���:true, ����:false)
	this->notePadForm->findSameText->searchDown = true;

	//1.4. ����ڰ� ��ü �ܾ ��ġ��ų�� ���θ� Ȯ���Ѵ�.
	this->notePadForm->findSameText->searchAround =
		((CButton*)this->notePadForm->replaceDialog->GetDlgItem(IDC_BUTTON_SEARCHAROUND))->GetCheck();
	//******************************************************************************************************

#if 0
	//2. ���� �ܾ �ִ� ���� �ݺ��Ѵ�.
	if (isFirst == true) {
		this->isFirst = true;
		this->notePadForm->selectWords->firstX = 0;
		this->notePadForm->selectWords->firstY = 0;
		this->notePadForm->selectWords->endX = 0;
		this->notePadForm->selectWords->endY = 0;
	}
#endif

	//FindSameTextŬ������ ReplaceAndSearchText�� ȣ���Ѵ�.
	this->notePadForm->findSameText->ReplaceAndSearchText();

	//(21.09.19 �߰�) ��� �����Ѵ�.
	this->firstX = this->notePadForm->selectWords->beforeFirstX;
	this->firstY = this->notePadForm->selectWords->beforeFirstY;
	this->beforeEndX = this->notePadForm->selectWords->beforeEndX;
	this->beforeEndY = this->notePadForm->selectWords->beforeEndY;
	this->afterEndX = this->notePadForm->selectWords->afterEndX;
	this->afterEndY = this->notePadForm->selectWords->afterEndY;
	this->replaceStr = this->notePadForm->findSameText->replaceStr;
	this->findStr = this->notePadForm->findSameText->findStr;
	this->isReplaced = this->notePadForm->selectWords->isReplaced;
	this->isLineChangeButtonClicked = this->notePadForm->selectWords->isLineChangeButtonClicked;
	this->windowWidth = this->notePadForm->selectWords->windowWidth;

#if 0
	if (this->isReplacedForMessage == false && this->beforeEndX>0) {
		this->isReplacedForMessage = true;
	}

	if (this->isReplacedForMessage == true) {
		CString str;
		str.Format("\"%s\"��(��) ã�� �� �����ϴ�.", (LPCTSTR)this->notePadForm->findSameText->findStr);
		this->notePadForm->MessageBox(str, "�޸���", MB_OK);
	}
#endif



#if 0

	while (this->notePadForm->selectWords->firstX > 0 || this->notePadForm->selectWords->firstY > 0 ||
		this->notePadForm->selectWords->endX > 0 || this->notePadForm->selectWords->endY > 0 || isFirst == TRUE) {

		//FindSameTextŬ������ ReplaceAndSearchText�� ȣ���Ѵ�.
		this->notePadForm->findSameText->ReplaceAndSearchText();


		//(21.09.19 �߰�) ��� �����Ѵ�.
		this->firstX = this->notePadForm->selectWords->beforeFirstX;
		this->firstY = this->notePadForm->selectWords->beforeFirstY;
		this->beforeEndX = this->notePadForm->selectWords->beforeEndX;
		this->beforeEndY = this->notePadForm->selectWords->beforeEndY;
		this->afterEndX = this->notePadForm->selectWords->afterEndX;
		this->afterEndY = this->notePadForm->selectWords->afterEndY;
		this->replaceStr = this->notePadForm->findSameText->replaceStr;
		this->findStr = this->notePadForm->findSameText->findStr;
		this->isReplaced = this->notePadForm->selectWords->isReplaced;
		this->isLineChangeButtonClicked = this->notePadForm->selectWords->isLineChangeButtonClicked;
		this->windowWidth = this->notePadForm->selectWords->windowWidth;

		//�ϳ��� �ٲ�� ������ CommandStory�� �״´�.
		if (this->beforeEndX > 0) {
			this->notePadForm->commandStory->Pile(this); //dynamic_cast<Command*>(this)
		}

		//(21.10.29.�߰�) ���� Undolist�� ��ü�׸��� ���캻��.
		this->notePadForm->commandStory->undoList->Check();


		isFirst = false;
		this->isFirst = false;
		isReplaced++;
	}

	//�� �ܾ�� �ٲ����� Ȯ���Ѵ�.
	if (isReplaced > 1) {
		this->isReplaced = true;
	}
	else {
		CString str;
		str.Format("\"%s\"��(��) ã�� �� �����ϴ�.", (LPCTSTR)this->notePadForm->findSameText->findStr);
		this->notePadForm->MessageBox(str, "�޸���", MB_OK);
	}

#endif

	//1.7. ��ũ�� �̵����� Ȯ���Ѵ�. ******************************************************************
	this->notePadForm->isJumpOverForPrevious = FALSE;
	this->notePadForm->isJumpOverForNext = FALSE;
	this->notePadForm->isUp = FALSE;
	this->notePadForm->isDown = FALSE;
	this->notePadForm->isDoubleByte = FALSE;

	this->notePadForm->Notify(); // ���� ��ũ�� ���� �ۿ� �ܾ ���� ���
	this->notePadForm->Invalidate();
	//**************************************************************************************************

#if 0
	BOOL isFirst = true;
	int isReplaced = 0;

	//1. "��� �ٲٴ�" ��ư�� ������ ���,

	//1.0. �ٲ� ������ Ȯ���Ѵ�.
	this->notePadForm->replaceDialog->GetDlgItem(IDC_EDIT_REPLACETEXT)->GetWindowText(this->notePadForm->findSameText->replaceStr);
	this->replaceStr = (LPCTSTR)this->notePadForm->findSameText->replaceStr;

	//1.1. ã�� ������ Ȯ���Ѵ�.
	this->notePadForm->replaceDialog->GetDlgItem(IDC_EDIT_FINDTEXT)->GetWindowText(this->notePadForm->findSameText->findStr);
	this->findStr = (LPCTSTR)this->notePadForm->findSameText->findStr;

	//1.2. ����ڰ� �˻� ���ڿ��� ��/�ҹ��ڸ� ��Ȯ�� ��ġ��Ű���� Ȯ���Ѵ�. (����:TRUE, �ȱ���:FALSE)
	this->notePadForm->findSameText->matchCase = ((CButton*)this->notePadForm->replaceDialog->GetDlgItem(IDC_BUTTON_MATCHCASE))->GetCheck();

	//1.3. ����ڰ� �˻��� �Ʒ��� �������� �����ϴ��� Ȯ���Ѵ� (�Ʒ���:TRUE, ����:FALSE)
	this->notePadForm->findSameText->searchDown = TRUE;

	//1.4. ����ڰ� ��ü �ܾ ��ġ��ų�� ���θ� Ȯ���Ѵ�.
	this->notePadForm->findSameText->searchAround = TRUE;

	//1.5. FindSameTextŬ������ ReplaceAndSearchText�� ȣ���Ѵ�.
	//1.5.1. ���� Ŀ���� ��ġ�� (0,0)���� �������ش�.
	this->notePadForm->note->MoveCurrent(0);
	Glyph* row = this->notePadForm->note->GetChild(0);
	row->MoveCurrent(0);

	//1.5.2. ���� �ܾ �ִ� ���� �ݺ��Ѵ�.

	if (isFirst == true) {
		this->notePadForm->selectWords->firstX = 0;
		this->notePadForm->selectWords->firstY = 0;
		this->notePadForm->selectWords->endX = 0;
		this->notePadForm->selectWords->endY = 0;
	}

	while (this->notePadForm->selectWords->firstX > 0 || this->notePadForm->selectWords->firstY > 0 ||
		this->notePadForm->selectWords->endX > 0 || this->notePadForm->selectWords->endY > 0 || isFirst == TRUE) {

		this->notePadForm->findSameText->ReplaceAllText();
		isFirst = false;

		isReplaced++;
		//this->isReplaced = TRUE;
	}

	//�� �ܾ�� �ٲ����� Ȯ���Ѵ�.
	//this->isReplaced = this->notePadForm->selectWords->isReplaced;

	if (isReplaced > 1) {
		this->isReplaced = true;
	}


	//1.5.3. ���� ĳ���� ��ġ�� �ʱ�ȭ�Ѵ�.
	this->notePadForm->note->MoveCurrent(0);
	row = this->notePadForm->note->GetChild(0);
	row->MoveCurrent(0);

#if 0
	//1.6. ���� �ܾ ������ �޼����ڽ��� ����Ѵ�. (""��(��) ã�� �� �����ϴ�.)
	if ((this->notePadForm->selectWords->firstX <= 0 && this->notePadForm->selectWords->firstY <= 0 &&
		this->notePadForm->selectWords->endX <= 0 && this->notePadForm->selectWords->endY <= 0)
		|| this->notePadForm->findSameText->exception == TRUE) {

		CString str;
		str.Format("\"%s\"��(��) ã�� �� �����ϴ�.", (LPCTSTR)this->notePadForm->findSameText->findStr);
		MessageBox(str, "�޸���", MB_OK);
	}
#endif

	//1.7. ��ũ�� �̵����� Ȯ���Ѵ�. ******************************************************************
	this->notePadForm->isJumpOverForPrevious = FALSE;
	this->notePadForm->isJumpOverForNext = FALSE;
	this->notePadForm->isUp = FALSE;
	this->notePadForm->isDown = FALSE;
	this->notePadForm->isDoubleByte = FALSE;

	this->notePadForm->Notify(); // ���� ��ũ�� ���� �ۿ� �ܾ ���� ���
	this->notePadForm->Invalidate();
	//**************************************************************************************************

#endif
}

void OnReplaceAllCommand::Undo() {

	int difference;
	Glyph* row;
	int windowWidth = this->notePadForm->scroll->windowWidth; //���� ������ʺ�
	int changedX;
	int changedY;

	//0. firstX, firstY ���� �����.
	int firstX = this->firstX;
	int firstY = this->firstY;
	int endX = this->afterEndX;
	int endY = this->afterEndY;

	//1. (21.09.30 �߰�) ���� �ڵ������ư ��Ȳ�� �������� ��Ȳ�� �ٸ���,
	if (this->notePadForm->isLineChangeButtonClicked != this->isLineChangeButtonClicked) {

		//1.1. ���� �ڵ������ư�� ������ ������, �ڵ����� ���� ��ǥ�� ���Ѵ�. 
		if (this->notePadForm->isLineChangeButtonClicked == true) {
			this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
			this->notePadForm->lineChange->FindCaretAfterLineChange(endX, endY, &endX, &endY);
		}
		//1.2. ���� �ڵ������ư�� �ȴ����� ������, �ڵ����� ���� ��ǥ�� ���Ѵ�.
		else {
			//1.2.1. �ڵ����� �����ش�.
			this->notePadForm->lineChange->LineChangeButtonClicked(this->windowWidth);
			//1.2.2. �ڵ����� �� ��ǥ ���Ѵ�.
			this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
			this->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);

			//1.2.3. �ڵ����� ����Ѵ�.
			this->notePadForm->lineChange->LineChangeButtonNotClicked();
		}
	}
	//2. ���� �ڵ������ư ��Ȳ�� �������� ��Ȳ�� ������, �ڵ������� �� ������ �ʺ� �ٸ� ���,
	else if (this->isLineChangeButtonClicked == true && windowWidth != this->windowWidth) {
		//2.1. �ڵ������ ������ ��ģ��.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

		//2.2. �ڵ����� �����ش�.
		this->notePadForm->lineChange->LineChangeButtonClicked(this->windowWidth);

		//2.3. �ڵ����� �� ��ǥ�� ���Ѵ�.
		this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
		this->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);

		//2.4. �ڵ������ ������ ��ģ��.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

		//2.5. ��ģ ������ ���� windowWidth��ŭ �ڵ������Ѵ�.
		this->notePadForm->lineChange->LineChangeButtonClicked(windowWidth);

		//2.6. �ڵ����� �� ��ǥ�� ���Ѵ�.
		this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
		this->notePadForm->lineChange->FindCaretAfterLineChange(endX, endY, &endX, &endY);
    }

	//2. ���õ� �ܾ ���Ѵ�.
	this->notePadForm->selectWords->firstX = firstX;
	this->notePadForm->selectWords->firstY = firstY;
	this->notePadForm->selectWords->endX = endX;
	this->notePadForm->selectWords->endY = endY;

	//3. endX, endY�� ���� ��ġ�� �������ش�.
	this->notePadForm->note->MoveCurrent(endY);
	row = this->notePadForm->note->GetChild(endY);
	row->MoveCurrent(endX);

	KeyAction* keyAction = NULL;
	FactoryProduct factoryProduct(this->notePadForm);

	keyAction = factoryProduct.Make(VK_BACK, NULL, NULL);
	keyAction->KeyDown();

	//3. ���� ��ġ�� firstX,Y�� �������ش�.
	this->notePadForm->note->MoveCurrent(firstY);
	row = this->notePadForm->note->GetChild(firstY);
	row->MoveCurrent(firstX);

	//4. �ڵ������ư�� ������ ���,
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		//4.1. �ڵ����� �� firstXY�� ��ǥ�� ���Ѵ�.
		this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);

		//4.2. �ڵ������ ������ ��ģ��.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();
	}

	//5. ���� ��ġ�� findStr �����ش�.
	this->notePadForm->findSameText->findAndReplace->AddWordForUndo(firstX, firstY, this->findStr, &changedX, &changedY);

	//6. �ڵ������� ������ ���,
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		//6.1. ��ģ ������ �ٽ� �ڵ������Ѵ�. (ON)
		this->notePadForm->lineChange->LineChangeButtonClicked();

		//6.2. �ڵ����� �� �̵��� ĳ���� ��ġ�� ���Ѵ�.
		this->notePadForm->lineChange->FindCaretAfterLineChange(changedX, changedY, &changedX, &changedY);
	}

	//7. �ش� ��ġ�� ĳ���� �̵���Ų��.
	this->notePadForm->note->MoveCurrent(changedY);
	row = this->notePadForm->note->GetChild(changedY);
	row->MoveCurrent(changedX);


	//5. ���õ� ��ǥ�� �ʱ�ȭ�Ѵ�.
	this->notePadForm->selectWords->firstY = 0;
	this->notePadForm->selectWords->firstX = 0;
	this->notePadForm->selectWords->endY = 0;
	this->notePadForm->selectWords->endX = 0;

	//6. Scroll ������Ʈ �� ���� ������ �����Ѵ�.
	this->notePadForm->isJumpOverForPrevious = FALSE;
	this->notePadForm->isJumpOverForNext = FALSE;
	this->notePadForm->isDoubleByte = FALSE;
	this->notePadForm->isUp = TRUE;
	this->notePadForm->isDown = FALSE;

	this->notePadForm->Notify();

#if 0
	BOOL isFirst = TRUE;
	CString replaceStr = (CString)this->replaceStr.c_str();
	CString findStr = (CString)this->findStr.c_str();

	//1. "��� �ٲٴ�" ��ư�� ������ ���,

	//1.0. �ٲ� ������ Ȯ���Ѵ�.
	this->notePadForm->findSameText->replaceStr = findStr;

	//1.1. ã�� ������ Ȯ���Ѵ�.
	this->notePadForm->findSameText->findStr = replaceStr;

	//1.2. ����ڰ� �˻� ���ڿ��� ��/�ҹ��ڸ� ��Ȯ�� ��ġ��Ű���� Ȯ���Ѵ�. (����:TRUE, �ȱ���:FALSE)
	//this->notePadForm->findSameText->matchCase = ((CButton*)this->notePadForm->replaceDialog->GetDlgItem(IDC_BUTTON_MATCHCASE))->GetCheck();

	//1.3. ����ڰ� �˻��� �Ʒ��� �������� �����ϴ��� Ȯ���Ѵ� (�Ʒ���:TRUE, ����:FALSE)
	this->notePadForm->findSameText->searchDown = TRUE;

	//1.4. ����ڰ� ��ü �ܾ ��ġ��ų�� ���θ� Ȯ���Ѵ�.
	this->notePadForm->findSameText->searchAround = TRUE;

	//1.5. FindSameTextŬ������ ReplaceAndSearchText�� ȣ���Ѵ�.
	//1.5.1. ���� Ŀ���� ��ġ�� (0,0)���� �������ش�.
	this->notePadForm->note->MoveCurrent(0);
	Glyph* row = this->notePadForm->note->GetChild(0);
	row->MoveCurrent(0);

	//1.5.2. ���� �ܾ �ִ� ���� �ݺ��Ѵ�.

	if (isFirst == TRUE) {
		this->notePadForm->selectWords->firstX = 0;
		this->notePadForm->selectWords->firstY = 0;
		this->notePadForm->selectWords->endX = 0;
		this->notePadForm->selectWords->endY = 0;
	}

	while (this->notePadForm->selectWords->firstX > 0 || this->notePadForm->selectWords->firstY > 0 ||
		this->notePadForm->selectWords->endX > 0 || this->notePadForm->selectWords->endY > 0 || isFirst == TRUE) {

		this->notePadForm->findSameText->ReplaceAllText();
		isFirst = FALSE;
	}

	//1.5.3. ���� ĳ���� ��ġ�� �ʱ�ȭ�Ѵ�.
	this->notePadForm->note->MoveCurrent(0);
	row = this->notePadForm->note->GetChild(0);
	row->MoveCurrent(0);
#endif

}

void OnReplaceAllCommand::Redo() {

	int difference;
	Glyph* row;
	int windowWidth = this->notePadForm->scroll->windowWidth; //���� ������ʺ�
	int changedX;
	int changedY;

	//0. firstX, firstY ���� �����.
	int firstX = this->firstX;
	int firstY = this->firstY;
	int endX = this->beforeEndX;
	int endY = this->beforeEndY;

	//1. (21.09.30 �߰�) ���� �ڵ������ư ��Ȳ�� �������� ��Ȳ�� �ٸ���,
	if (this->notePadForm->isLineChangeButtonClicked != this->isLineChangeButtonClicked) {

		//1.1. ���� �ڵ������ư�� ������ ������, �ڵ����� ���� ��ǥ�� ���Ѵ�. 
		if (this->notePadForm->isLineChangeButtonClicked == true) {
			this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
			this->notePadForm->lineChange->FindCaretAfterLineChange(endX, endY, &endX, &endY);
		}
		//1.2. ���� �ڵ������ư�� �ȴ����� ������, �ڵ����� ���� ��ǥ�� ���Ѵ�.
		else {
			//1.2.1. �ڵ����� �����ش�.
			this->notePadForm->lineChange->LineChangeButtonClicked(this->windowWidth);
			//1.2.2. �ڵ����� �� ��ǥ ���Ѵ�.
			this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
			this->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);

			//1.2.3. �ڵ����� ����Ѵ�.
			this->notePadForm->lineChange->LineChangeButtonNotClicked();
		}
	}
	//2. ���� �ڵ������ư ��Ȳ�� �������� ��Ȳ�� ������, �ڵ������� �� ������ �ʺ� �ٸ� ���,
	else if (this->isLineChangeButtonClicked == true && windowWidth != this->windowWidth) {
		//2.1. �ڵ������ ������ ��ģ��.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

		//2.2. �ڵ����� �����ش�.
		this->notePadForm->lineChange->LineChangeButtonClicked(this->windowWidth);

		//2.3. �ڵ����� �� ��ǥ�� ���Ѵ�.
		this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);
		this->notePadForm->lineChange->FindCaretBeforeLineChange(endX, endY, &endX, &endY);

		//2.4. �ڵ������ ������ ��ģ��.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();

		//2.5. ��ģ ������ ���� windowWidth��ŭ �ڵ������Ѵ�.
		this->notePadForm->lineChange->LineChangeButtonClicked(windowWidth);

		//2.6. �ڵ����� �� ��ǥ�� ���Ѵ�.
		this->notePadForm->lineChange->FindCaretAfterLineChange(firstX, firstY, &firstX, &firstY);
		this->notePadForm->lineChange->FindCaretAfterLineChange(endX, endY, &endX, &endY);
}

	//2. ���õ� �ܾ ���Ѵ�.
	this->notePadForm->selectWords->firstX = firstX;
	this->notePadForm->selectWords->firstY = firstY;
	this->notePadForm->selectWords->endX = endX;
	this->notePadForm->selectWords->endY = endY;

	//3. endX, endY�� ���� ��ġ�� �������ش�.
	this->notePadForm->note->MoveCurrent(endY);
	row = this->notePadForm->note->GetChild(endY);
	row->MoveCurrent(endX);

	KeyAction* keyAction = NULL;
	FactoryProduct factoryProduct(this->notePadForm);

	keyAction = factoryProduct.Make(VK_BACK, NULL, NULL);
	keyAction->KeyDown();

	//3. ���� ��ġ�� firstX,Y�� �������ش�.
	this->notePadForm->note->MoveCurrent(firstY);
	row = this->notePadForm->note->GetChild(firstY);
	row->MoveCurrent(firstX);

	//4. �ڵ������ư�� ������ ���,
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		//4.1. �ڵ����� �� firstXY�� ��ǥ�� ���Ѵ�.
		this->notePadForm->lineChange->FindCaretBeforeLineChange(firstX, firstY, &firstX, &firstY);

		//4.2. �ڵ������ ������ ��ģ��.
		this->notePadForm->lineChange->LineChangeButtonNotClicked();
	}

	//5. ���� ��ġ�� findStr �����ش�.
	this->notePadForm->findSameText->findAndReplace->AddWordForUndo(firstX, firstY, this->replaceStr, &changedX, &changedY);

	//6. �ڵ������� ������ ���,
	if (this->notePadForm->isLineChangeButtonClicked == TRUE) {

		//6.1. ��ģ ������ �ٽ� �ڵ������Ѵ�. (ON)
		this->notePadForm->lineChange->LineChangeButtonClicked();

		//6.2. �ڵ����� �� �̵��� ĳ���� ��ġ�� ���Ѵ�.
		this->notePadForm->lineChange->FindCaretAfterLineChange(changedX, changedY, &changedX, &changedY);
	}

	//7. �ش� ��ġ�� ĳ���� �̵���Ų��.
	this->notePadForm->note->MoveCurrent(changedY);
	row = this->notePadForm->note->GetChild(changedY);
	row->MoveCurrent(changedX);


	//5. ���õ� ��ǥ�� �ʱ�ȭ�Ѵ�.
	this->notePadForm->selectWords->firstY = 0;
	this->notePadForm->selectWords->firstX = 0;
	this->notePadForm->selectWords->endY = 0;
	this->notePadForm->selectWords->endX = 0;

	//6. Scroll ������Ʈ �� ���� ������ �����Ѵ�.
	this->notePadForm->isJumpOverForPrevious = FALSE;
	this->notePadForm->isJumpOverForNext = FALSE;
	this->notePadForm->isDoubleByte = FALSE;
	this->notePadForm->isUp = TRUE;
	this->notePadForm->isDown = FALSE;

	this->notePadForm->Notify();


#if 0
	BOOL isFirst = TRUE;
	CString replaceStr = (CString)this->replaceStr.c_str();
	CString findStr = (CString)this->findStr.c_str();
#if 0
	CString replaceStr = this->notePadForm->findSameText->findStr;
	CString findStr = this->notePadForm->findSameText->replaceStr;
#endif

	//1. "��� �ٲٴ�" ��ư�� ������ ���,

	//1.0. �ٲ� ������ Ȯ���Ѵ�.
	this->notePadForm->findSameText->replaceStr = replaceStr;

	//1.1. ã�� ������ Ȯ���Ѵ�.
	this->notePadForm->findSameText->findStr = findStr;

	//1.2. ����ڰ� �˻� ���ڿ��� ��/�ҹ��ڸ� ��Ȯ�� ��ġ��Ű���� Ȯ���Ѵ�. (����:TRUE, �ȱ���:FALSE)
	//this->notePadForm->findSameText->matchCase = ((CButton*)this->notePadForm->replaceDialog->GetDlgItem(IDC_BUTTON_MATCHCASE))->GetCheck();

	//1.3. ����ڰ� �˻��� �Ʒ��� �������� �����ϴ��� Ȯ���Ѵ� (�Ʒ���:TRUE, ����:FALSE)
	this->notePadForm->findSameText->searchDown = TRUE;

	//1.4. ����ڰ� ��ü �ܾ ��ġ��ų�� ���θ� Ȯ���Ѵ�.
	this->notePadForm->findSameText->searchAround = TRUE;

	//1.5. FindSameTextŬ������ ReplaceAndSearchText�� ȣ���Ѵ�.
	//1.5.1. ���� Ŀ���� ��ġ�� (0,0)���� �������ش�.
	this->notePadForm->note->MoveCurrent(0);
	Glyph* row = this->notePadForm->note->GetChild(0);
	row->MoveCurrent(0);

	//1.5.2. ���� �ܾ �ִ� ���� �ݺ��Ѵ�.

	if (isFirst == TRUE) {
		this->notePadForm->selectWords->firstX = 0;
		this->notePadForm->selectWords->firstY = 0;
		this->notePadForm->selectWords->endX = 0;
		this->notePadForm->selectWords->endY = 0;
	}

	while (this->notePadForm->selectWords->firstX > 0 || this->notePadForm->selectWords->firstY > 0 ||
		this->notePadForm->selectWords->endX > 0 || this->notePadForm->selectWords->endY > 0 || isFirst == TRUE) {

		this->notePadForm->findSameText->ReplaceAllText();
		isFirst = FALSE;
	}

	//1.5.3. ���� ĳ���� ��ġ�� �ʱ�ȭ�Ѵ�.
	this->notePadForm->note->MoveCurrent(0);
	row = this->notePadForm->note->GetChild(0);
	row->MoveCurrent(0);
#endif
}


bool OnReplaceAllCommand::GetIsFirst() {
	return this->isFirst;
}

Long OnReplaceAllCommand::GetCurrentX() {
	return this->currentX;
}

Long OnReplaceAllCommand::GetCurrentY() {
	return this->currentY;
}

string OnReplaceAllCommand::GetCharacterForAll() {
	return this->findStr;
}

void OnReplaceAllCommand::SetCheck() {
	this->isChecked = true;
}

bool OnReplaceAllCommand::GetChecked() {
	return this->isChecked;
}

bool OnReplaceAllCommand::GetIsReplaced() {
	return this->isReplaced;
}

void OnReplaceAllCommand::SetIsFirst() {
	this->isFirst = true;
}





