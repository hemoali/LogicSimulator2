#include "AddNORgate2.h"
AddNORgate2::AddNORgate2(ApplicationManager *pApp) :Action(pApp)
{
}

AddNORgate2::~AddNORgate2(void)
{
}

bool AddNORgate2::ReadActionParameters()
{
	//Get a Pointer to the Input / Output Interfaces
	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();

	//Print Action Message
	pOut->PrintMsg(" 2-Input NOR gate : Click to add the gate");

	if (pOut->SetDragImage(ADD_NOR_GATE_2, GInfo)){
		string s = "Please enter gate label: ";
		pOut->PrintMsg(s);
		gateLabel = pIn->GetSrting(pOut, s);
	}


	//Clear Status Bar
	pOut->ClearStatusBar();

}

void AddNORgate2::Execute()
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
	NOR2 *pA = new NOR2(GInfotmp, AND2_FANOUT);
	pManager->AddComponent(pA);
}

void AddNORgate2::Undo()
{}

void AddNORgate2::Redo()
{}