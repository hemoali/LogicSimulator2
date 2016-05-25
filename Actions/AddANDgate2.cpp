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
	if (pOut->SetDragImage(ADD_AND_GATE_2, GInfo, smallImageBeforeAddingComponent, false, c)){ // start the dragging image function
		gateLabel = pIn->getStringBox(); // Getting the label from windows pop up box
		done = true;
	}
		
	return done;
}

void AddANDgate2::Execute()
{
	//Get Center point of the Gate
	image* smallImageBeforeAddingComponent = new image; // image to store pre-component image
	AND2 *pA = new AND2({0,0,0,0}, FANOUT); // Create the component
	if (ReadActionParameters(smallImageBeforeAddingComponent, pA)){	 	

		GraphicsInfo GInfotmp; // Corners
		GInfotmp.x1 = GInfo.x1 - UI.GATE_Width / 2;
		GInfotmp.x2 = GInfo.x1 + UI.GATE_Width / 2;
		GInfotmp.y1 = GInfo.y1 - UI.GATE_Height / 2;
		GInfotmp.y2 = GInfo.y1 + UI.GATE_Height / 2;
		pA->setLabel(gateLabel);
		pA->setSmallCleanImageBeforeAddingComp(smallImageBeforeAddingComponent);
		pA->setNewCenterLocation(GInfo);
		pManager->AddComponent(pA); // Add the component to application manager
		Utils::allComponentsCorners.push_back(GInfotmp); // Add the component corners to the corners vector
		Utils::undoActions.push(this); // register this action for undo/redo process
		Action::pA = pA;//set the component of the Action for undo/redo
	}
	else { // if not placed successfully or escaped clicked
		delete pA;
		pA = NULL;
	}
	
}
