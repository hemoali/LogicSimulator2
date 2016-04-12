#include "AddANDgate2.h"
#include "..\ApplicationManager.h"
#include<iostream>
using namespace std;
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
	pOut->PrintMsg("2-Input AND Gate: Click to add the gate, ESCAPE to stop");
	if (pOut->SetDragImage(ADD_AND_GATE_2, GInfo, smallImageBeforeAddingComponent)){

		string s = "Please enter gate label: ";
		pOut->PrintMsg(s);
		gateLabel = pIn->GetSrting(pOut, s);

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

		//Gfx info to be used to construct the AND2 gate
		GraphicsInfo GInfotmp;

		GInfotmp.x1 = GInfo.x1 - UI.GATE_Width / 2;
		GInfotmp.x2 = GInfo.x1 + UI.GATE_Width / 2;
		GInfotmp.y1 = GInfo.y1 - UI.GATE_Height / 2;
		GInfotmp.y2 = GInfo.y1 + UI.GATE_Height / 2;
		AND2 *pA = new AND2(GInfotmp, AND2_FANOUT);
		pA->setLabel(gateLabel);
		pManager->vec.push_back(GInfotmp);
		pManager->AddComponent(pA);
		pA->setSmallCleanImageBeforeAddingComp(smallImageBeforeAddingComponent);
	}
}

void AddANDgate2::Undo()
{}

void AddANDgate2::Redo()
{}

