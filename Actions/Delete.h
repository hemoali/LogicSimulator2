#pragma once
#include "Action.h"
#include "../Components/Connection.h"
#include "../Components/Gate.h"
#include "../Components/LED.h"
#include "../Components/SWITCH.h"
class Delete :
	public Action
{
	Component *theComponent;
	vector<Connection*> allInputConnections, allOutputConnections;
	bool pushToUndo;
public:
	Delete(ApplicationManager* pApp);
	Delete(ApplicationManager * pApp, Component * C, bool pushToUndo = true);

	//Reads parameters required for action to execute
	virtual bool ReadActionParameters(image *, Component* c);
	void Delete::reconnectConenction(Connection* conn);
	//Execute action (code depends on action type)
	virtual void Execute();

	//To undo this action (code depends on action type)
	virtual void Undo();
	void Undo(int c);
	//To redo this action (code depends on action type)
	virtual void Redo();

	~Delete();
};

