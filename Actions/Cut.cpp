#include "Cut.h"
#include"Delete.h"
Cut::Cut(ApplicationManager* pApp) :Action(pApp)
{

}

Cut::Cut(ApplicationManager* pApp, Component* C, bool pushToUndo) : Action(pApp)
{
	theComponent = C;
}

bool Cut::ReadActionParameters(image *, Component* c)
{
	if (theComponent == NULL)
		return false;
	return true;
}
void Cut::Execute()
{
	if (this->ReadActionParameters(NULL, NULL))
	{
		pManager->setActionType(CUTAction);
		pManager->setPastedComponent(theComponent);
		ptrfordeletegate = new Delete(pManager, pManager->getPastedComponent(), true);
		ptrfordeletegate->Execute();
	}
}
void Cut::Redo()
{

}
void Cut::Undo()
{

}
Cut::~Cut()
{
	delete ptrfordeletegate;
}