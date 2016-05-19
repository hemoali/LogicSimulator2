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
bool Load::ReadActionParameters(image *I)
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
	Clear* newAction = new Clear(pManager);
	Output *pOut = pManager->GetOutput();

	if (ReadActionParameters()) {
		// To clear the screen
		newAction->setLoading(true);
		newAction->Execute();
		file.open(path);
		int compCount, src, dest, pnum, connectionCount = 0;
		string compName, compLabel;
		GraphicsInfo point;
		Component* pA = 0;
		//New Image For creating Gate Object
		image* img = new image;
		//Loading Gates
		file >> compCount;
		for (int i = 0; i < compCount; i++)
		{
			file >> compName >> point.x1 >> point.y1;// >> compLabel;
			//if (compLabel.size() <= 1) {
			//	//Means that the label is empty as we have put an extra L 
			//	//char at the begining of te saved label to know whetherit has a name or not
			//	// in order to avoid misreading the input file
			//	compLabel = "";
			//}
			//else {
			//	compLabel = compLabel.substr(1, compLabel.size());
			//}
			//Completing the Component Corners
			point.x2 = point.x1 + UI.GATE_Width;
			point.y2 = point.y1 + UI.GATE_Height;
			if (compName == "AND2")
			{
				AND2*ptr = new AND2(point, 3);
				pA = ptr;
				pManager->componentLoading(file, ptr);
			}
			else if (compName == "AND3")
			{
				AND3*ptr = new AND3(point, 3);
				pA = ptr;
				pManager->componentLoading(file, ptr);
			}
			else if (compName == "BUFFER")
			{
				BUFFER*ptr = new BUFFER(point, 3);
				pA = ptr;
				pManager->componentLoading(file, ptr);
			}
			else if (compName == "LED")
			{
				LED*ptr = new LED(point, 3);
				pA = ptr;
				pManager->componentLoading(file, ptr);
			}
			else if (compName == "NAND2")
			{
				NAND2*ptr = new NAND2(point, 3);
				pA = ptr;
				pManager->componentLoading(file, ptr);
			}
			else if (compName == "NOR2")
			{
				NOR2*ptr = new NOR2(point, 3);
				pA = ptr;
				pManager->componentLoading(file, ptr);
			}
			else if (compName == "NOR3")
			{
				NOR3*ptr = new NOR3(point, 3);
				pA = ptr;
				pManager->componentLoading(file, ptr);
			}
			else if (compName == "NOT")
			{
				NOT*ptr = new NOT(point, 3);
				pA = ptr;
				pManager->componentLoading(file, ptr);
			}
			else if (compName == "OR2")
			{
				OR2*ptr = new OR2(point, 3);
				pA = ptr;
				pManager->componentLoading(file, ptr);
			}
			else if (compName == "SWITCH")
			{
				SWITCH*ptr = new SWITCH(point, 3);
				pA = ptr;
				pManager->componentLoading(file, ptr);
			}
			else if (compName == "XNOR2")
			{
				XNOR2*ptr = new XNOR2(point, 3);
				pA = ptr;
				pManager->componentLoading(file, ptr);
			}
			else if (compName == "XOR2")
			{
				XOR2*ptr = new XOR2(point, 3);
				pA = ptr;
				pManager->componentLoading(file, ptr);
			}
			else if (compName == "XOR3")
			{
				XOR3*ptr = new XOR3(point, 3);
				pA = ptr;
				pManager->componentLoading(file, ptr);
			}
			//Filling the needed arrays
			GraphicsInfo GInfotmp = point;
			int Len = UI.GATE_Width;
			int Wdth = UI.GATE_Height;
			Utils::allComponentsCorners.push_back(point);

			pOut->storeImage(img, pA->getCenterLocation().x1 - UI.GRID_SIZE - 5, pA->getCenterLocation().y1 - UI.GRID_SIZE - 5, 2 * UI.GRID_SIZE + 4, UI.GATE_Height + 3);

			pA->setSmallCleanImageBeforeAddingComp(img);
			for (int k = GInfotmp.y1 / UI.GRID_SIZE + 1; k <= GInfotmp.y2 / UI.GRID_SIZE; k++) {
				for (int l = GInfotmp.x1 / UI.GRID_SIZE; l <= GInfotmp.x2 / UI.GRID_SIZE; l++) {
					Utils::setArrayOfComponents(k, l, pA);
				}
			}

			//Don't FILL Used Pixels Now
			GraphicsInfo gfx = pA->getCornersLocation();
			Component* C = pA;
			int xbegin = (C->getCenterLocation().x1 - UI.GATE_Width / 2.0) / UI.GRID_SIZE;
			int xend = (C->getCenterLocation().x1 + UI.GATE_Width / 2.0) / UI.GRID_SIZE;
			int ybegin = (C->getCenterLocation().y1 - UI.GATE_Height / 2.0) / UI.GRID_SIZE;
			int yend = (C->getCenterLocation().y1 + UI.GATE_Height / 2.0) / UI.GRID_SIZE;

			for (int i = ybegin + 1; i <= yend; i++)
			{
				for (int j = xbegin; j <= xend; j++)
				{
					if (xbegin == j || xend == j)
					{
						if (pOut->getUsedPixel(i, j) != INTERSECTION)
						{
							pOut->setUsedPixel(i, j, PIN);
						}
						continue;
					}
					pOut->setUsedPixel(i, j, GATE);
				}
			}
			//Draw the Loaded Gate 
			pA->Draw(pOut);
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
			AddConnection *theConnection = new AddConnection(pManager);
			if (compLabel.size() == 1) {
				//Means that the label is empty as we have put an extra L 
				//char at the begining of te saved label to know whetherit has a name or not
				// in order to avoid misreading the input file
				compLabel = "";
			}
			else {
				compLabel = compLabel.substr(1, compLabel.size());
			}
			theConnection->AddConnectionSilent(c1, c2, c3, c4, compLabel);
		}
		//pManager->GetOutput()->PrintMsg("Design loaded successfully");
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
