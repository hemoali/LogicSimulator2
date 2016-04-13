#ifndef SWITCH_Header
#define SWITCH_Header
#include"../Components/SWITCH.h"
#include"..\ApplicationManager.h"
#include"Action.h"
class AddSWITCH:public Action
{
private:
	 string gateLabel;
	GraphicsInfo GInfo;
public:
	AddSWITCH(ApplicationManager *pApp);
	virtual ~AddSWITCH(void);
	//Reads parameters required for action to execute
	virtual bool  ReadActionParameters(image * );
	//Execute action (code depends on action type)
	virtual void Execute();

	virtual void Undo();
	virtual void Redo();

};

#endif