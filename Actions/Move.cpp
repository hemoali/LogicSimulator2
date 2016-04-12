#include "Move.h"
#include "..\ApplicationManager.h"
#include "..\GetComponent.h"
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
	GetComponent* getComp = NULL;
	while (pIn->GetButtonStatus(LEFT_BUTTON, x, y) == BUTTON_DOWN) {
		int compIdx;
		for (int i = 0; i < pManager->vec.size(); i++)
		{
			if (x >= pManager->vec[i].x1 && x <= pManager->vec[i].x2 && y >= pManager->vec[i].y1&&y <= pManager->vec[i].y2)
			{
				compIdx = i;
				getComp = new GetComponent(pManager->getGate(i));
			}
		}
		if (getComp != NULL && getComp->ptr != NULL){
			getComp->ptr->setDelete(true);
			getComp->ptr->Draw(pManager->GetOutput());

			int xbegin = (getComp->ptr->getCenterLocation().x1 - UI.GATE_Width / 2.0) / UI.GRID_SIZE, xend = (getComp->ptr->getCenterLocation().x1 + UI.GATE_Width / 2.0) / UI.GRID_SIZE, ybegin = (getComp->ptr->getCenterLocation().y1 - UI.GATE_Height / 2.0) / UI.GRID_SIZE, yend = (getComp->ptr->getCenterLocation().y1 + UI.GATE_Height / 2.0) / UI.GRID_SIZE;
			for (int i = ybegin + 1; i <= yend - 1; i++)
			{
				for (int j = xbegin + 1; j <= xend - 1; j++)
				{
					pOut->setUsedPixel(i, j, EMPTY);
				}
			}
			//Get Action type
			ActionType ActType;
			if (dynamic_cast<AND2*> (getComp->ptr))
			{
				ActType = ADD_AND_GATE_2;
			}
			else if (dynamic_cast<AND3*> (getComp->ptr))
			{
				ActType = ADD_AND_GATE_3;
			}
			else if (dynamic_cast<BUFFER*> (getComp->ptr))
			{
				ActType = ADD_Buff;
			}
			else if (dynamic_cast<LED*> (getComp->ptr))
			{
				ActType = ADD_LED;
			}
			else if (dynamic_cast<NAND2*> (getComp->ptr))
			{
				ActType = ADD_NAND_GATE_2;
			}
			else if (dynamic_cast<NOR2*> (getComp->ptr))
			{
				ActType = ADD_NOR_GATE_2;
			}
			else if (dynamic_cast<NOR3*> (getComp->ptr))
			{
				ActType = ADD_NOR_GATE_3;
			}
			else if (dynamic_cast<NOT*> (getComp->ptr))
			{
				ActType = ADD_INV;
			}
			else if (dynamic_cast<OR2*> (getComp->ptr))
			{
				ActType = ADD_OR_GATE_2;
			}
			else if (dynamic_cast<SWITCH*> (getComp->ptr))
			{
				ActType = ADD_Switch;
			}
			else if (dynamic_cast<XNOR2*> (getComp->ptr))
			{
				ActType = ADD_XNOR_GATE_2;
			}
			else if (dynamic_cast<XOR2*> (getComp->ptr))
			{
				ActType = ADD_XOR_GATE_2;
			}
			else if (dynamic_cast<XOR3*> (getComp->ptr))
			{
				ActType = ADD_XOR_GATE_3;
			}
			//Drag
			GraphicsInfo newCoor;
			image* newSmallImageForGate = new image;
			if (pManager->GetOutput()->SetDragImage(ActType, newCoor, newSmallImageForGate, true)){

				getComp->ptr->setNewLocation(newCoor);
				pManager->vec[compIdx].x1 = newCoor.x1 - UI.GATE_Width / 2;
				pManager->vec[compIdx].y1 = newCoor.y1 - UI.GATE_Height / 2;
				pManager->vec[compIdx].x2 = newCoor.x1 + UI.GATE_Width / 2;
				pManager->vec[compIdx].y2 = newCoor.y1 + UI.GATE_Height / 2;
				getComp->ptr->setSmallCleanImageBeforeAddingComp(newSmallImageForGate);
			}
			else{

				int xbegin = (getComp->ptr->getCenterLocation().x1 - UI.GATE_Width / 2.0) / UI.GRID_SIZE, xend = (getComp->ptr->getCenterLocation().x1 + UI.GATE_Width / 2.0) / UI.GRID_SIZE, ybegin = (getComp->ptr->getCenterLocation().y1 - UI.GATE_Height / 2.0) / UI.GRID_SIZE, yend = (getComp->ptr->getCenterLocation().y1 + UI.GATE_Height / 2.0) / UI.GRID_SIZE;
				for (int i = ybegin + 1; i <= yend - 1; i++)
				{
					for (int j = xbegin + 1; j <= xend - 1; j++)
					{
						pOut->setUsedPixel(i, j, GATE);
					}
				}

			}

		}
	}
	if (getComp != NULL && getComp->ptr != NULL){
		getComp->ptr->setDelete(false);
		getComp->ptr->Draw(pManager->GetOutput());
	}

}

void Move::Undo()
{}

void Move::Redo()
{}


Move::~Move()
{
}
