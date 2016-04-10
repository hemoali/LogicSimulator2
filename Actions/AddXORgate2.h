#pragma once
#include"..\Components/XOR2.h"
#include"..\ApplicationManager.h"
#include"Action.h"
class AddXORgate2:public Action
{
private:
	 string gateLabel;
	GraphicsInfo GInfo;
public:
	AddXORgate2(ApplicationManager *pApp);
	virtual ~AddXORgate2(void);

	//Reads parameters required for action to execute
	virtual void ReadActionParameters();
	//Execute action (code depends on action type)
	virtual void Execute();

	virtual void Undo();
	virtual void Redo();

};

