#ifndef _ACTION_H
#define _ACTION_H
#include "..\ApplicationManager.h" 
class Action
{
protected:
	ApplicationManager *pManager;	//Actions needs AppMngr to do their job
	string gateLabel;
	GraphicsInfo GInfo;
public:
	Action(ApplicationManager *pApp) { pManager = pApp; }	//constructor
	
	//Reads parameters required for action to execute
	virtual bool ReadActionParameters(image *)=0;
	
	//Execute action (code depends on action type)
	virtual void Execute()=0;

	//To undo this action (code depends on action type)
	virtual void Undo()=0;

	//To redo this action (code depends on action type)
	virtual void Redo()=0;

};

#endif