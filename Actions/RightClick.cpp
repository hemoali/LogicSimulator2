#include "RightClick.h"


RightClick::RightClick(ApplicationManager* pApp) :Action(pApp)
{
	C = NULL;
}


bool RightClick::ReadActionParameters(image* img) {
	int x, y; //Point Coordinates
	GraphicsInfo MenuRectangle; //the Menu Region
	Output *pOut = pManager->GetOutput(); //Pointer to Output Class
	Input *pIn = pManager->GetInput(); //Pointer to Input Class
	pIn->getSelectionPoint(x, y); //New Member Function in input Class to get the selected points coordinates
	pOut->PrintMsg("Right Select"); //Debugging Code

	//Check Which Component is Selected and gets a pointer to that Component
	for (int i = 0; i < pManager->allComponentsCorners.size(); i++) {
		//Checking For Coordinates of the Selected Gate 
		if ( (x >= pManager->allComponentsCorners[i].x1 && x <= pManager->allComponentsCorners[i].x2) && (
			y >= pManager->allComponentsCorners[i].y1 &&  y <= pManager->allComponentsCorners[i].y2)) {
			C = pManager->getComponent(i);
			i = pManager->allComponentsCorners.size(); //Breaks the Loop
		}
	}
	//Checking if the selected a Gate...
	if (C != NULL && dynamic_cast<Gate*>(C)) {
		string s = "A Gate is selected"; //Debugging Code 
		pOut->PrintMsg(s); //Debugging Code
		//Correct Point First call with false parameter (No Drawing)
		pOut->DrawRClickMenu_CorrectPoints(x, y, 1, false);
		//Take ScreenShot of the Original non Modified Region
		img = pOut->StoreBeforeMenu(x, y);
		//Draw the Menu
		pOut->DrawRClickMenu_CorrectPoints(x, y, 1); //New Member Function to draw the relevant Menu
		MenuRectangle.x1 = x;
		MenuRectangle.x2 = x + UI.RightClickMenuLength;
		MenuRectangle.y1 = y;
		MenuRectangle.y2 = y + UI.RightClickMenuWidth;
	/*	s = to_string(x) + "    " + to_string(y);
		pOut->PrintMsg(s);*/
		C->Draw(pOut, true);
		pIn->WaitSelectionPoint(x, y); //New Member Function to wait for Selection Point.
		//Check if the new Point is in the Menu Bar or Not
		if (x > MenuRectangle.x1 && x < MenuRectangle.x2) {
			//Get the Action Type and Save it in the data Member
			int inc = 8, theIconWidth = 30, lastEnd = 0, actionNum = -1;
			y -= MenuRectangle.y1;
			for (int i = 0; i < 5; i++) {
				if (i == 2) inc = 4;
			
				if (y > lastEnd &&  y < lastEnd + inc + theIconWidth) {
					actionNum = i;
					i = 6;
				}
				lastEnd += theIconWidth+ inc;
			}
			SelectedAction = ActionType(actionNum + 37); //37 is the start of the needed actions in enum ActionType
		}
		else {
			
		}
	}	
	//Delete the DrawMenu
	if( img != NULL)
		pOut->DrawAfterMenu(img, MenuRectangle.x1, MenuRectangle.y1);
	pOut->ClearStatusBar();
	if (C == NULL) return false;
	return true;
}

void RightClick::Execute() {
	bool x = ReadActionParameters();
	Output *pOut = pManager->GetOutput();
	if (x && actionNum != -1) {
		switch (SelectedAction)
		{
		case DEL: {
			pOut->PrintMsg("Delete is Selected");
			C->setDelete(true);
			C->Draw(pOut);
			GraphicsInfo gfx = C->getCornersLocation();
			int xbegin = (C->getCenterLocation().x1 - UI.GATE_Width / 2.0) / UI.GRID_SIZE; 
			int xend = (C->getCenterLocation().x1 + UI.GATE_Width / 2.0) / UI.GRID_SIZE; 
			int ybegin = (C->getCenterLocation().y1 - UI.GATE_Height / 2.0) / UI.GRID_SIZE;
			int yend = (C->getCenterLocation().y1 + UI.GATE_Height / 2.0) / UI.GRID_SIZE;
			for (int i = ybegin + 1; i <= yend; i++)
			{
				for (int j = xbegin; j <= xend; j++)
				{
					pOut->setUsedPixel(i, j, EMPTY);
					pOut->setArrayOfComponents(i, j, NULL);
				}
			}

			// Needed  Removing Connection
			vector<Connection*> allInConnections, allOutConnections;
			C->getAllInputConnections(allInConnections);
			C->getAllOutputConnections(allOutConnections);
			
			pOut->clearConnections(allInConnections, C->getCenterLocation().x1, C->getCenterLocation().y1, true, true);
			pOut->clearConnections(allOutConnections, C->getCenterLocation().x1, C->getCenterLocation().y1, false, true);

			//Remove From All Component
			//Delete the Component  Selected 
			/*int index = -1, vectorSize = pManager->allComponentsCorners.size();
			for (int i = 0; i < pManager->allComponentsCorners.size(); i++) {
				if (C == pManager->getComponent(i)) {
					index = i;
					i = pManager->allComponentsCorners.size();
				}
			}
			pManager->allComponentsCorners[index] = pManager->allComponentsCorners[vectorSize - 1];
			pManager->allComponentsCorners.pop_back();*/
			break;
		}
		case COPY:
			pOut->PrintMsg("Copy is Selected");
			break;
		case CUT:
			pOut->PrintMsg("CUT is Selected");
			break;
		case EDIT_Label:
			pOut->PrintMsg("Edit Label is Selected");
			break;
		case PASTE:
			pOut->PrintMsg("Paste is Selected");
			break;
		default:
			break;
		}
		C->Draw(pOut, false);

	}
}
void RightClick::Undo()
{
}
void RightClick::Redo()
{
}
RightClick::~RightClick()
{
}

ActionType RightClick::getSelectedAction() {
	return SelectedAction;
}