#include "AddANDgate2.h"
AddANDgate2::AddANDgate2(ApplicationManager *pApp) :Action(pApp)
{
}

AddANDgate2::~AddANDgate2(void)
{
}
bool AddANDgate2::ReadActionParameters(image * smallImageBeforeAddingComponent, Component * c)
{
	bool done = false;
	//Get a Pointer to the Input / Output Interfaces
	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();

	//Print Action Message
	pOut->PrintStatusBox("2-Input AND Gate: Click to add the gate, ESCAPE to stop");
	if (pOut->SetDragImage(ADD_AND_GATE_2, GInfo, smallImageBeforeAddingComponent, false, c)){
		gateLabel = pIn->getStringBox();

		done = true;
	}
		
	return done;
}

void AddANDgate2::Execute()
{
	//Get Center point of the Gate
	image* smallImageBeforeAddingComponent = new image;
	AND2 *pA = new AND2({0,0,0,0}, FANOUT);
	if (ReadActionParameters(smallImageBeforeAddingComponent, pA)){		
		GraphicsInfo GInfotmp;
		GInfotmp.x1 = GInfo.x1 - UI.GATE_Width / 2;
		GInfotmp.x2 = GInfo.x1 + UI.GATE_Width / 2;
		GInfotmp.y1 = GInfo.y1 - UI.GATE_Height / 2;
		GInfotmp.y2 = GInfo.y1 + UI.GATE_Height / 2;
		pA->setLabel(gateLabel);
		pA->setSmallCleanImageBeforeAddingComp(smallImageBeforeAddingComponent);
		pA->setNewCenterLocation(GInfo);
		pManager->AddComponent(pA);
		Utils::allComponentsCorners.push_back(GInfotmp);
		Utils::undoActions.push(this);
		Action::pA = pA;
	}
	else {
		delete pA;
		pA = NULL;
	}
	
}
