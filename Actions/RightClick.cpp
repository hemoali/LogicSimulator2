#include "RightClick.h"


RightClick::RightClick(ApplicationManager* pApp) :Action(pApp)
{
	C = NULL;
	theAction = NULL;
}

bool RightClick::ReadActionParameters(image* img, Component* c) {

	int x, y; //Point Coordinates
	GraphicsInfo MenuRectangle; //the Menu Region
	Output *pOut = pManager->GetOutput(); //Pointer to Output Class
	Input *pIn = pManager->GetInput(); //Pointer to Input Class
	pIn->getSelectionPoint(x, y); //New Member Function in input Class to get the selected points coordinates
	//pOut->PrintStatusBox("Right Select"); //Debugging Code

	//Checking for MultiSelect having some Gates Or MultiSlect With No Gates(i.e. Connections Only)  
	//This Code will be kept if we are supporting MultiSelect Components
	int atLeastOneGateinMS = 0;
	bool allAreOnlyConnections = false;
	if (pIn->getSelectMode()) {
		vector<pair<int, Component*>> V = pIn->getSelectedComponents();
		for (int i = 0; i < V.size(); i++) {
			if (V[i].second->getComponentActionType() != ADD_CONNECTION)
				atLeastOneGateinMS++;
		}
	}

	//Check Which Component is Selected and gets a pointer to that Component
	for (int i = 0; i < Utils::allComponentsCorners.size(); i++) {
		//Checking For Coordinates of the Selected Gate 
		if ((x >= Utils::allComponentsCorners[i].x1 && x <= Utils::allComponentsCorners[i].x2) && (
			y >= Utils::allComponentsCorners[i].y1 &&  y <= Utils::allComponentsCorners[i].y2)) {
			C = pManager->getComponent(i);
			i = Utils::allComponentsCorners.size(); //Breaks the Loop
		}
	}

	//Checking if the selected a Gate / Switch /LED...
	if (C != NULL && ((dynamic_cast<Gate*>(C) || dynamic_cast<LED*> (C) || dynamic_cast<SWITCH*>(C)) && !C->getDelete()))
	{
		string s = "Gate : " + (C->getLabel()) + " is selected";
		pOut->PrintStatusBox(s);
		//Correct Point First call with false parameter (No Drawing)
		if (pIn->getSelectMode())
			pOut->DrawRClickMenu_CorrectPoints(x, y, 5, false);
		else
			pOut->DrawRClickMenu_CorrectPoints(x, y, 1, false);
		//Take ScreenShot of the Original non Modified Region
		if (pIn->getSelectMode())
			img = pOut->StoreBeforeMenu(x, y, 5);
		else 
			img = pOut->StoreBeforeMenu(x, y, 1);
		//Make the Component Highlited
		C->Draw(pOut, true);
		//Draw the Menu
		if (pIn->getSelectMode())
			pOut->DrawRClickMenu_CorrectPoints(x, y, 5, true);
		else
			pOut->DrawRClickMenu_CorrectPoints(x, y, 1, true); //New ember Function to draw the relevant Menu
		MenuRectangle.x1 = x;
		MenuRectangle.x2 = x + UI.RightClickMenuLength;
		MenuRectangle.y1 = y;
		if (pIn->getSelectMode()) 
			MenuRectangle.y2 = y + UI.RightClickCMenuH/2;
		else
			MenuRectangle.y2 = y + UI.RightClickMenuHeight;
		//New member function to wait for Selection Point.
		pIn->WaitSelectionPoint(x, y);
		//Check if the new Point is in the Menu Bar or Not
		if (x > MenuRectangle.x1 && x < MenuRectangle.x2 && y > MenuRectangle.y1 && y < MenuRectangle.y2) {
			//Get the Action Type and Save it in the data Membere
			if (pIn->getSelectMode())
				SelectedAction = Multi_Del;
			else {
				int theIconWidth = 30, actionNum = -1;
				y -= MenuRectangle.y1;
				actionNum = y / 30;
				SelectedAction = ActionType(actionNum + COPYAction); //Copy is the start of the needed actions in enum ActionType
				
			}
		}
		//Delete the DrawMenu
		if (img != NULL) {
			if (pIn->getSelectMode())
				pOut->DrawAfterMenu(img, MenuRectangle.x1, MenuRectangle.y1, 5);
			else
				pOut->DrawAfterMenu(img, MenuRectangle.x1, MenuRectangle.y1, 1);
		}
		//Remove the Highlighting
		if(! pIn->getSelectMode())
			C->Draw(pOut, false);
		else {
			vector<pair<int, Component*>> V = pIn->getSelectedComponents();
			for (int i = 0; i < V.size(); i++) {
				V[i].second->Draw(pOut, false);
			}
			vector <Connection*> allConnections = Utils::allConnections;

			for (size_t i = 0; i < allConnections.size(); i++)
			{
				allConnections[i]->selectYourSelf(pOut, UI.ConnColor);
			}
			pIn->setSelectMode(false);
		}
		
		if (actionNum != -1) return true;
	}

	//Checking for Connectioon
	pIn->getExactConnectionLocation(x, y);
	C =  pIn->getArrOfComponents(y / UI.GRID_SIZE, x / UI.GRID_SIZE);
	if (C != NULL && dynamic_cast<Connection*> (C)) {
		if (pIn->getSelectMode() && atLeastOneGateinMS != 0 ) {
			vector<pair<int, Component*>> V = pIn->getSelectedComponents();
			for (int i = 0; i < V.size(); i++) {
				V[i].second->Draw(pOut, false);
			}
			vector <Connection*> allConnections = Utils::allConnections;

			for (size_t i = 0; i < allConnections.size(); i++)
			{
				allConnections[i]->selectYourSelf(pOut, UI.ConnColor);
			}
			pIn->setSelectMode(false);
		}
		//Get Gates information And Print this info
		Connection* D = (Connection*)C;
		string pinSource, pinDestination;
		pinSource = D->getSourcePin()->getComponent()->getLabel();
		pinDestination = D->getDestPin()->getComponent()->getLabel();
		if (!pIn->getSelectMode()) {
			string s = "Connection between Gate: " + pinSource + " and Gate: " + pinDestination + " is selected";
			pOut->PrintStatusBox(s);
		}
		//Correct Point First call with false parameter (No Drawing)
		if (pIn->getSelectMode())
			pOut->DrawRClickMenu_CorrectPoints(x, y, 5, false);
		else pOut->DrawRClickMenu_CorrectPoints(x, y, 2, false);
		//Take ScreenShot of the Original non Modified Region
		if (pIn->getSelectMode())
			img = pOut->StoreBeforeMenu(x, y, 5);
		else img = pOut->StoreBeforeMenu(x, y, 2);
		//Make the Connection Highlited 
		if (pIn->getSelectMode())
		{	
			vector<pair<int, Component*>> V = pIn->getSelectedComponents();
			for (int i = 0; i < V.size(); i++) {
				pOut->changeConnectionColor((Connection*)(V[i].second));
			}
		}
		else pOut->changeConnectionColor(D);
		//Draw the Menu
		if (pIn->getSelectMode())
			pOut->DrawRClickMenu_CorrectPoints(x, y, 5);
		else pOut->DrawRClickMenu_CorrectPoints(x, y, 2); //New ember Function to draw the relevant Menu
		MenuRectangle.x1 = x;
		MenuRectangle.x2 = x + UI.RightClickMenuLength;
		MenuRectangle.y1 = y;
		if (pIn->getSelectMode())
			MenuRectangle.y2 = y + UI.RightClickCMenuH / 2;
		else MenuRectangle.y2 = y + UI.RightClickCMenuH + UI.RightClickCMenuH/2;
		//New member function to wait for Selection Point.
		pIn->WaitSelectionPoint(x, y);
		//Check if the new Point is in the Menu Bar or Not
		if (x > MenuRectangle.x1 && x < MenuRectangle.x2 && y > MenuRectangle.y1 && y < MenuRectangle.y2) {
			if (pIn->getSelectMode()) {
				SelectedAction = Multi_Del;
			}
			else {
				//Get the Action Type and Save it in the data Member
				int theIconWidth = 30, actionNum = -1;
				y -= MenuRectangle.y1;
				actionNum = y / 30;
				switch (actionNum) {
				case 0:
					SelectedAction = EDIT_Label;
					break;
				case 1:
					SelectedAction = Modify_Connection;
					break;
				case 2:
					SelectedAction = DEL;
					break;
				default:
					actionNum = -1;
				}
			}
		}
		//Make the Connection look Normal 
		if (pIn->getSelectMode())
		{
			vector<pair<int, Component*>> V = pIn->getSelectedComponents();
			for (int i = 0; i < V.size(); i++) {
				pOut->changeConnectionColor(((Connection*)(V[i].second)), UI.ConnColor); 
			}
		}
		else pOut->changeConnectionColor(D, UI.ConnColor);
		//Delete the DrawMenu
		if (img != NULL) {
			if (pIn->getSelectMode()) {
				pOut->DrawAfterMenu(img, MenuRectangle.x1, MenuRectangle.y1, 5);
			}
			else pOut->DrawAfterMenu(img, MenuRectangle.x1, MenuRectangle.y1, 2);
		}
		
		if (pIn->getSelectMode()) {
			vector<pair<int, Component*>> V = pIn->getSelectedComponents();
			for (int i = 0; i < V.size(); i++) {
				V[i].second->Draw(pOut, false);
			}
			vector <Connection*> allConnections = Utils::allConnections;

			for (size_t i = 0; i < allConnections.size(); i++)
			{
				allConnections[i]->selectYourSelf(pOut, UI.ConnColor);
			}
			pIn->setSelectMode(false);
		}
		if (actionNum != -1) return true;
	}
	if (C == NULL) {
		pManager->pastepoint.x1 = x; pManager->pastepoint.y1 = y;
		//User Right Clicked on a free space
		if (pIn->getSelectMode()) {
			vector<pair<int, Component*>> V = pIn->getSelectedComponents();
			for (int i = 0; i < V.size(); i++) {
				V[i].second->Draw(pOut, false);
			}
			vector <Connection*> allConnections = Utils::allConnections;

			for (size_t i = 0; i < allConnections.size(); i++)
			{
				allConnections[i]->selectYourSelf(pOut, UI.ConnColor);
			}
			pIn->setSelectMode(false);
		}
		//Correct Point First call with false parameter (No Drawing)
		pOut->DrawRClickMenu_CorrectPoints(x, y, 3, false);
		//Take ScreenShot of the Original non Modified Region
		img = pOut->StoreBeforeMenu(x, y, 3);
		//Draw the Menu
		pOut->DrawRClickMenu_CorrectPoints(x, y, 3); //New ember Function to draw the relevant Menu
		MenuRectangle.x1 = x;
		MenuRectangle.x2 = x + UI.RightClickMenuLength;
		MenuRectangle.y1 = y;
		MenuRectangle.y2 = y + (UI.RightClickCMenuH / 2);
		//New member function to wait for Selection Point.
		pIn->WaitSelectionPoint(x, y);
		//Check if the new Point is in the Menu Bar or Not
		if (x > MenuRectangle.x1 && x < MenuRectangle.x2 && y > MenuRectangle.y1 && y < MenuRectangle.y2) {
			//Get the Action Type and Save it in the data Member
			SelectedAction = PASTEAction;
			actionNum = 39;
		}

		//Delete the DrawMenu
		if (img != NULL)
			pOut->DrawAfterMenu(img, MenuRectangle.x1, MenuRectangle.y1, 3);
		
		
		if (actionNum != -1) return true;
	}

	if (actionNum == -1) return false;
	return true;
}


void RightClick::Execute() {
	bool x = ReadActionParameters();
	Output *pOut = pManager->GetOutput();
	if (x) {
		switch (SelectedAction)
		{
		case COPYAction:
			pOut->PrintStatusBox("Components Copied");
			theAction = new Copy(pManager, C);
			break;
		case CUTAction:
			pOut->PrintStatusBox("Components Cut");
			theAction = new Cut(pManager, C);
			break;
		case PASTEAction:
			pOut->PrintStatusBox("Components Pasted");
			theAction = new Paste(pManager);
			break;
		case EDIT_Label: {
			theAction = new EditLabel(pManager, C);
			break;
		}
		case DEL: {
			theAction = new Delete(pManager,C);
			break;
		}
		case Modify_Connection: {
			theAction = new ModifyConnection(pManager,(Connection*)(C));
			break;
		}
		case Multi_Del: {
			Input *pIn = pManager->GetInput();
			pOut->PrintStatusBox("Mult Delete is Selected");
			theAction = new MultiDelete(pManager,pIn->getSelectedComponents());
			break;
		}
		default:
			break;
		}
	}
}


void RightClick::Undo()
{
	//Nothing to do here 
}
void RightClick::Redo()
{
	//Nothing to do here
}
Action * RightClick::getAction()
{
	return theAction;
}
RightClick::~RightClick()
{
}

ActionType RightClick::getSelectedAction() {
	return SelectedAction;
}