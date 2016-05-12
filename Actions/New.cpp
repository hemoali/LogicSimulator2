#include "New.h"
#include"..\Components\Component.h"
#include"..\Components\Gate.h"
#include"..\ApplicationManager.h"
#include"..\Components\Connection.h"
#include <iostream>
using namespace std;
New::New(ApplicationManager*pApp) : Action(pApp)
{

}
bool New::ReadActionParameters(image *I)
{
	Output *pOut = pManager->GetOutput();
	Input *pIn = pManager->GetInput();
	image *img = pOut->StoreBeforeWarning();
	int x, y;
	pOut->DrawWarningMenues('L');
	cout << '\a' << endl;
	bool finished = false, draw = false;
	while (!finished) {

		pIn->WaitSelectionPoint(x, y);
		int xbegin, xend, ybegin, yend;
		xbegin = UI.width / 2 - UI.WarningMenuWidth / 2;
		ybegin = UI.height / 2 - UI.WarningMenuHeight / 2;
		x -= xbegin;
		y -= ybegin;
		if (x > 106 && x < 168 && y > 102 && y < 128) {
			finished = true;
			draw = true;
		}
		else if (x > 184 && x < 242 && y > 102 && y < 128) {
			finished = true;
		}
		else if (x > 329 && x < 350 && y >= 5 && y <= 26) {
			finished = true;
		}
	}
	if (finished) {
		pOut->DrawAfterWarning(img);
		delete img;
	}
	if (draw) return true;
	return false;
}
void New::Execute()
{
	if (ReadActionParameters(NULL)) {

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
				//Deleteion Completely
				delete C;
			}
		}
		// clear status bar
		pOut->ClearStatusBar();
		//Clear Drawing Area
		pOut->ClearDrawingArea();
		//Resetting Interface
		pOut->resetInterfaceArrays();
		pManager->setCompCount(0);
		pManager->allComponentsCorners.resize(0);
	}
}
void New::Undo()
{

}
void New::Redo()
{

}
New::~New()
{
}
