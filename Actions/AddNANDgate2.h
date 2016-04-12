#pragma once
#include"Action.h"
#include"..\Components\NAND2.h"
#include"..\ApplicationManager.h"
class AddNANDgate2:public Action
{
private:
	 string gateLabel;
	GraphicsInfo GInfo;
public:
	AddNANDgate2(ApplicationManager *pApp);
	virtual ~AddNANDgate2(void);

	//Reads parameters required for action to execute
	virtual bool  ReadActionParameters();
	//Execute action (code depends on action type)
	virtual void Execute();

	virtual void Undo();
	virtual void Redo();

};

