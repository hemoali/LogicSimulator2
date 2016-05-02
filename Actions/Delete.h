#pragma once
#include "Action.h"
#include "../Components/Connection.h"
#include "../Components/Gate.h"
class Delete :
	public Action
{
	Component *theComponent;
public:
	Delete(ApplicationManager* pApp);
	Delete(ApplicationManager * pApp, Component * C );

	//Reads parameters required for action to execute
	virtual bool ReadActionParameters(image * = NULL);

	//Execute action (code depends on action type)
	virtual void Execute();

	//To undo this action (code depends on action type)
	virtual void Undo();

	//To redo this action (code depends on action type)
	virtual void Redo();

	~Delete();
};

