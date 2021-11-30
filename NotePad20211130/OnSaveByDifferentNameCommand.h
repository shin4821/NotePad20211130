//OnSaveByDifferentNameCommand.h (21.09.07 Ãß°¡)
#ifndef _ONSAVEBYDIFFERENTNAMECOMMAND_H
#define _ONSAVEBYDIFFERENTNAMECOMMAND_H
#include"Command.h"

class NotePadForm;
class OnSaveByDifferentNameCommand : public Command {
public:
	OnSaveByDifferentNameCommand(NotePadForm* notePadForm);
	virtual ~OnSaveByDifferentNameCommand();
	virtual void Execute();
};

#endif //_ONSAVEBYDIFFERENTNAMECOMMAND_H
