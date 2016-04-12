#include "AddXORgate3.h"
AddXORgate3::AddXORgate3(ApplicationManager *pApp) :Action(pApp)
{
}

AddXORgate3::~AddXORgate3(void)
{
}

bool AddXORgate3::ReadActionParameters()
{
	bool done = false;
	//Get a Pointer to the Input / Output Interfaces
	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();

	//Print Action Message
	pOut->PrintMsg("3-Input XOR Gate: Click to add the gate");

	if (pOut->SetDragImage(ADD_XOR_GATE_3, GInfo)){
		string s = "Please enter gate label: ";
		pOut->PrintMsg(s);
		gateLabel = pIn->GetSrting(pOut, s);
		done = true;
	}

	//Clear Status Bar
	pOut->ClearStatusBar();
	return done;

}

void AddXORgate3::Execute()
{
	//Get Center point of the Gate
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
	XOR3 *pA = new XOR3(GInfotmp, AND2_FANOUT);
	pManager->AddComponent(pA);}
}

void AddXORgate3::Undo()
{}

void AddXORgate3::Redo()
{}
