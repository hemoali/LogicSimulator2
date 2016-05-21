#ifndef PASTE_Header
#define PASTE_Header
#include "Action.h"
class Paste :
	public Action
{
	Component*pastedcomponent;
	Component*theComponent;
	Action*ptrfordeletegate;
	bool pushToUndo;
public:
	Paste(ApplicationManager *pApp);
	Paste(ApplicationManager* pApp, Component* C, bool pushToUndo);
	virtual ~Paste();

	//Reads parameters required for action to execute
	virtual bool ReadActionParameters(image *, Component* c);
	//Execute action (code depends on action type)
	virtual void Execute();
	virtual void Undo();
	virtual void Redo();

};

#endif
