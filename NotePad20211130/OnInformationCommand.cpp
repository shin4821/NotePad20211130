//OnInformationCommand.cpp
#include"OnInformationCommand.h"
#include"NotePadForm.h"
#include"GetInformation.h"

OnInformationCommand::OnInformationCommand(NotePadForm* notePadForm)
	:Command(notePadForm) {
}

OnInformationCommand::~OnInformationCommand() {
}

void OnInformationCommand::Execute() {

	GetInformation getInformation(NULL, this->notePadForm);
	getInformation.DoModal();
}







