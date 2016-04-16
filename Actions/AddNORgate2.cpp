#include "AddNORgate2.h"
AddNORgate2::AddNORgate2(ApplicationManager *pApp) :Action(pApp)
{
}

AddNORgate2::~AddNORgate2(void)
{
}

bool AddNORgate2::ReadActionParameters(image * smallImageBeforeAddingComponent)
{
	bool done = false;

	//Get a Pointer to the Input / Output Interfaces
	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();

	//Print Action Message
	pOut->PrintMsg(" 2-Input NOR gate : Click to add the gate");

	if (pOut->SetDragImage(ADD_NOR_GATE_2, GInfo, smallImageBeforeAddingComponent)){
		string s = "Please enter gate label: ";
		pOut->PrintMsg(s);
		gateLabel = pIn->GetSrting(pOut, s);
		done = true;
	}


	//Clear Status Bar
	pOut->ClearStatusBar();
	return done;

}

void AddNORgate2::Execute()
{
	//Get Center point of the Gate
	  image* smallImageBeforeAddingComponent = new image; if (ReadActionParameters(smallImageBeforeAddingComponent)){

	//Calculate the rectangle Corners
	int Len = UI.GATE_Width;
	int Wdth = UI.GATE_Height;

	
	GraphicsInfo GInfotmp;

	GInfotmp.x1 = GInfo.x1 - Len / 2;
	GInfotmp.x2 = GInfo.x1 + Len / 2;
	GInfotmp.y1 = GInfo.y1 - Wdth / 2;
	GInfotmp.y2 = GInfo.y1 + Wdth / 2;
	NOR2 *pA = new NOR2(GInfotmp, FANOUT);
	pManager->allComponentsCorners.push_back(GInfotmp);pManager->AddComponent(pA);pA->setLabel(gateLabel);pA->setSmallCleanImageBeforeAddingComp(smallImageBeforeAddingComponent);}
}

void AddNORgate2::Undo()
{}

void AddNORgate2::Redo()
{}