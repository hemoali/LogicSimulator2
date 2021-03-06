#include "ModifyConnection.h"
#include"..\Components\Component.h"
#include"..\ApplicationManager.h"
#include"..\Components\Connection.h"
#include"..\Components\SWITCH.h"
#include"..\Components\LED.h"
#include <iostream>

//=======================================================================================================================
//							Check adding connection action for more comments for the same code
//=======================================================================================================================
ModifyConnection::ModifyConnection(ApplicationManager*pApp) : Action(pApp)
{

}
ModifyConnection::ModifyConnection(ApplicationManager*pApp, Connection* c) : Action(pApp), theConnection(c)
{

}
bool ModifyConnection::validateOutputComponent(Component* comp, Component* dstComp) { // check for feedback
	if (comp == NULL)
	{
		return true;
	}
	if (comp == dstComp) {
		return false;
	}
	bool t[3] = { true, true, true };
	for (size_t j = 0; j < comp->getOutputPin()->connectedConnectionsCount(); j++)
	{
		t[j] = validateOutputComponent(comp->getOutputPin()->getConnection(j)->getDestPin()->getComponent(), dstComp);
	}
	return t[0] && t[1] && t[2];
}
bool ModifyConnection::ReadActionParameters(image *, Component* c)
{
	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();

	pOut->PrintStatusBox("Connection : Click to select the Source");
	pIn->GetPointClicked(Cx1, Cy1);
	pOut->PrintStatusBox("Connection : Click to select the Destination");
	pIn->GetPointClicked(Cx2, Cy2);
	

	return true;
}
void ModifyConnection::Execute()
{

	for (size_t i = 0; i < theConnection->getCellsBeforeAddingConnection().size(); i++)
	{
		oldCells.push_back(theConnection->getCellsBeforeAddingConnection()[i]);
	}

	Output* pOut = pManager->GetOutput();
	vector<Connection*> connection; connection.push_back(theConnection);
	pOut->clearConnections(connection, -1, -1, false, false); // clear old connection
	//remove connection from old pins
	theConnection->getSourcePin()->removeConnectedConnection(theConnection);
	theConnection->getDestPin()->setConnection(NULL);

	bool isCorrectNewConnection = false;
	while (!isCorrectNewConnection) {

		ReadActionParameters(NULL, NULL);
		int numOfInputs = 0;
		int indxOfInputComponent;
		int ii;
		outputComponent = pManager->getComponentByCoordinates(Cx1, Cy1, true, true, ii);
		inputComponent = pManager->getComponentByCoordinates(Cx2, Cy2, true, true, ii);

		if (inputComponent == NULL || outputComponent == NULL || inputComponent == outputComponent || dynamic_cast<SWITCH*> (inputComponent) || dynamic_cast<LED*> (outputComponent))
		{
			pManager->GetOutput()->PrintStatusBox("Invalid Connection", UI.ErrorColor);
			Sleep(600);
			isCorrectNewConnection = false;
		}
		else {
			//Check for feedback
			bool isValidRegardingFeedback = true;
			for (size_t i = 0; i < inputComponent->getOutputPin()->connectedConnectionsCount(); i++)
			{
				isValidRegardingFeedback = isValidRegardingFeedback && validateOutputComponent(inputComponent->getOutputPin()->getConnection(i)->getDestPin()->getComponent(), outputComponent);

				if (!isValidRegardingFeedback)
				{
					pManager->GetOutput()->PrintStatusBox("Feedback isn't allowed", UI.ErrorColor);
					Sleep(600);
					isCorrectNewConnection = false;
					goto end;
					break;
				}
			}
			numOfInputs = inputComponent->getNumOfInputs();
			if (numOfInputs == 3)
			{
				if (Cy2 <= inputComponent->getCornersLocation().y2 - UI.GATE_Height / 2 - 6)numOfInputs = inputPin = 0;
				else if (Cy2 >= inputComponent->getCornersLocation().y2 - UI.GATE_Height / 2 + 6)numOfInputs = inputPin = 2;
				else numOfInputs = inputPin = 1;
			}
			else if (numOfInputs == 2)
			{
				if (Cy2 <= inputComponent->getCornersLocation().y2 - UI.GATE_Height / 2)numOfInputs = inputPin = 0;
				else if (Cy2 >= inputComponent->getCornersLocation().y2 - UI.GATE_Height / 2) {
					numOfInputs = 2; inputPin = 1;
				}
			}
			else { numOfInputs = 1; inputPin = 0; }

			if (inputComponent->getInputPin(inputPin)->getConnection() != NULL || outputComponent->getOutputPin()->connectedConnectionsCount() == FANOUT)
			{
				pManager->GetOutput()->PrintStatusBox("Invalid Connection", UI.ErrorColor);
				Sleep(100);
			}
			else {
				//Save old graphics info
				oldGInfo.x1 = theConnection->getCornersLocation().x1;
				oldGInfo.y1 = theConnection->getCornersLocation().y1;
				oldGInfo.x2 = theConnection->getCornersLocation().x2;
				oldGInfo.y2 = theConnection->getCornersLocation().y2;
				oldInputComponent = theConnection->getDestPin()->getComponent();
				oldOutputComponent = theConnection->getSourcePin()->getComponent();
				oldInputPin = theConnection->getDestPin();
				oldOutputPin = theConnection->getSourcePin();
				oldInputPinPosition = theConnection->getDestPin()->getPosition();
				//New Data
				GInfo.x1 = outputComponent->getCenterLocation().x1 + UI.GATE_Width / 2 - 2;
				GInfo.y1 = outputComponent->getCenterLocation().y1;
				GInfo.x2 = inputComponent->getCenterLocation().x1 - UI.GATE_Width / 2 - 2;
				vector<Cell> cellsBeforeAddingConnection;
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
				if (pManager->GetOutput()->DrawConnection(GInfo, numOfInputs, inputComponent->getCenterLocation(), cellsBeforeAddingConnection, false, theConnection) && !(outputComponent->getOutputPin()->connectedConnectionsCount() == FANOUT))
				{
					theConnection->setCornersLocation(GInfo);

					theConnection->setSourcePin(newOutputPin = outputComponent->getOutputPin());
					theConnection->setDestPin(newInputPin = inputComponent->getInputPin(inputPin));
					theConnection->setCellsBeforeAddingConnection(cellsBeforeAddingConnection);
					outputComponent->getOutputPin()->ConnectTo(theConnection);
					inputComponent->getInputPin(inputPin)->setConnection(theConnection);
					inputComponent->getInputPin(inputPin)->setPosition(numOfInputs);
					theConnection->setIsDrawn(true);
					for (size_t i = 0; i < cellsBeforeAddingConnection.size(); i++)
					{
						newCells.push_back(cellsBeforeAddingConnection[i]);
					}
					isCorrectNewConnection = true;
					Utils::undoActions.push(this);
				}
				else {
					pManager->GetOutput()->PrintStatusBox("No Available Connection");
					isCorrectNewConnection = false;
				}
			}
		}
	end: if (outputComponent != NULL)
	{
		// Remove red pin
		outputComponent->refresh(pOut);
	}
	}
}
void ModifyConnection::Undo()
{
	Output* pOut = pManager->GetOutput();
	vector<Connection*> connection; connection.push_back(theConnection);
	pOut->clearConnections(connection, -1, -1, false, false);
	//remove conenctoin from pins
	theConnection->getSourcePin()->removeConnectedConnection(theConnection);
	theConnection->getDestPin()->setConnection(NULL);

	theConnection->setCornersLocation(oldGInfo);
	theConnection->setSourcePin(oldOutputPin);
	theConnection->setDestPin(oldInputPin);
	theConnection->setCellsBeforeAddingConnection(oldCells);
	oldOutputComponent->getOutputPin()->ConnectTo(theConnection);
	oldInputComponent->getInputPin(oldInputPinPosition)->setConnection(theConnection);
	oldInputComponent->getInputPin(oldInputPinPosition)->setPosition(oldInputPinPosition);
	theConnection->setIsDrawn(true);
	pManager->GetOutput()->DrawConnection(oldGInfo, oldInputPinPosition, oldInputComponent->getCenterLocation(), oldCells, false, theConnection);
}
void ModifyConnection::Redo()
{
	Output* pOut = pManager->GetOutput();
	vector<Connection*> connection; connection.push_back(theConnection);
	pOut->clearConnections(connection, -1, -1, false, false);
	//remove conenctoin from pins
	theConnection->getSourcePin()->removeConnectedConnection(theConnection);
	theConnection->getDestPin()->setConnection(NULL);

	theConnection->setCornersLocation(GInfo);

	theConnection->setSourcePin(newOutputPin);
	theConnection->setDestPin(newInputPin);
	theConnection->setCellsBeforeAddingConnection(newCells);
	outputComponent->getOutputPin()->ConnectTo(theConnection);
	inputComponent->getInputPin(inputPin)->setConnection(theConnection);
	inputComponent->getInputPin(inputPin)->setPosition(inputPin);
	theConnection->setIsDrawn(true);

	pManager->GetOutput()->DrawConnection(GInfo, inputPin, inputComponent->getCenterLocation(), newCells, false, theConnection);

}
ModifyConnection::~ModifyConnection()
{
}
