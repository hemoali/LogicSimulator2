#include "AddCONNECTION.h"
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

	Connection *pA = new Connection(GInfo, AND2_FANOUT);

	pManager->AddComponent(pA);
	*/
}

void AddCONNECTION::Undo()
{}

void AddCONNECTION::Redo()
{}
