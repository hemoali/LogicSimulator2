#ifndef COPY_Header
#define COPY_Header
#include "Action.h"
class Copy:
	public Action
{
	Component*theComponent;
	bool pushToUndo;
public:
	Copy(ApplicationManager *pApp);
	Copy(ApplicationManager* pApp, Component* C, bool pushToUndo=true);
	virtual ~Copy();

	//Reads parameters required for action to execute
	virtual bool  ReadActionParameters(image *, Component* c);
	//Execute action (code depends on action type)
	virtual void Execute();

	virtual void Undo();
	virtual void Redo();

};

#endif
