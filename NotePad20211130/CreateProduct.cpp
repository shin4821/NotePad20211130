//CreateProduct.cpp
#include"CreateProduct.h"
#include"Note.h"
#include"Row.h"
#include"SingleByteCharacter.h"
#include"DoubleByteCharacter.h"

CreateProduct::CreateProduct() {
}

CreateProduct::~CreateProduct() {
}

CreateProduct::CreateProduct(const CreateProduct& source) {//복사생성자

}

CreateProduct& CreateProduct::operator=(const CreateProduct& source) {//치환연산자
	return *this;
}

Glyph* CreateProduct::Make(char(*character)) { //virtual Glyph* Write(char(*character));
	Glyph* glyph = 0;


	if (character == 0) { //널이 들어오면 노트를 만든다.
		glyph =  new Note();
	}
	else if (character[0] == '\n' || character[0] == '\r') { //개행문자 입력되면 줄을 만든다.
		glyph = new Row();
	}
	else if (character[1] == '\0') { //두번째 칸이 널문자이면 1byte 만든다.
		glyph = new SingleByteCharacter(character[0]);
	}
	else if (character[1] != '\0') { //두번째 칸이 널문자가 아니면 2byte 만든다.
		glyph = new DoubleByteCharacter(character);
	}
#if 0
	else if (character[1] == '\t') {// 탭키이면 8개 공백 띄운다.
		return new SingleByteCharacter((' ')*8);
	}
#endif
	else {
		glyph = 0;
	}
	return glyph;
}
















