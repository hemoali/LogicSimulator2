#include "Action.h"
#include "..\Components\Component.h"
int Action::ID = 0; // counter for constructed actions
int Action::IDD = 0; // counter for destructed  actions
void Action::Undo()
{
	// Set component as deleted and draw the clean image
	pA->setDelete(true);
	pA->Draw(pManager->GetOutput(), false);
}

void Action::Redo()
{
	// Set component as not deleted and draw the component
	pA->setDelete(false);
	pA->Draw(pManager->GetOutput(), false);	
}