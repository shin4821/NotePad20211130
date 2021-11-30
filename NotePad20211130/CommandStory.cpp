//CommandStory.cpp
#include"CommandStory.h"
#include"NotePadForm.h"
#include"Command.h"
#include"UndoList.h"
#include"RedoList.h"
#include"OnCharCommand.h"
#include"OnImeCompositionCommand.h"
#include"OnImeCharCommand.h"
#include"OnBackSpaceCommand.h"
#include"OnPasteCommand.h"
#include"OnCutOffCommand.h"
#include"OnDeleteCommand.h"
#include"OnReplaceCommand.h"
#include"OnReplaceAllCommand.h"
#include"OnTimeAndDateCommand.h"
#include"LineChange.h"
#include"Note.h"
#include<afxcmn.h>


CommandStory::CommandStory(NotePadForm* notePadForm) {
	this->notePadForm = notePadForm;
	this->undoList = new UndoList();
	this->redoList = new RedoList();
}

CommandStory::~CommandStory() {

	if (this->undoList != 0) {
		delete this->undoList;
		this->undoList = 0;
	}
	if (this->redoList != 0) {
		delete this->redoList;
		this->redoList = 0;
	}
}

Command* CommandStory::Undo() {
	Command* command = 0;

	//1. undoList의 length가 0보다 큰 경우,
	if (this->undoList->GetLength() > 0) {
		//undo를 실행할때 undoList에 저장됐던 command를 꺼내어 redoList에 push 해준다.
#if 0
		command = this->undoList->Top();
		
		this->redoList->Push(command);

		this->undoList->Pop();

		//그런 후 꺼내온 command를 실행한다.
		command = this->redoList->Top();
		command->Execute();
#endif
		//redoList에서 command를 꺼내온다.
		command = this->undoList->Pop();

		//그 command를 undoList에 넣는다.
		this->redoList->Push(command);

		//그런 후 꺼내온 command를 실행한다.
		command->Execute();
		this->notePadForm->Invalidate();
	}

	return command;
}

Command* CommandStory::Redo() {
	Command* command = 0;

	//1. redoList의 length가 0보다 큰 경우,
	if (this->redoList->GetLength() > 0) {
		//redoList에서 command를 꺼내온다.
		command = this->redoList->Pop();

		//그 command를 undoList에 넣는다.
		this->undoList->Push(command);

		//그런 후 꺼내온 command를 실행한다.
		command->Execute();
	}

	return command;
}


void CommandStory::Pile(Command* command) {

	Command* previousCommand;
	int previousX;
	int previousY;
	int currentX;
	int currentY;
	char previousCharacter;

	//1. 1Byte일 경우
	if (dynamic_cast<OnCharCommand*>(command)) {

		currentX = dynamic_cast<OnCharCommand*>(command)->GetCurrentX();
		currentY = dynamic_cast<OnCharCommand*>(command)->GetCurrentY();


		// 자동개행인 경우, 자동개행 전 좌표를 찾는다.
		if (this->notePadForm->isLineChangeButtonClicked == true) {
			this->notePadForm->lineChange->FindCaretBeforeLineChange(currentX, currentY, &currentX, &currentY);
		}


		//1. 커맨드를 PUSH하기 전, 체크가 필요하면 체크해준다. 

		//1.1. undoList가 비어있지 않으면,
		if (this->undoList->IsEmpty() == false) {
			previousCommand = this->undoList->Top();
			previousX = previousCommand->GetCurrentX();
			previousY = previousCommand->GetCurrentY();
			previousCharacter = previousCommand->GetCharacter();
		
			

			// 자동개행인 경우, 자동개행 전 좌표를 찾는다.
			if (this->notePadForm->isLineChangeButtonClicked == true) {
				this->notePadForm->lineChange->FindCaretBeforeLineChange(previousX, previousY, &previousX, &previousY);
			}


			bool isChecked = previousCommand->GetChecked();

			//1.1.1. 전 컬럼과 현 컬럼의 차이가 1이 아닌 경우,
			if (dynamic_cast<OnCharCommand*>(previousCommand) &&
				(currentX - previousX != 1 || currentY != previousY || previousCharacter == '\r' || previousCharacter == '\n')) {
				dynamic_cast<OnCharCommand*>(command)->SetCheck();
			}
			else if (!dynamic_cast<OnCharCommand*>(previousCommand)) {
				dynamic_cast<OnCharCommand*>(command)->SetCheck();
			}
			

#if 0
			//1.1.1. 자동개행이 아닌 경우, 전 컬럼과 현 컬럼의 차이가 1이 아닌 경우 체크한다.
			if (this->notePadForm->isLineChangeButtonClicked == false
				&& (currentX - previousX != 1 || currentY != previousY)) {
				dynamic_cast<OnCharCommand*>(command)->SetCheck();
			}
			//1.1.2. 자동개행인 경우, "같은 줄인데" 전 컬럼과 현 컬럼의 차이가 1이 아닌경우 체크한다.
			else if (this->notePadForm->isLineChangeButtonClicked == true
				&& currentX - previousX != 1 && currentY == previousY) {
				dynamic_cast<OnCharCommand*>(command)->SetCheck();
			}
#endif
		}
		//1.2. undoList가 비어있으면,
		else {
			//1.2.1. command의 isChecked를 true로 바꿔준다.
			dynamic_cast<OnCharCommand*>(command)->SetCheck();
		}

		//2. UndoList에 추가한다.
		this->undoList->Push(command);
	}

	//2. 2Byte일 경우(조합중일때),
	else if (dynamic_cast<OnImeCompositionCommand*>(command)) {

		//2.0. undoList에서 새로 시작한 글자 외에 기존 커맨드를 지운다.
		if (this->notePadForm->isFirstComposingForCommand == FALSE) {
			this->undoList->Pop();
		}

		// 커맨드를 PUSH하기 전, 체크가 필요하면 체크해준다. 
#if 0
		//2.1. undoList가 비어있지 않으면,
		if (this->undoList->IsEmpty() == false) {
			previousCommand = this->undoList->Top();
			previousX = previousCommand->GetCurrentX();
			previousY = previousCommand->GetCurrentY();

			//1.1.1. 전 컬럼과 현 컬럼의 차이가 1이 아닌 경우,
			if (currentX - previousX != 1 || currentY != previousY) {
				dynamic_cast<OnCharCommand*>(command)->SetCheck();
			}
		}
#endif
		//2.2. undoList가 비어있으면,
		if (this->undoList->IsEmpty() == true) {
			//1.2.1. command의 isChecked를 true로 바꿔준다.
			dynamic_cast<OnImeCompositionCommand*>(command)->SetCheck();
		}

		//2. UndoList에 추가한다.
		this->undoList->Push(command);
	}

	//3. 2Byte일 경우(완성됬을때)
	else if (dynamic_cast<OnImeCharCommand*>(command)) {
		currentX = dynamic_cast<OnImeCharCommand*>(command)->GetCurrentX();
		currentY = dynamic_cast<OnImeCharCommand*>(command)->GetCurrentY();

		// 자동개행인 경우, 자동개행 전 좌표를 찾는다.
		if (this->notePadForm->isLineChangeButtonClicked == true) {
			this->notePadForm->lineChange->FindCaretBeforeLineChange(currentX, currentY, &currentX, &currentY);
		}

		//2.2. 기존에 쓰여진 커맨드를 undoList에서 지운다.(조합중 지운다.)
		this->undoList->Pop();

		// 커맨드를 PUSH하기 전, 체크가 필요하면 체크해준다. 
		//2.1. undoList가 비어있지 않으면,
		if (this->undoList->IsEmpty() == false) {
			previousCommand = this->undoList->Top();
			previousX = previousCommand->GetCurrentX();
			previousY = previousCommand->GetCurrentY();
			previousCharacter = previousCommand->GetCharacter();

			// 자동개행인 경우, 자동개행 전 좌표를 찾는다.
			if (this->notePadForm->isLineChangeButtonClicked == true) {
				this->notePadForm->lineChange->FindCaretBeforeLineChange(previousX, previousY, &previousX, &previousY);
			}

			//1.1.1. 전 컬럼과 현 컬럼의 차이가 1이 아닌 경우,
			if (dynamic_cast<OnImeCharCommand*>(previousCommand) &&
				(currentX - previousX != 1 || currentY != previousY || previousCharacter == '\r' || previousCharacter == '\n')) {
				dynamic_cast<OnImeCharCommand*>(command)->SetCheck();
			}
			else if (!dynamic_cast<OnImeCharCommand*>(previousCommand)) {
				dynamic_cast<OnImeCharCommand*>(command)->SetCheck();
			}

		}
		//2.2. undoList가 비어있으면,
		else {
			//1.2.1. command의 isChecked를 true로 바꿔준다.
			dynamic_cast<OnImeCharCommand*>(command)->SetCheck();
		}

		//2. UndoList에 추가한다.
		this->undoList->Push(command);
	}

	//4.BackSpace일 때
	else if (dynamic_cast<OnBackSpaceCommand*>(command)) {
		dynamic_cast<OnBackSpaceCommand*>(command)->SetCheck();

		//2. UndoList에 추가한다.
		this->undoList->Push(command);
	}
	//5. Paste일 때
	else if (dynamic_cast<OnPasteCommand*>(command)) {

	    currentX = dynamic_cast<OnPasteCommand*>(command)->GetCurrentX();
	    currentY = dynamic_cast<OnPasteCommand*>(command)->GetCurrentY();

	    //1. 커맨드를 PUSH하기 전, 체크가 필요하면 체크해준다. 

	    //1.1. undoList가 비어있지 않으면,
	    if (this->undoList->IsEmpty() == false) {
		    previousCommand = this->undoList->Top();
		    previousX = previousCommand->GetEndX();

			dynamic_cast<OnPasteCommand*>(command)->SetCheck();

			if (dynamic_cast<OnBackSpaceCommand*>(previousCommand) && previousX > 0) {
				dynamic_cast<OnPasteCommand*>(command)->UnCheck();
			}

	    }
	    //1.2. undoList가 비어있으면,
	    else {
		    //1.2.1. command의 isChecked를 true로 바꿔준다.
		    dynamic_cast<OnPasteCommand*>(command)->SetCheck();
	    }

	    //2. UndoList에 추가한다.
	    this->undoList->Push(command);


#if 0
	    dynamic_cast<OnPasteCommand*>(command)->SetCheck();

	    //2. UndoList에 추가한다.
	    this->undoList->Push(command);
#endif
    }
	//6. CutOff일 때
	else if (dynamic_cast<OnCutOffCommand*>(command)) {
	    dynamic_cast<OnCutOffCommand*>(command)->SetCheck();

	    //2. UndoList에 추가한다.
	    this->undoList->Push(command);
	}
	//7. Delete일 때
	else if (dynamic_cast<OnDeleteCommand*>(command)) {
	    dynamic_cast<OnDeleteCommand*>(command)->SetCheck();

	    //2. UndoList에 추가한다.
	    this->undoList->Push(command);
    }
	//8. Replace일 때
	else if (dynamic_cast<OnReplaceCommand*>(command)) {
	    dynamic_cast<OnReplaceCommand*>(command)->SetCheck();

	    //2. UndoList에 추가한다.
	    this->undoList->Push(command);
	}

	//9. ReplaceAll일 때
	else if (dynamic_cast<OnReplaceAllCommand*>(command)) {
	     
	    //9.1. (21.11.15. 추가) 처음에 들어오는 OnReplaceAllCommand의 command만 체크해준다.
		if (dynamic_cast<OnReplaceAllCommand*>(command)->GetIsFirst() == true) {
			dynamic_cast<OnReplaceAllCommand*>(command)->SetCheck();
		}

	    //2. UndoList에 추가한다.
	    this->undoList->Push(command);
    }
	//10. TimeAndDate일 때
	else if (dynamic_cast<OnTimeAndDateCommand*>(command)) {
	    dynamic_cast<OnTimeAndDateCommand*>(command)->SetCheck();

	    //2. UndoList에 추가한다.
	    this->undoList->Push(command);
    }

	//Redo List를 초기화한다.
	this->redoList->DeleteAll();
}
















