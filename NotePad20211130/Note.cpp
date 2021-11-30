//Note.cpp
#include"Note.h"
#include"Visitor.h"

Note::Note(Long capacity)
	:Composite(capacity) {
}

Note::~Note() {
}

Note& Note::operator=(const Note& source) {//치환연산자
	this->glyphs = source.glyphs;
	this->capacity = source.capacity;
	this->length = source.length;

	return *this;
}

Note::Note(const Note& source) {// 복사생성자
	this->glyphs = source.glyphs;
	this->capacity = source.capacity;
	this->length = source.length;
}

Glyph* Note::Clone() {

	return new Note(*this);
}


string Note::GetContent() {
	Long i = 0;
	Glyph* row;
	string contents;

	while (i < this->GetLength()) {
		row = this->glyphs.GetAt(i);

		contents.append(row->GetContent());

		i++;

		if (i < this->GetLength()) {
			contents.append("\r\n");
		}
	}
	return contents;
}

void Note::Accept(Visitor* visitor) {

	visitor->VisitNote(this);
}

void Note::AcceptForSelect(Visitor* visitor) {
}