#include "MultiMove.h"
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

MultiMove::MultiMove(ApplicationManager *pApp) :Action(pApp)
{
}

bool MultiMove::ReadActionParameters(image * smallImageBeforeAddingComponent)
{
	return true;
}

void MultiMove::Execute()
{
	//Get a Pointer to the Input / Output Interfaces
	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();

	int x, y;
	Component* Comp = NULL;
	bool allmoved = false;
	while (pIn->GetButtonStatus(LEFT_BUTTON, x, y) == BUTTON_DOWN || !allmoved) {
		int countOfMovedItems = 0;
		for (size_t i = 0; i < pIn->getSelectedComponents().size(); i++)
		{
			Component* Comp = pIn->getSelectedComponents()[i].second;
			Comp->setDelete(true);
			Comp->Draw(pManager->GetOutput(), true);
			//Free gate location
			int xbegin = (Comp->getCenterLocation().x1 - UI.GATE_Width / 2.0) / UI.GRID_SIZE, xend = (Comp->getCenterLocation().x1 + UI.GATE_Width / 2.0) / UI.GRID_SIZE, ybegin = (Comp->getCenterLocation().y1 - UI.GATE_Height / 2.0) / UI.GRID_SIZE, yend = (Comp->getCenterLocation().y1 + UI.GATE_Height / 2.0) / UI.GRID_SIZE;
			for (int i = ybegin + 1; i <= yend; i++)
			{
				for (int j = xbegin; j <= xend; j++)
				{
					pOut->setUsedPixel(i, j, EMPTY);
					pOut->setArrayOfComponents(i, j, NULL);
				}
			}
			// Free related connections
			vector <Connection*> allIns, allOuts;
			Comp->getAllInputConnections(allIns);
			Comp->getAllOutputConnections(allOuts);

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
			if (pManager->GetOutput()->SetDragImage(ActType, newCoor, newSmallImageForGate, true, Comp)) {
				Comp->setNewCenterLocation(newCoor);
				Comp->setDelete(false);
				pManager->allComponentsCorners[pIn->getSelectedComponents()[i].first].x1 = newCoor.x1 - UI.GATE_Width / 2;
				pManager->allComponentsCorners[pIn->getSelectedComponents()[i].first].y1 = newCoor.y1 - UI.GATE_Height / 2;
				pManager->allComponentsCorners[pIn->getSelectedComponents()[i].first].x2 = newCoor.x1 + UI.GATE_Width / 2;
				pManager->allComponentsCorners[pIn->getSelectedComponents()[i].first].y2 = newCoor.y1 + UI.GATE_Height / 2;
				Comp->setSmallCleanImageBeforeAddingComp(newSmallImageForGate);
				Comp->Draw(pOut, false);
				countOfMovedItems++;
				vector<Connection*> allInConnections, allOutConnections;
				Comp->getAllInputConnections(allInConnections);
				Comp->getAllOutputConnections(allOutConnections);
				for (size_t i = 0; i < allOutConnections.size(); i++)
				{
					allOutConnections[i]->selectYourSelf(pOut, UI.ConnColor);
				}
				for (size_t i = 0; i < allInConnections.size(); i++)
				{
					allInConnections[i]->selectYourSelf(pOut, UI.ConnColor);
				}
				int xbegin = (newCoor.x1 - UI.GATE_Width / 2.0) / UI.GRID_SIZE, xend = (newCoor.x1 + UI.GATE_Width / 2.0) / UI.GRID_SIZE, ybegin = (newCoor.y1 - UI.GATE_Height / 2.0) / UI.GRID_SIZE, yend = (newCoor.y1 + UI.GATE_Height / 2.0) / UI.GRID_SIZE;

				for (int i = ybegin + 1; i <= yend; i++)
				{
					for (int j = xbegin; j <= xend; j++)
					{
						pManager->GetOutput()->setArrayOfComponents(i, j, Comp);
					}
				}
			}
			else {
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
						pOut->setArrayOfComponents(i, j, Comp);
						pOut->setUsedPixel(i, j, GATE);
					}
				}
			}
			if (countOfMovedItems == pIn->getSelectedComponents().size())
			{
				allmoved = true;
			}
		}
	}
}

void MultiMove::Undo()
{}

void MultiMove::Redo()
{}


MultiMove::~MultiMove()
{
}
