#ifndef SWITCH_Header
#define SWITCH_Header
#include"Action.h"
#include"..\Components\SWITCH.h"
class AddSWITCH:public Action
{
public:
	AddSWITCH(ApplicationManager *pApp);
	virtual ~AddSWITCH(void);
	//Reads parameters required for action to execute
	virtual bool  ReadActionParameters(image * , Component*);
	//Execute action (code depends on action type)
	virtual void Execute();

};

#endif