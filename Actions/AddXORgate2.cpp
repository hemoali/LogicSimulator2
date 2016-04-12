#include "AddXORgate2.h"
AddXORgate2::AddXORgate2(ApplicationManager *pApp) :Action(pApp)
{
}

AddXORgate2::~AddXORgate2(void)
{
}

bool AddXORgate2::ReadActionParameters()
{
	bool done = false;

	//Get a Pointer to the Input / Output Interfaces
	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();

	//Print Action Message
	pOut->PrintMsg("2-Input XOR Gate: Click to add the gate");

	if (pOut->SetDragImage(ADD_XOR_GATE_2, GInfo, NULL)){
		string s = "Please enter gate label: ";
		pOut->PrintMsg(s);
		gateLabel = pIn->GetSrting(pOut, s);
		done = true;
	}

	//Clear Status Bar
	pOut->ClearStatusBar();
	return done;

}

void AddXORgate2::Execute()
{	//Get Center point of the Gate
	 if (ReadActionParameters()){

	//Calculate the rectangle Corners
	int Len = UI.GATE_Width;
	int Wdth = UI.GATE_Height;

	//Gfx info to be used to construct the AND2 gate
	GraphicsInfo GInfotmp;

	GInfotmp.x1 = GInfo.x1 - Len / 2;
	GInfotmp.x2 = GInfo.x1 + Len / 2;
	GInfotmp.y1 = GInfo.y1 - Wdth / 2;
	GInfotmp.y2 = GInfo.y1 + Wdth / 2;
	 XOR2*pA = new XOR2(GInfotmp, AND2_FANOUT);
	pManager->AddComponent(pA);}
}

void AddXORgate2::Undo()
{}

void AddXORgate2::Redo()
{}
