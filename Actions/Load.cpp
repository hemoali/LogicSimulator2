#include "Load.h"
#include<string>
#include"..\Components\Component.h"
#include"..\Components\AND2.h"
#include"..\Components\AND3.h"
#include"..\Components\BUFFER.h"
#include"..\Components\LED.h"
#include"..\Components\NAND2.h"
#include"..\Components\NOR2.h"
#include"..\Components\NOR3.h"
#include"..\Components\NOT.h"
#include"..\Components\OR2.h"
#include"..\Components\SWITCH.h"
#include"..\Components\XNOR2.h"
#include"..\Components\XOR2.h"
#include"..\Components\XOR3.h"
#include"..\ApplicationManager.h"
#include"..\Components\Connection.h"
#include "AddConnection.h"
#include "New.h"
#include <iostream>
using namespace std;

Load::Load(ApplicationManager*pApp) : Action(pApp)
{
	path = "";
}
bool Load::ReadActionParameters(image *, Component* c)
{
	Output *pOut = pManager->GetOutput();
	Input *pIn = pManager->GetInput();
	/*
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
		else if (x > 329 && x < 350 && y >= 5 && y <=26) {
			finished = true;
		}
	}
	if (finished) {
		pOut->DrawAfterWarning(img);
		delete img;
	}
	if (draw) return true;
	return false;
	*/
	int selected = pOut->printPopUpMessage("Do you want to load? All unsaved progress will be lost.", 'L');
	if (selected == 1) {
		bool loadSuccess = false;
		loadSuccess = pOut->loadFile();
		path = pIn->getLoadPath();
		return true;
	}
	return false;
}
void Load::Execute()
{
	Clear newAction(pManager);
	Output *pOut = pManager->GetOutput();

	if (ReadActionParameters(NULL, NULL)) {
		// To clear the screen
		newAction.setLoading(true);
		newAction.Execute();
		file.open(path);
		int compCount, connectionCount = 0;
		string compName, compLabel;
		GraphicsInfo point;
		//Loading Gates
		file >> compCount;
		for (int i = 0; i < compCount; i++)
		{
			file >> compName >> point.x1 >> point.y1;
			//Completing the Component Corners
			point.x2 = point.x1 + UI.GATE_Width;
			point.y2 = point.y1 + UI.GATE_Height;
			//Leaving the componenet instantaiting for the application manager
			pManager->componentLoading(file, compName, point);

		}

		//Loading The Connection What've saved in connection 
		//the input pin Coordinates and the output pin Coordinates
		//Then I initiated the ADD CONNECTION Action With Silent Parameter to draw it without 
		// The need to press mouse buttons like normal
		file >> compName;
		file >> connectionCount;
		int c1, c2, c3, c4;
		c1 = c2 = c3 = c4 = 0;
		for (int l = 0; l < connectionCount; l++)
		{
			// (c1,c2) Point of The output Pin (Source Pin)
			// (c3,c4) Point of The input pin (Dest pin)
			file >> c1 >> c2 >> c3 >> c4 >> compLabel;
			//Adding Connection Action Silently
			AddConnection theConnection(pManager);
			if (compLabel.size() == 1) {
				//Means that the label is empty as we have put an extra L 
				//char at the begining of te saved label to know whetherit has a name or not
				// in order to avoid misreading the input file
				compLabel = "";
			}
			else {
				compLabel = compLabel.substr(1, compLabel.size());
			}
			theConnection.AddConnectionSilent(c1, c2, c3, c4, compLabel);
		}
		pOut->PrintStatusBox("Design loaded successfully");
		file.close();
		// clear undo/redo stacks
		while (!Utils::undoActions.empty()) Utils::undoActions.pop();
		while (!Utils::redoActions.empty()) Utils::redoActions.pop(); 
}

}
void Load::Undo()
{

}
void Load::Redo()
{

}
Load::~Load()
{
}
