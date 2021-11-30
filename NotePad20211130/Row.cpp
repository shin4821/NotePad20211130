//Row.cpp
#include"Row.h"
#include"Visitor.h"
#include<iostream>
using namespace std;

Row::Row(Long capacity)
	:Composite(capacity) {
}

Row::~Row() {
}

Row& Row::operator=(const Row& source) { //ġȯ������
	this->glyphs = source.glyphs;
	this->capacity = source.capacity;
	this->length = source.length;

	return *this;
}

Row::Row(const Row& source) { //���������
	this->glyphs = source.glyphs;
	this->capacity = source.capacity;
	this->length = source.length;
}


Glyph* Row::Clone() {

	return new Row(*this);
}


string Row::GetContent() {
	Long i = 0;
	Glyph* content;
	string contents;
	string content_;

	while (i < this->GetLength()) {
		content = this->glyphs.GetAt(i);
		
//#if 0
		//(21.11.02. �߰�)
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

#if 0
		
		if (content_=="\t") {
			content_ = ' ' * 8;
		}
#endif
		// ��Ű�̸� ���� 8���� ġȯ�Ѵ�.
		if (content_ == "\t") {
			content_ = "        ";
		}

		contents.append(content_);
		//cout << content->GetContent() << endl;
		i++;
	}
	//�������� �ι��� �߰��Ѵ�.
	contents.append("\0");

	return contents;
}

void Row::Accept(Visitor* visitor) {

	visitor->VisitRow(this);
}

void Row::AcceptForSelect(Visitor* visitor) {
	visitor->VisitRowForSelect(this);
}
















