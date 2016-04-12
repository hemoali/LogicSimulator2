#pragma once
#include"Action.h"
#include"..\Components\BUFFER.h"
#include"..\ApplicationManager.h"
class AddBUFFER:public Action
{
private:
	 string gateLabel;
	GraphicsInfo GInfo;
public:
	AddBUFFER(ApplicationManager *pApp);
	virtual ~AddBUFFER(void);

	//Reads parameters required for action to execute
	virtual bool  ReadActionParameters();
	//Execute action (code depends on action type)
	virtual void Execute();

	virtual void Undo();
	virtual void Redo();

	
};

