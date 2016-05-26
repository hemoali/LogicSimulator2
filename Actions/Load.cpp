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
	bool cleared = false;
	if (ReadActionParameters(NULL, NULL) && path != "") {
		// To clear the screen

		file.open(path);
		int compCount, connectionCount = 0, id;
		string compName, compLabel;
		GraphicsInfo point;
		//Loading Gates
		file >> compCount;
		for (int i = 0; i < compCount; i++)
		{
			if (!cleared)
			{
				newAction.setLoading(true);
				newAction.Execute();
				cleared = true;
			}
			file >> compName >> id >> point.x1 >> point.y1;
			//Completing the Component Corners
			point.x2 = point.x1 + UI.GATE_Width;
			point.y2 = point.y1 + UI.GATE_Height;
			//Leaving the componenet instantaiting for the application manager
			pManager->componentLoading(file, compName, point,id);

		}

		//Loading The Connection What've saved in connection 
		//the input pin Coordinates and the output pin Coordinates
		//Then I initiated the ADD CONNECTION Action With Silent Parameter to draw it without 
		// The need to press mouse buttons like normal
		file >> compName;
		file >> id;
		int c1, c2, c3, c4, I, O;
		c1 = c2 = c3 = c4 = 0;
		while(id != - 1)
		{
			// (c1,c2) Point of The output Pin (Source Pin)
			// (c3,c4) Point of The input pin (Dest pin)
			//file >> c1 >> c2 >> c3 >> c4 >> compLabel;
			file >> O >> I;
			Component *theInputPinComponent ;
			Component *theOutputPinComponent ;
			int inputPosistion = 3;
			for (int k = 0; k < pManager->getCompCount(); k++) {
				if (pManager->getComponent(k)->getID() == O)
					theOutputPinComponent = pManager->getComponent(k);
				else if( pManager->getComponent(k)->getID() == I)
					theInputPinComponent = pManager->getComponent(k);
			}
			
			//generating the pins Points by checking for empty input pins
			if (theInputPinComponent->getNumOfInputs() == 3) {
				if (theInputPinComponent->getInputPin(0)->getConnection() == NULL) {
					inputPosistion = 0;
				}
				else if (theInputPinComponent->getInputPin(1)->getConnection() == NULL) {
					inputPosistion = 1;
				}
				else if (theInputPinComponent->getInputPin(2)->getConnection() == NULL) {
					inputPosistion = 2;
				}
				
			}
			else if ( theInputPinComponent->getNumOfInputs() == 2) {
				if (theInputPinComponent->getInputPin(0)->getConnection() == NULL) {
					inputPosistion = 0;
				}
				else if (theInputPinComponent->getInputPin(1)->getConnection() == NULL) {
					inputPosistion = 2;
				}
			}
			else if (theInputPinComponent->getNumOfInputs() == 1) 
				if (theInputPinComponent->getInputPin(0)->getConnection() == NULL) {
					inputPosistion = 0;
				}
			int c1, c2, c3, c4;
			c1 = c2 = c3 = c4 = 0;
			switch (inputPosistion) {
			
			case 0:
				c3 = theInputPinComponent->getCenterLocation().x1 - 10;
				c4 = theInputPinComponent->getCenterLocation().y1 - 13;
				break;
			case 1:
				c3 = theInputPinComponent->getCenterLocation().x1 - 10;
				c4 = theInputPinComponent->getCenterLocation().y1;
				break;
			case 2:
				c3 = theInputPinComponent->getCenterLocation().x1 - 10;
				c4 = theInputPinComponent->getCenterLocation().y1 + 13;
				break;
			default:
				c3 = 0;
				c4 = 0;
				break;
			}
			c1 = theOutputPinComponent->getCenterLocation().x1 + 9;
			c2 = theOutputPinComponent->getCenterLocation().y1;
			//Adding Connection Action Silently
			AddConnection theConnection(pManager);
			//if (compLabel.size() == 1) {
			//	//Means that the label is empty as we have put an extra L 
			//	//char at the begining of te saved label to know whetherit has a name or not
			//	// in order to avoid misreading the input file
			//	compLabel = "";
			//}
			//else {
			//	compLabel = compLabel.substr(1, compLabel.size());
			//}
			compLabel = "";
			theConnection.AddConnectionSilent(c1, c2, c3, c4, compLabel);
			file >> id;
		}
		if (cleared)
		{
			pOut->PrintStatusBox("Design loaded successfully");
		}
		else {
			pOut->PrintStatusBox("Error Loading Design");
		}

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
