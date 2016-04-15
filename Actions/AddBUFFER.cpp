#include "AddBUFFER.h"
AddBUFFER::AddBUFFER(ApplicationManager *pApp) :Action(pApp)
{
}

AddBUFFER::~AddBUFFER(void)
{
}

bool AddBUFFER::ReadActionParameters(image * smallImageBeforeAddingComponent)
{
	bool done = false;

	//Get a Pointer to the Input / Output Interfaces
	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();

	//Print Action Message
	pOut->PrintMsg("BUFFER : Click to add the gate");

	if (pOut->SetDragImage(ADD_Buff, GInfo, smallImageBeforeAddingComponent)){
		string s = "Please enter gate label: ";
		pOut->PrintMsg(s);
		gateLabel = pIn->GetSrting(pOut, s);
		done = true;
	}


	//Clear Status Bar
	pOut->ClearStatusBar();
	return done;

}

void AddBUFFER::Execute()
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
		BUFFER *pA = new BUFFER(GInfotmp, FANOUT);
		pA->setSmallCleanImageBeforeAddingComp(smallImageBeforeAddingComponent);
		pManager->allComponentsCorners.push_back(GInfotmp);
		pManager->AddComponent(pA);
	}
}

void AddBUFFER::Undo()
{}

void AddBUFFER::Redo()
{}
