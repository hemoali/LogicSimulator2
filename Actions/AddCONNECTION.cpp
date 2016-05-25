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

bool AddConnection::ReadActionParameters(image * smallImageBeforeAddingComponent, Component* c)
{
	//Get a Pointer to the Input / Output Interfaces
	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();
	if (!Silent) { // Get next point from the user (the first point is got from the input file when he clicks the output pin)
		//Print Action Message
		pIn->getConnectionStartPoint(Cx1, Cy1);
		pOut->PrintStatusBox("Connection : Click to select the Destination");
		pIn->GetPointClicked(Cx2, Cy2);
		pIn->GetPointClicked(Cx2, Cy2);
	}
	else { // if adding silent connection (used for modifying/loading connection)
		pIn->CorrectPointClickedSilent(Cx1, Cy1);
		pIn->CorrectPointClickedSilent(Cx2, Cy2);
	}
	return true;
}
bool AddConnection::validateOutputComponent(Component* comp, Component* dstComp) { // check for feedback
	if (comp == NULL)
	{
		return true;
	}
	if (comp == dstComp) { // if we returned to same component -> there's feedback
		return false;
	}
	bool t[3] = { true, true, true }; // check for all connections out of the output component
	for (size_t j = 0; j < comp->getOutputPin()->connectedConnectionsCount(); j++)
	{
		t[j] = validateOutputComponent(comp->getOutputPin()->getConnection(j)->getDestPin()->getComponent(), dstComp);
	}
	return t[0] && t[1] && t[2];
}
void AddConnection::Execute()
{
	//Get Center point of the Gate
	ReadActionParameters(NULL, NULL);

	int indxOfInputComponent;

	int ii;
	Component* outputComponent = pManager->getComponentByCoordinates(Cx1, Cy1, true, true, ii);
	Component* inputComponent  = pManager->getComponentByCoordinates(Cx2, Cy2, true, true, ii);
	if (inputComponent == NULL || outputComponent == NULL || inputComponent == outputComponent || dynamic_cast<SWITCH*> (inputComponent) || dynamic_cast<LED*> (outputComponent))
	{
		pManager->GetOutput()->PrintStatusBox("Invalid Connection", UI.ErrorColor);
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
				pManager->GetOutput()->PrintStatusBox("Feedback isn't allowed", UI.ErrorColor);
				Sleep(600);
				goto end;
				break;
			}
		}

		//Detect input pin position
		numOfInputs = inputComponent->getNumOfInputs();
		int inputPin;
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
			// Set connection corners
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

			vector<Cell> cellsBeforeAddingConnection; // Cells to hold the state of grid pixels before adding the conenction
			Connection *pA = new Connection(GInfo, outputComponent->getOutputPin(), inputComponent->getInputPin(inputPin));
			//Draw the conenction
			if (pManager->GetOutput()->DrawConnection(GInfo, numOfInputs, inputComponent->getCenterLocation(), cellsBeforeAddingConnection, false, pA) && !(outputComponent->getOutputPin()->connectedConnectionsCount() == FANOUT))
			{
				pManager->AddComponent(pA); // Add to complist
				// set parameters
				pA->setCellsBeforeAddingConnection(cellsBeforeAddingConnection);
				// connect the connection to the inout/outputn pins
				outputComponent->getOutputPin()->ConnectTo(pA);
				inputComponent->getInputPin(inputPin)->setConnection(pA);
				inputComponent->getInputPin(inputPin)->setPosition(numOfInputs);

				if (!Silent) {
					gateLabel = pManager->GetInput()->getStringBox(); // get the label
				}
				pA->setLabel(gateLabel);
				pA->setIsDrawn(true);

				Utils::undoActions.push(this);
				Action::pA = pA;
			}
			else { // if not added successfully
				delete pA;
				pA = NULL;
				pManager->GetOutput()->PrintStatusBox("No Available Connection");
			}
		}
	}
end: if (outputComponent != NULL)
{
	// Remove red pin
	outputComponent->refresh(pManager->GetOutput());
}
}

void AddConnection::AddConnectionSilent(int c1, int c2, int c3, int c4, string s)
{ // set the new corners manually for loading/modifying connections
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
	pManager->GetOutput()->clearConnections(connection, -1, -1, false, true); // clear the connection from grid/visually
	((Connection*)pA)->setIsDrawn(false); // set not drawn
}

void AddConnection::Redo()
{
	// redraw the connection (check execute function for the same code comments)
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
	pManager->GetOutput()->DrawConnection(GInfo, numOfInputs, inputComponent->getCenterLocation(), conn->getCellsBeforeAddingConnection(), false, conn);
}
