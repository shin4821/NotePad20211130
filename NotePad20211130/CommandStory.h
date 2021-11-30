//CommandStory.h
//(21.09.02 Ãß°¡)
#ifndef _COMMANDSTORY_H
#define _COMMANDSTORY_H

class UndoList;
class RedoList;
class Command;
class NotePadForm;
class CommandStory {
public:
	CommandStory(NotePadForm* notePadForm);
	~CommandStory();
	Command* Undo();
	Command* Redo();
	void Pile(Command* command);

public:
	NotePadForm* notePadForm;
	UndoList* undoList;
	RedoList* redoList;

};


#endif//_COMMANDSTORY_H

