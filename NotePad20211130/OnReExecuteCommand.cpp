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
	
	//(21.11.01. �߰�) ���� Undolist�� ��ü�׸��� ���캻��.
    this->notePadForm->commandStory->redoList->Check();


	//1. ó������ command�� üũ�� true������, �ش� command�� �������ش�.

	if (this->notePadForm->commandStory->redoList->IsEmpty() == false) {

		command = this->notePadForm->commandStory->redoList->Pop();
		bool isLineChanged = command->GetLineChanged();

		//0. ���ڸ��� redoList�� �������� ���,
		if (this->notePadForm->commandStory->redoList->IsEmpty() == true) {
			//0.1. Command�� OnChar�� ���,
			if (dynamic_cast<OnCharCommand*>(command)) {
				this->notePadForm->nChar = command->GetCharacter();
			}
			//0.2. Command�� OnImeComposition �Ǵ� OnImeChar�� ���,
			else if (dynamic_cast<OnImeCompositionCommand*>(command) || dynamic_cast<OnImeCharCommand*>(command)) {
				this->notePadForm->letter = command->GetCharacterForDoubleByte();
			}

			command->Redo();

			//0.3. �ش� Command�� UndoList�� �����.
			this->notePadForm->commandStory->undoList->Push(command);

			isEnd = true;
		}
		

		//1. ù��° Ŀ�ǵ尡 üũ�̰� redoList�� ������� ������,
		if (this->notePadForm->commandStory->redoList->IsEmpty() == false && command->GetChecked() == true) {
						
			//1.1. �ش� Command�� Undo�� �����Ѵ�.
 
			//1.1.1. Command�� OnChar�� ���,
			if (dynamic_cast<OnCharCommand*>(command)) {
				this->notePadForm->nChar = command->GetCharacter();
			}
			//1.1.2. Command�� OnImeComposition �Ǵ� OnImeChar�� ���,
			else if (dynamic_cast<OnImeCompositionCommand*>(command) || dynamic_cast<OnImeCharCommand*>(command)){
				this->notePadForm->letter = command->GetCharacterForDoubleByte();
			}

			command->Redo();

			//1.2. �ش� Command�� UndoList�� �����.
			this->notePadForm->commandStory->undoList->Push(command);

			command = this->notePadForm->commandStory->redoList->Pop(); //������� �����̸� ���� �ٽ� redo�� �־������.
			//�ٽ� ���� Ŀ�ǵ尡 üũ�̸� �ٽ� �Ⱦ������.
		}

		//2. ���๮�ڰ� �ƴϿ����� �ݺ��Ѵ�.
		if (isLineChanged == false) {

			//2.1.redoList�� ������� �ʰ�, command�� üũ�� false�� ���� �ݺ��Ѵ�.
			//(21.09.14) ����
			//while (this->notePadForm->commandStory->redoList->IsEmpty() == false && command->GetChecked() == false)
			while (isEmpty == false && command->GetChecked() == false) {

				//2.1.1. �ش� Command�� Redo�� �����Ѵ�.
				//Command�� OnChar�� ���,
				if (dynamic_cast<OnCharCommand*>(command)) {
					this->notePadForm->nChar = command->GetCharacter();
				}
				//Command�� OnImeComposition �Ǵ� OnImeChar�� ���,
				else if (dynamic_cast<OnImeCompositionCommand*>(command) || dynamic_cast<OnImeCharCommand*>(command)) {
					this->notePadForm->letter = command->GetCharacterForDoubleByte();
				}
				command->Redo();

				//2.1.2. �ش� Command�� UndoList�� �����.
				this->notePadForm->commandStory->undoList->Push(command);


				command = this->notePadForm->commandStory->redoList->Pop();

				//(21.09.14) ����
				if (this->notePadForm->commandStory->redoList->IsEmpty() == true) {
					 isEmpty = true;	
				}

				isFirstChecked = false;

			}

			//2.2. �ݺ�ó�� �� ������ ���� command�� üũ �Ǿ��ִ� ���, 
			//this->notePadForm->commandStory->redoList->IsEmpty() == false/true
			if (command!=NULL &&isFirstChecked == false && command->GetChecked() == true) {
				this->notePadForm->commandStory->redoList->Push(command);

			}

			//2.3. �ƹ��͵� ������ �ƿ� �����.
			//isFirstChecked == true && 
			else if (command!=NULL && 
				isFirstChecked == false && this->notePadForm->commandStory->redoList->IsEmpty() == true) {
				
				//2.3.1. �ش� Command�� Undo�� �����Ѵ�.
				//Command�� OnChar�� ���,
				if (dynamic_cast<OnCharCommand*>(command)) {
					this->notePadForm->nChar = command->GetCharacter();
				}
				//Command�� OnImeComposition �Ǵ� OnImeChar�� ���,
				else if (dynamic_cast<OnImeCompositionCommand*>(command) || dynamic_cast<OnImeCharCommand*>(command)) {
					this->notePadForm->letter = command->GetCharacterForDoubleByte();
				}
				command->Redo();

				//2.3.2. �ش� Command�� UndoList�� �����.
				this->notePadForm->commandStory->undoList->Push(command);
			}
		}
		//3. ù��° Ŀ�ǵ尡 �����̿�����,
		if (isFirstChecked == true && (isLineChanged == true || command->GetChecked() == true) && isEnd == false) {
			//3.1. ���� Ŀ�ǵ带 �ٽ� �ȾƵд�.
			this->notePadForm->commandStory->redoList->Push(command);
		}

		


	}
#if 0
	while (this->notePadForm->commandStory->redoList->IsEmpty() == false && command->GetChecked() == false
		&& command->GetLineChanged() == false) {

		//1.1. �ش� Command�� Redo�� �����Ѵ�.
		this->notePadForm->nChar = command->GetCharacter(); //***************************
		command->Redo();

		//1.2. �ش� Command�� UndoList�� �����.
		this->notePadForm->commandStory->undoList->Push(command);

		command = this->notePadForm->commandStory->redoList->Pop();

		isFirstChecked = false;

	}

	//3. �ݺ�ó�� �� ������ ���� command�� üũ �Ǿ��ִ� ���,
	if (isFirstChecked == false && command->GetChecked() == true) {
		this->notePadForm->commandStory->redoList->Push(command);

	}
	//4. �ƹ��͵� ������ �ƿ� �����.
	else if (isFirstChecked == true && command->GetLineChanged() == false) {
		//4.1. �ش� Command�� Undo�� �����Ѵ�.
		this->notePadForm->nChar = command->GetCharacter();//*********************
		command->Redo();

		//4.2. �ش� Command�� UndoList�� �����.
		this->notePadForm->commandStory->undoList->Push(command);
	}

	//�����̸� �������� ���� command�� �ٽ� �־��ش�.
	if (command->GetLineChanged() == true) {
		this->notePadForm->commandStory->redoList->Push(command);
	}
#endif
}



