#include "AddANDgate2.h"
#include "..\ApplicationManager.h"
#include<iostream>
using namespace std;
AddANDgate2::AddANDgate2(ApplicationManager *pApp):Action(pApp)
{
}

AddANDgate2::~AddANDgate2(void)
{
}

void AddANDgate2::ReadActionParameters()
{
	//Get a Pointer to the Input / Output Interfaces
	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();

	//Print Action Message
	pOut->PrintMsg("2-Input AND Gate: Click to add the gate");

	//Wait for User Input
	pIn->GetPointClicked(Cx, Cy);
	
	//Clear Status Bar
	pOut->ClearStatusBar();
	
}

void AddANDgate2::Execute()
{
	//Get Center point of the Gate
	ReadActionParameters();
	
	//Calculate the rectangle Corners
	int Len = UI.GATE_Width;
	int Wdth = UI.GATE_Height;
	
	GraphicsInfo GInfo; //Gfx info to be used to construct the AND2 gate
	
	GInfo.x1 = Cx - Len/2;
	cout <<"X1 "<< GInfo.x1 << endl;
	GInfo.x2 = Cx + Len/2;
	cout << "X2 "<<GInfo.x2 << endl;
	GInfo.y1 = Cy - Wdth/2;
	cout << "Y1 "<<GInfo.y1 << endl;
	GInfo.y2 = Cy + Wdth/2;
	cout << "Y2 "<<GInfo.y2 << endl;
	AND2 *pA=new AND2(GInfo, AND2_FANOUT); 
	pManager->vec.push_back(GInfo);
	pManager->AddComponent(pA);
}

void AddANDgate2::Undo()
{}

void AddANDgate2::Redo()
{}

