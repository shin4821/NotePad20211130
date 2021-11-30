//OnPageSetUpCommand.h (21.09.23 Ãß°¡)
#ifndef _ONPAGESETUPCOMMAND_H
#define _ONPAGESETUPCOMMAND_H
#include"Command.h"
#include<afxwin.h>

class NotePadForm;
class OnPageSetUpCommand :public Command {
public:
	OnPageSetUpCommand(NotePadForm* notePadForm);
	virtual ~OnPageSetUpCommand();
	virtual void Execute();

#if 0
private:
	int top;
	int bottom;
	int left;
	int right;
	int paperWidth;
	int paperHeight;
	int actualWidth;
	int actualHeight;
	CString header;
	CString footer;
#endif

};

#endif//_ONPAGESETUP_H

