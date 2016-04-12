#pragma once
#include"Action.h"
#include"..\Components\AND3.h"
#include"..\ApplicationManager.h"
class AddANDgate3 : public Action
{
private:
	string gateLabel;
	GraphicsInfo GInfo;

public:
	AddANDgate3(ApplicationManager *pApp);
	virtual ~AddANDgate3(void);

	//Reads parameters required for action to execute
	virtual bool  ReadActionParameters();
	//Execute action (code depends on action type)
	virtual void Execute();

	virtual void Undo();
	virtual void Redo();

};

