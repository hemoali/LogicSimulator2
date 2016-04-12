#pragma once
#include"Action.h"
#include"..\Components\NOR3.h"
#include"..\ApplicationManager.h"
class AddNORgate3:public Action
{
private:
	 string gateLabel;
	GraphicsInfo GInfo;
public:
	AddNORgate3(ApplicationManager *pApp);
	virtual ~AddNORgate3(void);

	//Reads parameters required for action to execute
	virtual bool  ReadActionParameters();
	//Execute action (code depends on action type)
	virtual void Execute();

	virtual void Undo();
	virtual void Redo();

};

