#include "Cut.h"
#include"Delete.h"
Cut::Cut(ApplicationManager* pApp) :Action(pApp)
{

}

Cut::Cut(ApplicationManager* pApp, Component* C, bool pushToUndo) : Action(pApp)
{
	theComponent = C;
}

bool Cut::ReadActionParameters(image *)
{
	if (theComponent == NULL)
		return false;
	return true;
}
void Cut::Execute()
{
	if (this->ReadActionParameters())
	{
		ptrfordeletegate = new Delete(pManager, pManager->PastedComponent, true);
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

}