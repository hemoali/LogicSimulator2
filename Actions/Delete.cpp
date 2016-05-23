#include "Delete.h"
Delete::Delete(ApplicationManager* pApp) :Action(pApp)
{

}

Delete::Delete(ApplicationManager* pApp, Component* C, bool pushToUndo) : Action(pApp)
{
	theComponent = C;
	this->pushToUndo = pushToUndo;
}

bool Delete::ReadActionParameters(image *, Component* c)
{
	if (theComponent == NULL)
		return false;
	return true;
}

void Delete::Execute()
{
	Output* pOut = pManager->GetOutput();
	if (this->ReadActionParameters(NULL, NULL)) {
		if (theComponent != NULL && (dynamic_cast<Gate*> (theComponent) || dynamic_cast<LED*>(theComponent) || dynamic_cast<SWITCH*> (theComponent))) {
			Output *pOut = pManager->GetOutput();
			string s = "Gate: " + (theComponent->getLabel()) + " has been deleted successfully";
			pOut->PrintStatusBox(s);
			theComponent->setDelete(true);
			theComponent->Draw(pOut);

			// Removing Connection
			vector<Connection*> allInConnections, allOutConnections;
			theComponent->getAllInputConnections(allInConnections);
			theComponent->getAllOutputConnections(allOutConnections);

			theComponent->getAllInputConnections(allInputConnections);
			theComponent->getAllOutputConnections(allOutputConnections);

			pOut->clearConnections(allInConnections, theComponent->getCenterLocation().x1, theComponent->getCenterLocation().y1, true, true);
			pOut->clearConnections(allOutConnections, theComponent->getCenterLocation().x1, theComponent->getCenterLocation().y1, false, true);
		}
		else if (theComponent != NULL && dynamic_cast<Connection*> (theComponent))
		{
			vector<Connection*> connection;
			connection.push_back((Connection*)theComponent);
			pOut->clearConnections(connection, -1, -1, false, true);
		}
		if (pushToUndo)
		{
			Utils::undoActions.push(this);
			Action::pA = theComponent;
		}
	}
	else {
		pOut->PrintStatusBox("It's not deleted.. Error");
	}
}
void Delete::reconnectConenction(Connection* conn) {
	conn->setDelete(false);
	conn->getDestPin()->setConnection(conn);
	conn->getSourcePin()->ConnectTo(conn);
	conn->setIsDrawn(true);

	Component* outputComponent = conn->getSourcePin()->getComponent();
	Component* inputComponent = conn->getDestPin()->getComponent();

	GraphicsInfo GInfo;
	GInfo.x1 = outputComponent->getCenterLocation().x1 + UI.GATE_Width / 2 - 2;
	GInfo.y1 = outputComponent->getCenterLocation().y1;
	GInfo.x2 = inputComponent->getCenterLocation().x1 - UI.GATE_Width / 2 - 2;
	int numOfInputs = conn->getDestPin()->getPosition();
	if (numOfInputs == 0)
	{
		GInfo.y2 = inputComponent->getCenterLocation().y1 - UI.GATE_Height / 2 + 13;
	}
	else if (numOfInputs == 1) {
		GInfo.y2 = inputComponent->getCenterLocation().y1;
	}
	else {
		GInfo.y2 = inputComponent->getCenterLocation().y1 + UI.GATE_Height / 2 - 2;
	}

	pManager->GetOutput()->DrawConnection(GInfo, numOfInputs, inputComponent->getCenterLocation(), conn->getCellsBeforeAddingConnection(), false, conn);

}
void Delete::Undo()
{
	Output* pOut = pManager->GetOutput();

	if (theComponent != NULL && (dynamic_cast<Gate*> (theComponent) || dynamic_cast<LED*>(theComponent) || dynamic_cast<SWITCH*> (theComponent))) {
		theComponent->setDelete(false);
		theComponent->Draw(pOut);
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
	}
	else if (theComponent != NULL && dynamic_cast<Connection*> (theComponent))
	{
		if (!((Connection*)theComponent)->getIsDrawn())
		{
			reconnectConenction((Connection*)theComponent);
		}
	}
}
void Delete::Undo(int c)
{
	Output* pOut = pManager->GetOutput();
	if (c == 0) // re draw gates
	{
		if (theComponent != NULL && (dynamic_cast<Gate*> (theComponent) || dynamic_cast<LED*>(theComponent) || dynamic_cast<SWITCH*> (theComponent))) {
			theComponent->setDelete(false);
			theComponent->Draw(pOut);
		}
	}
	// reconnect connections
	if (c == 1) {
		if (theComponent != NULL && (dynamic_cast<Gate*> (theComponent) || dynamic_cast<LED*>(theComponent) || dynamic_cast<SWITCH*> (theComponent))) {
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
		}
		else if (theComponent != NULL && dynamic_cast<Connection*> (theComponent))
		{
			if (!((Connection*)theComponent)->getIsDrawn())
			{
				reconnectConenction((Connection*)theComponent);
			}
		}
	}
}
void Delete::Redo()
{
	Output *pOut = pManager->GetOutput();

	if (theComponent != NULL && (dynamic_cast<Gate*> (theComponent) || dynamic_cast<LED*>(theComponent) || dynamic_cast<SWITCH*> (theComponent))) {
		theComponent->setDelete(true);
		theComponent->Draw(pOut);

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

Delete::~Delete()
{
}
