//FactoryProductCommand.h
#ifndef _FACTORYPRODUCTCOMMAND_H
#define _FACTORYPRODUCTCOMMAND_H

class Command;
class NotePadForm;
class FactoryProductCommand {
public:
	FactoryProductCommand(NotePadForm* notePadForm);
	~FactoryProductCommand();
	Command* Make(int ID);

private:
	NotePadForm* notePadForm;
};

#endif//_FACTORYPRODUCTCOMMAND_H
