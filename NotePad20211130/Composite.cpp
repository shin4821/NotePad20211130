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
#include<cstring> //21.08.25. 찾기 위해 추가 (대소문자 구분함수)
#include"CreateGlyph.h" // 21.07.14 Load 위해 추가
#include"CreateProduct.h" // 21.07.14 Load 위해 추가
#include <cstdio>
#include <mbstring.h>
#include <fstream> //C++ 입출력 함수
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

Composite& Composite::operator=(const Composite& source) { //치환연산자
	return *this;
}

Composite::Composite(const Composite& source) { //복사생성자

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

Long Composite::Save(string fileName) { 
	Glyph::Save(fileName); 

	FILE* file = 0;
	Long i = 0;
	Long j;
	Long rowLength;
	Glyph* row;
	Glyph* character;
	string character_= { 0, };
	std::ofstream ofs;

	//1. 텍스트파일을 ofstream으로 연다.
	ofs.open(fileName);

	//2. 파일이 제대로 열렸으면,
	if (!ofs.fail()) {

		//2.1. note의 length만큼 반복한다.
		while (i < this->length) {
			//2.1.1. 해당 row의 length만큼 반복한다.
			row = this->glyphs.GetAt(i);
			character_ = row->GetContent();

			ofs << character_.c_str() <<std::endl;

			i++;
		}
		//2.2. 파일을 닫는다.
		ofs.close();
	}
	return this->length;
}

#if 0
Long Composite::Save(string fileName) { //string fileName
	Glyph::Save(fileName); //fileName

	FILE* file = 0;
	Long i = 0;
	Long j;
	Long rowLength;
	Glyph* row;
	Glyph* character;
	string character_ = { 0, };


	//1. 텍스트파일을 wt로 연다.
	file = fopen(fileName.c_str(), "wt"); //"NotePad.txt" 
	//2. 파일이 제대로 열렸으면,
	if (file != NULL) {

		//2.1. note의 length만큼 반복한다.
		while (i < this->length) {
			//2.1.1. 해당 row의 length만큼 반복한다.
			row = this->glyphs.GetAt(i);
			//rowLength = row->GetLength();
			character_ = row->GetContent();

			//fwrite(&character_, sizeof(string), 1, file); //sizeof(string), 1

			fputs(character_.c_str(), file);

			//2.1.2. 텍스트파일에 개행문자를 write한다.
			//fwrite("\r\n", sizeof(char), 1, file);

			//(21.11.17.추가) 맨 마지막 줄일 경우, 추가하지 않는다.
			if (i + 1 < this->length) {
				fputs("\n", file);
			}


			i++;
		}
		//2.2. 파일을 닫는다.
		fclose(file);
	}
	return this->length;
}


#endif


Long Composite::Load(string fileName) {
	Glyph::Load(fileName);

	char buffer[768] = { 0, };
	char character_[2];
	Long i;
	Long index = 0;
	Long j = 0;
	Glyph* row = 0;
	Glyph* character = 0;
	bool isFirst = true;
	std::ifstream ifs;

	//1. 파일명, note를 입력받는다.
	//2. 해당 파일을 ofstream로 연다.
	ifs.open(fileName);

	//3. 파일이 제대로 열렸으면,
	if (!ifs.fail()) {
		
		//3.1. 파일의 끝이 아닌동안 반복한다.
		while (ifs.getline(buffer, 768)) {

			if (isFirst == false) {
				//3.3. 새로운 줄을 만들어 note에 Add한다.
				row = new Row();
				this->Add(row);
			}

			//3.2.1. 노트의 현재 줄을 구한다.
			row = this->glyphs.GetAt(j);
			index = 0;

			//3.2.2. 개행문자가 아닌동안 반복한다.
			while (buffer[index] != '\n' && buffer[index] != '\0') {

				isFirst = false;

				//3.2.3.1. 현재 문자가 1byte인지 2byte인지 구분한다.
				character_[0] = buffer[index];
				character_[1] = buffer[index + 1];


				//3.2.3.2. 1byte인 경우,
				if (character_[0] >= 0 && character_[0] < 128) {
					character_[0] = buffer[index];
					character = new SingleByteCharacter(character_[0]);
					index++;
				}
				//3.2.3.3. 2byte인 경우
				else {
					character = new DoubleByteCharacter(character_);
					index = index + 2;
				}
				//3.2.3.4. row에 Add한다.
				row->Add(character); //row->Add(character);
				row->MoveCurrent(row->GetLength());
			}

			if (isFirst == true && (buffer[index] == '\n' || buffer[index] == '\0')) {
				//3.3. 새로운 줄을 만들어 note에 Add한다.
				row = new Row();
				this->Add(row);
			}

			this->current = this->length;
			j++;
		}

		//3.4. 디스크파일을 닫는다.
		ifs.close();
	}
	return this->length;
}

#if 0
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

	//1. 파일명, note를 입력받는다.
	//2. 해당 파일을 텍스트모드 rt로 연다.
	file = fopen(fileName.c_str(), "rt");
	//3. 파일이 제대로 열렸으면,
	if (file != NULL) {
		//3.1. 디스크 파일로부터 한 줄씩 입력받는다.
		fgets(buffer, 768, file); 
		//3.2. 파일의 끝이 아닌동안 반복한다.
		while (!feof(file)) { 

			if (isFirst == false) {
				//3.3. 새로운 줄을 만들어 note에 Add한다.
				row = new Row();
				this->Add(row);
			}

			//3.2.1. 노트의 현재 줄을 구한다.
			row = this->glyphs.GetAt(j);
			index = 0;

			//3.2.2. 개행문자가 아닌동안 반복한다.
			while (buffer[index] != '\n') {

				isFirst = false;

				//3.2.3.1. 현재 문자가 1byte인지 2byte인지 구분한다.
				character_[0] = buffer[index];
				character_[1] = buffer[index + 1];


#if 0
				//3.2.3.2. 한글일 경우,  이거 왜안되는지 테스트해보자...........
				if (character_ >= "ㄱ" && character_ <= "힣") {
					character = new DoubleByteCharacter(character_);
					index = index + 2;
				}
#endif
				//3.2.3.2. 1byte인 경우,
				if(character_[0]>=0 && character_[0]<128){
					character_[0] = buffer[index];
					character = new SingleByteCharacter(character_[0]);
					index++;
				}
				//3.2.3.3. 2byte인 경우
				else {
					character = new DoubleByteCharacter(character_);
					index = index + 2;
				}
				//3.2.3.4. row에 Add한다.
				row->Add(character); //row->Add(character);
				row->MoveCurrent(row->GetLength());
			}

			if (isFirst == true && buffer[index] == '\n') {
				//3.3. 새로운 줄을 만들어 note에 Add한다.
				row = new Row();
				this->Add(row);
			}
			//3.3. 새로운 줄을 만들어 note에 Add한다.
			//row = new Row();
			//this->Add(row);


			this->current = this->length;
			//note->MoveCurrent(note->GetLength());
			j++;
			fgets(buffer, 768, file);
//#if 0
			//(21.11.17.추가) 현재 buffer의 맨 끝에 개행문자가 없으면 개행문자를 붙혀준다. *************
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

			//3.2.1. 노트의 현재 줄을 구한다.
			row = this->glyphs.GetAt(j);
			index = 0;

			//3.2.2. 개행문자가 아닌동안 반복한다.
			while (buffer[index] != '\n') {

				isFirst = false;

				//3.2.3.1. 현재 문자가 1byte인지 2byte인지 구분한다.
				character_[0] = buffer[index];
				character_[1] = buffer[index + 1];


#if 0
				//3.2.3.2. 한글일 경우,  이거 왜안되는지 테스트해보자...........
				if (character_ >= "ㄱ" && character_ <= "힣") {
					character = new DoubleByteCharacter(character_);
					index = index + 2;
				}
#endif
				//3.2.3.2. 1byte인 경우,
				if (character_[0] >= 0 && character_[0] < 128) {
					character_[0] = buffer[index];
					character = new SingleByteCharacter(character_[0]);
					index++;
				}
				//3.2.3.3. 2byte인 경우
				else {
					character = new DoubleByteCharacter(character_);
					index = index + 2;
				}
				//3.2.3.4. row에 Add한다.
				row->Add(character); //row->Add(character);
				row->MoveCurrent(row->GetLength());
			}

			this->current = this->length;	
		}
		//개행문자가 없는 한 문장 처리 위해 추가.

//#if 0
		else if (isOneLine == true) {
			//개행문자를 붙혀준다.
			bufferS = buffer;
			bufferS.push_back('\n');
			strcpy(buffer, bufferS.c_str());

			//3.2.1. 노트의 현재 줄을 구한다.
			row = this->glyphs.GetAt(j);
			index = 0;

			//3.2.2. 개행문자가 아닌동안 반복한다.
			while (buffer[index] != '\n') {
				//3.2.3.1. 현재 문자가 1byte인지 2byte인지 구분한다.
				character_[0] = buffer[index];
				character_[1] = buffer[index + 1];

				//3.2.3.2. 1byte인 경우,
				if (character_[0] >= 0 && character_[0] < 128) {
					character_[0] = buffer[index];
					character = new SingleByteCharacter(character_[0]);
					index++;
				}
				//3.2.3.3. 2byte인 경우
				else {
					character = new DoubleByteCharacter(character_);
					index = index + 2;
				}
				//3.2.3.4. row에 Add한다.
				row->Add(character); //row->Add(character);
				row->MoveCurrent(row->GetLength());
			}

			this->current = this->length;
		}
//#endif

		//3.4. 디스크파일을 닫는다.
		fclose(file);
	}
	return this->length;
}
#endif


void Composite::First() { //노트가 들어오든 줄이 들어오든 current를 0으로
	this->current = 0;
}

void Composite::Last() {//노트가 들어오든 줄이 들어오든 current를 length로
	this->current = this->length-1;
}

void Composite::Previous() {
	//1. Previous 호출했을 때
	  //1.1. 현재 줄을 찾는다.
	  Long currentY = this->current;
	  Glyph* row = this->glyphs.GetAt(this->current);

	  //1.2. 해당 줄의 current를 찾는다.
	  Long currentX = row->GetCurrent();

	  //1.3. current를 1씩 뺀다.
	  currentX--;

	  //1.4. current가 0보다 작은 경우,
	  if (currentX < 0) {
		  //1.4.1. 현재 y에서 1씩 뺀다.
		  currentY--;

		  //1.4.2. y가 0보다 크거나 같은 경우, 해당 y의 맨끝으로 x를 이동한다.
		  if (currentY >= 0) {
			  row = this->glyphs.GetAt(currentY);
			  currentX = row->GetLength();
		  }
		  //1.4.3. y가 0보다 작은 경우, y와 x를 0으로 정한다.
		  else {
			  currentY = 0;
			  currentX = 0;
		  }
	  }
	  //1.5. MoveCurrent한다.
	  this->current = currentY;

	  row = this->glyphs.GetAt(currentY);
	 
	  row->MoveCurrent(currentX);        
}

void Composite::Next() {

	//2. Next를 호출했을 때
	  //2.1. 현재 줄을 찾는다.
	  Long currentY = this->current;
	  Glyph* row = this->glyphs.GetAt(this->current);

	  //2.2. 해당 줄의 current를 찾는다.
	  Long currentX = row->GetCurrent();

	  //2.3. current를 1씩 더한다.
	  currentX++;

	  //2.4. current가 length보다 클 경우,
	  if (currentX > row->GetLength()) {
		  //2.4.1. 현재 y에서 1씩 더한다.
		  currentY++;
		  //2.4.2. y가 note의 length보다 작은 경우, 해당 y의 currentX를 0으로 이동한다.
		  if (currentY < this->length) {
			  currentX = 0;
		  }
		  //2.4.3. y가 noteLength보다 크거나 같은 경우, x와 y를 1씩 뺀다.
		  else {
			  currentX--;
			  currentY--;
		  }
	  }

	  //2.5. MoveCurrent한다.
	  this->current = currentY;
	  row = this->glyphs.GetAt(currentY);
	  row->MoveCurrent(currentX);
}

void Composite::PreviousWord() {
	Glyph* character;
	string strCharacter;
	Long length;

	//3. PreviousWord를 호출했을 때
	  //3.1. 현재 줄을 찾는다.
	  Long currentY = this->current;
	  Glyph* row = this->glyphs.GetAt(currentY);

	  //3.2. 해당 줄의 current를 찾는다.
	  Long currentX = row->GetCurrent();

	  //3.3. current가 0보다 큰 경우,
	  if (currentX > 0) {
		  //3.3.1. 커서 앞이 공백으로 시작하면, 
		  currentX--;
		  character = row->GetChild(currentX);
		  strCharacter = character->GetContent();
		  
		  if (strCharacter == " " || strCharacter == "\t") {
			  //1. x가 0보다 크고, 공백일 동안 반복한다.
			  while (currentX > 0 && (strCharacter == " " || strCharacter == "\t")) {
				  currentX--;
				  character = row->GetChild(currentX);
				  strCharacter = character->GetContent();
			  }

			  //2. x가 0보다 크면, 
				  //2.1. x가 0보다 크고, 공백이 아닌동안 반복한다.
			  while (currentX > 0 && strCharacter != " " && strCharacter != "\t") {
				  currentX--;
				  character = row->GetChild(currentX);
				  strCharacter = character->GetContent();
			  }
			  if (currentX > 0) {
			      currentX++;
			  }
			  
		  }
		  //3.3.2. 커서 앞이 공백으로 시작하지 않으면, X가 0보다 크고, 공백이 아닌동안 반복한다.
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
	  //3.4. current가 0보다 작은 경우,
	  else {
		  //3.4.1. 현재 y에서 1씩 뺀다.
		  currentY--;
		  //3.4.2. y가 0보다 크거나 같은 경우,  해당 y좌표의 맨끝으로 current 이동한다.
		  if (currentY >= 0) {
			  row = this->GetChild(currentY);
			  length = row->GetLength();
			  currentX = length;
		  }
		  //3.4.3. y가 0보다 작은 경우, x,y좌표를 0,0으로 정한다.
		  else {
			  currentX = 0;
			  currentY = 0;
		  }
	  }
	  //3.5. MoveCurrent한다.
	  this->current = currentY;
	  row = this->glyphs.GetAt(currentY);
      row->MoveCurrent(currentX);
}

void Composite::NextWord() {
	Glyph* character;
	string strCharacter;
	Long length;

	//4. NextWord를 호출하였을 때
	  //4.1. 현재 줄을 찾는다.
	  Long currentY = this->current;
	  Glyph* row = this->glyphs.GetAt(currentY);
	  length = row->GetLength();

	  //4.2. 해당 줄의 current를 찾는다.
	  Long currentX = row->GetCurrent();

	  //4.3. current가 row의 length보다 작은 경우,
	  if (currentX < length) {
		  //4.3.1. 커서 앞에 문자로 시작하면,
		  character = row->GetChild(currentX);
		  strCharacter = character->GetContent();
		  if (strCharacter != " " && strCharacter != "\t") {
			  //1. x가 length보다 작고, 문자일 동안 반복한다.
			  while (currentX < length -1 && strCharacter != " " && strCharacter != "\t") {
				  currentX++;
				  character = row->GetChild(currentX);
				  strCharacter = character->GetContent();
			  }
			  if (currentX <= length - 1 && strCharacter != " " && strCharacter != "\t") {
				  currentX++;
			  }

			  //2. x가 length보다 작으면, length보다 작고 공백일 동안 반복한다.
			  while (currentX < length - 1 && (strCharacter==" " || strCharacter == "\t")) {
				  currentX++;
				  character = row->GetChild(currentX);
				  strCharacter = character->GetContent();
			  }
			  if (currentX <= length - 1 && (strCharacter == " " || strCharacter == "\t")) {
				  currentX++;
			  }
			 
		  }
		  //4.3.2. 커서 앞이 공백으로 시작하면, x가 length보다 작고 공백일 동안 반복한다.
		  else {
			  while (currentX < length && (strCharacter == " " || strCharacter == "\t")) {
				  currentX++;
				  character = row->GetChild(currentX);
				  strCharacter = character->GetContent();
			  }
		  }
	  }
	  //4.4. current가 row의 length보다 크거나 같은 경우,
	  else {
		  //4.4.1. 현재 y에서 1씩 더한다.
		  currentY++;
		  //4.4.2. note의 length보다 y가 작은 경우, 해당 y좌표의 첫번째로 x를 이동한다.
		  if (currentY < this->length) {
			  currentX = 0;
		  }
		  //4.4.3. y가 note의 length와 같거나 더 큰 경우, 따로 이동하지 않는다.
		  else {
			  currentY--;
		  }
	  }
	  //4.5. MoveCurrent 한다.
	  this->current = currentY;
	  row = this->glyphs.GetAt(currentY);
	  row->MoveCurrent(currentX);


}


#if 0
void Composite::Up() {
	//1. 현재 줄의 currentX까지 byte 수 계산한다.
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
		//1. byteUp 계산한다. (밑에서 이용할 byte수 계산하기 위해 반복하는 것.)
		while (i < currentX) { 
			character = row->GetChild(i);
			strCharacter = character->GetContent();
			strcpy(chaCharacter, strCharacter.c_str());

			//1.1. 1byte면 1씩 더한다.
			if (chaCharacter[0] >= 0 && chaCharacter[0] < 128) {
				byte++;
			}
			//1.2. 2byte면 2씩 더한다.
			else {
				byte = byte + 2;
			}
#if 0
			//1.1. 2byte면 2씩 더한다.
			if (strCharacter >= "ㄱ" && strCharacter <= "힣") {
				byte = byte + 2;
			}
			//1.2. 1byte면 1씩 더한다.
			else {
				byte++;
			}
#endif
			i++;
		}
		//2. y-1 좌표에서 현재 좌표의 byte 수보다 작거나 같을 동안 byte수 센다.
		i = 0;
		row = this->glyphs.GetAt(currentY - 1);
		while (byteUp <= byte && i < row->GetLength()) {
			character = row->GetChild(i);
			strCharacter = character->GetContent();
			strcpy(chaCharacter, strCharacter.c_str());

			//1.1. 1byte면 1씩 더한다.
			if (chaCharacter[0] >= 0 && chaCharacter[0] < 128) {
				byteUp++;
			}
			//1.2. 2byte면 2씩 더한다.
			else {
				byteUp = byteUp + 2;
		    }
#if 0 
			//2.1. 2byte면 2씩 더한다.
			if (strCharacter >= "ㄱ" && strCharacter <= "힣") {
				byteUp = byteUp + 2;
			}
			//2.2. 1byte면 1씩 더한다.
			else {
				byteUp++;
			}
#endif
			i++;
		}
		if (byteUp > byte) {
			i--;
		}


		//3. y-1좌표로 이동하고 해당 x좌표로 이동한다.
		this->current = currentY - 1;
		row = this->glyphs.GetAt(currentY - 1);
		row->MoveCurrent(i); //i가 currentX임.
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

	//1. currentY+1이 length보다 작은 경우,
	if (currentY + 1 < this->length) {
		//1.1. 현재 좌표까지의 byte수 계산한다.
		while (i < currentX) {
			character = row->GetChild(i);
			strCharacter = character->GetContent();
			strcpy(chaCharacter, strCharacter.c_str());

			//1.1.1. 1byte면 1씩 더한다.
			if (chaCharacter[0] >= 0 && chaCharacter[0] < 128) {
				byte++;
			}
			//1.1.2. 2byte면 2씩 더한다.
			else {
				byte = byte + 2;
			}
#if 0
			//1.1.1. 2byte면 2씩 더한다.
			if (strCharacter >= "ㄱ" && strCharacter <= "힣") {
				byte = byte + 2;
			}
			//1.1.2. 1byte면 1씩 더한다.
			else {
				byte++;
			}
#endif
			i++;
		}
		//1.2. y+1 좌표에서 현재 좌표의 byte 수보다 작거나 같을 동안 byte 수를 센다.
		i = 0;
		row = this->glyphs.GetAt(currentY + 1);
		while (byteDown <= byte && i < row->GetLength()) {
			character = row->GetChild(i);
			strCharacter = character->GetContent();

			strcpy(chaCharacter, strCharacter.c_str());

			//1.1.1. 1byte면 1씩 더한다.
			if (chaCharacter[0] >= 0 && chaCharacter[0] < 128) {
				byteDown++;
			}
			//1.1.2. 2byte면 2씩 더한다.
			else {
				byteDown = byteDown + 2;
		}

#if 0
			//1.2.1. 2byte면 2씩 더한다
			if (strCharacter >= "ㄱ" && strCharacter <= "힣") {
				byteDown = byteDown + 2;
			}
			//1.2.2. 1byte면 1씩 더한다.
			else {
				byteDown++;
			}
#endif
			i++;
		}
		if (byteDown > byte) {
			i--;
		}
		//1.3. y+1 좌표로 이동하고, 해당 x좌표로 이동한다.
		this->current = currentY + 1;
		row = this->glyphs.GetAt(currentY + 1);
		row->MoveCurrent(i); //i가 currentX임.
	}
}
#endif

Glyph* Composite::Split(Glyph* dummyRow, int location) { //index: 어디다가 insert할지
	int dummyRowIndex;
	Glyph* character;


	//1. insert할 dummyRow, 자를 위치를 입력받는다.
	//2. length만큼 반복한다.
	while (location < this->length) {
		//2.1. 위치 이후의 글자를 dummyRow에 Add한다.
		character = this->glyphs.GetAt(location);
		dummyRow->Add(character);

		//4.2. Add한 글자를 기존 줄에서 지운다.
		this->glyphs.Delete(location);
		this->capacity--;
		this->length--;
	}

	//5. dummyRow를 출력한다.
	return dummyRow;
}


void Composite::Combine(Glyph* dummyRow) {
	int i = 0;
	Glyph* character;

	//1.Combine호출할 때
	  //1.1. 줄을 입력받는다.
	  //1.2. 입력받은 줄의 length만큼 반복한다.
	  while (i < dummyRow->GetLength()) {
		  //1.2.1. 글자를 읽는다.
		  character = dummyRow->GetChild(i);

		  //1.2.2. 현재 줄에 추가한다.
		  this->Add(character);

		  //1.2.3. 입력받은 줄에서 삭제한다.
		  dummyRow->Remove(i);
	  }
	  //1.3. dummyRow에서 다 옮겼으면 줄에서 삭제한다.(인터페이스에서)
}

Glyph* Composite::SplitForFront(Glyph* dummyRow, int location) {
	Glyph* character;
	int i = 0;

	//1. length만큼 반복한다.
	while (location < this->GetLength()) {
		//1.1. 위치 이후의 글자를 dummyRow에 Add한다. (제일 앞에)
		character = this->glyphs.GetAt(location);
		dummyRow->Add(i, character);

		//1.2. Add한 글자를 기존 줄에서 지운다.
		this->glyphs.Delete(location);
		this->capacity--;
		this->length--;
		i++;
	}

	//2. dummyRow를 출력한다.
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

	//1. 찾을 문자, 대소문자 구분여부, 자동개행 전 캐럿의 좌표를 입력받는다.

	//(21.09.17 추가) findStr에 개행문자가 포함되어 있는지 확인한다.
	size_t nPos = findStr.find("\n");
	if (nPos != string::npos) { //개행문자가 있는 경우,
		isThereLineChange = true;
	}

	
	//2. 찾을 문자의 총 개수를 구한다.
	//int findLength = findStr.size();
	
	const unsigned char* constStr = reinterpret_cast<const unsigned char*> (findStr.c_str());
	int findLength = _mbslen(constStr);
	

	currentY = firstCurrentY;
	noteLength = this->length;

	//3. 개행 문자가 포함되어 있지 않은 경우, 문서의 끝이 아닌동안 반복한다.
	while (isThereLineChange == false && currentY < noteLength && isThereSameText == false) {

		//3.1. 현재 캐럿이 있는 줄을 찾는다.
		row = this->glyphs.GetAt(currentY);
		rowLength = row->GetLength();

		//3.2. i를 초기화하고 restLength를 정한다.
		if (currentY == firstCurrentY) {
			i = beforeLineChangeCurrentX;
			restLength = rowLength - i;
		}
		else {
			i = 0;
			restLength = rowLength;
		}

		//3.3. restLength가 findLength보다 크거나 같고, 같은 단어가 없는 동안 반복한다.
		while (restLength >= findLength && isThereSameText == false) {
			
			//3.3.1. 해당 줄에서 찾을 문자의 총 개수만큼 문자를 만든다.
			temp.clear(); //문자열 지우기
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

			//3.3.2. 대소문자 구분하면,
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
			//3.3.3. 대소문자 구분하지 않으면,
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

	//4. 개행 문자가 포함되어 있는 경우, 문서의 끝이 아닌동안 반복한다.
	while (isThereLineChange == true && currentY < noteLength && isThereSameText == false) {
		
		//4.1. temp 초기화한다.
		temp.clear();
		tempCurrentY = currentY;
		j = i;
		k = 0;

		//4.2. findLength만큼 비교할 문자를 만든다.
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

		//4.3. 대소문자 구분하면,
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

		//4.4. 대소문자 구분하지 않으면,
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

	//5. 같은 단어가 없으면 firstXY, endXY에 0을 입력한다.
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

	//1. 문서의 처음이 아닌동안 반복한다.
	//int findLength = findStr.size();

	//(21.09.17 추가) findStr에 개행문자가 포함되어 있는지 확인한다.
	size_t nPos = findStr.find("\n");
	if (nPos != string::npos) { //개행문자가 있는 경우,
		isThereLineChange = true;
	}
	
	//i를 초기화한다.
	int currentY = firstCurrentY;
	i = beforeLineChangeCurrentX - 1;

	while (i < 0 && currentY>=0) {
		currentY--;
		if (currentY >= 0) {
			row = this->glyphs.GetAt(currentY);
			i = row->GetLength() - 1;
		}
	}

	//2. 찾을 문자의 총 개수를 구한다.
	const unsigned char* constStr = reinterpret_cast<const unsigned char*> (findStr.c_str());
	int findLength = _mbslen(constStr);

	

	//3. 개행 문자가 포함되어 있지 않은 경우, 문서의 처음이 아닌동안 반복한다.
	while (isThereLineChange == false && currentY >= 0 && isThereSameText == false) {
		//1.1. 현재 캐럿이 있는 줄을 찾는다.
		row = this->glyphs.GetAt(currentY);
		rowLength = row->GetLength();

		//1.2. i를 초기화하고 restLength를 정한다.
		if (currentY == firstCurrentY) {
			i = beforeLineChangeCurrentX - 1;
			restLength = i + 1;
		}
		else {
			i = rowLength - 1;
			restLength = rowLength;
		}

		//1.3. restLength가 findLength보다 크거나 같고, 같은 단어가 없는동안 반복한다.
		while (restLength >= findLength && isThereSameText == false) {
			
			//1.3.1. 해당 줄에서 현재 currentX에서 찾은 문자의 총 개수만큼 문자를 만든다.
			temp.clear(); //문자열 지우기
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

			//1.3.2. 대소문자 구분하면
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
			//1.3.3. 대소문자 구분 안하면
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

	//4. 개행 문자가 포함되어 있는 경우, 문서의 처음이 아닌동안 반복한다.
	while (isThereLineChange == true && currentY >= 0 && isThereSameText == false) {

		//4.1. temp 초기화한다.
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

		//4.2. 대소문자 구분하면
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
		//4.3. 대소문자 구분 안하면
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


	//5. 같은 단어가 없으면, firstXY, endXY에 0을 입력한다.
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

	//예외초기화
	(*exception) = 0;

	//0. 찾을 내용, 바꿀 내용의 총 개수를 구한다.
	const unsigned char* constStr = reinterpret_cast<const unsigned char*> (findStr.c_str());
	int findLength = _mbslen(constStr);

	//constStr = reinterpret_cast<const unsigned char*> (const_cast<const char*>(replaceStr.c_str()));
	constStr = reinterpret_cast<const unsigned char*> (replaceStr.c_str());
	int replaceLength = _mbslen(constStr);


	//1. 찾을 내용, 바꿀 내용, 현재 선택된 내용, 대소문자 구분, 자동개행 전 캐럿의 좌표를 입력받는다.
	//2. 현재 선택된 단어가 찾고자 하는 단어와 같은지 확인한다.
	  //2.1. 대소문자 구분하면,
	if (matchCase == true && strcmp(cFindStr, cSelectedStr) == 0) {
		isSameText = true;
	}

	//2.2. 대소문자 구분 안하면,
	else if (matchCase == false && stricmp(cFindStr, cSelectedStr) == 0) {
		isSameText = true;
	}

	//2.3. 선택된 단어와 찾고자 하는 단어가 같으면, (바꾸려는 단어가 아닌)
	if (isSameText == true) {
		//2.3.0. string형을 char형으로 변환한다.
		//chaCharacter = new char(replaceStr.length());
		chaCharacter = new char[replaceStr.length() +1];
		strcpy(chaCharacter, cReplaceStr);


		//2.3.1. 선택된 단어를 지운다.
		selectedFirstX = beforeLineChangeCurrentX - findLength;
		row = this->glyphs.GetAt(firstCurrentY);


		while (i <= findLength) {
			row->Remove(selectedFirstX);
			i++;
		}

		//2.3.2. 바꿀 내용을 적는다.
		creator = new CreateProduct();
		i = 0;
		j = selectedFirstX;
		while (i < replaceStr.length()) {
			afterCharacter[0] = chaCharacter[i];
			afterCharacter[1] = chaCharacter[i + 1];

			//1. 1byte인 경우,
			if (afterCharacter[0] >= 0 && afterCharacter[0] < 128) {
				afterCharacter[1] = '\0';
				character = creator->Make(afterCharacter);
				i++;
			}
			//2. 2byte인 경우,
			else {
				character = creator->Make(afterCharacter);
				i = i + 2;
			}
			//3. 내용을 적는다.
			row->Add(j, character);
			j++;
		}

		//2.3.3. 캐럿의 좌표를 새로 적은 내용의 맨 뒤로 정한다.
		beforeLineChangeCurrentX = selectedFirstX + replaceLength;
	}
	//FindSameTextWithDown과 같음 ----------------------------------------------------------------------------------
	//3. 문서의 끝이 아닌동안 반복한다.

	//(21.08.30 추가) 주위에 배치가 눌려져있는 경우,
	if (searchAround == true) {
		currentY = 0;
	}
	else {
		currentY = firstCurrentY;
	}
	noteLength = this->length;


	//3. 문서의 끝이 아닌동안 반복한다.
	while (currentY < noteLength && isThereSameText == false) {

		//3.1. 현재 캐럿이 있는 줄을 찾는다.
		row = this->glyphs.GetAt(currentY);
		rowLength = row->GetLength();

		//3.2. i를 초기화하고 restLength를 정한다.
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

		//3.3. restLength가 findLength보다 크거나 같고, 같은 단어가 없는 동안 반복한다.
		while (restLength >= findLength && isThereSameText == false) {

			//3.3.1. 해당 줄에서 찾을 문자의 총 개수만큼 문자를 만든다.
			temp.clear(); //문자열 지우기
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

			//3.3.2. 대소문자 구분하면,
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
			//3.3.3. 대소문자 구분하지 않으면,
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
	//4. 그 다음 같은 단어는 없지만, 처음 들어왔을 때 replace한 경우,
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

		//4.1. 그 다음 같은 단어도 없고, 처음 들어왔을 때 replace하지 않은 경우, firstXY, endXY에 0을 입력한다.
		else if(isSameText == false && searchAround == false){
			(*firstY) = 0;
			(*firstX) = 0;
			(*endY) = 0;
			(*endX) = 0;
		}
	}
	//------------------------------------------------------------------------------------------------------

//#if 0
	//할당해제 해준다.
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



	//0. 찾을 내용, 바꿀 내용의 총 개수를 구한다.
	const unsigned char* constStr = reinterpret_cast<const unsigned char*> (findStr.c_str());
	int findLength = _mbslen(constStr);

	constStr = reinterpret_cast<const unsigned char*> (replaceStr.c_str());
	int replaceLength = _mbslen(constStr);




}
#endif






