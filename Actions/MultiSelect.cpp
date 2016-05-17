#include "MultiSelect.h"
#include "..\Utils.h"
#include<iostream>
MultiSelect::MultiSelect(ApplicationManager* pManager) :Action(pManager) {
	
}
bool MultiSelect::ReadActionParameters(image * smallImageBeforeAddingComponent)
{
	return true;
}

void MultiSelect::Execute()
{
	//Get a Pointer to the Input / Output Interfaces
	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();

	pIn->setSelectedComponents(&allSelectedComponents);
	int x, y, initX, initY;
	pIn->getMouseCoordinates(initX, initY);
	if (Utils::CheckPointInBorders(initX, initY))
	{
		image* originalImage = new image;
		pOut->storeDrawingAreaImage(originalImage);
		int xOld = 0, yOld = 0;

		while (pIn->GetButtonStatus(LEFT_BUTTON, x, y) == BUTTON_DOWN) {
			if ((x != xOld || y != yOld) && Utils::CheckPointInBorders(x, y))
			{
				pOut->drawStoredDrawingAreaImage(originalImage);
				pOut->drawRectangle(x, y, initX, initY);
				xOld = x;
				yOld = y;
				//Get components
				allSelectedComponents.clear();
				for (size_t i = 0; i < pManager->allComponentsCorners.size(); i++)
				{
					int gateCenterX = pManager->allComponentsCorners[i].x1 + UI.GATE_Width / 2;
					int gateCenterY = pManager->allComponentsCorners[i].y1 + UI.GATE_Height / 2;
					if (
						(x > initX && gateCenterX < x && gateCenterX > initX && initY < y && gateCenterY < y && gateCenterY > initY) ||
						(x<initX && gateCenterX > x && gateCenterX < initX && initY < y && gateCenterY < y && gateCenterY > initY) ||
						(x<initX && gateCenterX > x && gateCenterX < initX && initY > y && gateCenterY > y && gateCenterY < initY) ||
						(x > initX && gateCenterX < x && gateCenterX > initX && initY > y && gateCenterY > y && gateCenterY < initY)
						) {
						if (!dynamic_cast<Connection*>(pManager->getComponent(i)) && !pManager->getComponent(i)->getDelete())
						{
							allSelectedComponents.push_back(make_pair(i, pManager->getComponent(i)));
							pManager->getComponent(i)->Draw(pOut, true);
							vector<Connection*> allInConnections, allOutConnections;
							pManager->getComponent(i)->getAllInputConnections(allInConnections);
							pManager->getComponent(i)->getAllOutputConnections(allOutConnections);
							for (size_t i = 0; i < allInConnections.size(); i++)
							{
								allInConnections[i]->selectYourSelf(pOut, UI.SelectColor);
							}
							for (size_t i = 0; i < allOutConnections.size(); i++)
							{
								allOutConnections[i]->selectYourSelf(pOut, UI.SelectColor);
							}
						}
					}
					if (dynamic_cast<Connection*> (pManager->getComponent(i)) && !pManager->getComponent(i)->getDelete())
					{
						Connection* conn = (Connection*)pManager->getComponent(i);
						for (size_t j = 0; j < conn->getCellsBeforeAddingConnection().size(); j++)
						{
							Cell c =  conn->getCellsBeforeAddingConnection()[j];
							if (
								(x > initX && c.x *UI.GRID_SIZE< x &&  c.x*UI.GRID_SIZE > initX && initY < y &&  c.y*UI.GRID_SIZE < y &&  c.y*UI.GRID_SIZE > initY) ||
								(x<initX &&  c.x*UI.GRID_SIZE > x &&  c.x*UI.GRID_SIZE < initX && initY < y &&  c.y*UI.GRID_SIZE < y &&  c.y*UI.GRID_SIZE > initY) ||
								(x<initX &&  c.x*UI.GRID_SIZE > x &&  c.x*UI.GRID_SIZE < initX && initY > y &&  c.y *UI.GRID_SIZE > y &&  c.y*UI.GRID_SIZE < initY) ||
								(x > initX &&  c.x*UI.GRID_SIZE < x &&  c.x*UI.GRID_SIZE > initX && initY > y &&  c.y*UI.GRID_SIZE > y &&  c.y *UI.GRID_SIZE < initY)
								) {
								allSelectedComponents.push_back(make_pair(i, pManager->getComponent(i)));
								pManager->getComponent(i)->selectYourSelf(pOut, UI.SelectColor);
								pIn->setisSelectionContainConnections(true);
								break;
							}
						}
					}
				}
			}
		}
		pOut->drawStoredDrawingAreaImage(originalImage);
		if (allSelectedComponents.size() > 0)
		{
			for (size_t i = 0; i < allSelectedComponents.size(); i++)
			{
				if (dynamic_cast<Connection*>(allSelectedComponents[i].second))
				{
					allSelectedComponents[i].second->selectYourSelf(pOut, UI.SelectColor);
					continue;
				}
				allSelectedComponents[i].second->Draw(pOut, true);
				vector<Connection*> allInConnections, allOutConnections;
				allSelectedComponents[i].second->getAllInputConnections(allInConnections);
				allSelectedComponents[i].second->getAllOutputConnections(allOutConnections);
				for (size_t i = 0; i < allInConnections.size(); i++)
				{
					allInConnections[i]->selectYourSelf(pOut, UI.SelectColor);
				}
				for (size_t i = 0; i < allOutConnections.size(); i++)
				{
					allOutConnections[i]->selectYourSelf(pOut, UI.SelectColor);
				}
			}

			pIn->setSelectMode(true);
			Utils::undoActions.push(this);
		}
	}

}
void MultiSelect::Undo()
{
	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();

	if (allSelectedComponents.size() > 0)
	{
		for (size_t i = 0; i < allSelectedComponents.size(); i++)
		{
			if (dynamic_cast<Connection*>(allSelectedComponents[i].second))
			{
				allSelectedComponents[i].second->selectYourSelf(pOut, UI.DrawColor);
				continue;
			}
			allSelectedComponents[i].second->Draw(pOut, false);

			vector<Connection*> allInConnections, allOutConnections;
			allSelectedComponents[i].second->getAllInputConnections(allInConnections);
			allSelectedComponents[i].second->getAllOutputConnections(allOutConnections);
			for (size_t i = 0; i < allInConnections.size(); i++)
			{
				allInConnections[i]->selectYourSelf(pOut, UI.DrawColor);
			}
			for (size_t i = 0; i < allOutConnections.size(); i++)
			{
				allOutConnections[i]->selectYourSelf(pOut, UI.DrawColor);
			}
		}

		pIn->setSelectMode(false);
		pIn->clearSelectedComponents();
	}
}

void MultiSelect::Redo()
{
	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();

	if (allSelectedComponents.size() > 0)
	{
		for (size_t i = 0; i < allSelectedComponents.size(); i++)
		{
			if (dynamic_cast<Connection*>(allSelectedComponents[i].second))
			{
				allSelectedComponents[i].second->selectYourSelf(pOut, UI.SelectColor);
				continue;
			}
			allSelectedComponents[i].second->Draw(pOut, true);

			vector<Connection*> allInConnections, allOutConnections;
			allSelectedComponents[i].second->getAllInputConnections(allInConnections);
			allSelectedComponents[i].second->getAllOutputConnections(allOutConnections);
			for (size_t i = 0; i < allInConnections.size(); i++)
			{
				allInConnections[i]->selectYourSelf(pOut, UI.SelectColor);
			}
			for (size_t i = 0; i < allOutConnections.size(); i++)
			{
				allOutConnections[i]->selectYourSelf(pOut, UI.SelectColor);
			}
		}

		pIn->setSelectMode(true);
	}
}
MultiSelect::~MultiSelect()
{}
