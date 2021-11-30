//MacroCommand.h
#ifndef _MACROCOMMAND_H
#define _MACROCOMMAND_H
#include"Command.h"
#include"Array.h"
typedef signed long int Long;

class NotePadForm;
class MacroCommand : public Command {
public:
	MacroCommand(NotePadForm* notePadForm, Long capacity = 256);
	virtual ~MacroCommand();
	virtual void Execute();
	virtual Long Add(Command* command);
	virtual void Remove(Command* command);

private:
	Array<Command*>commands; //21.07.20 _ FOR Observe Pattern
	Long capacity;
	Long length;
};

int CompareCommands(void* one, void* other);

#endif //_MACROCOMMAND_H
