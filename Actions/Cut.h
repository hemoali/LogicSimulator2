#ifndef CUT_Header
#define CUT_Header
#include "Action.h"
class Cut :
	public Action
{
	Action*ptrfordeletegate;
	Component*theComponent;
	bool pushToUndo;
public:
	Cut(ApplicationManager *pApp);
	Cut(ApplicationManager* pApp, Component* C, bool pushToUndo=true);
	virtual ~Cut();
	//Reads parameters required for action to execute
	virtual bool  ReadActionParameters(image *, Component* c);
	//Execute action (code depends on action type)
	virtual void Execute();

	virtual void Undo();
	virtual void Redo();

};

#endif
