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
	pOut->PrintStatusBox("BUFFER : Click to add the gate");

	if (pOut->SetDragImage(ADD_Buff, GInfo, smallImageBeforeAddingComponent)) {
		string s = "Please enter gate label: ";
		pOut->PrintMsg(s);
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
	if (ReadActionParameters(smallImageBeforeAddingComponent)) {

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
		pManager->AddComponent(pA); pA->setLabel(gateLabel);
		for (int i = GInfotmp.y1 / UI.GRID_SIZE + 1; i <= GInfotmp.y2 / UI.GRID_SIZE; i++) { for (int j = GInfotmp.x1 / UI.GRID_SIZE; j <= GInfotmp.x2 / UI.GRID_SIZE; j++) { pManager->GetOutput()->setArrayOfComponents(i, j, pA); } }
		
		pManager->undoActions.push(this);
		Action::pA = pA;
	}

}
