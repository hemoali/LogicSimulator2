#include "AddCONNECTION.h"
AddCONNECTION::AddCONNECTION(ApplicationManager *pApp) :Action(pApp)
{
}

AddCONNECTION::~AddCONNECTION(void)
{
}

bool AddCONNECTION::ReadActionParameters(image * smallImageBeforeAddingComponent)
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

void AddCONNECTION::Execute()
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

	GetComponent*poutput = NULL;
	GetComponent*pinput = NULL;
	int numofinputs = 0;
	int indxofinpgate;
	SWITCH*temp2 = NULL;  Gate*temp1 = NULL; Gate*temp3 = NULL; LED*temp4 = NULL;
	for (int i = 0; i < pManager->vec.size(); i++)
	{
		if (Cx1 >= pManager->vec[i].x1&&Cx1 <= pManager->vec[i].x2&& Cy1 >= pManager->vec[i].y1&&Cy1 <= pManager->vec[i].y2)
		{
			if (Cx1 < (pManager->vec[i].x1 + UI.GATE_Width / 2))
			{
				pinput = new GetComponent(pManager->getGate(i));
				indxofinpgate = i;
			}
			else
			{
				poutput = new GetComponent(pManager->getGate(i));
			}
		}
		if (Cx2 >= pManager->vec[i].x1&&Cx2 <= pManager->vec[i].x2&& Cy2 >= pManager->vec[i].y1&&Cy2 <= pManager->vec[i].y2)
		{
			if (Cx2 < (pManager->vec[i].x1 + UI.GATE_Width / 2))
			{
				pinput = new GetComponent(pManager->getGate(i));
				indxofinpgate = i;
			}
			else
			{
				poutput = new GetComponent(pManager->getGate(i));
			}
		}
	}


	if (pinput != NULL && pinput->ptr != NULL && dynamic_cast<Gate*>((pinput->ptr)))
	{
		temp1 = ((Gate*)pinput->ptr);
		numofinputs = temp1->getnumofinputs();
		if (numofinputs == 3)
		{
			if ((abs(Cy2 - pManager->vec[indxofinpgate].y1) < UI.GATE_Height / 2) && abs(Cy2 - pManager->vec[indxofinpgate].y1) <= abs(UI.GATE_Height / 2 - Cy1))numofinputs = 0;
			else if ((abs(Cy2 - pManager->vec[indxofinpgate].y2) < UI.GATE_Height / 2) && abs(Cy2 - pManager->vec[indxofinpgate].y2) <= abs(UI.GATE_Height / 2 - Cy1))numofinputs = 2;
			else numofinputs = 1;
		}
		else if (numofinputs == 2)
		{
			if ((abs(Cy2 - pManager->vec[indxofinpgate].y1) < UI.GATE_Height / 2))numofinputs = 0;
			else if ((abs(Cy2 - pManager->vec[indxofinpgate].y2) < UI.GATE_Height / 2))numofinputs = 2;
		}
		else numofinputs = 2;
	}
	else if (pinput != NULL && pinput->ptr != NULL && dynamic_cast<LED*>((pinput->ptr)))
	{
		temp4 = ((LED*)pinput->ptr);
		numofinputs = 1;
	}
	else{
		validConnection = false;
	}
	if (poutput != NULL && poutput->ptr != NULL && dynamic_cast<Gate*>((poutput->ptr)))
	{
		temp3 = ((Gate*)poutput->ptr);
	}
	else if (poutput != NULL && poutput->ptr != NULL && dynamic_cast<SWITCH*>((poutput->ptr)))
	{
		temp2 = ((SWITCH*)poutput->ptr);
	}
	else{
		validConnection = false;
	}
	if (validConnection && pinput->ptr != poutput->ptr)
	{
		GInfo.x1 = poutput->ptr->getCenterLocation().x1 + UI.GATE_Width / 2 - 2;
		GInfo.y1 = poutput->ptr->getCenterLocation().y1;
		GInfo.x2 = pinput->ptr->getCenterLocation().x1 - UI.GATE_Width / 2 - 2;
		if (pinput->ptr->getnumofinputs() == 3)
		{
			if (numofinputs == 0)
			{
				GInfo.y2 = pinput->ptr->getCenterLocation().y1 - UI.GATE_Height / 2 + 13;
			}
			else if (numofinputs == 1){
				GInfo.y2 = pinput->ptr->getCenterLocation().y1;
			}
			else{
				GInfo.y2 = pinput->ptr->getCenterLocation().y1 + UI.GATE_Height / 2 - 2;
			}
		}
		else if (pinput->ptr->getnumofinputs() == 2){
			if (numofinputs == 0)
			{
				GInfo.y2 = pinput->ptr->getCenterLocation().y1 - UI.GATE_Height / 2 + 13;
			}
			else{
				GInfo.y2 = pinput->ptr->getCenterLocation().y1 + UI.GATE_Height / 2 - 13;
			}
		}
		else{
			GInfo.y2 = pinput->ptr->getCenterLocation().y1;
		}
		if (pManager->GetOutput()->DrawConnection(GInfo, numofinputs, pinput->ptr))
		{
			Connection *pA = new Connection(GInfo, (temp3 == NULL) ? temp2->getoutpin() : temp3->getoutpin(), (temp1 == NULL) ? temp4->getinppin() : temp1->getinppin(numofinputs));
			pManager->AddComponent(pA);
		}
		else{
			pManager->GetOutput()->PrintMsg("No Available Connection");
		}		
	}
	else{
		pManager->GetOutput()->PrintMsg("Invalid Connection");
	}
}



void AddCONNECTION::Undo()
{}

void AddCONNECTION::Redo()
{}
