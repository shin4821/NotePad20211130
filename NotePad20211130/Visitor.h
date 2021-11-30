//Visitor.h
#ifndef _VISITOR_H
#define _VISITOR_H

class Glyph;
class NotePadForm;
class Visitor {
public:
	Visitor(NotePadForm* notePadForm);
	~Visitor();
	void VisitNote(Glyph* note);
	void VisitNoteForSelect(Glyph* note);
	void VisitRow(Glyph* row);
	void VisitRowForSelect(Glyph* row);
	void VisitCharacter(Glyph* character);
	void SetFirstY(int firstY);
	void SetFirstX(int firstX);
	void SetEndY(int endY);
	void SetEndX(int endX);

private:
	NotePadForm* notePadForm;
	int distanceX;
	int rowCount;
	int firstY;
	int firstX;
	int endY;
	int endX;
	int startX;
	int limitX;

};

#endif//_VISITOR_H