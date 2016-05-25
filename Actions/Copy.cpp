#include "Copy.h"
Copy::Copy(ApplicationManager* pApp) :Action(pApp)
{

}

Copy::Copy(ApplicationManager* pApp, Component* C, bool pushToUndo) : Action(pApp)
{
	theComponent = C;
}

bool Copy::ReadActionParameters(image *, Component* c)
{
	if (theComponent == NULL)
		return false;
	return true;
}
void Copy::Execute()
{
	pManager->setActionType(COPYAction); 
	pManager->setPastedComponent(theComponent);
}
void Copy::Redo()
{

}
void Copy::Undo()
{

}
Copy::~Copy()
{

}