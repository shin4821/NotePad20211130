//PageInfo.cpp
#include"PageInfo.h"
#include"NotePadForm.h"

PageInfo::PageInfo(NotePadForm* notePadForm) {
	this->notePadForm = notePadForm;
	this->top = 0; //20
	this->bottom = 0; //20
	this->left = 0; //25
	this->right = 0; //25
	this->paperWidth = 210; //A4 기본 너비
	this->paperHeight = 297; //A4 기본 높이
	this->actualWidth = this->paperWidth - (this->left + this->right); //A4 기본 너비 - 4000
	this->actualHeight = this->paperHeight - (this->top + this->bottom); //A4 기본 높이 - 5000
	this->formName = "A4";
	this->orientation = 1; //기본 방향 세로
	this->header = "";
	this->footer = "";
	this->paperSize = 9;
}

PageInfo::~PageInfo() {

}

CString PageInfo::GetHeader() {
	return this->header;
}

CString PageInfo::GetFooter() {
	return this->footer;
}

CString PageInfo::GetFormName() {
	return this->formName;
}

int PageInfo::GetPaperWidth() {
	return this->paperWidth;
}

int PageInfo::GetPaperHeight() {
	return this->paperHeight;
}

int PageInfo::GetOrientation() {
	return this->orientation;
}

int PageInfo::GetTop() {
	return this->top;
}
int PageInfo::GetLeft() {
	return this->left;
}
int PageInfo::GetRight() {
	return this->right;
}
int PageInfo::GetBottom() {
	return this->bottom;
}
int PageInfo::GetActualWidth() {
	return this->actualWidth;
}
int PageInfo::GetActualHeight() {
	return this->actualHeight;
}
int PageInfo::GetPaperSize() {
	return this->paperSize;
}



void PageInfo::FixHeader(CString header) {
	this->header = header;
}
void PageInfo::FixFooter(CString footer) {
	this->footer = footer;
}
void PageInfo::FixFormName(CString formName) {
	this->formName = formName;
}
void PageInfo::FixPaperWidth(int paperWidth) {
	this->paperWidth = paperWidth;
}
void PageInfo::FixPaperHeight(int paperHeight) {
	this->paperHeight = paperHeight;
}
void PageInfo::FixOrientation(int orientation) {
	this->orientation = orientation;
}
void PageInfo::FixTop(int top) {
	this->top = top;
}
void PageInfo::FixLeft(int left) {
	this->left = left;
}
void PageInfo::FixRight(int right) {
	this->right = right;
}
void PageInfo::FixBottom(int bottom) {
	this->bottom = bottom;
}
void PageInfo::FixActualWidth(int actualWidth) {
	this->actualWidth = actualWidth;
}
void PageInfo::FixActualHeight(int actualHeight) {
	this->actualHeight = actualHeight;
}
void PageInfo::FixPaperSize(int paperSize) {
	this->paperSize = paperSize;
}






