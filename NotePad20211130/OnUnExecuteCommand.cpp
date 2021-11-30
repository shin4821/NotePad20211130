//OnUnExecuteCommand.cpp
#include"OnUnExecuteCommand.h"
#include"NotePadForm.h"
#include"CommandStory.h"
#include"UndoList.h"
#include"RedoList.h"
#include"OnImeCompositionCommand.h"


OnUnExecuteCommand::OnUnExecuteCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
}

OnUnExecuteCommand::~OnUnExecuteCommand() {
}



void OnUnExecuteCommand::Execute() {
	Command* command;
	Command* previous;
	bool isFirstChecked = true;

	//(21.10.29.�߰�) ���� Undolist�� ��ü�׸��� ���캻��.
	this->notePadForm->commandStory->undoList->Check();

	if (this->notePadForm->commandStory->undoList->IsEmpty() == false) {

		//1. undoList�� ������� �ʰ�, command�� üũ�� false�� ���� �ݺ��Ѵ�.
		command = this->notePadForm->commandStory->undoList->Pop();

		//(21.11.10. �߰�) OnImeCompositionCommand�� command�ε�, character�� ""�̸� �ƿ� �̾��ְ�, �� ���� ���� �̴´�.
		if (dynamic_cast<OnImeCompositionCommand*>(command)) {

			string testCharacter = command->GetCharacterForDoubleByte();

			if (testCharacter == "" && this->notePadForm->commandStory->undoList->IsEmpty() == false) {
				command = this->notePadForm->commandStory->undoList->Pop();
			}
			else {
				command = NULL;
			}
		}

		if (command != NULL) {

			while (this->notePadForm->commandStory->undoList->IsEmpty() == false && command->GetChecked() == false) {

				//1.1. �ش� Command�� Undo�� �����Ѵ�.
				command->Undo();

				//1.2. �ش� Command�� RedoList�� �����.
				this->notePadForm->commandStory->redoList->Push(command);

				command = this->notePadForm->commandStory->undoList->Pop();

				isFirstChecked = false;

			}

			//2. �ݺ�ó�� �� ������ ���� command�� üũ �Ǿ��ִ� ���,
			if (isFirstChecked == false && command->GetChecked() == true) {

				//2.1. ������ ���� �ϳ��̰ų�, ���๮�ڰ� �ƴѰ��, undoList���� �����. 
				if (this->notePadForm->commandStory->undoList->GetLength() <= 0 || command->GetLineChanged() != true) {

					//2.1.1. �ش� Command�� Undo�� �����Ѵ�.
					command->Undo();

					//2.1.2. �ش� Command�� RedoList�� �����.
					this->notePadForm->commandStory->redoList->Push(command);
				}
				//2.2. 0�� �ʰ��ε� ���๮���� ���, �ٽ� �־��ش�.
				else if (this->notePadForm->commandStory->undoList->GetLength() > 0 && command->GetLineChanged() == true) {

					this->notePadForm->commandStory->undoList->Push(command);
				}
			}

			//3. ó������ command�� üũ�� true������, �ش� command�� �������ش�.
			if (isFirstChecked == true && command->GetChecked() == true) {
				//3.1. �ش� Command�� Undo�� �����Ѵ�.
				command->Undo();

				//3.2. �ش� Command�� RedoList�� �����.
				this->notePadForm->commandStory->redoList->Push(command);
			}

		}

	}


}

















