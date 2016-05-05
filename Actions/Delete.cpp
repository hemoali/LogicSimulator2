#include "Delete.h"



Delete::Delete(ApplicationManager* pApp):Action(pApp)
{

}

Delete::Delete(ApplicationManager* pApp,Component* C) : Action(pApp)
{
	theComponent = C;
}

bool Delete::ReadActionParameters(image *)
{
	if( theComponent == NULL ) 
		return false;
	return true;
}

void Delete::Execute()
{
	Output* pOut = pManager->GetOutput();
	if (this->ReadActionParameters()) {
		if (theComponent != NULL && dynamic_cast<Gate*> (theComponent))
			pOut->DeleteGate(theComponent);
		else if (theComponent != NULL && dynamic_cast<Connection*> (theComponent))
		{
			vector<Connection*> connection;
			connection.push_back((Connection*)theComponent);
			pOut->clearConnections(connection, -1, -1, false, true);
		}
	}
	else {
		pOut->PrintMsg("It's not deleted.. Error");
	}
}

void Delete::Undo()
{
	Output* pOut = pManager->GetOutput();
	if (theComponent != NULL && dynamic_cast<Gate*> (theComponent))
		pOut->DeleteGate(theComponent, true);
	else if (theComponent != NULL && dynamic_cast<Connection*> (theComponent))
	{
		//Undo Deleting Connection
	}
}

void Delete::Redo()
{
	Output* pOut = pManager->GetOutput();
	if (this->ReadActionParameters()) {
		if (theComponent != NULL && dynamic_cast<Gate*> (theComponent))
			pOut->DeleteGate(theComponent);
		else if (theComponent != NULL && dynamic_cast<Connection*> (theComponent))
		{
			//Deleting Connection

		}
	}
}

Delete::~Delete()
{
}
