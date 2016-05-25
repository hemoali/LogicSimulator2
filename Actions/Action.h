#ifndef _ACTION_H
#define _ACTION_H
#include "..\ApplicationManager.h" 
class Action
{
protected:
	ApplicationManager *pManager;	//Actions needs AppMngr to do their job
	string gateLabel;
	GraphicsInfo GInfo;

	//Debugging For the deleted
	int theId;
	//For undo/redo
	Component* pA;
public:
	static int ID;
	static int IDD;
	Action(ApplicationManager *pApp) { pManager = pApp;
	theId = ID++;
	}	//constructor
	
	//Reads parameters required for action to execute
	virtual bool ReadActionParameters(image *, Component*)=0;
	
	//Execute action (code depends on action type)
	virtual void Execute()=0;

	//To undo this action (code depends on action type)
	virtual void Undo();

	//To redo this action (code depends on action type)
	virtual void Redo();

	virtual ~Action() {
		IDD++;
	}
};

#endif