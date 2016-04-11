#include "AddCONNECTION.h"
#include<iostream>
using namespace std;
AddCONNECTION::AddCONNECTION(ApplicationManager *pApp) :Action(pApp)
{
}

AddCONNECTION::~AddCONNECTION(void)
{
}

void AddCONNECTION::ReadActionParameters()
{
	//Get a Pointer to the Input / Output Interfaces
	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();

	//Print Action Message
	pOut->PrintMsg("Connection : Click to the Source and the Destination");

	//Wait for User Input
	pIn->GetPointClicked(Cx1, Cy1);
	pIn->GetPointClicked(Cx2, Cy2);
	//Clear Status Bar
	pOut->ClearStatusBar();

}

void AddCONNECTION::Execute()
{
	//Get Center point of the Gate
	ReadActionParameters();

	//Calculate the rectangle Corners
	int Len = UI.GATE_Width;
	int Wdth = UI.GATE_Height;

	GraphicsInfo GInfo; //Gfx info to be used to construct the AND2 gate

	/*GInfo.x1 = Cx - Len / 2;
	GInfo.x2 = Cx + Len / 2;
	GInfo.y1 = Cy - Wdth / 2;
	GInfo.y2 = Cy + Wdth / 2;
	*/
	cout << "CX1 " << Cx1 << " " << "CX2 " << Cx2 << " " << "CY1 " << Cy1 << " " << "CY2 " << Cy2 << endl;
	cout << "CENTER "<<pManager->vec[0].x1+UI.GATE_Width/2 << endl;
	GetComponent*poutput=NULL;
	GetComponent*pinput=NULL;
	int numofinputs;
	int indxofinpgate;
	SWITCH*temp2=NULL;  Gate*temp1=NULL; Gate*temp3=NULL; LED*temp4=NULL;
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
	}
	if (dynamic_cast<Gate*>((pinput->ptr)))
	{
		
		temp1 = ((Gate*)pinput->ptr);
		int numofinputs = temp1->getnumofinputs();
		if (numofinputs == 3)
		{
			
			if ((abs(Cy1 - pManager->vec[indxofinpgate].y1) < UI.GATE_Height / 2) && abs(Cy1 - pManager->vec[indxofinpgate].y1) <= abs(UI.GATE_Height / 2 - Cy1))numofinputs = 1;
			else if ((abs(Cy1 - pManager->vec[indxofinpgate].y2) < UI.GATE_Height / 2) && abs(Cy1 - pManager->vec[indxofinpgate].y2) <= abs(UI.GATE_Height / 2 - Cy1))numofinputs = 3;
			else numofinputs = 2;
		}
		else if (numofinputs == 2)
		{
			if ((abs(Cy1 - pManager->vec[indxofinpgate].y1) < UI.GATE_Height / 2) )numofinputs = 1;
			else if ((abs(Cy1 - pManager->vec[indxofinpgate].y2) < UI.GATE_Height / 2))numofinputs = 2;
		}
		else numofinputs = 1;
	}
	else if (dynamic_cast<LED*>((pinput->ptr)))
	{
	   temp4=((LED*)pinput->ptr);
		numofinputs = 1;
	}
	if (dynamic_cast<Gate*>((poutput->ptr)))
	{
	  temp3=((Gate*)poutput->ptr);
	}
	else if (dynamic_cast<SWITCH*>((poutput->ptr)))
	{
		temp2=((SWITCH*)poutput->ptr);
	}
	Connection *pA = new Connection(GInfo,(temp3==NULL)?temp2->getoutpin():temp3->getoutpin(),(temp1==NULL)?temp4->getinppin():temp1->getinppin(numofinputs));
	pManager->AddComponent(pA);
	
}

void AddCONNECTION::Undo()
{}

void AddCONNECTION::Redo()
{}
