//DummyRow.cpp
#include"DummyRow.h"
#include<iostream>
using namespace std;

DummyRow::DummyRow(Long capacity)
	:Row(capacity) {
}

DummyRow::~DummyRow() {
}

DummyRow& DummyRow::operator=(const DummyRow& source) { //치환연산자
	this->glyphs = source.glyphs;
	this->capacity = source.capacity;
	this->length = source.length;

	return *this;
}

DummyRow::DummyRow(const DummyRow& source) { //복사생성자
	this->glyphs = source.glyphs;
	this->capacity = source.capacity;
	this->length = source.length;
}


Glyph* DummyRow::Clone() {

	return new DummyRow(*this);
}


string DummyRow::GetContent() {
	Long i = 0;
	Glyph* content;
	string contents;
	string content_;

	while (i < this->GetLength()) {
		content = this->glyphs.GetAt(i);

//#if 0
		//(21.11.02. 추가)
		if (content == NULL) {
			i++;
			if (i < this->GetLength()) {
				content = this->glyphs.GetAt(i);
			}
			else {
				break;
			}
		}
//#endif

		content_ = content->GetContent();

		// 탭키이면 공백 8개로 치환한다.
		if (content_ == "\t") {
			content_ = "        ";
		}


		contents.append(content_);
		//cout << content->GetContent() << endl;
		i++;
	}
	//마지막에 널문자 추가한다.
	contents.append("\0");

	return contents;
}
