//Composite.cpp
#include"Composite.h"
#include"SingleByteCharacter.h"
#include"DoubleByteCharacter.h"
#include"Row.h"
#include"Note.h"
#include"DummyRow.h"
#include"CreateGlyph.h"
#include"CreateProduct.h"
#include<cstdio>
#include<string>
#include<cstring> //21.08.25. ã�� ���� �߰� (��ҹ��� �����Լ�)
#include"CreateGlyph.h" // 21.07.14 Load ���� �߰�
#include"CreateProduct.h" // 21.07.14 Load ���� �߰�
#include <cstdio>
#include <mbstring.h>

using namespace std;
#pragma warning(disable:4996)


Composite::Composite(Long capacity)
	:glyphs(capacity) {
	this->capacity = capacity;
	this->length = 0;
	this->current = 0;
}

Composite::~Composite() {
	Long i = 0;
	Glyph* glyph;

	while (i < this->length) {
		glyph = this->glyphs.GetAt(i);
		delete glyph;
		i++;
	}
}

Long Composite::Add(Glyph* glyph) {
	Glyph::Add(glyph);

	Long index;
	
	if (this->length < this->capacity) {
		index = this->glyphs.Store(this->length, glyph);
	}
	else if (this->length >= this->capacity) {
		index = this->glyphs.AppendForRear(glyph);
		this->capacity++;
	}

	this->length++;
	
	return index;
}

Long Composite::Add(Long index, Glyph* glyph) {

	index = this->glyphs.Insert(index, glyph);
	this->capacity++;
	this->length++;

	return index;
}

Long Composite::Remove(Long index) {
	Glyph::Remove(index);
	
	index = this->glyphs.Delete(index);
	this->capacity--;
	this->length--;

	return index;
}

Glyph* Composite::GetChild(Long index) {
	Glyph::GetChild(index);

	Glyph* glyph = this->glyphs.GetAt(index);

	return glyph;
}

Composite& Composite::operator=(const Composite& source) { //ġȯ������
	return *this;
}

Composite::Composite(const Composite& source) { //���������

}

Long Composite::GetCapacity(){

	return this->capacity;
}

Long Composite::GetLength(){

	return this->length;
}

Long Composite::GetCurrent() {
	return this->current;
}

void Composite::MoveCurrent(Long current) {
	this->current = current;
}

Long Composite::Save(string fileName) { //string fileName
	Glyph::Save(fileName); //fileName

	FILE* file = 0;
	Long i = 0;
	Long j;
	Long rowLength;
	Glyph* row;
	Glyph* character;
	string character_= { 0, };


	//1. �ؽ�Ʈ������ wt�� ����.
	file = fopen(fileName.c_str(), "wt"); //"NotePad.txt" 
	//2. ������ ����� ��������,
	if (file != NULL) {

		//2.1. note�� length��ŭ �ݺ��Ѵ�.
		while (i < this->length) {
			//2.1.1. �ش� row�� length��ŭ �ݺ��Ѵ�.
			row = this->glyphs.GetAt(i);
			//rowLength = row->GetLength();
			character_ = row->GetContent();

			//fwrite(&character_, sizeof(string), 1, file); //sizeof(string), 1

			fputs(character_.c_str(), file);

			//2.1.2. �ؽ�Ʈ���Ͽ� ���๮�ڸ� write�Ѵ�.
			//fwrite("\r\n", sizeof(char), 1, file);

			//(21.11.17.�߰�) �� ������ ���� ���, �߰����� �ʴ´�.
			if (i + 1 < this->length) {
				fputs("\n", file);
			}


			i++;
		}
		//2.2. ������ �ݴ´�.
		fclose(file);
	}
	return this->length;
}

Long Composite::Load(string fileName) {
	Glyph::Load(fileName);

	FILE* file;
	char buffer[768] = { 0, };
	char character_[2];
	Long i;
	Long index = 0;
	Long j = 0;
	Glyph* row=0;
	Glyph* character=0;
	bool isFirst = true;
	string bufferS;
	string isLine;
	bool isLast = false;
	bool isOneLine = true;
	//Array<Glyph*> temps;

	//1. ���ϸ�, note�� �Է¹޴´�.
	//2. �ش� ������ �ؽ�Ʈ��� rt�� ����.
	file = fopen(fileName.c_str(), "rt");
	//3. ������ ����� ��������,
	if (file != NULL) {
		//3.1. ��ũ ���Ϸκ��� �� �پ� �Է¹޴´�.
		fgets(buffer, 768, file); 
		//3.2. ������ ���� �ƴѵ��� �ݺ��Ѵ�.
		while (!feof(file)) { 

			if (isFirst == false) {
				//3.3. ���ο� ���� ����� note�� Add�Ѵ�.
				row = new Row();
				this->Add(row);
			}

			//3.2.1. ��Ʈ�� ���� ���� ���Ѵ�.
			row = this->glyphs.GetAt(j);
			index = 0;

			//3.2.2. ���๮�ڰ� �ƴѵ��� �ݺ��Ѵ�.
			while (buffer[index] != '\n') {

				isFirst = false;

				//3.2.3.1. ���� ���ڰ� 1byte���� 2byte���� �����Ѵ�.
				character_[0] = buffer[index];
				character_[1] = buffer[index + 1];


#if 0
				//3.2.3.2. �ѱ��� ���,  �̰� �־ȵǴ��� �׽�Ʈ�غ���...........
				if (character_ >= "��" && character_ <= "�R") {
					character = new DoubleByteCharacter(character_);
					index = index + 2;
				}
#endif
				//3.2.3.2. 1byte�� ���,
				if(character_[0]>=0 && character_[0]<128){
					character_[0] = buffer[index];
					character = new SingleByteCharacter(character_[0]);
					index++;
				}
				//3.2.3.3. 2byte�� ���
				else {
					character = new DoubleByteCharacter(character_);
					index = index + 2;
				}
				//3.2.3.4. row�� Add�Ѵ�.
				row->Add(character); //row->Add(character);
				row->MoveCurrent(row->GetLength());
			}

			if (isFirst == true && buffer[index] == '\n') {
				//3.3. ���ο� ���� ����� note�� Add�Ѵ�.
				row = new Row();
				this->Add(row);
			}
			//3.3. ���ο� ���� ����� note�� Add�Ѵ�.
			//row = new Row();
			//this->Add(row);


			this->current = this->length;
			//note->MoveCurrent(note->GetLength());
			j++;
			fgets(buffer, 768, file);
//#if 0
			//(21.11.17.�߰�) ���� buffer�� �� ���� ���๮�ڰ� ������ ���๮�ڸ� �����ش�. *************
			bufferS = buffer;

			isLine = bufferS.back();
			if (isLine.compare("\n")!=0) {

				bufferS.push_back('\n');
				strcpy(buffer, bufferS.c_str());	
				isLast = true;
			}
			isOneLine = false;
			//******************************************************************************************
//#endif
		}

		if (isLast == true) {
			row = new Row();
			this->Add(row);

			//3.2.1. ��Ʈ�� ���� ���� ���Ѵ�.
			row = this->glyphs.GetAt(j);
			index = 0;

			//3.2.2. ���๮�ڰ� �ƴѵ��� �ݺ��Ѵ�.
			while (buffer[index] != '\n') {

				isFirst = false;

				//3.2.3.1. ���� ���ڰ� 1byte���� 2byte���� �����Ѵ�.
				character_[0] = buffer[index];
				character_[1] = buffer[index + 1];


#if 0
				//3.2.3.2. �ѱ��� ���,  �̰� �־ȵǴ��� �׽�Ʈ�غ���...........
				if (character_ >= "��" && character_ <= "�R") {
					character = new DoubleByteCharacter(character_);
					index = index + 2;
				}
#endif
				//3.2.3.2. 1byte�� ���,
				if (character_[0] >= 0 && character_[0] < 128) {
					character_[0] = buffer[index];
					character = new SingleByteCharacter(character_[0]);
					index++;
				}
				//3.2.3.3. 2byte�� ���
				else {
					character = new DoubleByteCharacter(character_);
					index = index + 2;
				}
				//3.2.3.4. row�� Add�Ѵ�.
				row->Add(character); //row->Add(character);
				row->MoveCurrent(row->GetLength());
			}

			this->current = this->length;	
		}
		//���๮�ڰ� ���� �� ���� ó�� ���� �߰�.

//#if 0
		else if (isOneLine == true) {
			//���๮�ڸ� �����ش�.
			bufferS = buffer;
			bufferS.push_back('\n');
			strcpy(buffer, bufferS.c_str());

			//3.2.1. ��Ʈ�� ���� ���� ���Ѵ�.
			row = this->glyphs.GetAt(j);
			index = 0;

			//3.2.2. ���๮�ڰ� �ƴѵ��� �ݺ��Ѵ�.
			while (buffer[index] != '\n') {
				//3.2.3.1. ���� ���ڰ� 1byte���� 2byte���� �����Ѵ�.
				character_[0] = buffer[index];
				character_[1] = buffer[index + 1];

				//3.2.3.2. 1byte�� ���,
				if (character_[0] >= 0 && character_[0] < 128) {
					character_[0] = buffer[index];
					character = new SingleByteCharacter(character_[0]);
					index++;
				}
				//3.2.3.3. 2byte�� ���
				else {
					character = new DoubleByteCharacter(character_);
					index = index + 2;
				}
				//3.2.3.4. row�� Add�Ѵ�.
				row->Add(character); //row->Add(character);
				row->MoveCurrent(row->GetLength());
			}

			this->current = this->length;
		}
//#endif

		//3.4. ��ũ������ �ݴ´�.
		fclose(file);
	}
	return this->length;
}

void Composite::First() { //��Ʈ�� ������ ���� ������ current�� 0����
	this->current = 0;
}

void Composite::Last() {//��Ʈ�� ������ ���� ������ current�� length��
	this->current = this->length-1;
}

void Composite::Previous() {
	//1. Previous ȣ������ ��
	  //1.1. ���� ���� ã�´�.
	  Long currentY = this->current;
	  Glyph* row = this->glyphs.GetAt(this->current);

	  //1.2. �ش� ���� current�� ã�´�.
	  Long currentX = row->GetCurrent();

	  //1.3. current�� 1�� ����.
	  currentX--;

	  //1.4. current�� 0���� ���� ���,
	  if (currentX < 0) {
		  //1.4.1. ���� y���� 1�� ����.
		  currentY--;

		  //1.4.2. y�� 0���� ũ�ų� ���� ���, �ش� y�� �ǳ����� x�� �̵��Ѵ�.
		  if (currentY >= 0) {
			  row = this->glyphs.GetAt(currentY);
			  currentX = row->GetLength();
		  }
		  //1.4.3. y�� 0���� ���� ���, y�� x�� 0���� ���Ѵ�.
		  else {
			  currentY = 0;
			  currentX = 0;
		  }
	  }
	  //1.5. MoveCurrent�Ѵ�.
	  this->current = currentY;

	  row = this->glyphs.GetAt(currentY);
	 
	  row->MoveCurrent(currentX);        
}

void Composite::Next() {

	//2. Next�� ȣ������ ��
	  //2.1. ���� ���� ã�´�.
	  Long currentY = this->current;
	  Glyph* row = this->glyphs.GetAt(this->current);

	  //2.2. �ش� ���� current�� ã�´�.
	  Long currentX = row->GetCurrent();

	  //2.3. current�� 1�� ���Ѵ�.
	  currentX++;

	  //2.4. current�� length���� Ŭ ���,
	  if (currentX > row->GetLength()) {
		  //2.4.1. ���� y���� 1�� ���Ѵ�.
		  currentY++;
		  //2.4.2. y�� note�� length���� ���� ���, �ش� y�� currentX�� 0���� �̵��Ѵ�.
		  if (currentY < this->length) {
			  currentX = 0;
		  }
		  //2.4.3. y�� noteLength���� ũ�ų� ���� ���, x�� y�� 1�� ����.
		  else {
			  currentX--;
			  currentY--;
		  }
	  }

	  //2.5. MoveCurrent�Ѵ�.
	  this->current = currentY;
	  row = this->glyphs.GetAt(currentY);
	  row->MoveCurrent(currentX);
}

void Composite::PreviousWord() {
	Glyph* character;
	string strCharacter;
	Long length;

	//3. PreviousWord�� ȣ������ ��
	  //3.1. ���� ���� ã�´�.
	  Long currentY = this->current;
	  Glyph* row = this->glyphs.GetAt(currentY);

	  //3.2. �ش� ���� current�� ã�´�.
	  Long currentX = row->GetCurrent();

	  //3.3. current�� 0���� ū ���,
	  if (currentX > 0) {
		  //3.3.1. Ŀ�� ���� �������� �����ϸ�, 
		  currentX--;
		  character = row->GetChild(currentX);
		  strCharacter = character->GetContent();
		  
		  if (strCharacter == " " || strCharacter == "\t") {
			  //1. x�� 0���� ũ��, ������ ���� �ݺ��Ѵ�.
			  while (currentX > 0 && (strCharacter == " " || strCharacter == "\t")) {
				  currentX--;
				  character = row->GetChild(currentX);
				  strCharacter = character->GetContent();
			  }

			  //2. x�� 0���� ũ��, 
				  //2.1. x�� 0���� ũ��, ������ �ƴѵ��� �ݺ��Ѵ�.
			  while (currentX > 0 && strCharacter != " " && strCharacter != "\t") {
				  currentX--;
				  character = row->GetChild(currentX);
				  strCharacter = character->GetContent();
			  }
			  if (currentX > 0) {
			      currentX++;
			  }
			  
		  }
		  //3.3.2. Ŀ�� ���� �������� �������� ������, X�� 0���� ũ��, ������ �ƴѵ��� �ݺ��Ѵ�.
		  else {
			  while (currentX > 0 && strCharacter != " " && strCharacter != "\t") {
				  currentX--;
				  character = row->GetChild(currentX);
				  strCharacter = character->GetContent();
			  }
			  if (currentX > 0) {
				  currentX++;
			  }
		  }
	  }
	  //3.4. current�� 0���� ���� ���,
	  else {
		  //3.4.1. ���� y���� 1�� ����.
		  currentY--;
		  //3.4.2. y�� 0���� ũ�ų� ���� ���,  �ش� y��ǥ�� �ǳ����� current �̵��Ѵ�.
		  if (currentY >= 0) {
			  row = this->GetChild(currentY);
			  length = row->GetLength();
			  currentX = length;
		  }
		  //3.4.3. y�� 0���� ���� ���, x,y��ǥ�� 0,0���� ���Ѵ�.
		  else {
			  currentX = 0;
			  currentY = 0;
		  }
	  }
	  //3.5. MoveCurrent�Ѵ�.
	  this->current = currentY;
	  row = this->glyphs.GetAt(currentY);
      row->MoveCurrent(currentX);
}

void Composite::NextWord() {
	Glyph* character;
	string strCharacter;
	Long length;

	//4. NextWord�� ȣ���Ͽ��� ��
	  //4.1. ���� ���� ã�´�.
	  Long currentY = this->current;
	  Glyph* row = this->glyphs.GetAt(currentY);
	  length = row->GetLength();

	  //4.2. �ش� ���� current�� ã�´�.
	  Long currentX = row->GetCurrent();

	  //4.3. current�� row�� length���� ���� ���,
	  if (currentX < length) {
		  //4.3.1. Ŀ�� �տ� ���ڷ� �����ϸ�,
		  character = row->GetChild(currentX);
		  strCharacter = character->GetContent();
		  if (strCharacter != " " && strCharacter != "\t") {
			  //1. x�� length���� �۰�, ������ ���� �ݺ��Ѵ�.
			  while (currentX < length -1 && strCharacter != " " && strCharacter != "\t") {
				  currentX++;
				  character = row->GetChild(currentX);
				  strCharacter = character->GetContent();
			  }
			  if (currentX <= length - 1 && strCharacter != " " && strCharacter != "\t") {
				  currentX++;
			  }

			  //2. x�� length���� ������, length���� �۰� ������ ���� �ݺ��Ѵ�.
			  while (currentX < length - 1 && (strCharacter==" " || strCharacter == "\t")) {
				  currentX++;
				  character = row->GetChild(currentX);
				  strCharacter = character->GetContent();
			  }
			  if (currentX <= length - 1 && (strCharacter == " " || strCharacter == "\t")) {
				  currentX++;
			  }
			 
		  }
		  //4.3.2. Ŀ�� ���� �������� �����ϸ�, x�� length���� �۰� ������ ���� �ݺ��Ѵ�.
		  else {
			  while (currentX < length && (strCharacter == " " || strCharacter == "\t")) {
				  currentX++;
				  character = row->GetChild(currentX);
				  strCharacter = character->GetContent();
			  }
		  }
	  }
	  //4.4. current�� row�� length���� ũ�ų� ���� ���,
	  else {
		  //4.4.1. ���� y���� 1�� ���Ѵ�.
		  currentY++;
		  //4.4.2. note�� length���� y�� ���� ���, �ش� y��ǥ�� ù��°�� x�� �̵��Ѵ�.
		  if (currentY < this->length) {
			  currentX = 0;
		  }
		  //4.4.3. y�� note�� length�� ���ų� �� ū ���, ���� �̵����� �ʴ´�.
		  else {
			  currentY--;
		  }
	  }
	  //4.5. MoveCurrent �Ѵ�.
	  this->current = currentY;
	  row = this->glyphs.GetAt(currentY);
	  row->MoveCurrent(currentX);


}


#if 0
void Composite::Up() {
	//1. ���� ���� currentX���� byte �� ����Ѵ�.
	Long byteUp = 0;
	Long byte = 0;
	Glyph* character;
	string strCharacter;
	Long currentY = this->current;
	Glyph* row = this->glyphs.GetAt(currentY);
	Long currentX = row->GetCurrent();
	Long i = 0;
	char chaCharacter[3];

	if (currentY - 1 >= 0) {
		//1. byteUp ����Ѵ�. (�ؿ��� �̿��� byte�� ����ϱ� ���� �ݺ��ϴ� ��.)
		while (i < currentX) { 
			character = row->GetChild(i);
			strCharacter = character->GetContent();
			strcpy(chaCharacter, strCharacter.c_str());

			//1.1. 1byte�� 1�� ���Ѵ�.
			if (chaCharacter[0] >= 0 && chaCharacter[0] < 128) {
				byte++;
			}
			//1.2. 2byte�� 2�� ���Ѵ�.
			else {
				byte = byte + 2;
			}
#if 0
			//1.1. 2byte�� 2�� ���Ѵ�.
			if (strCharacter >= "��" && strCharacter <= "�R") {
				byte = byte + 2;
			}
			//1.2. 1byte�� 1�� ���Ѵ�.
			else {
				byte++;
			}
#endif
			i++;
		}
		//2. y-1 ��ǥ���� ���� ��ǥ�� byte ������ �۰ų� ���� ���� byte�� ����.
		i = 0;
		row = this->glyphs.GetAt(currentY - 1);
		while (byteUp <= byte && i < row->GetLength()) {
			character = row->GetChild(i);
			strCharacter = character->GetContent();
			strcpy(chaCharacter, strCharacter.c_str());

			//1.1. 1byte�� 1�� ���Ѵ�.
			if (chaCharacter[0] >= 0 && chaCharacter[0] < 128) {
				byteUp++;
			}
			//1.2. 2byte�� 2�� ���Ѵ�.
			else {
				byteUp = byteUp + 2;
		    }
#if 0 
			//2.1. 2byte�� 2�� ���Ѵ�.
			if (strCharacter >= "��" && strCharacter <= "�R") {
				byteUp = byteUp + 2;
			}
			//2.2. 1byte�� 1�� ���Ѵ�.
			else {
				byteUp++;
			}
#endif
			i++;
		}
		if (byteUp > byte) {
			i--;
		}


		//3. y-1��ǥ�� �̵��ϰ� �ش� x��ǥ�� �̵��Ѵ�.
		this->current = currentY - 1;
		row = this->glyphs.GetAt(currentY - 1);
		row->MoveCurrent(i); //i�� currentX��.
	}
}


void Composite::Down() {
	Long byteDown = 0;
	Long byte = 0;
	Glyph* character;
	string strCharacter;
	Long currentY = this->current;
	Glyph* row = this->glyphs.GetAt(currentY);
	Long currentX = row->GetCurrent();
	Long i = 0;
	char chaCharacter[3];

	//1. currentY+1�� length���� ���� ���,
	if (currentY + 1 < this->length) {
		//1.1. ���� ��ǥ������ byte�� ����Ѵ�.
		while (i < currentX) {
			character = row->GetChild(i);
			strCharacter = character->GetContent();
			strcpy(chaCharacter, strCharacter.c_str());

			//1.1.1. 1byte�� 1�� ���Ѵ�.
			if (chaCharacter[0] >= 0 && chaCharacter[0] < 128) {
				byte++;
			}
			//1.1.2. 2byte�� 2�� ���Ѵ�.
			else {
				byte = byte + 2;
			}
#if 0
			//1.1.1. 2byte�� 2�� ���Ѵ�.
			if (strCharacter >= "��" && strCharacter <= "�R") {
				byte = byte + 2;
			}
			//1.1.2. 1byte�� 1�� ���Ѵ�.
			else {
				byte++;
			}
#endif
			i++;
		}
		//1.2. y+1 ��ǥ���� ���� ��ǥ�� byte ������ �۰ų� ���� ���� byte ���� ����.
		i = 0;
		row = this->glyphs.GetAt(currentY + 1);
		while (byteDown <= byte && i < row->GetLength()) {
			character = row->GetChild(i);
			strCharacter = character->GetContent();

			strcpy(chaCharacter, strCharacter.c_str());

			//1.1.1. 1byte�� 1�� ���Ѵ�.
			if (chaCharacter[0] >= 0 && chaCharacter[0] < 128) {
				byteDown++;
			}
			//1.1.2. 2byte�� 2�� ���Ѵ�.
			else {
				byteDown = byteDown + 2;
		}

#if 0
			//1.2.1. 2byte�� 2�� ���Ѵ�
			if (strCharacter >= "��" && strCharacter <= "�R") {
				byteDown = byteDown + 2;
			}
			//1.2.2. 1byte�� 1�� ���Ѵ�.
			else {
				byteDown++;
			}
#endif
			i++;
		}
		if (byteDown > byte) {
			i--;
		}
		//1.3. y+1 ��ǥ�� �̵��ϰ�, �ش� x��ǥ�� �̵��Ѵ�.
		this->current = currentY + 1;
		row = this->glyphs.GetAt(currentY + 1);
		row->MoveCurrent(i); //i�� currentX��.
	}
}
#endif

Glyph* Composite::Split(Glyph* dummyRow, int location) { //index: ���ٰ� insert����
	int dummyRowIndex;
	Glyph* character;


	//1. insert�� dummyRow, �ڸ� ��ġ�� �Է¹޴´�.
	//2. length��ŭ �ݺ��Ѵ�.
	while (location < this->length) {
		//2.1. ��ġ ������ ���ڸ� dummyRow�� Add�Ѵ�.
		character = this->glyphs.GetAt(location);
		dummyRow->Add(character);

		//4.2. Add�� ���ڸ� ���� �ٿ��� �����.
		this->glyphs.Delete(location);
		this->capacity--;
		this->length--;
	}

	//5. dummyRow�� ����Ѵ�.
	return dummyRow;
}


void Composite::Combine(Glyph* dummyRow) {
	int i = 0;
	Glyph* character;

	//1.Combineȣ���� ��
	  //1.1. ���� �Է¹޴´�.
	  //1.2. �Է¹��� ���� length��ŭ �ݺ��Ѵ�.
	  while (i < dummyRow->GetLength()) {
		  //1.2.1. ���ڸ� �д´�.
		  character = dummyRow->GetChild(i);

		  //1.2.2. ���� �ٿ� �߰��Ѵ�.
		  this->Add(character);

		  //1.2.3. �Է¹��� �ٿ��� �����Ѵ�.
		  dummyRow->Remove(i);
	  }
	  //1.3. dummyRow���� �� �Ű����� �ٿ��� �����Ѵ�.(�������̽�����)
}

Glyph* Composite::SplitForFront(Glyph* dummyRow, int location) {
	Glyph* character;
	int i = 0;

	//1. length��ŭ �ݺ��Ѵ�.
	while (location < this->GetLength()) {
		//1.1. ��ġ ������ ���ڸ� dummyRow�� Add�Ѵ�. (���� �տ�)
		character = this->glyphs.GetAt(location);
		dummyRow->Add(i, character);

		//1.2. Add�� ���ڸ� ���� �ٿ��� �����.
		this->glyphs.Delete(location);
		this->capacity--;
		this->length--;
		i++;
	}

	//2. dummyRow�� ����Ѵ�.
	return dummyRow;

}

void Composite::FindSameTextWithDown(string findStr, bool matchCase, int firstCurrentY, int beforeLineChangeCurrentX,
	int* firstY, int* firstX, int* endY, int* endX)  {
	
	int noteLength;
	int rowLength;
	int restLength;
	int currentY;
	int tempCurrentY;
	Glyph* row;
	Glyph* character;
	bool isThereSameText = false;
	string strCharacter;
	string temp;
	int i = beforeLineChangeCurrentX;
	int j;
	int k;
	const char* cFindStr = findStr.c_str();
	const char* cTemp;
	bool isThereLineChange = false;

	//1. ã�� ����, ��ҹ��� ���п���, �ڵ����� �� ĳ���� ��ǥ�� �Է¹޴´�.

	//(21.09.17 �߰�) findStr�� ���๮�ڰ� ���ԵǾ� �ִ��� Ȯ���Ѵ�.
	size_t nPos = findStr.find("\n");
	if (nPos != string::npos) { //���๮�ڰ� �ִ� ���,
		isThereLineChange = true;
	}

	
	//2. ã�� ������ �� ������ ���Ѵ�.
	//int findLength = findStr.size();
	
	const unsigned char* constStr = reinterpret_cast<const unsigned char*> (findStr.c_str());
	int findLength = _mbslen(constStr);
	

	currentY = firstCurrentY;
	noteLength = this->length;

	//3. ���� ���ڰ� ���ԵǾ� ���� ���� ���, ������ ���� �ƴѵ��� �ݺ��Ѵ�.
	while (isThereLineChange == false && currentY < noteLength && isThereSameText == false) {

		//3.1. ���� ĳ���� �ִ� ���� ã�´�.
		row = this->glyphs.GetAt(currentY);
		rowLength = row->GetLength();

		//3.2. i�� �ʱ�ȭ�ϰ� restLength�� ���Ѵ�.
		if (currentY == firstCurrentY) {
			i = beforeLineChangeCurrentX;
			restLength = rowLength - i;
		}
		else {
			i = 0;
			restLength = rowLength;
		}

		//3.3. restLength�� findLength���� ũ�ų� ����, ���� �ܾ ���� ���� �ݺ��Ѵ�.
		while (restLength >= findLength && isThereSameText == false) {
			
			//3.3.1. �ش� �ٿ��� ã�� ������ �� ������ŭ ���ڸ� �����.
			temp.clear(); //���ڿ� �����
			j = i;
			k = 0;
			while (k < findLength) {//temp.length()
				character = row->GetChild(j);
				strCharacter = character->GetContent();
				temp.append(strCharacter);
				j++;
				k++;
			}
			cTemp = temp.c_str();

			//3.3.2. ��ҹ��� �����ϸ�,
			if (matchCase == true) {
				if (strcmp(cTemp, cFindStr) == 0) {
					isThereSameText = true;
					(*firstY) = currentY;
					(*firstX) = i;
					(*endY) = currentY;
					(*endX) = j;
				}
				else {
					isThereSameText = false;
				}
			}
			//3.3.3. ��ҹ��� �������� ������,
			else if (matchCase == false) {
				if (stricmp(cTemp, cFindStr) == 0) {
					isThereSameText = true;
					(*firstY) = currentY;
					(*firstX) = i;
					(*endY) = currentY;
					(*endX) = j;
				}
				else {
					isThereSameText = false;
				}
			}
			restLength--;
			i++;
		}
		currentY++;
	}

	//4. ���� ���ڰ� ���ԵǾ� �ִ� ���, ������ ���� �ƴѵ��� �ݺ��Ѵ�.
	while (isThereLineChange == true && currentY < noteLength && isThereSameText == false) {
		
		//4.1. temp �ʱ�ȭ�Ѵ�.
		temp.clear();
		tempCurrentY = currentY;
		j = i;
		k = 0;

		//4.2. findLength��ŭ ���� ���ڸ� �����.
		while (k < findLength && tempCurrentY < noteLength && isThereSameText == false) {
			
			row = this->glyphs.GetAt(tempCurrentY);

			while (k < findLength && j < row->GetLength()) {

				character = row->GetChild(j);
				strCharacter = character->GetContent();
				temp.append(strCharacter);
				j++;
				k++;
			}
			if (k < findLength) {
				tempCurrentY++;
				temp.append("\n");
				j = 0;
				k++;
			}
		}

		cTemp = temp.c_str();

		//4.3. ��ҹ��� �����ϸ�,
		if (matchCase == true) {
			if (strcmp(cTemp, cFindStr) == 0) {
				isThereSameText = true;
				(*firstY) = currentY;
				(*firstX) = i;
				(*endY) = tempCurrentY;
				(*endX) = j;
			}
			else {
				isThereSameText = false;
			}
		}

		//4.4. ��ҹ��� �������� ������,
		else if (matchCase == false) {
			if (stricmp(cTemp, cFindStr) == 0) {
				isThereSameText = true;
				(*firstY) = currentY;
				(*firstX) = i;
				(*endY) = tempCurrentY;
				(*endX) = j;
			}
			else {
				isThereSameText = false;
			}
		}

		i++;
		row = this->glyphs.GetAt(currentY);
		if (row->GetLength() <= i) {
			currentY++;
			i = 0;
		}
	}

	//5. ���� �ܾ ������ firstXY, endXY�� 0�� �Է��Ѵ�.
	if (isThereSameText == false) {
		(*firstY) = 0;
		(*firstX) = 0;
		(*endY) = 0;
		(*endX) = 0;
	}

}

void Composite::FindSameTextWithUp(string findStr, bool matchCase, int firstCurrentY, int beforeLineChangeCurrentX,
	int* firstY, int* firstX, int* endY, int* endX) {

	int rowLength;
	int restLength;
	Glyph* row;
	Glyph* character;
	bool isThereSameText = false;
	string strCharacter;
	string temp;
	bool isThereLineChange = false;
	int i;
	int j;
	int k;
	bool isFirstX;
	int tempCurrentY;
	const char* cFindStr = findStr.c_str();
	const char* cTemp;

	//1. ������ ó���� �ƴѵ��� �ݺ��Ѵ�.
	//int findLength = findStr.size();

	//(21.09.17 �߰�) findStr�� ���๮�ڰ� ���ԵǾ� �ִ��� Ȯ���Ѵ�.
	size_t nPos = findStr.find("\n");
	if (nPos != string::npos) { //���๮�ڰ� �ִ� ���,
		isThereLineChange = true;
	}
	
	//i�� �ʱ�ȭ�Ѵ�.
	int currentY = firstCurrentY;
	i = beforeLineChangeCurrentX - 1;

	while (i < 0 && currentY>=0) {
		currentY--;
		if (currentY >= 0) {
			row = this->glyphs.GetAt(currentY);
			i = row->GetLength() - 1;
		}
	}

	//2. ã�� ������ �� ������ ���Ѵ�.
	const unsigned char* constStr = reinterpret_cast<const unsigned char*> (findStr.c_str());
	int findLength = _mbslen(constStr);

	

	//3. ���� ���ڰ� ���ԵǾ� ���� ���� ���, ������ ó���� �ƴѵ��� �ݺ��Ѵ�.
	while (isThereLineChange == false && currentY >= 0 && isThereSameText == false) {
		//1.1. ���� ĳ���� �ִ� ���� ã�´�.
		row = this->glyphs.GetAt(currentY);
		rowLength = row->GetLength();

		//1.2. i�� �ʱ�ȭ�ϰ� restLength�� ���Ѵ�.
		if (currentY == firstCurrentY) {
			i = beforeLineChangeCurrentX - 1;
			restLength = i + 1;
		}
		else {
			i = rowLength - 1;
			restLength = rowLength;
		}

		//1.3. restLength�� findLength���� ũ�ų� ����, ���� �ܾ ���µ��� �ݺ��Ѵ�.
		while (restLength >= findLength && isThereSameText == false) {
			
			//1.3.1. �ش� �ٿ��� ���� currentX���� ã�� ������ �� ������ŭ ���ڸ� �����.
			temp.clear(); //���ڿ� �����
			j = i;
			k = 0;
			while (k < findLength) { //temp.length() 
				character = row->GetChild(j);
				strCharacter = character->GetContent();
				temp.insert(0, strCharacter);
				j--;
				k++;
			}
			cTemp = temp.c_str();

			//1.3.2. ��ҹ��� �����ϸ�
			if (matchCase == true) {
				if (strcmp(cTemp, cFindStr) == 0) {
					isThereSameText = true;
					(*firstY) = currentY;
					(*firstX) = j+1;
					(*endY) = currentY;
					(*endX) = i + 1;
				}
				else {
					isThereSameText = false;
				}
			}
			//1.3.3. ��ҹ��� ���� ���ϸ�
			else if (matchCase == false) {
				if (stricmp(cTemp, cFindStr) == 0) {
					isThereSameText = true;
					(*firstY) = currentY;
					(*firstX) = j+1;
					(*endY) = currentY;
					(*endX) = i + 1;
				}
				else {
					isThereSameText = false;
				}
			}
			restLength--;
			i--;
		}
		currentY--;
	}

	//4. ���� ���ڰ� ���ԵǾ� �ִ� ���, ������ ó���� �ƴѵ��� �ݺ��Ѵ�.
	while (isThereLineChange == true && currentY >= 0 && isThereSameText == false) {

		//4.1. temp �ʱ�ȭ�Ѵ�.
		temp.clear();
		tempCurrentY = currentY;
		isFirstX = true;

		j = i;
		k = 0;

		while (k < findLength && tempCurrentY >= 0 && isThereSameText == false) {	
			row = this->glyphs.GetAt(tempCurrentY);

			if (isFirstX == false) {
				j = row->GetLength() - 1;
			}

			while (k < findLength && j >= 0) {
				character = row->GetChild(j);
				strCharacter = character->GetContent();
				temp.insert(0, strCharacter);
				j--;
				k++;
			}
			if (k < findLength) {
				tempCurrentY--;
				temp.insert(0, "\n");
				k++;	
				isFirstX = false;
			}
		}
		cTemp = temp.c_str();

		//4.2. ��ҹ��� �����ϸ�
		if (matchCase == true) {
			if (strcmp(cTemp, cFindStr) == 0) {
				isThereSameText = true;
				(*firstY) = tempCurrentY;
				(*firstX) = j + 1;
				(*endY) = currentY;
				(*endX) = i + 1;
			}
			else {
				isThereSameText = false;
			}
		}
		//4.3. ��ҹ��� ���� ���ϸ�
		else if (matchCase == false) {
			if (stricmp(cTemp, cFindStr) == 0) {
				isThereSameText = true;
				(*firstY) = tempCurrentY;
				(*firstX) = j + 1;
				(*endY) = currentY;
				(*endX) = i + 1;
			}
			else {
				isThereSameText = false;
			}
		}

		i--;

		if (i < 0) {
			currentY--;

			if (currentY >= 0) {
				row = this->glyphs.GetAt(currentY);
				i = row->GetLength() - 1;
			}
				
		}
	}


	//5. ���� �ܾ ������, firstXY, endXY�� 0�� �Է��Ѵ�.
	if (isThereSameText == false) {
		(*firstY) = 0;
		(*firstX) = 0;
		(*endY) = 0;
		(*endX) = 0;
	}
}

void Composite::ReplaceSameText(string findStr, string replaceStr, string selectedStr, bool matchCase, bool searchAround,
	int firstCurrentY,
	int beforeLineChangeCurrentX, int* firstY, int* firstX, int* endY, int* endX, int* exception) {

	char* chaCharacter = 0;
	char afterCharacter[2];
	int currentY;
	int rowLength;
	int restLength;
	Glyph* row;
	Glyph* character;
	bool isSameText = false;
	bool isThereSameText = false;
	string strCharacter;
	string temp;
	int i = 1;
	int j;
	int k;
	int noteLength;
	int selectedFirstX;
	CreateGlyph* creator = 0;
	const char* cFindStr = findStr.c_str();
	const char* cSelectedStr = selectedStr.c_str();
	const char* cReplaceStr = replaceStr.c_str();
	const char* cTemp;

	//�����ʱ�ȭ
	(*exception) = 0;

	//0. ã�� ����, �ٲ� ������ �� ������ ���Ѵ�.
	const unsigned char* constStr = reinterpret_cast<const unsigned char*> (findStr.c_str());
	int findLength = _mbslen(constStr);

	//constStr = reinterpret_cast<const unsigned char*> (const_cast<const char*>(replaceStr.c_str()));
	constStr = reinterpret_cast<const unsigned char*> (replaceStr.c_str());
	int replaceLength = _mbslen(constStr);


	//1. ã�� ����, �ٲ� ����, ���� ���õ� ����, ��ҹ��� ����, �ڵ����� �� ĳ���� ��ǥ�� �Է¹޴´�.
	//2. ���� ���õ� �ܾ ã���� �ϴ� �ܾ�� ������ Ȯ���Ѵ�.
	  //2.1. ��ҹ��� �����ϸ�,
	if (matchCase == true && strcmp(cFindStr, cSelectedStr) == 0) {
		isSameText = true;
	}

	//2.2. ��ҹ��� ���� ���ϸ�,
	else if (matchCase == false && stricmp(cFindStr, cSelectedStr) == 0) {
		isSameText = true;
	}

	//2.3. ���õ� �ܾ�� ã���� �ϴ� �ܾ ������, (�ٲٷ��� �ܾ �ƴ�)
	if (isSameText == true) {
		//2.3.0. string���� char������ ��ȯ�Ѵ�.
		//chaCharacter = new char(replaceStr.length());
		chaCharacter = new char[replaceStr.length() +1];
		strcpy(chaCharacter, cReplaceStr);


		//2.3.1. ���õ� �ܾ �����.
		selectedFirstX = beforeLineChangeCurrentX - findLength;
		row = this->glyphs.GetAt(firstCurrentY);


		while (i <= findLength) {
			row->Remove(selectedFirstX);
			i++;
		}

		//2.3.2. �ٲ� ������ ���´�.
		creator = new CreateProduct();
		i = 0;
		j = selectedFirstX;
		while (i < replaceStr.length()) {
			afterCharacter[0] = chaCharacter[i];
			afterCharacter[1] = chaCharacter[i + 1];

			//1. 1byte�� ���,
			if (afterCharacter[0] >= 0 && afterCharacter[0] < 128) {
				afterCharacter[1] = '\0';
				character = creator->Make(afterCharacter);
				i++;
			}
			//2. 2byte�� ���,
			else {
				character = creator->Make(afterCharacter);
				i = i + 2;
			}
			//3. ������ ���´�.
			row->Add(j, character);
			j++;
		}

		//2.3.3. ĳ���� ��ǥ�� ���� ���� ������ �� �ڷ� ���Ѵ�.
		beforeLineChangeCurrentX = selectedFirstX + replaceLength;
	}
	//FindSameTextWithDown�� ���� ----------------------------------------------------------------------------------
	//3. ������ ���� �ƴѵ��� �ݺ��Ѵ�.

	//(21.08.30 �߰�) ������ ��ġ�� �������ִ� ���,
	if (searchAround == true) {
		currentY = 0;
	}
	else {
		currentY = firstCurrentY;
	}
	noteLength = this->length;


	//3. ������ ���� �ƴѵ��� �ݺ��Ѵ�.
	while (currentY < noteLength && isThereSameText == false) {

		//3.1. ���� ĳ���� �ִ� ���� ã�´�.
		row = this->glyphs.GetAt(currentY);
		rowLength = row->GetLength();

		//3.2. i�� �ʱ�ȭ�ϰ� restLength�� ���Ѵ�.
		if (currentY == firstCurrentY) {

			if (searchAround == true) {
				i = 0;
			}
			else {
				i = beforeLineChangeCurrentX;
			}
			restLength = rowLength - i;
		}
		else {
			i = 0;
			restLength = rowLength;
		}

		//3.3. restLength�� findLength���� ũ�ų� ����, ���� �ܾ ���� ���� �ݺ��Ѵ�.
		while (restLength >= findLength && isThereSameText == false) {

			//3.3.1. �ش� �ٿ��� ã�� ������ �� ������ŭ ���ڸ� �����.
			temp.clear(); //���ڿ� �����
			j = i;
			k = 0;
			while (k < findLength) {//temp.length()
				character = row->GetChild(j);
				strCharacter = character->GetContent();
				temp.append(strCharacter);
				j++;
				k++;
			}
			cTemp = temp.c_str();

			//3.3.2. ��ҹ��� �����ϸ�,
			if (matchCase == true) {
				if (strcmp(cTemp, cFindStr) == 0) {
					isThereSameText = true;
					(*firstY) = currentY;
					(*firstX) = i;
					(*endY) = currentY;
					(*endX) = j;
				}
				else {
					isThereSameText = false;
				}
			}
			//3.3.3. ��ҹ��� �������� ������,
			else if (matchCase == false) {
				if (stricmp(cTemp, cFindStr) == 0) {
					isThereSameText = true;
					(*firstY) = currentY;
					(*firstX) = i;
					(*endY) = currentY;
					(*endX) = j;
				}
				else {
					isThereSameText = false;
				}
			}
			restLength--;
			i++;
		}
		currentY++;
	}
	//4. �� ���� ���� �ܾ�� ������, ó�� ������ �� replace�� ���,
	if (isThereSameText == false) {

//#if 0
		if (isSameText == true) {
			(*firstY) = firstCurrentY; 
			(*firstX) = selectedFirstX;
			(*endY) = firstCurrentY;
			(*endX) = beforeLineChangeCurrentX;
			(*exception) = 1;
		}
//#endif

		//4.1. �� ���� ���� �ܾ ����, ó�� ������ �� replace���� ���� ���, firstXY, endXY�� 0�� �Է��Ѵ�.
		else if(isSameText == false && searchAround == false){
			(*firstY) = 0;
			(*firstX) = 0;
			(*endY) = 0;
			(*endX) = 0;
		}
	}
	//------------------------------------------------------------------------------------------------------

//#if 0
	//�Ҵ����� ���ش�.
	if (creator != 0) {
		delete creator;
		creator = 0;
	}
	if (chaCharacter != 0) {
		delete[] chaCharacter;
		chaCharacter = 0;
	}
//#endif


}


#if 0
void Composite::ReplaceAllSameText(string findStr, string replaceStr, bool matchCase, int firstCurrentY,
	int beforeLineChangeCurrentX) {

	char* chaCharacter;
	char afterCharacter[2];
	int currentY;
	int rowLength;
	int restLength;
	Glyph* row;
	Glyph* character;
	bool isSameText = false;
	bool isThereSameText = false;
	string strCharacter;
	string temp;
	int i = 1;
	int j;
	int k;
	int noteLength;
	int selectedFirstX;
	const char* cFindStr = findStr.c_str();
	const char* cReplaceStr = replaceStr.c_str();
	const char* cTemp;



	//0. ã�� ����, �ٲ� ������ �� ������ ���Ѵ�.
	const unsigned char* constStr = reinterpret_cast<const unsigned char*> (findStr.c_str());
	int findLength = _mbslen(constStr);

	constStr = reinterpret_cast<const unsigned char*> (replaceStr.c_str());
	int replaceLength = _mbslen(constStr);




}
#endif






