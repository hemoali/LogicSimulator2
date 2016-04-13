#pragma once
#include "Action.h"
class Select :
	public Action
{
public:
	Select(ApplicationManager *pApp);
	virtual ~Select();

	//Reads parameters required for action to execute
	virtual bool  ReadActionParameters(image * );
	//Execute action (code depends on action type)
	virtual void Execute();

	virtual void Undo();
	virtual void Redo();

};

