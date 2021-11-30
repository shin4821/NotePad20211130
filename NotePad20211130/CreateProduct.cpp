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

CreateProduct::CreateProduct(const CreateProduct& source) {//���������

}

CreateProduct& CreateProduct::operator=(const CreateProduct& source) {//ġȯ������
	return *this;
}

Glyph* CreateProduct::Make(char(*character)) { //virtual Glyph* Write(char(*character));
	Glyph* glyph = 0;


	if (character == 0) { //���� ������ ��Ʈ�� �����.
		glyph =  new Note();
	}
	else if (character[0] == '\n' || character[0] == '\r') { //���๮�� �ԷµǸ� ���� �����.
		glyph = new Row();
	}
	else if (character[1] == '\0') { //�ι�° ĭ�� �ι����̸� 1byte �����.
		glyph = new SingleByteCharacter(character[0]);
	}
	else if (character[1] != '\0') { //�ι�° ĭ�� �ι��ڰ� �ƴϸ� 2byte �����.
		glyph = new DoubleByteCharacter(character);
	}
#if 0
	else if (character[1] == '\t') {// ��Ű�̸� 8�� ���� ����.
		return new SingleByteCharacter((' ')*8);
	}
#endif
	else {
		glyph = 0;
	}
	return glyph;
}
















