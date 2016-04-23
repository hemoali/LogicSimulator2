#include "Input.h"
#include "Output.h"
#include "..\Utils.h"
Input::Input(window* pW)
{
	pWind = pW; //point to the passed window
}

void Input::GetPointClicked(int &x, int &y, bool DrawGate, bool DrawConnection)
{
	pWind->WaitMouseClick(x, y);	//Wait for mouse click
	Utils::correctPointClicked(x, y, DrawGate, DrawConnection);

}
void Input::getSelectionPoint(int & x, int & y)
{
	pWind->GetMouseClick(x, y);
	pWind->FlushMouseQueue(); 
}
buttonstate Input::GetButtonStatus(const button btMouse, int &iX, int &iY) const{
	return pWind->GetButtonState(btMouse, iX, iY);
}
string Input::GetSrting(Output *pOut, string sOriginal = "")
{
	string s = "";
	char ch;
	keytype k;
	while ((k = pWind->WaitKeyPress(ch)) != '\n' && (int)ch != 13) {
		if (k == ESCAPE) {
			s = "";
			pOut->PrintMsg(sOriginal);
		}
		else if (k == ASCII && (int)ch == 8) {
			s = s.substr(0, s.size() - 1);
		}
		else if (k == ASCII && (int)ch != 27) {
			s += ch;
		}
		pOut->PrintMsg(sOriginal + " " + s);
	}
	pOut->PrintMsg("");
	pWind->FlushMouseQueue();
	return s;
}

//This function reads the position where the user clicks to determine the desired action
ActionType Input::GetUserAction(ApplicationManager *pManager) const
{
	int x = 0, y = 0, xT, yT;
	while (true) {
		if (pWind->GetButtonState(LEFT_BUTTON, xT, yT) == BUTTON_DOWN && yT >= UI.ToolBarHeight && yT < UI.height - UI.StatusBarHeight) {
			Component* comp = NULL;
			for (int i = 0; i < pManager->allComponentsCorners.size(); i++)
			{
				if (dynamic_cast<Connection*>(pManager->getComponent(i)))
					continue;
				if (xT >= pManager->allComponentsCorners[i].x1&&xT <= pManager->allComponentsCorners[i].x2&& yT >= pManager->allComponentsCorners[i].y1&&yT <= pManager->allComponentsCorners[i].y2)
				{
					comp = pManager->getComponent(i);
				}
			}
			//
			if (comp != NULL &&comp->getDelete()) comp = NULL;
			if (comp != NULL)
				return MOVE;
			else {
				if (pManager->GetOutput()->getAllPixels(yT, xT) != NULL)
				{
					pManager->GetOutput()->getAllPixels(yT, xT)->selectYourSelf(pManager->GetOutput() ,RED);
				}
				else {
					vector <Connection*> allConnections;
					pManager->getAllConnections(allConnections);
					for (size_t i = 0; i < allConnections.size(); i++)
					{
						allConnections[i]->selectYourSelf(pManager->GetOutput(), color(23, 79, 181));
					}
					return MULTI_SELECT;
				}
			}
		}
		else {
			break;
		}
	}

	clicktype s = LEFT_CLICK;
	s = pWind->GetMouseClick(x, y, false);	//Get the coordinates of the user click 
	// We Called it with false argument inorder not to delete the click to be used in RightSelect
	//Otherwise we call that function agian with true to delete that click 

	if (UI.AppMode == DESIGN)	//application is in design mode
	{
		//[1] If user clicks on the Toolbar
		if (y >= 0 && y < UI.ToolBarHeight)
		{
			pWind->GetMouseClick(x, y);
			//TODO
			//Check whick Menu item was clicked
			//==> This assumes that menu items are lined up horizontally <==


			int ClickedItemOrder = -1;//TODO:Modify
			for (; x > 0; ClickedItemOrder++)
			{
				x -= (UI.ToolItemWidth + 5);
			}
			//Divide x coord of the point clicked by the menu item width (int division)
			//if division result is 0 ==> first item is clicked, if 1 ==> 2nd item and so on
			if (s == LEFT_CLICK)
			{
				switch (ClickedItemOrder)
				{
				case D2AND:  return ADD_AND_GATE_2;
				case D3AND:  return ADD_AND_GATE_3;
				case D2OR: return ADD_OR_GATE_2;
				case D2XOR: return ADD_XOR_GATE_2;
				case D3XOR: return ADD_XOR_GATE_3;
				case DBUFFER: return ADD_Buff;
				case DNOT: return ADD_INV;
				case D2NAND: return ADD_NAND_GATE_2;
				case D2NOR: return ADD_NOR_GATE_2;
				case D3NOR: return ADD_NOR_GATE_3;
				case D2XNOR: return ADD_XNOR_GATE_2;
				case DCONNECTION: return ADD_CONNECTION;
				case DSWITCH: return ADD_Switch;
				case DLED: return ADD_LED;
				case DSIMULATION: return SIM_MODE;
				case DSAVE: return SAVE;
				case DLOAD: return LOAD;
				case DEXIT: return EXIT;
				default: return DSN_TOOL;	//A click on empty place in desgin toolbar
				}
			}
			else {
				switch (ClickedItemOrder)
				{
				case D2AND:  return ADD_AND_GATE_2_H;
				case D3AND:  return ADD_AND_GATE_3_H;
				case D2OR: return ADD_OR_GATE_2_H;
				case D2XOR: return ADD_XOR_GATE_2_H;
				case D3XOR: return ADD_XOR_GATE_3_H;
				case DBUFFER: return ADD_Buff_H;
				case DNOT: return ADD_INV_H;
				case D2NAND: return ADD_NAND_GATE_2_H;
				case D2NOR: return ADD_NOR_GATE_2_H;
				case D3NOR: return ADD_NOR_GATE_3_H;
				case D2XNOR: return ADD_XNOR_GATE_2_H;
				case DCONNECTION: return ADD_CONNECTION_H;
				case DSWITCH: return ADD_Switch_H;
				case DLED: return ADD_LED_H;
				case DSIMULATION: return SIM_MODE;
				case DSAVE: return SAVE;
				case DLOAD: return LOAD;
				case DEXIT: return EXIT;
				default: return DSN_TOOL;	//A click on empty place in desgin toolbar
				}
			}

		}

		//[2] User clicks on the drawing area //TODO:
		if (y >= UI.ToolBarHeight && y < UI.height - UI.StatusBarHeight)
		{
				//user want to select/unselect a statement i;
			 if (s == RIGHT_CLICK)
				return RIGHT_CLICKSELECT;
			 else 
				 pWind->GetMouseClick(x, y,true); //Remove the last Saved Click
		}

		//[3] User clicks on the status bar
		return STATUS_BAR;
	}
	else	//Application is in Simulation mode
	{
		if (y > 0 && y < UI.ToolBarHeight) {
			int ClickedItemOrder = -1;//TODO:Modify
			for (; x > 0; ClickedItemOrder++) {
				x -= (UI.ToolItemWidth + 5);
			}
			switch (ClickedItemOrder) {
			case SVALIDATE: return Validate;
			case SSIMULATE: return Simulate;
			case STT: return Create_TruthTable;
			case SDESIGN: return DSN_MODE;
			case SSAVE: return SAVE;
			case SLOAD: return LOAD;
			case SEXIT: return EXIT;
			default: return DSN_TOOL; //TODO:
			}
		}
		//return SIM_MODE;	//This should be changed after creating the compelete simulation bar 
		//[2] User clicks on the drawing area //TODO:
		if (y >= UI.ToolBarHeight && y < UI.height - UI.StatusBarHeight)
		{
			return SELECT;	//user want to select/unselect a statement i;
		}

		//[3] User clicks on the status bar
		return STATUS_BAR;
	}
	pWind->FlushMouseQueue();

}

void Input::WaitSelectionPoint(int &X, int &Y)
{
	pWind->WaitMouseClick(X, Y);
}

Input::~Input()
{
}
