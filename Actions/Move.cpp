#include "Move.h"
#include "..\ApplicationManager.h"
#include<iostream>
using namespace std;

Move::Move(ApplicationManager *pApp) :Action(pApp)
{
}

bool Move::ReadActionParameters()
{
	//Get a Pointer to the Input / Output Interfaces
	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();

	int x, y;

	while (pIn->GetButtonStatus(LEFT_BUTTON, x, y) == BUTTON_DOWN) {
		cout << x << " " << y << endl;
		if (pManager->getGate(0) != NULL){
			pManager->getGate(0)->setDelete(true);//ToDO get required gate
			pManager->getGate(0)->Draw(pManager->GetOutput());
			//Drag
			GraphicsInfo newCoor;
			image* newSmallImageForGate = new image;
			if (pManager->GetOutput()->SetDragImage(ADD_AND_GATE_2, newCoor, newSmallImageForGate, true)){
				pManager->getGate(0)->setNewLocation(newCoor);
				pManager->getGate(0)->setSmallCleanImageBeforeAddingComp(newSmallImageForGate);
			}
		}
	}
	if (pManager->getGate(0) != NULL){
		pManager->getGate(0)->setDelete(false);
		pManager->getGate(0)->Draw(pManager->GetOutput());
	}

	return true;
}

void Move::Execute()
{
	//Get Center point of the Gate
	ReadActionParameters();

}

void Move::Undo()
{}

void Move::Redo()
{}


Move::~Move()
{
}
