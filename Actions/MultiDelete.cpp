#include "MultiDelete.h"



MultiDelete::MultiDelete(ApplicationManager* pApp):Delete(pApp)
{
}

MultiDelete::MultiDelete(ApplicationManager * pApp, vector<pair<int, Component*>> V):Delete(pApp)
{
	theVector = V;
}


MultiDelete::~MultiDelete()
{
	for (size_t i = 0; i < allDeleteActions.size(); i++)
	{
		delete allDeleteActions[i];
	}
}

bool MultiDelete::ReadActionParameters(image *, Component* c)
{
	if( theVector.size() == 0) 
		return false;
	return true;
}

void MultiDelete::Execute()
{
	Output* pOut = pManager->GetOutput();
	if (this->ReadActionParameters(NULL, NULL)) {
		for (int i = 0; i < theVector.size(); i++) {
			Action* act = new Delete(pManager, theVector[i].second, false);
			allDeleteActions.push_back(act);
			act->Execute();
		}
		Utils::undoActions.push(this);
	}
	else {
		pOut->PrintStatusBox("Error deleting the selected gate(s) ");
	}
}

void MultiDelete::Undo()
{
	for (size_t i = 0; i < allDeleteActions.size(); i++)
	{
		allDeleteActions[i]->Undo();
	}
}

void MultiDelete::Redo()
{
	for (size_t i = 0; i < allDeleteActions.size(); i++)
	{
		allDeleteActions[i]->Redo();
	}
}
