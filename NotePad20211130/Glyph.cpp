//Glyph.cpp
#include"Glyph.h"
#include"Character.h"
#include"SingleByteCharacter.h"
#include"DoubleByteCharacter.h"
#include"Note.h"
#include"Row.h"



Glyph::Glyph() {
}

Glyph::~Glyph() {
}

Long Glyph::Add(Glyph* glyph) {
	return -1;
}

Long Glyph::Remove(Long index) {
	return -1;
}

Glyph* Glyph::GetChild(Long index) {
	return this;
}

Glyph& Glyph::operator=(const Glyph& source) {
	return *this;
}

Glyph::Glyph(const Glyph& source) {
}

Long Glyph::GetCapacity(){
	return -1;
}
Long Glyph::GetLength(){
	return -1;
}


Long Glyph::GetCurrent() {
	return -1;
}

void Glyph::MoveCurrent(Long location) {
}

Long Glyph::Save(string fileName) { //string fileName
	return -1;
}

Long Glyph::Load(string fileName) {
	return -1;
}

void Glyph::First() {
}
void Glyph::Last() {
}
void Glyph::Previous() {
}
void Glyph::Next() {
}
void Glyph::PreviousWord() {
}
void Glyph::NextWord() {
}

Long Glyph::Add(Long index, Glyph* glyph) {
	return -1;
}

Glyph* Glyph::Split(Glyph* dummyRow, int location) {
	return this;
}

void Glyph::Combine(Glyph* dummyRow) {
}

Glyph* Glyph::SplitForFront(Glyph* dummyRow, int location) {
	return this;
}


void Glyph::FindSameTextWithDown(string findStr, bool matchCase, int firstCurrentY, int beforeLineChangeCurrentX,
	int* firstY, int* firstX, int* endY, int* endX) {
}

void Glyph::FindSameTextWithUp(string findStr, bool matchCase, int firstCurrentY, int beforeLineChangeCurrentX,
	int* firstY, int* firstX, int* endY, int* endX) {
}

void Glyph::ReplaceSameText(string findStr, string replaceStr, string selectedStr, bool matchCase, bool searchAround,
	int firstCurrentY,
	int beforeLineChangeCurrentX, int* firstY, int* firstX, int* endY, int* endX, int* exception) {
}


#if 0
void Glyph::ReplaceAllSameText(string findStr, string replaceStr, bool matchCase, int firstCurrentY,
	int beforeLineChangeCurrentX) {
}
#endif




#if 0

#include<iostream>
using namespace std;
int main(int argc, char* argv[]) {
	Long index;
	Glyph* glyph;
	Glyph* character_;
	Glyph* row_;
	Long i = 0;

	//(21.08.25 찾기 객체 테스트용)
	int firstX;
	int firstY;
	int endX;
	int endY;


	Note note;
	Glyph* note1 = note.Clone();

	Row row;
	Glyph* row1 = row.Clone();

	//SingleByteCharacter singleByteCharacter1('B');
	//Glyph* character1 = singleByteCharacter1.Clone();
	Glyph* character1 = new DoubleByteCharacter("안");
	index = row1->Add(character1);
	row1->MoveCurrent(1);

	glyph = row1->GetChild(index);
	cout << glyph->GetContent() << endl;


	DoubleByteCharacter doubleByteCharacter("녕");
	Glyph* character2 = doubleByteCharacter.Clone();


	index = row1->Add(character2);
	row1->MoveCurrent(2);
	glyph = row1->GetChild(index);
	cout << glyph->GetContent() << endl;
	
//#if 0
	Glyph* character32 = new DoubleByteCharacter("하");
	index = row1->Add(character32);
	row1->MoveCurrent(3);
//#endif

	index = note1->Add(row1);
	glyph = note1->GetChild(index);
	
#if 0
	while (i < glyph->GetLength()) {
		character_ = glyph->GetChild(i);
		cout << character_->GetContent() << endl;
		i++;
	}
#endif

	//두번째 줄------------------------------------------------------------
	Glyph* row2 = row.Clone();
	
	DoubleByteCharacter doubleByteCharacter1("안");
	Glyph* character3 = doubleByteCharacter1.Clone();
	//Glyph* character3 = new SingleByteCharacter('a');
    row2->Add(character3);
	row2->MoveCurrent(1);

	DoubleByteCharacter doubleByteCharacter2("녕");
	Glyph* character4 = doubleByteCharacter2.Clone();
	//Glyph* character4 = new SingleByteCharacter('b');
	row2->Add(character4);
	row2->MoveCurrent(2);

	DoubleByteCharacter doubleByteCharacter3("하");
	Glyph* character5 = doubleByteCharacter3.Clone();
	//Glyph* character5 = new SingleByteCharacter('c');
	row2->Add(character5);
	row2->MoveCurrent(3);


	Glyph* character30 = new SingleByteCharacter('d');
	row2->Add(character30);
	row2->MoveCurrent(4);


	Glyph* character31 = new SingleByteCharacter('e');
	row2->Add(character31);
	row2->MoveCurrent(5);

	DoubleByteCharacter doubleByteCharacter4("세");
	Glyph* character6 = doubleByteCharacter4.Clone();
	row2->Add(character6);
	row2->MoveCurrent(4);

	DoubleByteCharacter doubleByteCharacter5("요");
	Glyph* character7 = doubleByteCharacter5.Clone();
	row2->Add(character7);
	row2->MoveCurrent(5);

	Glyph* character26 = new SingleByteCharacter(' ');
	row2->Add(character26);
	row2->MoveCurrent(6);

	Glyph* character27 = new DoubleByteCharacter("반");
	row2->Add(character27);
	row2->MoveCurrent(7);

	Glyph* character28 = new DoubleByteCharacter("갑");
	row2->Add(character28);
	row2->MoveCurrent(8);

	Glyph* character29 = new DoubleByteCharacter("습");
	row2->Add(character29);
	row2->MoveCurrent(9);

	Glyph* character50 = new DoubleByteCharacter("니");
	row2->Add(character50);
	row2->MoveCurrent(10);

	Glyph* character51 = new DoubleByteCharacter("다");
	row2->Add(character51);
	row2->MoveCurrent(11);


	index = note1->Add(row2);
	note1->MoveCurrent(12);
	row_ = note1->GetChild(index);
#if 0
	i = 0;
	while (i < row_->GetLength()) {
		character_ = row_->GetChild(i);
		cout << character_->GetContent() << endl;
		i++;
	}
#endif

	cout << "Row GetContent 테스트" << endl;
	cout << row_->GetContent() << endl;

	cout << "note GetContent 테스트" << endl;
	cout << note1->GetContent() << endl;

	//세번째 줄------------------------------------------------------------
	Glyph* row3 = new Row();

	Glyph* character8 = new DoubleByteCharacter("반");
	row3->Add(character8);
	row3->MoveCurrent(1);

	Glyph* character9 = new DoubleByteCharacter("갑");
	row3->Add(character9);
	row3->MoveCurrent(2);

	Glyph* character10 = new DoubleByteCharacter("안");
	row3->Add(character10);
	row3->MoveCurrent(3);

	Glyph* character11 = new DoubleByteCharacter("녕");
	row3->Add(character11);
	row3->MoveCurrent(4);

	Glyph* character12 = new DoubleByteCharacter("다");
	row3->Add(character12);
	row3->MoveCurrent(5);

	Glyph* character13 = new SingleByteCharacter(' ');
	row3->Add(character13);
	row3->MoveCurrent(6);

	Glyph* character14 = new DoubleByteCharacter("어");
	row3->Add(character14);
	row3->MoveCurrent(7);

	Glyph* character15 = new DoubleByteCharacter("디");
	row3->Add(character15);
	row3->MoveCurrent(8);

	Glyph* character16 = new DoubleByteCharacter("안");
	row3->Add(character16);
	row3->MoveCurrent(9);

	Glyph* character17 = new DoubleByteCharacter("녕");
	row3->Add(character17);
	row3->MoveCurrent(10);

	Glyph* character18 = new DoubleByteCharacter("요");
	row3->Add(character18);
	row3->MoveCurrent(11);

	Glyph* character19 = new SingleByteCharacter(' ');
	row3->Add(character19);
	row3->MoveCurrent(12);

	Glyph* character20 = new DoubleByteCharacter("궁");
	row3->Add(character20);
	row3->MoveCurrent(13);

	Glyph* character21 = new DoubleByteCharacter("금");
	row3->Add(character21);
	row3->MoveCurrent(14);

	Glyph* character22 = new DoubleByteCharacter("해");
	row3->Add(character22);
	row3->MoveCurrent(15);

	Glyph* character23 = new DoubleByteCharacter("서");
	row3->Add(character23);
	row3->MoveCurrent(16);

	Glyph* character24 = new DoubleByteCharacter("요");
	row3->Add(character24);
	row3->MoveCurrent(17);

	Glyph* character25 = new SingleByteCharacter(' ');
	row3->Add(character25);
	row3->MoveCurrent(18);


	note1->Add(row3);
	note1->MoveCurrent(2);



	cout << "note1 GetContent 테스트" << endl;
	cout << note1->GetContent() << endl;

#if 0
	//21.08.25. 찾기 추가
	cout << "찾기 테스트" << endl;
	note1->FindSameTextWithDown("안녕",true, 1, 11, &firstY, &firstX, &endY, &endX);
	
	cout << "firstY: " << firstY << endl;
	cout << "firstX: " << firstX << endl;
	cout << "endY: " << endY << endl;
	cout << "endX: " << endX << endl;


	//21.08.25. 찾기 추가
	cout << "찾기 테스트" << endl;
	note1->FindSameTextWithUp("안녕하de세요 반갑", true, 2, 7, &firstY, &firstX, &endY, &endX);

	cout << "firstY: " << firstY << endl;
	cout << "firstX: " << firstX << endl;
	cout << "endY: " << endY << endl;
	cout << "endX: " << endX << endl;
#endif



#if 0
	Glyph* row2 = new Row();
	note1->Add(1,row2);
	Long rowLength = row1->GetLength();
	Long currentX = 1;
	while (currentX < rowLength) {
		character_ = row1->GetChild(currentX);
		row2->Add(character_);
		row1->Remove(currentX);
		rowLength--;
	}

	cout << "note1 GetContent 테스트 - 이후" << endl;
	cout << note1->GetContent() << endl;
#endif


#if 0
	cout << "row1 테스트" << endl;
	cout << row1->GetContent() << endl;

	Glyph* character50 = new DoubleByteCharacter("히");
	index = row1->AddInsert(1, character50);

	cout << "row1 테스트" << endl;
	cout << row1->GetContent() << endl;
#endif


#if 0
	cout << "Down 테스트 - 이전" << endl;
	note1->MoveCurrent(1);
	row2->MoveCurrent(3);

	Long currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	Glyph* currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;



	cout << "Down 테스트 - 이후" << endl;
	note1->Down();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;
#endif


#if 0
	cout << "Up 테스트 - 이전" << endl;
	note1->MoveCurrent(1);
	row1->MoveCurrent(5);

	Long currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	Glyph* currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;



	cout << "Up 테스트 - 이후" << endl;
	note1->Up();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;
#endif

#if 0
	cout << "NextWord 테스트 - 이전" << endl;
	note1->MoveCurrent(0);
	row1->MoveCurrent(0);

	Long currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	Glyph* currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "NextWord 테스트 - 이후" << endl;
	note1->NextWord();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "NextWord 테스트 - 이후" << endl;
	note1->NextWord();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "NextWord 테스트 - 이후" << endl;
	note1->NextWord();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "NextWord 테스트 - 이후" << endl;
	note1->NextWord();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "NextWord 테스트 - 이후" << endl;
	note1->NextWord();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "NextWord 테스트 - 이후" << endl;
	note1->NextWord();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "NextWord 테스트 - 이후" << endl;
	note1->NextWord();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "NextWord 테스트 - 이후" << endl;
	note1->NextWord();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "NextWord 테스트 - 이후" << endl;
	note1->NextWord();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "NextWord 테스트 - 이후" << endl;
	note1->NextWord();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "NextWord 테스트 - 이후" << endl;
	note1->NextWord();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "NextWord 테스트 - 이후" << endl;
	note1->NextWord();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "NextWord 테스트 - 이후" << endl;
	note1->NextWord();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "NextWord 테스트 - 이후" << endl;
	note1->NextWord();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;
#endif


#if 0
	cout << "PreviousWord 테스트 - 이전" << endl;
	Long currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	Glyph* currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "PreviousWord 테스트 - 이후" << endl;
	note1->PreviousWord();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "PreviousWord 테스트 - 이후" << endl;
	note1->PreviousWord();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "PreviousWord 테스트 - 이후" << endl;
	note1->PreviousWord();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "PreviousWord 테스트 - 이후" << endl;
	note1->PreviousWord();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "PreviousWord 테스트 - 이후" << endl;
	note1->PreviousWord();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "PreviousWord 테스트 - 이후" << endl;
	note1->PreviousWord();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "PreviousWord 테스트 - 이후" << endl;
	note1->PreviousWord();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "PreviousWord 테스트 - 이후" << endl;
	note1->PreviousWord();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;
#endif


#if 0
	cout << "Next 테스트 - 이전" << endl;
	note1->MoveCurrent(0);
	row1->MoveCurrent(0);

	Long currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	Glyph* currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;


	cout << "Next 테스트 - 이후" << endl;
	note1->Next();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "Next 테스트 - 이후" << endl;
	note1->Next();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "Next 테스트 - 이후" << endl;
	note1->Next();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "Next 테스트 - 이후" << endl;
	note1->Next();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "Next 테스트 - 이후" << endl;
	note1->Next();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "Next 테스트 - 이후" << endl;
	note1->Next();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "Next 테스트 - 이후" << endl;
	note1->Next();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "Next 테스트 - 이후" << endl;
	note1->Next();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;


	cout << "Next 테스트 - 이후" << endl;
	note1->Next();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "Next 테스트 - 이후" << endl;
	note1->Next();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "Next 테스트 - 이후" << endl;
	note1->Next();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "Next 테스트 - 이후" << endl;
	note1->Next();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "Next 테스트 - 이후" << endl;
	note1->Next();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;
#endif


#if 0
	cout << "Previous 테스트 - 이전" << endl;
	Long currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	Glyph* currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "Previous 테스트 - 이후" << endl;
	note1->Previous();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;


	cout << "Previous 테스트 - 이후" << endl;
	note1->Previous();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "Previous 테스트 - 이후" << endl;
	note1->Previous();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;


	cout << "Previous 테스트 - 이후" << endl;
	note1->Previous();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "Previous 테스트 - 이후" << endl;
	note1->Previous();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "Previous 테스트 - 이후" << endl;
	note1->Previous();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "Previous 테스트 - 이후" << endl;
	note1->Previous();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "Previous 테스트 - 이후" << endl;
	note1->Previous();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "Previous 테스트 - 이후" << endl;
	note1->Previous();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "Previous 테스트 - 이후" << endl;
	note1->Previous();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "Previous 테스트 - 이후" << endl;
	note1->Previous();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "Previous 테스트 - 이후" << endl;
	note1->Previous();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

	cout << "Previous 테스트 - 이후" << endl;
	note1->Previous();
	currentY = note1->GetCurrent();
	cout << "현재 currentY:" << currentY << endl;
	currentRow = note1->GetChild(currentY);
	cout << "현재 currentX:" << currentRow->GetCurrent() << endl;

#endif

#if 0
	//Save 테스트 

	Long saveLength = note1->Save();
	cout << saveLength << endl;

	//Load 테스트


	Glyph* note2 = new Note();
	Glyph* row4 = new Row();
	note2->Add(row4);

	Long loadLength = note2->Load("NotePad.txt");
	cout << loadLength << endl;

	cout << "찐찐 Load 테스트 " << endl;
	cout << note2->GetContent() << endl;
#endif

#if 0
	row_->Remove(2);
	i = 0;
	while (i < row_->GetLength()) {
		character_ = row_->GetChild(i);
		cout << character_->GetContent() << endl;
		i++;
	}
#endif

#if 0
	cout << "REMOVE" << endl;

	glyph = note1->GetChild(0);
	glyph->Remove(1);
	character_ = glyph->GetChild(0);
	cout << character_->GetContent() << endl;
#endif





	return 0;
}

#endif




