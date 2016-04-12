#include "AddSWITCH.h"
AddSWITCH::AddSWITCH(ApplicationManager *pApp) :Action(pApp)
{
}

AddSWITCH::~AddSWITCH(void)
{
}

bool AddSWITCH::ReadActionParameters()
{
	//Get a Pointer to the Input / Output Interfaces
	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();

	//Print Action Message
	pOut->PrintMsg(" SWITCH : Click to add the gate");

	if (pOut->SetDragImage(ADD_Switch, GInfo)){
		string s = "Please enter gate label: ";
		pOut->PrintMsg(s);
		gateLabel = pIn->GetSrting(pOut, s);
	}

	//Clear Status Bar
	pOut->ClearStatusBar();

}

void AddSWITCH::Execute()
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
	SWITCH *pA = new SWITCH(GInfotmp, AND2_FANOUT);
	pManager->AddComponent(pA);
}

void AddSWITCH::Undo()
{}

void AddSWITCH::Redo()
{}
