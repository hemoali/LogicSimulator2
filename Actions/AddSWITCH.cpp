#include "AddSWITCH.h"
AddSWITCH::AddSWITCH(ApplicationManager *pApp) :Action(pApp)
{
}

AddSWITCH::~AddSWITCH(void)
{
}
//Check AddANDgate2 for the explanation of the next lines of code

bool AddSWITCH::ReadActionParameters(image * smallImageBeforeAddingComponent, Component* c)
{
	bool done = false;

	//Get a Pointer to the Input / Output Interfaces
	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();

	//Print Action Message
	pOut->PrintStatusBox(" SWITCH : Click to add the gate");

	if (pOut->SetDragImage(ADD_Switch, GInfo, smallImageBeforeAddingComponent, false, c)) {

		gateLabel =pIn->getStringBox();
		done = true;
	}

	
	
	return done;

}

void AddSWITCH::Execute()
{
	//Get Center point of the Gate
	SWITCH *pA = new SWITCH({0,0,0,0}, FANOUT);

	image* smallImageBeforeAddingComponent = new image; if (ReadActionParameters(smallImageBeforeAddingComponent, pA)) {

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
