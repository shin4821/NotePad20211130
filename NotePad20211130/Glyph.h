//Glyph.h
#ifndef _GLYPH_H
#define _GLYPH_H
#include<string>
using namespace std;
typedef signed long int Long;
#include"Array.h" //21.07.12. 추가 (꽂아놓다): row의 Array<Glyph*> glyphs 출력 위해;

class Visitor; //21.11.24.추가
class Glyph {
public:
	Glyph();
	virtual ~Glyph() = 0;//순수가상함수
	virtual Long Add(Glyph* glyph);
	virtual Long Remove(Long index);
	virtual Glyph* GetChild(Long index);
	virtual Glyph* Clone() = 0;//순수가상함수
	virtual string GetContent()=0; //순수가상함수
	Glyph& operator=(const Glyph& source); //치환연산자
	Glyph(const Glyph& source); //복사생성자

	virtual Long GetCapacity();
	virtual Long GetLength();
	virtual Long GetCurrent();
	virtual void MoveCurrent(Long location);

	virtual Long Save(string fileName); //21.07.12. 추가 (꽂아놓다)    string fileName 다시 추가하기
	virtual Long Load(string fileName); 
	virtual void First();
	virtual void Last();
	virtual void Previous();
	virtual void Next();
	virtual void PreviousWord();
	virtual void NextWord();
	virtual Long Add(Long index, Glyph* glyph); //21.07.23. 추가(끼워쓰기)
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
	//virtual void ReplaceAllSameText(string findStr, string replaceStr, bool matchCase, int firstCurrentY,
		//int beforeLineChangeCurrentX);

	virtual void Accept(Visitor* visitor) =0; //(21.11.24.추가)순수가상함수
	virtual void AcceptForSelect(Visitor* visitor) = 0;



};
#endif//_GLYPH_H







