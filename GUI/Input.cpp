#include "Input.h"
#include "Output.h"
#include "..\Utils.h"
#include <iostream>
Input::Input(window* pW)
{
	pWind = pW; //point to the passed window
	isSelectMode = false;
	storedImageBeforeShowingAddBar = new image;
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
	int x = 0, y = 0, xT, yT, hoverXOld = 0, hoverYOld = 0, oldTopHoveredItemOrder = -1, oldLeftHoverItem = -1;
	clicktype s = LEFT_CLICK;
	Component* preComp = NULL;
	while (true) {
		bool drawConnection = false;
		if (pWind->GetButtonState(LEFT_BUTTON, xT, yT) == BUTTON_DOWN && yT >= 20 && xT >= UI.LeftToolBarWidth && yT < UI.height - UI.StatusBarHeight) {
			if((yT > UI.ToolBarHeight)||(yT <= UI.ToolBarHeight && !UI.isTopToolBarVisible))
			{
				Component* comp = NULL;
				for (int i = 0; i < pManager->allComponentsCorners.size(); i++)
				{
					if (dynamic_cast<Connection*>(pManager->getComponent(i)))
						continue;
					if (xT >= pManager->allComponentsCorners[i].x1&&xT <= pManager->allComponentsCorners[i].x2&& yT >= pManager->allComponentsCorners[i].y1&&yT <= pManager->allComponentsCorners[i].y2)
					{
						comp = pManager->getComponent(i);
						if (xT >= pManager->allComponentsCorners[i].x2 - UI.GATE_Width / 2 + 11 && yT > (pManager->allComponentsCorners[i].y1 + UI.GATE_Height / 2) - 3 && yT < (pManager->allComponentsCorners[i].y1 + UI.GATE_Height / 2) + 3)
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
							if (selectedComponents[i].second == comp)
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
				break;
			}
			else {
				int hoverX, hoverY;
				pWind->GetMouseCoord(hoverX, hoverY);
				//If top bar and design mode and top bar down
				if (hoverY >= 5 && hoverY < UI.ToolBarHeight - 20 && hoverX > UI.LeftToolBarWidth && UI.isTopToolBarVisible && UI.AppMode == DESIGN && !UI.componentSelected)
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
						case DADD: {if (!UI.isAddEnabled)pWind->DrawImage("images\\Menu\\left_bar_add_hovered.jpg", 0, 0, UI.LeftToolBarWidth, UI.height); break; }
						case DSIMULATION: {if (!UI.isAddEnabled)pWind->DrawImage("images\\Menu\\left_bar_simulate_hovered.jpg", 0, 0, UI.LeftToolBarWidth, UI.height);
										  else pWind->DrawImage("images\\Menu\\left_bar_simulate_hovered_and_enabled.jpg", 0, 0, UI.LeftToolBarWidth, UI.height);  break; }
						case DNEW: {if (!UI.isAddEnabled)pWind->DrawImage("images\\Menu\\left_bar_new_hovered.jpg", 0, 0, UI.LeftToolBarWidth, UI.height);
								   else pWind->DrawImage("images\\Menu\\left_bar_new_hovered_and_enabled.jpg", 0, 0, UI.LeftToolBarWidth, UI.height); break; }
						case DSAVE: {if (!UI.isAddEnabled)pWind->DrawImage("images\\Menu\\left_bar_save_hovered.jpg", 0, 0, UI.LeftToolBarWidth, UI.height); else
							pWind->DrawImage("images\\Menu\\left_bar_save_hovered_and_enabled.jpg", 0, 0, UI.LeftToolBarWidth, UI.height); break; }
						case DLOAD: {if (!UI.isAddEnabled)pWind->DrawImage("images\\Menu\\left_bar_load_hovered.jpg", 0, 0, UI.LeftToolBarWidth, UI.height); else
							pWind->DrawImage("images\\Menu\\left_bar_load_hovered_and_enabled.jpg", 0, 0, UI.LeftToolBarWidth, UI.height); break; }
						case DEXIT: {if (!UI.isAddEnabled)pWind->DrawImage("images\\Menu\\left_bar_exit_hovered.jpg", 0, 0, UI.LeftToolBarWidth, UI.height); else
							pWind->DrawImage("images\\Menu\\left_bar_exit_hovered_and_enabled.jpg", 0, 0, UI.LeftToolBarWidth, UI.height); break; }
						default: {if (!UI.isAddEnabled) pWind->DrawImage("images\\Menu\\left_bar_normal.jpg", 0, 0, UI.LeftToolBarWidth, UI.height);
								 else  pWind->DrawImage("images\\Menu\\left_bar_add_enabled.jpg", 0, 0, UI.LeftToolBarWidth, UI.height);  break;
						}
						}
					}
					oldLeftHoverItem = HoveredLeftItemOrder;
				}
				else {
					oldTopHoveredItemOrder = oldLeftHoverItem = -1;
					if (!UI.componentSelected && UI.isTopToolBarVisible) {

						pWind->DrawImage("images\\Menu\\top_bar_normal.jpg", UI.LeftToolBarWidth, 0, UI.width - UI.LeftToolBarWidth - 14, UI.TopToolBarHeight);
					}if (!UI.isAddEnabled)
					{
						pWind->DrawImage("images\\Menu\\left_bar_normal.jpg", 0, 0, UI.LeftToolBarWidth, UI.height);
					}
					else {
						pWind->DrawImage("images\\Menu\\left_bar_add_enabled.jpg", 0, 0, UI.LeftToolBarWidth, UI.height);
					}

					Utils::correctPointClicked(hoverX, hoverY, true, false);
					if (Utils::CheckPointInBorders(hoverX, hoverY)) {
						Component* comp = pManager->GetOutput()->getArrayOfComponents(hoverY / UI.GRID_SIZE, hoverX / UI.GRID_SIZE);
						if (comp != NULL && comp != preComp)
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
		if (y >= 5 && y < UI.ToolBarHeight - 20 && x > UI.LeftToolBarWidth && UI.isAddEnabled && UI.isTopToolBarVisible)
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
				case D2AND: pWind->DrawImage(storedImageBeforeShowingAddBar, 0, 0, UI.width, UI.height); UI.isTopToolBarVisible = false;  return ADD_AND_GATE_2;
				case D3AND: pWind->DrawImage(storedImageBeforeShowingAddBar, 0, 0, UI.width, UI.height); UI.isTopToolBarVisible = false;  return ADD_AND_GATE_3;
				case D2OR:pWind->DrawImage(storedImageBeforeShowingAddBar, 0, 0, UI.width, UI.height); UI.isTopToolBarVisible = false;  return ADD_OR_GATE_2;
				case D2XOR:pWind->DrawImage(storedImageBeforeShowingAddBar, 0, 0, UI.width, UI.height);  UI.isTopToolBarVisible = false; return ADD_XOR_GATE_2;
				case D3XOR:pWind->DrawImage(storedImageBeforeShowingAddBar, 0, 0, UI.width, UI.height); UI.isTopToolBarVisible = false;  return ADD_XOR_GATE_3;
				case DBUFFER:pWind->DrawImage(storedImageBeforeShowingAddBar, 0, 0, UI.width, UI.height); UI.isTopToolBarVisible = false;  return ADD_Buff;
				case DNOT: pWind->DrawImage(storedImageBeforeShowingAddBar, 0, 0, UI.width, UI.height); UI.isTopToolBarVisible = false;  return ADD_INV;
				case D2NAND: pWind->DrawImage(storedImageBeforeShowingAddBar, 0, 0, UI.width, UI.height); UI.isTopToolBarVisible = false;  return ADD_NAND_GATE_2;
				case D2NOR:pWind->DrawImage(storedImageBeforeShowingAddBar, 0, 0, UI.width, UI.height); UI.isTopToolBarVisible = false;  return ADD_NOR_GATE_2;
				case D3NOR:pWind->DrawImage(storedImageBeforeShowingAddBar, 0, 0, UI.width, UI.height); UI.isTopToolBarVisible = false;  return ADD_NOR_GATE_3;
				case D2XNOR: pWind->DrawImage(storedImageBeforeShowingAddBar, 0, 0, UI.width, UI.height);  UI.isTopToolBarVisible = false; return ADD_XNOR_GATE_2;
				case DSWITCH:pWind->DrawImage(storedImageBeforeShowingAddBar, 0, 0, UI.width, UI.height); UI.isTopToolBarVisible = false;  return ADD_Switch;
				case DLED:pWind->DrawImage(storedImageBeforeShowingAddBar, 0, 0, UI.width, UI.height); return ADD_LED;
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
			case DADD: {
				if (!UI.isAddEnabled)
				{
					UI.isAddEnabled = true;
					pWind->StoreImage(storedImageBeforeShowingAddBar, 0, 0, UI.width, UI.height);
					pWind->DrawImage("images\\Menu\\top_bar_normal.jpg", UI.LeftToolBarWidth, 0, UI.width - UI.LeftToolBarWidth - 14, UI.TopToolBarHeight);
					UI.isTopToolBarVisible = true;
					pWind->DrawImage("images\\Menu\\left_bar_add_enabled.jpg", 0, 0, UI.LeftToolBarWidth, UI.height);
				}
				else {
					pWind->DrawImage(storedImageBeforeShowingAddBar, 0, 0, UI.width, UI.height);
					UI.isAddEnabled = false;
					UI.isTopToolBarVisible = false;
					pWind->DrawImage("images\\Menu\\left_bar_normal.jpg", 0, 0, UI.LeftToolBarWidth, UI.height);
				}
				break;
			}
			case DSIMULATION:return Simulate; break;
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
			if (s == RIGHT_CLICK)
				return RIGHT_CLICKSELECT;
			else
				pWind->GetMouseClick(x, y, true); //Remove the last Saved Click
		}

		//[3] User clicks on the status bar
		return STATUS_BAR;
	}
	else	//Application is in Simulation mode
	{
		//if (y > 0 && y < UI.ToolBarHeight) {
		//	int ClickedItemOrder = -1;//TODO:Modify
		//	for (; x > 0; ClickedItemOrder++) {
		//		x -= (UI.ToolItemWidth + 5);
		//	}
		//	switch (ClickedItemOrder) {
		//	case SVALIDATE: return Validate;
		//	case SSIMULATE: return Simulate;
		//	case STT: return Create_TruthTable;
		//	case SDESIGN: return DSN_MODE;
		//	case SSAVE: return SAVE;
		//	case SLOAD: return LOAD;
		//	case SEXIT: return EXIT;
		//	default: return DSN_TOOL; //TODO:
		//	}
		//}
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
void Input::reShowToolbar() {
	pWind->StoreImage(storedImageBeforeShowingAddBar, 0, 0, UI.width, UI.height);
	pWind->DrawImage("images\\Menu\\top_bar_normal.jpg", UI.LeftToolBarWidth, 0, UI.width - UI.LeftToolBarWidth - 14, UI.TopToolBarHeight);
	UI.isTopToolBarVisible = true;
}

buttonstate Input::GetButtonStatus(const button btMouse, int &iX, int &iY) const {
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

void Input::WaitSelectionPoint(int &X, int &Y)
{
	pWind->WaitMouseClick(X, Y);
}

void Input::setSelectMode(bool b)
{
	isSelectMode = b;
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

Input::~Input()
{
}
