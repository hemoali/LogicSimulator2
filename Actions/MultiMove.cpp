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

	if (pIn->GetButtonStatus(LEFT_BUTTON, x, y) == BUTTON_DOWN)
	{
		Component* comp = NULL;

		for (int i = 0; i < pIn->getSelectedComponents().size(); i++)
		{
			Component* c = pIn->getSelectedComponents()[i].second;
			if (dynamic_cast<Connection*>(c))
				continue;
			if (x >= c->getCornersLocation().x1&&x <= c->getCornersLocation().x2&& y >= c->getCornersLocation().y1&&y <= c->getCornersLocation().y2)
			{
				comp = pIn->getSelectedComponents()[i].second;
			}
			//Store old data before moving
			selectedComponents.push_back(c);
			oldSmallCleanImage.push_back(c->getSmallCleanImageBeforeAddingComp());
			oldGraphicsInfo.push_back({ c->getCenterLocation().x1 ,  c->getCenterLocation().y1 });
			compIdx.push_back(pIn->getSelectedComponents()[i].first);

			//start moving process
			c->setDelete(true);
			c->Draw(pManager->GetOutput());

			int xbegin = (c->getCenterLocation().x1 - UI.GATE_Width / 2.0) / UI.GRID_SIZE, xend = (c->getCenterLocation().x1 + UI.GATE_Width / 2.0) / UI.GRID_SIZE, ybegin = (c->getCenterLocation().y1 - UI.GATE_Height / 2.0) / UI.GRID_SIZE, yend = (c->getCenterLocation().y1 + UI.GATE_Height / 2.0) / UI.GRID_SIZE;
			for (int i = ybegin + 1; i <= yend; i++)
			{
				for (int j = xbegin; j <= xend; j++)
				{
					pOut->setUsedPixel(i, j, EMPTY);
					Utils::setArrayOfComponents(i, j, NULL);
				}
			}
		}
		vector < pair<int, Component*> > selectedGates;
		for (size_t i = 0; i < pIn->getSelectedComponents().size(); i++)
		{
			if (dynamic_cast<Connection*> (pIn->getSelectedComponents()[i].second))
			{
				continue;
			}
			selectedGates.push_back(pIn->getSelectedComponents()[i]);
		}
		if (pOut->SetMultiDragImage(x, y, comp, selectedGates)) {
			for (int i = 0; i < pIn->getSelectedComponents().size(); i++)
			{
				newCoor.push_back({ pIn->getSelectedComponents()[i].second->getCenterLocation().x1,pIn->getSelectedComponents()[i].second->getCenterLocation().y1 });
				newSmallImageForGate.push_back(pIn->getSelectedComponents()[i].second->getSmallCleanImageBeforeAddingComp());

				Utils::allComponentsCorners[pIn->getSelectedComponents()[i].first].x1 = pIn->getSelectedComponents()[i].second->getCornersLocation().x1;
				Utils::allComponentsCorners[pIn->getSelectedComponents()[i].first].y1 = pIn->getSelectedComponents()[i].second->getCornersLocation().y1;
				Utils::allComponentsCorners[pIn->getSelectedComponents()[i].first].x2 = pIn->getSelectedComponents()[i].second->getCornersLocation().x2;
				Utils::allComponentsCorners[pIn->getSelectedComponents()[i].first].y2 = pIn->getSelectedComponents()[i].second->getCornersLocation().y2;
				if (dynamic_cast<Connection*> (pIn->getSelectedComponents()[i].second))
				{
					((Connection*)pIn->getSelectedComponents()[i].second)->selectYourSelf(pManager->GetOutput(), UI.DrawColor);

				}
			}
			Utils::undoActions.push(this);
		}
		else {
			for (int i = 0; i < pIn->getSelectedComponents().size(); i++)
			{
				int xbegin = (pIn->getSelectedComponents()[i].second->getCenterLocation().x1 - UI.GATE_Width / 2.0) / UI.GRID_SIZE, xend = (pIn->getSelectedComponents()[i].second->getCenterLocation().x1 + UI.GATE_Width / 2.0) / UI.GRID_SIZE, ybegin = (pIn->getSelectedComponents()[i].second->getCenterLocation().y1 - UI.GATE_Height / 2.0) / UI.GRID_SIZE, yend = (pIn->getSelectedComponents()[i].second->getCenterLocation().y1 + UI.GATE_Height / 2.0) / UI.GRID_SIZE;
				for (int i = ybegin + 1; i <= yend; i++)
				{
					for (int j = xbegin; j <= xend; j++)
					{
						if (xbegin == j || xend == j)
						{
							pOut->setUsedPixel(i, j, PIN);
							continue;
						}
						Utils::setArrayOfComponents(i, j, pIn->getSelectedComponents()[i].second);
						pOut->setUsedPixel(i, j, GATE);
					}
				}
			}
		}

	}
	pIn->setSelectMode(false);
	pIn->getSelectedComponents().clear();


}

void MultiMove::Undo()
{
	Output* pOut = pManager->GetOutput();
	for (size_t i = 0; i < selectedComponents.size(); i++)
	{
		pA = selectedComponents[i];

		pA->setDelete(true);
		pA->Draw(pOut, false);
		//free new location
		int xbegin = (newCoor[i].x1 - UI.GATE_Width / 2.0) / UI.GRID_SIZE, xend = (newCoor[i].x1 + UI.GATE_Width / 2.0) / UI.GRID_SIZE, ybegin = (newCoor[i].y1 - UI.GATE_Height / 2.0) / UI.GRID_SIZE, yend = (newCoor[i].y1 + UI.GATE_Height / 2.0) / UI.GRID_SIZE;
		for (int i = ybegin + 1; i <= yend; i++)
		{
			for (int j = xbegin; j <= xend; j++)
			{
				pOut->setUsedPixel(i, j, EMPTY);
				Utils::setArrayOfComponents(i, j, NULL);
			}
		}
		//remove new Connections
		vector<Connection*> allInputConnections, allOutputConnections;
		pA->getAllInputConnections(allInputConnections);
		pA->getAllOutputConnections(allOutputConnections);

		pOut->clearConnections(allInputConnections, pA->getCenterLocation().x1, pA->getCenterLocation().y1, true, false);
		pOut->clearConnections(allOutputConnections, pA->getCenterLocation().x1, pA->getCenterLocation().y1, false, false);

		//redraw and reassign data

		pA->setNewCenterLocation(oldGraphicsInfo[i]);
		Utils::allComponentsCorners[compIdx[i]].x1 = oldGraphicsInfo[i].x1 - UI.GATE_Width / 2;
		Utils::allComponentsCorners[compIdx[i]].y1 = oldGraphicsInfo[i].y1 - UI.GATE_Height / 2;
		Utils::allComponentsCorners[compIdx[i]].x2 = oldGraphicsInfo[i].x1 + UI.GATE_Width / 2;
		Utils::allComponentsCorners[compIdx[i]].y2 = oldGraphicsInfo[i].y1 + UI.GATE_Height / 2;
		pA->setSmallCleanImageBeforeAddingComp(oldSmallCleanImage[i]);

		pA->setDelete(false);
		pA->Draw(pOut, false);
		//refill old location
		xbegin = (oldGraphicsInfo[i].x1 - UI.GATE_Width / 2.0) / UI.GRID_SIZE;
		xend = (oldGraphicsInfo[i].x1 + UI.GATE_Width / 2.0) / UI.GRID_SIZE;
		ybegin = (oldGraphicsInfo[i].y1 - UI.GATE_Height / 2.0) / UI.GRID_SIZE;
		yend = (oldGraphicsInfo[i].y1 + UI.GATE_Height / 2.0) / UI.GRID_SIZE;
		for (int i = ybegin + 1; i <= yend; i++)
		{
			for (int j = xbegin; j <= xend; j++)
			{
				if (xbegin == j || xend == j)
				{
					pOut->setUsedPixel(i, j, PIN);
					continue;
				}
				Utils::setArrayOfComponents(i, j, pA);
				pOut->setUsedPixel(i, j, GATE);
			}
		}
		//redraw old connections
		for (size_t j = 0; j < allInputConnections.size(); j++)
		{
			GraphicsInfo currentGfx = allInputConnections[j]->getCornersLocation();
			currentGfx.x2 = currentGfx.x2 - (newCoor[i].x1 - oldGraphicsInfo[i].x1);
			currentGfx.y2 = currentGfx.y2 - (newCoor[i].y1 - oldGraphicsInfo[i].y1);
			allInputConnections[j]->setCornersLocation({ currentGfx.x1 ,currentGfx.y1,currentGfx.x2 ,currentGfx.y2 });
			pOut->DrawConnection(currentGfx, allInputConnections[j]->getDestPin()->getPosition(), { oldGraphicsInfo[i].x1, oldGraphicsInfo[i].y1,0,0 }, allInputConnections[j]->getCellsBeforeAddingConnection(), false);

			for (size_t k = 0; k < allInputConnections[j]->getCellsBeforeAddingConnection().size(); k++)
			{
				Utils::setArrayOfComponents(allInputConnections[j]->getCellsBeforeAddingConnection()[k].y, allInputConnections[j]->getCellsBeforeAddingConnection()[k].x, allInputConnections[j]);
			}
		}
		for (size_t j = 0; j < allOutputConnections.size(); j++)
		{
			GraphicsInfo currentGfx = allOutputConnections[j]->getCornersLocation();
			currentGfx.x1 = currentGfx.x1 - (newCoor[i].x1 - oldGraphicsInfo[i].x1);
			currentGfx.y1 = currentGfx.y1 - (newCoor[i].y1 - oldGraphicsInfo[i].y1);
			allOutputConnections[j]->setCornersLocation({ currentGfx.x1 ,currentGfx.y1,currentGfx.x2 ,currentGfx.y2 });
			Component* dstComp = allOutputConnections[j]->getDestPin()->getComponent();
			pOut->DrawConnection(currentGfx, allOutputConnections[j]->getDestPin()->getPosition(), { dstComp->getCenterLocation().x1, dstComp->getCenterLocation().y1,0,0 }, allOutputConnections[j]->getCellsBeforeAddingConnection(), false);

			for (size_t k = 0; k < allOutputConnections[j]->getCellsBeforeAddingConnection().size(); k++)
			{
				Utils::setArrayOfComponents(allOutputConnections[j]->getCellsBeforeAddingConnection()[k].y, allOutputConnections[j]->getCellsBeforeAddingConnection()[k].x, allOutputConnections[j]);
			}
		}

	}
}

void MultiMove::Redo()
{
	Output* pOut = pManager->GetOutput();
	for (size_t i = 0; i < selectedComponents.size(); i++)
	{
		pA = selectedComponents[i];
		pA->setDelete(true);
		pA->Draw(pOut, false);
		//free new location
		int xbegin = (oldGraphicsInfo[i].x1 - UI.GATE_Width / 2.0) / UI.GRID_SIZE, xend = (oldGraphicsInfo[i].x1 + UI.GATE_Width / 2.0) / UI.GRID_SIZE, ybegin = (oldGraphicsInfo[i].y1 - UI.GATE_Height / 2.0) / UI.GRID_SIZE, yend = (oldGraphicsInfo[i].y1 + UI.GATE_Height / 2.0) / UI.GRID_SIZE;
		for (int i = ybegin + 1; i <= yend; i++)
		{
			for (int j = xbegin; j <= xend; j++)
			{
				pOut->setUsedPixel(i, j, EMPTY);
				Utils::setArrayOfComponents(i, j, NULL);
			}
		}
		//remove new Connections
		vector<Connection*> allInputConnections, allOutputConnections;
		pA->getAllInputConnections(allInputConnections);
		pA->getAllOutputConnections(allOutputConnections);

		pOut->clearConnections(allInputConnections, pA->getCenterLocation().x1, pA->getCenterLocation().y1, true, false);
		pOut->clearConnections(allOutputConnections, pA->getCenterLocation().x1, pA->getCenterLocation().y1, false, false);

		//redraw and reassign data

		pA->setNewCenterLocation(newCoor[i]);
		Utils::allComponentsCorners[compIdx[i]].x1 = newCoor[i].x1 - UI.GATE_Width / 2;
		Utils::allComponentsCorners[compIdx[i]].y1 = newCoor[i].y1 - UI.GATE_Height / 2;
		Utils::allComponentsCorners[compIdx[i]].x2 = newCoor[i].x1 + UI.GATE_Width / 2;
		Utils::allComponentsCorners[compIdx[i]].y2 = newCoor[i].y1 + UI.GATE_Height / 2;
		pA->setSmallCleanImageBeforeAddingComp(newSmallImageForGate[i]);

		pA->setDelete(false);
		pA->Draw(pOut, false);
		//refill old location
		xbegin = (newCoor[i].x1 - UI.GATE_Width / 2.0) / UI.GRID_SIZE;
		xend = (newCoor[i].x1 + UI.GATE_Width / 2.0) / UI.GRID_SIZE;
		ybegin = (newCoor[i].y1 - UI.GATE_Height / 2.0) / UI.GRID_SIZE;
		yend = (newCoor[i].y1 + UI.GATE_Height / 2.0) / UI.GRID_SIZE;
		for (int i = ybegin + 1; i <= yend; i++)
		{
			for (int j = xbegin; j <= xend; j++)
			{
				if (xbegin == j || xend == j)
				{
					pOut->setUsedPixel(i, j, PIN);
					continue;
				}
				Utils::setArrayOfComponents(i, j, pA);
				pOut->setUsedPixel(i, j, GATE);
			}
		}
		//redraw old connections
		for (size_t j = 0; j < allInputConnections.size(); j++)
		{
			GraphicsInfo currentGfx = allInputConnections[j]->getCornersLocation();
			currentGfx.x2 = currentGfx.x2 + (newCoor[i].x1 - oldGraphicsInfo[i].x1);
			currentGfx.y2 = currentGfx.y2 + (newCoor[i].y1 - oldGraphicsInfo[i].y1);
			allInputConnections[j]->setCornersLocation({ currentGfx.x1 ,currentGfx.y1,currentGfx.x2 ,currentGfx.y2 });
			pOut->DrawConnection(currentGfx, allInputConnections[j]->getDestPin()->getPosition(), { newCoor[i].x1, newCoor[i].y1,0,0 }, allInputConnections[j]->getCellsBeforeAddingConnection(), false);

			for (size_t k = 0; k < allInputConnections[j]->getCellsBeforeAddingConnection().size(); k++)
			{
				Utils::setArrayOfComponents(allInputConnections[j]->getCellsBeforeAddingConnection()[k].y, allInputConnections[j]->getCellsBeforeAddingConnection()[k].x, allInputConnections[j]);
			}
		}
		for (size_t j = 0; j < allOutputConnections.size(); j++)
		{
			GraphicsInfo currentGfx = allOutputConnections[j]->getCornersLocation();
			currentGfx.x1 = currentGfx.x1 + (newCoor[i].x1 - oldGraphicsInfo[i].x1);
			currentGfx.y1 = currentGfx.y1 + (newCoor[i].y1 - oldGraphicsInfo[i].y1);
			allOutputConnections[j]->setCornersLocation({ currentGfx.x1 ,currentGfx.y1,currentGfx.x2 ,currentGfx.y2 });
			Component* dstComp = allOutputConnections[j]->getDestPin()->getComponent();
			pOut->DrawConnection(currentGfx, allOutputConnections[j]->getDestPin()->getPosition(), { dstComp->getCenterLocation().x1, dstComp->getCenterLocation().y1,0,0 }, allOutputConnections[j]->getCellsBeforeAddingConnection(), false);

			for (size_t k = 0; k < allOutputConnections[j]->getCellsBeforeAddingConnection().size(); k++)
			{
				Utils::setArrayOfComponents(allOutputConnections[j]->getCellsBeforeAddingConnection()[k].y, allOutputConnections[j]->getCellsBeforeAddingConnection()[k].x, allOutputConnections[j]);
			}
		}
	}
}


MultiMove::~MultiMove()
{
}
