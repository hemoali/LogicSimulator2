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
}

bool MultiDelete::ReadActionParameters(image *)
{
	if( theVector.size() == 0) 
		return false;
	return true;
}

void MultiDelete::Execute()
{
	Output* pOut = pManager->GetOutput();
	if (this->ReadActionParameters()) {
		for (int i = 0; i < theVector.size(); i++) {
			if (!dynamic_cast<Connection*> (theVector[i].second))
			{
				pOut->DeleteGate(theVector[i].second);
			}
			else {
				vector<Connection*> connection;
				connection.push_back((Connection*)theVector[i].second);
				pOut->clearConnections(connection, -1, -1, false, true);
			}
		}
	}
	else {
		pOut->PrintMsg("Error deleting the selected gate(s) ");
	}
}

void MultiDelete::Undo()
{
}

void MultiDelete::Redo()
{
}
