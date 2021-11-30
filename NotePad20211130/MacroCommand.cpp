//MacroCommand.cpp
#include"MacroCommand.h"
#include"NotePadForm.h"


MacroCommand::MacroCommand(NotePadForm* notePadForm, Long capacity)
	:Command(notePadForm), commands(capacity) {
	this->capacity = capacity;
	this->length = 0;
}

MacroCommand::~MacroCommand() {
}

void MacroCommand::Execute() {
	Command* command;
	Long i = 0;

	while (i < this->commands.GetLength()) {
		command = this->commands.GetAt(i);
		command->Execute();
		i++;
	}
}

Long MacroCommand::Add(Command* command) {

	Long index;

	if (this->length < this->capacity) {
		index = this->commands.Store(this->length, command);
	}
	else {
		index = this->commands.AppendForRear(command);
		this->capacity++;
	}
	this->length++;
	return index;

}

void MacroCommand::Remove(Command* command) {
	Long index;

	index = this->commands.LinearSearchUnique(command, CompareCommands);

	this->commands.Delete(index);
	this->capacity--;
	this->length--;
}


int CompareCommands(void* one, void* other) {
	int ret;

	Command** one_ = (Command**)one;

	if (*one_ == other) {
		ret = 0;
	}
	else {
		ret = -1;
	}

	return ret;
}















