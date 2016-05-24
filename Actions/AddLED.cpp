#include "AddLED.h"
AddLED::AddLED(ApplicationManager *pApp) :Action(pApp)
{
}

AddLED::~AddLED(void)
{
}

bool AddLED::ReadActionParameters(image * smallImageBeforeAddingComponent, Component* c)
{
	bool done = false;

	//Get a Pointer to the Input / Output Interfaces
	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();

	//Print Action Message
	pOut->PrintStatusBox(" SWITCH : Click to add the gate");

	if (pOut->SetDragImage(ADD_LED, GInfo, smallImageBeforeAddingComponent, false, c)){

		gateLabel =pIn->getStringBox();
		done = true;
	}


	
	
	return done;

}

void AddLED::Execute()
{
	//Get Center point of the Gate
	image* smallImageBeforeAddingComponent = new image;
	LED *pA = new LED({0,0,0,0}, FANOUT);

	if (ReadActionParameters(smallImageBeforeAddingComponent, pA)){

		//Calculate the rectangle Corners
		int Len = UI.GATE_Width;
		int Wdth = UI.GATE_Height;
		GraphicsInfo GInfotmp;
		GInfotmp.x1 = GInfo.x1 - Len / 2;
		GInfotmp.x2 = GInfo.x1 + Len / 2;
		GInfotmp.y1 = GInfo.y1 - Wdth / 2;
		GInfotmp.y2 = GInfo.y1 + Wdth / 2;
		pA->setNewCenterLocation(GInfo);
		Utils::allComponentsCorners.push_back(GInfotmp); 
		pManager->AddComponent(pA);pA->setLabel(gateLabel);
		pA->setSmallCleanImageBeforeAddingComp(smallImageBeforeAddingComponent);
		Utils::undoActions.push(this);
		Action::pA = pA;
	}
	else {
		delete pA;
		pA = NULL;
	}
}

