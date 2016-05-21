#include "AddBUFFER.h"
AddBUFFER::AddBUFFER(ApplicationManager *pApp) :Action(pApp)
{
}

AddBUFFER::~AddBUFFER(void)
{
}

bool AddBUFFER::ReadActionParameters(image * smallImageBeforeAddingComponent, Component* c)
{
	bool done = false;

	//Get a Pointer to the Input / Output Interfaces
	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();

	//Print Action Message
	pOut->PrintStatusBox("BUFFER : Click to add the gate");

	if (pOut->SetDragImage(ADD_Buff, GInfo, smallImageBeforeAddingComponent, false, c)) {
		gateLabel =pIn->getStringBox();
		done = true;
	}


	//Clear Status Bar
	pOut->ClearStatusBar();
	return done;

}

void AddBUFFER::Execute()
{
	//Get Center point of the Gate
	image* smallImageBeforeAddingComponent = new image;
	BUFFER *pA = new BUFFER({0,0,0,0}, FANOUT);

	if (ReadActionParameters(smallImageBeforeAddingComponent, pA)) {

		//Calculate the rectangle Corners
		int Len = UI.GATE_Width;
		int Wdth = UI.GATE_Height;
		GraphicsInfo GInfotmp;
		GInfotmp.x1 = GInfo.x1 - Len / 2;
		GInfotmp.x2 = GInfo.x1 + Len / 2;
		GInfotmp.y1 = GInfo.y1 - Wdth / 2;
		GInfotmp.y2 = GInfo.y1 + Wdth / 2;
		pA->setNewCenterLocation(GInfo);
		pA->setSmallCleanImageBeforeAddingComp(smallImageBeforeAddingComponent);
		Utils::allComponentsCorners.push_back(GInfotmp);
		pManager->AddComponent(pA);
		pA->setLabel(gateLabel);
		Utils::undoActions.push(this);
		Action::pA = pA;
	}
	else {
		delete pA;
		pA = NULL;
	}
}
