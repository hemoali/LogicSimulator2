#pragma once
#include"..\Components\OR2.h"
#include"..\ApplicationManager.h"
class AddORgate2:public Action
{
private:
	 string gateLabel;
	GraphicsInfo GInfo;
public:
	AddORgate2(ApplicationManager *pApp);
	virtual ~AddORgate2(void);

	//Reads parameters required for action to execute
	virtual bool  ReadActionParameters(image * );
	//Execute action (code depends on action type)
	virtual void Execute();

	virtual void Undo();
	virtual void Redo();

};

