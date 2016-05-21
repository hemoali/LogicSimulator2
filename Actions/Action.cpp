#include "Action.h"
#include "..\Components\Component.h"
int Action::ID = 0;
int Action::IDD = 0;
void Action::Undo()
{
	pA->setDelete(true);
	pA->Draw(pManager->GetOutput(), false);
}

void Action::Redo()
{
	pA->setDelete(false);
	pA->Draw(pManager->GetOutput(), false);	
}