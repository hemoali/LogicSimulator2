#include "ModifyConnection.h"
#include"..\Components\Component.h"
#include"..\ApplicationManager.h"
#include"..\Components\Connection.h"
#include"..\Components\SWITCH.h"
#include"..\Components\LED.h"
#include <iostream>
ModifyConnection::ModifyConnection(ApplicationManager*pApp) : Action(pApp)
{

}
ModifyConnection::ModifyConnection(ApplicationManager*pApp, Connection* c) : Action(pApp), theConnection(c)
{

}
bool ModifyConnection::validateOutputComponent(Component* comp, Component* dstComp) {
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
bool ModifyConnection::ReadActionParameters(image *I)
{
	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();

	pOut->PrintMsg("Connection : Click to select the Source");
	pIn->GetPointClicked(Cx1, Cy1);
	pOut->PrintMsg("Connection : Click to select the Destination");
	pIn->GetPointClicked(Cx2, Cy2);
	//Clear Status Bar
	pOut->ClearStatusBar();

	return true;
}
void ModifyConnection::Execute()
{
	Output* pOut = pManager->GetOutput();
	vector<Connection*> connection; connection.push_back(theConnection);
	pOut->clearConnections(connection, -1, -1, false, false);
	//remove conenctoin from pins
	theConnection->getSourcePin()->removeConnectedConnection(theConnection);
	theConnection->getDestPin()->setConnection(NULL);

	bool isCorrectNewConnection = false;
	while (!isCorrectNewConnection) {

		ReadActionParameters(NULL);

		Component* outputComponent = NULL;
		Component*inputComponent = NULL;

		int numOfInputs = 0;
		int indxOfInputComponent;

		for (int i = 0; i < pManager->allComponentsCorners.size(); i++)
		{
			if (dynamic_cast<Connection*>(pManager->getComponent(i)) || pManager->getComponent(i)->getDelete())
				continue;
			if (Cx1 >= pManager->allComponentsCorners[i].x1&&Cx1 <= pManager->allComponentsCorners[i].x2&& Cy1 >= pManager->allComponentsCorners[i].y1&&Cy1 <= pManager->allComponentsCorners[i].y2)
			{
				if (Cx1 > (pManager->allComponentsCorners[i].x1 + UI.GATE_Width / 2))
				{
					outputComponent = pManager->getComponent(i);
				}
			}
			if (Cx2 >= pManager->allComponentsCorners[i].x1&&Cx2 <= pManager->allComponentsCorners[i].x2&& Cy2 >= pManager->allComponentsCorners[i].y1&&Cy2 <= pManager->allComponentsCorners[i].y2)
			{
				if (Cx2 < (pManager->allComponentsCorners[i].x1 + UI.GATE_Width / 2))
				{
					inputComponent = pManager->getComponent(i);
					indxOfInputComponent = i;
				}
			}
		}

		if (inputComponent == NULL || outputComponent == NULL || inputComponent == outputComponent || dynamic_cast<SWITCH*> (inputComponent) || dynamic_cast<LED*> (outputComponent))
		{
			pManager->GetOutput()->PrintMsg("Invalid Connection", UI.ErrorColor);
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
					pManager->GetOutput()->PrintMsg("Feedback isn't allowed", UI.ErrorColor);
					Sleep(600);
					isCorrectNewConnection = false;
					goto end;
					break;
				}
			}
			numOfInputs = inputComponent->getNumOfInputs();
			int inputPin;
			if (numOfInputs == 3)
			{
				if (Cy2 <= pManager->allComponentsCorners[indxOfInputComponent].y2 - UI.GATE_Height / 2 - 6)numOfInputs = inputPin = 0;
				else if (Cy2 >= pManager->allComponentsCorners[indxOfInputComponent].y2 - UI.GATE_Height / 2 + 6)numOfInputs = inputPin = 2;
				else numOfInputs = inputPin = 1;
			}
			else if (numOfInputs == 2)
			{
				if (Cy2 <= pManager->allComponentsCorners[indxOfInputComponent].y2 - UI.GATE_Height / 2)numOfInputs = inputPin = 0;
				else if (Cy2 >= pManager->allComponentsCorners[indxOfInputComponent].y2 - UI.GATE_Height / 2) {
					numOfInputs = 2; inputPin = 1;
				}
			}
			else { numOfInputs = 1; inputPin = 0; }

			if (inputComponent->getInputPin(inputPin)->getConnection() != NULL || outputComponent->getOutputPin()->connectedConnectionsCount() == FANOUT)
			{
				pManager->GetOutput()->PrintMsg("Invalid Connection", UI.ErrorColor);
				Sleep(100);
			}
			else {
				GraphicsInfo GInfo;
				GInfo.x1 = outputComponent->getCenterLocation().x1 + UI.GATE_Width / 2 - 2;
				GInfo.y1 = outputComponent->getCenterLocation().y1;
				GInfo.x2 = inputComponent->getCenterLocation().x1 - UI.GATE_Width / 2 - 2;

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
				vector<Cell> cellsBeforeAddingConnection;
				if (pManager->GetOutput()->DrawConnection(GInfo, numOfInputs, inputComponent->getCenterLocation(), cellsBeforeAddingConnection) && !(outputComponent->getOutputPin()->connectedConnectionsCount() == FANOUT))
				{
					theConnection->setCornersLocation(GInfo);
					theConnection->setSourcePin(outputComponent->getOutputPin());
					theConnection->setDestPin(inputComponent->getInputPin(inputPin));
					for (size_t i = 0; i < pManager->allComponentsCorners.size(); i++)
					{
						if (dynamic_cast<Connection*>(pManager->getComponent(i)) && pManager->getComponent(i) == theConnection) {
							pManager->allComponentsCorners[i].x1 = GInfo.x1;
							pManager->allComponentsCorners[i].x2 = GInfo.x2;
							pManager->allComponentsCorners[i].y1 = GInfo.y1;
							pManager->allComponentsCorners[i].y2 = GInfo.y2;
						}
					}
					theConnection->setCellsBeforeAddingConnection(cellsBeforeAddingConnection);
					outputComponent->getOutputPin()->ConnectTo(theConnection);
					inputComponent->getInputPin(inputPin)->setConnection(theConnection);
					inputComponent->getInputPin(inputPin)->setPosition(numOfInputs);
					theConnection->setIsDrawn(true);

					for (size_t i = 0; i < cellsBeforeAddingConnection.size(); i++)
					{
						pManager->GetOutput()->setArrayOfComponents(cellsBeforeAddingConnection[i].y, cellsBeforeAddingConnection[i].x, theConnection);
					}
					isCorrectNewConnection = true;
				}
				else {
					pManager->GetOutput()->PrintMsg("No Available Connection"); 
					isCorrectNewConnection = false;
				}
			}
		}
	end: if (outputComponent != NULL)
	{
		// Remove red pin
		outputComponent->setDelete(true);
		outputComponent->Draw(pManager->GetOutput(), false);
		outputComponent->setDelete(false);
		outputComponent->Draw(pManager->GetOutput(), false);
	}
	}
}
void ModifyConnection::Undo()
{

}
void ModifyConnection::Redo()
{

}
ModifyConnection::~ModifyConnection()
{
}
