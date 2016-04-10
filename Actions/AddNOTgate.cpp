#include "AddNOTgate.h"
AddNOTgate::AddNOTgate(ApplicationManager *pApp) :Action(pApp)
{
}

AddNOTgate::~AddNOTgate(void)
{
}

void AddNOTgate::ReadActionParameters()
{
	//Get a Pointer to the Input / Output Interfaces
	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();

	//Print Action Message
	pOut->PrintMsg(" NOT gate : Click to add the gate");

	if (pOut->SetDragImage(ADD_INV, GInfo)){
		string s = "Please enter gate label: ";
		pOut->PrintMsg(s);
		gateLabel = pIn->GetSrting(pOut, s);
	}


	//Clear Status Bar
	pOut->ClearStatusBar();

}

void AddNOTgate::Execute()
{
	//Get Center point of the Gate
	ReadActionParameters();

	//Calculate the rectangle Corners
	int Len = UI.GATE_Width;
	int Wdth = UI.GATE_Height;

	//Gfx info to be used to construct the AND2 gate
	GraphicsInfo GInfotmp;

	GInfotmp.x1 = GInfo.x1 - Len / 2;
	GInfotmp.x2 = GInfo.x1 + Len / 2;
	GInfotmp.y1 = GInfo.y1 - Wdth / 2;
	GInfotmp.y2 = GInfo.y1 + Wdth / 2;
	NOT *pA = new NOT(GInfotmp, AND2_FANOUT);
	pManager->AddComponent(pA);
}

void AddNOTgate::Undo()
{}

void AddNOTgate::Redo()
{}