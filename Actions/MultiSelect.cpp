#include "MultiSelect.h"
#include "..\Utils.h"
#include "..\Components\\Connection.h"
#include<iostream>
MultiSelect::MultiSelect(ApplicationManager* pManager) :Action(pManager) {

}
bool MultiSelect::ReadActionParameters(image * smallImageBeforeAddingComponent, Component* c)
{
	return true;
}

void MultiSelect::Execute()
{
	//Get a Pointer to the Input / Output Interfaces
	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();
	if (pManager->GetInput()->getComponentToBeAddedToSelected() != NULL) // check if new comp to be added for the selected components
	{
		allSelectedComponents = pManager->GetInput()->getSelectedComponents();
		
		int idx = pManager->getComponentIndex(pManager->GetInput()->getComponentToBeAddedToSelected());
		allSelectedComponents.push_back(make_pair(idx, pManager->GetInput()->getComponentToBeAddedToSelected()));
		pManager->GetInput()->setComponentToBeAddedToSelected(NULL);
	}
	else if (pManager->GetInput()->getComponentToBeRemovedFromSelected() != NULL) {  // check if comp to be removed from the selected components
		allSelectedComponents = pManager->GetInput()->getSelectedComponents();
		// Removing highlighting

		for (size_t i = 0; i < Utils::allConnections.size(); i++)
		{
			if (!Utils::allConnections[i]->getDelete())
			{
				Utils::allConnections[i]->selectYourSelf(pOut, UI.ConnColor);
			}
		}
		for (size_t i = 0; i < pIn->getSelectedComponents().size(); i++)
		{
			pIn->getSelectedComponents()[i].second->Draw(pOut, false);
		}
		// remove the comp from selected vector
		for (size_t i = 0; i <allSelectedComponents.size(); i++)
		{
			if (allSelectedComponents.at(i).second == pManager->GetInput()->getComponentToBeRemovedFromSelected())
			{
				allSelectedComponents.erase(allSelectedComponents.begin() + i);
				break;
			}
		}
		pManager->GetInput()->setComponentToBeRemovedFromSelected(NULL);
	}
	else {
		// Removing highlighting
		for (size_t i = 0; i < Utils::allConnections.size(); i++)
		{
			Utils::allConnections[i]->selectYourSelf(pOut, UI.ConnColor);
		}

		for (size_t i = 0; i < pIn->getSelectedComponents().size(); i++)
		{
			if (!pIn->getSelectedComponents()[i].second->getDelete())
			{
				pIn->getSelectedComponents()[i].second->Draw(pOut, false);
			}
		}
		pIn->setSelectMode(false);
		pIn->setisSelectionContainConnections(false);
		pIn->clearSelectedComponents();

		int x, y, initX, initY;
		pIn->getMouseCoordinates(initX, initY); // get inital points for rectangle
		if (Utils::CheckPointInBorders(initX, initY))
		{
			// store screenshot for drawing area
			image* originalImage = new image;
			pOut->storeDrawingAreaImage(originalImage);

			int xOld = 0, yOld = 0; // used for stop flickering of the rectangle while holding at same position

			while (pIn->GetButtonStatus(LEFT_BUTTON, x, y) == BUTTON_DOWN) {
				if ((x != xOld || y != yOld) && Utils::CheckPointInBorders(x, y))
				{
					pOut->drawStoredDrawingAreaImage(originalImage);
					pOut->drawRectangle(x, y, initX, initY);
					xOld = x;
					yOld = y;
					//clear components of exists
					allSelectedComponents.clear();
					//Add components to selected vector
					for (size_t i = 0; i < pManager->getCompCount(); i++)
					{
						int gateCenterX = pManager->getComponent(i)->getCornersLocation().x1 + UI.GATE_Width / 2;
						int gateCenterY = pManager->getComponent(i)->getCornersLocation().y1 + UI.GATE_Height / 2;
						if (
							// Check if rectangle crossed the center
							(x > initX && gateCenterX < x && gateCenterX > initX && initY < y && gateCenterY < y && gateCenterY > initY) ||
							(x<initX && gateCenterX > x && gateCenterX < initX && initY < y && gateCenterY < y && gateCenterY > initY) ||
							(x<initX && gateCenterX > x && gateCenterX < initX && initY > y && gateCenterY > y && gateCenterY < initY) ||
							(x > initX && gateCenterX < x && gateCenterX > initX && initY > y && gateCenterY > y && gateCenterY < initY)
							) {
							if (!dynamic_cast<Connection*>(pManager->getComponent(i)) && !pManager->getComponent(i)->getDelete())
							{
								//Add to vector and highlught
								allSelectedComponents.push_back(make_pair(i, pManager->getComponent(i)));
								pManager->getComponent(i)->Draw(pOut, true);

								//Highlight all relative connections
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
						// Push and highlight connections
						if (dynamic_cast<Connection*> (pManager->getComponent(i)) && !pManager->getComponent(i)->getDelete())
						{
							Connection* conn = (Connection*)pManager->getComponent(i);
							for (size_t j = 0; j < conn->getCellsBeforeAddingConnection().size(); j++)
							{
								Cell c = conn->getCellsBeforeAddingConnection()[j];
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
			pOut->drawStoredDrawingAreaImage(originalImage); //restore old image after finishing 
		}
	}
	//if selected elements
	if (allSelectedComponents.size() > 0)
	{
		//hightlight all
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
		// set input parameters
		pIn->setSelectMode(true);
		pIn->setSelectedComponents(allSelectedComponents);
		Utils::undoActions.push(this);
	}
	Sleep(100);
}
void MultiSelect::Undo()
{
	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();

	if (allSelectedComponents.size() > 0)
	{
		//unhighlight all selected components
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

	//rehighlight selected components
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
		pIn->setSelectedComponents(allSelectedComponents);
	}
}
MultiSelect::~MultiSelect()
{}
