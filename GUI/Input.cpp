#include "Input.h"
#include "Output.h"
#include "..\Utils.h"
#include <iostream>
#include "..\Components\SWITCH.h"
Input::Input(window* pW)
{
	pWind = pW; //point to the passed window
	isSelectMode = isSelectionContainConnections = false;
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
void Input::getMouseCoordinates(int & x, int & y)
{
	pWind->GetMouseCoord(x, y);
	pWind->FlushMouseQueue();
}
//This function reads the position where the user clicks to determine the desired action
ActionType Input::GetUserAction(ApplicationManager *pManager)
{

	//	pWind->GetKeyPress(c2);
		/*if ((int(c1)) == 13 && c2 == 'Z') {
			cout << "UNDO";
		}
		else if ((int(c1)) == 13 && c2 == 'Y') {
			cout << "READO";
		}*/
	int x = 0, y = 0, xT, yT, hoverXOld = 0, hoverYOld = 0, oldTopHoveredItemOrder = -1, oldLeftHoverItem = -1;
	clicktype s = LEFT_CLICK;
	Component* preComp = NULL;
	while (true) {
		// Check for keyboard hotkeys
		char c1;
		keytype key = pWind->GetKeyPress(c1);
		if ((int)c1 == 26) {
			return UNDOACTION;
		}
		else if ((int)c1 == 25) {
			return REDOACTION;
		}
		//cout << c2;
		// Working with mouse

		bool drawConnection = false;
		if (pWind->GetButtonState(LEFT_BUTTON, xT, yT) == BUTTON_DOWN && yT >= UI.ToolBarHeight + 20 && xT >= UI.LeftToolBarWidth && yT < UI.height - UI.StatusBarHeight) {
			if (UI.AppMode == DESIGN)
			{
				Component* comp = NULL;
				for (int i = 0; i < pManager->allComponentsCorners.size(); i++)
				{
					if (dynamic_cast<Connection*>(pManager->getComponent(i)) || pManager->getComponent(i)->getDelete())
						continue;
					if (xT >= pManager->allComponentsCorners[i].x1&&xT <= pManager->allComponentsCorners[i].x2&& yT >= pManager->allComponentsCorners[i].y1&&yT <= pManager->allComponentsCorners[i].y2)
					{
						comp = pManager->getComponent(i);
						if (xT >= pManager->allComponentsCorners[i].x2 - UI.GATE_Width / 2 + 8 && yT > (pManager->allComponentsCorners[i].y1 + UI.GATE_Height / 2) - 6 && yT < (pManager->allComponentsCorners[i].y1 + UI.GATE_Height / 2) + 6)
						{
							pWind->SetPen(UI.ErrorColor, 5);
							pWind->DrawLine(pManager->allComponentsCorners[i].x2 - UI.GATE_Width / 2 + 11, pManager->allComponentsCorners[i].y1 + UI.GATE_Height / 2, pManager->allComponentsCorners[i].x2 - UI.GATE_Width / 2 + 12, pManager->allComponentsCorners[i].y1 + UI.GATE_Height / 2, FRAME);
							startXPointForConnections = xT;
							startYPointForConnections = yT;
							drawConnection = true;
						}
					}
				}
				if (drawConnection)
				{
					return ADD_CONNECTION;
				}
				if (comp != NULL &&comp->getDelete()) comp = NULL;

				if (comp != NULL) {
					if (isSelectMode)
					{
						for (size_t i = 0; i < selectedComponents.size(); i++)
						{
							if (!dynamic_cast<Connection*>(selectedComponents[i].second) && selectedComponents[i].second == comp)
								return MULTI_MOVE;
						}
					}
					return MOVE;
				}
				else {
					bool found = false;
					vector <Connection*> allConnections;
					pManager->getAllConnections(allConnections);

					for (size_t i = 0; i < allConnections.size(); i++)
					{
						allConnections[i]->selectYourSelf(pManager->GetOutput(), UI.ConnColor);
					}
					if (isSelectMode)
					{
						for (size_t i = 0; i < selectedComponents.size(); i++)
						{
							selectedComponents[i].second->Draw(pManager->GetOutput(), false);
						}
						setSelectMode(false);
						isSelectionContainConnections = false;
						selectedComponents.clear();
					}
					for (size_t i = 0; i < allConnections.size() && !found; i++)
					{
						for (size_t j = 0; j < allConnections[i]->getCellsBeforeAddingConnection().size() - 1; j++)
						{
							Cell cell = allConnections[i]->getCellsBeforeAddingConnection()[j];
							Cell cell2 = allConnections[i]->getCellsBeforeAddingConnection()[j + 1];
							if (cell.x > cell2.x)
							{
								if (xT > cell2.x * UI.GRID_SIZE && xT < cell.x * UI.GRID_SIZE && abs(yT - cell.y * UI.GRID_SIZE) <= 3)
								{
									allConnections[i]->selectYourSelf(pManager->GetOutput(), UI.SelectColor);
									found = true;
									break;
								}
							}
							else if (cell.x < cell2.x) {
								if (xT < cell2.x * UI.GRID_SIZE && xT > cell.x * UI.GRID_SIZE && abs(yT - cell.y * UI.GRID_SIZE) <= 3)
								{
									allConnections[i]->selectYourSelf(pManager->GetOutput(), UI.SelectColor);
									found = true;
									break;
								}
							}
							else if (cell.y > cell2.y)
							{
								if (yT > cell2.y * UI.GRID_SIZE && yT < cell.y * UI.GRID_SIZE && abs(xT - cell.x * UI.GRID_SIZE) <= 3)
								{
									allConnections[i]->selectYourSelf(pManager->GetOutput(), UI.SelectColor);
									found = true;
									break;
								}
							}
							else if (cell.y < cell2.y) {
								if (yT < cell2.y * UI.GRID_SIZE && yT > cell.y * UI.GRID_SIZE && abs(xT - cell.x * UI.GRID_SIZE) <= 3)
								{
									allConnections[i]->selectYourSelf(pManager->GetOutput(), UI.SelectColor);
									found = true;
									break;
								}
							}
						}
					}

					if (!found) {
						return MULTI_SELECT;
					}
				}
			}
		}
		else {
			//Get the coordinates of the user click 
			// We Called it with false argument inorder not to delete the click to be used in RightSelect
			//Otherwise we call that function agian with true to delete that click 
			if ((s = pWind->GetMouseClick(x, y, false)) != NO_CLICK)
			{
				//pWind->GetMouseClick(x, y);
				break;
			}
			else {
				int hoverX, hoverY;
				pWind->GetMouseCoord(hoverX, hoverY);
				//If top bar and design mode and top bar down
				if (hoverY >= 5 && hoverY < UI.ToolBarHeight - 20 && hoverX > UI.LeftToolBarWidth && UI.AppMode == DESIGN)
				{
					vector<pair<int, int> > TopItemsRanges;
					TopItemsRanges.push_back(make_pair(107, 170));
					TopItemsRanges.push_back(make_pair(190, 245));
					TopItemsRanges.push_back(make_pair(275, 332));
					TopItemsRanges.push_back(make_pair(360, 415));
					TopItemsRanges.push_back(make_pair(443, 496));
					TopItemsRanges.push_back(make_pair(527, 592));
					TopItemsRanges.push_back(make_pair(612, 662));
					TopItemsRanges.push_back(make_pair(697, 753));
					TopItemsRanges.push_back(make_pair(782, 836));
					TopItemsRanges.push_back(make_pair(867, 922));
					TopItemsRanges.push_back(make_pair(953, 1010));
					TopItemsRanges.push_back(make_pair(1035, 1091));
					TopItemsRanges.push_back(make_pair(1121, 1166));

					int HoveredItemOrder = -1;//TODO:Modify

					for (size_t i = 0; i < TopItemsRanges.size(); i++)
					{
						if (hoverX >= TopItemsRanges[i].first - 5 && hoverX <= TopItemsRanges[i].second + 3) {
							HoveredItemOrder = i;
						}
					}
					if (HoveredItemOrder != oldTopHoveredItemOrder)
					{

						switch (HoveredItemOrder)
						{
						case D2AND:  pWind->DrawImage("images\\Menu\\top_bar_and2_hovered.jpg", UI.LeftToolBarWidth, 0, UI.width - UI.LeftToolBarWidth - 14, UI.TopToolBarHeight); break;
						case D3AND:  pWind->DrawImage("images\\Menu\\top_bar_and3_hovered.jpg", UI.LeftToolBarWidth, 0, UI.width - UI.LeftToolBarWidth - 14, UI.TopToolBarHeight); break;
						case D2OR: pWind->DrawImage("images\\Menu\\top_bar_or2_hovered.jpg", UI.LeftToolBarWidth, 0, UI.width - UI.LeftToolBarWidth - 14, UI.TopToolBarHeight); break;
						case D2XOR: pWind->DrawImage("images\\Menu\\top_bar_xor2_hovered.jpg", UI.LeftToolBarWidth, 0, UI.width - UI.LeftToolBarWidth - 14, UI.TopToolBarHeight); break;
						case D3XOR: pWind->DrawImage("images\\Menu\\top_bar_xor3_hovered.jpg", UI.LeftToolBarWidth, 0, UI.width - UI.LeftToolBarWidth - 14, UI.TopToolBarHeight); break;
						case DBUFFER:  pWind->DrawImage("images\\Menu\\top_bar_buffer_hovered.jpg", UI.LeftToolBarWidth, 0, UI.width - UI.LeftToolBarWidth - 14, UI.TopToolBarHeight); break;
						case DNOT: pWind->DrawImage("images\\Menu\\top_bar_not_hovered.jpg", UI.LeftToolBarWidth, 0, UI.width - UI.LeftToolBarWidth - 14, UI.TopToolBarHeight); break;
						case D2NAND:  pWind->DrawImage("images\\Menu\\top_bar_nand2_hovered.jpg", UI.LeftToolBarWidth, 0, UI.width - UI.LeftToolBarWidth - 14, UI.TopToolBarHeight); break;
						case D2NOR:  pWind->DrawImage("images\\Menu\\top_bar_nor2_hovered.jpg", UI.LeftToolBarWidth, 0, UI.width - UI.LeftToolBarWidth - 14, UI.TopToolBarHeight); break;
						case D3NOR: pWind->DrawImage("images\\Menu\\top_bar_nor3_hovered.jpg", UI.LeftToolBarWidth, 0, UI.width - UI.LeftToolBarWidth - 14, UI.TopToolBarHeight); break;
						case D2XNOR: pWind->DrawImage("images\\Menu\\top_bar_xnor2_hovered.jpg", UI.LeftToolBarWidth, 0, UI.width - UI.LeftToolBarWidth - 14, UI.TopToolBarHeight); break;
						case DSWITCH: pWind->DrawImage("images\\Menu\\top_bar_switch_hovered.jpg", UI.LeftToolBarWidth, 0, UI.width - UI.LeftToolBarWidth - 14, UI.TopToolBarHeight); break;
						case DLED: pWind->DrawImage("images\\Menu\\top_bar_led_hovered.jpg", UI.LeftToolBarWidth, 0, UI.width - UI.LeftToolBarWidth - 14, UI.TopToolBarHeight); break;
						default:  pWind->DrawImage("images\\Menu\\top_bar_normal.jpg", UI.LeftToolBarWidth, 0, UI.width - UI.LeftToolBarWidth - 14, UI.TopToolBarHeight); break;
						}
					}
					oldTopHoveredItemOrder = HoveredItemOrder;
				}
				else if (hoverY >= 0 && hoverY < UI.height && UI.AppMode == DESIGN && hoverX <= UI.LeftToolBarWidth - 12 && hoverX >= 5) {
					vector<pair<int, int> > LeftItemsRanges;
					LeftItemsRanges.push_back(make_pair(6, 85));
					LeftItemsRanges.push_back(make_pair(103, 180));
					LeftItemsRanges.push_back(make_pair(197, 273));
					LeftItemsRanges.push_back(make_pair(286, 366));
					LeftItemsRanges.push_back(make_pair(382, 460));
					LeftItemsRanges.push_back(make_pair(475, 550));

					int HoveredLeftItemOrder = -1;//TODO:Modify

					for (size_t i = 0; i < LeftItemsRanges.size(); i++)
					{
						if (hoverY >= LeftItemsRanges[i].first && hoverY <= LeftItemsRanges[i].second) {
							HoveredLeftItemOrder = i;
						}
					}
					if (HoveredLeftItemOrder != oldLeftHoverItem)
					{
						switch (HoveredLeftItemOrder + 14)
						{
						case DSIMULATION: pWind->DrawImage("images\\Menu\\left_bar_simulate_hovered.jpg", 0, 0, UI.LeftToolBarWidth, UI.height); break;
						case DNEW:pWind->DrawImage("images\\Menu\\left_bar_new_hovered.jpg", 0, 0, UI.LeftToolBarWidth, UI.height); break;
						case DSAVE: pWind->DrawImage("images\\Menu\\left_bar_save_hovered.jpg", 0, 0, UI.LeftToolBarWidth, UI.height); break;
						case DLOAD: pWind->DrawImage("images\\Menu\\left_bar_load_hovered.jpg", 0, 0, UI.LeftToolBarWidth, UI.height); break;
						case DEXIT:pWind->DrawImage("images\\Menu\\left_bar_exit_hovered.jpg", 0, 0, UI.LeftToolBarWidth, UI.height); break;
						default: pWind->DrawImage("images\\Menu\\left_bar_normal.jpg", 0, 0, UI.LeftToolBarWidth, UI.height); break;
						}
					}
					oldLeftHoverItem = HoveredLeftItemOrder;
				}
				else if (hoverY >= 0 && hoverY < UI.height && UI.AppMode == SIMULATION && hoverX <= UI.LeftToolBarWidth - 12 && hoverX >= 5) {
					vector<pair<int, int> > LeftItemsRanges;
					LeftItemsRanges.push_back(make_pair(6, 85));
					LeftItemsRanges.push_back(make_pair(103, 180));
					LeftItemsRanges.push_back(make_pair(197, 273));

					int HoveredLeftItemOrder = -1;//TODO:Modify

					for (size_t i = 0; i < LeftItemsRanges.size(); i++)
					{
						if (hoverY >= LeftItemsRanges[i].first && hoverY <= LeftItemsRanges[i].second) {
							HoveredLeftItemOrder = i;
						}
					}
					if (HoveredLeftItemOrder != oldLeftHoverItem)
					{
						switch (HoveredLeftItemOrder)
						{
						case SDESIGN: pWind->DrawImage("images\\Menu\\left_bar_simulation_design_hovered.jpg", 0, 0, UI.LeftToolBarWidth, UI.height); break;
						case STT:pWind->DrawImage("images\\Menu\\left_bar_simulation_tt_hovered.jpg", 0, 0, UI.LeftToolBarWidth, UI.height); break;
						case SEXIT: pWind->DrawImage("images\\Menu\\left_bar_simulation_exit_hovered.jpg", 0, 0, UI.LeftToolBarWidth, UI.height); break;
						default: pWind->DrawImage("images\\Menu\\left_bar_simulation_normal.jpg", 0, 0, UI.LeftToolBarWidth, UI.height); break;
						}
					}
					oldLeftHoverItem = HoveredLeftItemOrder;
				}
				else {
					oldTopHoveredItemOrder = oldLeftHoverItem = -1;
					if (UI.AppMode == DESIGN)
					{
						pWind->DrawImage("images\\Menu\\top_bar_normal.jpg", UI.LeftToolBarWidth, 0, UI.width - UI.LeftToolBarWidth - 14, UI.TopToolBarHeight);
						pWind->DrawImage("images\\Menu\\left_bar_normal.jpg", 0, 0, UI.LeftToolBarWidth, UI.height);
					}
					else {
						pWind->DrawImage("images\\Menu\\left_bar_simulation_normal.jpg", 0, 0, UI.LeftToolBarWidth, UI.height);
					}

					Utils::correctPointClicked(hoverX, hoverY, true, false);
					if (Utils::CheckPointInBorders(hoverX, hoverY)) {
						Component* comp = pManager->GetOutput()->getArrayOfComponents(hoverY / UI.GRID_SIZE, hoverX / UI.GRID_SIZE);
						if (comp != NULL && !comp->getDelete() && comp != preComp)
						{
							pManager->GetOutput()->PrintMsg(comp->getLabel());
						}
						else if (comp == NULL) {
							pManager->GetOutput()->PrintMsg("");
						}

						preComp = comp;
					}
				}
				hoverYOld = hoverY;
				hoverXOld = hoverX;
			}
		}
	}

	if (UI.AppMode == DESIGN)	//application is in design mode
	{
		//[1] If user clicks on the Toolbar
		if (y >= 5 && y < UI.ToolBarHeight - 20 && x > UI.LeftToolBarWidth)
		{
			pWind->GetMouseClick(x, y);
			//TODO
			//Check whick Menu item was clicked
			//==> This assumes that menu items are lined up horizontally <==

			int ClickedItemOrder = -1;//TODO:Modify
			vector<pair<int, int> > TopItemsRanges;
			TopItemsRanges.push_back(make_pair(107, 170));
			TopItemsRanges.push_back(make_pair(190, 245));
			TopItemsRanges.push_back(make_pair(275, 332));
			TopItemsRanges.push_back(make_pair(360, 415));
			TopItemsRanges.push_back(make_pair(443, 496));
			TopItemsRanges.push_back(make_pair(527, 592));
			TopItemsRanges.push_back(make_pair(612, 662));
			TopItemsRanges.push_back(make_pair(697, 753));
			TopItemsRanges.push_back(make_pair(782, 836));
			TopItemsRanges.push_back(make_pair(867, 922));
			TopItemsRanges.push_back(make_pair(953, 1010));
			TopItemsRanges.push_back(make_pair(1035, 1091));
			TopItemsRanges.push_back(make_pair(1121, 1166));

			for (size_t i = 0; i < TopItemsRanges.size(); i++)
			{
				if (x >= TopItemsRanges[i].first - 5 && x <= TopItemsRanges[i].second + 3) {
					ClickedItemOrder = i;
				}
			}

			//Divide x coord of the point clicked by the menu item width (int division)
			//if division result is 0 ==> first item is clicked, if 1 ==> 2nd item and so on
			pWind->FlushMouseQueue();
			if (s == LEFT_CLICK)
			{
				switch (ClickedItemOrder)
				{
				case D2AND:  return ADD_AND_GATE_2;
				case D3AND:  return ADD_AND_GATE_3;
				case D2OR:  return ADD_OR_GATE_2;
				case D2XOR: return ADD_XOR_GATE_2;
				case D3XOR:  return ADD_XOR_GATE_3;
				case DBUFFER:  return ADD_Buff;
				case DNOT:  return ADD_INV;
				case D2NAND:  return ADD_NAND_GATE_2;
				case D2NOR:  return ADD_NOR_GATE_2;
				case D3NOR:  return ADD_NOR_GATE_3;
				case D2XNOR:  return ADD_XNOR_GATE_2;
				case DSWITCH:  return ADD_Switch;
				case DLED: return ADD_LED;
				default: return DSN_TOOL;	//A click on empty place in desgin toolbar
				}
			}

		}
		else if (y >= 0 && y < UI.height && UI.AppMode == DESIGN && x <= UI.LeftToolBarWidth - 12 && x >= 5) {
			pWind->GetMouseClick(x, y);
			vector<pair<int, int> > LeftItemsRanges;
			LeftItemsRanges.push_back(make_pair(6, 85));
			LeftItemsRanges.push_back(make_pair(103, 180));
			LeftItemsRanges.push_back(make_pair(197, 273));
			LeftItemsRanges.push_back(make_pair(286, 366));
			LeftItemsRanges.push_back(make_pair(382, 460));
			LeftItemsRanges.push_back(make_pair(475, 550));

			int ClickedItemOrder = -1;//TODO:Modify

			for (size_t i = 0; i < LeftItemsRanges.size(); i++)
			{
				if (y >= LeftItemsRanges[i].first && y <= LeftItemsRanges[i].second) {
					ClickedItemOrder = i;
				}
			}
			switch (ClickedItemOrder + 14)
			{
			case DSIMULATION: {
				bool returnValidateion = false;

				for (size_t i = 0; i < pManager->allComponentsCorners.size(); i++)
				{
					Component* comp = pManager->getComponent(i);
					if (!comp->getDelete())
					{
						returnValidateion = true;
					}
					else {
						pManager->GetOutput()->PrintMsg("Please add components first!", UI.ErrorColor);
					}
				}
				if (returnValidateion) { return ValidateAction; };
				break; }
			case DNEW:return NEW; break;
			case DSAVE:return SAVE; break;
			case DLOAD:return LOAD; break;
			case DEXIT:return EXIT; break;
			default:  pWind->DrawImage("images\\Menu\\left_bar_normal.jpg", 0, 0, UI.LeftToolBarWidth, UI.height); break;
			}
		}
		//[2] User clicks on the drawing area //TODO:
		if (y > UI.ToolBarHeight && y < UI.height - UI.StatusBarHeight)
		{
			//user want to select/unselect a statement i;
			if (s == RIGHT_CLICK) {
				return RIGHT_CLICKSELECT;
			}
			else
				pWind->GetMouseClick(x, y, true); //Remove the last Saved Click
		}

		//[3] User clicks on the status bar
		return STATUS_BAR;
	}
	else	//Application is in Simulation mode
	{
		pWind->GetMouseClick(x, y);

		if (y >= 0 && y < UI.height && x <= UI.LeftToolBarWidth - 12 && x >= 5) {
			vector<pair<int, int> > LeftItemsRanges;
			LeftItemsRanges.push_back(make_pair(6, 85));
			LeftItemsRanges.push_back(make_pair(103, 180));
			LeftItemsRanges.push_back(make_pair(197, 273));

			int ClickedItemOrder = -1;//TODO:Modify

			for (size_t i = 0; i < LeftItemsRanges.size(); i++)
			{
				if (y >= LeftItemsRanges[i].first && y <= LeftItemsRanges[i].second) {
					ClickedItemOrder = i;
				}
			}
			switch (ClickedItemOrder) {
			case SDESIGN: {switchMode(DESIGN); break; }
			case STT: return Create_TruthTable;
			case SEXIT: return EXIT;
			default: return DSN_TOOL; //TODO:
			}
		}
		else if (y >= UI.TopToolBarHeight && y < UI.height && x >= UI.LeftToolBarWidth && x < UI.width) {

			for (int i = 0; i < pManager->allComponentsCorners.size(); i++)
			{
				if (!dynamic_cast<SWITCH*>(pManager->getComponent(i)) || pManager->getComponent(i)->getDelete())
					continue;
				if (x >= pManager->allComponentsCorners[i].x1&&x <= pManager->allComponentsCorners[i].x2&& y >= pManager->allComponentsCorners[i].y1&&y <= pManager->allComponentsCorners[i].y2)
				{
					((SWITCH*)pManager->getComponent(i))->setOutputPinStatus(static_cast<STATUS>(!((SWITCH*)pManager->getComponent(i))->GetOutPinStatus()));
					((SWITCH*)pManager->getComponent(i))->Draw(pManager->GetOutput(), false);
					pWind->FlushMouseQueue();
					return SimulateAction;
				}
			}
		}
		return SELECT_SIM;
		//return SIM_MODE;	//This should be changed after creating the compelete simulation bar 
		//[2] User clicks on the drawing area //TODO:
		//if (y >= UI.ToolBarHeight && y < UI.height - UI.StatusBarHeight)
		//{
		//	return SELECT;	//user want to select/unselect a statement i;
		//}

		//[3] User clicks on the status bar
		//return STATUS_BAR;
	}
	pWind->FlushMouseQueue();

}
void Input::switchMode(MODE appMode) {
	if (appMode == DESIGN)
	{
		UI.AppMode = DESIGN;
		pWind->DrawImage("images\\Menu\\top_bar_normal.jpg", UI.LeftToolBarWidth, 0, UI.width - UI.LeftToolBarWidth - 14, UI.TopToolBarHeight);
		pWind->DrawImage("images\\Menu\\left_bar_normal.jpg", 0, 0, UI.LeftToolBarWidth, UI.height);
	}
	else {
		pWind->DrawImage("images\\Menu\\top_bar_simulate.jpg", UI.LeftToolBarWidth, 0, UI.width - UI.LeftToolBarWidth - 14, UI.TopToolBarHeight);
		pWind->DrawImage("images\\Menu\\left_bar_simulation_normal.jpg", 0, 0, UI.LeftToolBarWidth, UI.height);
		UI.AppMode = SIMULATION;
	}
}
buttonstate Input::GetButtonStatus(const button btMouse, int &iX, int &iY) const {
	return pWind->GetButtonState(btMouse, iX, iY);
}
string Input::GetSrting(Output *pOut, string sOriginal = "", bool EditingLabel)
{
	string s = "";
	char ch;
	keytype k;
	if (EditingLabel) pOut->PrintMsg(sOriginal);
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

void Input::WaitSelectionPoint(int &X, int &Y)
{
	pWind->WaitMouseClick(X, Y);
}

void Input::setisSelectionContainConnections(bool b)
{
	isSelectionContainConnections = b;
}

void Input::setSelectMode(bool b)
{
	isSelectMode = b;
}
bool Input::getisSelectionContainConnections()
{
	return isSelectionContainConnections;
}
bool Input::getSelectMode()
{
	return isSelectMode;
}

void Input::setSelectedComponents(vector<pair<int, Component*> > comp)
{
	selectedComponents = comp;
}

vector<pair<int, Component*> >& Input::getSelectedComponents()
{
	return selectedComponents;
}

void Input::CorrectPointClickedSilent(int& x, int& y, bool drawImage, bool drawConnection)
{
	Utils::correctPointClicked(x, y, drawImage, drawConnection);
}

string Input::EditComponenetLabel(Output *pOut)
{
	//To be Modified Later with Menus 
	pWind->FlushKeyQueue();
	string msg = "Type the new name";
	string ret = this->GetSrting(pOut, msg, true);
	return ret;
}

void Input::getExactConnectionLocation(int & x, int & y)
{
	Utils::correctPointClicked(x, y, true, false);
}


Input::~Input()
{
}
