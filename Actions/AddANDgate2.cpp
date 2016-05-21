#include "AddANDgate2.h"
AddANDgate2::AddANDgate2(ApplicationManager *pApp) :Action(pApp)
{
}

AddANDgate2::~AddANDgate2(void)
{
}
bool AddANDgate2::ReadActionParameters(image * smallImageBeforeAddingComponent)
{
	bool done = false;
	//Get a Pointer to the Input / Output Interfaces
	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();

	//Print Action Message
	pOut->PrintStatusBox("2-Input AND Gate: Click to add the gate, ESCAPE to stop");
	if (pOut->SetDragImage(ADD_AND_GATE_2, GInfo, smallImageBeforeAddingComponent)){
		gateLabel =pIn->getStringBox();

		done = true;
	}

	//Clear Status Bar
	pOut->ClearStatusBar();
	return done;
}

void AddANDgate2::Execute()
{
	//Get Center point of the Gate
	image* smallImageBeforeAddingComponent = new image;

	if (ReadActionParameters(smallImageBeforeAddingComponent)){

		
		GraphicsInfo GInfotmp;

		GInfotmp.x1 = GInfo.x1 - UI.GATE_Width / 2;
		GInfotmp.x2 = GInfo.x1 + UI.GATE_Width / 2;
		GInfotmp.y1 = GInfo.y1 - UI.GATE_Height / 2;
		GInfotmp.y2 = GInfo.y1 + UI.GATE_Height / 2;
		AND2 *pA = new AND2(GInfotmp, FANOUT);
		pA->setLabel(gateLabel);
		pA->setSmallCleanImageBeforeAddingComp(smallImageBeforeAddingComponent);
		Utils::allComponentsCorners.push_back(GInfotmp);
		pManager->AddComponent(pA);
		pA->setLabel(gateLabel);
		for (int i = GInfotmp.y1 / UI.GRID_SIZE + 1; i <= GInfotmp.y2 / UI.GRID_SIZE; i++)
		{
			for (int j = GInfotmp.x1 / UI.GRID_SIZE; j <= GInfotmp.x2 / UI.GRID_SIZE; j++)	
			{	
			Utils::setArrayOfComponents(i,j,pA);
			}
		}
		Utils::undoActions.push(this);
		Action::pA = pA;
	}
	
}
