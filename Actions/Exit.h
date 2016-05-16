#pragma once
#include "Action.h"
#include "../Actions/Save.h"
#include "../Actions/New.h"
class Exit :
	public Action
{
	int choice;
public:
	Exit(ApplicationManager* pApp);
	virtual ~Exit();
	
	//Reads parameters required for action to execute
	virtual bool  ReadActionParameters(image * = NULL);
	//Execute action (code depends on action type)
	virtual void Execute();

	virtual void Undo();
	virtual void Redo();
};

