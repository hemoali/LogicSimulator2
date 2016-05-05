#include "AddConnection.h"
#include <iostream>
AddConnection::AddConnection(ApplicationManager *pApp) :Action(pApp)
{
	Silent = false;
	int numOfInputs = 0;
}

AddConnection::~AddConnection(void)
{
}

bool AddConnection::ReadActionParameters(image * smallImageBeforeAddingComponent)
{
	//Get a Pointer to the Input / Output Interfaces
	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();
	if (!Silent) {
		//Print Action Message
		pIn->getConnectionStartPoint(Cx1, Cy1);
		pOut->PrintMsg("Connection : Click to select the Destination");
		pIn->GetPointClicked(Cx2, Cy2);
		pIn->GetPointClicked(Cx2, Cy2);

		//Clear Status Bar
		pOut->ClearStatusBar();
	}
	else {
		pIn->CorrectPointClickedSilent(Cx1, Cy1);
		pIn->CorrectPointClickedSilent(Cx2, Cy2);
	}
	return true;
}
bool AddConnection::validateOutputComponent(Component* comp, Component* dstComp) {
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
void AddConnection::Execute()
{
	//Get Center point of the Gate
	ReadActionParameters(NULL);

	Component* outputComponent = NULL;
	Component*inputComponent = NULL;

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
		Sleep(100);
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
				Connection *pA = new Connection(GInfo, outputComponent->getOutputPin(), inputComponent->getInputPin(inputPin));
				pManager->AddComponent(pA);
				pManager->allComponentsCorners.push_back(GInfo);
				pA->setCellsBeforeAddingConnection(cellsBeforeAddingConnection);
				outputComponent->getOutputPin()->ConnectTo(pA);
				inputComponent->getInputPin(inputPin)->setConnection(pA);
				inputComponent->getInputPin(inputPin)->setPosition(numOfInputs);
				if (!Silent) {
					string s = "Please enter connection label: ";
					pManager->GetOutput()->PrintMsg(s);
					gateLabel = pManager->GetInput()->GetSrting(pManager->GetOutput(), s);
				}
				pA->setLabel(gateLabel);
				pA->setIsDrawn(true);

				for (size_t i = 0; i < cellsBeforeAddingConnection.size(); i++)
				{
					pManager->GetOutput()->setArrayOfComponents(cellsBeforeAddingConnection[i].y, cellsBeforeAddingConnection[i].x, pA);
				}
				pManager->undoActions.push(this);
				Action::pA = pA;
			}
			else {
				pManager->GetOutput()->PrintMsg("No Available Connection");
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

void AddConnection::AddConnectionSilent(int c1, int c2, int c3, int c4, string s)
{
	Silent = true;
	Cx1 = c1;
	Cy1 = c2;
	Cx2 = c3;
	Cy2 = c4;
	gateLabel = s;
	this->Execute();
}


void AddConnection::Undo()
{
	vector<Connection*> connection;
	connection.push_back((Connection*)pA);
	pManager->GetOutput()->clearConnections(connection, -1, -1, false, true);
	((Connection*)pA)->setIsDrawn(false);
}

void AddConnection::Redo()
{
	Connection* conn = (Connection*)pA;
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

	pManager->GetOutput()->DrawConnection(GInfo, numOfInputs, inputComponent->getCenterLocation(), conn->getCellsBeforeAddingConnection());

	for (size_t i = 0; i < conn->getCellsBeforeAddingConnection().size(); i++)
	{
		pManager->GetOutput()->setArrayOfComponents(conn->getCellsBeforeAddingConnection()[i].y, conn->getCellsBeforeAddingConnection()[i].x, pA);
	}
	Action::pA = pA;
}
