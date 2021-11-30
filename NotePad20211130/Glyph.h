//Glyph.h
#ifndef _GLYPH_H
#define _GLYPH_H
#include<string>
using namespace std;
typedef signed long int Long;
#include"Array.h" //21.07.12. �߰� (�ȾƳ���): row�� Array<Glyph*> glyphs ��� ����;

class Visitor; //21.11.24.�߰�
class Glyph {
public:
	Glyph();
	virtual ~Glyph() = 0;//���������Լ�
	virtual Long Add(Glyph* glyph);
	virtual Long Remove(Long index);
	virtual Glyph* GetChild(Long index);
	virtual Glyph* Clone() = 0;//���������Լ�
	virtual string GetContent()=0; //���������Լ�
	Glyph& operator=(const Glyph& source); //ġȯ������
	Glyph(const Glyph& source); //���������

	virtual Long GetCapacity();
	virtual Long GetLength();
	virtual Long GetCurrent();
	virtual void MoveCurrent(Long location);

	virtual Long Save(string fileName); //21.07.12. �߰� (�ȾƳ���)    string fileName �ٽ� �߰��ϱ�
	virtual Long Load(string fileName); 
	virtual void First();
	virtual void Last();
	virtual void Previous();
	virtual void Next();
	virtual void PreviousWord();
	virtual void NextWord();
	virtual Long Add(Long index, Glyph* glyph); //21.07.23. �߰�(��������)
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

	virtual void Accept(Visitor* visitor) =0; //(21.11.24.�߰�)���������Լ�
	virtual void AcceptForSelect(Visitor* visitor) = 0;



};
#endif//_GLYPH_H







