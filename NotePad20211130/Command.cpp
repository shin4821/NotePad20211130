//Command.cpp
#include"Command.h"
#include"NotePadForm.h"
using namespace std;

Command::Command(NotePadForm* notePadForm) {
	this->notePadForm = notePadForm;
	//this->isChecked = false;
	//this->column = 0;
}

Command::~Command() {
}

void Command::Execute() {
}

void Command::Undo() {
}

void Command::Redo() {
}


Long Command::Add(Command* command) {
	return -1;
}

void Command::Remove(Command* command) {
}

bool Command::GetChecked() {
	return false;
}

Long Command::GetCurrentX() {
	return -1;
}

Long Command::GetCurrentY() {
	return -1;
}

bool Command::GetLineChanged() {
	return false;
}

void Command::SetCheck() {

}

void Command::UnCheck() {

}

char Command::GetCharacter() {
	return 0;
}

#if 0
WPARAM Command::GetCharacterFor2Byte() {
	return 0;
}
#endif

string Command::GetCharacterForDoubleByte() {
	return 0;
}

string Command::GetCharacterForAll() {
	return 0;
}

bool Command::GetIsReplaced() {
	return false;
}

Long Command::GetEndX() {
	return 0;
}

bool Command::GetIsFirst() {
	return false;
}

void Command::SetIsFirst() {
}