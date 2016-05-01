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
			c->setDelete(true);
			c->Draw(pManager->GetOutput());

			int xbegin = (c->getCenterLocation().x1 - UI.GATE_Width / 2.0) / UI.GRID_SIZE, xend = (c->getCenterLocation().x1 + UI.GATE_Width / 2.0) / UI.GRID_SIZE, ybegin = (c->getCenterLocation().y1 - UI.GATE_Height / 2.0) / UI.GRID_SIZE, yend = (c->getCenterLocation().y1 + UI.GATE_Height / 2.0) / UI.GRID_SIZE;
			for (int i = ybegin + 1; i <= yend; i++)
			{
				for (int j = xbegin; j <= xend; j++)
				{
					pOut->setUsedPixel(i, j, EMPTY);
					pOut->setArrayOfComponents(i, j, NULL);
				}
			}
		}

		if (pOut->SetMultiDragImage(x, y, comp, pIn->getSelectedComponents())) {
			for (int i = 0; i < pIn->getSelectedComponents().size(); i++)
			{
				pManager->allComponentsCorners[pIn->getSelectedComponents()[i].first].x1 = pIn->getSelectedComponents()[i].second->getCornersLocation().x1;
				pManager->allComponentsCorners[pIn->getSelectedComponents()[i].first].y1 = pIn->getSelectedComponents()[i].second->getCornersLocation().y1;
				pManager->allComponentsCorners[pIn->getSelectedComponents()[i].first].x2 = pIn->getSelectedComponents()[i].second->getCornersLocation().x2;
				pManager->allComponentsCorners[pIn->getSelectedComponents()[i].first].y2 = pIn->getSelectedComponents()[i].second->getCornersLocation().y2;

			}
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
						pOut->setArrayOfComponents(i, j, pIn->getSelectedComponents()[i].second);
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
{}

void MultiMove::Redo()
{}


MultiMove::~MultiMove()
{
}
