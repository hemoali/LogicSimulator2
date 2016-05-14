#include "New.h"
#include"..\Components\Component.h"
#include"..\Components\Gate.h"
#include"..\ApplicationManager.h"
#include"..\Components\Connection.h"
#include <iostream>
using namespace std;
New::New(ApplicationManager*pApp) : Action(pApp)
{
	Loading = false;
}
bool New::ReadActionParameters(image *I)
{
	Output *pOut = pManager->GetOutput();
	int selected = pOut->printPopUpMessage("",'N');
	if (selected == 1) return true;
	return false;
}
void New::Execute()
{
	if (Loading || ReadActionParameters()) {
		Output *pOut = pManager->GetOutput();
		for (int i = 0; i < pManager->allComponentsCorners.size(); i++) {
			Component *C = pManager->getComponent(i);
			if (dynamic_cast<Gate*> (C)) {
				C->setDelete(true);
				C->Draw(pOut);
				//Removing Connections
				vector<Connection*> allInConnections, allOutConnections;
				C->getAllInputConnections(allInConnections);
				C->getAllOutputConnections(allOutConnections);
				pOut->clearConnections(allInConnections, C->getCenterLocation().x1, C->getCenterLocation().y1, true, true);
				pOut->clearConnections(allOutConnections, C->getCenterLocation().x1, C->getCenterLocation().y1, false, true);
			}
		}
		//Deleteion Completely
		for (int i = 0; i < pManager->allComponentsCorners.size(); i++) {
			Component *C = pManager->getComponent(i);
			delete C;
		}
		// clear status bar
		pOut->ClearStatusBar();
		//Clear Drawing Area
		pOut->ClearDrawingArea();
		//Resetting Interface
		pOut->resetInterfaceArrays();
		pManager->setCompCount(0);
		pManager->allComponentsCorners.resize(0);
		if (Loading)
			Loading = false;
	}
}
void New::Undo()
{

}
void New::Redo()
{

}
void New::setLoading(bool T)
{
	Loading = T;
}
New::~New()
{
}
