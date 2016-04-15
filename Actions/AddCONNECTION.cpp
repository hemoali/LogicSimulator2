#include "AddConnection.h"
AddConnection::AddConnection(ApplicationManager *pApp) :Action(pApp)
{
}

AddConnection::~AddConnection(void)
{
}

bool AddConnection::ReadActionParameters(image * smallImageBeforeAddingComponent)
{
	//Get a Pointer to the Input / Output Interfaces
	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();

	//Print Action Message
	pOut->PrintMsg("Connection : Click to select the Source");
	pIn->GetPointClicked(Cx1, Cy1);

	pOut->PrintMsg("Connection : Click to select the Destination");
	pIn->GetPointClicked(Cx2, Cy2);
	//Clear Status Bar
	pOut->ClearStatusBar();

	return true;
}

void AddConnection::Execute()
{
	bool validConnection = true;
	//Get Center point of the Gate
	ReadActionParameters(NULL);

	//Calculate the rectangle Corners
	GraphicsInfo GInfo; //Gfx info to be used to construct the AND2 gate

	GInfo.x1 = Cx1;
	GInfo.x2 = Cx2;
	GInfo.y1 = Cy1;
	GInfo.y2 = Cy2;

	Component* poutput = NULL;
	Component*pinput = NULL;
	int numofinputs = 0;
	int indxofinpgate;
	SWITCH*temp2 = NULL;  Gate*temp1 = NULL; Gate*temp3 = NULL; LED*temp4 = NULL;
	for (int i = 0; i < pManager->allComponentsCorners.size(); i++)
	{
		if (dynamic_cast<Connection*>(pManager->getGate(i)))
			continue;
		if (Cx1 >= pManager->allComponentsCorners[i].x1&&Cx1 <= pManager->allComponentsCorners[i].x2&& Cy1 >= pManager->allComponentsCorners[i].y1&&Cy1 <= pManager->allComponentsCorners[i].y2)
		{
			if (Cx1 < (pManager->allComponentsCorners[i].x1 + UI.GATE_Width / 2))
			{
				pinput = pManager->getGate(i);
				indxofinpgate = i;
			}
			else
			{
				poutput = pManager->getGate(i);
			}
		}
		if (Cx2 >= pManager->allComponentsCorners[i].x1&&Cx2 <= pManager->allComponentsCorners[i].x2&& Cy2 >= pManager->allComponentsCorners[i].y1&&Cy2 <= pManager->allComponentsCorners[i].y2)
		{
			if (Cx2 < (pManager->allComponentsCorners[i].x1 + UI.GATE_Width / 2))
			{
				pinput = pManager->getGate(i);
				indxofinpgate = i;
			}
			else
			{
				poutput = pManager->getGate(i);
			}
		}
	}


	if (pinput != NULL && dynamic_cast<Gate*>((pinput)))
	{
		temp1 = ((Gate*)pinput);
		numofinputs = temp1->getnumofinputs();
		if (numofinputs == 3)
		{
			if (Cy2 <= pManager->allComponentsCorners[indxofinpgate].y2 - UI.GATE_Height / 2 - 6)numofinputs = 0;
			else if (Cy2 >= pManager->allComponentsCorners[indxofinpgate].y2 - UI.GATE_Height / 2 + 6)numofinputs = 2;
			else numofinputs = 1;
		}
		else if (numofinputs == 2)
		{
			if (Cy2 <= pManager->allComponentsCorners[indxofinpgate].y2 - UI.GATE_Height / 2)numofinputs = 0;
			else if (Cy2 >= pManager->allComponentsCorners[indxofinpgate].y2 - UI.GATE_Height / 2)numofinputs = 2;
		}
		else numofinputs = 2;
	}
	else if (pinput != NULL&& dynamic_cast<LED*>((pinput)))
	{
		temp4 = ((LED*)pinput);
		numofinputs = 1;
	}
	else{
		validConnection = false;
	}
	if (poutput != NULL&& dynamic_cast<Gate*>((poutput)))
	{
		temp3 = ((Gate*)poutput);
	}
	else if (poutput != NULL  && dynamic_cast<SWITCH*>((poutput)))
	{
		temp2 = ((SWITCH*)poutput);
	}
	else{
		validConnection = false;
	}
	if (validConnection && pinput != poutput)
	{
		GInfo.x1 = poutput->getCenterLocation().x1 + UI.GATE_Width / 2 - 2;
		GInfo.y1 = poutput->getCenterLocation().y1;
		GInfo.x2 = pinput->getCenterLocation().x1 - UI.GATE_Width / 2 - 2;
		if (pinput->getnumofinputs() == 3)
		{
			if (numofinputs == 0)
			{
				GInfo.y2 = pinput->getCenterLocation().y1 - UI.GATE_Height / 2 + 13;
			}
			else if (numofinputs == 1){
				GInfo.y2 = pinput->getCenterLocation().y1;
			}
			else{
				GInfo.y2 = pinput->getCenterLocation().y1 + UI.GATE_Height / 2 - 2;
			}
		}
		else if (pinput->getnumofinputs() == 2){
			if (numofinputs == 0)
			{
				GInfo.y2 = pinput->getCenterLocation().y1 - UI.GATE_Height / 2 + 13;
			}
			else{
				GInfo.y2 = pinput->getCenterLocation().y1 + UI.GATE_Height / 2 - 13;
			}
		}
		else{
			GInfo.y2 = pinput->getCenterLocation().y1;
		}
		if (poutput != NULL && dynamic_cast<Gate*>((poutput)))
		{
			if (((Gate*)poutput)->getoutpin()->connectedConnectionsCount() == 3){
				validConnection = false;
			}
		}
		else if (poutput != NULL && dynamic_cast<SWITCH*>((poutput)))
		{
			if (((SWITCH*)poutput)->getoutpin()->connectedConnectionsCount() == 3){
				validConnection = false;
			}
		}

		if (pManager->GetOutput()->DrawConnection(GInfo, numofinputs, pinput->getCenterLocation()) && validConnection)
		{
			Connection *pA = new Connection(GInfo, (temp3 == NULL) ? temp2->getoutpin() : temp3->getoutpin(), (temp1 == NULL) ? temp4->getinppin() : temp1->getinppin(numofinputs));
			pManager->AddComponent(pA);
			pManager->allComponentsCorners.push_back(GInfo);
			
			if (poutput != NULL && dynamic_cast<Gate*>((poutput)))
			{
				((Gate*)poutput)->getoutpin()->ConnectTo(pA);
				pA->setSourcePin(((Gate*)poutput)->getoutpin());
			}
			else if (poutput != NULL && dynamic_cast<SWITCH*>((poutput)))
			{
				((SWITCH*)poutput)->getoutpin()->ConnectTo(pA);
				pA->setSourcePin(((SWITCH*)poutput)->getoutpin());
			}

			if (pinput != NULL && dynamic_cast<Gate*>((pinput)))
			{
				int idxOfInputPin = numofinputs;

				if (pinput->getnumofinputs() == 2 && numofinputs == 2){
					idxOfInputPin = 1;
				}
				pA->setDestPin(((Gate*)pinput)->getinppin(idxOfInputPin));
				((Gate*)pinput)->getinppin(idxOfInputPin)->setPosition(numofinputs);
				((Gate*)pinput)->getinppin(idxOfInputPin)->setConnection(pA);
			}
			else if (pinput != NULL && dynamic_cast<LED*>((pinput)))
			{
				((LED*)pinput)->getinppin()->setPosition(numofinputs);
				pA->setDestPin(((LED*)pinput)->getinppin());
				((LED*)pinput)->getinppin()->setConnection(pA);
			}
		}
		else{
			pManager->GetOutput()->PrintMsg("No Available Connection");
		}
	}
	else{
		pManager->GetOutput()->PrintMsg("Invalid Connection");
	}
}



void AddConnection::Undo()
{}

void AddConnection::Redo()
{}
