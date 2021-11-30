//FactoryProductCommand.cpp
#include"FactoryProductCommand.h"
#include"Command.h"
#include"OnCharCommand.h"
#include"OnUnExecuteCommand.h"
#include"OnReExecuteCommand.h"
#include"OnImeCompositionCommand.h"
#include"OnImeCharCommand.h"
#include"OnSaveCommand.h"
#include"OnSaveByDifferentNameCommand.h"
#include"OnOpenCommand.h"
#include"OnCloseCommand.h"
#include"OnFontCommand.h"
#include"OnHScrollCommand.h"
#include"OnVScrollCommand.h"
#include"OnBackSpaceCommand.h"
#include"OnLineChangeCommand.h"
#include"OnSelectPreviousCommand.h"
#include"OnSelectNextCommand.h"
#include"OnSelectNextWordCommand.h"
#include"OnSelectPreviousWordCommand.h"
#include"OnSelectFirstRowCommand.h"
#include"OnSelectEndRowCommand.h"
#include"OnSelectFirstDocCommand.h"
#include"OnSelectEndDocCommand.h"
#include"OnSelectUpCommand.h"
#include"OnSelectDownCommand.h"
#include"OnPreviousCommand.h"
#include"OnNextCommand.h"
#include"OnPreviousCommand.h"
#include"OnPreviousWordCommand.h"
#include"OnNextWordCommand.h"
#include"OnFirstRowCommand.h"
#include"OnEndRowCommand.h"
#include"OnFirstDocCommand.h"
#include"OnEndDocCommand.h"
#include"OnUpCommand.h"
#include"OnDownCommand.h"
#include"OnCopyCommand.h"
#include"OnPasteCommand.h"
#include"OnCutOffCommand.h"
#include"OnDeleteCommand.h"
#include"OnFindDialogCommand.h"
#include"OnReplaceDialogCommand.h"
#include"OnFindNextCommand.h"
#include"OnFindPreviousCommand.h"
#include"OnReplaceCommand.h"
#include"OnReplaceAllCommand.h"
#include"OnPageSetUpCommand.h"
#include"OnPrintCommand.h"
#include"OnNewCreateCommand.h"
#include"OnSelectAllCommand.h"
#include"OnTimeAndDateCommand.h"
#include"OnGetHelpCommand.h"
#include"OnInformationCommand.h"
#include"OnPreviewCommand.h"
#include"resource.h"


FactoryProductCommand::FactoryProductCommand(NotePadForm* notePadForm) {
	this->notePadForm = notePadForm;
}

FactoryProductCommand::~FactoryProductCommand() {
}

Command* FactoryProductCommand::Make(int ID) {
	Command* command = 0;

	if (ID == 40001) {
		command = new OnCharCommand(this->notePadForm);
	}
	else if (ID == 40002) {
		command = new OnUnExecuteCommand(this->notePadForm);
	}
	else if (ID == 40003) {
		command = new OnReExecuteCommand(this->notePadForm);
	}
	else if (ID == 40004) {
		command = new OnImeCompositionCommand(this->notePadForm);
	}
	else if (ID == 40005) {
		command = new OnImeCharCommand(this->notePadForm);
	}
	else if (ID == 40006) {
		command = new OnSaveCommand(this->notePadForm);
	}
	else if (ID == 40007) {
		command = new OnSaveByDifferentNameCommand(this->notePadForm);
	}
	else if (ID == 40008) {
		command = new OnOpenCommand(this->notePadForm);
	}
	else if (ID == 40009) {
		command = new OnCloseCommand(this->notePadForm);
	}
	else if (ID == 40010) {
		command = new OnFontCommand(this->notePadForm);
	}
	else if (ID == 40011) {
		command = new OnHScrollCommand(this->notePadForm);
	}
	else if (ID == 40012) {
		command = new OnVScrollCommand(this->notePadForm);
	}
	else if (ID == 40013) {
		command = new OnBackSpaceCommand(this->notePadForm);
	}
	else if (ID == 40014) {
		command = new OnLineChangeCommand(this->notePadForm);
	}
	else if (ID == 40015) {
		command = new OnSelectPreviousCommand(this->notePadForm);
	}
	else if (ID == 40016) {
		command = new OnSelectNextCommand(this->notePadForm);
	}
	else if (ID == 40017) {
		command = new OnSelectNextWordCommand(this->notePadForm);
	}
	else if (ID == 40018) {
		command = new OnSelectPreviousWordCommand(this->notePadForm);
	}
	else if (ID == 40019) {
		command = new OnSelectFirstRowCommand(this->notePadForm);
	}
	else if (ID == 40020) {
		command = new OnSelectEndRowCommand(this->notePadForm);
	}
	else if (ID == 40021) {
		command = new OnSelectFirstDocCommand(this->notePadForm);
	}
	else if (ID == 40022) {
		command = new OnSelectEndDocCommand(this->notePadForm);
	}
	else if (ID == 40023) {
		command = new OnSelectUpCommand(this->notePadForm);
	}
	else if (ID == 40024) {
		command = new OnSelectDownCommand(this->notePadForm);
	}
	else if (ID == 40025) {
		command = new OnPreviousCommand(this->notePadForm);
	}
	else if (ID == 40026) {
		command = new OnNextCommand(this->notePadForm);
	}
	else if (ID == 40027) {
		command = new OnPreviousWordCommand(this->notePadForm);
	}
	else if (ID == 40028) {
		command = new OnNextWordCommand(this->notePadForm);
	}
	else if (ID == 40029) {
		command = new OnFirstRowCommand(this->notePadForm);
	}
	else if (ID == 40030) {
		command = new OnEndRowCommand(this->notePadForm);
	}
	else if (ID == 40031) {
		command = new OnFirstDocCommand(this->notePadForm);
	}
	else if (ID == 40032) {
		command = new OnEndDocCommand(this->notePadForm);
	}
	else if (ID == 40033) {
		command = new OnUpCommand(this->notePadForm);
	}
	else if (ID == 40034) {
		command = new OnDownCommand(this->notePadForm);
	}
	else if (ID == 40035) {
	    command = new OnCopyCommand(this->notePadForm);
    }
	else if (ID == 40036) { //40036
	    command = new OnPasteCommand(this->notePadForm);
    }
	else if (ID == 40037) {
	    command = new OnCutOffCommand(this->notePadForm);
    }
	else if (ID == 40038) {
	    command = new OnDeleteCommand(this->notePadForm);
    }
	else if (ID == 40039) {
	    command = new OnFindDialogCommand(this->notePadForm);
    }
	else if (ID == 40040) {
	    command = new OnFindNextCommand(this->notePadForm);
    }
	else if (ID == 40041) {
	    command = new OnFindPreviousCommand(this->notePadForm);
    }
	else if (ID == 40043) {
	    command = new OnReplaceDialogCommand(this->notePadForm);
    }
	else if (ID == 40044) {
	    command = new OnReplaceCommand(this->notePadForm);
    }
	else if (ID == 40045) {
	    command = new OnReplaceAllCommand(this->notePadForm);
    }
	else if (ID == 40046) {
	    command = new OnPageSetUpCommand(this->notePadForm);
    }
	else if (ID == 40047) {
	    command = new OnPrintCommand(this->notePadForm);
    }
	else if (ID == 40048) {
	    command = new OnNewCreateCommand(this->notePadForm);
    }
	else if (ID == 40049) { 
	    command = new OnSelectAllCommand(this->notePadForm);
    }
	else if (ID == 40050) {
	    command = new OnTimeAndDateCommand(this->notePadForm);
    }
	else if (ID == 40051) {
	    command = new OnGetHelpCommand(this->notePadForm);
    }
	else if (ID == 40052) {
	    command = new OnInformationCommand(this->notePadForm);
    }
	else if (ID == 40053) {
	    command = new OnPreviewCommand(this->notePadForm);
    }


	return command;
}













