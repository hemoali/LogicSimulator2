#include "Select.h"
#include "..\ApplicationManager.h"
#include<iostream>
using namespace std;

Select::Select(ApplicationManager *pApp) :Action(pApp)
{
}

bool Select::ReadActionParameters()
{
	bool done = false;
	//Get a Pointer to the Input / Output Interfaces
	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();

	//Print Action Message
	pOut->PrintMsg("2-Input AND Gate: Click to add the gate, ESCAPE to stop");

	if (pOut->SetDragImage(ADD_AND_GATE_2, GInfo)){

		string s = "Please enter gate label: ";
		pOut->PrintMsg(s);
		gateLabel = pIn->GetSrting(pOut, s);

		done = true;
	}

	//Clear Status Bar
	pOut->ClearStatusBar();
	return done;
}

void Select::Execute()
{
	//Get Center point of the Gate
	if (ReadActionParameters()){

		//Calculate the rectangle Corners
		int Len = UI.GATE_Width;
		int Wdth = UI.GATE_Height;

		//Gfx info to be used to construct the AND2 gate
		GraphicsInfo GInfotmp;

		GInfotmp.x1 = GInfo.x1 - Len / 2;
		GInfotmp.x2 = GInfo.x1 + Len / 2;
		GInfotmp.y1 = GInfo.y1 - Wdth / 2;
		GInfotmp.y2 = GInfo.y1 + Wdth / 2;
		AND2 *pA = new AND2(GInfotmp, AND2_FANOUT);
		pA->setLabel(gateLabel);
		pManager->AddComponent(pA);
	}
}

void AddANDgate2::Undo()
{}

void AddANDgate2::Redo()
{}


Select::~Select()
{
}
