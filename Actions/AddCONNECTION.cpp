#include "AddCONNECTION.h"
AddCONNECTION::AddCONNECTION(ApplicationManager *pApp) :Action(pApp)
{
}

AddCONNECTION::~AddCONNECTION(void)
{
}

bool AddCONNECTION::ReadActionParameters()
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

}

void AddCONNECTION::Execute()
{
	//Get Center point of the Gate
	ReadActionParameters();

	GraphicsInfo GInfo; //Gfx info to be used to construct the AND2 gate

	GInfo.x1 = Cx1;
	GInfo.x2 = Cx2;
	GInfo.y1 = Cy1;
	GInfo.y2 = Cy2;

	Connection *pA = new Connection(GInfo, NULL, NULL); // ToDo : add pins to connection object

	pManager->AddComponent(pA);
}

void AddCONNECTION::Undo()
{}

void AddCONNECTION::Redo()
{}
