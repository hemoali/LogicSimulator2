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
		int compIdx = -1;
		for (int i = 0; i < pManager->allComponentsCorners.size(); i++)
		{
			if (x >= pManager->allComponentsCorners[i].x1 && x <= pManager->allComponentsCorners[i].x2 && y >= pManager->allComponentsCorners[i].y1&&y <= pManager->allComponentsCorners[i].y2 && !dynamic_cast<Connection*> (pManager->getComponent(i)))
			{
				compIdx = i;
				Comp = pManager->getComponent(i);
			}
		}
		//
		if (Comp != NULL &&Comp->getDelete()) Comp = NULL;
		if (Comp != NULL && compIdx != -1) {
			Comp->setDelete(true);
			Comp->Draw(pManager->GetOutput());
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
			ActionType ActType = Comp->getComponentActionType();
			//Drag
			GraphicsInfo newCoor;
			image* newSmallImageForGate = new image;
			if (pManager->GetOutput()->SetDragImage(ActType, newCoor, newSmallImageForGate, true, Comp)) {
				Comp->setNewCenterLocation(newCoor);
				Comp->setDelete(false);
				pManager->allComponentsCorners[compIdx].x1 = newCoor.x1 - UI.GATE_Width / 2;
				pManager->allComponentsCorners[compIdx].y1 = newCoor.y1 - UI.GATE_Height / 2;
				pManager->allComponentsCorners[compIdx].x2 = newCoor.x1 + UI.GATE_Width / 2;
				pManager->allComponentsCorners[compIdx].y2 = newCoor.y1 + UI.GATE_Height / 2;
				Comp->setSmallCleanImageBeforeAddingComp(newSmallImageForGate);
				Comp->Draw(pOut, false);
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

		}
	}
	if (Comp != NULL) {
		Comp->setDelete(false);
		Comp->Draw(pManager->GetOutput());
	}
	pManager->GetInput()->reShowToolbar();

}

void Move::Undo()
{}

void Move::Redo()
{}


Move::~Move()
{
}
