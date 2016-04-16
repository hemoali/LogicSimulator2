#include "AddNORgate3.h"
AddNORgate3::AddNORgate3(ApplicationManager *pApp) :Action(pApp)
{
}

AddNORgate3::~AddNORgate3(void)
{
}

bool AddNORgate3::ReadActionParameters(image * smallImageBeforeAddingComponent)
{
	bool done = false;

	//Get a Pointer to the Input / Output Interfaces
	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();

	//Print Action Message
	pOut->PrintMsg(" 3-Input NOR gate : Click to add the gate");

	if (pOut->SetDragImage(ADD_NOR_GATE_3, GInfo, smallImageBeforeAddingComponent)){
		string s = "Please enter gate label: ";
		pOut->PrintMsg(s);
		gateLabel = pIn->GetSrting(pOut, s);
		done = true;
	}


	//Clear Status Bar
	pOut->ClearStatusBar();
	return done;

}

void AddNORgate3::Execute()
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
	NOR3 *pA = new NOR3(GInfotmp, FANOUT);
	pManager->allComponentsCorners.push_back(GInfotmp);pManager->AddComponent(pA);pA->setLabel(gateLabel);pA->setSmallCleanImageBeforeAddingComp(smallImageBeforeAddingComponent);}
}

void AddNORgate3::Undo()
{}

void AddNORgate3::Redo()
{}