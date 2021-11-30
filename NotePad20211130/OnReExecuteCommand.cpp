//OnReExecuteCommand.cpp
#include"OnReExecuteCommand.h"
#include"NotePadForm.h"
#include"CommandStory.h"
#include"UndoList.h"
#include"RedoList.h"
#include"OnImeCharCommand.h"
#include"OnImeCompositionCommand.h"
#include"OnCharCommand.h"

OnReExecuteCommand::OnReExecuteCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
}

OnReExecuteCommand::~OnReExecuteCommand() {
}


void OnReExecuteCommand::Execute() {

	Command* command = 0;
	bool isFirstChecked = true;
	bool isEnd = false;
	bool isEmpty = false;
	
	//(21.11.01. 추가) 현재 Undolist의 전체항목을 살펴본다.
    this->notePadForm->commandStory->redoList->Check();


	//1. 처음부터 command의 체크가 true였으면, 해당 command만 실행해준다.

	if (this->notePadForm->commandStory->redoList->IsEmpty() == false) {

		command = this->notePadForm->commandStory->redoList->Pop();
		bool isLineChanged = command->GetLineChanged();

		//0. 뽑자마자 redoList가 비어버리는 경우,
		if (this->notePadForm->commandStory->redoList->IsEmpty() == true) {
			//0.1. Command가 OnChar인 경우,
			if (dynamic_cast<OnCharCommand*>(command)) {
				this->notePadForm->nChar = command->GetCharacter();
			}
			//0.2. Command가 OnImeComposition 또는 OnImeChar인 경우,
			else if (dynamic_cast<OnImeCompositionCommand*>(command) || dynamic_cast<OnImeCharCommand*>(command)) {
				this->notePadForm->letter = command->GetCharacterForDoubleByte();
			}

			command->Redo();

			//0.3. 해당 Command를 UndoList에 끼운다.
			this->notePadForm->commandStory->undoList->Push(command);

			isEnd = true;
		}
		

		//1. 첫번째 커맨드가 체크이고 redoList가 비어있지 않으면,
		if (this->notePadForm->commandStory->redoList->IsEmpty() == false && command->GetChecked() == true) {
						
			//1.1. 해당 Command의 Undo를 실행한다.
 
			//1.1.1. Command가 OnChar인 경우,
			if (dynamic_cast<OnCharCommand*>(command)) {
				this->notePadForm->nChar = command->GetCharacter();
			}
			//1.1.2. Command가 OnImeComposition 또는 OnImeChar인 경우,
			else if (dynamic_cast<OnImeCompositionCommand*>(command) || dynamic_cast<OnImeCharCommand*>(command)){
				this->notePadForm->letter = command->GetCharacterForDoubleByte();
			}

			command->Redo();

			//1.2. 해당 Command를 UndoList에 끼운다.
			this->notePadForm->commandStory->undoList->Push(command);

			command = this->notePadForm->commandStory->redoList->Pop(); //현재것이 개행이면 추후 다시 redo에 넣어줘야함.
			//다시 뽑은 커맨드가 체크이면 다시 꽂아줘야함.
		}

		//2. 개행문자가 아니였으면 반복한다.
		if (isLineChanged == false) {

			//2.1.redoList가 비어있지 않고, command의 체크가 false인 동안 반복한다.
			//(21.09.14) 수정
			//while (this->notePadForm->commandStory->redoList->IsEmpty() == false && command->GetChecked() == false)
			while (isEmpty == false && command->GetChecked() == false) {

				//2.1.1. 해당 Command의 Redo를 실행한다.
				//Command가 OnChar인 경우,
				if (dynamic_cast<OnCharCommand*>(command)) {
					this->notePadForm->nChar = command->GetCharacter();
				}
				//Command가 OnImeComposition 또는 OnImeChar인 경우,
				else if (dynamic_cast<OnImeCompositionCommand*>(command) || dynamic_cast<OnImeCharCommand*>(command)) {
					this->notePadForm->letter = command->GetCharacterForDoubleByte();
				}
				command->Redo();

				//2.1.2. 해당 Command를 UndoList에 끼운다.
				this->notePadForm->commandStory->undoList->Push(command);


				command = this->notePadForm->commandStory->redoList->Pop();

				//(21.09.14) 수정
				if (this->notePadForm->commandStory->redoList->IsEmpty() == true) {
					 isEmpty = true;	
				}

				isFirstChecked = false;

			}

			//2.2. 반복처리 후 마지막 남은 command가 체크 되어있는 경우, 
			//this->notePadForm->commandStory->redoList->IsEmpty() == false/true
			if (command!=NULL &&isFirstChecked == false && command->GetChecked() == true) {
				this->notePadForm->commandStory->redoList->Push(command);

			}

			//2.3. 아무것도 없으면 아예 지운다.
			//isFirstChecked == true && 
			else if (command!=NULL && 
				isFirstChecked == false && this->notePadForm->commandStory->redoList->IsEmpty() == true) {
				
				//2.3.1. 해당 Command의 Undo를 실행한다.
				//Command가 OnChar인 경우,
				if (dynamic_cast<OnCharCommand*>(command)) {
					this->notePadForm->nChar = command->GetCharacter();
				}
				//Command가 OnImeComposition 또는 OnImeChar인 경우,
				else if (dynamic_cast<OnImeCompositionCommand*>(command) || dynamic_cast<OnImeCharCommand*>(command)) {
					this->notePadForm->letter = command->GetCharacterForDoubleByte();
				}
				command->Redo();

				//2.3.2. 해당 Command를 UndoList에 끼운다.
				this->notePadForm->commandStory->undoList->Push(command);
			}
		}
		//3. 첫번째 커맨드가 개행이였으면,
		if (isFirstChecked == true && (isLineChanged == true || command->GetChecked() == true) && isEnd == false) {
			//3.1. 뽑은 커맨드를 다시 꽂아둔다.
			this->notePadForm->commandStory->redoList->Push(command);
		}

		


	}
#if 0
	while (this->notePadForm->commandStory->redoList->IsEmpty() == false && command->GetChecked() == false
		&& command->GetLineChanged() == false) {

		//1.1. 해당 Command의 Redo를 실행한다.
		this->notePadForm->nChar = command->GetCharacter(); //***************************
		command->Redo();

		//1.2. 해당 Command를 UndoList에 끼운다.
		this->notePadForm->commandStory->undoList->Push(command);

		command = this->notePadForm->commandStory->redoList->Pop();

		isFirstChecked = false;

	}

	//3. 반복처리 후 마지막 남은 command가 체크 되어있는 경우,
	if (isFirstChecked == false && command->GetChecked() == true) {
		this->notePadForm->commandStory->redoList->Push(command);

	}
	//4. 아무것도 없으면 아예 지운다.
	else if (isFirstChecked == true && command->GetLineChanged() == false) {
		//4.1. 해당 Command의 Undo를 실행한다.
		this->notePadForm->nChar = command->GetCharacter();//*********************
		command->Redo();

		//4.2. 해당 Command를 UndoList에 끼운다.
		this->notePadForm->commandStory->undoList->Push(command);
	}

	//개행이면 마지막에 뽑은 command는 다시 넣어준다.
	if (command->GetLineChanged() == true) {
		this->notePadForm->commandStory->redoList->Push(command);
	}
#endif
}



