#pragma once
#include "Delete.h"
class MultiDelete :
	public Delete
{
	vector < pair<int, Component*> > theVector;
	vector<Action*> allDeleteActions;
public:
	MultiDelete(ApplicationManager* pApp);
	MultiDelete(ApplicationManager* pApp, vector<pair<int, Component*>> V);

	~MultiDelete();

	//Reads parameters required for action to execute
	virtual bool ReadActionParameters(image * = NULL) ;

	//Execute action (code depends on action type)
	virtual void Execute();

	//To undo this action (code depends on action type)
	virtual void Undo();

	//To redo this action (code depends on action type)
	virtual void Redo();
};

