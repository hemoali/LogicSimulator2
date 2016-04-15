#include "Move.h"
#include "..\ApplicationManager.h"
#include "..\Components\NOR3.h"
#include "..\Components\AND2.h"
#include "..\Components\AND3.h"
#include "..\Components\BUFFER.h"
#include "..\Components\LED.h"
#include "..\Components\NAND2.h"
#include "..\Components\NOR2.h"
#include "..\Components\NOT.h"
#include "..\Components\OR2.h"
#include "..\Components\SWITCH.h"
#include "..\Components\XNOR2.h"
#include "..\Components\XOR3.h"
#include "..\Components\XOR2.h"
#include "..\Components\Connection.h"
#include<iostream>
using namespace std;

Move::Move(ApplicationManager *pApp) :Action(pApp)
{
}

bool Move::ReadActionParameters(image * smallImageBeforeAddingComponent)
{
	return true;
}

void Move::Execute()
{
	//Get a Pointer to the Input / Output Interfaces
	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();

	int x, y;
	Component* Comp = NULL;
	while (pIn->GetButtonStatus(LEFT_BUTTON, x, y) == BUTTON_DOWN) {
		int compIdx;
		for (int i = 0; i < pManager->allComponentsCorners.size(); i++)
		{
			if (x >= pManager->allComponentsCorners[i].x1 && x <= pManager->allComponentsCorners[i].x2 && y >= pManager->allComponentsCorners[i].y1&&y <= pManager->allComponentsCorners[i].y2 && !dynamic_cast<Connection*> (pManager->getComponent(i)))
			{
				compIdx = i;
				Comp = pManager->getComponent(i);
			}
		}
		if (Comp != NULL){
			Comp->setDelete(true);
			Comp->Draw(pManager->GetOutput());

			int xbegin = (Comp->getCenterLocation().x1 - UI.GATE_Width / 2.0) / UI.GRID_SIZE, xend = (Comp->getCenterLocation().x1 + UI.GATE_Width / 2.0) / UI.GRID_SIZE, ybegin = (Comp->getCenterLocation().y1 - UI.GATE_Height / 2.0) / UI.GRID_SIZE, yend = (Comp->getCenterLocation().y1 + UI.GATE_Height / 2.0) / UI.GRID_SIZE;
			for (int i = ybegin+1; i <= yend; i++)
			{
				for (int j = xbegin; j <= xend; j++)
				{
					pOut->setUsedPixel(i, j, EMPTY);
				}
			}
			//Get Action type
			ActionType ActType;
			if (dynamic_cast<AND2*> (Comp))
			{
				ActType = ADD_AND_GATE_2;
			}
			else if (dynamic_cast<AND3*> (Comp))
			{
				ActType = ADD_AND_GATE_3;
			}
			else if (dynamic_cast<BUFFER*> (Comp))
			{
				ActType = ADD_Buff;
			}
			else if (dynamic_cast<LED*> (Comp))
			{
				ActType = ADD_LED;
			}
			else if (dynamic_cast<NAND2*> (Comp))
			{
				ActType = ADD_NAND_GATE_2;
			}
			else if (dynamic_cast<NOR2*> (Comp))
			{
				ActType = ADD_NOR_GATE_2;
			}
			else if (dynamic_cast<NOR3*> (Comp))
			{
				ActType = ADD_NOR_GATE_3;
			}
			else if (dynamic_cast<NOT*> (Comp))
			{
				ActType = ADD_INV;
			}
			else if (dynamic_cast<OR2*> (Comp))
			{
				ActType = ADD_OR_GATE_2;
			}
			else if (dynamic_cast<SWITCH*> (Comp))
			{
				ActType = ADD_Switch;
			}
			else if (dynamic_cast<XNOR2*> (Comp))
			{
				ActType = ADD_XNOR_GATE_2;
			}
			else if (dynamic_cast<XOR2*> (Comp))
			{
				ActType = ADD_XOR_GATE_2;
			}
			else if (dynamic_cast<XOR3*> (Comp))
			{
				ActType = ADD_XOR_GATE_3;
			}
			//Drag
			GraphicsInfo newCoor;
			image* newSmallImageForGate = new image;
			if (pManager->GetOutput()->SetDragImage(ActType, newCoor, newSmallImageForGate, true, Comp)){
				Comp->setNewCenterLocation(newCoor);
				pManager->allComponentsCorners[compIdx].x1 = newCoor.x1 - UI.GATE_Width / 2;
				pManager->allComponentsCorners[compIdx].y1 = newCoor.y1 - UI.GATE_Height / 2;
				pManager->allComponentsCorners[compIdx].x2 = newCoor.x1 + UI.GATE_Width / 2;
				pManager->allComponentsCorners[compIdx].y2 = newCoor.y1 + UI.GATE_Height / 2;
				Comp->setSmallCleanImageBeforeAddingComp(newSmallImageForGate);
			}
			else{
				int xbegin = (Comp->getCenterLocation().x1 - UI.GATE_Width / 2.0) / UI.GRID_SIZE, xend = (Comp->getCenterLocation().x1 + UI.GATE_Width / 2.0) / UI.GRID_SIZE, ybegin = (Comp->getCenterLocation().y1 - UI.GATE_Height / 2.0) / UI.GRID_SIZE, yend = (Comp->getCenterLocation().y1 + UI.GATE_Height / 2.0) / UI.GRID_SIZE;
				for (int i = ybegin + 1; i <= yend; i++)
				{
					for (int j = xbegin; j <= xend; j++)
					{
						if (xbegin == j || xend == j)
						{
							pOut->setUsedPixel(i, j, PIN); 
							continue;
						}
						pOut->setUsedPixel(i, j, GATE);
					}
				}
			}

		}
	}
	if (Comp != NULL){
		Comp->setDelete(false);
		Comp->Draw(pManager->GetOutput());
	}

}

void Move::Undo()
{}

void Move::Redo()
{}


Move::~Move()
{
}
