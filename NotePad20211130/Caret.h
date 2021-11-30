//Caret.h
#ifndef _CARET_H
#define _CARET_H
typedef signed long int Long;

class Location;
class Caret {
public:
	Caret(Location* location);
	Caret(Location* location, int x, int y);
	~Caret();
	Long GetX();
	Long GetY();
	void Move(int x, int y);
public:
	Long x;
	Long y;
	Location* location;
};

#endif //_CARET_H
