#include "AddANDgate3.h"
AddANDgate3::AddANDgate3(ApplicationManager *pApp) :Action(pApp)
{
}

AddANDgate3::~AddANDgate3(void)
{
}

void AddANDgate3::ReadActionParameters()
{
	//Get a Pointer to the Input / Output Interfaces
	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();

	//Print Action Message
	pOut->PrintMsg("3-Input AND Gate: Click to add the gate");

	if (pOut->SetDragImage(ADD_AND_GATE_3, GInfo)){
		string s = "Please enter gate label: ";
		pOut->PrintMsg(s);
		gateLabel = pIn->GetSrting(pOut, s);
	}

	//Clear Status Bar
	pOut->ClearStatusBar();

}

void AddANDgate3::Execute()
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
	AND3 *pA = new AND3(GInfotmp, AND2_FANOUT);
	pManager->AddComponent(pA);
}

void AddANDgate3::Undo()
{}

void AddANDgate3::Redo()
{}

