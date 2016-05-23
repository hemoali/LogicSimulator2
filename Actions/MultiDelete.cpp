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
	Output* pOut = pManager->GetOutput();
	for (size_t i = 0; i <  theVector.size(); i++)
	{
		Component* theComponent = theVector[i].second;
		if ((dynamic_cast<Gate*> (theComponent) || dynamic_cast<LED*>(theComponent) || dynamic_cast<SWITCH*> (theComponent)))
		{
			theComponent->setDelete(false);
			theComponent->Draw(pOut);
		}
	}
	for (size_t i = 0; i < theVector.size(); i++)
	{
		Component* theComponent = theVector[i].second;
		vector<Connection*> allInputConnections, allOutputConnections;
		theComponent->getAllInputConnections(allInputConnections);
		theComponent->getAllOutputConnections(allOutputConnections);

		if ((dynamic_cast<Gate*> (theComponent) || dynamic_cast<LED*>(theComponent) || dynamic_cast<SWITCH*> (theComponent)))
		{

			// reconnect Connection
			for (size_t i = 0; i < allInputConnections.size(); i++)
			{
				if (allInputConnections[i]->getIsDrawn())
				{
					continue;
				}
				reconnectConenction(allInputConnections[i]);
			}

			for (size_t i = 0; i < allOutputConnections.size(); i++)
			{
				if (allOutputConnections[i]->getIsDrawn())
				{
					continue;
				}
				reconnectConenction(allOutputConnections[i]);
			}
		}else if (dynamic_cast<Connection*> (theComponent)){
			if (!((Connection*)theComponent)->getIsDrawn())
			{
				reconnectConenction((Connection*)theComponent);
			}
		}
	}
}

void MultiDelete::Redo()
{
	Output* pOut = pManager->GetOutput();
	for (size_t i = 0; i < theVector.size(); i++)
	{
		Component* theComponent = theVector[i].second;
		if ((dynamic_cast<Gate*> (theComponent) || dynamic_cast<LED*>(theComponent) || dynamic_cast<SWITCH*> (theComponent)))
		{
			string s = "Gate: " + (theComponent->getLabel()) + " has been deleted successfully";
			pOut->PrintStatusBox(s);
			theComponent->setDelete(true);
			theComponent->Draw(pOut);
		}
	}
	for (size_t i = 0; i < theVector.size(); i++)
	{
		Component* theComponent = theVector[i].second;
		if (theComponent != NULL && (dynamic_cast<Gate*> (theComponent) || dynamic_cast<LED*>(theComponent) || dynamic_cast<SWITCH*> (theComponent))) {
			// Removing Connection
			vector<Connection*> allInConnections, allOutConnections;
			theComponent->getAllInputConnections(allInConnections);
			theComponent->getAllOutputConnections(allOutConnections);

			pOut->clearConnections(allInConnections, theComponent->getCenterLocation().x1, theComponent->getCenterLocation().y1, true, true);
			pOut->clearConnections(allOutConnections, theComponent->getCenterLocation().x1, theComponent->getCenterLocation().y1, false, true);
		}
		else if (theComponent != NULL && dynamic_cast<Connection*> (theComponent))
		{
			vector<Connection*> connection;
			connection.push_back((Connection*)theComponent);
			pOut->clearConnections(connection, -1, -1, false, true);
		}
	}
}
