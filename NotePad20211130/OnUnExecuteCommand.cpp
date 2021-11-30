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

	//(21.10.29.추가) 현재 Undolist의 전체항목을 살펴본다.
	this->notePadForm->commandStory->undoList->Check();

	if (this->notePadForm->commandStory->undoList->IsEmpty() == false) {

		//1. undoList가 비어있지 않고, command의 체크가 false인 동안 반복한다.
		command = this->notePadForm->commandStory->undoList->Pop();

		//(21.11.10. 추가) OnImeCompositionCommand의 command인데, character가 ""이면 아예 뽑아주고, 그 다음 것을 뽑는다.
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

				//1.1. 해당 Command의 Undo를 실행한다.
				command->Undo();

				//1.2. 해당 Command를 RedoList에 끼운다.
				this->notePadForm->commandStory->redoList->Push(command);

				command = this->notePadForm->commandStory->undoList->Pop();

				isFirstChecked = false;

			}

			//2. 반복처리 후 마지막 남은 command가 체크 되어있는 경우,
			if (isFirstChecked == false && command->GetChecked() == true) {

				//2.1. 마지막 남은 하나이거나, 개행문자가 아닌경우, undoList에서 지운다. 
				if (this->notePadForm->commandStory->undoList->GetLength() <= 0 || command->GetLineChanged() != true) {

					//2.1.1. 해당 Command의 Undo를 실행한다.
					command->Undo();

					//2.1.2. 해당 Command를 RedoList에 끼운다.
					this->notePadForm->commandStory->redoList->Push(command);
				}
				//2.2. 0개 초과인데 개행문자인 경우, 다시 넣어준다.
				else if (this->notePadForm->commandStory->undoList->GetLength() > 0 && command->GetLineChanged() == true) {

					this->notePadForm->commandStory->undoList->Push(command);
				}
			}

			//3. 처음부터 command의 체크가 true였으면, 해당 command만 실행해준다.
			if (isFirstChecked == true && command->GetChecked() == true) {
				//3.1. 해당 Command의 Undo를 실행한다.
				command->Undo();

				//3.2. 해당 Command를 RedoList에 끼운다.
				this->notePadForm->commandStory->redoList->Push(command);
			}

		}

	}


}

















