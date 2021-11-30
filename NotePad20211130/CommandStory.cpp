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

	//1. undoList�� length�� 0���� ū ���,
	if (this->undoList->GetLength() > 0) {
		//undo�� �����Ҷ� undoList�� ����ƴ� command�� ������ redoList�� push ���ش�.
#if 0
		command = this->undoList->Top();
		
		this->redoList->Push(command);

		this->undoList->Pop();

		//�׷� �� ������ command�� �����Ѵ�.
		command = this->redoList->Top();
		command->Execute();
#endif
		//redoList���� command�� �����´�.
		command = this->undoList->Pop();

		//�� command�� undoList�� �ִ´�.
		this->redoList->Push(command);

		//�׷� �� ������ command�� �����Ѵ�.
		command->Execute();
		this->notePadForm->Invalidate();
	}

	return command;
}

Command* CommandStory::Redo() {
	Command* command = 0;

	//1. redoList�� length�� 0���� ū ���,
	if (this->redoList->GetLength() > 0) {
		//redoList���� command�� �����´�.
		command = this->redoList->Pop();

		//�� command�� undoList�� �ִ´�.
		this->undoList->Push(command);

		//�׷� �� ������ command�� �����Ѵ�.
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

	//1. 1Byte�� ���
	if (dynamic_cast<OnCharCommand*>(command)) {

		currentX = dynamic_cast<OnCharCommand*>(command)->GetCurrentX();
		currentY = dynamic_cast<OnCharCommand*>(command)->GetCurrentY();


		// �ڵ������� ���, �ڵ����� �� ��ǥ�� ã�´�.
		if (this->notePadForm->isLineChangeButtonClicked == true) {
			this->notePadForm->lineChange->FindCaretBeforeLineChange(currentX, currentY, &currentX, &currentY);
		}


		//1. Ŀ�ǵ带 PUSH�ϱ� ��, üũ�� �ʿ��ϸ� üũ���ش�. 

		//1.1. undoList�� ������� ������,
		if (this->undoList->IsEmpty() == false) {
			previousCommand = this->undoList->Top();
			previousX = previousCommand->GetCurrentX();
			previousY = previousCommand->GetCurrentY();
			previousCharacter = previousCommand->GetCharacter();
		
			

			// �ڵ������� ���, �ڵ����� �� ��ǥ�� ã�´�.
			if (this->notePadForm->isLineChangeButtonClicked == true) {
				this->notePadForm->lineChange->FindCaretBeforeLineChange(previousX, previousY, &previousX, &previousY);
			}


			bool isChecked = previousCommand->GetChecked();

			//1.1.1. �� �÷��� �� �÷��� ���̰� 1�� �ƴ� ���,
			if (dynamic_cast<OnCharCommand*>(previousCommand) &&
				(currentX - previousX != 1 || currentY != previousY || previousCharacter == '\r' || previousCharacter == '\n')) {
				dynamic_cast<OnCharCommand*>(command)->SetCheck();
			}
			else if (!dynamic_cast<OnCharCommand*>(previousCommand)) {
				dynamic_cast<OnCharCommand*>(command)->SetCheck();
			}
			

#if 0
			//1.1.1. �ڵ������� �ƴ� ���, �� �÷��� �� �÷��� ���̰� 1�� �ƴ� ��� üũ�Ѵ�.
			if (this->notePadForm->isLineChangeButtonClicked == false
				&& (currentX - previousX != 1 || currentY != previousY)) {
				dynamic_cast<OnCharCommand*>(command)->SetCheck();
			}
			//1.1.2. �ڵ������� ���, "���� ���ε�" �� �÷��� �� �÷��� ���̰� 1�� �ƴѰ�� üũ�Ѵ�.
			else if (this->notePadForm->isLineChangeButtonClicked == true
				&& currentX - previousX != 1 && currentY == previousY) {
				dynamic_cast<OnCharCommand*>(command)->SetCheck();
			}
#endif
		}
		//1.2. undoList�� ���������,
		else {
			//1.2.1. command�� isChecked�� true�� �ٲ��ش�.
			dynamic_cast<OnCharCommand*>(command)->SetCheck();
		}

		//2. UndoList�� �߰��Ѵ�.
		this->undoList->Push(command);
	}

	//2. 2Byte�� ���(�������϶�),
	else if (dynamic_cast<OnImeCompositionCommand*>(command)) {

		//2.0. undoList���� ���� ������ ���� �ܿ� ���� Ŀ�ǵ带 �����.
		if (this->notePadForm->isFirstComposingForCommand == FALSE) {
			this->undoList->Pop();
		}

		// Ŀ�ǵ带 PUSH�ϱ� ��, üũ�� �ʿ��ϸ� üũ���ش�. 
#if 0
		//2.1. undoList�� ������� ������,
		if (this->undoList->IsEmpty() == false) {
			previousCommand = this->undoList->Top();
			previousX = previousCommand->GetCurrentX();
			previousY = previousCommand->GetCurrentY();

			//1.1.1. �� �÷��� �� �÷��� ���̰� 1�� �ƴ� ���,
			if (currentX - previousX != 1 || currentY != previousY) {
				dynamic_cast<OnCharCommand*>(command)->SetCheck();
			}
		}
#endif
		//2.2. undoList�� ���������,
		if (this->undoList->IsEmpty() == true) {
			//1.2.1. command�� isChecked�� true�� �ٲ��ش�.
			dynamic_cast<OnImeCompositionCommand*>(command)->SetCheck();
		}

		//2. UndoList�� �߰��Ѵ�.
		this->undoList->Push(command);
	}

	//3. 2Byte�� ���(�ϼ�������)
	else if (dynamic_cast<OnImeCharCommand*>(command)) {
		currentX = dynamic_cast<OnImeCharCommand*>(command)->GetCurrentX();
		currentY = dynamic_cast<OnImeCharCommand*>(command)->GetCurrentY();

		// �ڵ������� ���, �ڵ����� �� ��ǥ�� ã�´�.
		if (this->notePadForm->isLineChangeButtonClicked == true) {
			this->notePadForm->lineChange->FindCaretBeforeLineChange(currentX, currentY, &currentX, &currentY);
		}

		//2.2. ������ ������ Ŀ�ǵ带 undoList���� �����.(������ �����.)
		this->undoList->Pop();

		// Ŀ�ǵ带 PUSH�ϱ� ��, üũ�� �ʿ��ϸ� üũ���ش�. 
		//2.1. undoList�� ������� ������,
		if (this->undoList->IsEmpty() == false) {
			previousCommand = this->undoList->Top();
			previousX = previousCommand->GetCurrentX();
			previousY = previousCommand->GetCurrentY();
			previousCharacter = previousCommand->GetCharacter();

			// �ڵ������� ���, �ڵ����� �� ��ǥ�� ã�´�.
			if (this->notePadForm->isLineChangeButtonClicked == true) {
				this->notePadForm->lineChange->FindCaretBeforeLineChange(previousX, previousY, &previousX, &previousY);
			}

			//1.1.1. �� �÷��� �� �÷��� ���̰� 1�� �ƴ� ���,
			if (dynamic_cast<OnImeCharCommand*>(previousCommand) &&
				(currentX - previousX != 1 || currentY != previousY || previousCharacter == '\r' || previousCharacter == '\n')) {
				dynamic_cast<OnImeCharCommand*>(command)->SetCheck();
			}
			else if (!dynamic_cast<OnImeCharCommand*>(previousCommand)) {
				dynamic_cast<OnImeCharCommand*>(command)->SetCheck();
			}

		}
		//2.2. undoList�� ���������,
		else {
			//1.2.1. command�� isChecked�� true�� �ٲ��ش�.
			dynamic_cast<OnImeCharCommand*>(command)->SetCheck();
		}

		//2. UndoList�� �߰��Ѵ�.
		this->undoList->Push(command);
	}

	//4.BackSpace�� ��
	else if (dynamic_cast<OnBackSpaceCommand*>(command)) {
		dynamic_cast<OnBackSpaceCommand*>(command)->SetCheck();

		//2. UndoList�� �߰��Ѵ�.
		this->undoList->Push(command);
	}
	//5. Paste�� ��
	else if (dynamic_cast<OnPasteCommand*>(command)) {

	    currentX = dynamic_cast<OnPasteCommand*>(command)->GetCurrentX();
	    currentY = dynamic_cast<OnPasteCommand*>(command)->GetCurrentY();

	    //1. Ŀ�ǵ带 PUSH�ϱ� ��, üũ�� �ʿ��ϸ� üũ���ش�. 

	    //1.1. undoList�� ������� ������,
	    if (this->undoList->IsEmpty() == false) {
		    previousCommand = this->undoList->Top();
		    previousX = previousCommand->GetEndX();

			dynamic_cast<OnPasteCommand*>(command)->SetCheck();

			if (dynamic_cast<OnBackSpaceCommand*>(previousCommand) && previousX > 0) {
				dynamic_cast<OnPasteCommand*>(command)->UnCheck();
			}

	    }
	    //1.2. undoList�� ���������,
	    else {
		    //1.2.1. command�� isChecked�� true�� �ٲ��ش�.
		    dynamic_cast<OnPasteCommand*>(command)->SetCheck();
	    }

	    //2. UndoList�� �߰��Ѵ�.
	    this->undoList->Push(command);


#if 0
	    dynamic_cast<OnPasteCommand*>(command)->SetCheck();

	    //2. UndoList�� �߰��Ѵ�.
	    this->undoList->Push(command);
#endif
    }
	//6. CutOff�� ��
	else if (dynamic_cast<OnCutOffCommand*>(command)) {
	    dynamic_cast<OnCutOffCommand*>(command)->SetCheck();

	    //2. UndoList�� �߰��Ѵ�.
	    this->undoList->Push(command);
	}
	//7. Delete�� ��
	else if (dynamic_cast<OnDeleteCommand*>(command)) {
	    dynamic_cast<OnDeleteCommand*>(command)->SetCheck();

	    //2. UndoList�� �߰��Ѵ�.
	    this->undoList->Push(command);
    }
	//8. Replace�� ��
	else if (dynamic_cast<OnReplaceCommand*>(command)) {
	    dynamic_cast<OnReplaceCommand*>(command)->SetCheck();

	    //2. UndoList�� �߰��Ѵ�.
	    this->undoList->Push(command);
	}

	//9. ReplaceAll�� ��
	else if (dynamic_cast<OnReplaceAllCommand*>(command)) {
	     
	    //9.1. (21.11.15. �߰�) ó���� ������ OnReplaceAllCommand�� command�� üũ���ش�.
		if (dynamic_cast<OnReplaceAllCommand*>(command)->GetIsFirst() == true) {
			dynamic_cast<OnReplaceAllCommand*>(command)->SetCheck();
		}

	    //2. UndoList�� �߰��Ѵ�.
	    this->undoList->Push(command);
    }
	//10. TimeAndDate�� ��
	else if (dynamic_cast<OnTimeAndDateCommand*>(command)) {
	    dynamic_cast<OnTimeAndDateCommand*>(command)->SetCheck();

	    //2. UndoList�� �߰��Ѵ�.
	    this->undoList->Push(command);
    }

	//Redo List�� �ʱ�ȭ�Ѵ�.
	this->redoList->DeleteAll();
}
















