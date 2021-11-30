//Composite.h
#ifndef _COMPOSITE_H
#define _COMPOSITE_H
#include"Glyph.h"
#include"Array.h"
typedef signed long int Long;

class Composite :public Glyph {
public:
	Composite(Long capacity = 256);
	virtual ~Composite() = 0; //순수지정자
	virtual Long Add(Glyph* glyph);
	virtual Long Remove(Long index);
	virtual Glyph* GetChild(Long index);
	Composite& operator=(const Composite& source); //치환연산자
	Composite(const Composite& source); //복사생성자
	virtual Long GetCapacity();
	virtual Long GetLength();
	virtual Long GetCurrent();
	virtual void MoveCurrent(Long current);
	virtual Long Save(string fileName); //21.07.12. 추가 (꽂아놓다)  
	virtual Long Load(string fileName);

	virtual void First();
	virtual void Last();
	virtual void Previous();
	virtual void Next();
	virtual void PreviousWord();
	virtual void NextWord();
	//virtual void Up();
	//virtual void Down();
	virtual Long Add(Long index, Glyph* glyph);
	virtual Glyph* Split(Glyph* dummyRow, int location);
	virtual void Combine(Glyph* dummyRow);
	virtual Glyph* SplitForFront(Glyph* dummyRow, int location);
	virtual void FindSameTextWithDown(string findStr, bool matchCase, int firstCurrentY, int beforeLineChangeCurrentX,
		int* firstY, int* firstX, int* endY, int* endX);
	virtual void FindSameTextWithUp(string findStr, bool matchCase, int firstCurrentY, int beforeLineChangeCurrentX,
		int* firstY, int* firstX, int* endY, int* endX);
	virtual void ReplaceSameText(string findStr, string replaceStr, string selectedStr, bool matchCase, bool searchAround, 
		int firstCurrentY,
		int beforeLineChangeCurrentX, int* firstY, int* firstX, int* endY, int* endX, int* exception);


protected:
	Array<Glyph*> glyphs;
	Long capacity;
	Long length;
	Long current;
};



#endif//_COMPOSITE_H

