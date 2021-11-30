//FindAndReplace.h
#ifndef _FINDANDREPLACE_H
#define _FINDANDREPLACE_H
#include<string>
using namespace std;
#pragma warning(disable:4996)

class FindSameText;
class FindAndReplace {
public:
	FindAndReplace(FindSameText* findSameText);
	~FindAndReplace();
	void ReplaceSameText(string findStr, string replaceStr, string selectedStr, bool matchCase, bool searchAround,
		int firstCurrentY, int beforeLineChangeCurrentX, int* firstY, int* firstX, 
		int* endY, int* endX, int* exception, int* isReplaced);
	void RemoveSelectedWord();
	void AddWordForUndo(int firstX, int firstY, string findStr, int* changedX, int* changedY);
	void RemoveWordForRedo(int firstX, int firstY, int beforeEndX, int beforeEndY);



private:
	FindSameText* findSameText;
	bool isReplaced;

};

#endif//_FINDANDREPLACE_H

