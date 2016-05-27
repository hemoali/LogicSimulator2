#include "Select.h"
#include "..\ApplicationManager.h"
#include<iostream>
#include "..\Components\Component.h"
#include "..\Components\Connection.h"
using namespace std;

Select::Select(ApplicationManager *pApp) :Action(pApp)
{
	theSelectedComponent = NULL;
}

bool Select::ReadActionParameters(image * smallImageBeforeAddingComponent, Component* c)
{
	//Get a Pointer to the Input / Output Interfaces
	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();
	int x, y, ii;
	pIn->getMouseCoordinates(x, y);
	Utils::correctPointClicked(x, y, true);
	theSelectedComponent = pManager->getComponentByCoordinates(x, y, true, true, ii);
	return true;
}

void Select::Execute()
{
	ReadActionParameters(NULL, NULL);
	Output* pOut = pManager->GetOutput();
	if (theSelectedComponent!= NULL)
	{
		theSelectedComponent->Draw(pOut, true);

		// Select the connected with  normal color after releasing the moved gate
		vector<Connection*> allInConnections, allOutConnections;
		theSelectedComponent->getAllInputConnections(allInConnections);
		theSelectedComponent->getAllOutputConnections(allOutConnections);
		for (size_t i = 0; i < allOutConnections.size(); i++)
		{
			allOutConnections[i]->selectYourSelf(pOut, UI.SelectColor);
		}
		for (size_t i = 0; i < allInConnections.size(); i++)
		{
			allInConnections[i]->selectYourSelf(pOut, UI.SelectColor);
		}
		Utils::undoActions.push(this);
		Action::pA = theSelectedComponent;
	}
}

void Select::Undo()
{
	Output* pOut = pManager->GetOutput();
	theSelectedComponent->Draw(pOut, false);
	// Select the connected with  normal color after releasing the moved gate
	vector<Connection*> allInConnections, allOutConnections;
	theSelectedComponent->getAllInputConnections(allInConnections);
	theSelectedComponent->getAllOutputConnections(allOutConnections);
	for (size_t i = 0; i < allOutConnections.size(); i++)
	{
		allOutConnections[i]->selectYourSelf(pOut, UI.ConnColor);
	}
	for (size_t i = 0; i < allInConnections.size(); i++)
	{
		allInConnections[i]->selectYourSelf(pOut, UI.ConnColor);
	}
	Utils::undoActions.push(this);
}

void Select::Redo()
{
	Output* pOut = pManager->GetOutput();
	if (theSelectedComponent != NULL)
	{
		theSelectedComponent->Draw(pOut, true);

		// Select the connected with  normal color after releasing the moved gate
		vector<Connection*> allInConnections, allOutConnections;
		theSelectedComponent->getAllInputConnections(allInConnections);
		theSelectedComponent->getAllOutputConnections(allOutConnections);
		for (size_t i = 0; i < allOutConnections.size(); i++)
		{
			allOutConnections[i]->selectYourSelf(pOut, UI.SelectColor);
		}
		for (size_t i = 0; i < allInConnections.size(); i++)
		{
			allInConnections[i]->selectYourSelf(pOut, UI.SelectColor);
		}
		Utils::undoActions.push(this);
		Action::pA = theSelectedComponent;
	}
}


Select::~Select()
{
}
