#pragma once
#include"..\Components\NOT.h"
#include"..\ApplicationManager.h"
class AddNOTgate:public Action
{
private:
	 string gateLabel;
	GraphicsInfo GInfo;
public:
	AddNOTgate(ApplicationManager *pApp);
	virtual ~AddNOTgate(void);

	//Reads parameters required for action to execute
	virtual bool  ReadActionParameters(image * );
	//Execute action (code depends on action type)
	virtual void Execute();

	virtual void Undo();
	virtual void Redo();

};

