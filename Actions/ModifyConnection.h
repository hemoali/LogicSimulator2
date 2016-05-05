#pragma once
#include "Action.h"
class ModifyConnection :
	public Action
{
	Connection *theConnection; // The Selected Connection
public:
	ModifyConnection(ApplicationManager* pApp);

	ModifyConnection(ApplicationManager* pApp, Connection* C);
	
	~ModifyConnection();

	//Reads parameters required for action to execute
	virtual bool ReadActionParameters(image * = NULL);

	//Execute action (code depends on action type)
	virtual void Execute();

	//To undo this action (code depends on action type)
	virtual void Undo();

	//To redo this action (code depends on action type)
	virtual void Redo();
};

