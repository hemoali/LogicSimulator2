#include "AddXORgate2.h"
AddXORgate2::AddXORgate2(ApplicationManager *pApp) :Action(pApp)
{
}

AddXORgate2::~AddXORgate2(void)
{
}

bool AddXORgate2::ReadActionParameters(image * smallImageBeforeAddingComponent)
{
	bool done = false;

	//Get a Pointer to the Input / Output Interfaces
	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();

	//Print Action Message
	pOut->PrintMsg("2-Input XOR Gate: Click to add the gate");

	if (pOut->SetDragImage(ADD_XOR_GATE_2, GInfo, smallImageBeforeAddingComponent)){
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
	  image* smallImageBeforeAddingComponent = new image; if (ReadActionParameters(smallImageBeforeAddingComponent)){

	//Calculate the rectangle Corners
	int Len = UI.GATE_Width;
	int Wdth = UI.GATE_Height;

	
	GraphicsInfo GInfotmp;

	GInfotmp.x1 = GInfo.x1 - Len / 2;
	GInfotmp.x2 = GInfo.x1 + Len / 2;
	GInfotmp.y1 = GInfo.y1 - Wdth / 2;
	GInfotmp.y2 = GInfo.y1 + Wdth / 2;
	 XOR2*pA = new XOR2(GInfotmp, FANOUT);
	pManager->allComponentsCorners.push_back(GInfotmp);pManager->AddComponent(pA);pA->setLabel(gateLabel);for (int i = GInfotmp.y1 / UI.GRID_SIZE + 1; i <= GInfotmp.y2 / UI.GRID_SIZE; i++)		{for (int j = GInfotmp.x1 / UI.GRID_SIZE; j <= GInfotmp.x2 / UI.GRID_SIZE; j++)	{	pManager->GetOutput()->setArrayOfComponents(i,j,pA);}}pA->setSmallCleanImageBeforeAddingComp(smallImageBeforeAddingComponent);}
}

void AddXORgate2::Undo()
{}

void AddXORgate2::Redo()
{}
