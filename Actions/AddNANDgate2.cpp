#include "AddNANDgate2.h"
AddNANDgate2::AddNANDgate2(ApplicationManager *pApp) :Action(pApp)
{
}

AddNANDgate2::~AddNANDgate2(void)
{
}

void AddNANDgate2::ReadActionParameters()
{
	//Get a Pointer to the Input / Output Interfaces
	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();

	//Print Action Message
	pOut->PrintMsg(" 2-Input NAND gate : Click to add the gate");

	if (pOut->SetDragImage(ADD_NAND_GATE_2, GInfo)){
		string s = "Please enter gate label: ";
		pOut->PrintMsg(s);
		gateLabel = pIn->GetSrting(pOut, s);
	}


	//Clear Status Bar
	pOut->ClearStatusBar();

}

void AddNANDgate2::Execute()
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
	NAND2 *pA = new NAND2(GInfotmp, AND2_FANOUT);
	pManager->AddComponent(pA);
}

void AddNANDgate2::Undo()
{}

void AddNANDgate2::Redo()
{}