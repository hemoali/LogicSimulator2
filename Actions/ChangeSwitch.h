#pragma once
#include "Action.h"
#include "../Components/Connection.h"
#include "../Components/SWITCH.h"
class ChangeSwitch :
	public Action
{
	SWITCH *theComponent;
	int oldStatus, newStatus;
public:
	ChangeSwitch(ApplicationManager* pApp);
	ChangeSwitch(ApplicationManager * pApp, SWITCH * C);

	//Reads parameters required for action to execute
	virtual bool ReadActionParameters(image * , Component* c);
	//Execute action (code depends on action type)
	virtual void Execute();

	//To undo this action (code depends on action type)
	virtual void Undo();

	//To redo this action (code depends on action type)
	virtual void Redo();

	~ChangeSwitch();
};

