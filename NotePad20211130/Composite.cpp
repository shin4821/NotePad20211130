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
#include<cstring> //21.08.25. Ã£±â À§ÇØ Ãß°¡ (´ë¼Ò¹®ÀÚ ±¸ºĞÇÔ¼ö)
#include"CreateGlyph.h" // 21.07.14 Load À§ÇØ Ãß°¡
#include"CreateProduct.h" // 21.07.14 Load À§ÇØ Ãß°¡
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

Composite& Composite::operator=(const Composite& source) { //Ä¡È¯¿¬»êÀÚ
	return *this;
}

Composite::Composite(const Composite& source) { //º¹»ç»ı¼ºÀÚ

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


	//1. ÅØ½ºÆ®ÆÄÀÏÀ» wt·Î ¿¬´Ù.
	file = fopen(fileName.c_str(), "wt"); //"NotePad.txt" 
	//2. ÆÄÀÏÀÌ Á¦´ë·Î ¿­·ÈÀ¸¸é,
	if (file != NULL) {

		//2.1. noteÀÇ length¸¸Å­ ¹İº¹ÇÑ´Ù.
		while (i < this->length) {
			//2.1.1. ÇØ´ç rowÀÇ length¸¸Å­ ¹İº¹ÇÑ´Ù.
			row = this->glyphs.GetAt(i);
			//rowLength = row->GetLength();
			character_ = row->GetContent();

			//fwrite(&character_, sizeof(string), 1, file); //sizeof(string), 1

			fputs(character_.c_str(), file);

			//2.1.2. ÅØ½ºÆ®ÆÄÀÏ¿¡ °³Çà¹®ÀÚ¸¦ writeÇÑ´Ù.
			//fwrite("\r\n", sizeof(char), 1, file);

			//(21.11.17.Ãß°¡) ¸Ç ¸¶Áö¸· ÁÙÀÏ °æ¿ì, Ãß°¡ÇÏÁö ¾Ê´Â´Ù.
			if (i + 1 < this->length) {
				fputs("\n", file);
			}


			i++;
		}
		//2.2. ÆÄÀÏÀ» ´İ´Â´Ù.
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

	//1. ÆÄÀÏ¸í, note¸¦ ÀÔ·Â¹Ş´Â´Ù.
	//2. ÇØ´ç ÆÄÀÏÀ» ÅØ½ºÆ®¸ğµå rt·Î ¿¬´Ù.
	file = fopen(fileName.c_str(), "rt");
	//3. ÆÄÀÏÀÌ Á¦´ë·Î ¿­·ÈÀ¸¸é,
	if (file != NULL) {
		//3.1. µğ½ºÅ© ÆÄÀÏ·ÎºÎÅÍ ÇÑ ÁÙ¾¿ ÀÔ·Â¹Ş´Â´Ù.
		fgets(buffer, 768, file); 
		//3.2. ÆÄÀÏÀÇ ³¡ÀÌ ¾Æ´Ñµ¿¾È ¹İº¹ÇÑ´Ù.
		while (!feof(file)) { 

			if (isFirst == false) {
				//3.3. »õ·Î¿î ÁÙÀ» ¸¸µé¾î note¿¡ AddÇÑ´Ù.
				row = new Row();
				this->Add(row);
			}

			//3.2.1. ³ëÆ®ÀÇ ÇöÀç ÁÙÀ» ±¸ÇÑ´Ù.
			row = this->glyphs.GetAt(j);
			index = 0;

			//3.2.2. °³Çà¹®ÀÚ°¡ ¾Æ´Ñµ¿¾È ¹İº¹ÇÑ´Ù.
			while (buffer[index] != '\n') {

				isFirst = false;

				//3.2.3.1. ÇöÀç ¹®ÀÚ°¡ 1byteÀÎÁö 2byteÀÎÁö ±¸ºĞÇÑ´Ù.
				character_[0] = buffer[index];
				character_[1] = buffer[index + 1];


#if 0
				//3.2.3.2. ÇÑ±ÛÀÏ °æ¿ì,  ÀÌ°Å ¿Ö¾ÈµÇ´ÂÁö Å×½ºÆ®ÇØº¸ÀÚ...........
				if (character_ >= "¤¡" && character_ <= "ÆR") {
					character = new DoubleByteCharacter(character_);
					index = index + 2;
				}
#endif
				//3.2.3.2. 1byteÀÎ °æ¿ì,
				if(character_[0]>=0 && character_[0]<128){
					character_[0] = buffer[index];
					character = new SingleByteCharacter(character_[0]);
					index++;
				}
				//3.2.3.3. 2byteÀÎ °æ¿ì
				else {
					character = new DoubleByteCharacter(character_);
					index = index + 2;
				}
				//3.2.3.4. row¿¡ AddÇÑ´Ù.
				row->Add(character); //row->Add(character);
				row->MoveCurrent(row->GetLength());
			}

			if (isFirst == true && buffer[index] == '\n') {
				//3.3. »õ·Î¿î ÁÙÀ» ¸¸µé¾î note¿¡ AddÇÑ´Ù.
				row = new Row();
				this->Add(row);
			}
			//3.3. »õ·Î¿î ÁÙÀ» ¸¸µé¾î note¿¡ AddÇÑ´Ù.
			//row = new Row();
			//this->Add(row);


			this->current = this->length;
			//note->MoveCurrent(note->GetLength());
			j++;
			fgets(buffer, 768, file);
//#if 0
			//(21.11.17.Ãß°¡) ÇöÀç bufferÀÇ ¸Ç ³¡¿¡ °³Çà¹®ÀÚ°¡ ¾øÀ¸¸é °³Çà¹®ÀÚ¸¦ ºÙÇôÁØ´Ù. *************
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

			//3.2.1. ³ëÆ®ÀÇ ÇöÀç ÁÙÀ» ±¸ÇÑ´Ù.
			row = this->glyphs.GetAt(j);
			index = 0;

			//3.2.2. °³Çà¹®ÀÚ°¡ ¾Æ´Ñµ¿¾È ¹İº¹ÇÑ´Ù.
			while (buffer[index] != '\n') {

				isFirst = false;

				//3.2.3.1. ÇöÀç ¹®ÀÚ°¡ 1byteÀÎÁö 2byteÀÎÁö ±¸ºĞÇÑ´Ù.
				character_[0] = buffer[index];
				character_[1] = buffer[index + 1];


#if 0
				//3.2.3.2. ÇÑ±ÛÀÏ °æ¿ì,  ÀÌ°Å ¿Ö¾ÈµÇ´ÂÁö Å×½ºÆ®ÇØº¸ÀÚ...........
				if (character_ >= "¤¡" && character_ <= "ÆR") {
					character = new DoubleByteCharacter(character_);
					index = index + 2;
				}
#endif
				//3.2.3.2. 1byteÀÎ °æ¿ì,
				if (character_[0] >= 0 && character_[0] < 128) {
					character_[0] = buffer[index];
					character = new SingleByteCharacter(character_[0]);
					index++;
				}
				//3.2.3.3. 2byteÀÎ °æ¿ì
				else {
					character = new DoubleByteCharacter(character_);
					index = index + 2;
				}
				//3.2.3.4. row¿¡ AddÇÑ´Ù.
				row->Add(character); //row->Add(character);
				row->MoveCurrent(row->GetLength());
			}

			this->current = this->length;	
		}
		//°³Çà¹®ÀÚ°¡ ¾ø´Â ÇÑ ¹®Àå Ã³¸® À§ÇØ Ãß°¡.

//#if 0
		else if (isOneLine == true) {
			//°³Çà¹®ÀÚ¸¦ ºÙÇôÁØ´Ù.
			bufferS = buffer;
			bufferS.push_back('\n');
			strcpy(buffer, bufferS.c_str());

			//3.2.1. ³ëÆ®ÀÇ ÇöÀç ÁÙÀ» ±¸ÇÑ´Ù.
			row = this->glyphs.GetAt(j);
			index = 0;

			//3.2.2. °³Çà¹®ÀÚ°¡ ¾Æ´Ñµ¿¾È ¹İº¹ÇÑ´Ù.
			while (buffer[index] != '\n') {
				//3.2.3.1. ÇöÀç ¹®ÀÚ°¡ 1byteÀÎÁö 2byteÀÎÁö ±¸ºĞÇÑ´Ù.
				character_[0] = buffer[index];
				character_[1] = buffer[index + 1];

				//3.2.3.2. 1byteÀÎ °æ¿ì,
				if (character_[0] >= 0 && character_[0] < 128) {
					character_[0] = buffer[index];
					character = new SingleByteCharacter(character_[0]);
					index++;
				}
				//3.2.3.3. 2byteÀÎ °æ¿ì
				else {
					character = new DoubleByteCharacter(character_);
					index = index + 2;
				}
				//3.2.3.4. row¿¡ AddÇÑ´Ù.
				row->Add(character); //row->Add(character);
				row->MoveCurrent(row->GetLength());
			}

			this->current = this->length;
		}
//#endif

		//3.4. µğ½ºÅ©ÆÄÀÏÀ» ´İ´Â´Ù.
		fclose(file);
	}
	return this->length;
}

void Composite::First() { //³ëÆ®°¡ µé¾î¿Àµç ÁÙÀÌ µé¾î¿Àµç current¸¦ 0À¸·Î
	this->current = 0;
}

void Composite::Last() {//³ëÆ®°¡ µé¾î¿Àµç ÁÙÀÌ µé¾î¿Àµç current¸¦ length·Î
	this->current = this->length-1;
}

void Composite::Previous() {
	//1. Previous È£ÃâÇßÀ» ¶§
	  //1.1. ÇöÀç ÁÙÀ» Ã£´Â´Ù.
	  Long currentY = this->current;
	  Glyph* row = this->glyphs.GetAt(this->current);

	  //1.2. ÇØ´ç ÁÙÀÇ current¸¦ Ã£´Â´Ù.
	  Long currentX = row->GetCurrent();

	  //1.3. current¸¦ 1¾¿ »«´Ù.
	  currentX--;

	  //1.4. current°¡ 0º¸´Ù ÀÛÀº °æ¿ì,
	  if (currentX < 0) {
		  //1.4.1. ÇöÀç y¿¡¼­ 1¾¿ »«´Ù.
		  currentY--;

		  //1.4.2. y°¡ 0º¸´Ù Å©°Å³ª °°Àº °æ¿ì, ÇØ´ç yÀÇ ¸Ç³¡À¸·Î x¸¦ ÀÌµ¿ÇÑ´Ù.
		  if (currentY >= 0) {
			  row = this->glyphs.GetAt(currentY);
			  currentX = row->GetLength();
		  }
		  //1.4.3. y°¡ 0º¸´Ù ÀÛÀº °æ¿ì, y¿Í x¸¦ 0À¸·Î Á¤ÇÑ´Ù.
		  else {
			  currentY = 0;
			  currentX = 0;
		  }
	  }
	  //1.5. MoveCurrentÇÑ´Ù.
	  this->current = currentY;

	  row = this->glyphs.GetAt(currentY);
	 
	  row->MoveCurrent(currentX);        
}

void Composite::Next() {

	//2. Next¸¦ È£ÃâÇßÀ» ¶§
	  //2.1. ÇöÀç ÁÙÀ» Ã£´Â´Ù.
	  Long currentY = this->current;
	  Glyph* row = this->glyphs.GetAt(this->current);

	  //2.2. ÇØ´ç ÁÙÀÇ current¸¦ Ã£´Â´Ù.
	  Long currentX = row->GetCurrent();

	  //2.3. current¸¦ 1¾¿ ´õÇÑ´Ù.
	  currentX++;

	  //2.4. current°¡ lengthº¸´Ù Å¬ °æ¿ì,
	  if (currentX > row->GetLength()) {
		  //2.4.1. ÇöÀç y¿¡¼­ 1¾¿ ´õÇÑ´Ù.
		  currentY++;
		  //2.4.2. y°¡ noteÀÇ lengthº¸´Ù ÀÛÀº °æ¿ì, ÇØ´ç yÀÇ currentX¸¦ 0À¸·Î ÀÌµ¿ÇÑ´Ù.
		  if (currentY < this->length) {
			  currentX = 0;
		  }
		  //2.4.3. y°¡ noteLengthº¸´Ù Å©°Å³ª °°Àº °æ¿ì, x¿Í y¸¦ 1¾¿ »«´Ù.
		  else {
			  currentX--;
			  currentY--;
		  }
	  }

	  //2.5. MoveCurrentÇÑ´Ù.
	  this->current = currentY;
	  row = this->glyphs.GetAt(currentY);
	  row->MoveCurrent(currentX);
}

void Composite::PreviousWord() {
	Glyph* character;
	string strCharacter;
	Long length;

	//3. PreviousWord¸¦ È£ÃâÇßÀ» ¶§
	  //3.1. ÇöÀç ÁÙÀ» Ã£´Â´Ù.
	  Long currentY = this->current;
	  Glyph* row = this->glyphs.GetAt(currentY);

	  //3.2. ÇØ´ç ÁÙÀÇ current¸¦ Ã£´Â´Ù.
	  Long currentX = row->GetCurrent();

	  //3.3. current°¡ 0º¸´Ù Å« °æ¿ì,
	  if (currentX > 0) {
		  //3.3.1. Ä¿¼­ ¾ÕÀÌ °ø¹éÀ¸·Î ½ÃÀÛÇÏ¸é, 
		  currentX--;
		  character = row->GetChild(currentX);
		  strCharacter = character->GetContent();
		  
		  if (strCharacter == " " || strCharacter == "\t") {
			  //1. x°¡ 0º¸´Ù Å©°í, °ø¹éÀÏ µ¿¾È ¹İº¹ÇÑ´Ù.
			  while (currentX > 0 && (strCharacter == " " || strCharacter == "\t")) {
				  currentX--;
				  character = row->GetChild(currentX);
				  strCharacter = character->GetContent();
			  }

			  //2. x°¡ 0º¸´Ù Å©¸é, 
				  //2.1. x°¡ 0º¸´Ù Å©°í, °ø¹éÀÌ ¾Æ´Ñµ¿¾È ¹İº¹ÇÑ´Ù.
			  while (currentX > 0 && strCharacter != " " && strCharacter != "\t") {
				  currentX--;
				  character = row->GetChild(currentX);
				  strCharacter = character->GetContent();
			  }
			  if (currentX > 0) {
			      currentX++;
			  }
			  
		  }
		  //3.3.2. Ä¿¼­ ¾ÕÀÌ °ø¹éÀ¸·Î ½ÃÀÛÇÏÁö ¾ÊÀ¸¸é, X°¡ 0º¸´Ù Å©°í, °ø¹éÀÌ ¾Æ´Ñµ¿¾È ¹İº¹ÇÑ´Ù.
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
	  //3.4. current°¡ 0º¸´Ù ÀÛÀº °æ¿ì,
	  else {
		  //3.4.1. ÇöÀç y¿¡¼­ 1¾¿ »«´Ù.
		  currentY--;
		  //3.4.2. y°¡ 0º¸´Ù Å©°Å³ª °°Àº °æ¿ì,  ÇØ´ç yÁÂÇ¥ÀÇ ¸Ç³¡À¸·Î current ÀÌµ¿ÇÑ´Ù.
		  if (currentY >= 0) {
			  row = this->GetChild(currentY);
			  length = row->GetLength();
			  currentX = length;
		  }
		  //3.4.3. y°¡ 0º¸´Ù ÀÛÀº °æ¿ì, x,yÁÂÇ¥¸¦ 0,0À¸·Î Á¤ÇÑ´Ù.
		  else {
			  currentX = 0;
			  currentY = 0;
		  }
	  }
	  //3.5. MoveCurrentÇÑ´Ù.
	  this->current = currentY;
	  row = this->glyphs.GetAt(currentY);
      row->MoveCurrent(currentX);
}

void Composite::NextWord() {
	Glyph* character;
	string strCharacter;
	Long length;

	//4. NextWord¸¦ È£ÃâÇÏ¿´À» ¶§
	  //4.1. ÇöÀç ÁÙÀ» Ã£´Â´Ù.
	  Long currentY = this->current;
	  Glyph* row = this->glyphs.GetAt(currentY);
	  length = row->GetLength();

	  //4.2. ÇØ´ç ÁÙÀÇ current¸¦ Ã£´Â´Ù.
	  Long currentX = row->GetCurrent();

	  //4.3. current°¡ rowÀÇ lengthº¸´Ù ÀÛÀº °æ¿ì,
	  if (currentX < length) {
		  //4.3.1. Ä¿¼­ ¾Õ¿¡ ¹®ÀÚ·Î ½ÃÀÛÇÏ¸é,
		  character = row->GetChild(currentX);
		  strCharacter = character->GetContent();
		  if (strCharacter != " " && strCharacter != "\t") {
			  //1. x°¡ lengthº¸´Ù ÀÛ°í, ¹®ÀÚÀÏ µ¿¾È ¹İº¹ÇÑ´Ù.
			  while (currentX < length -1 && strCharacter != " " && strCharacter != "\t") {
				  currentX++;
				  character = row->GetChild(currentX);
				  strCharacter = character->GetContent();
			  }
			  if (currentX <= length - 1 && strCharacter != " " && strCharacter != "\t") {
				  currentX++;
			  }

			  //2. x°¡ lengthº¸´Ù ÀÛÀ¸¸é, lengthº¸´Ù ÀÛ°í °ø¹éÀÏ µ¿¾È ¹İº¹ÇÑ´Ù.
			  while (currentX < length - 1 && (strCharacter==" " || strCharacter == "\t")) {
				  currentX++;
				  character = row->GetChild(currentX);
				  strCharacter = character->GetContent();
			  }
			  if (currentX <= length - 1 && (strCharacter == " " || strCharacter == "\t")) {
				  currentX++;
			  }
			 
		  }
		  //4.3.2. Ä¿¼­ ¾ÕÀÌ °ø¹éÀ¸·Î ½ÃÀÛÇÏ¸é, x°¡ lengthº¸´Ù ÀÛ°í °ø¹éÀÏ µ¿¾È ¹İº¹ÇÑ´Ù.
		  else {
			  while (currentX < length && (strCharacter == " " || strCharacter == "\t")) {
				  currentX++;
				  character = row->GetChild(currentX);
				  strCharacter = character->GetContent();
			  }
		  }
	  }
	  //4.4. current°¡ rowÀÇ lengthº¸´Ù Å©°Å³ª °°Àº °æ¿ì,
	  else {
		  //4.4.1. ÇöÀç y¿¡¼­ 1¾¿ ´õÇÑ´Ù.
		  currentY++;
		  //4.4.2. noteÀÇ lengthº¸´Ù y°¡ ÀÛÀº °æ¿ì, ÇØ´ç yÁÂÇ¥ÀÇ Ã¹¹øÂ°·Î x¸¦ ÀÌµ¿ÇÑ´Ù.
		  if (currentY < this->length) {
			  currentX = 0;
		  }
		  //4.4.3. y°¡ noteÀÇ length¿Í °°°Å³ª ´õ Å« °æ¿ì, µû·Î ÀÌµ¿ÇÏÁö ¾Ê´Â´Ù.
		  else {
			  currentY--;
		  }
	  }
	  //4.5. MoveCurrent ÇÑ´Ù.
	  this->current = currentY;
	  row = this->glyphs.GetAt(currentY);
	  row->MoveCurrent(currentX);


}


#if 0
void Composite::Up() {
	//1. ÇöÀç ÁÙÀÇ currentX±îÁö byte ¼ö °è»êÇÑ´Ù.
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
		//1. byteUp °è»êÇÑ´Ù. (¹Ø¿¡¼­ ÀÌ¿ëÇÒ byte¼ö °è»êÇÏ±â À§ÇØ ¹İº¹ÇÏ´Â °Í.)
		while (i < currentX) { 
			character = row->GetChild(i);
			strCharacter = character->GetContent();
			strcpy(chaCharacter, strCharacter.c_str());

			//1.1. 1byte¸é 1¾¿ ´õÇÑ´Ù.
			if (chaCharacter[0] >= 0 && chaCharacter[0] < 128) {
				byte++;
			}
			//1.2. 2byte¸é 2¾¿ ´õÇÑ´Ù.
			else {
				byte = byte + 2;
			}
#if 0
			//1.1. 2byte¸é 2¾¿ ´õÇÑ´Ù.
			if (strCharacter >= "¤¡" && strCharacter <= "ÆR") {
				byte = byte + 2;
			}
			//1.2. 1byte¸é 1¾¿ ´õÇÑ´Ù.
			else {
				byte++;
			}
#endif
			i++;
		}
		//2. y-1 ÁÂÇ¥¿¡¼­ ÇöÀç ÁÂÇ¥ÀÇ byte ¼öº¸´Ù ÀÛ°Å³ª °°À» µ¿¾È byte¼ö ¼¾´Ù.
		i = 0;
		row = this->glyphs.GetAt(currentY - 1);
		while (byteUp <= byte && i < row->GetLength()) {
			character = row->GetChild(i);
			strCharacter = character->GetContent();
			strcpy(chaCharacter, strCharacter.c_str());

			//1.1. 1byte¸é 1¾¿ ´õÇÑ´Ù.
			if (chaCharacter[0] >= 0 && chaCharacter[0] < 128) {
				byteUp++;
			}
			//1.2. 2byte¸é 2¾¿ ´õÇÑ´Ù.
			else {
				byteUp = byteUp + 2;
		    }
#if 0 
			//2.1. 2byte¸é 2¾¿ ´õÇÑ´Ù.
			if (strCharacter >= "¤¡" && strCharacter <= "ÆR") {
				byteUp = byteUp + 2;
			}
			//2.2. 1byte¸é 1¾¿ ´õÇÑ´Ù.
			else {
				byteUp++;
			}
#endif
			i++;
		}
		if (byteUp > byte) {
			i--;
		}


		//3. y-1ÁÂÇ¥·Î ÀÌµ¿ÇÏ°í ÇØ´ç xÁÂÇ¥·Î ÀÌµ¿ÇÑ´Ù.
		this->current = currentY - 1;
		row = this->glyphs.GetAt(currentY - 1);
		row->MoveCurrent(i); //i°¡ currentXÀÓ.
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

	//1. currentY+1ÀÌ lengthº¸´Ù ÀÛÀº °æ¿ì,
	if (currentY + 1 < this->length) {
		//1.1. ÇöÀç ÁÂÇ¥±îÁöÀÇ byte¼ö °è»êÇÑ´Ù.
		while (i < currentX) {
			character = row->GetChild(i);
			strCharacter = character->GetContent();
			strcpy(chaCharacter, strCharacter.c_str());

			//1.1.1. 1byte¸é 1¾¿ ´õÇÑ´Ù.
			if (chaCharacter[0] >= 0 && chaCharacter[0] < 128) {
				byte++;
			}
			//1.1.2. 2byte¸é 2¾¿ ´õÇÑ´Ù.
			else {
				byte = byte + 2;
			}
#if 0
			//1.1.1. 2byte¸é 2¾¿ ´õÇÑ´Ù.
			if (strCharacter >= "¤¡" && strCharacter <= "ÆR") {
				byte = byte + 2;
			}
			//1.1.2. 1byte¸é 1¾¿ ´õÇÑ´Ù.
			else {
				byte++;
			}
#endif
			i++;
		}
		//1.2. y+1 ÁÂÇ¥¿¡¼­ ÇöÀç ÁÂÇ¥ÀÇ byte ¼öº¸´Ù ÀÛ°Å³ª °°À» µ¿¾È byte ¼ö¸¦ ¼¾´Ù.
		i = 0;
		row = this->glyphs.GetAt(currentY + 1);
		while (byteDown <= byte && i < row->GetLength()) {
			character = row->GetChild(i);
			strCharacter = character->GetContent();

			strcpy(chaCharacter, strCharacter.c_str());

			//1.1.1. 1byte¸é 1¾¿ ´õÇÑ´Ù.
			if (chaCharacter[0] >= 0 && chaCharacter[0] < 128) {
				byteDown++;
			}
			//1.1.2. 2byte¸é 2¾¿ ´õÇÑ´Ù.
			else {
				byteDown = byteDown + 2;
		}

#if 0
			//1.2.1. 2byte¸é 2¾¿ ´õÇÑ´Ù
			if (strCharacter >= "¤¡" && strCharacter <= "ÆR") {
				byteDown = byteDown + 2;
			}
			//1.2.2. 1byte¸é 1¾¿ ´õÇÑ´Ù.
			else {
				byteDown++;
			}
#endif
			i++;
		}
		if (byteDown > byte) {
			i--;
		}
		//1.3. y+1 ÁÂÇ¥·Î ÀÌµ¿ÇÏ°í, ÇØ´ç xÁÂÇ¥·Î ÀÌµ¿ÇÑ´Ù.
		this->current = currentY + 1;
		row = this->glyphs.GetAt(currentY + 1);
		row->MoveCurrent(i); //i°¡ currentXÀÓ.
	}
}
#endif

Glyph* Composite::Split(Glyph* dummyRow, int location) { //index: ¾îµğ´Ù°¡ insertÇÒÁö
	int dummyRowIndex;
	Glyph* character;


	//1. insertÇÒ dummyRow, ÀÚ¸¦ À§Ä¡¸¦ ÀÔ·Â¹Ş´Â´Ù.
	//2. length¸¸Å­ ¹İº¹ÇÑ´Ù.
	while (location < this->length) {
		//2.1. À§Ä¡ ÀÌÈÄÀÇ ±ÛÀÚ¸¦ dummyRow¿¡ AddÇÑ´Ù.
		character = this->glyphs.GetAt(location);
		dummyRow->Add(character);

		//4.2. AddÇÑ ±ÛÀÚ¸¦ ±âÁ¸ ÁÙ¿¡¼­ Áö¿î´Ù.
		this->glyphs.Delete(location);
		this->capacity--;
		this->length--;
	}

	//5. dummyRow¸¦ Ãâ·ÂÇÑ´Ù.
	return dummyRow;
}


void Composite::Combine(Glyph* dummyRow) {
	int i = 0;
	Glyph* character;

	//1.CombineÈ£ÃâÇÒ ¶§
	  //1.1. ÁÙÀ» ÀÔ·Â¹Ş´Â´Ù.
	  //1.2. ÀÔ·Â¹ŞÀº ÁÙÀÇ length¸¸Å­ ¹İº¹ÇÑ´Ù.
	  while (i < dummyRow->GetLength()) {
		  //1.2.1. ±ÛÀÚ¸¦ ÀĞ´Â´Ù.
		  character = dummyRow->GetChild(i);

		  //1.2.2. ÇöÀç ÁÙ¿¡ Ãß°¡ÇÑ´Ù.
		  this->Add(character);

		  //1.2.3. ÀÔ·Â¹ŞÀº ÁÙ¿¡¼­ »èÁ¦ÇÑ´Ù.
		  dummyRow->Remove(i);
	  }
	  //1.3. dummyRow¿¡¼­ ´Ù ¿Å°åÀ¸¸é ÁÙ¿¡¼­ »èÁ¦ÇÑ´Ù.(ÀÎÅÍÆäÀÌ½º¿¡¼­)
}

Glyph* Composite::SplitForFront(Glyph* dummyRow, int location) {
	Glyph* character;
	int i = 0;

	//1. length¸¸Å­ ¹İº¹ÇÑ´Ù.
	while (location < this->GetLength()) {
		//1.1. À§Ä¡ ÀÌÈÄÀÇ ±ÛÀÚ¸¦ dummyRow¿¡ AddÇÑ´Ù. (Á¦ÀÏ ¾Õ¿¡)
		character = this->glyphs.GetAt(location);
		dummyRow->Add(i, character);

		//1.2. AddÇÑ ±ÛÀÚ¸¦ ±âÁ¸ ÁÙ¿¡¼­ Áö¿î´Ù.
		this->glyphs.Delete(location);
		this->capacity--;
		this->length--;
		i++;
	}

	//2. dummyRow¸¦ Ãâ·ÂÇÑ´Ù.
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

	//1. Ã£À» ¹®ÀÚ, ´ë¼Ò¹®ÀÚ ±¸ºĞ¿©ºÎ, ÀÚµ¿°³Çà Àü Ä³·µÀÇ ÁÂÇ¥¸¦ ÀÔ·Â¹Ş´Â´Ù.

	//(21.09.17 Ãß°¡) findStr¿¡ °³Çà¹®ÀÚ°¡ Æ÷ÇÔµÇ¾î ÀÖ´ÂÁö È®ÀÎÇÑ´Ù.
	size_t nPos = findStr.find("\n");
	if (nPos != string::npos) { //°³Çà¹®ÀÚ°¡ ÀÖ´Â °æ¿ì,
		isThereLineChange = true;
	}

	
	//2. Ã£À» ¹®ÀÚÀÇ ÃÑ °³¼ö¸¦ ±¸ÇÑ´Ù.
	//int findLength = findStr.size();
	
	const unsigned char* constStr = reinterpret_cast<const unsigned char*> (findStr.c_str());
	int findLength = _mbslen(constStr);
	

	currentY = firstCurrentY;
	noteLength = this->length;

	//3. °³Çà ¹®ÀÚ°¡ Æ÷ÇÔµÇ¾î ÀÖÁö ¾ÊÀº °æ¿ì, ¹®¼­ÀÇ ³¡ÀÌ ¾Æ´Ñµ¿¾È ¹İº¹ÇÑ´Ù.
	while (isThereLineChange == false && currentY < noteLength && isThereSameText == false) {

		//3.1. ÇöÀç Ä³·µÀÌ ÀÖ´Â ÁÙÀ» Ã£´Â´Ù.
		row = this->glyphs.GetAt(currentY);
		rowLength = row->GetLength();

		//3.2. i¸¦ ÃÊ±âÈ­ÇÏ°í restLength¸¦ Á¤ÇÑ´Ù.
		if (currentY == firstCurrentY) {
			i = beforeLineChangeCurrentX;
			restLength = rowLength - i;
		}
		else {
			i = 0;
			restLength = rowLength;
		}

		//3.3. restLength°¡ findLengthº¸´Ù Å©°Å³ª °°°í, °°Àº ´Ü¾î°¡ ¾ø´Â µ¿¾È ¹İº¹ÇÑ´Ù.
		while (restLength >= findLength && isThereSameText == false) {
			
			//3.3.1. ÇØ´ç ÁÙ¿¡¼­ Ã£À» ¹®ÀÚÀÇ ÃÑ °³¼ö¸¸Å­ ¹®ÀÚ¸¦ ¸¸µç´Ù.
			temp.clear(); //¹®ÀÚ¿­ Áö¿ì±â
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

			//3.3.2. ´ë¼Ò¹®ÀÚ ±¸ºĞÇÏ¸é,
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
			//3.3.3. ´ë¼Ò¹®ÀÚ ±¸ºĞÇÏÁö ¾ÊÀ¸¸é,
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

	//4. °³Çà ¹®ÀÚ°¡ Æ÷ÇÔµÇ¾î ÀÖ´Â °æ¿ì, ¹®¼­ÀÇ ³¡ÀÌ ¾Æ´Ñµ¿¾È ¹İº¹ÇÑ´Ù.
	while (isThereLineChange == true && currentY < noteLength && isThereSameText == false) {
		
		//4.1. temp ÃÊ±âÈ­ÇÑ´Ù.
		temp.clear();
		tempCurrentY = currentY;
		j = i;
		k = 0;

		//4.2. findLength¸¸Å­ ºñ±³ÇÒ ¹®ÀÚ¸¦ ¸¸µç´Ù.
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

		//4.3. ´ë¼Ò¹®ÀÚ ±¸ºĞÇÏ¸é,
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

		//4.4. ´ë¼Ò¹®ÀÚ ±¸ºĞÇÏÁö ¾ÊÀ¸¸é,
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

	//5. °°Àº ´Ü¾î°¡ ¾øÀ¸¸é firstXY, endXY¿¡ 0À» ÀÔ·ÂÇÑ´Ù.
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

	//1. ¹®¼­ÀÇ Ã³À½ÀÌ ¾Æ´Ñµ¿¾È ¹İº¹ÇÑ´Ù.
	//int findLength = findStr.size();

	//(21.09.17 Ãß°¡) findStr¿¡ °³Çà¹®ÀÚ°¡ Æ÷ÇÔµÇ¾î ÀÖ´ÂÁö È®ÀÎÇÑ´Ù.
	size_t nPos = findStr.find("\n");
	if (nPos != string::npos) { //°³Çà¹®ÀÚ°¡ ÀÖ´Â °æ¿ì,
		isThereLineChange = true;
	}
	
	//i¸¦ ÃÊ±âÈ­ÇÑ´Ù.
	int currentY = firstCurrentY;
	i = beforeLineChangeCurrentX - 1;

	while (i < 0 && currentY>=0) {
		currentY--;
		if (currentY >= 0) {
			row = this->glyphs.GetAt(currentY);
			i = row->GetLength() - 1;
		}
	}

	//2. Ã£À» ¹®ÀÚÀÇ ÃÑ °³¼ö¸¦ ±¸ÇÑ´Ù.
	const unsigned char* constStr = reinterpret_cast<const unsigned char*> (findStr.c_str());
	int findLength = _mbslen(constStr);

	

	//3. °³Çà ¹®ÀÚ°¡ Æ÷ÇÔµÇ¾î ÀÖÁö ¾ÊÀº °æ¿ì, ¹®¼­ÀÇ Ã³À½ÀÌ ¾Æ´Ñµ¿¾È ¹İº¹ÇÑ´Ù.
	while (isThereLineChange == false && currentY >= 0 && isThereSameText == false) {
		//1.1. ÇöÀç Ä³·µÀÌ ÀÖ´Â ÁÙÀ» Ã£´Â´Ù.
		row = this->glyphs.GetAt(currentY);
		rowLength = row->GetLength();

		//1.2. i¸¦ ÃÊ±âÈ­ÇÏ°í restLength¸¦ Á¤ÇÑ´Ù.
		if (currentY == firstCurrentY) {
			i = beforeLineChangeCurrentX - 1;
			restLength = i + 1;
		}
		else {
			i = rowLength - 1;
			restLength = rowLength;
		}

		//1.3. restLength°¡ findLengthº¸´Ù Å©°Å³ª °°°í, °°Àº ´Ü¾î°¡ ¾ø´Âµ¿¾È ¹İº¹ÇÑ´Ù.
		while (restLength >= findLength && isThereSameText == false) {
			
			//1.3.1. ÇØ´ç ÁÙ¿¡¼­ ÇöÀç currentX¿¡¼­ Ã£Àº ¹®ÀÚÀÇ ÃÑ °³¼ö¸¸Å­ ¹®ÀÚ¸¦ ¸¸µç´Ù.
			temp.clear(); //¹®ÀÚ¿­ Áö¿ì±â
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

			//1.3.2. ´ë¼Ò¹®ÀÚ ±¸ºĞÇÏ¸é
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
			//1.3.3. ´ë¼Ò¹®ÀÚ ±¸ºĞ ¾ÈÇÏ¸é
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

	//4. °³Çà ¹®ÀÚ°¡ Æ÷ÇÔµÇ¾î ÀÖ´Â °æ¿ì, ¹®¼­ÀÇ Ã³À½ÀÌ ¾Æ´Ñµ¿¾È ¹İº¹ÇÑ´Ù.
	while (isThereLineChange == true && currentY >= 0 && isThereSameText == false) {

		//4.1. temp ÃÊ±âÈ­ÇÑ´Ù.
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

		//4.2. ´ë¼Ò¹®ÀÚ ±¸ºĞÇÏ¸é
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
		//4.3. ´ë¼Ò¹®ÀÚ ±¸ºĞ ¾ÈÇÏ¸é
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


	//5. °°Àº ´Ü¾î°¡ ¾øÀ¸¸é, firstXY, endXY¿¡ 0À» ÀÔ·ÂÇÑ´Ù.
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

	//¿¹¿ÜÃÊ±âÈ­
	(*exception) = 0;

	//0. Ã£À» ³»¿ë, ¹Ù²Ü ³»¿ëÀÇ ÃÑ °³¼ö¸¦ ±¸ÇÑ´Ù.
	const unsigned char* constStr = reinterpret_cast<const unsigned char*> (findStr.c_str());
	int findLength = _mbslen(constStr);

	//constStr = reinterpret_cast<const unsigned char*> (const_cast<const char*>(replaceStr.c_str()));
	constStr = reinterpret_cast<const unsigned char*> (replaceStr.c_str());
	int replaceLength = _mbslen(constStr);


	//1. Ã£À» ³»¿ë, ¹Ù²Ü ³»¿ë, ÇöÀç ¼±ÅÃµÈ ³»¿ë, ´ë¼Ò¹®ÀÚ ±¸ºĞ, ÀÚµ¿°³Çà Àü Ä³·µÀÇ ÁÂÇ¥¸¦ ÀÔ·Â¹Ş´Â´Ù.
	//2. ÇöÀç ¼±ÅÃµÈ ´Ü¾î°¡ Ã£°íÀÚ ÇÏ´Â ´Ü¾î¿Í °°ÀºÁö È®ÀÎÇÑ´Ù.
	  //2.1. ´ë¼Ò¹®ÀÚ ±¸ºĞÇÏ¸é,
	if (matchCase == true && strcmp(cFindStr, cSelectedStr) == 0) {
		isSameText = true;
	}

	//2.2. ´ë¼Ò¹®ÀÚ ±¸ºĞ ¾ÈÇÏ¸é,
	else if (matchCase == false && stricmp(cFindStr, cSelectedStr) == 0) {
		isSameText = true;
	}

	//2.3. ¼±ÅÃµÈ ´Ü¾î¿Í Ã£°íÀÚ ÇÏ´Â ´Ü¾î°¡ °°À¸¸é, (¹Ù²Ù·Á´Â ´Ü¾î°¡ ¾Æ´Ñ)
	if (isSameText == true) {
		//2.3.0. stringÇüÀ» charÇüÀ¸·Î º¯È¯ÇÑ´Ù.
		//chaCharacter = new char(replaceStr.length());
		chaCharacter = new char[replaceStr.length() +1];
		strcpy(chaCharacter, cReplaceStr);


		//2.3.1. ¼±ÅÃµÈ ´Ü¾î¸¦ Áö¿î´Ù.
		selectedFirstX = beforeLineChangeCurrentX - findLength;
		row = this->glyphs.GetAt(firstCurrentY);


		while (i <= findLength) {
			row->Remove(selectedFirstX);
			i++;
		}

		//2.3.2. ¹Ù²Ü ³»¿ëÀ» Àû´Â´Ù.
		creator = new CreateProduct();
		i = 0;
		j = selectedFirstX;
		while (i < replaceStr.length()) {
			afterCharacter[0] = chaCharacter[i];
			afterCharacter[1] = chaCharacter[i + 1];

			//1. 1byteÀÎ °æ¿ì,
			if (afterCharacter[0] >= 0 && afterCharacter[0] < 128) {
				afterCharacter[1] = '\0';
				character = creator->Make(afterCharacter);
				i++;
			}
			//2. 2byteÀÎ °æ¿ì,
			else {
				character = creator->Make(afterCharacter);
				i = i + 2;
			}
			//3. ³»¿ëÀ» Àû´Â´Ù.
			row->Add(j, character);
			j++;
		}

		//2.3.3. Ä³·µÀÇ ÁÂÇ¥¸¦ »õ·Î ÀûÀº ³»¿ëÀÇ ¸Ç µÚ·Î Á¤ÇÑ´Ù.
		beforeLineChangeCurrentX = selectedFirstX + replaceLength;
	}
	//FindSameTextWithDown°ú °°À½ ----------------------------------------------------------------------------------
	//3. ¹®¼­ÀÇ ³¡ÀÌ ¾Æ´Ñµ¿¾È ¹İº¹ÇÑ´Ù.

	//(21.08.30 Ãß°¡) ÁÖÀ§¿¡ ¹èÄ¡°¡ ´­·ÁÁ®ÀÖ´Â °æ¿ì,
	if (searchAround == true) {
		currentY = 0;
	}
	else {
		currentY = firstCurrentY;
	}
	noteLength = this->length;


	//3. ¹®¼­ÀÇ ³¡ÀÌ ¾Æ´Ñµ¿¾È ¹İº¹ÇÑ´Ù.
	while (currentY < noteLength && isThereSameText == false) {

		//3.1. ÇöÀç Ä³·µÀÌ ÀÖ´Â ÁÙÀ» Ã£´Â´Ù.
		row = this->glyphs.GetAt(currentY);
		rowLength = row->GetLength();

		//3.2. i¸¦ ÃÊ±âÈ­ÇÏ°í restLength¸¦ Á¤ÇÑ´Ù.
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

		//3.3. restLength°¡ findLengthº¸´Ù Å©°Å³ª °°°í, °°Àº ´Ü¾î°¡ ¾ø´Â µ¿¾È ¹İº¹ÇÑ´Ù.
		while (restLength >= findLength && isThereSameText == false) {

			//3.3.1. ÇØ´ç ÁÙ¿¡¼­ Ã£À» ¹®ÀÚÀÇ ÃÑ °³¼ö¸¸Å­ ¹®ÀÚ¸¦ ¸¸µç´Ù.
			temp.clear(); //¹®ÀÚ¿­ Áö¿ì±â
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

			//3.3.2. ´ë¼Ò¹®ÀÚ ±¸ºĞÇÏ¸é,
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
			//3.3.3. ´ë¼Ò¹®ÀÚ ±¸ºĞÇÏÁö ¾ÊÀ¸¸é,
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
	//4. ±× ´ÙÀ½ °°Àº ´Ü¾î´Â ¾øÁö¸¸, Ã³À½ µé¾î¿ÔÀ» ¶§ replaceÇÑ °æ¿ì,
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

		//4.1. ±× ´ÙÀ½ °°Àº ´Ü¾îµµ ¾ø°í, Ã³À½ µé¾î¿ÔÀ» ¶§ replaceÇÏÁö ¾ÊÀº °æ¿ì, firstXY, endXY¿¡ 0À» ÀÔ·ÂÇÑ´Ù.
		else if(isSameText == false && searchAround == false){
			(*firstY) = 0;
			(*firstX) = 0;
			(*endY) = 0;
			(*endX) = 0;
		}
	}
	//------------------------------------------------------------------------------------------------------

//#if 0
	//ÇÒ´çÇØÁ¦ ÇØÁØ´Ù.
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



	//0. Ã£À» ³»¿ë, ¹Ù²Ü ³»¿ëÀÇ ÃÑ °³¼ö¸¦ ±¸ÇÑ´Ù.
	const unsigned char* constStr = reinterpret_cast<const unsigned char*> (findStr.c_str());
	int findLength = _mbslen(constStr);

	constStr = reinterpret_cast<const unsigned char*> (replaceStr.c_str());
	int replaceLength = _mbslen(constStr);




}
#endif






