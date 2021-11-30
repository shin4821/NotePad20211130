//GetHelp.cpp
#include"GetInformation.h"
#include"NotePadForm.h"

GetInformation::GetInformation(NotePadForm* notePadForm, CWnd* parent)
	:CDialog(GetInformation::IDD) {
	this->notePadForm = notePadForm;
}

GetInformation::~GetInformation() {

}



