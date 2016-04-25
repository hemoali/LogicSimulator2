#include "RightClick.h"
#include "../Components/Connection.h"

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
	//pOut->PrintMsg("Right Select"); //Debugging Code

	//Check Which Component is Selected and gets a pointer to that Component
	for (int i = 0; i < pManager->allComponentsCorners.size(); i++) {
		//Checking For Coordinates of the Selected Gate 
		//ERROR HERE in CHECKING CONNECTION
		if ( (x >= pManager->allComponentsCorners[i].x1 && x <= pManager->allComponentsCorners[i].x2) && (
			y >= pManager->allComponentsCorners[i].y1 &&  y <= pManager->allComponentsCorners[i].y2)) {
			C = pManager->getComponent(i);
			i = pManager->allComponentsCorners.size(); //Breaks the Loop
		}
	}

	//Checking if the selected a Gate...
	if (C != NULL && dynamic_cast<Gate*>(C)) 
	{
		string s = "Gate : " + (C->getLabel()) + " is selected" ; 
		pOut->PrintMsg(s); 
		//Correct Point First call with false parameter (No Drawing)
		pOut->DrawRClickMenu_CorrectPoints(x, y, 1, false);
		//Take ScreenShot of the Original non Modified Region
		img = pOut->StoreBeforeMenu(x, y, 1);
		//Make the Component Highlited
		C->Draw(pOut, true);
		//Draw the Menu
		pOut->DrawRClickMenu_CorrectPoints(x, y, 1); //New ember Function to draw the relevant Menu
		MenuRectangle.x1 = x;
		MenuRectangle.x2 = x + UI.RightClickMenuLength;
		MenuRectangle.y1 = y;
		MenuRectangle.y2 = y + UI.RightClickMenuHeight;
		//New member function to wait for Selection Point.
		pIn->WaitSelectionPoint(x, y);
		//Check if the new Point is in the Menu Bar or Not
		if (x > MenuRectangle.x1 && x < MenuRectangle.x2 && y > MenuRectangle.y1 && y < MenuRectangle.y2) {
			//Get the Action Type and Save it in the data Member
			int theIconWidth = 30, actionNum = -1;
			y -= MenuRectangle.y1;
			actionNum = y / 30;
			SelectedAction = ActionType(actionNum + 37); //37 is the start of the needed actions in enum ActionType
		}
		//Delete the DrawMenu
		if (img != NULL)
			pOut->DrawAfterMenu(img, MenuRectangle.x1, MenuRectangle.y1, 1);
		//Remove the Highlighting
		C->Draw(pOut, false);
	}	
	else if (C != NULL && dynamic_cast<Connection*> (C)) {
		//Get Gates information And Print this info
		Connection* D = (Connection*)C;
		string pinSource, pinDestination;
		pinSource = D->getSourcePin()->getComponent()->getLabel();
		pinDestination = D->getDestPin()->getComponent()->getLabel();
		string s = "Connection between Gate: " + pinSource + " and Gate: " + pinDestination + " is selected";
		pOut->PrintMsg(s);
		//Correct Point First call with false parameter (No Drawing)
		pOut->DrawRClickMenu_CorrectPoints(x, y, 2, false);
		//Take ScreenShot of the Original non Modified Region
		img = pOut->StoreBeforeMenu(x, y, 2);
		//Make the Connection Highlited 
		pOut->changeConnectionColor(D);
		//Draw the Menu
		pOut->DrawRClickMenu_CorrectPoints(x, y, 2); //New ember Function to draw the relevant Menu
		MenuRectangle.x1 = x;
		MenuRectangle.x2 = x + UI.RightClickMenuLength;
		MenuRectangle.y1 = y;
		MenuRectangle.y2 = y + UI.RightClickCMenuH;
		//New member function to wait for Selection Point.
		pIn->WaitSelectionPoint(x, y);
		//Check if the new Point is in the Menu Bar or Not
		if (x > MenuRectangle.x1 && x < MenuRectangle.x2 && y > MenuRectangle.y1 && y < MenuRectangle.y2) {
			//Get the Action Type and Save it in the data Member
			int theIconWidth = 30, actionNum = -1;
			y -= MenuRectangle.y1;
			actionNum = y / 30;
			SelectedAction = (actionNum == 41) ? DEL:EDIT_Label; //41 is DEL number in THE Action Enum
		}

		//Make the Connection look Normal 
		pOut->changeConnectionColor(D,BLACK);
		//Delete the DrawMenu
		if (img != NULL)
			pOut->DrawAfterMenu(img, MenuRectangle.x1, MenuRectangle.y1, 2);
	}
	else if (C == NULL) {
		//User Right Clicked on a free space
		//Correct Point First call with false parameter (No Drawing)
		pOut->DrawRClickMenu_CorrectPoints(x, y, 3, false);
		//Take ScreenShot of the Original non Modified Region
		img = pOut->StoreBeforeMenu(x, y, 3);
		//Draw the Menu
		pOut->DrawRClickMenu_CorrectPoints(x, y, 3); //New ember Function to draw the relevant Menu
		MenuRectangle.x1 = x;
		MenuRectangle.x2 = x + UI.RightClickMenuLength;
		MenuRectangle.y1 = y;
		MenuRectangle.y2 = y + (UI.RightClickCMenuH/2);
		//New member function to wait for Selection Point.
		pIn->WaitSelectionPoint(x, y);
		//Check if the new Point is in the Menu Bar or Not
		if (x > MenuRectangle.x1 && x < MenuRectangle.x2 && y > MenuRectangle.y1 && y < MenuRectangle.y2) {
			//Get the Action Type and Save it in the data Member
			SelectedAction = PASTE;
			actionNum = 39;
		}

		//Delete the DrawMenu
		if (img != NULL)
			pOut->DrawAfterMenu(img, MenuRectangle.x1, MenuRectangle.y1, 3);
	}

	pOut->ClearStatusBar();
	if (actionNum == -1) return false;
	return true;
}

void RightClick::Execute() {
	bool x = ReadActionParameters();
	Output *pOut = pManager->GetOutput();
	if ( x ) {
		switch (SelectedAction)
		{
		case DEL: {
			pOut->DeleteGate(C);
			break;
		}
		case COPY:
			pOut->PrintMsg("Copy is Selected Remove ya Saleh when Finished");
			break;
		case CUT:
			pOut->PrintMsg("CUT is Selected Remove ya Saleh when Finished");
			break;
		case EDIT_Label:
			pOut->PrintMsg("Edit Label is Selected");
			break;
		case PASTE:
			pOut->PrintMsg("Paste is Selected Remove ya Saleh when Finished");
			break;
		default:
			break;
		}
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